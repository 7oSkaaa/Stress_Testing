#!/bin/bash

# to color the output text in different colours
green=$(tput setaf 71);
red=$(tput setaf 1);
blue=$(tput setaf 32);
orange=$(tput setaf 178);
bold=$(tput bold);
reset=$(tput sgr0);
white=$(tput setaf 15);

#Get current Directory
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

# You can change the version of C++ or add the compiler flags you wish
g++ -std=c++17 "$SCRIPT_DIR"/Generator.cpp -o Generator || { echo "${bold}${orange}Compilation Error ⚠️${reset}" in Generator.cpp; exit 1; }
g++ -std=c++17 "$SCRIPT_DIR"/Test.cpp -o Test || { echo "${bold}${orange}Compilation Error ⚠️${reset}" in Test.cpp; exit 1; }
g++ -std=c++17 "$SCRIPT_DIR"/Correct.cpp -o Correct || { echo "${bold}${orange}Compilation Error ⚠️${reset}" in Correct.cpp; exit 1; }

#Change number of testcasescc1plus: fatal error: Generator.cpp: No such file or directory
max_tests=10
diff_found=0
i=1

while [ $i -le $max_tests ]
do
    # Generate test_case and save it in input.txt
    ./Generator > input.txt

    # run Test solution, take input from above generated test case i.e. from input.txt
    # and save it in Test_output.txt
    ./Test < input.txt > Test_output.txt #|| {echo failed; exit 1;}

    # run Correct solution, take input from above generated test case i.e. from input.txt
    # and save it in Correct_output.txt
    ./Correct < input.txt > Correct_output.txt

    # check if files Test_output and Brute_output
    # differs(we are ignoring spaces and then comparing files)
    if diff --tabsize=1 -F --label --side-by-side --ignore-space-change Test_output.txt Correct_output.txt > dont_show_on_terminal.txt; then
        echo ""
        echo "${orange}Test Case #$i: ${bold}${green}Passed ✅${reset}"
    else
        echo ""
        echo "${orange}test case #$i: ${bold}${red}Failed ❌${reset}"
        diff_found=1
        break
    fi
    i=$((i+1))
done

if [ $diff_found -eq 1 ]
then
    echo ""
    echo "${white}Input: ${blue}"
    cat input.txt

    echo ""
    echo "${white}Output: ${blue}"
    cat Test_output.txt

    echo ""
    echo "${white}Expected: ${blue}"
    cat Correct_output.txt

    notify-send "Wrong Answer ❌"
else
    notify-send "Accepted ✅"
fi

rm Correct
rm Generator
rm Test
rm Test_output.txt
rm Correct_output.txt
rm dont_show_on_terminal.txt
echo ""