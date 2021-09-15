#!/usr/bin/sh

echo Enter the number
read n

i=2
flag=1
while [ $i -le $(($n/2)) ]
do
    if [ $(($n%$i)) -eq 0 ]
    then
    flag=0
    fi
    i=$(($i+1))
done

if [ $flag -eq 0 ]
then
echo not prime
else
echo prime
fi