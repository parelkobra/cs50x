#!/usr/bin/python

import cs50
import sys

houses = ("Gryffindor", "Hufflepuff", "Ravenclaw", "Slytherin")

select_query = "SELECT first, middle, last, birth FROM students WHERE house = ? ORDER BY last, first ASC"

if len(sys.argv) != 2:
    print(f"Usage: {sys.argv[0]} HOUSE")
    exit(1)

if sys.argv[1] not in houses:
    print("That house doesn't exist...")
    exit(2)

house = sys.argv[1]

con = cs50.SQL("sqlite:///students.db")

students = con.execute(select_query, house)

for student in students:
    if student["middle"] is not None:
        print(f"{student['first']} {student['middle']} {student['last']}, born {student['birth']}")
    else:
        print(f"{student['first']} {student['last']}, born {student['birth']}")
