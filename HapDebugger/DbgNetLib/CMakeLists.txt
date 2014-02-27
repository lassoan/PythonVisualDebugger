PROJECT(DbgNetLib)

SET (DbgNetLib_INCLUDE_DIRS 
  ${DbgNetLib_SOURCE_DIR} 
  ${DbgNetLib_BINARY_DIR} 
  )

SET(DbgNetLib_SRCS
  HeDbgMsg.cpp
  HeDbgMsg.h
  HeDbgSocket.cpp
  HeDbgSocket.h
  StdAfx.h 
)

INCLUDE_DIRECTORIES( ${DbgNetLib_INCLUDE_DIRS} )

ADD_LIBRARY(DbgNetLib STATIC ${DbgNetLib_SRCS})
