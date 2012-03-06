IF(${SEQAN_INCLUDE_PATH})
	LIST(APPEND SEQAN_INCLUDE_DIRECTORY ${SEQAN_INCLUDE_PATH})
ENDIF()

LIST(APPEND SEQAN_INCLUDE_DIRECTORY
	/usr/include
	/usr/local/include
	/opt/local/include
	/sw/include
)

FIND_PATH(SEQAN_INCLUDE_PATH seqan/align.h ${SEQAN_INCLUDE_DIRECTORY} DOC "The directory the seqan includes are located in")

IF(SEQAN_INCLUDE_PATH)
	SET(SEQAN_FOUND TRUE BOOL "Indicates whether SeQan could be found or not")
	MESSAGE(STATUS "SeqAn: Found " ${SEQAN_INCLUDE_PATH})
ELSE()
	SET(SEQAN_FOUND FALSE BOOL "Indicates whether SeQan could be found or not")
	MESSAGE(STATUS "SeqAn: Not found")
ENDIF()

MARK_AS_ADVANCED(SEQAN_FOUND)
