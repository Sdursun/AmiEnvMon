#ifndef SETTINGS_H
#define SETTINGS_H

// basic file for settings loading, settings struct, writing and reading the binary format

enum TextBoxAlignment {
  LEFT, RIGHT, CENTER
};

struct ApplicationSettings {
  char* ip;
  enum TextBoxAlignment number_boxes_alignment;
};

// Read application settings from a file and return a struct representing the data.
// If the file could not be read, is not correctly formatted etc., NULL is returned.
struct ApplicationSettings *Settings_ReadFromFile(const char* filename);

// Gives some sensible defaults to the settings
struct ApplicationSettings *Settings_Default();

// TODO: should we return a status or similar here? or just assume that writing
// will not fail
void Settings_WriteSettingsToFile(struct ApplicationSettings* settings, const char* filename);

// Cleans up memory for the application settings struct.
void Settings_Destroy(struct ApplicationSettings* settings);

#endif
