#include "check.h"

int executed = 0;
void first() { ++executed; }
void second() { ++executed; }

int main() {
  int failures = 0;
  char firstName[] = "same";
  char secondName[] = "same";
  check(mt::TestsManager::AddTest(first, firstName), "first registration accepted", failures);
  check(!mt::TestsManager::AddTest(second, secondName), "duplicate name rejected by content",
        failures);
  check(mt::TestsManager::tests().size() == 1, "duplicate not added", failures);
  check(mt::TestsManager::RegistrationErrorCount() == 1, "registration error retained", failures);
  check(mt::TestsManager::RunAllTests() == 1, "invalid suite fails", failures);
  check(executed == 0, "invalid suite executes no tests", failures);
  return failures == 0 ? 0 : 1;
}
