# A simple find macro for detecting a SpaceNavigator driver installation
# The driver is actually platform dependent. The following libraries are
# detected:
#
# Unix: libspnav
# MacOSX: lib3DConnexionClient
#
# This macro sets the following variables:
# - SPNAV_FOUND        if the library has been successfully detected
#                      this variable will be set to TRUE. Else it will have
#                      the value FALSE.
# - SPNAV_LIBRARIES    a listcontaining the appropriate libraries
# - SPNAV_INCLUDE_DIRS a listcontaining the approriate include directories


IF(UNIX)
	FIND_PATH(spnav_include
		NAMES spnav.h
	)

	FIND_LIBRARY(spnav_lib spnav)

	# Prepare the input for LIBFIND_PROCESS
	SET(SPNAV_PROCESS_INCLUDES spnav_include)
	SET(SPNAV_PROCESS_LIBS spnav_lib)

	MESSAGE(STATUS ${spnav_include})

	LIBFIND_PROCESS(SPNAV)
ELSEIF(APPLE)
	#TODO: Improve MacOS X code
	FIND_LIBRARY(SPNAV_LIBRARY 3DConnexionClient)
ENDIF()
