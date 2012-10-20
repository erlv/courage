#!/usr/bin/python
import collections

#input_fn="B-small-attempt0.in"
input_fn="B-input"
f=open(input_fn, 'r')
tt_case =int(f.readline())

for case in range(tt_case):
    result="Case #"+str(case+1)+": "
    line  = f.readline().rstrip()
    tt_gler_str, sur_num_str, low_b_str, score_str = line.split(None,3)
    score_lst = score_str.split()
    score_lst = sorted([int(i) for i in score_lst])
    #score_lst = score_lst.reverse()
    tt_gler = int(tt_gler_str)
    sur_num = int(sur_num_str)
    low_b = int(low_b_str)
   # print score_lst
   # print sur_num
   # print low_b

    sat_gler = tt_gler - sur_num
    max_res = 0
    for i in score_lst[:]:
       # print i
        if i > ((low_b -1)*3):
            max_res += 1
        elif (i >= low_b) & (sur_num > 0):
            max_res += 1
            sur_num -= 1
      #  print "max_res="+str( max_res)

    print result+str(max_res)
        
