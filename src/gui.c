#include "envmondata.h"
#include "proto/muimaster.h"
#include "libraries/mui.h"
#include "proto/exec.h"
#include <stdio.h>
#include <stdlib.h>

// need to open these ourselves when using MUI with clib2
struct Library* MUIMasterBase;
struct IntuitionBase* IntuitionBase;

struct EnvironmentMonitorData* envmondata;

static void initLibraries() {
  // TODO: why isn't mui version 20 working at my end?
  MUIMasterBase = OpenLibrary("muimaster.library", MUIMASTER_VMIN);
  IntuitionBase = (struct IntuitionBase*)OpenLibrary("intuition.library", 37);
}

static void closeLibraries() {
  CloseLibrary(IntuitionBase);
  CloseLibrary(MUIMasterBase);
}

void GUI_SetEnvMonData(struct EnvironmentMonitorData *data) {
  envmondata = data;
}

#define MAX_STRING_LENGTH 8

// TODO: prettify. Always a bit sceptical of global variable shit
// also, free it yourself biatch
static char *createTemperatureString() {
  char* result = calloc(MAX_STRING_LENGTH, sizeof(char));
  snprintf(result, MAX_STRING_LENGTH, "%f C", envmondata->temperature);
  return result;
}

static char *createECO2String() {
  char* result = calloc(MAX_STRING_LENGTH, sizeof(char));
  snprintf(result, MAX_STRING_LENGTH, "%d ppm", envmondata->eCO2);
  return result;
}

void GUI_Draw() {
  initLibraries();

  APTR application;
  APTR window;

  char* temperature_string = createTemperatureString();
  char* eco2_string = createECO2String();
  
  // TODO: would a pixel art termometer or other meassuring device at the top be a fun touch? :P
  application = ApplicationObject,
    MUIA_Application_Title, "AmiEnvMon",
    MUIA_Application_Version, "$VER: 0.0.1",
    MUIA_Application_Description, "Simple environment monitor for Amiga",
    MUIA_Application_Base, "PROGRAM",
    
    SubWindow, window = WindowObject,
      MUIA_Window_Title, "AmiEnvMon",
      MUIA_Window_Activate, TRUE,
      MUIA_Window_Height, 300,
      MUIA_Window_Width, 250,
      MUIA_Window_LeftEdge, 75,
      MUIA_Window_TopEdge, 75,
      MUIA_Window_CloseGadget, TRUE,
      MUIA_Window_DepthGadget, TRUE,
      MUIA_Window_SizeGadget, TRUE,
    
      WindowContents, VGroup,
        Child, HGroup,
          Child, TextObject,
            MUIA_Text_Contents, "Temperature:",
          End,
          Child, TextObject,
            MUIA_Text_Contents, temperature_string,
          End,
        End,
        Child, HGroup,
          Child, TextObject,
            MUIA_Text_Contents, "Humidity:",
          End,
          Child, GaugeObject,
            MUIA_Gauge_Current, (long)envmondata->humidity,
            MUIA_Gauge_InfoText, "%ld %%",
            MUIA_Gauge_Horiz, TRUE,
          End,
        End,
        Child, HGroup,
          Child, TextObject,
            MUIA_Text_Contents, "eCO2:",
          End,
          Child, TextObject,
            MUIA_Text_Contents, eco2_string,
          End,
        End,
      End,
    End,
  End;

  // MUI copies the text strings, so free our own copies
  free(temperature_string);
  free(eco2_string);
  
  set(window, MUIA_Window_Open, TRUE);
  DoMethod(application, MUIM_Application_Run);

  DoMethod(window, MUIM_Notify, MUIA_Window_CloseRequest, TRUE, application, 2, MUIM_Application_ReturnID, MUIV_Application_ReturnID_Quit);

  BOOL window_open = TRUE;
  ULONG signals;
  while(window_open) {    
    ULONG id = DoMethod(application, MUIM_Application_Input, &signals);
    
    if(MUIV_Application_ReturnID_Quit == id) {
      window_open = FALSE;
    }
    
    if(signals && window_open) {
      Wait(signals);
    }
  }
  
  set(window, MUIA_Window_Open, FALSE);
    
  MUI_DisposeObject(application);
  closeLibraries();
}
