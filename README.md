# microtest (C++)
[![Build Status](https://img.shields.io/travis/torpedro/microtest.svg?maxAge=2592000)](https://travis-ci.org/hyrise/sql-parser)

A bare **header-only** C++ testing framework. It allows to define small unit tests with set of assertions available.

## How to Use

To create a test file you only need to include `microtest/microtest.h`. Then you can define your unit tests. To execute all tests one of your source files needs to contain the `TEST_MAIN();` macro. See the following example:

```cpp
#include "microtest/microtest.h"

TEST(ExampleSucceedingTest) {
  ASSERT(true);
  
  ASSERT_TRUE(true);
  
  ASSERT_FALSE(false);

  ASSERT_EQ(20, 20);

  ASSERT_NULL(NULL);

  int* x = new int;
  ASSERT_NOTNULL(x);
  delete x;

  ASSERT_STREQ("abc", "abc");
}

TEST(ExampleFailingTest) {
  ASSERT_EQ(20, 5);
}

TEST_MAIN();
```
