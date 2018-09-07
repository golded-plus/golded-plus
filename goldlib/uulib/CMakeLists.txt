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
include_directories(../gall ../glibc)
ADD_LIBRARY(uulib STATIC ${uulib_SOURCES})
target_compile_definitions(uulib PRIVATE -DVERSION="15" -DPATCH="0.5")