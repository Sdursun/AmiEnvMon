#include <stdlib.h>
#include "unity.h"
#include "unity_fixture.h"

#include "httpget_responseparse.h"

// testing the result parsing when getting a http response
// (i.e, getting the json body from the complete HTTP response string)

TEST_GROUP(httpget_responseparse);

char* json_body;

TEST_SETUP(httpget_responseparse) {
  json_body = NULL;
}

TEST_TEAR_DOWN(httpget_responseparse) {
  if(json_body) {
    free(json_body);
  }
}

TEST(httpget_responseparse, EmptyStringShouldReturnNull) {
  json_body = getJsonBodyFromHttpResponse("");
  TEST_ASSERT_NULL(json_body);
}

// TODO: should we test different kinds of json bodies? or be content with a single test?
TEST(httpget_responseparse, JsonBodySuccess) {
  json_body = getJsonBodyFromHttpResponse("HTTP/1.1 200 OK\r\nContent-Length: 23\r\nContent-Type: application/json\r\nConnection: close\r\n{\"message\": \"hi!\"}");
  TEST_ASSERT_NOT_NULL(json_body);
  TEST_ASSERT_EQUAL_STRING("{\"message\": \"hi!\"}", json_body);
}

// test for non-200 responses. Might happen once in a while
TEST(httpget_responseparse, InvalidHttpResponse) {
  // json_body... yeah right
  json_body = getJsonBodyFromHttpResponse("HTTP/1.1 418 IM A TEAPOT\r\n");
  TEST_ASSERT_NULL(json_body);
}

// TODO: should we handle garbage after the json data? or assume it is okay?

TEST_GROUP_RUNNER(httpget_responseparse) {
  RUN_TEST_CASE(httpget_responseparse, EmptyStringShouldReturnNull);
  RUN_TEST_CASE(httpget_responseparse, JsonBodySuccess);
  RUN_TEST_CASE(httpget_responseparse, InvalidHttpResponse);
}
