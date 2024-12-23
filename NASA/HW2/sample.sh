#!/bin/bash
# 1
./compare.sh dir1 dir2 > output/1-1.txt
./compare.sh -r dir1 dir2/hi.txt > output/1-2.txt
./compare.sh dir1/hi.txt dir2/hi.txt > output/1-3.txt
./compare.sh -r dir1 > output/1-4.txt
./compare.sh -a -n hello dir1 dir2 > output/1-5.txt
./compare.sh -r -c dir1 dir2 > output/1-6.txt
./compare.sh -n -r dir1 dir2 > output/1-7.txt
# 2
./compare.sh dir1/hello.txt dir2/hi.txt > output/2-1.txt
./compare.sh dir1/link1 dir2/link1 > output/2-2.txt
# 3
./compare.sh -r dir1 dir2 > output/3-1.txt
# 4
./compare.sh -a -r dir1/code dir1/.code.backup > output/4-1.txt
./compare.sh -r -l dir1 dir2 > output/4-2.txt
./compare.sh -a -l -r dir1 dir2 > output/4-3.txt
# 5
./compare.sh -r -n link -l dir1 dir2 > output/5-1.txt
./compare.sh -a -r -n txt dir1 dir2 > output/5-2.txt
