set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(glibc_SOURCES
  regex.cpp
)
include_directories(../gall)
ADD_LIBRARY(glibc STATIC ${glibc_SOURCES})
