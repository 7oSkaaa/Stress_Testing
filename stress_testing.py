from helpers.colors import Colors
from utils import directory, toolkit


def stress_testing():
    test_cases = int(
        input(f"{Colors.fg.gold}Enter the number of test cases: {Colors.reset}")
    )
    time_limit = float(
        input(
            f"{Colors.fg.gold}Enter the time limit for each test case (seconds): {Colors.reset}"
        )
    )

    print(f"{Colors.fg.white}\n=========================={Colors.reset}")

    list_of_files = ["generator", "correct", "test"]
    list_of_input_files = ["", "input.txt", "input.txt"]
    list_of_output_files = ["input.txt", "correct_output.txt", "test_output.txt"]

    for file in list_of_files:
        try:
            toolkit.compile_file(file)
        except KeyError as e:
            print(f"\n{Colors.fg.red}{e}{Colors.reset}")
            return

    for test in range(1, test_cases + 1):
        for index in range(len(list_of_files)):
            try:
                toolkit.validate_errors(
                    list_of_files[index],
                    list_of_input_files[index],
                    list_of_output_files[index],
                    test,
                    time_limit,
                )
            except KeyError as e:
                print(f"\n{Colors.fg.red}{e}{Colors.reset}")
                return

        if toolkit.check_diff("correct_output.txt", "test_output.txt"):
            print(
                f"{Colors.fg.white}Test case {test} {Colors.fg.green}passed! ✅{Colors.reset}"
            )
        else:
            print(
                f"{Colors.fg.red}Wrong Answer {Colors.fg.white}on test case {test} ❌{Colors.reset}"
            )

            print(f"{Colors.fg.purple}Input: {Colors.reset}")
            toolkit.print_output("input.txt")

            print(f"{Colors.fg.purple}Expected Output: {Colors.reset}")
            toolkit.print_output("correct_output.txt")

            print(f"{Colors.fg.purple}Your Output: {Colors.reset}")
            toolkit.print_output("test_output.txt")

            return

    print(f"{Colors.fg.white}==========================\n{Colors.reset}")

    print(f"\n{Colors.fg.pink}All test cases {Colors.fg.green}passed! ✅{Colors.reset}")


if __name__ == "__main__":
    stress_testing()
    directory.clean_up()
