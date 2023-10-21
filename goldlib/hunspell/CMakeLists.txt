set(hunspell_SOURCES
  affentry.cxx
  affixmgr.cxx
  csutil.cxx
  filemgr.cxx
  hashmgr.cxx
  hunspell.cxx
  hunzip.cxx
  phonet.cxx
  replist.cxx
  suggestmgr.cxx
)
ADD_LIBRARY(hunspell STATIC ${hunspell_SOURCES})
