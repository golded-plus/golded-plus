cmake_minimum_required(VERSION 2.8.11)
project(golded-plus C CXX)

include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckIncludeFiles)
include(CheckSymbolExists)

CHECK_SYMBOL_EXISTS(snprintf stdio.h HAVE_SNPRINTF)
if (${HAVE_SNPRINTF})
  add_definitions(-DHAVE_SNPRINTF=1)
endif()
CHECK_SYMBOL_EXISTS(vsnprintf stdio.h HAVE_VSNPRINTF)
if (${HAVE_VSNPRINTF})
  add_definitions(-DHAVE_VSNPRINTF=1)
endif()
CHECK_INCLUDE_FILE("stdarg.h" HAVE_STDARG_H)
if (${HAVE_STDARG_H})
  add_definitions(-DHAVE_STDARG_H=1)
endif()
CHECK_INCLUDE_FILE("string.h" HAVE_STRING_H)
if (${HAVE_STRING_H})
  add_definitions(-DHAVE_STRING_H=1)
endif()
CHECK_INCLUDE_FILE("malloc.h" HAVE_MALLOC_H)
if (${HAVE_MALLOC_H})
  add_definitions(-DHAVE_MALLOC_H=1)
endif()
CHECK_INCLUDE_FILE("time.h" HAVE_TIME_H)
if (${HAVE_TIME_H})
  add_definitions(-DHAVE_TIME_H=1)
endif()
CHECK_INCLUDE_FILE("errno.h" HAVE_ERRNO_H)
if (${HAVE_ERRNO_H})
  add_definitions(-DHAVE_ERRNO_H=1)
endif()
CHECK_INCLUDE_FILE("fcntl.h" HAVE_FCNTL_H)
if (${HAVE_FCNTL_H})
  add_definitions(-DHAVE_FCNTL_H=1)
endif()
CHECK_INCLUDE_FILE("io.h" HAVE_IO_H)
if (${HAVE_IO_H})
  add_definitions(-DHAVE_IO_H=1)
endif()
CHECK_INCLUDE_FILE("unistd.h" HAVE_UNISTD_H)
if (${HAVE_UNISTD_H})
  add_definitions(-DHAVE_UNISTD_H=1)
endif()
CHECK_INCLUDE_FILES("stdarg.h;stdlib.h;stddef.h;string.h" HAVE_STDC_HEADERS)
if (${HAVE_STDC_HEADERS})
  add_definitions(-DSTDC_HEADERS=1)
endif()
check_include_files("time.h;sys/time.h" TIME_WITH_SYS_TIME)
if (${TIME_WITH_SYS_TIME})
  add_definitions(-DTIME_WITH_SYS_TIME=1)
endif()
include(FindCurses)
if (CURSES_NCURSES_LIBRARY)
  add_definitions(-D__USE_NCURSES__)
endif()

if (MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /J")
  add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS -D_ALLOW_RTCc_IN_STL  -D_CONSOLE -D__INCLUDE_NEW_KEYWORDS__)
  set(CompilerFlags
    CMAKE_CXX_FLAGS
    CMAKE_CXX_FLAGS_DEBUG
    CMAKE_CXX_FLAGS_RELEASE
    CMAKE_C_FLAGS
    CMAKE_C_FLAGS_DEBUG
    CMAKE_C_FLAGS_RELEASE
  )
  foreach(CompilerFlag ${CompilerFlags})
    string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
  endforeach()
else()
  add_definitions(-funsigned-char -D__INCLUDE_NEW_KEYWORDS__ -DPRAGMA_PACK -Wno-sign-compare -pedantic -Wno-write-strings)
endif()

add_subdirectory(goldlib)
add_subdirectory(golded3)
add_subdirectory(goldnode)
add_subdirectory(rddt)

set (CPACK_STRIP_FILES TRUE)

include(GNUInstallDirs)
install(FILES
  docs/copying
  docs/copying.lib
  DESTINATION ${CMAKE_INSTALL_DOCDIR}
)
install(FILES
  docs/golded.1
  docs/goldnode.1
  docs/rddt.1
  DESTINATION "${CMAKE_INSTALL_MANDIR}/man1"
)
install(DIRECTORY cfgs DESTINATION ${CMAKE_INSTALL_DOCDIR})
if (MSVC)
  set(CPACK_GENERATOR ${CPACK_GENERATOR} 7Z)
else()
  set(CPACK_GENERATOR ${CPACK_GENERATOR} TGZ)
endif()

set (CPACK_PACKAGE_VERSION_MAJOR "1")
set (CPACK_PACKAGE_VERSION_MINOR "1")
file(READ srcdate.h srcdate)
string(REPLACE "\n" ";" LINES ${srcdate})
list(GET LINES 1 LINETWO)
string(SUBSTRING ${LINETWO} 21 8 CPACK_PACKAGE_VERSION_PATCH)
set(CPACK_PACKAGE_VERSION_PATCH "5-${CPACK_PACKAGE_VERSION_PATCH}")
INCLUDE(CPack)
