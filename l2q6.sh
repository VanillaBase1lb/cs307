#!/usr/bin/sh

i=1
while [ $i -le 20 ]
do
touch "file$i.txt"
i=$(($i+1))
done

echo files created.enter anything to detele agian
read nothing

i=1
while [ $i -le 20 ]
do
rm "file$i.txt"
i=$(($i+1))
done