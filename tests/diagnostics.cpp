#include <stdexcept>
#include <vector>

#include "check.h"

struct Opaque {
  int value;
};
inline bool operator==(const Opaque& a, const Opaque& b) { return a.value == b.value; }
inline bool operator!=(const Opaque& a, const Opaque& b) { return a.value != b.value; }
inline bool operator==(const Opaque& a, int b) { return a.value == b; }

struct Printable {
  int value;
};
inline bool operator==(const Printable& a, const Printable& b) { return a.value == b.value; }
inline std::ostream& operator<<(std::ostream& stream, const Printable& value) {
  return stream << "Printable(" << value.value << ")";
}

TEST(OpaqueSuccess) {
  ASSERT_EQ(Opaque{1}, Opaque{1});
  ASSERT_NEQ(Opaque{1}, Opaque{2});
  ASSERT_EQ(std::vector<int>(2, 7), std::vector<int>(2, 7));
}
TEST(OpaqueEqualityFailure) { ASSERT_EQ(Opaque{1}, Opaque{2}); }
TEST(OpaqueInequalityFailure) { ASSERT_NEQ(Opaque{1}, Opaque{1}); }
TEST(MixedFailure) { ASSERT_EQ(Opaque{1}, 9); }
TEST(PrintableFailure) { ASSERT_EQ(Printable{1}, Printable{2}); }
TEST(StringFailure) { ASSERT_STREQ("left", "right"); }
TEST(StandardExceptionDiagnostic) { throw std::runtime_error("diagnostic error"); }
TEST(UnknownExceptionDiagnostic) { throw 42; }

int main(int argc, char** argv) {
  if (argc != 2) return 2;
  int failures = 0;
  // Directly caught assertions retain their diagnostics without writing to stdout.
  try {
    ASSERT_EQ(Opaque{1}, 9);
    check(false, "expected assertion failure", failures);
  } catch (const mt::AssertFailedException& error) {
    check(std::string(error.getDetails()) == "Actual values: <unprintable> != 9",
          "mixed operand diagnostic", failures);
    check(std::string(error.what()) == "Opaque{1} == 9", "original expression", failures);
  }

  std::FILE* file = nullptr;
#ifdef _MSC_VER
  if (fopen_s(&file, argv[1], "w") != 0) return 2;
#else
  file = std::fopen(argv[1], "w");
#endif
  if (!file) return 2;
  check(mt::TestsManager::RunAllTests(file) == 7, "custom-stream failure count", failures);
  check(std::fclose(file) == 0, "close diagnostics file", failures);
  return failures == 0 ? 0 : 1;
}
