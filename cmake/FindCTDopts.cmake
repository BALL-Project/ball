# Macro to find CTDopts
# More information on CTDopts under https://github.com/WorkflowConversion/CTDopts
#
# This macro will set the following variable:
#    CTDOPTS_MODULE
#

FIND_PACKAGE(PythonInterp)
IF (PYTHONINTERP_FOUND)
	# CTDopts is a module, so we don't need to find a script, rather, just make sure it is properly installed
	FIND_FILE(_find_ctdopts_py FindCTDopts.py PATHS ${CMAKE_MODULE_PATH})
	EXECUTE_PROCESS(COMMAND ${PYTHON_EXECUTABLE} ${_find_ctdopts_py} OUTPUT_VARIABLE stdout ERROR_VARIABLE stderr RESULT_VARIABLE exit_code OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_STRIP_TRAILING_WHITESPACE)
	STRING(STRIP "${stdout}" stdout)
	IF((stdout STREQUAL "CTDopts seems to be working.") AND (exit_code EQUAL 0))
    	SET(CTDOPTS_MODULE TRUE)
		MARK_AS_ADVANCED(CTDOPTS_MODULE)
		MESSAGE(STATUS "CTDopts module was found and seems to be working...")
	ELSE()
		MESSAGE(STATUS "CTDopts module could not be found.")
	ENDIF()
ELSE()
	MESSAGE(STATUS "CTDopts depends on PythonInterp, which was not found.")
ENDIF()
