# Detect libspnav

## First, search the headers
OPTION(USE_SPNAV "Use space navigator libraries" OFF)

IF (USE_SPNAV)

	IF (APPLE)
		FIND_LIBRARY(SPNAV_LIBRARY 3DConnexionClient) 
	ELSEIF(UNIX)
	ENDIF()

ENDIF()
