cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(athr VERSION 1.0.15 LANGUAGES C)
set(PROJECT_DESCRIPTION "Progress indicator library written in C.")

# Generate compile_commands.json file
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
# Set RPATH only if it's not a system directory
list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
if("${isSystemDir}" STREQUAL "-1")
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# merge request fix: https://gitlab.kitware.com/cmake/cmake/-/merge_requests/1799
if(CMAKE_VERSION VERSION_GREATER 3.12.0)
    # Hide things by default
    set(CMAKE_C_VISIBILITY_PRESET hidden)
    set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)
    set(ATHR_USE_OLD_EXPORT 0)
else()
    set(ATHR_USE_OLD_EXPORT 1)
endif()

find_package(Threads REQUIRED)

# We use curses to fetch terminal width.
find_package(Curses)

if (CURSES_FOUND)
    set(TERMINAL_CURSES TRUE)
    set(TERMINAL_DUMMY FALSE)
    set(TERMINAL_WIN32 FALSE)
elseif (WIN32)
    set(TERMINAL_CURSES FALSE)
    set(TERMINAL_DUMMY FALSE)
    set(TERMINAL_WIN32 TRUE)
else ()
    set(TERMINAL_CURSES FALSE)
    set(TERMINAL_DUMMY TRUE)
    set(TERMINAL_WIN32 FALSE)
endif()

if (UNIX AND NOT APPLE)
    find_package(LibRt)
endif()

include(CheckSymbolExists)

check_symbol_exists(gettimeofday sys/time.h HAVE_GETTIMEOFDAY)

add_library(athr
    $<$<BOOL:${HAVE_GETTIMEOFDAY}>:src/clock_gettime.c>
    $<$<BOOL:${TERMINAL_CURSES}>:src/terminal/curses.c>
    $<$<BOOL:${TERMINAL_DUMMY}>:src/terminal/dummy.c>
    $<$<BOOL:${TERMINAL_WIN32}>:src/terminal/win32.c>
    $<$<NOT:$<BOOL:${CMAKE_USE_PTHREADS_INIT}>>:src/thread/tinycthread.c>
    src/athr.c
    src/athr_time.c
    src/canvas.c
    src/report.c
    src/widget/bar.c
    src/widget/eta.c
    src/widget/line.c
    src/widget/perc.c
    src/widget/text.c
)
add_library(ATHR::athr ALIAS athr)

if(${CMAKE_C_COMPILER_ID} MATCHES "MSVC")
    set(WARNING_FLAGS /W3)
else()
    set(WARNING_FLAGS -Wall -Wextra -pedantic -Wstrict-prototypes -Wshadow
        -Wconversion -Wmissing-prototypes -Wno-unused-parameter -Wsign-conversion
        -Wno-unused-function # TODO: added only to please clangd for now
        -Wno-empty-translation-unit # TODO: added only to please clangd for now
    )
endif()

target_include_directories(athr
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
set_target_properties(athr PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(athr PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})
target_compile_features(athr PRIVATE c_std_99)
if(ATHR_USE_OLD_EXPORT)
    target_compile_definitions(athr PUBLIC ATHR_USE_OLD_EXPORT=1)
endif()

if(NOT ATHR_USE_OLD_EXPORT)
    # Write out mytools_export.h to the current binary directory
    # Must include ${CMAKE_CURRENT_BINARY_DIR} in the target's
    # search path.
    include(GenerateExportHeader)
    generate_export_header(athr BASE_NAME ATHR
        INCLUDE_GUARD_NAME ATHR_EXPORT_H
        EXPORT_MACRO_NAME ATHR_API
        EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/athr/export.h)
endif()

# It tells your compiler to include definitions for some extra functions that are defined in the
# X/Open and POSIX standards.
target_compile_definitions(athr PRIVATE _XOPEN_SOURCE=500)
set(CMAKE_REQUIRED_DEFINITIONS -D_XOPEN_SOURCE=500)

# If you define this macro to a value greater than or equal to 200112L, then the functionality from
# the 2001 edition of the POSIX standard (IEEE Standard 1003.1-2001) is made available.
target_compile_definitions(athr PRIVATE _POSIX_C_SOURCE=200112L)
set(CMAKE_REQUIRED_DEFINITIONS ${CMAKE_REQUIRED_DEFINITIONS};-D_POSIX_C_SOURCE=200112L)

if (CURSES_FOUND)
    target_compile_definitions(athr PRIVATE CURSES_FOUND)
    target_include_directories(athr PRIVATE ${CURSES_INCLUDE_DIRS})
    target_link_libraries(athr PUBLIC ${CURSES_LIBRARIES})
endif()

target_link_libraries(athr PUBLIC Threads::Threads)
target_compile_definitions(athr PRIVATE $<$<BOOL:${CMAKE_USE_PTHREADS_INIT}>:USE_PTHREADS>)
target_link_libraries(athr PUBLIC $<$<BOOL:${LIBRT_FOUND}>:LIBRT::LIBRT>)

# Include file check macros honor CMAKE_REQUIRED_LIBRARIES
cmake_policy(SET CMP0075 NEW)
if (LIBRT_FOUND)
    set(CMAKE_REQUIRED_LIBRARIES ${LIBRT_LIBRARIES})
endif()

include(CheckCSourceCompiles)
check_c_source_compiles(
    "
        static void f(char const* err, ...) __attribute__((format(printf, 1, 2)));
        int main(void) {return 0;}
    "
    HAVE_ATTR_FORMAT
)
target_compile_definitions(athr PRIVATE $<$<BOOL:${HAVE_ATTR_FORMAT}>:HAVE_ATTR_FORMAT>)

include(CheckFunctionExists)
include(CheckLibraryExists)
include(CheckIncludeFile)

check_include_file(windows.h HAVE_WINDOWS_H)
target_compile_definitions(athr PRIVATE $<$<BOOL:${HAVE_WINDOWS_H}>:HAVE_WINDOWS_H>)

check_include_file(unistd.h HAVE_UNISTD_H)
target_compile_definitions(athr PRIVATE $<$<BOOL:${HAVE_UNISTD_H}>:HAVE_UNISTD_H>)

check_symbol_exists(timespec_get time.h HAVE_TIMESPEC_GET)
target_compile_definitions(athr PRIVATE $<$<BOOL:${HAVE_TIMESPEC_GET}>:HAVE_TIMESPEC_GET>)

check_symbol_exists(clock_gettime time.h HAVE_CLOCK_GETTIME)
target_compile_definitions(athr PRIVATE $<$<BOOL:${HAVE_CLOCK_GETTIME}>:HAVE_CLOCK_GETTIME>)

if (NOT(HAVE_CLOCK_GETTIME OR HAVE_TIMESPEC_GET))
    message(WARNING "Neiher clock_gettime nor timespec_get could be found.")
endif()

install(TARGETS athr EXPORT athr-targets
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    PUBLIC_HEADER DESTINATION include
)
install(DIRECTORY include/ DESTINATION include)
if(NOT ATHR_USE_OLD_EXPORT)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/athr/export.h
        DESTINATION include/athr
    )
endif()
install(EXPORT athr-targets
    FILE athr-targets.cmake
    NAMESPACE ATHR::
    DESTINATION lib/cmake/athr
)
include(CMakePackageConfigHelpers)
configure_package_config_file(athr-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/athr-config.cmake
    INSTALL_DESTINATION lib/cmake/athr
    NO_SET_AND_CHECK_MACRO
)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/athr-config.cmake
    DESTINATION lib/cmake/athr
)
install(FILES ${CMAKE_CURRENT_LIST_DIR}/cmake/FindLibRt.cmake
    DESTINATION lib/cmake/athr/librt
)

enable_testing()
add_subdirectory(test)
