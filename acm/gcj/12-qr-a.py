#!/usr/bin/python
import collections

input_fn="A-small-attempt1.in"


a="ejp mysljylc kd kxveddknmc re jsicpdrysi"
a_x="our language is impossible to understand"
b="rbcpc ypc rtcsra dkh wyfrepkym veddknkmkrkcd"
b_x="there are twenty six factorial possibilities"
c="de kr kd eoya kw aej tysr re ujdr lkgc jv"
c_x="so it is okay if you want to just give up"
d="y qeez"
d_x="a zooq"

dict_map = dict(zip(a, a_x) + (zip(b, b_x)) + (zip(c, c_x)) + (zip(d, d_x)))
#print dict_map

f=open(input_fn, 'r')
tt_case =int(f.readline())
for case in range(tt_case):
    result="Case #"+str(case+1)+": "
    line = f.readline()
    line = line.rstrip()
    for char in line:
        result += dict_map[char]
    print result
        
