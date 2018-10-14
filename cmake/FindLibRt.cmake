# FindLibRt
# --------
#
# Find the native realtime includes and library.
#
# IMPORTED Targets
# ^^^^^^^^^^^^^^^^
#
# This module defines :prop_tgt:`IMPORTED` target ``LIBRT::LIBRT``, if
# LIBRT has been found.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# This module defines the following variables:
#
# ::
#
#   LIBRT_INCLUDE_DIRS  - where to find time.h, etc.
#   LIBRT_LIBRARIES     - List of libraries when using librt.
#   LIBRT_FOUND         - True if realtime library found.
#
# Hints
# ^^^^^
#
# A user may set ``LIBRT_ROOT`` to a realtime installation root to tell this
# module where to look.

find_package(PkgConfig)
pkg_check_modules(PC_RT QUIET rt)

# Use pkg-config to get hints about paths
execute_process(COMMAND pkg-config --cflags rt --silence-errors
  COMMAND cut -d I -f 2
  OUTPUT_VARIABLE RT_PKG_CONFIG_INCLUDE_DIRS)
set(RT_PKG_CONFIG_INCLUDE_DIRS "${RT_PKG_CONFIG_INCLUDE_DIRS}" CACHE STRING "Compiler flags for RT library"

find_path(LIBRT_INCLUDE_DIRS
  NAMES time.h
  PATHS ${LIBRT_ROOT}/include
  ${RT_PKG_CONFIG_INCLUDE_DIRS}
  /usr/local/include /usr/include
  ${CMAKE_EXTRA_INCLUDES}
)

find_library(LIBRT_LIBRARIES
  NAMES rt
  PATHS ${LIBRT_ROOT}/lib
  ${PC_RT_LIBDIR} ${PC_RT_LIBRARY_DIRS}
  /usr/local/lib /usr/lib /lib
  ${CMAKE_EXTRA_LIBRARIES}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LibRt DEFAULT_MSG LIBRT_LIBRARIES LIBRT_INCLUDE_DIRS)
mark_as_advanced(LIBRT_INCLUDE_DIRS LIBRT_LIBRARIES)

if(LIBRT_FOUND)
    if(NOT TARGET LIBRT::LIBRT)
      add_library(LIBRT::LIBRT UNKNOWN IMPORTED)
      set_target_properties(LIBRT::LIBRT PROPERTIES
        IMPORTED_LOCATION "${LIBRT_LIBRARIES}"
        INTERFACE_INCLUDE_DIRECTORIES "${LIBRT_INCLUDE_DIRS}")
    endif()
endif()
