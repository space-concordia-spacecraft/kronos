#pragma once

#include "KronosTest.h"

/***
 * Verifies that kronos::String::Size() works.
 * Checks that the Size() returns the actual size of the string.
 */
extern KT_TEST(StringSizeTest);

/***
 * Verifies that kronos::String::Concatenate(String, String) works.
 * Checks that the Concatenate() function properly concatenates two strings together.
 */
extern KT_TEST(StringConcatenateTest);

/***
 * Verifies that the kronos::String::Substring(size_t, size_t) works.
 * Checks that the Substring() function properly returns a substring of the string.
 */
extern KT_TEST(StringSubstringTest);


/***
 * Verifies that the kronos::String::operator!= works.
 * Checks that the != operator verifies if two strings are different.
 */
extern KT_TEST(StringComparisonTest);

/***
 * Verifies that the kronos::String::operator= works.
 * Checks that the = operator assigns a string to the variable.
 */
extern KT_TEST(StringAssignmentTest);

/***
 * Verifies that the kronos::String::operator+ works.
 * Checks that the + operator properly concatenates two strings together.
 */
extern KT_TEST(StringAdditionTest);

/***
 * Verifies that the kronos::String::operator+= works.
 * Checks that the += operator properly appends a string to another.
 */
extern KT_TEST(StringAdditionAssignmentTest);

/***
 * Verifies that the kronos::String::operator[] works.
 * Checks that the [] operator returns the character at a specific index.
 */
extern KT_TEST(StringIndexTest);