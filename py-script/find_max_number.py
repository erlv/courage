#!/usr/bin/python

import os
import re

FILENAME="vim.strace"
strace_file = open(FILENAME)
all_time = {}
line_count=1
#re_time = re.compile('\d+\.\d+')

line = strace_file.readline()

while line:
    #time_dur_str_match = re_time.match( line )
    #time_dur_str = time_dur_str_match.group(0)
    time_dur_str_lst = re.findall("\d+\.\d+", line)
    if( len(time_dur_str_lst)  == 1):
        time_dur_str = time_dur_str_lst[0]
    else:
        time_dur_str = "0"


    try:
        time_dur = float(time_dur_str)
    except ValueError:
        print line[line.find('>')+1:][(line.find('<')+1):line.find('>')]
        time_dur_str = float(line[line.find('<')+1:][(line.find('<')+1):line.find('>')])

    all_time[line_count] = time_dur
    line_count=line_count+1
    line = strace_file.readline()

sorted(all_time.items, key=lambda all_time:alltime[0])
print all_time[-5:0]
