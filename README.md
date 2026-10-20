# Stress Testing

## Overview

Stress Testing is a tool for competitive programming and algorithm development. It checks your C++ solution against a brute-force solution on random tests, and stops at the first input where they disagree, crash, or run out of time.

## Features

- Modular C++ test generators for numbers, arrays, strings and graphs
- Clear reports for Wrong Answer, Time Limit Exceeded and Runtime Error, with the input, both outputs, stderr, the crash signal and the first differing line
- Every test has a seed, so a failing test can be replayed with `--seed`
- `--sanitize` builds with AddressSanitizer and UBSan to catch overflow and out-of-bounds bugs
- `--eps` for floating-point answers, and an optional `cpp/checker.cpp` for problems with more than one valid answer
- Recompiles only when a source file changes
- Cross-platform: macOS, Linux and Windows

## Requirements

- Python 3.9+
- GCC (`g++`). The templates include `<bits/stdc++.h>`, which only GCC ships. Set the `CXX` environment variable to use another compiler.
- CMake (optional, only for building inside an IDE)

## Installation Requirements

### Python 3

- macOS: Pre-installed, or install via Homebrew:

  ```sh
  brew install python3
  ```

- Linux (Debian/Ubuntu):

  ```sh
  sudo apt update && sudo apt install python3 python3-pip
  ```

- Windows: [Download Python](https://www.python.org/downloads/) and add it to PATH.

### C++ Compiler

- macOS: Apple's `g++` is really Clang, so install GCC from Homebrew. The script picks the newest `g++-<version>` on your PATH.

  ```sh
  brew install gcc
  ```

- Linux (Debian/Ubuntu):

  ```sh
  sudo apt update && sudo apt install build-essential g++
  ```

- Windows: Install [MinGW-w64](http://mingw-w64.org/doku.php/download) or use [WSL](https://docs.microsoft.com/en-us/windows/wsl/). MinGW has no sanitizers, so `--sanitize` needs WSL.

### CMake (Optional, for CMake builds)

- macOS:

  ```sh
  brew install cmake
  ```

- Linux:

  ```sh
  sudo apt install cmake
  ```

- Windows: [Download CMake](https://cmake.org/download/) and add it to PATH.

## Installation

```bash
git clone https://github.com/7oSkaaa/stress_testing.git
cd stress_testing
```

## Folder Structure

```text
📦 Stress_Testing
├── 📄 CMakeLists.txt
├── 📄 README.md
├── 🐍 stress_testing.py
├── 🗂️ cpp/
│   ├── 📄 correct.cpp
│   ├── 📄 generator.cpp
│   ├── 📄 test.cpp
│   ├── 📄 checker.cpp        (optional)
│   └── 🗂️ generators/
│       ├── 📄 generator_arrays.h
│       ├── 📄 generator_graphs.h
│       ├── 📄 generator_numbers.h
│       ├── 📄 generator_strings.h
│       └── 📄 generator_utils.h
├── 🗂️ src/
│   ├── 📄 __init__.py
│   └── 🗂️ utils/
│       ├── 📄 __init__.py
│       ├── 📄 cpp_compiler.py
│       └── 📄 terminal_colors.py
├── 🗂️ bin/                   (generated: compiled programs)
└── 🗂️ data/                  (generated: the last failing test)
```

## How to Use

### 1. Prepare Your Solutions

- Place your brute-force solution in `cpp/correct.cpp`.
- Place your optimized solution in `cpp/test.cpp`.
- Edit `cpp/generator.cpp` to generate tests with the generators from `cpp/generators/`.

### 2. Write the Generator

The script passes a seed as the generator's first argument. Seed the random engine from it, so the script can replay a failing test:

```cpp
#include <bits/stdc++.h>
#include "generators/generator_numbers.h"
#include "generators/generator_arrays.h"
#include "generators/generator_strings.h"
#include "generators/generator_graphs.h"
#include "generators/generator_utils.h"

int main(int argc, char* argv[]) {
    std::mt19937_64 rng(argc > 1 ? std::stoull(argv[1]) : std::chrono::steady_clock::now().time_since_epoch().count());
    int n = gen_numbers::random<int>(5, 15, rng);
    auto arr = gen_arrays::random<int>(n, 1, 100, rng);
    gen_utils::print(arr);
    auto s = gen_strings::random(n, gen_strings::CaseType::Lower, rng);
    std::cout << s << '\n';
    auto tree = gen_graphs::tree(n, rng);
    gen_utils::print(tree);
    return 0;
}
```

Keep tests small. The brute force has to finish on every one of them.

### 3. Run the Stress Test

```bash
python stress_testing.py --tests 100 --time 2
```

- `--tests N`: number of tests. `0` keeps running until a test fails. If you leave it out, the script asks.
- `--time S`: time limit for `test.cpp` in seconds. The generator and the brute force get `max(10, 5 × S)` seconds, because they are allowed to be slow. If you leave it out, the script asks.
- `--seed N`: run only the test generated from seed `N`. A failure prints the full command for you.
- `--eps E`: accept numbers whose absolute or relative error is at most `E`.
- `--sanitize`: build with `-fsanitize=address,undefined`. Programs run a few times slower, so raise `--time` if needed. Memory leaks are not reported, since unfreed memory is normal in contest code. Set `ASAN_OPTIONS` yourself to change that.

The programs compile with `-std=c++17 -O2` into `bin/`. The script exits with code 0 when every test passes and 1 on a failure or a compile error.

### 4. Output

While tests pass, a single line updates in place with the test count and the slowest `test.cpp` run so far. At the first failure the script prints:

- the status (`WRONG ANSWER`, `TIME LIMIT EXCEEDED` or `RUNTIME ERROR`), the program that failed, and how it ended: a signal such as `SIGSEGV` on Linux and macOS, or a crash name such as `ACCESS_VIOLATION` or `STACK_OVERFLOW` on Windows
- the input, the expected output and your output, with the first differing line highlighted
- the stderr of the program that failed
- the command that reruns this exact test

The failing test is also saved to `data/input.txt`, `data/correct_output.txt` and `data/test_output.txt`.

Colors turn off when the output is not a terminal or when the `NO_COLOR` environment variable is set. Set `FORCE_COLOR` to keep them in an IDE run window that shows colors but is not a terminal.

### 5. Custom Checker (Optional)

When `cpp/checker.cpp` exists, it decides each verdict instead of the token comparison. It is called with the same arguments as a testlib checker, so testlib checkers work unchanged:

```text
checker <input file> <your output file> <expected output file>
```

Exit code 0 means accepted. Any other exit code is a Wrong Answer, and whatever the checker prints is shown as the reason. For example, this checker accepts any answer that is at least the expected one:

```cpp
#include <bits/stdc++.h>

int main(int argc, char* argv[]) {
    std::ifstream out(argv[2]), ans(argv[3]);
    long double yours, expected;
    if (!(out >> yours)) { std::cout << "no output"; return 1; }
    ans >> expected;
    if (yours < expected) { std::cout << std::fixed << "expected at least " << expected << ", got " << yours; return 1; }
    return 0;
}
```

### 6. Building with CMake (Optional)

```bash
cmake -S . -B build
cmake --build build
```

This builds generator, test, correct and checker (if present) into `build/` for use inside an IDE. The Python script does not use it.

CMake must use GCC, and stops with a message if the compiler has no `<bits/stdc++.h>`. CMake picks Apple Clang on macOS and Visual Studio on Windows by default, so choose GCC there. Delete `build/` first if a previous configure used another compiler:

```bash
cmake -S . -B build -DCMAKE_CXX_COMPILER=g++-16   # macOS, use your Homebrew GCC version
cmake -S . -B build -G "MinGW Makefiles"           # Windows with MinGW
```

In CLion, select a GCC toolchain under Settings, Build, Execution, Deployment, Toolchains.

## C++ Generator Libraries Reference

### generator_numbers.h

- `random < T > (T l, T r, rng)`: Random integer in [l, r].
- `random_range < T > (T l, T r, int count, rng)`: Vector of random integers in [l, r].
- `random_exclude < T > (T l, T r, set < T > exclude, rng)`: Random integer in [l, r] not in exclude.
- `random_range_exclude < T > (T l, T r, int count, set < T > exclude, rng)`: Vector of random integers not in exclude.
- `random_weighted < T > (vector < T > values, vector < double > weights, rng)`: Random value from values with custom weights.
- `random_real < T > (T l, T r, rng)`: Random floating-point in [l, r].
- `random_real_exclude < T > (T l, T r, T excl_l, T excl_r, rng)`: Random real in [l, r] excluding [excl_l, excl_r].

### generator_arrays.h

- `random < T > (int len, T l, T r, rng, unique, sorted)`: Random array of length len in [l, r]. With `unique`, len must be at most r - l + 1.
- `permutation < T > (int n, rng)`: Random permutation of 1..n.
- `matrix < T > (int rows, int cols, T l, T r, rng, unique_rows, sorted_rows)`: 2D array.
- `pairs < T > (int len, T l1, T r1, T l2, T r2, rng, ordered)`: Vector of pairs.
- `subset < T > (T l, T r, int k, rng, sorted)`: Unique subset of size k from [l, r].
- `random_with_sum < T > (int len, T sum, T min_val, T max_val, rng)`: Random array with exactly the given sum, each element in [min_val, max_val].
- `partition < T > (T sum, int k, T min_val, T max_val, rng)`: Same as `random_with_sum` with the arguments in a different order.
- `arithmetic_progression < T > (int len, T start, T step)`: Arithmetic progression.
- `geometric_progression < T > (int len, T start, T ratio)`: Geometric progression.
- `constant_array < T > (int len, T value)`: Array with all elements equal.
- `bit_array < T > (int len, double prob_one, rng)`: Bit array (0/1) with probability for 1s.
- `shuffled < T > (vector < T > v, rng)`: Shuffled copy of v.
- `strictly_increasing < T > (int len, T start, T step_min, T step_max, rng)`: Strictly increasing array.
- `strictly_decreasing < T > (int len, T start, T step_min, T step_max, rng)`: Strictly decreasing array.

### generator_strings.h

- `random (int len, CaseType, rng, char l, char r)`: Random string of letters in [l, r], in the given case.
- `palindrome (int len, CaseType, rng, char l, char r)`: Palindrome string.
- `random_alphanum (int len, bool letters, bool digits, CaseType, rng)`: Alphanumeric string.
- `random_custom (int len, string alphabet, rng)`: String from custom alphabet.
- `random_strings (int count, int len, CaseType, rng)`: Vector of random strings.
- `palindromes (int count, int len, CaseType, rng)`: Vector of palindromes.

### generator_graphs.h

Edge lists come out shuffled. Functions that cannot build the requested graph stop with a failed `assert`.

- `tree (int n, rng)`: Random tree (edges).
- `simple_graph (int n, int m, rng)`: Connected graph without self-loops or multi-edges, n - 1 <= m <= n(n - 1)/2.
- `weighted_graph (int n, int m, int min_w, int max_w, rng)`: Weighted `simple_graph`.
- `directed_graph (int n, int m, rng)`: Directed graph without self-loops or multi-edges.
- `dag (int n, int m, rng)`: Directed acyclic graph.
- `bipartite (int n1, int n2, int m, rng)`: Bipartite graph.
- `star (int n, rng, int center)`: Star graph.
- `cycle (int n, rng)`: Cycle graph, n >= 3.
- `complete (int n)`: Complete graph.
- `regular (int n, int d, rng)`: Random simple d-regular graph, for any d < n with n·d even.
- `tree_with_diameter (int n, int diameter, rng)`: Tree with exactly the given diameter (capped at n - 1).
- `chain_tree (int n)`: Chain tree (path).

### generator_utils.h

- `print (const vector < T > &v)`: Print vector.
- `print (const vector < pair < T, U > > &v)`: Print vector of pairs.
- `print (const vector < vector < T > > &mat)`: Print 2D vector.
- `print (const vector < tuple < Args... > > &v)`: Print vector of tuples.
- `print (const T &val)`: Print single value.
- `print (const vector < string > &v)`: Print vector of strings.

## Notes

- Set the compiler with the `CXX` environment variable. Otherwise the script uses `g++`, or on macOS the newest `g++-<version>` it finds.
- The project is modular and easy to extend for your own needs.
