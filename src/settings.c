#include "settings.h"
#include <stdio.h>
#include <stdlib.h>


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
  data_read = fread(settings->ip, 16*sizeof(char), 1, settings_file);

  // TODO: can we handle this in a more clean way? if we read more settings, there will be multiple of these..
  if (0 <= data_read) {
    Settings_Destroy(settings);
    settings = NULL;
  }
  
  fclose(settings_file);

  return settings;
}


void Settings_WriteSettingsToFile(struct ApplicationSettings *settings,
                                  const char *filename) {}


void Settings_Destroy(struct ApplicationSettings *settings) {
  free(settings->ip);
  free(settings);
}
