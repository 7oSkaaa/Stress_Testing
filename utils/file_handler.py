import functools
import os


def change_directory_to_root(func):
    @functools.wraps(func)
    def wrapper_change_directory_to_root(*args, **kwargs):
        current_directory = os.getcwd()
        root_directory = os.path.dirname(os.path.dirname(os.path.realpath(__file__)))

        try:
            os.chdir(root_directory)
            return func(*args, **kwargs)
        finally:
            os.chdir(current_directory)

    return wrapper_change_directory_to_root


@change_directory_to_root
def delete_file(file_path: str) -> None:
    full_path = os.path.join(os.getcwd(), file_path)
    if os.path.isfile(full_path):
        os.remove(full_path)


def clean_up():
    delete_file(os.path.join("cpp_src", "generator"))
    delete_file(os.path.join("cpp_src", "correct"))
    delete_file(os.path.join("cpp_src", "test"))
