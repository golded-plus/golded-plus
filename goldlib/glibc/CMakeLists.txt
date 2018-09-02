set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(glibc_SOURCES
  regex.cpp
)
add_definitions(-DHAVE_CONFIG_H)
include_directories(../gall)
ADD_LIBRARY(glibc STATIC ${glibc_SOURCES})
