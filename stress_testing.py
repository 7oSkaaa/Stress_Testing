import argparse
import math
import os
import platform
import random
import sys
from dataclasses import dataclass
from itertools import count, zip_longest
from pathlib import Path
from typing import Optional

from src.utils import cpp_compiler
from src.utils.cpp_compiler import decode
from src.utils.terminal_colors import Colors

DATA_DIR = Path(__file__).resolve().parent / "data"
INPUT_FILE = DATA_DIR / "input.txt"
EXPECTED_FILE = DATA_DIR / "correct_output.txt"
OUTPUT_FILE = DATA_DIR / "test_output.txt"
CHECKER_SOURCE = cpp_compiler.CPP_DIR / "checker.cpp"
MAX_LINES = 50
MAX_LINE_WIDTH = 200
WARM_UP_SECONDS = 5.0

STATUS_STYLES = {
    "TIME LIMIT EXCEEDED": (Colors.fg.yellow, "⏳"),
    "WRONG ANSWER": (Colors.fg.red, "❌"),
    "RUNTIME ERROR": (Colors.fg.purple, "💥"),
}
BORDER = f"{Colors.fg.blue}{'═' * 60}{Colors.reset}"
SECTION = f"{Colors.fg.cyan}{'─' * 60}{Colors.reset}"
SUB_BORDER = f"{Colors.fg.lightblue}{'-' * 60}{Colors.reset}"


@dataclass
class Case:
    seed: int
    # None means the program producing this output never ran.
    input: Optional[bytes] = None
    expected: Optional[bytes] = None
    output: Optional[bytes] = None
    status: Optional[str] = None
    program: str = ""
    exit_info: str = ""
    stderr: str = ""
    checker_message: str = ""
    diff_line: Optional[int] = None
    test_seconds: float = 0.0


def tokens_match(expected: bytes, output: bytes, eps: Optional[float]) -> bool:
    expected_tokens, output_tokens = expected.split(), output.split()
    return len(expected_tokens) == len(output_tokens) and all(
        a == b or (eps is not None and floats_close(a, b, eps))
        for a, b in zip(expected_tokens, output_tokens)
    )


def floats_close(a: bytes, b: bytes, eps: float) -> bool:
    try:
        return math.isclose(float(a), float(b), rel_tol=eps, abs_tol=eps)
    except ValueError:
        return False


def first_difference(expected: bytes, output: bytes, eps: Optional[float]) -> Optional[int]:
    lines = zip_longest(expected.splitlines(), output.splitlines(), fillvalue=b"")
    return next((i for i, (e, o) in enumerate(lines) if not tokens_match(e, o, eps)), None)


def save_case(case: Case) -> None:
    DATA_DIR.mkdir(exist_ok=True)
    for path, data in ((INPUT_FILE, case.input), (EXPECTED_FILE, case.expected), (OUTPUT_FILE, case.output)):
        path.write_bytes(data or b"")


def record_failure(case: Case, program: str, run: cpp_compiler.Run) -> bool:
    if run.status is None:
        return False
    case.status, case.program, case.exit_info, case.stderr = run.status, program, run.exit_info, run.stderr
    return True


def judge(case: Case, binaries: dict, eps: Optional[float], time_limit: float) -> None:
    if "checker" not in binaries:
        if not tokens_match(case.expected, case.output, eps):
            case.status, case.program = "WRONG ANSWER", "test.cpp"
            case.diff_line = first_difference(case.expected, case.output, eps)
        return
    save_case(case)
    # testlib argument order: input, contestant output, jury answer.
    checked = cpp_compiler.run(binaries["checker"], b"", time_limit, str(INPUT_FILE), str(OUTPUT_FILE), str(EXPECTED_FILE))
    if checked.returncode is None or checked.returncode < 0:
        record_failure(case, "checker.cpp", checked)
    elif checked.returncode != 0:
        case.status, case.program = "WRONG ANSWER", "test.cpp"
        case.checker_message = (decode(checked.stdout) + checked.stderr).strip()


def run_test(binaries: dict, seed: int, time_limit: float, eps: Optional[float]) -> Case:
    case = Case(seed)
    helper_limit = max(10.0, 5 * time_limit)  # brute force and generator may be slow; only test.cpp is judged

    generated = cpp_compiler.run(binaries["generator"], b"", helper_limit, str(seed))
    case.input = generated.stdout
    if record_failure(case, "generator.cpp", generated):
        return case

    expected = cpp_compiler.run(binaries["correct"], case.input, helper_limit)
    case.expected = expected.stdout
    if record_failure(case, "correct.cpp", expected):
        return case

    result = cpp_compiler.run(binaries["test"], case.input, time_limit)
    case.output, case.stderr, case.test_seconds = result.stdout, result.stderr, result.seconds
    if record_failure(case, "test.cpp", result):
        return case

    judge(case, binaries, eps, helper_limit)
    return case


def shorten(line: str) -> str:
    return line if len(line) <= MAX_LINE_WIDTH else line[:MAX_LINE_WIDTH] + " ..."


def quoted_line(data: bytes, index: int) -> str:
    lines = decode(data).splitlines()
    return f'"{shorten(lines[index])}"' if index < len(lines) else "<missing line>"


def print_block(title: str, color: str, data, highlight: Optional[int] = None) -> None:
    print(f"{color}{Colors.bold}{title}{Colors.reset}")
    print(SUB_BORDER)
    text = data if isinstance(data, str) or data is None else decode(data)
    if text is None or not text.strip():
        print(f"{Colors.fg.darkgrey}{'<not run>' if text is None else '<no output>'}{Colors.reset}")
        return
    lines = text.splitlines()
    for i, line in enumerate(lines[:MAX_LINES]):
        line_color = f"{color}{Colors.bold}" if i == highlight else Colors.fg.white
        print(f"{line_color}{shorten(line)}{Colors.reset}")
    if len(lines) > MAX_LINES:
        print(f"{Colors.fg.darkgrey}... {len(lines) - MAX_LINES} more lines{Colors.reset}")


def print_report(case: Case, test: int) -> None:
    color, emoji = STATUS_STYLES[case.status]
    details = ", ".join(filter(None, [case.program, case.exit_info]))
    print(BORDER)
    print(f"{Colors.fg.purple}🧪 {Colors.bold}Test Case: {test}{Colors.reset}  {Colors.fg.darkgrey}seed {case.seed}{Colors.reset}")
    print(SECTION)
    print(f"{color}{Colors.bold}{emoji} Status:{Colors.reset}  [{color}{case.status}{Colors.reset}]  {Colors.fg.lightgrey}({details}){Colors.reset}")
    if case.diff_line is not None:
        print(
            f"{Colors.fg.white}First difference on line {case.diff_line + 1}: "
            f"expected {Colors.fg.green}{quoted_line(case.expected, case.diff_line)}{Colors.fg.white}, "
            f"got {Colors.fg.red}{quoted_line(case.output, case.diff_line)}{Colors.reset}"
        )
    if case.checker_message:
        print(f"{Colors.fg.white}Checker: {case.checker_message}{Colors.reset}")
    print(SECTION)
    print_block("📥 Input:", Colors.fg.yellow, case.input)
    print(SECTION)
    print_block("✅ Expected Output:", Colors.fg.green, case.expected, case.diff_line)
    print(SECTION)
    print_block("❌ Your Output:", Colors.fg.red, case.output, case.diff_line)
    if case.stderr.strip():
        print(SECTION)
        print_block(f"📢 Stderr ({case.program}):", Colors.fg.cyan, case.stderr)
    print(BORDER)


def rerun_command(args: argparse.Namespace, seed: int, time_limit: float) -> str:
    command = f"python stress_testing.py --seed {seed} --time {time_limit:g}"
    if args.eps is not None:
        command += f" --eps {args.eps:g}"
    if args.sanitize:
        command += " --sanitize"
    return command


def clear_progress() -> None:
    if sys.stdout.isatty():
        print("\r\033[K", end="")


def print_progress(test: int, tests: int, slowest: tuple, time_limit: float) -> None:
    if not sys.stdout.isatty():
        return
    total = f"/{tests}" if tests else ""
    seconds, slowest_test = slowest
    print(
        f"\r{Colors.fg.white}Test {test}{total} {Colors.fg.green}passed{Colors.reset}  "
        f"{Colors.fg.darkgrey}slowest {seconds:.2f}s of {time_limit:g}s (test {slowest_test}){Colors.reset}\033[K",
        end="",
        flush=True,
    )


def prepare_console() -> None:
    # Box drawing and emoji would crash print() on a console or pipe whose encoding cannot show them.
    if hasattr(sys.stdout, "reconfigure"):
        sys.stdout.reconfigure(errors="replace")
    if platform.system() == "Windows":
        import ctypes

        os.system("")  # turns on ANSI escape codes in the Windows console
        # Inherited by child processes: a crash exits with an NTSTATUS code instead of opening
        # a "stopped working" dialog that would stall until the time limit.
        ctypes.windll.kernel32.SetErrorMode(0x0001 | 0x0002)  # SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Stress test cpp/test.cpp against cpp/correct.cpp on tests from cpp/generator.cpp.")
    parser.add_argument("--tests", type=int, help="number of tests, 0 runs until a test fails")
    parser.add_argument("--time", type=float, help="time limit for test.cpp in seconds")
    parser.add_argument("--seed", type=int, help="run only the test generated from this seed")
    parser.add_argument("--eps", type=float, help="accept numbers within this absolute or relative error")
    parser.add_argument("--sanitize", action="store_true", help="build with AddressSanitizer and UBSan (slower; not on MinGW)")
    return parser.parse_args()


def main() -> int:
    prepare_console()
    args = parse_args()
    try:
        tests = 1 if args.seed is not None else args.tests
        if tests is None:
            tests = int(input(f"{Colors.fg.gold}Number of tests (0 = until one fails): {Colors.reset}"))
        time_limit = args.time
        if time_limit is None:
            time_limit = float(input(f"{Colors.fg.gold}Time limit for test.cpp (seconds): {Colors.reset}"))
    except (ValueError, EOFError):
        print(f"{Colors.fg.red}Invalid input.{Colors.reset}")
        return 1
    if tests < 0 or time_limit <= 0:
        print(f"{Colors.fg.red}The number of tests must be >= 0 and the time limit > 0.{Colors.reset}")
        return 1
    if args.sanitize:
        # LeakSanitizer (on by default on Linux) would turn every unfreed `new` into a runtime error.
        os.environ.setdefault("ASAN_OPTIONS", "detect_leaks=0")

    names = ["generator", "correct", "test"] + (["checker"] if CHECKER_SOURCE.is_file() else [])
    flags = " ".join(cpp_compiler.compile_flags(args.sanitize))
    print(f"{Colors.fg.darkgrey}Compiling with {cpp_compiler.find_compiler()} {flags}{Colors.reset}")
    try:
        binaries = {name: cpp_compiler.compile_file(name, args.sanitize) for name in names}
    except cpp_compiler.CompilationError as e:
        print(f"{Colors.fg.red}{e}{Colors.reset}")
        return 1
    # macOS (and antivirus on Windows) can hold a new binary for seconds on its first launch
    # (measured up to 3s); pay that before anything is timed. Killed launches still count.
    for name in ("generator", "correct", "test"):
        cpp_compiler.run(binaries[name], b"", WARM_UP_SECONDS)

    slowest = (0.0, 0)
    for test in range(1, tests + 1) if tests else count(1):
        seed = args.seed if args.seed is not None else random.getrandbits(32)
        case = run_test(binaries, seed, time_limit, args.eps)
        slowest = max(slowest, (case.test_seconds, test))
        if case.status:
            clear_progress()
            save_case(case)
            print_report(case, test)
            print(f"{Colors.fg.darkgrey}Saved to data/. Rerun this test: {rerun_command(args, seed, time_limit)}{Colors.reset}")
            return 1
        print_progress(test, tests, slowest, time_limit)

    clear_progress()
    print(
        f"{Colors.fg.green}✅ Passed {tests} test{'s' if tests != 1 else ''}.{Colors.reset} "
        f"{Colors.fg.darkgrey}Slowest test.cpp run: {slowest[0]:.2f}s on test {slowest[1]} (limit {time_limit:g}s).{Colors.reset}"
    )
    return 0


if __name__ == "__main__":
    try:
        sys.exit(main())
    except KeyboardInterrupt:
        print(f"\n{Colors.fg.red}Interrupted.{Colors.reset}")
        sys.exit(130)
