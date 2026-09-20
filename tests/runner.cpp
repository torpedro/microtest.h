#include <stdexcept>

#include "microtest/microtest.h"

TEST(First) { ASSERT_EQ(mt::Runtime::args().size(), std::size_t(1)); }
TEST(Second) {
  if (mt::Runtime::args().size() > 1) throw std::runtime_error("runner test error");
}
TEST(Third) {
  if (mt::Runtime::args().size() > 1) throw 42;
}
TEST(Fourth) { ASSERT(true); }
TEST_MAIN();
