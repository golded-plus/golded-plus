set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(smblib_SOURCES
  lzh.cpp
  smblib.cpp
)
include_directories(../gall)
ADD_LIBRARY(smblib STATIC ${smblib_SOURCES})