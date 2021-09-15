#!/usr/bin/sh

echo Enter the number
read n

if [ $(($n%2)) -eq 0 ]
then
echo even
else
echo odd
fi