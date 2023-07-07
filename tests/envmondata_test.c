#include "unity.h"
#include "unity_fixture.h"

#include "envmondata.h"

TEST_GROUP(envmondata);

struct EnvironmentMonitorData* data;

TEST_SETUP(envmondata) {}

TEST_TEAR_DOWN(envmondata) {
  EnvMon_Destroy(data);
}

TEST(envmondata, EmptyInputShouldReturnNull) {
  data = EnvMon_FromJsonString("");
  TEST_ASSERT_NULL(data);
}

TEST(envmondata, JsonParsedSuccesfully) {
  data = EnvMon_FromJsonString("{\"eCO2\": 400, \"humidity\": 32.4188, \"temperature\": 23.2282}");
  
  TEST_ASSERT_NOT_NULL(data);
  TEST_ASSERT_EQUAL_FLOAT(23.2282, data->temperature);
  TEST_ASSERT_EQUAL_FLOAT(32.4188, data->humidity);
  TEST_ASSERT_EQUAL_INT(400, data->eCO2);
}

TEST(envmondata, JsonDataCuttoff) {
  // this seems to happen sometimes...
  // We should support it without segfaulting
  data = EnvMon_FromJsonString("{\"eCO2\": 400, \"humidity\": 32.4188, \"temperature\": 23.228");
  
  TEST_ASSERT_NOT_NULL(data);
  TEST_ASSERT_EQUAL_FLOAT(23.228, data->temperature);
  TEST_ASSERT_EQUAL_FLOAT(32.4188, data->humidity);
  TEST_ASSERT_EQUAL_INT(400, data->eCO2);
}

// TODO: test case for some sort of invalid data? is that even possible?

TEST_GROUP_RUNNER(envmondata) {
  RUN_TEST_CASE(envmondata, EmptyInputShouldReturnNull);
  RUN_TEST_CASE(envmondata, JsonParsedSuccesfully);
  RUN_TEST_CASE(envmondata, JsonDataCuttoff);
}
