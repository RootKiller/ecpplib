// Copyright (C) 2019-2025 by Eryk Dwornicki

#pragma once

#include <stdarg.h>

/** Severity of the log the unit tests can output. */
enum class LoggingFunctionSeverity { Info, Warning, Error, };

/**  Signature of the logging function. The passed parameters are promised to be valid by the library. */
using LoggingFunction = void(*)(const LoggingFunctionSeverity severity, const char *const message, const va_list args, void* const userData);

/**
 * Structure defining context of tests execution.
 */
struct RunTestsContext
{
	/** The function used for logging the execution of the tests. If not set standard printf will be used */
	LoggingFunction loggingFunction = nullptr;

	/** Optional user data passed to logging function. */
	void* loggingFunctionUserData = nullptr;

	/**
	 * When set to an array of null-terminated strings, only tests with the provided category are ran.
	 * The last element of the array should be a nullptr.
	 * The category names are case insensitive.
	 */
	const char** categoryFilters = nullptr;
};

/**
 * Runs all registered tests.
 *
 * @warning This method is not thread-safe and should be only executed once at a time.
 * @info This function is re-entrant.
 *
 * @return @c 0 if all tests succeeded, non zero value otherwise.
 */
int RunTests(const RunTestsContext& context = {});

/**
 * Defines a test.
 *
 * @param CategoryName The category name.
 * @param Name The test case name.
 *
 * Example:
 * @begincode
 * TEST(Core, Perf)
 * {
 *      ... test code
 * }
 * @endcode
 */
#define TEST(CategoryName, Name) \
	void TEST_CONCAT_MACRO(TEST_CONCAT_MACRO(CategoryName, Name), EntryPoint)(); \
	const TestCase TEST_CONCAT_MACRO(TEST_CONCAT_MACRO(CategoryName, Name), Case)( \
		#CategoryName, #Name, &TEST_CONCAT_MACRO(TEST_CONCAT_MACRO(CategoryName, Name), EntryPoint) \
	); \
	void TEST_CONCAT_MACRO(TEST_CONCAT_MACRO(CategoryName, Name), EntryPoint)()

/**
 * Check if two given values are the same using equals(==) operator.
 *
 * @param FirstValue
 * @param SecondValue
 */
#define EXPECT_EQ(FirstValue, SecondValue) \
	if (Test_ExpectEq((FirstValue) == (SecondValue), #FirstValue, #SecondValue, __FILE__, __LINE__)) return

/**
 * Check if two given values are not the same using not-equals(!=) operator.
 *
 * @param FirstValue
 * @param SecondValue
 */
#define EXPECT_NE(FirstValue, SecondValue) \
	if (Test_ExpectNe((FirstValue) != (SecondValue), #FirstValue, #SecondValue, __FILE__, __LINE__)) return

/**
 * Check if given expression is true.
 *
 * @param Expression
 */
#define EXPECT_TRUE(Expression) \
	if (Test_ExpectLogial((Expression), true, #Expression, __FILE__, __LINE__)) return

/**
 * Check if given expression is true.
 *
 * @param Expression
 */
#define EXPECT_FALSE(Expression) \
	if (Test_ExpectLogial((Expression), false, #Expression, __FILE__, __LINE__)) return

// Implementation detail at the bottom, 
// you can skip reading here unless you want to learn more on how the library is implemented.
//

/** The signature of entry point of the test. */
using TestEntryPoint = void (*)();

/** A auto-registering structure defining the single test case. */
struct TestCase
{
	/** Link to next registered test case. @c c nullptr if this is last test case. */
	TestCase *nextTestCase = nullptr;
	/** The entry point executing test logic. */
	const TestEntryPoint entryPoint = nullptr;
	/** Pointer to compile-time allocated ANSI string representing category of the test. */
	const char *const category = nullptr;
	/** Pointer to compile-time allocated ANSI string representing name of the test. */
	const char *const name = nullptr;

	TestCase(const char *const categoryName, const char *const caseName, const TestEntryPoint theEntryPoint);
};

bool Test_ExpectEq(const bool result, const char *firstValue, const char *secondValue,
				   const char *file, const unsigned line);
bool Test_ExpectNe(const bool result, const char *firstValue, const char *secondValue,
				   const char *file, const unsigned line);
bool Test_ExpectLogial(const bool result, const bool expected, const char *expression,
					   const char *file, const unsigned line);

#define TEST_CONCAT_MACRO_INNER(A,B) A##B
#define TEST_CONCAT_MACRO(A,B) TEST_CONCAT_MACRO_INNER(A, B)

/* eof */
