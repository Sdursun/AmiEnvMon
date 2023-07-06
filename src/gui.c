#include "proto/muimaster.h"
#include "libraries/mui.h"
#include "proto/exec.h"

// need to open these ourselves when using MUI with clib2
struct Library* MUIMasterBase;
struct IntuitionBase* IntuitionBase;

static void initLibraries() {
  // TODO: why isn't mui version 20 working at my end?
  MUIMasterBase = OpenLibrary("muimaster.library", MUIMASTER_VMIN);
  IntuitionBase = (struct IntuitionBase*)OpenLibrary("intuition.library", 37);
}

static void closeLibraries() {
  CloseLibrary(IntuitionBase);
  CloseLibrary(MUIMasterBase);
}

void draw() {
  initLibraries();

  APTR application;
  APTR window;

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
        Child, TextObject,
          MUIA_Text_Contents, "  ~ Yay, working ~",
          MUIA_Text_SetVMax, FALSE,
        End,
      End,
    End,
  End;

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
