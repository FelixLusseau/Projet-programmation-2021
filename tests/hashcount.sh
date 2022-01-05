#! /bin/bash

./tests > /tmp/log.txt
cat /tmp/log.txt | sort | uniq  > /tmp/loguniq.txt
cut -d ' ' -f 1 /tmp/loguniq.txt > /tmp/loguniqhash.txt
cat /tmp/loguniqhash.txt | uniq > /tmp/loguniqhashuniq.txt
wc -l /tmp/loguniqhash.txt /tmp/loguniqhashuniq.txt 