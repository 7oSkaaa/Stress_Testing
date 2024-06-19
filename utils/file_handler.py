import functools
import os


def change_directory_to_root(func):
    @functools.wraps(func)
    def wrapper_change_directory_to_root(*args, **kwargs):
        current_directory = os.getcwd()
        root_directory = os.path.dirname(os.path.dirname(os.path.realpath(__file__)))

        try:
            os.chdir(root_directory)
            result = func(*args, **kwargs)
        finally:
            os.chdir(current_directory)

        return result

    return wrapper_change_directory_to_root


@change_directory_to_root
def delete_file(file: str) -> None:
    directory = os.getcwd()
    try:
        os.remove(os.path.join(directory, file))
    except OSError:
        pass
    
    
def clean_up():
    delete_file("cpp_files/generator")
    delete_file("cpp_files/correct")
    delete_file("cpp_files/test")
