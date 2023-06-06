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

TEST_GROUP_RUNNER(envmondata) {
  RUN_TEST_CASE(envmondata, EmptyInputShouldReturnNull);
}
