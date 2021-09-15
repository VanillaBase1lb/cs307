#!/usr/bin/sh

echo Enter 1st number
read n1
echo Enter 2nd number
read n2

n3=$n1
n1=$n2
n2=$n3

echo n1=$n1
echo n2=$n2