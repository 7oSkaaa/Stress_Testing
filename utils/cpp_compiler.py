import os
import subprocess
import platform
from contextlib import nullcontext
from .file_handler import change_directory_to_root


def get_executable_extension() -> str:
    return ".exe" if platform.system().lower() == "windows" else ""


def get_default_compiler() -> str:
    return "g++-13" if platform.system().lower() == "darwin" else "g++"


@change_directory_to_root
def compile_file(file_name: str, output_dir="cpp_src") -> None:
    file_path = os.path.join(output_dir, file_name)
    output_file = file_path + get_executable_extension()
    command = [get_default_compiler(), f"{file_path}.cpp", "-o", output_file]
    try:
        subprocess.run(command, check=True, capture_output=True, text=True)
    except Exception:
        raise ValueError(f"Compilation Error in {file_name}.cpp ⚠️")


@change_directory_to_root
def run_file(
    file_name: str, input_file_name: str, output_file_name: str, time_limit: float = 2.0
):
    file_path = os.path.join("cpp_src", file_name + get_executable_extension())
    input_path = os.path.join("test_data", input_file_name) if input_file_name else None
    output_path = (
        os.path.join("test_data", output_file_name) if output_file_name else None
    )

    if not os.path.isfile(file_path):
        raise FileNotFoundError(f"Executable '{file_path}' not found.")
    if input_path and not os.path.isfile(input_path):
        raise FileNotFoundError(f"Input file '{input_path}' not found.")

    with open(input_path, "r") if input_path else nullcontext() as infile, (
        open(output_path, "w") if output_path else nullcontext()
    ) as outfile:
        command = [file_path]
        result = subprocess.run(
            command,
            check=True,
            timeout=time_limit,
            stdin=infile,
            stdout=outfile,
            stderr=subprocess.PIPE,
            text=True,
        )
    return result


def normalize_output(output_file: str) -> list[str]:
    output_path = os.path.join(os.getcwd(), "test_data", output_file)
    if not os.path.isfile(output_path):
        return []
    with open(output_path, "r") as file:
        return [token for line in file for token in line.strip().split()]


@change_directory_to_root
def check_diff(output1: str, output2: str) -> bool:
    return normalize_output(output1) == normalize_output(output2)


@change_directory_to_root
def validate_errors(
    file_name: str,
    input_file_name: str,
    output_file_name: str,
    test: int,
    time_limit: float = 2.0,
):
    try:
        return run_file(file_name, input_file_name, output_file_name, time_limit)
    except FileNotFoundError as e:
        raise ValueError(f"File not found: {e}")
    except subprocess.CalledProcessError:
        raise ValueError(f"Runtime Error in {file_name}.cpp on test case {test} ⚠️")
    except subprocess.TimeoutExpired:
        raise ValueError(
            f"Time Limit Exceeded in {file_name}.cpp on test case {test} ⏳"
        )


@change_directory_to_root
def print_output(output_file: str) -> None:
    output_path = os.path.join("test_data", output_file)
    if not os.path.isfile(output_path):
        raise FileNotFoundError(f"File '{output_path}' not found.")
    with open(output_path, "r") as file:
        print(file.read())
