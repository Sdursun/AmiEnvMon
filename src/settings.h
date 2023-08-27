#ifndef SETTINGS_H
#define SETTINGS_H

// basic file for settings loading, settings struct, writing and reading the binary format

#define DEFAULT_SETTINGS_FILENAME "amienvmon_settings"

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

// TODO: should we abstract away the setting of fields? or should the consumers just do that themselves?

// Writes the settings object to file
void Settings_WriteSettingsToFile(struct ApplicationSettings* settings, const char* filename);

// Cleans up memory for the application settings struct.
void Settings_Destroy(struct ApplicationSettings* settings);

#endif
