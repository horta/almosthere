cmake_minimum_required(VERSION 3.8.2 FATAL_ERROR)
project(athr VERSION 1.0.8 LANGUAGES C)
set(PROJECT_DESCRIPTION "Progress indicator library written in C.")

# Generate compile_commands.json file
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

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

find_package(LibRt)

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

target_include_directories(athr
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
set_target_properties(athr PROPERTIES PUBLIC_HEADER include/athr.h)
set_target_properties(athr PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(athr PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})
target_compile_features(athr PRIVATE c_std_11)
set_target_properties(athr PROPERTIES DEFINE_SYMBOL "ATHR_EXPORTS")
target_compile_options(athr PRIVATE
    $<$<STREQUAL:${CMAKE_C_COMPILER_ID},MSVC>:/W3>
    $<$<NOT:$<STREQUAL:${CMAKE_C_COMPILER_ID},MSVC>>:-Wall -Wextra -pedantic -Wstrict-prototypes
        -Wshadow -Wconversion -Wmissing-prototypes -Wno-unused-parameter>
)

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

enable_testing()
add_subdirectory(test)
