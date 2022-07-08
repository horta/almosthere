cmake_minimum_required(VERSION 3.16.3 FATAL_ERROR)
project(
  athr
  VERSION 4.0.1
  LANGUAGES C)
set(PROJECT_DESCRIPTION "Progress indicator library written in C.")

# Generate compile_commands.json file
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(cmake/compiler-options.cmake)
include(cmake/sanitizers.cmake)
include(cmake/CPM.cmake)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

cpmaddpackage(NAME elapsed VERSION 3.0.6 GITHUB_REPOSITORY horta/elapsed)

find_package(Threads REQUIRED)
find_package(Curses)
if(UNIX AND NOT APPLE)
  find_package(LibRt)
endif()

# Include file check macros honor CMAKE_REQUIRED_LIBRARIES
if(CMAKE_VERSION VERSION_GREATER 3.12.0)
  cmake_policy(SET CMP0075 NEW)
endif()

if(LIBRT_FOUND)
  set(CMAKE_REQUIRED_LIBRARIES ${LIBRT_LIBRARIES})
endif()

if(WIN32)
  set(CMAKE_REQUIRED_INCLUDES "windows.h")
endif()

include(CheckFunctionExists)
include(CheckLibraryExists)
include(CheckIncludeFile)

check_include_file(sys/ioctl.h HAVE_SYS_IOCTL_H)
check_include_file(stdnoreturn.h HAVE_STDNORETURN_H)
check_include_file(unistd.h HAVE_UNISTD_H)

if(CURSES_FOUND)
  set(TERMINAL_CURSES TRUE)
  set(TERMINAL_WIN32 FALSE)
  set(TERMINAL_IOCTL FALSE)
elseif(WIN32)
  set(TERMINAL_CURSES FALSE)
  set(TERMINAL_WIN32 TRUE)
  set(TERMINAL_IOCTL FALSE)
elseif(HAVE_SYS_IOCTL_H)
  set(TERMINAL_CURSES FALSE)
  set(TERMINAL_WIN32 FALSE)
  set(TERMINAL_IOCTL TRUE)
else()
  set(TERMINAL_CURSES FALSE)
  set(TERMINAL_WIN32 FALSE)
  set(TERMINAL_IOCTL FALSE)
endif()

add_library(
  athr
  $<$<BOOL:${TERMINAL_CURSES}>:src/term_curses.c>
  $<$<BOOL:${TERMINAL_WIN32}>:src/term_win32.c>
  $<$<BOOL:${TERMINAL_IOCTL}>:src/term_ioctl.c>
  src/athr.c
  src/canvas.c
  src/deprecated.c
  src/logger.c
  src/terminal.c
  src/thr.c
  src/widget_bar.c
  src/widget_eta.c
  src/widget_main.c
  src/widget_perc.c
  src/widget_text.c)
add_library(ATHR::athr ALIAS athr)
target_include_directories(
  athr
  PUBLIC $<INSTALL_INTERFACE:include>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
set_target_properties(athr PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(athr PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})
target_compile_options(athr PRIVATE ${WARNING_FLAGS})
target_compile_features(athr PRIVATE c_std_11)
target_compile_definitions(
  athr PRIVATE $<$<BOOL:${TERMINAL_CURSES}>:TERMINAL_CURSES>)
target_compile_definitions(athr
                           PRIVATE $<$<BOOL:${TERMINAL_WIN32}>:TERMINAL_WIN32>)
target_compile_definitions(athr
                           PRIVATE $<$<BOOL:${TERMINAL_IOCTL}>:TERMINAL_IOCTL>)
target_compile_definitions(athr
                           PRIVATE $<$<BOOL:${WIN32}>:_CRT_SECURE_NO_WARNINGS>)
target_compile_definitions(
  athr PRIVATE $<$<BOOL:${WIN32}>:_CRT_NONSTDC_NO_DEPRECATE>)
target_compile_definitions(
  athr PRIVATE $<$<BOOL:${HAVE_STDNORETURN_H}>:HAVE_STDNORETURN_H>)
target_compile_definitions(athr
                           PRIVATE $<$<BOOL:${HAVE_UNISTD_H}>:HAVE_UNISTD_H>)

if(CURSES_FOUND)
  target_compile_definitions(athr PRIVATE CURSES_FOUND)
  target_include_directories(athr PRIVATE ${CURSES_INCLUDE_DIRS})
  target_link_libraries(athr PUBLIC ${CURSES_LIBRARIES})
endif()

target_link_libraries(athr PUBLIC Threads::Threads)
target_link_libraries(athr PUBLIC ELAPSED::elapsed)
target_link_libraries(athr PUBLIC $<$<BOOL:${LIBRT_FOUND}>:LIBRT::LIBRT>)

# Write out athr/export.h to the current binary directory.
include(GenerateExportHeader)
generate_export_header(
  athr
  BASE_NAME
  ATHR
  EXPORT_MACRO_NAME
  ATHR_API
  EXPORT_FILE_NAME
  ${CMAKE_CURRENT_BINARY_DIR}/athr/export.h)

install(
  TARGETS athr
  EXPORT athr-targets
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  PUBLIC_HEADER DESTINATION include)
install(DIRECTORY src/athr/ DESTINATION include/athr)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/athr/export.h
        DESTINATION include/athr)
install(
  EXPORT athr-targets
  FILE athr-targets.cmake
  NAMESPACE ATHR::
  DESTINATION lib/cmake/athr)
include(CMakePackageConfigHelpers)
configure_package_config_file(
  athr-config.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/athr-config.cmake
  INSTALL_DESTINATION lib/cmake/athr
  NO_SET_AND_CHECK_MACRO)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/athr-config.cmake
        DESTINATION lib/cmake/athr)
install(FILES ${CMAKE_CURRENT_LIST_DIR}/cmake/FindLibRt.cmake
        DESTINATION lib/cmake/athr/librt)

enable_testing()
add_subdirectory(test)

set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VENDOR "Danilo Horta")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Progress indicator library written in C")
set(CPACK_PACKAGE_CONTACT "Danilo Horta")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.md)
set(CPACK_RESOURCE_FILE_README ${CMAKE_CURRENT_SOURCE_DIR}/README.md)
set(CPACK_OUTPUT_FILE_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/package)
set(CPACK_VERBATIM_VARIABLES YES)
set(CPACK_PACKAGE_RELOCATABLE YES)
set(CPACK_MONOLITHIC_INSTALL YES)
include(CPack)
