#! /bin/bash

#./tests > /tmp/log.txt
#cat /tmp/log.txt | sort | uniq  > /tmp/loguniq.txt

cat /tmp/out | sort | uniq  > /tmp/loguniq.txt
cut -d ' ' -f 1 /tmp/loguniq.txt > /tmp/loguniqhash.txt
cat /tmp/loguniqhash.txt | uniq > /tmp/loguniqhashuniq.txt
#wc -l /tmp/loguniqhash.txt /tmp/loguniqhashuniq.txt 
let i=$(wc -l /tmp/loguniqhash.txt | cut -d ' ' -f 1)-$(wc -l /tmp/loguniqhashuniq.txt | cut -d ' ' -f 1)
echo "collisions : $i"