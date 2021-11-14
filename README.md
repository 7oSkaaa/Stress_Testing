# Strees_Testing

## Stress.sh

Strees Testing is very important for every competitive programming to know if your code is accurated 100% or not, and if you want to know if it will be passed in the time limit or will exceeded it

### Installing
Download this repository manually or by using git clone on terminal.

Copy your original solution which you expect might fail in the file `Test.cpp`. <br>
Copy your brute force solution which is expected to give correct output in the file `Brute.cpp`. <br>
Change the `Generated.cpp` file so as to generate test cases according to the question. <br>
`Generated.cpp` will export the test cases to the `input.txt` file

You can run `Strees.sh` from the terminal or you can run with **vscode** or any **IDE**

### Changing number of test cases
You can open `Strees.sh` an choose the number of test cases by editing the line `16`

Output:<br> The output of running file `Strees.sh` on every test case is either `Accepted` if your solution's output matches the brute solution output or `Wrong Answer` and will show the input on which the solution failed, the output of your solution and expected output according to the brute force solution on terminal and the script will be terminated. Now you can debug your original solution and check where it goes wrong. <br>
If you wish to terminate the script at any moment you wish use the command `ctrl + c` in your terminal.<br>

Quick Demo: below the solution fails on randomly generated test #2: <br>

![ ](https://i.ibb.co/L1Dp4H0/Strees-Testing.png)



## Time.sh

Copy your solution which you want to check about his time to `Test.cpp`. <br>
Change the `Generated.cpp` file so as to generate test cases according to the question. <br>

### Changing number of test cases

You can run `Time.sh` from the terminal or you can run with **vscode** or any **IDE**
You can open `Time.sh` an choose the number of test cases by editing the line `1`

The `Time.sh` will excecute `Test.cpp` the number of times you want and print you the time that your code take and each time with distinct test

Quick Demo: below the solution times on randomly generated test: <br>

![ ](https://i.ibb.co/Dg2ZsJY/Time.png)


The Orignal Code for Strees and Generator before any edit is from [Bhupendra Yadav Stress Testing bash script
](https://github.com/bhupixb/Stress-Testing-bash-script) you can access it from here.
