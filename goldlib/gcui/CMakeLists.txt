set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(gcui_SOURCES
  gkbdbase.cpp
  gkbdgetm.cpp
  gkbdwait.cpp
  gsrchmgr.cpp
  gmoubase.cpp
  gvidbase.cpp
  gvidinit.cpp
  gwinbase.cpp
  gwindow.cpp
  gwinhlp1.cpp
  gwinhlp2.cpp
  gwininit.cpp
  gwinline.cpp
  gwinmenu.cpp
  gwinmnub.cpp
  gwinpckf.cpp
  gwinpcks.cpp
  gwinpick.cpp
  gwinput2.cpp
)
include_directories("../gall")
ADD_LIBRARY(gcui STATIC ${gcui_SOURCES})
