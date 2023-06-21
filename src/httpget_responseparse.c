#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// clrf is default line sep in http responses:
// https://www.w3.org/Protocols/rfc2616/rfc2616-sec4.html
#define HEADER_SEPERATOR "\r\n"

// TODO: duplicate of json string test in envmondata.c.
// maybe have a common string utils?
#define MAX_JSON_STRING_LENGTH 512
static bool isStringEmpty(const char* json_string) {
  return NULL == json_string || 0 == strncmp(json_string, "", MAX_JSON_STRING_LENGTH);
}

// stupid assumption to make life simpler: we can only get json body on HTTP status 200
// (the env monitor actually is super simple, which helps)
static bool isHttpStatus200(const char* http_response) {
  // "HTTP/1.1 200" length is 12
  return 0 == strncmp(http_response, "HTTP/1.1 200", 12);
}

char *getJsonBodyFromHttpResponse(const char *http_response) {
  if(isStringEmpty(http_response) || !isHttpStatus200(http_response)) {
    return NULL;
  }
  
  char* response = strdup(http_response);
  char* current;
  char* prev;
  char* json_body;
  for(current = strsep(&response, HEADER_SEPERATOR);
      current != NULL;
      current = strsep(&response, HEADER_SEPERATOR)) {
    prev = current;
  }
  
  json_body = strdup(prev);
  return json_body;
}
