#!/usr/bin/python
import sys
import hashlib
m=hashlib.md5()
str = input("Enter the string and Press Enter:")
m.update(str)
print m.hexdigest()
