#!/usr/bin/python

import sqlite3
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

connection = sqlite3.connect('students.db')
cursor = connection.cursor();

cursor.execute(select_query, (house,))
students = cursor.fetchall();

for student in students:
    if student[1] is not None:
        print(f"{student[0]} {student[1]} {student[2]}, born {student[3]}")
    else:
        print(f"{student[0]} {student[2]}, born {student[3]}")

connection.close();
