# - Try to find VRPN
# Once done, this will define
#
#  VRPN_FOUND - system has VRPN
#  QUAT_INCLUDE_DIRS - the QUAT include directories
#  QUAT_LIBRARIES - link these to use VRPN
#  VRPN_INCLUDE_DIRS - the VRPN include directories
#  VRPN_LIBRARIES - link these to use VRPN

INCLUDE(LibFindMacros)

# VRPN relies on the QUAT library that is typically shipped with VRPN
# As we onlxy need the QUAT library for VRPN, we check for it here

# QUAT Include dir
FIND_PATH(QUAT_INCLUDE_DIR
  NAMES quat.h
		PATH_SUFFIXES quat/include quat
)

# The QUAT library
FIND_LIBRARY(QUAT_LIBRARY
  NAMES quat
)

# VRPN Include dir
FIND_PATH(VRPN_INCLUDE_DIR
  NAMES vrpn_Tracker.h
		PATH_SUFFIXES vrpn/include vrpn 
)

# Finally the VRPN library itself
FIND_LIBRARY(VRPN_LIBRARY
  NAMES vrpn
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
SET(QUAT_PROCESS_INCLUDES QUAT_INCLUDE_DIR)
SET(QUAT_PROCESS_LIBS QUAT_LIBRARY)
SET(VRPN_PROCESS_INCLUDES VRPN_INCLUDE_DIR)
SET(VRPN_PROCESS_LIBS VRPN_LIBRARY)
LIBFIND_PROCESS(VRPN)
