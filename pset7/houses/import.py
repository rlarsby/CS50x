from cs50 import SQL
import cs50
import csv
import sys

# create database
open("students.db", "w").close()
db = cs50.SQL("sqlite:///students.db")

# create tables
db.execute("CREATE TABLE students (id INT, first TEXT, middle TEXT, last TEXT, house TEXT, birth NUMERIC, PRIMARY KEY(id))")

# check command-line arguments
if len(sys.argv) != 2:
    print("Usage: python import.py characters.csv")
    sys.exit(1)

# use increment-function to track individual id starting with 100
# each time number() is called it is incremented by 1
def incrementor():
    info = {"count": 99}
    def number():
        info["count"] += 1
        return info["count"]
    return number
number = incrementor()

# open CSV file given by command-line argument
with open (sys.argv[1]) as characters:

    charact = csv.reader(characters)
    next(charact) #skip first row straight to data

    for row in charact:
        name = row[0]
        name_sep = name.split()

        #if no middle name set it to = 0
        if len(name_sep) < 3:
            name_sep.insert(1, None)
            db.execute("INSERT INTO students (id, first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?, ?)", number(), name_sep[0], name_sep[1], name_sep[2], row[1], row[2])

        else:
            db.execute("INSERT INTO students (id, first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?, ?)", number(), name_sep[0], name_sep[1], name_sep[2], row[1], row[2])
