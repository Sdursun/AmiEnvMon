#include "unity.h"
#include "unity_fixture.h"
#include <stdbool.h>
#include "string_utils.h"

TEST_GROUP(string_utils_isStringEmpty);

TEST_SETUP(string_utils_isStringEmpty) {}

TEST_TEAR_DOWN(string_utils_isStringEmpty) {}

TEST(string_utils_isStringEmpty, NonEmptyString) {
  bool result = isStringEmpty("hello there");
  TEST_ASSERT_FALSE(result);
}

TEST(string_utils_isStringEmpty, NullInput) {
  bool result = isStringEmpty(NULL);
  TEST_ASSERT_TRUE(result);
}

TEST(string_utils_isStringEmpty, EmptyString) {
  bool result = isStringEmpty("");
  TEST_ASSERT_TRUE(result);
}

TEST_GROUP_RUNNER(string_utils_isStringEmpty) {
  RUN_TEST_CASE(string_utils_isStringEmpty, NonEmptyString);
  RUN_TEST_CASE(string_utils_isStringEmpty, NullInput);
  RUN_TEST_CASE(string_utils_isStringEmpty, EmptyString);
}
