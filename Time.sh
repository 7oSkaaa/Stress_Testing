#!/bin/bash

# to color the output text in different colours
orange=$(tput setaf 178);
white=$(tput setaf 15);
reset=$(tput sgr0);
bold=$(tput bold);

#Change number of testcases
tests=5
i=1

#Get current Directory
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

# You can change the version of C++ or add the compiler flags you wish
g++ -std=c++17 "$SCRIPT_DIR"/Generator.cpp -o Generator || { echo "${bold}${orange}Compilation Error in ${reset}" Generator.cpp; exit 1; }

g++ -std=c++17 "$SCRIPT_DIR"/Test.cpp -o Test || { echo "${bold}${orange}Compilation Error${reset}" in Test.cpp; exit 1; }

while [ $i -le $tests ]
do
    echo "${orange}Test Case #$i: ${white}"

    # Generate test_case and save it in input.txt
    ./Generator > input.txt

    # Check time for Test.cpp
    time ./Test <input.txt> output
    echo " ";
    i=$((i+1))
done

#Delete the files that we don't need it after finished
rm output
rm Generator
rm Test
rm Test_output.txt