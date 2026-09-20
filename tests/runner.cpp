#include "microtest/microtest.h"

TEST(First) { ASSERT_EQ(mt::Runtime::args().size(), std::size_t(1)); }
TEST(Second) { ASSERT(mt::Runtime::args().size() == 1); }
TEST(Third) { ASSERT(mt::Runtime::args().size() == 1); }
TEST(Fourth) { ASSERT(true); }
TEST_MAIN();
