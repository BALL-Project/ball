# Macro to find CTD2Galaxy
# More information on CTD2Galaxy under https://github.com/WorkflowConversion/CTD2Galaxy
#
# This macro will set the following variables:
#    CTD2GALAXY
#
# This macro will read from the following variable:
#    CTD2GALAXY_DIR - Directory under which CTD2Galaxy/generator.py is found
#
# This macro depends on the following variables:
#	 CTDOPTS_MODULE
#	 PYTHONINTERP

FIND_PACKAGE(PythonInterp)
FIND_PACKAGE(CTDopts)

IF(PYTHONINTERP_FOUND)
	IF(CTDOPTS_MODULE)
		# CTDopts module found, now find the scripts for CTD2Galaxy
		FIND_FILE(CTD2GALAXY
			NAMES generator.py
			PATHS ${CTD2GALAXY_DIR} ${CTD2GALAXY_DIR}/CTD2Galaxy)
		IF(CTD2GALAXY)
			MESSAGE(STATUS "Found generator.py in: " ${CTD2GALAXY})
			MARK_AS_ADVANCED(CTD2GALAXY)
		ELSE()	
			MESSAGE(STATUS "CTD2Galaxy could not be found.")
		ENDIF()
	ELSE()
		MESSAGE(STATUS "CTD2Galaxy depends on CTDopts and it could not be found.")
	ENDIF()
ELSE()
	MESSAGE(STATUS "CTD2Galaxy depends on PythonInterp, which was not found.")	
ENDIF()
