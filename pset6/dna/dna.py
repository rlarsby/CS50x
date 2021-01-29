import csv
import sys

#check if correct input
if len(sys.argv) != 3:
    print("Usage: python dna.py 'database.csv' 'sequence.txt'")
    sys.exit(1)

#open sequence and read
sequence = open(sys.argv[2], 'r').read()
for row in sequence:
    dnalist = row

#store sequence in a string
dna = dnalist[0]

#obtain individuals sequences from the database
with open(sys.argv[1]) as database:
    data = csv.reader(database)
    for row in data:
        dnaSeqs = row
        dnaSeqs.pop(0)
        break

#create dict where we store sequences to compare later
seqs = {}

#copy the list in a dict with genes as keys
for item in dnaSeqs:
    seqs[item] = 1

#iterate through sequence, count and store max value of each strand
for strand in seqs:

    i = 0
    cur_max = 0
    max_strand = 0

    #scan over sequence and find longest strand
    while i < len(sequence):
        window = sequence[i:i + len(strand)]

        if window == strand:
            cur_max = cur_max + 1
            max_strand = max(max_strand, cur_max)
            i = i + len(strand)

        #reset current max and move window one step
        else:
            cur_max = 0
            i = i + 1

    #store max in dict
    seqs[strand] = max_strand

#check if it matches
with open(sys.argv[1], newline='') as database:
    data = csv.DictReader(database)
    for person in data:
        match = 0
        for dna in seqs:
            if int(seqs[dna]) == int(person[dna]):
                match = match + 1
        if match == len(seqs):
            print(person['name'])
            sys.exit(0)

    print("No match")