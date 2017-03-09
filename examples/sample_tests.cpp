
#include "microtest/microtest.h"

TEST(ExampleSucceedingTest) {
  int a = 20;
  int b = 20;
  int c = 5;

  ASSERT(true);

  ASSERT_TRUE(true);

  ASSERT_FALSE(false);

  ASSERT_EQ(a, b);

  ASSERT_NEQ(a, c);

  ASSERT_NULL(NULL);

  int* x = new int;
  ASSERT_NOTNULL(x);
  delete x;

  ASSERT_STREQ("abc", "abc");
}

TEST(FailingTest) {
  int a = 20;
  int c = 5;

  ASSERT_EQ(a, c);
}

TEST(AssertTest_STREQ) {
  std::string a = "abcdefg";
  ASSERT_STREQ(a, "abcdefg");
}

TEST(AssertTestFail_STREQ) {
  std::string a = "abcdefg";
  ASSERT_STREQ(a, "xyz");
}

TEST_MAIN();
