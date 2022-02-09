#!/bin/bash

# to color the output text in different colours
orange=$(tput setaf 178);
white=$(tput setaf 15);
reset=$(tput sgr0);
bold=$(tput bold);

#Change number of testcases
tests=5

# You can change the version of C++ or add the compiler flags you wish
g++ -std=c++17 Generator.cpp -o Generator || { echo "${bold}${orange}Compilation Error in ${reset}" Generator.cpp; exit 1; }

./Generator $i
g++ -std=c++17 Test.cpp -o Test || { echo "${bold}${orange}Compilation Error${reset}" in Test.cpp; exit 1; }


for((i = 1; i <= $tests; i++));do
echo "${orange}test_case #$i: ${white}"

    # Generate test_case and save it in input.txt
    ./Generator > input.txt

    # Check time for Test.cpp
    time ./Test <input.txt> output
    echo " ";
done

#Delete the files that we don't need it after finished
rm output
rm Generator
rm Test
rm Test_output.txt