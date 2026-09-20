#ifndef MICROTEST_TEST_CHECK_H
#define MICROTEST_TEST_CHECK_H
#include <cstdio>
#include <cstring>
#include <exception>

#include "microtest/microtest.h"

inline void check(bool condition, const char* message, int& failures) {
  if (!condition) {
    std::fprintf(stderr, "CHECK FAILED: %s\n", message);
    ++failures;
  }
}

template <typename Function>
void expectAssertion(Function fn, bool expected, int& failures) {
  bool caught = false;
  try {
    fn();
  } catch (const mt::AssertFailedException& error) {
    caught = true;
    check(std::strlen(error.what()) != 0, "assertion description", failures);
  } catch (...) {
    check(false, "unexpected exception type", failures);
    return;
  }
  check(caught == expected, expected ? "expected assertion failure" : "expected success", failures);
}
#endif
