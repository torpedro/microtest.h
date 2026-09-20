#include "check.h"
#include "microtest/microtest.h"

int executed = 0;
TEST(FirstTranslationUnit) { ++executed; }

int main() {
  int failures = 0;
  check(mt::TestsManager::tests().size() == 4, "cross-translation-unit registration", failures);
  check(mt::TestsManager::RunAllTests() == 2, "exact failure count", failures);
  check(executed == 4, "all registered tests run after failures", failures);
  return failures == 0 ? 0 : 1;
}
