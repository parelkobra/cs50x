-- CREATE SAMPLES (user's one is given)
CREATE TABLE IF NOT EXISTS users (
  id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
  username TEXT NOT NULL,
  hash TEXT NOT NULL,
  cash NUMERIC NOT NULL DEFAULT 10000.00
);

CREATE TABLE IF NOT EXISTS history (
  id INTEGER PRIMARY KEY AUTOINCREMENT not NULL,
  symbol TEXT NOT NULL,
  shares INTEGER NOT NULL,
  price NUMERIC NOT NULL,
  transacted DATETIME DEFAULT CURRENT_TIMESTAMP,
  user_id INTEGER NOT NULL,
  FOREIGN KEY(user_id) REFERENCES users(id)
);

CREATE TABLE IF NOT EXISTS portfolio (
  id INTEGER PRIMARY KEY AUTOINCREMENT not NULL,
  symbol TEXT NOT NULL,
  name TEXT,
  shares INTEGER NOT NULL,
  price NUMERIC NOT NULL,
  user_id INTEGER NOT NULL,
  FOREIGN KEY(user_id) REFERENCES users(id)
);

-- SELECT SAMPLES
SELECT *, printf("%.2f", shares * price) AS TOTAL FROM portfolio

SELECT history.symbol, history.shares, history.price, history.transacted, users.username
FROM history JOIN users
ON history.user_id = users.id
WHERE users.username = 'david';


-- INSERT SAMPLES
INSERT INTO history (symbol, shares, price, user_id) VALUES ('aa', -2, 71.05, 1);

INSERT INTO users (username, hash) VALUES ('david', '123');
