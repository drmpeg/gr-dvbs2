if(NOT PKG_CONFIG_FOUND)
    INCLUDE(FindPkgConfig)
endif()
PKG_CHECK_MODULES(PC_PCAP pcap)

FIND_PATH(
    PCAP_INCLUDE_DIRS
    NAMES pcap.h
    HINTS ${PC_PCAP_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    PCAP_LIBRARIES
    NAMES pcap
    HINTS ${PC_PCAP_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Pcap DEFAULT_MSG PCAP_LIBRARIES PCAP_INCLUDE_DIRS)
MARK_AS_ADVANCED(PCAP_LIBRARIES PCAP_INCLUDE_DIRS)

