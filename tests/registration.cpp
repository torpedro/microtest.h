#include "check.h"
#include "microtest/microtest.h"

int executed = 0;
TEST(FirstTranslationUnit) { ++executed; }

int main() {
  int failures = 0;
  check(mt::TestsManager::tests().size() == 7, "cross-translation-unit registration", failures);
  check(mt::TestsManager::RunAllTests() == 4, "exact failure count including exceptions", failures);
  check(executed == 7, "all registered tests run after failures and exceptions", failures);
  return failures == 0 ? 0 : 1;
}
