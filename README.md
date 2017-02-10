# microtest (C++)
[![GitHub release](https://img.shields.io/github/release/torpedro/microtest.svg?maxAge=2592000)](https://github.com/torpedro/microtest/releases)
[![Build Status](https://api.travis-ci.org/torpedro/microtest.svg?branch=master)](https://travis-ci.org/torpedro/microtest)

This is a small **header-only** C++ unit testing framework. It allows to define small unit tests with set of assertions available. It is contained within a **single header file**, so you can simply copy that header file to your project and don't have to link libraries or include multiple header files, if all you need is a quick testing framework.

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
