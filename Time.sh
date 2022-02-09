#!/bin/bash

orange=$(tput setaf 178);
white=$(tput setaf 15);
red=$(tput setaf 1);
reset=$(tput sgr0);
bold=$(tput bold);


tests=5

for((i = 1; i <= $tests; i++));do
echo "${orange}test_case #$i: ${white}" 
./Generator $i
g++ -std=c++17 Test.cpp -o Test || { echo ${bold}${red}Compilation Error${reset} in Test.cpp; exit 1; }
time ./Test <input.txt> output
echo " ";
done

rm output