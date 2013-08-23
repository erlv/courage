#!/bin/sh
V=5
mkdir /mnt/data/00_smallfile_$V
echo `date +%s`
cp -ri ../00_smallfile/00/000*  /mnt/msheng_data/00_smallfile_$V/ &
cp -ri ../00_smallfile/00/001*  /mnt/data/00_smallfile_$V/ &
cp -ri ../00_smallfile/00/002*  /mnt/data/00_smallfile_$V/ &
cp -ri ../00_smallfile/00/003*  /mnt/data/00_smallfile_$V/ &
wait
echo `date +%s`
