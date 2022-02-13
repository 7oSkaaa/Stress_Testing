import os
import subprocess

#Colors to use
red = '\033[38;5;196m'
green = '\033[38;5;40m'
blue = '\033[34m'
gold = '\033[38;5;220m'
white = '\33[37m'
magenta = '\033[35m'
bold = '\033[01m'
reset = '\033[0m'

#Get the directory of current folder
directory = os.path.dirname(os.path.realpath(__file__))
os.chdir(directory)

#number of test cases 
tests = int(input(f'{magenta}\nEnter number of testcases: {white}'))
check_diff, Input, Correct_output, Test_output = check_diff = False, "", "", ""

#Compile the files
try:
    subprocess.run(["g++", "{}/Generator.cpp".format(directory), "-o", "{}/Generator".format(directory)], check= True, stdout=subprocess.PIPE,stderr=subprocess.PIPE)
except subprocess.CalledProcessError as e:
    print(f'{bold}{red}\nCompilation Error in Generator.cpp{reset}\n')
    exit(0)

try:
    subprocess.run(["g++", "{}/Correct.cpp".format(directory), "-o", "{}/Correct".format(directory)], check= True, stdout=subprocess.PIPE,stderr=subprocess.PIPE)
except subprocess.CalledProcessError as e:
    print(f'{bold}{red}\nCompilation Error in Correct.cpp{reset}\n')
    exit(0)
    
try:
    subprocess.run(["g++", "{}/Test.cpp".format(directory), "-o", "{}/Test".format(directory)], check= True, stdout=subprocess.PIPE,stderr=subprocess.PIPE)
except subprocess.CalledProcessError as e:
    print(f'{bold}{red}\nCompilation Error in Test.cpp{reset}\n')
    exit(0)

for i in range(1, tests + 1):
    subprocess.run(["./Generator"], capture_output = True)
    subprocess.run(["./Correct", "<", "/input.txt"], capture_output = True)
    subprocess.run(["./Test", "<", "/input.txt"], capture_output = True)
    Input = open("input.txt").read().strip()
    Correct_output = open("Correct_output.txt").read().strip()
    Test_output = open("Test_output.txt").read().strip()
    if Correct_output == Test_output:
        print(f'\n{bold}{gold}Test_Case #{i}: {green}Passed')
    else:
        print(f'\n{bold}{gold}Test_Case #{i}: {red}failed')
        check_diff = True
        break
    
if check_diff == True:
    #print the test
    print(f'{bold}{white}\nInput: {blue}')
    print(Input)
    
    #print the output
    print(f'{bold}{white}\nOutput: {blue}')
    print(Test_output)
    
    #print the expected output
    print(f'{bold}{white}\nExpected: {blue}')
    print(Correct_output)

# delete temporary files
os.remove(directory+'/Correct')
os.remove(directory+'/Generator')
os.remove(directory+'/Test')
os.remove(directory+'/Test_output.txt')
os.remove(directory+'/Correct_output.txt')
print()