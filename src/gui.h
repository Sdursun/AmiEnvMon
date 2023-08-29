#ifndef GUI_H
#define GUI_H
#include "envmondata.h"
#include "settings.h"

// Provide the environment monitor data for the GUI
void GUI_SetEnvMonData(struct EnvironmentMonitorData* data);

// Provide application settings object to GUI
void GUI_SetSettings(struct ApplicationSettings* settings);

// GUI main loop
void GUI_Draw();

#endif
