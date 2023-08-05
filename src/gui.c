#include "envmondata.h"
#include "proto/muimaster.h"
#include "libraries/mui.h"
#include "libraries/gadtools.h"
#include "proto/exec.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef __amigaos4__
#include "inline4/muimaster.h"
#endif

// need to open these ourselves when using MUI with clib2
#ifdef __amigaos4__
struct MUIMasterIFace *IMUIMaster;
struct Library* MUIMasterBase;
#else
struct MUIMasterIFace *IMUIMaster;
struct Library* MUIMasterBase;
struct IntuitionBase* IntuitionBase;
#endif

struct EnvironmentMonitorData* envmondata;

static void initLibraries() {
  // TODO: why isn't mui version 20 working at my end?
#ifdef __amigaos4__
  MUIMasterBase = IExec->OpenLibrary("muimaster.library", MUIMASTER_VMIN);
  IMUIMaster = IExec->GetInterface(MUIMasterBase, "main", 1, NULL);
#else
  MUIMasterBase = OpenLibrary("muimaster.library", MUIMASTER_VMIN);
  IntuitionBase = (struct IntuitionBase*)OpenLibrary("intuition.library", 37);
#endif
}

static void closeLibraries() {
#ifdef __amigaos4__
  IExec->DropInterface(IMUIMaster);
  IExec->CloseLibrary(MUIMasterBase);
#else
  CloseLibrary(MUIMasterBase);
  CloseLibrary(IntuitionBase);  
#endif
}

void GUI_SetEnvMonData(struct EnvironmentMonitorData *data) {
  envmondata = data;
}

#define MAX_STRING_LENGTH 8

// TODO: prettify. Always a bit sceptical of global variable shit
// also, free it yourself biatch
static char *createTemperatureString() {
  // keycode found from:
  // https://wiki.amigaos.net/wiki/Keymap_Library
  static const char degree_symbol = 176;
  char* result = calloc(MAX_STRING_LENGTH + 1, sizeof(char));
  snprintf(result, MAX_STRING_LENGTH, "%.1f %cC", envmondata->temperature, degree_symbol);
  return result;
}

static char *createECO2String() {
  char* result = calloc(MAX_STRING_LENGTH + 1, sizeof(char));
  snprintf(result, MAX_STRING_LENGTH, "%d ppm", envmondata->eCO2);
  return result;
}

static struct NewMenu menustrip[] = {
  { NM_TITLE, "File", 0, 0, 0, 0 },
  { NM_ITEM, "Settings", 0, 0, 0, 0 },
  { NM_ITEM, "Quit", "Q", 0, 0, 0 },
  { NM_END, NULL, 0, 0, 0, 0 }
};

void GUI_Draw() {
  initLibraries();

  APTR application;
  APTR window;

  char* temperature_string = createTemperatureString();
  char* eco2_string = createECO2String();
  
  application = ApplicationObject,
    MUIA_Application_Title, "AmiEnvMon",
    MUIA_Application_Version, "$VER: 0.0.1",
    MUIA_Application_Description, "Simple environment monitor for Amiga",
    MUIA_Application_Base, "PROGRAM",
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
    
    if(MUIV_Application_ReturnID_Quit == id) {
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
  closeLibraries();
}
