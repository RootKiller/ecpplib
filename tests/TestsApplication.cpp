#include "UnitTestLib/UnitTestLib.h"
#include <stdio.h>

TEST(ECppLib, TestUnitTestLib)
{
	EXPECT_EQ(10, 10);
	EXPECT_NE(10, 20);
	EXPECT_TRUE(true);
	EXPECT_FALSE(false);
}

TEST(ECppLibSkiped, SkippedTest) {}

void TestCustomUnitTestLogFunction(const LoggingFunctionSeverity severity, const char* const message, const va_list args, void* const userData);

int main()
{
	const char* onlyEppLibCategoryFilters[] = { "ecpplib", nullptr };
	const RunTestsContext runTestContext = {
		// We have option to override logging function here and log to file for example.
		.loggingFunction = &TestCustomUnitTestLogFunction,

		// Only launch ECppLib tests.
		.categoryFilters = onlyEppLibCategoryFilters,
	};

	return RunTests(runTestContext);
}

void TestCustomUnitTestLogFunction(const LoggingFunctionSeverity severity, const char* const message, const va_list args, void* const userData)
{
	FILE *const targetFile = (severity == LoggingFunctionSeverity::Error) ? stderr : stdout;

	fprintf(targetFile, "[custom] ");

	switch (severity) {
	case LoggingFunctionSeverity::Info:
		fprintf(targetFile, "[info] ");
		break;
	case LoggingFunctionSeverity::Warning:
		fprintf(targetFile, "[warning] ");
		break;
	case LoggingFunctionSeverity::Error:
		fprintf(targetFile, "[warning] ");
		break;
	}

	vfprintf(targetFile, message, args);
	fprintf(targetFile, "\n");
}

/* eof */
