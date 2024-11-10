import os
import subprocess
import platform
import shutil
from contextlib import nullcontext
from .file_handler import change_directory_to_root


def get_executable_extension() -> str:
    system_platform = platform.system().lower()
    if system_platform == "windows":
        return ".exe"
    return ""  # No extension for Linux/macOS


def get_default_compiler() -> str:
    return "g++" if platform.system().lower() != "darwin" else "g++-13"


@change_directory_to_root
def compile_file(file_name: str, output_dir="cpp_src") -> None:
    # Get the full path of the file and the output file
    file_path = os.path.join(output_dir, file_name)
    output_file = file_path + get_executable_extension()

    # Get the default C++ compiler available on the system
    compiler = get_default_compiler()

    # Prepare the compiler command dynamically
    command = [compiler, f"{file_path}.cpp", "-o", output_file]

    try:
        subprocess.run(command, check=True, capture_output=True, text=True, shell=False)
    except subprocess.CalledProcessError as e:
        raise KeyError(f"Compilation Error in {file_name}.cpp: {e.stderr}")
    except FileNotFoundError:
        raise KeyError(f"File not found: {file_path}.cpp")


@change_directory_to_root
def run_file(
    file_name: str, input_file_name: str, output_file_name: str, time_limit: float = 2.0
):
    # Get the platform-specific executable path
    file_path = os.path.join("cpp_src", file_name)
    file_path = file_path + get_executable_extension()

    # Resolve input and output file paths
    input_path = os.path.join("test_data", input_file_name) if input_file_name else None
    output_path = (
        os.path.join("test_data", output_file_name) if output_file_name else None
    )

    # Check if files exist
    if not os.path.isfile(file_path):
        raise FileNotFoundError(f"Executable '{file_path}' not found.")
    if input_path and not os.path.isfile(input_path):
        raise FileNotFoundError(f"Input file '{input_path}' not found.")

    try:
        # Open input and output files using context management
        with open(input_path, "r") if input_path else nullcontext() as infile, (
            open(output_path, "w") if output_path else nullcontext()
        ) as outfile:
            # Build the command with the correct platform-specific executable
            command = [file_path]
            # Run the command with proper file handling
            run_result = subprocess.run(
                command,
                check=True,
                timeout=time_limit,
                stdin=infile if input_path else subprocess.PIPE,
                stdout=outfile if output_path else subprocess.PIPE,
                stderr=subprocess.PIPE,
                text=True,
            )
        return run_result
    except subprocess.TimeoutExpired as e:
        raise TimeoutError(f"Execution timed out after {time_limit} seconds.") from e
    except subprocess.CalledProcessError as e:
        raise RuntimeError(f"Error occurred while running the file: {e.stderr}") from e


def normalize_output(output_file: str) -> list[str]:
    directory = os.getcwd()
    output_path = os.path.join(directory, "test_data", output_file)
    try:
        with open(output_path, "r") as file:
            output = file.read().strip()
            lines = output.split("\n")
            tokens = [token for line in lines for token in line.strip().split()]
            return tokens
    except FileNotFoundError:
        return []


@change_directory_to_root
def check_diff(output1: str, output2: str) -> bool:
    normal_output1 = normalize_output(output_file=output1)
    normal_output2 = normalize_output(output_file=output2)
    return normal_output1 == normal_output2


@change_directory_to_root
def validate_errors(
    file_name: str,
    input_file_name: str,
    output_file_name: str,
    test: int,
    time_limit: float = 2.0,
):
    try:
        run_result = run_file(file_name, input_file_name, output_file_name, time_limit)
        return run_result
    except FileNotFoundError as e:
        raise KeyError(f"File not found: {e}")
    except subprocess.TimeoutExpired:
        raise KeyError(f"Time Limit Exceeded in {file_name}.cpp on test case {test} ⏳")
    except subprocess.CalledProcessError as e:
        print(e.stderr)
        raise KeyError(f"Runtime Error in {file_name}.cpp on test case {test} ⚠️")


@change_directory_to_root
def print_output(output_file: str) -> None:
    output_path = os.path.join("test_data", output_file)
    try:
        with open(output_path, "r") as file:
            print(file.read())
    except FileNotFoundError:
        raise FileNotFoundError(f"File '{output_path}' not found.")
    except OSError:
        raise OSError(f"Error reading file '{output_path}'.")
