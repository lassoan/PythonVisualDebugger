PROJECT(HapDbg)

SET (HapDbg_INCLUDE_DIRS 
  ${HapDbg_SOURCE_DIR} 
  ${HapDbg_BINARY_DIR} 
  )

SET(HapDbg_SRCS
  Break.cpp
  Break.h
  Breakpoint.cpp
  Breakpoint.h
  Connect.cpp
  Connect.h
  DbgGlobals.h
  hapdbg.cpp
  hapdbg.h
  HeDbg.cpp
  HeDbg.h
  Init.cpp
  Init.h
  Run.cpp
  Run.h
  StdAfx.h
  VariablesMsg.cpp
  VariablesMsg.h
)

INCLUDE_DIRECTORIES( ${HapDbg_INCLUDE_DIRS} )

ADD_LIBRARY(HapDbg STATIC ${HapDbg_SRCS})
