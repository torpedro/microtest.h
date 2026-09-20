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
#include <cstring>
#include <exception>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

#if defined(_WIN32)
#include <io.h>
#elif defined(__unix__) || defined(__APPLE__)
#include <unistd.h>
#endif

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
  static void name();                                                              \
  namespace {                                                                      \
  const bool microtest_registered_##name = mt::TestsManager::AddTest(name, #name); \
  }                                                                                \
  static void name()

///////////////
// Framework //
///////////////

namespace mt {

enum class ColorMode { Auto, Always, Never };

namespace detail {
inline ColorMode& colorMode() {
  static ColorMode mode = ColorMode::Auto;
  return mode;
}

inline bool useColor(std::FILE* file) {
  if (colorMode() != ColorMode::Auto) return colorMode() == ColorMode::Always;
#if defined(_WIN32)
  return _isatty(_fileno(file)) != 0;
#elif defined(__unix__) || defined(__APPLE__)
  return isatty(fileno(file)) != 0;
#else
  (void)file;
  return false;
#endif
}
}  // namespace detail

inline void setColorMode(ColorMode mode) { detail::colorMode() = mode; }
inline ColorMode getColorMode() { return detail::colorMode(); }

inline const char* red(std::FILE* file = stdout) {
  return detail::useColor(file) ? "\033[1;31m" : "";
}
inline const char* green(std::FILE* file = stdout) {
  return detail::useColor(file) ? "\033[0;32m" : "";
}
inline const char* yellow(std::FILE* file = stdout) {
  return detail::useColor(file) ? "\033[0;33m" : "";
}
inline const char* def(std::FILE* file = stdout) { return detail::useColor(file) ? "\033[0m" : ""; }

inline void printRunning(const char* message, std::FILE* file = stdout) {
  std::fprintf(file, "%s{ running}%s %s\n", green(file), def(file), message);
}

inline void printOk(const char* message, std::FILE* file = stdout) {
  std::fprintf(file, "%s{      ok}%s %s\n", green(file), def(file), message);
}

inline void printFailed(const char* message, std::FILE* file = stdout) {
  std::fprintf(file, "%s{  failed} %s%s\n", red(file), message, def(file));
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
  // Reject duplicate names and record the error for the runner.
  inline static bool AddTest(void (*fn)(void), const char* name) {
    for (const Test& test : tests()) {
      if (std::strcmp(test.name, name) == 0) {
        registrationErrors().push_back(name);
        return false;
      }
    }
    tests().push_back({name, fn});
    return true;
  }

  inline static std::size_t RegistrationErrorCount() { return registrationErrors().size(); }

  // Run all tests that are registered.
  // Returns the number of failed tests, or registration errors if the suite is invalid.
  // An invalid suite is rejected before any tests execute.
  inline static std::size_t RunAllTests(std::FILE* file = stdout) {
    if (!registrationErrors().empty()) {
      for (const std::string& name : registrationErrors()) {
        std::fprintf(file, "%s{   error} Duplicate test name: %s%s\n", red(file), name.c_str(),
                     def(file));
      }
      return registrationErrors().size();
    }
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
          std::fprintf(file, "%s{    info} %s%s\n", yellow(file), def(file), e.getDetails());
        }
        std::fprintf(file, "           %sAssertion failed: %s%s\n", red(file), e.what(), def(file));
        std::fprintf(file, "           %s%s:%d%s\n", red(file), e.getFilepath(), e.getLine(),
                     def(file));
        ++num_failed;
      } catch (const std::exception& e) {
        printFailed(test.name, file);
        std::fprintf(file, "           %sUnexpected exception: %s%s\n", red(file), e.what(),
                     def(file));
        ++num_failed;
      } catch (...) {
        printFailed(test.name, file);
        std::fprintf(file, "           %sUnexpected non-standard exception%s\n", red(file),
                     def(file));
        ++num_failed;
      }
    }

    return num_failed;
  }

 private:
  static std::vector<std::string>& registrationErrors() {
    static std::vector<std::string> errors;
    return errors;
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

#define TEST_MAIN()                                                                          \
  int main(int argc, char* argv[]) {                                                         \
    mt::Runtime::args(argc, argv);                                                           \
                                                                                             \
    std::size_t num_failed = mt::TestsManager::RunAllTests(stdout);                          \
    if (mt::TestsManager::RegistrationErrorCount() != 0) {                                   \
      std::fprintf(                                                                          \
          stderr,                                                                            \
          "%s{ summary} Registration failed: %zu duplicate test name(s); no tests ran.%s\n", \
          mt::red(stderr), mt::TestsManager::RegistrationErrorCount(), mt::def(stderr));     \
      return 1;                                                                              \
    } else if (num_failed == 0) {                                                            \
      std::fprintf(stdout, "%s{ summary} All tests succeeded!%s\n", mt::green(stdout),       \
                   mt::def(stdout));                                                         \
      return 0;                                                                              \
    } else {                                                                                 \
      double percentage = 100.0 * static_cast<double>(num_failed) /                          \
                          static_cast<double>(mt::TestsManager::tests().size());             \
      std::fprintf(stderr, "%s{ summary} %zu tests failed (%.2f%%)%s\n", mt::red(stderr),    \
                   num_failed, percentage, mt::def(stderr));                                 \
      return 1;                                                                              \
    }                                                                                        \
  }

#endif  // MICROTEST_MICROTEST_H
