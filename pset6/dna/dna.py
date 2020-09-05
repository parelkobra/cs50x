#!/bin/python

import sys
import csv

def main():
    if invalid_args():
        sys.exit(1)

    db = load_file(sys.argv[1])
    sequence = load_file(sys.argv[2])

    repeats = get_repeats(db, sequence)
    user = find_user(db, repeats)
    
    if user is None:
        print("No match")
    else:
        print(user)

def find_user(db, repeats):
    for row in db:
        user = row.pop("name")
        row_values = list(row.values())
        row_values = [ int(val) for val in row_values ]
        if row_values == repeats:
            return user

    return None

def get_repeats(db, seq):
    repeats = list()

    keys = iter(db[0].keys())
    next(keys)

    for STR in keys:
        i, max_repeat, cur_repeat = 0, 0, 0
        while i < len(seq):
            cur_repeat = 0
            if seq.endswith(STR, i, len(STR) + i):
                cur_repeat = 1
                i += len(STR)
                while seq.endswith(STR, i, len(STR) + i):
                    cur_repeat += 1
                    i += len(STR)
                if max_repeat < cur_repeat:
                    max_repeat = cur_repeat
            else:
                i += 1
        repeats.append(max_repeat)

    return repeats

def load_file(f):
    try:
        with open(f) as f_stream:
            if f.endswith("csv"):
                content = list()
                reader = csv.DictReader(f_stream)
                for row in reader:
                    content.append(row)
            elif f.endswith("txt"):
                content = str()
                content = f_stream.read()
    except FileNotFoundError as e:
        print(e.strerror)
        sys.exit(1)

    return content

def invalid_args():
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} DATABASE SEQUENCE")
        return True

    if not sys.argv[1].endswith(".csv"):
        print("Incorrect file type")
        return True

    if not sys.argv[2].endswith(".txt"):
        print("Incorrect file type")
        return True

    return False

main()
