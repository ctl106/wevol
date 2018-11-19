# example code used older "include_directories()", and I am trying out
# "target_include_directories()", which I think goes into the same
# CMakeLists.txt file as builds the target being used (winlog)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)

# If pkg-config is not installed on the system, then the
# CHECK_INSTALL_DIR variable must be set to the install
# location of Check. For example, on Windows, this may
# be: C:/Program Files/check
# set(CHECK_INSTALL_DIR "C:/Program Files/check")

find_package(Threads REQUIRED)
find_package(Check REQUIRED)
include_directories(${CHECK_INCLUDE_DIRS})

set(TEST_HEADERS
  check_main.h
)

set(TEST_SOURCES
  check_evtx.c
  check_evt.c
  check_winlog.c
  check_main.c
)

add_executable(check_main ${TEST_HEADERS} ${TEST_SOURCES})
target_link_libraries(check_main winlog ${CHECK_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
