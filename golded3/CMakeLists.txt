set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(golded_SOURCES
  gcalst.cpp
  gcarea.cpp
  gccfgg.cpp
  gccfgg0.cpp
  gccfgg1.cpp
  gccfgg2.cpp
  gccfgg3.cpp
  gccfgg4.cpp
  gccfgg5.cpp
  gccfgg6.cpp
  gccfgg7.cpp
  gccfgg8.cpp
  gckeys.cpp
  gclang.cpp
  gcmisc.cpp
  gealst.cpp
  gearea.cpp
  gecarb.cpp
  gecmfd.cpp
  gectnr.cpp
  gectrl.cpp
  gedoit.cpp
  gedoss.cpp
  geedit.cpp
  geedit2.cpp
  geedit3.cpp
  gefile.cpp
  gefind.cpp
  geglob.cpp
  gehdre.cpp
  geinit.cpp
  geline.cpp
  gelmsg.cpp
  gemain.cpp
  gemenu.cpp
  gemlst.cpp
  gemnus.cpp
  gemrks.cpp
  gemsgs.cpp
  genode.cpp
  geplay.cpp
  gepost.cpp
  geqwks.cpp
  gerand.cpp
  geread.cpp
  geread2.cpp
  gescan.cpp
  gesrch.cpp
  gesoup.cpp
  getpls.cpp
  geusrbse.cpp
  geutil.cpp
  geutil2.cpp
  geview.cpp
  gmarea.cpp
  gehtml.cpp
  golded3.cpp
)
if (MSVC)
  list(APPEND golded_SOURCES gedcyg.rc)
endif()

include_directories(../goldlib/gall ../goldlib/gcui ../goldlib/gmb3 ../goldlib/gcfg ../goldlib/smblib ../goldlib/uulib ../goldlib/glibc)
add_executable(golded ${golded_SOURCES})
target_link_libraries(golded gmb3 gcfg  uulib gall  gcui smblib hunspell)
if (MSVC)
  target_link_libraries(golded glibc winmm.lib)
endif()
if (CURSES_NCURSES_LIBRARY)
  target_link_libraries(golded ${CURSES_NCURSES_LIBRARY})
endif()

INSTALL(TARGETS golded
  RUNTIME DESTINATION bin
)
