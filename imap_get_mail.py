#!/usr/bin/python
import imaplib
import string 
import StringIO
import rfc822

import cStringIO
import email
import base64
import re
import sys

host = 'HOST'

username = 'USERNAME'
password = 'PASSWORD'

# log in and get total email count
imap = imaplib.IMAP4(host)
imap.login(username, password)
imap.select('INBOX', 1)
typ,data = imap.search(None, 'SINCE','31-Dec-2010', 'BEFORE', '06-Jan-2011')
email_num_list = data[0].split()
total_email = len(email_num_list)
for num in email_num_list:
    typ, data = imap.fetch(num, '(RFC822)')
    print 'Message %s of %d' % (num, total_email)
    filename = "%.4d" % int(num)
    mailfile = open( filename, 'wb')
    mailfile.write(data[0][1])
imap.close()
imap.logout()
