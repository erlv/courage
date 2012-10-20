#!/usr/bin/python
import random


filename = 'emails.txt'

def shuffle(ary):
    length = len(ary)
    end_index = length -1
    for eachone in range( end_index, 0, -1):
        randint = random.randint(0, eachone)
	print randint
        if randint == eachone:
            continue
        ary[eachone], ary[randint] = ary[randint], ary[eachone]
    return ary

inputfile = file( filename, 'r')
emaillist = inputfile.readlines()
#print emaillist
newemaillist = shuffle( emaillist)
#print newemaillist
