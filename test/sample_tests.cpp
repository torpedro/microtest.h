
#include "microtest/microtest.h"

TEST(FailingTest) {
  // ASSERT(false);
}

TEST(SucceedingTest) {
  ASSERT(true);
}

TEST_MAIN();