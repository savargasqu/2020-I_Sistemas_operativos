#!/bin/bash

echo "TIMES FOR BINARY FILES" > "times.txt"
num=25
for i in 0 1 2 3 4 5
do
let "num=$num*10"
{ time ./bin/bin_files $num; } 2>> "times.txt"
done


echo "TIMES FOR PIPES" >> "times.txt"
num=25
for i in 0 1 2 3 4 5
do
let "num=$num*10"
{ time ./bin/pipes $num; } 2>> "times.txt"
done


echo "TIMES FOR SHARED MEMORY" >> "times.txt"
num=25
for i in 0 1 2 3 4 5
do
let "num=$num*10"
{ time ./bin/shared_memory $num; } 2>> "times.txt"
done


echo "TIMES FOR SOCKETS" >> "times.txt"
num=25
for i in 0 1 2 3 4 5
do
let "num=$num*10"
{ time ./bin/server $num & } 2>> times.txt # Keep in background to run client
sleep 0
./bin/client $num 127.0.0.1
done

