#!/bin/bash

# to color the output text in different colours
green=$(tput setaf 71);
red=$(tput setaf 1);
blue=$(tput setaf 32);
orange=$(tput setaf 178);
bold=$(tput bold);
reset=$(tput sgr0);

# You can change the version of C++ or add the compiler flags you wish
g++ -std=c++17 Generator.cpp -o Generator || { echo ${bold}${orange}Compilation Error in ${reset} Generator.cpp; exit 1; }
g++ -std=c++17 Test.cpp -o Test || { echo ${bold}${orange}Compilation Error${reset} in $Test.cpp; exit 1; }
g++ -std=c++17 Correct.cpp -o Correct || { echo ${bold}${orange}Compilation Error${reset} in $Correct.cpp; exit 1; }

max_tests=10
diff_found=0
i=1

while [ $i -le $max_tests ]
do
    # Generate test_case and save it in input.txt
    ./Generator > input.txt

    # run original solution, take input from above generated test case i.e. from input.txt
    # and save it in original_output.txt
    ./Test < input.txt > Test_output.txt #|| {echo failed; exit 1;}

    # run brute force solution, take input from above generated test case i.e. from input.txt
    # and save it in Brute_output.txt
    ./Correct < input.txt > Correct_output.txt

    # check if files Test_output and Brute_output
    # differs(we are ignoring spaces and then comparing files)
    if diff --tabsize=1 -F --label --side-by-side --ignore-space-change Test_output.txt Correct_output.txt > dont_show_on_terminal.txt; then
        echo "${orange}test_case #$i: ${bold}${green}passed${reset}"
    else
        echo "${orange}test_case #$i: ${bold}${red}failed${reset}"
        diff_found=1
        break
    fi
    i=$((i+1))
done

if [ $diff_found -eq 1 ]
then
    echo "${blue}Input: ${reset}"
    cat input.txt
    echo ""

    echo "${blue}Output: ${reset}"
    cat Test_output.txt
    echo ""

    echo "${blue}Expected: ${reset}"
    cat Correct_output.txt
    echo ""

    notify-send "Wrong Answer"
else
    notify-send "Accepted"
fi

rm Test_output.txt
rm Correct_output.txt
rm dont_show_on_terminal.txt