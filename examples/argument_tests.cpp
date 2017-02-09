
#include "microtest/microtest.h"

TEST(ArgumentedTest) {
	const std::vector<std::string>& args = mt::Runtime::args();
	ASSERT_EQ(args.size(), 2);
}

TEST_MAIN();
