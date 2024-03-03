#include "dos/dos.h"
#include "envmondata.h"
#include "exec/types.h"
#include "intuition/classusr.h"
#include "proto/muimaster.h"
#include "libraries/mui.h"
#include "libraries/gadtools.h"
#include "proto/exec.h"
#include <stdio.h>
#include <stdlib.h>

#include "amiga_libraries.h"
#include "settings.h"

#ifdef __amigaos4__
#include "inline4/muimaster.h"
#endif

struct EnvironmentMonitorData* envmondata;

struct ApplicationSettings* application_settings;

void GUI_SetEnvMonData(struct EnvironmentMonitorData *data) {
  envmondata = data;
}

// TODO: is this the best way to provide the settings to the GUI? Same with the env mon data..?
void GUI_SetSettings(struct ApplicationSettings* settings) {
  application_settings = settings;
}

#define MAX_STRING_LENGTH 10

static char getTextAlignmentControlCharFromEnum(enum TextBoxAlignment alignment) {
  switch (alignment) {
  case LEFT:
    return 'l';
  case RIGHT:
    return 'r';
  case CENTER:
    return 'c';
  }

  // wtf can we even return in this case lol
  return ' ';
}

// TODO: prettify. Always a bit sceptical of global variable shit
// also, free it yourself biatch
static char *createTemperatureString() {
  // keycode found from:
  // https://wiki.amigaos.net/wiki/Keymap_Library
  static const char degree_symbol = (char)176;
  char alignment_character = getTextAlignmentControlCharFromEnum(application_settings->number_boxes_alignment);
  char* result = calloc(MAX_STRING_LENGTH + 1, sizeof(char));
  snprintf(result, MAX_STRING_LENGTH, "\33%c%.1f %cC", alignment_character, envmondata->temperature, degree_symbol);
  return result;
}

static char *createECO2String() {
  char alignment_character = getTextAlignmentControlCharFromEnum(application_settings->number_boxes_alignment);
  char* result = calloc(MAX_STRING_LENGTH + 1, sizeof(char));
  snprintf(result, MAX_STRING_LENGTH, "\33%c%d ppm", alignment_character, envmondata->eCO2);
  return result;
}

#define MENUSTRIP_QUIT 1

static struct NewMenu menustrip[] = {
  { NM_TITLE, "File", 0, 0, 0, 0 },
  { NM_ITEM, "Quit", "Q", 0, 0, MENUSTRIP_QUIT },
  { NM_END, NULL, 0, 0, 0, 0 }
};

// needed globally for things like arexx commands
APTR application;

#ifndef __amigaos4__ 
// ARexx commands/ports for the application (OS3 only)
// TODO: see if this fits better in another place
static APTR arexxGetTemperature(struct Hook *hook,
                                Object *obj,
                                ULONG *arg) {
  // yolo on the error checking for now
  char* result = calloc(MAX_STRING_LENGTH + 1, sizeof(char));
  snprintf(result, MAX_STRING_LENGTH, "%0.1f", envmondata->temperature);
  set(application, MUIA_Application_RexxString, result);
  free(result);
  return 0;
}

static APTR arexxGetECO2(struct Hook *hook,
                         Object *obj,
                         ULONG *arg) {
  // yolo on the error checking for now
  char* result = calloc(MAX_STRING_LENGTH + 1, sizeof(char));
  snprintf(result, MAX_STRING_LENGTH, "%d", envmondata->eCO2);
  set(application, MUIA_Application_RexxString, result);
  free(result);
  return 0;
}

static APTR arexxGetHumidity(struct Hook *hook,
                             Object *obj,
                             ULONG *arg) {
  // yolo on the error checking for now
  char* result = calloc(MAX_STRING_LENGTH + 1, sizeof(char));
  snprintf(result, MAX_STRING_LENGTH, "%0.1f", envmondata->humidity);
  set(application, MUIA_Application_RexxString, result);
  free(result);
  return 0;
}

static struct Hook arexx_get_temperature_hook = {{NULL, NULL}, (void*)arexxGetTemperature, NULL, NULL};
static struct Hook arexx_get_eco2_hook = {{NULL, NULL}, (void *)arexxGetECO2, NULL, NULL};
static struct Hook arexx_get_humidity_hook = {{NULL, NULL}, (void*)arexxGetHumidity, NULL, NULL};

static struct MUI_Command arexx_commands[] = {
  {"temperature", NULL, 0, &arexx_get_temperature_hook},
  {"eco2", NULL, 0, &arexx_get_eco2_hook},
  {"humidity", NULL, 0, &arexx_get_humidity_hook},
  { NULL, NULL, NULL, NULL}
};
#endif

void GUI_Draw() {
  Amiga_InitLibraries();

  APTR window;
  
  char* temperature_string = createTemperatureString();
  char* eco2_string = createECO2String();
  
  application = ApplicationObject,
    MUIA_Application_Title, "AmiEnvMon",
    MUIA_Application_Version, "$VER: 0.0.1",
    MUIA_Application_Description, "Simple environment monitor for Amiga",
    MUIA_Application_Base, "AMIENVMON",
    MUIA_Application_Commands, arexx_commands,
    MUIA_Application_Menustrip, MUI_MakeObject(MUIO_MenustripNM, menustrip, 0),
    
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
        Child, ImageObject,
          MUIA_Image_Spec, (long)"5:amienvmon.png",
          MUIA_FixWidth, 250,
          MUIA_FixHeight, 100,
          // TODO: these have to be set for some reason, or else nothing shows
          MUIA_Image_FreeVert, TRUE,
          MUIA_Image_FreeHoriz, TRUE,
        End,
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

  #ifdef __amigaos4__
  IIntuition->set(window, MUIA_Window_Open, TRUE);
  #else
  set(window, MUIA_Window_Open, TRUE);
  #endif
  
  #ifdef __amigaos4__
  IIntuition->IDoMethod(application, MUIM_Application_Run);
  IIntuition->IDoMethod(window, MUIM_Notify, MUIA_Window_CloseRequest, TRUE, application, 2, MUIM_Application_ReturnID, MUIV_Application_ReturnID_Quit);
  #else
  DoMethod(application, MUIM_Application_Run);
  DoMethod(window, MUIM_Notify, MUIA_Window_CloseRequest, TRUE, application, 2, MUIM_Application_ReturnID, MUIV_Application_ReturnID_Quit);
  #endif

  BOOL window_open = TRUE;
  ULONG signals;
  while(window_open) {
#ifdef __amigaos4__
    ULONG id = IIntuition->IDoMethod(application, MUIM_Application_Input, &signals);
#else
    ULONG id = DoMethod(application, MUIM_Application_Input, &signals);
#endif
    
    if(MUIV_Application_ReturnID_Quit == id || id == MENUSTRIP_QUIT) {
      window_open = FALSE;
    }
    
    if(signals && window_open) {
#ifdef __amigaos4__
      IExec->Wait(signals);
#else
      Wait(signals);
#endif
    }
  }


  #ifdef __amigaos4__
  IIntuition->set(window, MUIA_Window_Open, FALSE);
  #else
  set(window, MUIA_Window_Open, FALSE);
  #endif
  
  MUI_DisposeObject(application);
  Amiga_CloseLibraries();
}
