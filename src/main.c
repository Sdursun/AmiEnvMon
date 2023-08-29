#include <stdlib.h>
#include "envmondata.h"
#include "httpget.h"
#include "httpget_responseparse.h"
#include "gui.h"
#include "settings.h"

int main() {
  // read current settings if available
  struct ApplicationSettings* application_settings;
  application_settings = Settings_ReadFromFile(DEFAULT_SETTINGS_FILENAME);

  // default to some nice defaults if no current settings are created
  if (NULL == application_settings) {
    application_settings = Settings_Default();
  }
  
  char* http_response = httpget(application_settings->ip);
  char* json_body = getJsonBodyFromHttpResponse(http_response);  
  struct EnvironmentMonitorData* envmondata = EnvMon_FromJsonString(json_body);
  
  free(http_response);
  free(json_body);
  
  GUI_SetEnvMonData(envmondata);
  GUI_SetSettings(application_settings);
  GUI_Draw();

  EnvMon_Destroy(envmondata);
  Settings_Destroy(application_settings);
  return 0;
}
