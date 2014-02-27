PROJECT(HapDebugger)

# --------------------------------------------------------------------------
# Sources

SET (HapDebugger_SRCS
  AboutDlg.cpp 
  AboutDlg.h 
  AppOptionsMainPg.cpp 
  AppOptionsMainPg.h 
  BreakpointDlg.cpp 
  BreakpointDlg.h 
  BrowseSelDlg.cpp 
  BrowseSelDlg.h 
  ChildFrm.cpp 
  ChildFrm.h 
  ConnectSettingsDlg.cpp 
  ConnectSettingsDlg.h 
  ConnectStatusBar.cpp 
  ConnectStatusBar.h 
  DataDisplayDlg.cpp 
  DataDisplayDlg.h 
  DbgRemote.cpp 
  DbgRemote.h 
  DbgRemoteDoc.cpp 
  DbgRemoteDoc.h 
  DbgRemoteView.cpp 
  DbgRemoteView.h 
  DebugOptionsPage.cpp 
  DebugOptionsPage.h 
  DocSelectDlg.cpp 
  DocSelectDlg.h 
  EditorOptsPg.cpp 
  EditorOptsPg.h 
  ExceptionHandlerHook.cpp 
  FileFindDlg.cpp 
  FileFindDlg.h 
  FileGroup.cpp 
  FileGroup.h 
  FileModifiedDlg.cpp 
  FileModifiedDlg.h 
  FilePropDlg.cpp 
  FilePropDlg.h 
  FindInProjectDlg.cpp 
  FindInProjectDlg.h 
  GotoLineDlg.cpp 
  GotoLineDlg.h 
  GroupPropertiesDlg.cpp 
  GroupPropertiesDlg.h 
  HapDebugger.vcproj 
  MainFrm.cpp MainFrm.h 
  OptionsDlg.cpp 
  OptionsDlg.h 
  PdpManager.cpp 
  PdpManager.h 
  ProjectSettingsDlg.cpp 
  ProjectSettingsDlg.h 
  ProjectViewBar.cpp 
  ProjectViewBar.h 
  RegistryOptionsPg.cpp 
  RegistryOptionsPg.h 
  resource.h 
  RunOptionsDlg.cpp 
  RunOptionsDlg.h 
  scbarg.cpp 
  scbarg.h 
  SccGeneric.h 
  sizecbar.cpp 
  sizecbar.h 
  SourceControlInterface.cpp 
  SourceControlInterface.h 
  StackViewBar.cpp 
  StackViewBar.h 
  StdAfx.h 
  Tags.cpp 
  Tags.h 
  TextFindDlg.cpp 
  TextFindDlg.h 
  TextReplaceDlg.cpp 
  TextReplaceDlg.h 
  ToolConfigDlg.cpp 
  ToolConfigDlg.h 
  TraceViewBar.cpp 
  TraceViewBar.h 
  UserCfg.cpp 
  UserCfg.h 
  VariableViewBar.cpp 
  VariableViewBar.h 
  varlistctrl.cpp 
  varlistctrl.h 
  WatchBar.cpp 
  WatchBar.h 
  WatchBar.h 
  )

if (WIN32)
  SET (HapDebugger_SRCS
    ${HapDebugger_SRCS}
    DbgRemote.rc
    )
endif (WIN32)	

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

# --------------------------------------------------------------------------
# Build the library

SET (HapDebugger_LIBS 
  hecore
  DbgNetLib
  expat
  )

# shared MFC dll
set( CMAKE_MFC_FLAG 2 )

LINK_DIRECTORIES("${PYTHON_INCLUDE_DIR}/../libs")

ADD_EXECUTABLE( HapDebugger WIN32 ${HapDebugger_SRCS} )
set_target_properties(HapDebugger PROPERTIES
  COMPILE_DEFINITIONS _AFXDLL,_MBCS
  )

#set_target_properties(HapDebugger PROPERTIES
    #COMPILE_DEFINITIONS _AFXDLL,_UNICODE,UNICODE,_BIND_TO_CURRENT_CRT_VERSION,_BIND_TO_CURRENT_MFC_VERSION
    #LINK_FLAGS "/ENTRY:\"wWinMainCRTStartup\"")

TARGET_LINK_LIBRARIES( HapDebugger ${HapDebugger_LIBS} )
