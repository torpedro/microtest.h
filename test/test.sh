#!/bin/bash
RET=0

expectSuccess() {
  if [ $? != 0 ]; then
    RET=$(($RET + 1))
    echo -e "\033[1;31m{  failed} "$1 "\033[0m"
  else
    echo -e "\033[0;32m{      ok} "$1 "\033[0m"
  fi
}

expectFailure() {
  if [ $? == 0 ]; then
    RET=$(($RET + 1))
    echo -e "\033[1;31m{      ok} "$1" (expected failure)\033[0m"
  else
    echo -e "\033[0;32m{  failed} "$1" (expected failure)\033[0m"
  fi
}

# Runs the example binaries and checks if the return the expected result.
# Run Sample Tests: Expect Failure
bin/sample_tests >> /dev/null 2> /dev/null
expectFailure "Sample Test"

# Run Argumented Test: Expect Failure
bin/argument_tests >> /dev/null 2> /dev/null
expectFailure "Argument Test 1"

# Run Argumented Test: Expect Success
bin/argument_tests 100 >> /dev/null 2> /dev/null
expectSuccess "Argument Test 2"

exit $RET