#ifndef ENVMONDATA_H
#define ENVMONDATA_H

// Simple ADT for environment monitor data

// TODO: should these just be strings?
//       or is it okay to use soft-float for simplicity?
struct EnvironmentMonitorData {
  float temperature;
  float humidity;
  float eCO2;
};

struct EnvironmentMonitorData* EnvMon_FromJsonString(const char* json_string);
void EnvMon_Destroy(struct EnvironmentMonitorData* envmon_data);

#endif
