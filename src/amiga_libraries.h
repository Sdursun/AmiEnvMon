#ifndef AMIGA_LIBRARIES_H
#define AMIGA_LIBRARIES_H

#include "proto/muimaster.h"
#include "libraries/mui.h"
#include "proto/exec.h"

// need to open these ourselves when using MUI with clib2
#ifdef __amigaos4__
struct MUIMasterIFace *IMUIMaster;
struct Library* MUIMasterBase;
#else
struct MUIMasterIFace *IMUIMaster;
struct Library* MUIMasterBase;
struct IntuitionBase* IntuitionBase;
#endif

extern void Amiga_InitLibraries() {
#ifdef __amigaos4__
  MUIMasterBase = IExec->OpenLibrary("muimaster.library", MUIMASTER_VMIN);
  IMUIMaster = IExec->GetInterface(MUIMasterBase, "main", 1, NULL);
#else
  MUIMasterBase = OpenLibrary("muimaster.library", MUIMASTER_VMIN);
  IntuitionBase = (struct IntuitionBase*)OpenLibrary("intuition.library", 37);
#endif
}

extern void Amiga_CloseLibraries() {
#ifdef __amigaos4__
  IExec->DropInterface(IMUIMaster);
  IExec->CloseLibrary(MUIMasterBase);
#else
  CloseLibrary(MUIMasterBase);
  CloseLibrary(IntuitionBase);  
#endif
}

#endif
