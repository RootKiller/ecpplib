# Simple unit test framework library

This is simple unit test framework library.

All you need to do is include both the .cpp file in your project's makefile.
And then write following code:

```
#include "UnitTestLib/UnitTestLib.h"

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

The `RunTests` function accepts argument of tpye `RunTestContext` which allows you to for example override logging function. Check its documentation in code to see all capabilities.
