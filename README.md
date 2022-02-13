# Strees_Testing

<br>

### [Tutorial Video](https://www.youtube.com/watch?v=8UvUyVjpWdU)

<br>

## Stress.py

**Windows and Linux Users**

Strees Testing is very important for every competitive programming to know if your code is accurated 100% or not, and if you want to know if it will be passed in the time limit or will exceeded it

### Requirements

```
Install python3
```
### Installing
Download this repository manually or by using git clone on terminal.

Copy your original solution which you expect might fail in the file `Test.cpp`. <br>
Copy your brute force solution which is expected to give correct output in the file `Correct.cpp`. <br>
Change the `Generated.cpp` file so as to generate test cases according to the question. <br>
`Generated.cpp` will export the test cases to the `input.txt` file

You can run `Strees.py` from the terminal or you can run with **vscode** or any **IDE**

### Changing number of test cases

The script will ask you to enter the number of `testcases`

Output:<br> The output of running file `Strees.sh` on every test case is either `Accepted` if your solution's output matches the brute solution output or `Wrong Answer` and will show the input on which the solution failed, the output of your solution and expected output according to the brute force solution on terminal and the script will be terminated. Now you can debug your original solution and check where it goes wrong. <br>
If you wish to terminate the script at any moment you wish use the command `ctrl + c` in your terminal.<br>

<br> <br>

Quick Demo: below the solution fails on randomly generated test #5: 

<br>

![Screenshot from 2022-02-13 21-43-38](https://user-images.githubusercontent.com/63050133/153771930-0525c5bb-5bd5-48f4-8fdc-0934a46ea56a.png)

<br><br>

## Stress.sh

**Linux Users Only**

Strees Testing is very important for every competitive programming to know if your code is accurated 100% or not, and if you want to know if it will be passed in the time limit or will exceeded it

### Installing
Download this repository manually or by using git clone on terminal.

Copy your original solution which you expect might fail in the file `Test.cpp`. <br>
Copy your brute force solution which is expected to give correct output in the file `Correct.cpp`. <br>
Change the `Generated.cpp` file so as to generate test cases according to the question. <br>
`Generated.cpp` will export the test cases to the `input.txt` file

You can run `Strees.sh` from the terminal or you can run with **vscode** or any **IDE**

### Changing number of test cases
You can open `Strees.sh` an choose the number of test cases by editing the line `21`

Output:<br> The output of running file `Strees.sh` on every test case is either `Accepted` if your solution's output matches the brute solution output or `Wrong Answer` and will show the input on which the solution failed, the output of your solution and expected output according to the brute force solution on terminal and the script will be terminated. Now you can debug your original solution and check where it goes wrong. <br>
If you wish to terminate the script at any moment you wish use the command `ctrl + c` in your terminal.<br>

### Install Notify-Send


We will install notify-send to send to us notification with accepted or wrong answer


```
sudo apt-get install notify-osd
```

<br> <br>

Quick Demo: below the solution fails on randomly generated test #2: <br>

![  ](https://i.ibb.co/3drRPL4/Screenshot-from-2022-02-09-23-19-36.png)


<br><br>
<hr>
<br><br>

## Time.sh

**Linux Users Only**

Copy your solution which you want to check about his time to `Test.cpp`. <br>
Change the `Generated.cpp` file so as to generate test cases according to the question. <br>

### Changing number of test cases

You can run `Time.sh` from the terminal or you can run with **vscode** or any **IDE**
You can open `Time.sh` an choose the number of test cases by editing the line `10`

The `Time.sh` will excecute `Test.cpp` the number of times you want and print you the time that your code take and each time with distinct test

Quick Demo: below the solution times on randomly generated test: <br>

![image](https://user-images.githubusercontent.com/63050133/153291510-924cd5ec-ea1b-4421-8947-ccd0419f2dda.png)
