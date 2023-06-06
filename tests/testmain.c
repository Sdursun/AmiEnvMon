#include "unity_fixture.h"

static void runTests() {
  RUN_TEST_GROUP(envmondata);
}

int main(int argc, char *argv[]) {
  return UnityMain(argc, argv, runTests);
}

