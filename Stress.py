import os
import os.path as path
import subprocess

import pyperclip

# Colors to use
red = "\033[38;5;196m"
green = "\033[38;5;40m"
blue = "\033[34m"
gold = "\033[38;5;220m"
white = "\33[37m"
magenta = "\033[35m"
bold = "\033[01m"
reset = "\033[0m"

# Get the directory of current folder
directory = os.path.dirname(os.path.realpath(__file__))
os.chdir(directory)
Input, Correct_output, Test_output, check_diff = "", "", "", False
WA_Error = 255


# Function to Handle error of delete file not found
def delete_file(file):
    try:
        os.remove(file)
    except OSError:
        pass


# delete temporary files and make input.txt empty
def Reset_Folder():
    open("input.txt", "w")
    delete_file(path.join(directory, "Correct"))
    delete_file(path.join(directory, "Generator"))
    delete_file(path.join(directory, "Test"))
    delete_file(path.join(directory, "Test_output.txt"))
    delete_file(path.join(directory, "Correct_output.txt"))
    delete_file(path.join(directory, "Checker"))
    delete_file("Failed.txt")
    print()


# copy the failed test cases to clipboard
def copy_failed_test():
    with open("Failed.txt", "w") as Failed_Test:
        print(
            f"""Input:\n{Input}
            \n\nOutput:\n
            {Test_output}
            \n\nExpected:\n
            {Correct_output}""",
            file=Failed_Test,
        )

    Failed_Test = open("Failed.txt", "r").read()
    pyperclip.copy(Failed_Test)


# Function to Compile the file
def compile(file_name):
    try:
        subprocess.run(
            [
                "g++",
                "-std=c++2a",
                f"{path.join(directory, file_name + '.cpp')}",
                "-o",
                f"{path.join(directory, file_name)}",
            ],
            check=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
        )
    except subprocess.CalledProcessError:
        print(f"{bold}{red}\nCompilation Error in {file_name}.cpp ⚠️{reset}\n")
        exit(0)


# Function to run the file
def run(file_name, with_time_limit, timelimit=1):
    try:
        if not with_time_limit:
            subprocess.run(
                [f"{path.join('.', file_name)}"], capture_output=True, check=True
            )
        else:
            subprocess.run(
                [f"{path.join('.', file_name)}", "<", "input.txt"],
                capture_output=True,
                timeout=timelimit,
                check=True,
            )
    except subprocess.TimeoutExpired:
        print(f"{bold}{red}\nTime Limit Exceeded in {file_name}.cpp ⏳{reset}\n")
        with open("input.txt", "r") as Input_File:
            test_case = Input_File.read()
            pyperclip.copy("Input:\n" + test_case)
            # print the test
            print(f"{bold}{white}\nInput: {blue}")
            print(test_case)
        Reset_Folder()
        exit(0)
    except subprocess.CalledProcessError as e:
        if e.returncode == WA_Error:
            return e.returncode
        print(f"{bold}{red}\nRuntime Error in {file_name}.cpp ⚠️{reset}\n")
        with open("input.txt", "r") as Input_File:
            test_case = Input_File.read()
            pyperclip.copy("Input:\n" + test_case)
            # print the test
            print(f"{bold}{white}\nInput: {blue}")
            print(test_case)
        Reset_Folder()
        exit(0)
    return 0


def is_Passed(is_default_check=True):
    global check_diff, Input, Correct_output, Test_output
    Correct_output = open("Correct_output.txt").read().strip()
    Test_output = open("Test_output.txt").read().strip()

    # if it is default checker then just compare the output and return the result
    if is_default_check:
        return Correct_output == Test_output

    # run the checker file and if it returns -1 then the test case failed
    return run(file_name="Checker", with_time_limit=False) != WA_Error


def main():
    global check_diff, Input, Correct_output, Test_output
    # number of test cases
    tests = int(input(f"{magenta}\nEnter number of testcases: {white}"))
    default_checker = (
        input(f"{magenta}\nUse default checker? (y/n): {white}").lower() != "n"
    )
    timelimit = float(input(f"{magenta}\nSet Time Limit (in seconds): {white}"))

    # Compile the files
    compile("Generator")
    compile("Correct")
    compile("Test")
    if not default_checker:
        compile("Checker")

    for i in range(1, tests + 1):
        # run files
        run(file_name="Generator", with_time_limit=False, timelimit=timelimit)
        run(file_name="Correct", with_time_limit=True, timelimit=timelimit)
        run(file_name="Test", with_time_limit=True, timelimit=timelimit)

        if is_Passed(is_default_check=default_checker):
            print(f"\n{bold}{gold}Test Case #{i}: {green}Passed ✅")
        else:
            print(f"\n{bold}{gold}Test Case #{i}: {red}failed ❌")
            check_diff = True
            break

    # print the test case if failed and copy it to clipboard
    if check_diff is True:
        # get the failed test case if any and copy it to clipboard
        Input = open("input.txt").read().strip()
        Correct_output = open("Correct_output.txt").read().strip()
        Test_output = open("Test_output.txt").read().strip()

        # print the test
        print(f"{bold}{white}\nInput: {blue}")
        print(Input)

        # print the output
        print(f"{bold}{white}\nOutput: {blue}")
        print(Test_output)

        # print the expected output
        print(f"{bold}{white}\nExpected: {blue}")
        print(Correct_output)

        # copy the failed test to clipboard
        copy_failed_test()

    Reset_Folder()


if __name__ == "__main__":
    main()
