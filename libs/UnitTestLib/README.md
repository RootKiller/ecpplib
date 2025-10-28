# Simple unit test framework library

This is simple unit test framework library.

All you need to do is include both the .cpp file in your project's makefile.
And then write following code:

```
#include "unit_test_lib/unit_test_lib.h"

TEST(TestCategory, TestName)
{
	EXPECT_EQ(10, 10);
}

int main()
{
	// In some function, in example main() call
	return RunTests();
}
```
