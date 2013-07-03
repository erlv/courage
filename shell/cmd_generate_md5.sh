#!/bin/sh

find . -name "*" | grep -v _log | sort  >> filelist
for line in `cat filelist`
do
  echo $line
  md5sum $line >> checksumrecord
done
