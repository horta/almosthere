cmake_minimum_required(VERSION 3.4 FATAL_ERROR)
include(cmake/limix.cmake)
limix_initialise()
project(${PROJECT_NAME} VERSION ${PROJECT_VERSION} LANGUAGES C)
limix_config()

set(CMAKE_C_STANDARD 11)
add_definitions(-D_XOPEN_SOURCE=500 -D_POSIX_C_SOURCE=200112L)

set(LIBS "")

find_package(Curses)
if (CURSES_FOUND)
  include_directories(${CURSES_INCLUDE_DIRS})
  add_definitions(-DCURSES_FOUND)
  list(APPEND LIBS ${CURSES_LIBRARIES})
endif()

find_package(Threads)
if (THREADS_FOUND)
  list(APPEND LIBS ${CMAKE_THREAD_LIBS_INIT})
endif()

if (UNIX AND NOT APPLE)
  list(APPEND LIBS rt)
endif()

limix_process_default_dirs()

limix_add_library(${PROJECT_NAME} ${PROJECT_VERSION}
  "${SOURCES}" "${HEADERS}" "${LIBS}")

add_subdirectory(test)
