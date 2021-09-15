#!/usr/bin/sh

n=`find . -name "*.c" | wc -l`
i=1
while [ $i -le $n ]
do
cfile=`find . -name "*.c" | head -n $i`
gcc $cfile
./a.out
i=$(($i+1))
done
