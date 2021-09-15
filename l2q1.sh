#!/usr/bin/sh

echo Enter radius
read r
pi=3.14

area=$(echo $pi*$r*$r | bc)
circ=$(echo $pi*2*$r | bc)
echo area=$area
echo circumference=$circ