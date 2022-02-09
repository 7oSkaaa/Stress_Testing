#!/bin/bash

tests=5

for((i = 1; i <= $tests; i++));do
echo $i 
./Generator $i
time ./Test <input.txt> output
echo " ";
done

rm output.txt
rm output