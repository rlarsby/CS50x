from cs50 import SQL
import cs50
import csv
import sys
import sqlite3

# check command-line arguments
if len(sys.argv) != 2:
    print("Usage: python roster.py 'house'")
    sys.exit(1)

#set the command-line argument for which house
house = sys.argv[1]

#connect to database
connection = sqlite3.connect('students.db')
cursor = connection.cursor()

#SQL code
sqlite_select_query = f"""SELECT * FROM students WHERE house = '{house}' ORDER BY last, first"""

#get results
cursor.execute(sqlite_select_query)
result = cursor.fetchall()

#print
for row in result:
    if row[2] == None:
        print(row[1], end=' ')
        print(row[3], end=', ')
        print(f"born {row[5]}")
    else:
        print(row[1], end=' ')
        print(row[2], end=' ')
        print(row[3], end=', ')
        print(f"born {row[5]}")
