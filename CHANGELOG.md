# Changelog

## Unreleased

- Treat unexpected standard and non-standard exceptions as individual test failures,
  report them, and continue running the remaining tests.

- Add CMake targets, presets, installation and package discovery, and independent
  CTest regression coverage while retaining C++11 compatibility.
- Move the public header to `include/microtest/microtest.h`.
- Replace the Travis/shell harness with GitHub Actions, cross-platform builds,
  formatting checks, and sanitizer tests.
- Fix `ASSERT_STRNEQ`, repeated argument evaluation, expression precedence, and
  assertion use in conditional statements.
- Preserve the full failure count in `RunAllTests()` and summary percentages;
  return exit status 1 on test failures.
