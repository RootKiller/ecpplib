// Copyright (C) 2019-2025 by Dwornicki Eryk

#include "UnitTestLib.h"

#include <stdio.h>
#include <string.h>

struct UnitTestLibState
{
	TestCase *firstTestCase = nullptr;
	bool currentTestFailed = false;
	RunTestsContext runContext = {};
};

/** A global state of unit test framework. */
static UnitTestLibState g_unitTestLibState;

static void DefaultUnitTestLogFunction(LoggingFunctionSeverity severity, const char* message, va_list args, void* userData)
{
	FILE *targetFile = (severity == LoggingFunctionSeverity::Error) ? stderr : stdout;

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

static void UnitTestLog(LoggingFunctionSeverity severity, const char* message, ...)
{
	va_list argumentsList;
	va_start(argumentsList, message);
	(*g_unitTestLibState.runContext.loggingFunction)(severity, message, argumentsList, g_unitTestLibState.runContext.loggingFunctionUserData);
	va_end(argumentsList);
}

static bool ShouldSkipTestCategory(const char* category)
{
	if (g_unitTestLibState.runContext.categoryFilters == nullptr)
	{
		return false;
	}

	const char** categoryFilters = g_unitTestLibState.runContext.categoryFilters;
	for (const char* categoryFilter = *categoryFilters; categoryFilter != nullptr; ++categoryFilters, categoryFilter = *categoryFilters)
	{
		if (stricmp(categoryFilter, category) == 0)
		{
			// This category is included in filters.
			return false;
		}
	}
	return true;
}

TestCase::TestCase(const char *categoryName, const char *caseName, TestEntryPoint theEntryPoint)
	: entryPoint(theEntryPoint)
	, category(categoryName)
	, name(caseName)
{
	nextTestCase = g_unitTestLibState.firstTestCase;
	g_unitTestLibState.firstTestCase = this;
}

int RunTests(const RunTestsContext &runTestsContext)
{
	g_unitTestLibState.runContext = runTestsContext;

	// Ensure that we have valid logging function.
	if (g_unitTestLibState.runContext.loggingFunction == nullptr)
	{
		g_unitTestLibState.runContext.loggingFunction = &DefaultUnitTestLogFunction;
	}

	g_unitTestLibState.currentTestFailed = false;

	bool allTestsSucceeded = true;
	for (TestCase *testCase = g_unitTestLibState.firstTestCase; testCase; testCase = testCase->nextTestCase)
	{
		if (ShouldSkipTestCategory(testCase->category))
		{
			continue;
		}

		UnitTestLog(LoggingFunctionSeverity::Info, "Running %s.%s test ..", testCase->category, testCase->name);

		g_unitTestLibState.currentTestFailed = false;
		(*testCase->entryPoint)();

		if (g_unitTestLibState.currentTestFailed)
		{
			UnitTestLog(LoggingFunctionSeverity::Error, "Test %s.%s failed!", testCase->category, testCase->name);
			allTestsSucceeded = false;
		} else
		{
			UnitTestLog(LoggingFunctionSeverity::Info, "Test %s.%s succeeded!", testCase->category, testCase->name);
		}
	}
	return allTestsSucceeded ? 0 : 1;
}

bool Test_ExpectEq(const bool result, const char *firstValue, const char *secondValue, const char *file, const unsigned line)
{
	if (result)
	{
		return false;
	}

	UnitTestLog(LoggingFunctionSeverity::Error, "EXPECT_EQ FAILED!");
	UnitTestLog(LoggingFunctionSeverity::Error, "  At %s:%u", file, line);
	UnitTestLog(LoggingFunctionSeverity::Error, "  %s != %s", firstValue, secondValue);
	g_unitTestLibState.currentTestFailed = true;
	return true;
}

bool Test_ExpectNe(const bool result, const char *firstValue, const char *secondValue, const char *file, const unsigned line)
{
	if (result)
	{
		return false;
	}

	UnitTestLog(LoggingFunctionSeverity::Error, "EXPECT_NE FAILED!");
	UnitTestLog(LoggingFunctionSeverity::Error, "  At %s:%u", file, line);
	UnitTestLog(LoggingFunctionSeverity::Error, "  %s == %s", firstValue, secondValue);
	g_unitTestLibState.currentTestFailed = true;
	return true;
}

bool Test_ExpectLogial(const bool result, const bool expected, const char *expression, const char *file, const unsigned line)
{
	if (result == expected)
	{
		return false;
	}

	UnitTestLog(LoggingFunctionSeverity::Error, "EXPECT_%s FAILED!", expected ? "TRUE" : "FALSE");
	UnitTestLog(LoggingFunctionSeverity::Error, "  At %s:%u", file, line);
	UnitTestLog(LoggingFunctionSeverity::Error, "  %s != %s", expression, expected ? "TRUE" : "FALSE");
	g_unitTestLibState.currentTestFailed = true;
	return true;
}
