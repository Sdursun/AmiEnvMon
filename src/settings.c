#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "settings.h"

struct ApplicationSettings *Settings_ReadFromFile(const char *filename) {
  FILE* settings_file;
  settings_file = fopen(filename, "r");

  if (NULL == settings_file) {
    return NULL;
  }

  size_t data_read = 0;
  struct ApplicationSettings* settings = calloc(1, sizeof(struct ApplicationSettings));
  // TODO: should probably have a constant to avoid specifying this everywhere
  //       16 = 15 characters ip + nul byte
  settings->ip = calloc(16, sizeof(char));
  data_read = fread(settings->ip, sizeof(char), 16, settings_file);

  // TODO: can we handle this in a more clean way? if we read more settings, there will be multiple of these..
  if (0 >= data_read) {
    Settings_Destroy(settings);
    settings = NULL;
  }
  
  fclose(settings_file);

  return settings;
}

struct ApplicationSettings *Settings_Default() {
  struct ApplicationSettings* settings = calloc(1, sizeof(struct ApplicationSettings));
  settings->ip = strndup("10.0.0.6", 16);
  settings->number_boxes_alignment = RIGHT;
  // TODO: the rest of the defaults
  return settings;
}

void Settings_WriteSettingsToFile(struct ApplicationSettings *settings,
                                  const char *filename) {
  // TODO: should we maybe error check more?
  FILE* settings_file;
  settings_file = fopen(filename, "w");

  if (NULL == settings_file) {
    // TODO: see if there is a better scenario then just ignoring lol
    return;
  }
  
  fwrite(settings->ip, sizeof(char), 16, settings_file);
  // TODO: other settings

  fclose(settings_file);
}


void Settings_Destroy(struct ApplicationSettings *settings) {
  // TODO: getting a recoverable error in Amiga OS here. Any places we do not allocate the and free the correct way here?
  free(settings->ip);
  free(settings);
}
