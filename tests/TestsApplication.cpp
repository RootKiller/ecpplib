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

void TestCustomUnitTestLogFunction(LoggingFunctionSeverity severity, const char* message, va_list args, void* userData);

int main()
{
	RunTestsContext runTestContext;

	// We have option to override logging function here and log to file for example.
	runTestContext.loggingFunction = &TestCustomUnitTestLogFunction;

	// Only launch ECppLib tests.
	const char* categoryFilters[] = {"ecpplib", nullptr};
	runTestContext.categoryFilters = categoryFilters;

	return RunTests(runTestContext);
}

void TestCustomUnitTestLogFunction(LoggingFunctionSeverity severity, const char* message, va_list args, void* userData)
{
	FILE *targetFile = (severity == LoggingFunctionSeverity::Error) ? stderr : stdout;

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
