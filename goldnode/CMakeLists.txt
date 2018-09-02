set(goldnode_SOURCES
  goldnode.cpp
  ../golded3/golded3.cpp
)
add_executable(goldnode ${goldnode_SOURCES})
target_link_libraries(goldnode gall)
include_directories(../goldlib/gall ../golded3)

INSTALL(TARGETS goldnode
  RUNTIME DESTINATION bin
)