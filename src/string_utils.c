#include <stdbool.h>
#include <string.h>

// should be sufficient to just read a few bytes ahead to see if the string is empty
// "clever optimization" lol :P 
#define MAX_STRING_LENGTH 25

bool isStringEmpty(const char *str) {
  return NULL == str || 0 == strncmp(str, "", MAX_STRING_LENGTH); 
}
