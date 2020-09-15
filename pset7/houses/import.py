#!/usr/bin/python

import csv
import sqlite3
import sys

if len(sys.argv) != 2:
    print(f"Usage: {sys.argv[0]} CSV_FILE")
    exit(1)

connection = sqlite3.connect('students.db')
cursor = connection.cursor();

insert_query = "INSERT INTO students (first, middle, last, house, birth) VALUES (?, ?, ?, ?, ?)"

try:
    with open(sys.argv[1]) as f:
        reader = csv.DictReader(f)

        for row in reader:
            names = row["name"].split()

            first_name = names[0]
            if len(names) == 2:
                middle_name = None
                last_name = names[1]
            else:
                middle_name = names[1]
                last_name = names[2]
            house = row["house"]
            birth = row["birth"]

            fields = [first_name, middle_name, last_name, house, birth]

            cursor.execute(insert_query, fields)

        connection.commit();
        connection.close();

except FileNotFoundError as e:
    print(e.strerror)
    exit(3)
