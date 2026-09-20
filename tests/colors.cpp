#include "microtest/microtest.h"

TEST(ColorFailure) { ASSERT_EQ(1, 2); }
TEST(ColorSuccess) { ASSERT(true); }

int main(int argc, char** argv) {
  if (argc != 2) return 2;
  const std::string mode = argv[1];
  if (mode == "always")
    mt::setColorMode(mt::ColorMode::Always);
  else if (mode == "never")
    mt::setColorMode(mt::ColorMode::Never);
  else if (mode == "auto")
    mt::setColorMode(mt::ColorMode::Auto);
  else
    return 2;
  // Exercise both destinations independently, including assertion details.
  mt::printFailed("stderr diagnostic", stderr);
  return mt::TestsManager::RunAllTests() == 1 ? 0 : 1;
}
