#include "check.h"
#include "microtest/microtest.h"

int main() {
  int failures = 0;
  int value = 7;
  expectAssertion([] { ASSERT(true); }, false, failures);
  expectAssertion([] { ASSERT(false); }, true, failures);
  expectAssertion([] { ASSERT_TRUE(true); }, false, failures);
  expectAssertion([] { ASSERT_TRUE(false); }, true, failures);
  expectAssertion([] { ASSERT_FALSE(false); }, false, failures);
  expectAssertion([] { ASSERT_FALSE(true); }, true, failures);
  expectAssertion([] { ASSERT_NULL(nullptr); }, false, failures);
  expectAssertion([] { ASSERT_NULL(NULL); }, false, failures);
  expectAssertion([] { ASSERT_NULL(0); }, false, failures);
  int* nullPointer = nullptr;
  expectAssertion([&] { ASSERT_NULL(nullPointer); }, false, failures);
  expectAssertion([&] { ASSERT_NULL(&value); }, true, failures);
  expectAssertion([&] { ASSERT_NOTNULL(&value); }, false, failures);
  expectAssertion([] { ASSERT_NOTNULL(nullptr); }, true, failures);
  expectAssertion([] { ASSERT_NOTNULL(NULL); }, true, failures);
  expectAssertion([] { ASSERT_NOTNULL(0); }, true, failures);
  expectAssertion([&] { ASSERT_NOTNULL(nullPointer); }, true, failures);
  expectAssertion([] { ASSERT_EQ(1, 1); }, false, failures);
  expectAssertion([] { ASSERT_EQ(1, 2); }, true, failures);
  expectAssertion([] { ASSERT_NEQ(1, 2); }, false, failures);
  expectAssertion([] { ASSERT_NEQ(1, 1); }, true, failures);
  expectAssertion([] { ASSERT_STREQ("abc", std::string("abc")); }, false, failures);
  expectAssertion([] { ASSERT_STREQ("abc", "xyz"); }, true, failures);
  expectAssertion([] { ASSERT_STRNEQ("abc", "xyz"); }, false, failures);
  expectAssertion([] { ASSERT_STRNEQ("abc", "abc"); }, true, failures);

  int calls = 0;
  expectAssertion([&] { ASSERT_EQ(++calls, 1); }, false, failures);
  check(calls == 1, "EQ evaluates once on success", failures);
  calls = 0;
  expectAssertion([&] { ASSERT_EQ(9, ++calls); }, true, failures);
  check(calls == 1, "EQ evaluates once on failure", failures);
  calls = 0;
  expectAssertion([&] { ASSERT_NEQ(++calls, 2); }, false, failures);
  check(calls == 1, "NEQ evaluates once on success", failures);
  calls = 0;
  expectAssertion([&] { ASSERT_NEQ(1, ++calls); }, true, failures);
  check(calls == 1, "NEQ evaluates once on failure", failures);
  auto text = [&calls]() {
    ++calls;
    return "abc";
  };
  calls = 0;
  expectAssertion([&] { ASSERT_STREQ(text(), "xyz"); }, true, failures);
  check(calls == 1, "STREQ evaluates once on failure", failures);
  calls = 0;
  expectAssertion([&] { ASSERT_STRNEQ("abc", text()); }, true, failures);
  check(calls == 1, "STRNEQ evaluates once on failure", failures);

  expectAssertion([] { ASSERT_EQ(2 & 1, 1); }, true, failures);
  expectAssertion([] { ASSERT_EQ(2 & 1, 0); }, false, failures);
  expectAssertion([] { ASSERT_NEQ(2 & 1, 1); }, false, failures);
  expectAssertion([&] { ASSERT_NULL(false ? &value : nullptr); }, false, failures);
  expectAssertion([&] { ASSERT_NOTNULL(true ? &value : nullptr); }, false, failures);

  expectAssertion(
      [] {
        if (false)
          ASSERT_EQ(1, 2);
        else
          ASSERT_EQ(1, 1);
        if (false)
          ASSERT_NEQ(1, 1);
        else
          ASSERT_NEQ(1, 2);
        if (true)
          ASSERT(true);
        else
          ASSERT(false);
        if (true)
          ASSERT_TRUE(true);
        else
          ASSERT_TRUE(false);
        if (true)
          ASSERT_FALSE(false);
        else
          ASSERT_FALSE(true);
        if (true)
          ASSERT_NULL(nullptr);
        else
          ASSERT_NOTNULL(nullptr);
        if (false)
          ASSERT_NOTNULL(nullptr);
        else
          ASSERT_NULL(nullptr);
        if (true)
          ASSERT_STREQ("a", "a");
        else
          ASSERT_STREQ("a", "b");
        if (true)
          ASSERT_STRNEQ("a", "b");
        else
          ASSERT_STRNEQ("a", "a");
      },
      false, failures);

  const int expectedLine = __LINE__ + 2;
  try {
    ASSERT_EQ(1, 2);
    check(false, "failure must throw", failures);
  } catch (mt::AssertFailedException& error) {
    check(error.getLine() == expectedLine, "source line", failures);
    check(std::strcmp(error.getFilepath(), __FILE__) == 0, "source file", failures);
    check(std::strcmp(error.what(), "1 == 2") == 0, "source expression", failures);
  }
  return failures == 0 ? 0 : 1;
}
