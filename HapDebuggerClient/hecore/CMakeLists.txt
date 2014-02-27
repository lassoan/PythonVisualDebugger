PROJECT(hecore)

SET (hecore_INCLUDE_DIRS 
  ${hecore_SOURCE_DIR} 
  ${hecore_BINARY_DIR} 
  )

SET(hecore_SRCS
  errorhandler.cpp
  errorhandler.h
  fixedassert.cpp
  handleexceptions.cpp
  hecore.h
  hestring.cpp
  hestring.h
  miscwinfunctions.cpp
  precompiled.cpp
  priv_coreprecompiled.h
  randomnumbers.cpp
  randomnumbers.h
  threadcore.cpp
  threadcore.h
)

INCLUDE_DIRECTORIES( ${hecore_INCLUDE_DIRS} )

ADD_LIBRARY(hecore STATIC ${hecore_SRCS})
