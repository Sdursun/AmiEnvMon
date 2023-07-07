#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "jsmn.h"
#include "envmondata.h"
#include "string_utils.h"

#define MAX_JSON_TOKENS 30
#define MAX_JSON_STRING_LENGTH 512

// TODO: prettify
struct EnvironmentMonitorData *EnvMon_FromJsonString(const char* json_string) {
  if(isStringEmpty(json_string)) {
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
  size_t num_read = 0;
  for(i = 0; i < num_parsed_tokens; i++) {
    size_t string_length = json_tokens[i].end - json_tokens[i].start;

    // weird bug that somtimes happen only on Amiga
    if(-1 == string_length) {
      continue;
    }
    
    if(0 == strncmp(json_string + json_tokens[i].start, "temperature", string_length)) {
      num_read++;
      data->temperature = atof(json_string + json_tokens[i + 1].start);
    }
    else if(0 == strncmp(json_string + json_tokens[i].start, "humidity", string_length)) {
      num_read++;
      data->humidity = atof(json_string + json_tokens[i + 1].start);
    }
    else if(0 == strncmp(json_string + json_tokens[i].start, "eCO2", string_length)) {
      num_read++;
      data->eCO2 = atoi(json_string + json_tokens[i + 1].start);
    }

    // stupid fix for invalid data to avoid segfaulting
    // far from perfect. Need to find a better more longterm solution
    if(3 <= num_read) {
      break;
    }
  }
  
  return data;
}


void EnvMon_Destroy(struct EnvironmentMonitorData* envmon_data) {
  free(envmon_data);
}
