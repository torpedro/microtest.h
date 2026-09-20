#include "microtest/microtest.h"
extern int executed;
TEST(SecondTranslationUnit) { ++executed; }
TEST(FirstFailure) {
  ++executed;
  ASSERT(false);
}
TEST(SecondFailure) {
  ++executed;
  ASSERT(false);
}
