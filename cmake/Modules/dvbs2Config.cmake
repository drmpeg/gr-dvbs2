INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_DVBS2 dvbs2)

FIND_PATH(
    DVBS2_INCLUDE_DIRS
    NAMES dvbs2/api.h
    HINTS $ENV{DVBS2_DIR}/include
        ${PC_DVBS2_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    DVBS2_LIBRARIES
    NAMES gnuradio-dvbs2
    HINTS $ENV{DVBS2_DIR}/lib
        ${PC_DVBS2_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/dvbs2Target.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(DVBS2 DEFAULT_MSG DVBS2_LIBRARIES DVBS2_INCLUDE_DIRS)
MARK_AS_ADVANCED(DVBS2_LIBRARIES DVBS2_INCLUDE_DIRS)
