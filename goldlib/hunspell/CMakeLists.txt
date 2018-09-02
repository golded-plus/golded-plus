set(hunspell_SOURCES
  affentry.cxx
  affixmgr.cxx
  csutil.cxx
  dictmgr.cxx
  hashmgr.cxx
  hunspell.cxx
  suggmgr.cxx
  utf_info.cxx
)
ADD_LIBRARY(hunspell STATIC ${hunspell_SOURCES})