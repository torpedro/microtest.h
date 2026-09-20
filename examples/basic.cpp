#include "microtest/microtest.h"

TEST(BasicAssertions) {
  ASSERT(true);
  ASSERT_TRUE(true);
  ASSERT_FALSE(false);
  ASSERT_EQ(20, 20);
  ASSERT_NEQ(20, 5);
  ASSERT_NULL(nullptr);
  int value = 42;
  ASSERT_NOTNULL(&value);
  ASSERT_STREQ("abc", "abc");
  ASSERT_STRNEQ("abc", "xyz");
}

TEST_MAIN();
