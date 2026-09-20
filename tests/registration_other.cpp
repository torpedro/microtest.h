#include <stdexcept>

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
TEST(StandardException) {
  ++executed;
  throw std::runtime_error("registration test error");
}
TEST(NonStandardException) {
  ++executed;
  throw 42;
}
TEST(AfterExceptions) { ++executed; }
