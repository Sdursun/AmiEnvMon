#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "jsmn.h"
#include "envmondata.h"

#define MAX_JSON_TOKENS 30
#define MAX_JSON_STRING_LENGTH 512

static bool isJsonStringEmpty(const char* json_string) {
  return NULL == json_string || 0 == strncmp(json_string, "", MAX_JSON_STRING_LENGTH);
}

// TODO: prettify
struct EnvironmentMonitorData *EnvMon_FromJsonString(const char* json_string) {
  if(isJsonStringEmpty(json_string)) {
    return NULL;
  }
  
  struct EnvironmentMonitorData* data = calloc(1, sizeof(struct EnvironmentMonitorData));

  // parse the json body
  jsmn_parser json_parser;
  jsmntok_t json_tokens[MAX_JSON_TOKENS];
  jsmn_init(&json_parser);
  size_t num_parsed_tokens = jsmn_parse(&json_parser,
                                        json_string,
                                        strnlen(json_string, MAX_JSON_STRING_LENGTH),
                                        json_tokens,
                                        MAX_JSON_TOKENS);

  size_t i;
  for(i = 0; i < num_parsed_tokens; i++) {
    size_t string_length = json_tokens[i].end - json_tokens[i].start;
    
    if(0 == strncmp(json_string + json_tokens[i].start, "temperature", string_length)) {
      data->temperature = atof(json_string + json_tokens[i + 1].start);
    }
    else if(0 == strncmp(json_string + json_tokens[i].start, "humidity", string_length)) {
      data->humidity = atof(json_string + json_tokens[i + 1].start);
    }
    else if(0 == strncmp(json_string + json_tokens[i].start, "eCO2", string_length)) {
      data->eCO2 = atoi(json_string + json_tokens[i + 1].start);
    }
  }
  
  return data;
}


void EnvMon_Destroy(struct EnvironmentMonitorData* envmon_data) {}
