# This short CMake macro searches for the ctags executable
# It sets the following variables:
#   - CTAGS_FOUND      True if the ctags executable could be found
#   - CTAGS_EXECUTABLE The path to the ctags executable
#

FIND_PROGRAM(CTAGS_EXECUTABLE ctags DOC "The path to the ctags executable")

IF(CTAGS_EXECUTABLE)
	SET(CTAGS_FOUND TRUE)
	MARK_AS_ADVANCED(CTAGS_FOUND)
ENDIF()
