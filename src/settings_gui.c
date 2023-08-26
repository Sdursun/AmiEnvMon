#include "exec/types.h"
#include "proto/muimaster.h"
#include "libraries/mui.h"
#include "proto/exec.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

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


static void draw() {
  APTR application;
  APTR window;
   
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
          Child, StringObject,
            MUIA_String_Contents, "10.0.0.7",
            // Maximum length of an IP address is 15 characters
            MUIA_String_MaxLen, 15,
          End,
        End,
        // TODO: something like a CycleObject to select the alignment setting?
        // TODO: Mui knob to select if the application should continously refresh or not
        Child, HGroup,
          Child, SimpleButton("Save and exit"),
          Child, SimpleButton("Cancel"),
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
}

int main(int argc, char *argv[]) {
  // read current settings if available
  application_settings = Settings_ReadFromFile("amienvmon_settings");

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

