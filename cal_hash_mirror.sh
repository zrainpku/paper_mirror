#!/bin/bash

echo "hello world!"
for file in `cat ./filter.txt`
do 
    array=(${file#*=})
    array=(${array#\"})
    array=(${array%\"})
    echo $array
    python hash_code.py $array
    sleep 1s 
    echo $array >> result.txt
    ./main xx.png >>result.txt
    #break
done
