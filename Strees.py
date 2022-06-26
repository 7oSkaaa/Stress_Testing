import os
import subprocess
import pyperclip

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

def delete_file(file):
    try:
        os.remove(file)
    except:
        pass

def Reset_Folder():
    # delete temporary files and make input.txt empty
    open('input.txt', "w")
    delete_file(directory+'/Correct')
    delete_file(directory+'/Generator')
    delete_file(directory+'/Test')
    delete_file(directory+'/Test_output.txt')
    delete_file(directory+'/Correct_output.txt')
    delete_file("Failed.txt")
    print()

def copy_failed_test():
    with open('Failed.txt', 'w') as Failed_Test:
        print("Input:\n" + Input + "\n\nOutput:\n" + Test_output + "\n\nExpected:\n" + Correct_output, file=Failed_Test)
    
    Failed_Test = open("Failed.txt", "r").read()
    pyperclip.copy(Failed_Test)

# Function to Compile the file
def compile(file_name):
    try:
        subprocess.run(["g++", "{dir}/{file}.cpp".format(dir = directory, file = file_name), "-o", "{dir}/{file}".format(dir = directory, file = file_name)], check= True, stdout=subprocess.PIPE,stderr=subprocess.PIPE)
    except subprocess.CalledProcessError as e:
        print(f'{bold}{red}\nCompilation Error in {file_name}.cpp ⚠️{reset}\n')
        exit(0)

# Function to run the file
def run(file_name, is_Generator):
    try:
        if is_Generator:
            subprocess.run(["./{}".format(file_name)], capture_output = True)
        else:
            subprocess.run(["./{}".format(file_name), "<", "/input.txt"], capture_output = True, timeout=5)
    except subprocess.TimeoutExpired:
        print(f'{bold}{red}\nTime Limit Exceeded in {file_name}.cpp ⏳{reset}\n')
        with open('input.txt', "r") as Input_File:
            test_case = Input_File.read()
            pyperclip.copy('Input:\n' + test_case)
            print('Input:\n' + test_case)
        Reset_Folder()
        exit(0)

#number of test cases 
tests = int(input(f'{magenta}\nEnter number of testcases: {white}'))
check_diff, Input, Correct_output, Test_output = check_diff = False, "", "", ""

#Compile the files
compile('Generator')
compile('Correct')
compile('Test')

for i in range(1, tests + 1):
    # run files
    run('Generator', True)
    run('Correct', False)
    run('Test', False)
    
    Input = open("input.txt").read().strip()
    Correct_output = open("Correct_output.txt").read().strip()
    Test_output = open("Test_output.txt").read().strip()
    
    if Correct_output == Test_output:
        print(f'\n{bold}{gold}Test Case #{i}: {green}Passed ✅')
    else:
        print(f'\n{bold}{gold}Test Case #{i}: {red}failed ❌')
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
    
    # copy the failed test to clipboard
    copy_failed_test()

Reset_Folder()