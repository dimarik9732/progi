#! /bin/bash

for (( count=1; count<1000; count++ ))
do 
 sleep 10 | echo "$count" &
 done
