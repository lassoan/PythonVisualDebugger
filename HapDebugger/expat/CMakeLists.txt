PROJECT(expat)

SET (expat_INCLUDE_DIRS 
  ${expat_SOURCE_DIR} 
  ${expat_BINARY_DIR} 
  )

SET(expat_SRCS
  xmlparse.c
  xmlrole.c
  xmltok.c
  #xmltok_impl.c
  #xmltok_ns.c 
)

INCLUDE_DIRECTORIES( ${expat_INCLUDE_DIRS} )

add_definitions(-DCOMPILED_FROM_DSP)
ADD_LIBRARY(expat STATIC ${expat_SRCS})
