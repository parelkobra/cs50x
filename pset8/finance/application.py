import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

from sys import stderr

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Get the logged user from users table to get the available cash
    user = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])

    # Get the portfolio of the user
    portfolio = db.execute("""SELECT p.symbol, p.name, p.shares
                            FROM portfolio p JOIN users u ON p.user_id = u.id
                            WHERE u.id = ?;""", session["user_id"])

    # Add to the list of stocks the updated price and the current total
    for p in portfolio:
        quote = lookup(p["symbol"])
        p["price"] = quote["price"]
        p["total"] = format(float(p["shares"]) * float(p["price"]), ".2f")

    # Range of months and years for the add cash modal
    months = [m for m in range(1, 13)]
    years = [y for y in range(20, 60)]

    return render_template("index.html", stocks=portfolio, cash=usd(user[0]["cash"]),
            months=months, years=years)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":

        quote = lookup(request.form.get("symbol"))
        shares = int(request.form.get("shares"))
        symbol = str(request.form.get("symbol")).upper()

        # Ensure the symbol exists
        if quote is None:
            return apology("invalid symbol")

        # Ensure the number of shares is a number and is bigger than 0
        if not str(shares).isnumeric() or shares <= 0:
            return apology("invalid number of shares")

        # Get current cash from the logged user
        user = db.execute("""
                SELECT cash FROM users
                WHERE id = ?
                """, session["user_id"])

        # Ensure the user can afford the purchase
        if quote["price"] * float(shares) > int(user[0]["cash"]):
            return apology("not affordable")

        # Add the purchase to the history table
        db.execute("""
                INSERT INTO history (symbol, shares, price, user_id)
                VALUES (?, ?, ?, ?)
                """, symbol, shares, quote["price"], session["user_id"])

        cur_symbol = db.execute("""
                SELECT id FROM portfolio
                WHERE symbol = ? AND user_id = ?
                """, symbol, session["user_id"])

        # Check whether the user has already purchased a stock of a certain symbol or not
        # If it is, increase the shares field, and if it isn't, insert it
        if len(cur_symbol) != 0:
            db.execute("""
                    UPDATE portfolio
                    SET shares = shares + ?
                    WHERE id = ?
                    """, shares, cur_symbol[0]["id"])
        else:
            db.execute("""
                    INSERT INTO portfolio (symbol, name, shares, user_id)
                    VALUES (?, ?, ?, ?)
                    """, symbol, quote["name"], shares, session["user_id"])

        # Subtract the price from the users cash
        db.execute("""
                UPDATE users
                SET cash = cash - ?
                WHERE id = ?
                """, shares * quote["price"], session["user_id"])

        return index()

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    transactions = db.execute("""
            SELECT symbol, shares, price, transacted
            FROM history ORDER BY transacted
            """)

    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "POST":

        quote = lookup(request.form.get("symbol"))

        if quote is None:
            return apology("invalid symbol")

        output = "A share of {0} ({1}) costs {2}.".format(quote["name"],
                quote["symbol"], usd(quote["price"]))

        return render_template("quoted.html", quote=output)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":

        username = request.form.get("username")
        password = request.form.get("password")

        # Ensure the username field is not empty
        if username == "":
            return apology("Invalid username");

        # Ensure the password is not empty and the user has correctly typed it
        if password == "" or password != request.form.get("confirm"):
            return apology("Invalid password");

        # Ensure the username isn't taken
        rows = db.execute("""
                SELECT * FROM users WHERE username = ?
                """, username)

        if len(rows) != 0:
            return apology("This username is already taken");

        # Insert the new user into the database
        db.execute("""
                INSERT INTO users (username, hash)
                VALUES (?, ?)
                """, username, generate_password_hash(password))

        userid = db.execute("""
                SELECT id FROM users
                WHERE username = :username
                """, username=request.form.get("username"))

        # Remember which user has logged in
        session["user_id"] = userid[0]["id"]

        # Create history and portfolio tables
        db.execute("""
        CREATE TABLE IF NOT EXISTS history ( 
            id INTEGER PRIMARY KEY AUTOINCREMENT not NULL,
            symbol TEXT NOT NULL,
            shares INTEGER NOT NULL,
            price NUMERIC NOT NULL,
            transacted DATETIME DEFAULT CURRENT_TIMESTAMP,
            user_id INTEGER NOT NULL,
            FOREIGN KEY(user_id) REFERENCES users(id)
        )""")

        db.execute("""
        CREATE TABLE IF NOT EXISTS portfolio (
            id INTEGER PRIMARY KEY AUTOINCREMENT not NULL,
            symbol TEXT NOT NULL,
            name TEXT,
            shares INTEGER NOT NULL,
            user_id INTEGER NOT NULL,
            FOREIGN KEY(user_id) REFERENCES users(id)
        )""")

        # Create indexes in case they don't exist
        db.execute("CREATE UNIQUE INDEX IF NOT EXISTS symbol ON portfolio (symbol)")
        db.execute("CREATE INDEX IF NOT EXISTS userid ON portfolio (user_id)")
        db.execute("CREATE INDEX IF NOT EXISTS transacted ON history (transacted)")

        # Redirect user to home page
        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":

        quote = lookup(request.form.get("symbol"))
        symbol = str(request.form.get("symbol"))
        shares = int(request.form.get("shares"))

        # Ensure the symbol submitted exists
        if request.form.get("symbol") is None:
            return apology("invalid symbol")

        # Ensure the number of shares if valid
        if not str(shares).isnumeric() or shares <= 0:
            return apology("invalid number of shares")

        stock = db.execute("""
                SELECT * FROM portfolio
                WHERE symbol = ? AND user_id = ?
                """, symbol, session["user_id"])
        
        # The user could sell its shares and would still have shares left
        if shares < stock[0]["shares"]:
            db.execute("""
                    UPDATE portfolio SET shares = shares - ?
                    WHERE symbol = ? AND user_id = ?
                    """, shares, symbol, session["user_id"])

        # The user could sell its shares and wouldn't have shares left
        elif shares == stock[0]["shares"]:
            db.execute("""
                    DELETE FROM portfolio 
                    WHERE symbol = ? AND user_id = ?
                    """, symbol, session["user_id"])

        # The user try to sell more shares than he has of the certain symbol
        else:
            return apology("too many shares")

        # Insert the transaction to the history table
        db.execute("""
                INSERT INTO history (symbol, shares, price, user_id)
                VALUES (?, ?, ?, ?)
                """, symbol.upper(), shares * -1, quote["price"],
                session["user_id"])

        # Subtract cash from the users table
        db.execute("""
                UPDATE users SET cash = cash + ?
                WHERE id = ?
                """, shares * quote["price"], session["user_id"])

        return index()

    else:
        symbols = db.execute("""
                SELECT DISTINCT symbol FROM portfolio
                WHERE user_id = ? ORDER BY symbol
                """, session["user_id"])

        return render_template("sell.html", symbols=symbols)


@app.route("/cash", methods=["GET", "POST"])
@login_required
def cash():
    """User add more cash to its account"""
    if request.method == "POST":
        creditcard = str(request.form.get("creditcard"))
        cash = int(request.form.get("cash"))

        if not creditcard.isnumeric() or len(creditcard) != 16:
            return apology("invalid credit card")

        # Ensure the form field 'cash' is not alphabetic or below 0
        if not str(cash).isnumeric() or cash <= 0:
            return apology("invalid cash")

        # Add cash to user account
        db.execute("""
                UPDATE users SET cash = cash + ?
                WHERE id = ?
                """, cash, session["user_id"])

        return index()

    else:
        return apology("page not found", 404)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
