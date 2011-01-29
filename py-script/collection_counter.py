#!/usr/bin/python
import collections

filename = 'filename'

inputfile = file( filename, 'r')
emaillist = inputfile.readlines()
emailinfo_iter = emaillist[0]
emailinfo_pos_pre, emailinfo_addr_pre = emailinfo_iter.split(':')
distance_lst = []
for emailinfo_iter_pos in range(1, len(emaillist) ):
    emailinfo_iter = emaillist[emailinfo_iter_pos]
    emailinfo_pos_cur, emailinfo_addr_cur = emailinfo_iter.split(':')
    distance = int(emailinfo_pos_cur) - int(emailinfo_pos_pre)
    distance_lst.append(distance)
    emailinfo_pos_pre = emailinfo_pos_cur
    mailinfo_addr_pre = emailinfo_pos_cur

cnt = collections.Counter()
for dis in distance_lst:
    cnt[dis] += 1

cnt_dict = dict(cnt)

# write the count to file
csvfilename = filename + '.csv'
csvfile = open(csvfilename, 'wb')
cnt_list = cnt_dict.keys()
cnt_list.sort()
for cnt in cnt_list:
    line = str(cnt) + ',' + str(cnt_dict[cnt]) + '\n'
    csvfile.write(line)

csvfile.close()
print "done"
