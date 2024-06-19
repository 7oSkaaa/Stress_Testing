# Stress Testing

## Overview

Stress Testing is crucial for competitive programming to ensure the accuracy and efficiency of your code. This project helps you verify whether your solution passes all test cases within the time limit or if it might get a Time Limit Exceeded (TLE) error.

## Requirements

- Python 3

## Installation

  ```bash
  git clone https://github.com/yourusername/stress_testing.git
  cd stress_testing
  ```

## Usage

### Preparing Your Code

1. **Original Solution**: Copy your original solution which you expect might fail into `cpp_files/test.cpp`.
2. **Brute Force Solution**: Copy your brute force solution, which is expected to give the correct output, into `cpp_files/correct.cpp`.
3. **Test Case Generator**: Modify `cpp_files/generator.cpp` to generate test cases according to the question. `generator.cpp` will export the test cases to the `txt_files/input.txt` file.

### Running the Stress Test

You can run `stress_testing.py` from the terminal or any IDE.

1. **Terminal**:

    ```bash
    python stress_testing.py
    ```

2. **IDE**:
    - Open `stress_testing.py` in your preferred IDE and run it.

### Changing Number of Test Cases

The script will prompt you to enter the number of test cases and the time limit in seconds. The script will terminate if your solution exceeds the time limit.

## Outputs

- ***Accepted***: If your solution's output matches the brute force solution output.
- ***Wrong Answer***: Displays the input on which the solution failed, the output of your solution, and the expected output according to the brute force solution.
- ***Time Limit Exceeded***: If your solution exceeds the time limit.
- ***Runtime Error***: If your solution crashes.

## Generating Test Cases with Generator.cpp

If you use Generator.cpp, these functions will help you generate test cases:

- gen_string: Generate a string (uppercase/lowercase) in any range with your length.
- gen_string_u_and_l: Generate a string (uppercase and lowercase) in any range with your length.
- gen_int: Generate an integer number in the range from L to R.
- gen_palindrome: Generate a palindrome string with your length in the range you want.
- gen_array: Generate an array with your size and range, which can be normal, increasing, or decreasing.
- gen_unique_array: Generate an array with your size and range, with unique elements.
- gen_pyramid_array: Generate an array with your size and range, which can be a pyramid or inverted pyramid.
- gen_array_2D: Generate a 2D array with your size and range.
- gen_permutation: Generate an array with a permutation of N.
- gen_bit_int: Generate a string with your length consisting of digits in the range you want.
- gen_array_of_pairs: Generate an array of pairs with your length in the range you want, with an option for intervals.
- gen_char: Generate a character in the range you want.
- gen_tree: Generate a tree or a specific type like a bamboo or star tree.
- gen_graph: Generate a simple graph.

## Folder Structure

```python
📁 Stress_Testing
├───📁 cpp_files
│   ├───📄 generator.cpp
│   ├───📄 test.cpp
│   └───📄 correct.cpp
├───📁 txt_files
│   ├───📄 input.txt
│   ├───📄 correct_output.txt
│   └───📄 test_output.txt
├───📄 Structures.md
├───📁 utils
│   ├───📄 toolkit.py
│   └───📄 directory.py
├───📄 README.md
├───📄 .gitignore
├───📄 stress_testing.py
└───📁 helpers
    └───📄 colors.py
```
