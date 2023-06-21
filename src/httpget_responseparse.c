#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "string_utils.h"

// clrf is default line sep in http responses:
// https://www.w3.org/Protocols/rfc2616/rfc2616-sec4.html
#define HEADER_SEPERATOR "\r\n"

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
  free(response);
  return json_body;
}
