import os
import platform
import signal
import subprocess
import time
from dataclasses import dataclass
from pathlib import Path
from typing import Optional

ROOT = Path(__file__).resolve().parents[2]
CPP_DIR = ROOT / "cpp"
BIN_DIR = ROOT / "bin"
EXE_SUFFIX = ".exe" if platform.system() == "Windows" else ""
FLAGS = ["-std=c++17", "-O2"]
SANITIZE_FLAGS = ["-fsanitize=address,undefined", "-fno-sanitize-recover=all", "-g"]

# Windows reports crashes as NTSTATUS exit codes instead of signals.
WINDOWS_CRASHES = {
    0xC0000005: "ACCESS_VIOLATION",
    0xC000001D: "ILLEGAL_INSTRUCTION",
    0xC0000094: "INTEGER_DIVIDE_BY_ZERO",
    0xC00000FD: "STACK_OVERFLOW",
    0xC0000409: "STACK_BUFFER_OVERRUN",
}


class CompilationError(Exception):
    pass


def decode(data: Optional[bytes]) -> str:
    # backslashreplace shows invalid bytes as \xff, so outputs differing only in such bytes still look different.
    return (data or b"").decode("utf-8", errors="backslashreplace")


@dataclass
class Run:
    stdout: bytes
    stderr: str
    returncode: Optional[int]  # None when the program was killed for exceeding the time limit
    seconds: float

    @property
    def status(self) -> Optional[str]:
        if self.returncode is None:
            return "TIME LIMIT EXCEEDED"
        if self.returncode != 0:
            return "RUNTIME ERROR"
        return None

    @property
    def exit_info(self) -> str:
        if self.returncode is None:
            return f"killed after {self.seconds:.2f}s"
        if self.returncode < 0:
            try:
                return signal.Signals(-self.returncode).name
            except ValueError:
                return f"signal {-self.returncode}"
        if self.returncode in WINDOWS_CRASHES:
            return WINDOWS_CRASHES[self.returncode]
        hex_code = f" ({self.returncode:#x})" if self.returncode > 255 else ""
        return f"exit code {self.returncode}{hex_code}"


def find_compiler() -> str:
    """Return $CXX, else the newest g++-<version> on PATH on macOS, else g++."""
    if os.environ.get("CXX"):
        return os.environ["CXX"]
    if platform.system() == "Darwin":
        # Apple's g++ is Clang, which has no <bits/stdc++.h>; Homebrew GCC installs as g++-<version>.
        versioned = [
            path
            for directory in os.environ.get("PATH", "").split(os.pathsep)
            for path in Path(directory).glob("g++-*")
            if path.name[4:].isdigit()
        ]
        if versioned:
            return max(versioned, key=lambda path: int(path.name[4:])).name
    return "g++"


def compile_flags(sanitize: bool) -> list:
    return FLAGS + (SANITIZE_FLAGS if sanitize else [])


def compile_command(name: str, sanitize: bool) -> list:
    # Paths relative to ROOT: shorter error messages, and MinGW cannot open some non-ASCII absolute paths.
    return [find_compiler(), *compile_flags(sanitize), f"cpp/{name}.cpp", "-o", f"bin/{name}{EXE_SUFFIX}"]


def _is_up_to_date(binary: Path, stamp: Path, source: Path, command: list) -> bool:
    if not binary.is_file() or not stamp.is_file() or stamp.read_text(encoding="utf-8") != " ".join(command):
        return False
    newest_source = max(path.stat().st_mtime for path in [source, *CPP_DIR.rglob("*.h")])
    return binary.stat().st_mtime >= newest_source


def _hint(stderr: str, sanitize: bool) -> str:
    if "bits/stdc++.h" in stderr:
        return "\nHint: this compiler has no <bits/stdc++.h>. Install GCC (macOS: brew install gcc) or set CXX."
    if sanitize and ("-lasan" in stderr or "-lubsan" in stderr or "sanitize" in stderr):
        return "\nHint: this compiler has no sanitizers (MinGW never does). Run without --sanitize, or use Linux, macOS or WSL."
    return ""


def compile_file(name: str, sanitize: bool = False) -> Path:
    """Compile cpp/<name>.cpp into bin/, skipping the build when the binary is newer than every source."""
    source = CPP_DIR / f"{name}.cpp"
    binary = BIN_DIR / (name + EXE_SUFFIX)
    stamp = binary.with_name(binary.name + ".args")
    command = compile_command(name, sanitize)
    if not source.is_file():
        raise CompilationError(f"Source file 'cpp/{name}.cpp' not found.")
    if _is_up_to_date(binary, stamp, source, command):
        return binary

    BIN_DIR.mkdir(exist_ok=True)
    try:
        result = subprocess.run(command, cwd=ROOT, capture_output=True)
    except FileNotFoundError:
        raise CompilationError(f"Compiler '{command[0]}' not found. Install g++ or set the CXX environment variable.")
    if result.returncode != 0:
        stderr = decode(result.stderr)
        raise CompilationError(f"Compilation error in {name}.cpp\n{stderr}{_hint(stderr, sanitize)}")
    stamp.write_text(" ".join(command), encoding="utf-8")
    return binary


def run(binary: Path, stdin: bytes, time_limit: float, *args: str) -> Run:
    """Run a program on raw bytes, so tests pass between programs unchanged on every OS."""
    start = time.perf_counter()
    try:
        result = subprocess.run([str(binary), *args], input=stdin, capture_output=True, timeout=time_limit)
    except subprocess.TimeoutExpired as e:
        return Run(e.stdout or b"", decode(e.stderr), None, time.perf_counter() - start)
    return Run(result.stdout, decode(result.stderr), result.returncode, time.perf_counter() - start)
