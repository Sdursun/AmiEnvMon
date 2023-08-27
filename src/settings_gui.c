#include "exec/types.h"
#include "proto/muimaster.h"
#include "libraries/mui.h"
#include "proto/exec.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "envmondata.h"
#include "settings.h"

#ifdef __amigaos4__
#include "inline4/muimaster.h"
#endif

// TODO: maybe there should be a common settings header? that way we can maybe have a struct we read data to (or writes from). Maybe just reading a simple binary file? 


// TODO: see if there is a nice and easy way I can combine settings + main program in the way I want it to work..
//       a bit more duplicate code than I would like. Probably okay to keep it simple...
//       Putting the library init in a common place is probably a good idea

// need to open these ourselves when using MUI with clib2
#ifdef __amigaos4__
struct MUIMasterIFace *IMUIMaster;
struct Library* MUIMasterBase;
#else
struct MUIMasterIFace *IMUIMaster;
struct Library* MUIMasterBase;
struct IntuitionBase* IntuitionBase;
#endif

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


struct ApplicationSettings* application_settings;

#define SAVE_BUTTON_PRESS 1
#define CANCEL_BUTTON_PRESS 2

static APTR ip_input_box;

static void handleSave() {
  // TODO: handle input fields, maybe IP validation(?), and writing to file
  char* ip_input;
  size_t get_result = get(ip_input_box, MUIA_String_Contents, &ip_input);

  
  // TODO: should we free the current one? maybe have a Settings_ function to handle something like this to avoid low level string operations everywhere
  //free(application_settings->ip);
  application_settings->ip = strndup(ip_input, 16);

  Settings_WriteSettingsToFile(application_settings, DEFAULT_SETTINGS_FILENAME);

  // TODO: should we exit once done?
}

static void draw() {
  APTR application;
  APTR window;
  APTR cancel_button;
  APTR save_button;
   
  application = ApplicationObject,
    MUIA_Application_Title, "AmiEnvMon - Settings",
    MUIA_Application_Version, "$VER: 0.0.1",
    MUIA_Application_Description, "Settings for AmiEnvMon",
    MUIA_Application_Base, "PROGRAM",
    
    SubWindow, window = WindowObject,
      MUIA_Window_Title, "Settings - AmiEnvMon",
      MUIA_Window_Activate, TRUE,
      MUIA_Window_Height, 300,
      MUIA_Window_Width, 250,
      MUIA_Window_LeftEdge, 75,
      MUIA_Window_TopEdge, 75,
      MUIA_Window_CloseGadget, TRUE,
      MUIA_Window_DepthGadget, TRUE,
      MUIA_Window_SizeGadget, FALSE,

      WindowContents, VGroup,
        Child, HGroup,
          Child, TextObject,
            MUIA_Text_Contents, "IP/URL:",
          End,
          Child, ip_input_box = StringObject,
            MUIA_String_Contents, application_settings->ip,
            // Maximum length of an IP address is 15 characters
            MUIA_String_MaxLen, 15,
          End,
        End,
        // TODO: something like a CycleObject to select the alignment setting?
        // TODO: Mui knob to select if the application should continously refresh or not
        Child, HGroup,
          Child, save_button = SimpleButton("Save and exit"),
          Child, cancel_button = SimpleButton("Cancel"),
        End,
      End,
    End,
  End;

  #ifdef __amigaos4__
  IIntuition->set(window, MUIA_Window_Open, TRUE);
  #else
  set(window, MUIA_Window_Open, TRUE);
  #endif
  
  #ifdef __amigaos4__
  IIntuition->IDoMethod(application, MUIM_Application_Run);
  IIntuition->IDoMethod(window, MUIM_Notify, MUIA_Window_CloseRequest, TRUE, application, 2, MUIM_Application_ReturnID, MUIV_Application_ReturnID_Quit);
  IIntuition->DoMethod(save_button, MUIM_Notify, MUIA_Pressed, FALSE, application, 2, MUIM_Application_ReturnID, SAVE_BUTTON_PRESS);
  IIntuition->DoMethod(cancel_button, MUIM_Notify, MUIA_Pressed, FALSE, application, 2, MUIM_Application_ReturnID, CANCEL_BUTTON_PRESS);
  #else
  DoMethod(application, MUIM_Application_Run);
  DoMethod(window, MUIM_Notify, MUIA_Window_CloseRequest, TRUE, application, 2, MUIM_Application_ReturnID, MUIV_Application_ReturnID_Quit);
  DoMethod(save_button, MUIM_Notify, MUIA_Pressed, FALSE, application, 2, MUIM_Application_ReturnID, SAVE_BUTTON_PRESS);
  DoMethod(cancel_button, MUIM_Notify, MUIA_Pressed, FALSE, application, 2, MUIM_Application_ReturnID, CANCEL_BUTTON_PRESS);
  #endif

  BOOL window_open = TRUE;
  ULONG signals;
  while(window_open) {
#ifdef __amigaos4__
    ULONG id = IIntuition->IDoMethod(application, MUIM_Application_Input, &signals);
#else
    ULONG id = DoMethod(application, MUIM_Application_Input, &signals);
#endif
    
    if(MUIV_Application_ReturnID_Quit == id || CANCEL_BUTTON_PRESS == id) {
      window_open = FALSE;
    }

    if (SAVE_BUTTON_PRESS == id) {
      handleSave();
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
}

int main(int argc, char *argv[]) {
  // read current settings if available
  application_settings = Settings_ReadFromFile(DEFAULT_SETTINGS_FILENAME);

  // default to some nice defaults if no current settings are created
  if (NULL == application_settings) {
    application_settings = Settings_Default();
  }
  
  // rendering and amiga specific logic
  initLibraries();
  draw();
  closeLibraries();

  // clean up settings data memory
  Settings_Destroy(application_settings);
  
  return 0;
}

