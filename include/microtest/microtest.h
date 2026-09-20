//
// microtest.h
//
// URL: https://github.com/torpedro/microtest.h
// Author: Pedro Flemming (http://torpedro.com/)
// License: MIT License (https://github.com/torpedro/microtest.h/blob/master/LICENSE)
// Copyright (c) 2017 Pedro Flemming
//
// This is a small header-only C++ unit testing framework.
// It allows to define small unit tests with set of assertions available.
//
#ifndef MICROTEST_MICROTEST_H
#define MICROTEST_MICROTEST_H

#include <cstddef>
#include <cstdio>
#include <exception>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

////////////////
// Assertions //
////////////////

#define ASSERT(cond) ASSERT_TRUE(cond)

#define ASSERT_TRUE(cond)                                                    \
  do {                                                                       \
    if (!(cond)) throw mt::AssertFailedException(#cond, __FILE__, __LINE__); \
  } while (0)

#define ASSERT_FALSE(cond) ASSERT_TRUE(!(cond))
#define ASSERT_NULL(value)                                                       \
  do {                                                                           \
    if (!mt::detail::isNull((value)))                                            \
      throw mt::AssertFailedException(#value " == nullptr", __FILE__, __LINE__); \
  } while (0)
#define ASSERT_NOTNULL(value)                                                    \
  do {                                                                           \
    if (mt::detail::isNull((value)))                                             \
      throw mt::AssertFailedException(#value " != nullptr", __FILE__, __LINE__); \
  } while (0)

#define ASSERT_EQ(a, b)                                                  \
  do {                                                                   \
    mt::detail::assertEqual((a), (b), #a " == " #b, __FILE__, __LINE__); \
  } while (0)

#define ASSERT_NEQ(a, b)                                                    \
  do {                                                                      \
    mt::detail::assertNotEqual((a), (b), #a " != " #b, __FILE__, __LINE__); \
  } while (0)

#define ASSERT_STREQ(a, b)                                                                     \
  do {                                                                                         \
    mt::detail::assertEqual(std::string(a), std::string(b), #a " == " #b, __FILE__, __LINE__); \
  } while (0)

#define ASSERT_STRNEQ(a, b)                                                                       \
  do {                                                                                            \
    mt::detail::assertNotEqual(std::string(a), std::string(b), #a " != " #b, __FILE__, __LINE__); \
  } while (0)

////////////////
// Unit Tests //
////////////////

#define TEST(name)                                                                 \
  void name();                                                                     \
  namespace {                                                                      \
  const bool microtest_registered_##name = mt::TestsManager::AddTest(name, #name); \
  }                                                                                \
  void name()

///////////////
// Framework //
///////////////

namespace mt {

inline const char* red() { return "\033[1;31m"; }

inline const char* green() { return "\033[0;32m"; }

inline const char* yellow() { return "\033[0;33m"; }

inline const char* def() { return "\033[0m"; }

inline void printRunning(const char* message, std::FILE* file = stdout) {
  std::fprintf(file, "%s{ running}%s %s\n", green(), def(), message);
}

inline void printOk(const char* message, std::FILE* file = stdout) {
  std::fprintf(file, "%s{      ok}%s %s\n", green(), def(), message);
}

inline void printFailed(const char* message, std::FILE* file = stdout) {
  std::fprintf(file, "%s{  failed} %s%s\n", red(), message, def());
}

// Exception that is thrown when an assertion fails.
class AssertFailedException : public std::exception {
 public:
  AssertFailedException(std::string description, std::string filepath, int line,
                        std::string details = std::string())
      : std::exception(),
        description_(description),
        filepath_(filepath),
        line_(line),
        details_(details) {};

  const char* what() const noexcept override { return description_.c_str(); }

  inline const char* getFilepath() const { return filepath_.c_str(); }

  inline int getLine() const { return line_; }

  inline const char* getDetails() const { return details_.c_str(); }

 protected:
  std::string description_;
  std::string filepath_;
  int line_;
  std::string details_;
};

namespace detail {
// The overload accepts nullptr and legacy null pointer constants (NULL or 0).
inline bool isNull(std::nullptr_t) { return true; }

template <typename T>
auto isNull(const T& value) -> decltype(value == nullptr) {
  return value == nullptr;
}

template <typename T>
auto printValue(std::ostream& stream, const T& value, int) -> decltype(stream << value, void()) {
  stream << value;
}

template <typename T>
void printValue(std::ostream& stream, const T&, long) {
  stream << "<unprintable>";
}

template <typename A, typename B>
std::string comparisonDetails(const A& a, const B& b, const char* relation) {
  std::ostringstream stream;
  stream << "Actual values: ";
  printValue(stream, a, 0);
  stream << relation;
  printValue(stream, b, 0);
  return stream.str();
}

template <typename A, typename B>
void assertEqual(const A& a, const B& b, const char* description, const char* file, int line) {
  if (!(a == b)) {
    throw AssertFailedException(description, file, line, comparisonDetails(a, b, " != "));
  }
}

template <typename A, typename B>
void assertNotEqual(const A& a, const B& b, const char* description, const char* file, int line) {
  if (!(a != b)) {
    throw AssertFailedException(description, file, line, comparisonDetails(a, b, " == "));
  }
}
}  // namespace detail

class TestsManager {
  // Note: static initialization fiasco
  // http://www.parashift.com/c++-faq-lite/static-init-order.html
  // http://www.parashift.com/c++-faq-lite/static-init-order-on-first-use.html
 public:
  struct Test {
    const char* name;
    void (*fn)(void);
  };

  static std::vector<Test>& tests() {
    static std::vector<Test> tests_;
    return tests_;
  }

  // Adds a new test to the current set of tests.
  // Returns true after registration.
  inline static bool AddTest(void (*fn)(void), const char* name) {
    tests().push_back({name, fn});
    return true;
  }

  // Run all tests that are registered.
  // Returns the number of tests that failed.
  inline static std::size_t RunAllTests(std::FILE* file = stdout) {
    std::size_t num_failed = 0;

    for (const Test& test : tests()) {
      // Run the test.
      // Any exception escaping the test counts as one failure.
      try {
        printRunning(test.name, file);

        (*test.fn)();

        printOk(test.name, file);

      } catch (const AssertFailedException& e) {
        printFailed(test.name, file);
        if (e.getDetails()[0] != '\0') {
          std::fprintf(file, "%s{    info} %s%s\n", yellow(), def(), e.getDetails());
        }
        std::fprintf(file, "           %sAssertion failed: %s%s\n", red(), e.what(), def());
        std::fprintf(file, "           %s%s:%d%s\n", red(), e.getFilepath(), e.getLine(), def());
        ++num_failed;
      } catch (const std::exception& e) {
        printFailed(test.name, file);
        std::fprintf(file, "           %sUnexpected exception: %s%s\n", red(), e.what(), def());
        ++num_failed;
      } catch (...) {
        printFailed(test.name, file);
        std::fprintf(file, "           %sUnexpected non-standard exception%s\n", red(), def());
        ++num_failed;
      }
    }

    return num_failed;
  }
};

// Class that will capture the arguments passed to the program.
class Runtime {
 public:
  static const std::vector<std::string>& args(int argc = -1, char** argv = NULL) {
    static std::vector<std::string> args_;
    if (argc >= 0) {
      for (int i = 0; i < argc; ++i) {
        args_.push_back(argv[i]);
      }
    }
    return args_;
  }
};
}  // namespace mt

#define TEST_MAIN()                                                                             \
  int main(int argc, char* argv[]) {                                                            \
    mt::Runtime::args(argc, argv);                                                              \
                                                                                                \
    std::size_t num_failed = mt::TestsManager::RunAllTests(stdout);                             \
    if (num_failed == 0) {                                                                      \
      std::fprintf(stdout, "%s{ summary} All tests succeeded!%s\n", mt::green(), mt::def());    \
      return 0;                                                                                 \
    } else {                                                                                    \
      double percentage = 100.0 * static_cast<double>(num_failed) /                             \
                          static_cast<double>(mt::TestsManager::tests().size());                \
      std::fprintf(stderr, "%s{ summary} %zu tests failed (%.2f%%)%s\n", mt::red(), num_failed, \
                   percentage, mt::def());                                                      \
      return 1;                                                                                 \
    }                                                                                           \
  }

#endif  // MICROTEST_MICROTEST_H
