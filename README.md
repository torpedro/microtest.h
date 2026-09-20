# microtest.h

A small, dependency-free, header-only C++11 unit testing framework. Copy
[`include/microtest/microtest.h`](include/microtest/microtest.h) into your project,
or use the CMake target `microtest::microtest`.

```cpp
#include <microtest/microtest.h>

TEST(Addition) {
  ASSERT_EQ(2 + 2, 4);
  ASSERT_STRNEQ("hello", "world");
}

TEST_MAIN();
```

Define `TEST_MAIN()` in exactly one source file. Tests can live in multiple source
files linked into the same executable; give each test a unique name. See
[`examples/basic.cpp`](examples/basic.cpp) for all assertions.

## Requirements

- C++11 or newer, with exceptions enabled.
- No runtime libraries beyond the C++ standard library.
- For repository development: CMake 3.23+ and Ninja (used by the presets).
  Other CMake generators work with a manual configure command.

Copying the header does not require CMake, Ninja, or Python.

## Integrating with CMake

As a source subdirectory:

```cmake
add_subdirectory(external/microtest)
add_executable(my_tests tests.cpp)
target_link_libraries(my_tests PRIVATE microtest::microtest)
```

`FetchContent_MakeAvailable(microtest)` also exposes the same target. Pin the
repository to a reviewed commit or release when using FetchContent. Tests and
examples are disabled by default when microtest is included as a subproject.
The library does not impose warning or sanitizer flags on consumers.

To install a package:

```sh
cmake -S . -B build/package -DMICROTEST_BUILD_TESTS=OFF -DMICROTEST_BUILD_EXAMPLES=OFF
cmake --install build/package --prefix /your/install/prefix
```

Then configure the consumer with `-DCMAKE_PREFIX_PATH=/your/install/prefix`:

```cmake
find_package(microtest 0.1 CONFIG REQUIRED)
add_executable(my_tests tests.cpp)
target_link_libraries(my_tests PRIVATE microtest::microtest)
```

## Assertions and execution

| Macro | Passes when |
| --- | --- |
| `ASSERT(condition)`, `ASSERT_TRUE(condition)` | The condition is true |
| `ASSERT_FALSE(condition)` | The condition is false |
| `ASSERT_NULL(value)` | The pointer is null |
| `ASSERT_NOTNULL(value)` | The pointer is not null |
| `ASSERT_EQ(a, b)` | `a == b` |
| `ASSERT_NEQ(a, b)` | `a != b` |
| `ASSERT_STREQ(a, b)` | String contents are equal |
| `ASSERT_STRNEQ(a, b)` | String contents differ |

Each argument is evaluated once. Assertions work as single statements, including
in unbraced `if`/`else` branches. C++11 does not specify the evaluation order
between two arguments; do not modify the same object in both operands.
Equality operands must support the relevant comparison and stream insertion for
diagnostics. String assertions accept `std::string` and non-null C strings.

A failed assertion throws `mt::AssertFailedException`, ends the current test,
and reports its expression, source file, and line. The runner continues with the
remaining tests. Other `std::exception` instances fail the current test and report
their `what()` message; non-standard exceptions fail it with a generic diagnostic.
The runner continues after either kind of exception. Crashes are not recovered.
Tests run sequentially, and registration order across source files is unspecified.

`TEST_MAIN()` returns **0** when all tests pass (including an empty suite), and
**1** when assertions fail or tests throw. `mt::TestsManager::RunAllTests()` returns the actual
failure count. `mt::Runtime::args()` exposes command-line arguments, including
the executable name. Output currently includes ANSI colors; assertion value
diagnostics use standard output even when a custom runner output file is supplied.

## Development

```sh
cmake --preset dev
cmake --build --preset dev
ctest --preset dev
```

The `dev` preset enforces C++11. Use `modern` for C++23, or `sanitizers` for
AddressSanitizer and UndefinedBehaviorSanitizer with GCC/Clang. Each preset has
its own build directory. `make test` wraps the `dev` workflow.

Tests independently check assertions, operand evaluation, expression precedence,
conditional use, source diagnostics, registration across translation units,
runner exit codes and summaries, and an installed, relocated CMake package.
The public header is also compiled without prerequisite includes.

Format C++ files with clang-format **18.1.8** using the checked-in configuration.
CI checks formatting and builds with GCC, Clang, Apple Clang, and MSVC, including
a C++23 job and a Linux sanitizer job.

Build options:

- `MICROTEST_BUILD_TESTS`: regression tests; defaults on for standalone builds.
- `MICROTEST_BUILD_EXAMPLES`: passing examples; defaults on for standalone builds.
- `MICROTEST_SANITIZERS`: development instrumentation; defaults off.

The header moved from `src/` to `include/`; update manual include search paths
from `-Isrc` to `-Iinclude`. The public include spelling remains unchanged.

## License

MIT; see [LICENSE](LICENSE).
