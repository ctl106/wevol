set(LIB_SOURCES
  evtx.c
  evt.c
  winlog.c
)

set(MAIN_SOURCES
  main.c
)

set(HEADERS 
  ${CONFIG_HEADER}
  evtx.h
  evt.h
  winlog.h
)

add_library(winlog STATIC ${LIB_SOURCES} ${HEADERS})
target_include_directories(winlog INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(main ${HEADERS} ${MAIN_SOURCES})
target_link_libraries(main winlog)

install(TARGETS winlog
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

install(
  FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/evtx.h
    ${CMAKE_CURRENT_SOURCE_DIR}/evt.h
    ${CMAKE_CURRENT_SOURCE_DIR}/winlog.h
  DESTINATION include)
