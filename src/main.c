#include <stdio.h>
#include <stdlib.h>
#include "envmondata.h"
#include "httpget.h"
#include "httpget_responseparse.h"
#include "gui.h"

int main() {
  char* http_response = httpget();
  char* json_body = getJsonBodyFromHttpResponse(http_response);  
  struct EnvironmentMonitorData* envmondata = EnvMon_FromJsonString(json_body);

  printf("json string: %s\n", json_body);
  
  free(http_response);
  free(json_body);
  
  GUI_SetEnvMonData(envmondata);
  GUI_Draw();

  EnvMon_Destroy(envmondata);
  return 0;
}
