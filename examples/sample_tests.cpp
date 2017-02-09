
#include "microtest/microtest.h"

TEST(SucceedingTest) {
  ASSERT(true);
}

TEST(FailingTest) {
  ASSERT(false);
}

TEST_MAIN();
