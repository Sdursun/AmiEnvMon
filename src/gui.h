#ifndef GUI_H
#define GUI_H
#include "envmondata.h"

// Provide the environment monitor data for the GUI
void GUI_SetEnvMonData(struct EnvironmentMonitorData* data);

// GUI main loop
void GUI_Draw();

#endif
