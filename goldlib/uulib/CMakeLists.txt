set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(uulib_SOURCES
  fptools.cpp
  uucheck.cpp
  uuencode.cpp
  uulib.cpp
  uunconc.cpp
  uuscan.cpp
  uustring.cpp
  uuutil.cpp
)
add_definitions(-DHAVE_CONFIG_H)
include_directories(../gall)
ADD_LIBRARY(uulib STATIC ${uulib_SOURCES})
