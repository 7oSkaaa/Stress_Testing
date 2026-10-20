import os
import sys

# FORCE_COLOR is for IDE run consoles that render colors but are not a TTY.
ENABLED = (sys.stdout.isatty() or "FORCE_COLOR" in os.environ) and "NO_COLOR" not in os.environ


def _code(code: int) -> str:
    return f"\033[{code}m" if ENABLED else ""


class Colors:
    reset = _code(0)
    bold = _code(1)

    class fg:
        red = _code(31)
        green = _code(32)
        gold = _code(33)
        blue = _code(34)
        purple = _code(35)
        cyan = _code(36)
        lightgrey = _code(37)
        darkgrey = _code(90)
        yellow = _code(93)
        lightblue = _code(94)
        pink = _code(95)
        white = _code(97)
