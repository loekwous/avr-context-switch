add_library(OsHeap OBJECT)
target_sources(OsHeap PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/OsHeap.cpp")

add_library(OsScheduler STATIC)
target_sources(OsScheduler PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Scheduler.cpp")
target_include_directories(OsScheduler INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/")
set_target_properties(
  OsScheduler
  PROPERTIES
    PUBLIC_HEADER
    "${CMAKE_CURRENT_SOURCE_DIR}/Scheduler.h;${CMAKE_CURRENT_SOURCE_DIR}/OsConfig.h;${CMAKE_CURRENT_SOURCE_DIR}/Tasks.h"
)
target_link_libraries(OsScheduler PRIVATE OsHeap)
