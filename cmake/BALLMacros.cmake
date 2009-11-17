## fills ${varname} with the names of Debug and Release libraries (which usually only differ on MSVC)
## @param varname Name of the variable which will hold the result string (e.g. "optimized myLib.so debug myLibDebug.so")
## @param libnames   List of library names which are searched (release libs)
## @param libnames_d List of library names which are searched (debug libs)
## @param human_libname Name of the library (for display only) 
MACRO (OPENMS_CHECKLIB varname libnames libnames_d human_libname)
	FIND_LIBRARY(${varname}_OPT NAMES ${libnames} PATHS ${CONTRIB_LIB_DIR} DOC "${human_libname} library dir" NO_DEFAULT_PATH)
	if ("${varname}_OPT" STREQUAL "${varname}_OPT-NOTFOUND")
		MESSAGE(FATAL_ERROR "Unable to find ${human_libname} library! Searched names are: [${libnames}] Please make sure it is part of the contrib (which we assume to be at: ${CONTRIB_DIR}")
	else()
		MESSAGE(STATUS "Found ${human_libname} library (Release) at: " ${${varname}_OPT})
	endif()
	FIND_LIBRARY(${varname}_DBG NAMES ${libnames_d} PATHS ${CONTRIB_LIB_DIR} DOC "${human_libname} (Debug) library dir" NO_DEFAULT_PATH)
	if ("${varname}_DBG" STREQUAL "${varname}_DBG-NOTFOUND")
		MESSAGE(FATAL_ERROR "Unable to find ${human_libname} (Debug) library! Searched names are: [${libnames_d}] Please make sure it is part of the contrib (which we assume to be at: ${CONTRIB_DIR}")
	else()
		MESSAGE(STATUS "Found ${human_libname} library (Debug) at: " ${${varname}_DBG})
	endif()
	## combine result and include "optimized" and "debug" keywords which are essential for target_link_libraries()
	set(${varname} optimized ${${varname}_OPT} debug ${${varname}_DBG})
ENDMACRO (OPENMS_CHECKLIB)

### This macro is a terrible ugly hack and if life was fair, it would never exist.
### However, the QT4_WRAP_UI - macro from FindQt4.cmake does not allow changing the
### output directory, and the default choice is unacceptable.
###
### So this macro is a nearly one-to-one copy of the FindQt4 - version with only
### minor modifications (marked with ## BALL ###)
###
MACRO(QT4_WRAP_UI_BALL outfiles )
  QT4_EXTRACT_OPTIONS(ui_files ui_options ${ARGN})

	### BALL ###
  # create output directory (will not exist for out-of-source builds)
  FILE(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/include/BALL/VIEW/UIC/)

  FOREACH (it ${ui_files})
    GET_FILENAME_COMPONENT(outfile ${it} NAME_WE)
    GET_FILENAME_COMPONENT(infile ${it} ABSOLUTE)
		### BALL ###
    SET(outfile ${PROJECT_BINARY_DIR}/include/BALL/VIEW/UIC/ui_${outfile}.h)
    ADD_CUSTOM_COMMAND(OUTPUT ${outfile}
      COMMAND ${QT_UIC_EXECUTABLE}
      ARGS ${ui_options} -o ${outfile} ${infile}
      MAIN_DEPENDENCY ${infile})
    SET(${outfiles} ${${outfiles}} ${outfile})
  ENDFOREACH (it)
ENDMACRO (QT4_WRAP_UI_BALL)

MACRO(ADD_BALL_DEFINITIONS NEW_DEFINITIONS)
	SET(BALL_DEFINITIONS ${NEW_DEFINITIONS} ${BALL_DEFINITIONS})
	SET(CMAKE_REQUIRED_DEFINITIONS ${BALL_DEFINITIONS})
	ADD_DEFINITIONS(${NEW_DEFINITIONS})
ENDMACRO(ADD_BALL_DEFINITIONS)

## Add the source files in SOURCES_LIST to the list
## of files compiled into libBALL, and mark them as
## part of source group GROUP
MACRO(ADD_BALL_SOURCES GROUP SOURCES_LIST)
	SET(DIRECTORY source/${GROUP})

	### add full path to the filenames ###
	SET(SOURCES)
	FOREACH(i ${SOURCES_LIST})
		LIST(APPEND SOURCES ${DIRECTORY}/${i})
	ENDFOREACH()

	### pass source file list to the upper instance ###
	SET(BALL_sources ${BALL_sources} ${SOURCES})

	### source group definition ###
	STRING(REGEX REPLACE "/" "\\\\" S_GROUP ${GROUP})
	SOURCE_GROUP("Source Files\\\\${S_GROUP}" FILES ${SOURCES})
ENDMACRO()

## Add the header files in HEADERS_LIST to the list
## of files compiled into libBALL, and mark them as
## part of source group GROUP
MACRO(ADD_BALL_HEADERS GROUP HEADERS_LIST)
	SET(DIRECTORY include/BALL/${GROUP})

	### add full path to the filenames ###
	SET(HEADERS)
	FOREACH(i ${HEADERS_LIST})
		### make sure we do not have absolute paths flying around...
		GET_FILENAME_COMPONENT(i ${i} NAME)
		LIST(APPEND HEADERS ${DIRECTORY}/${i})
	ENDFOREACH()

	### pass source file list to the upper instance ###
	SET(BALL_headers ${BALL_headers} ${HEADERS})

	### source group definition ###
	STRING(REGEX REPLACE "/" "\\\\" S_GROUP ${GROUP})
	SOURCE_GROUP("Header Files\\\\${S_GROUP}" FILES ${HEADERS})
ENDMACRO()

## Add a parser and corresponding lexer to libBALL
MACRO(ADD_BALL_PARSER_LEXER GROUP BASENAME PREFIX)
	SET(DIRECTORY source/${GROUP})

	SET(PARSERINPUT ${DIRECTORY}/${BASENAME}Parser.y)
	SET(LEXERINPUT ${DIRECTORY}/${BASENAME}Lexer.l)

	FILE(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/${DIRECTORY})

	SET(PARSEROUTPUT ${PROJECT_BINARY_DIR}/${DIRECTORY}/${BASENAME}Parser.C)
	SET(PARSERHEADER ${PROJECT_BINARY_DIR}/${DIRECTORY}/${BASENAME}Parser.h)
	SET(LEXEROUTPUT  ${PROJECT_BINARY_DIR}/${DIRECTORY}/${BASENAME}Lexer.C )

	BISON_TARGET(${BASENAME}Parser ${PARSERINPUT} ${PARSEROUTPUT} COMPILE_FLAGS "--defines=${PARSERHEADER} -p${PREFIX}")
	FLEX_TARGET(${BASENAME}Lexer ${LEXERINPUT} ${LEXEROUTPUT} COMPILE_FLAGS "-P${PREFIX}")

	ADD_FLEX_BISON_DEPENDENCY(${BASENAME}Lexer ${BASENAME}Parser)

	SET(BALL_sources ${BALL_sources} ${PARSERINPUT} ${PARSEROUTPUT} ${LEXERINPUT} ${LEXEROUTPUT})

	SOURCE_GROUP("Source Files\\\\${GROUP}" FILES ${PARSEROUTPUT} ${LEXEROUTPUT})
	SOURCE_GROUP("Parser Files\\\\${GROUP}" FILES ${PARSERINPUT})
	SOURCE_GROUP("Lexer Files\\\\${GROUP}" FILES ${LEXERINPUT})
ENDMACRO()

## Add the source files in SOURCES_LIST to the list
## of files compiled into libVIEW, and mark them as
## part of source group GROUP
MACRO(ADD_VIEW_SOURCES GROUP SOURCES_LIST)
	SET(DIRECTORY source/${GROUP})

	### add full path to the filenames ###
	SET(SOURCES)
	FOREACH(i ${SOURCES_LIST})
		LIST(APPEND SOURCES ${DIRECTORY}/${i})
	ENDFOREACH()

	### pass source file list to the upper instance ###
	SET(VIEW_sources ${VIEW_sources} ${SOURCES})

	### source group definition ###
	STRING(REGEX REPLACE "/" "\\\\" S_GROUP ${GROUP})
	SOURCE_GROUP("Source Files\\\\${S_GROUP}" FILES ${SOURCES})
ENDMACRO()

### Generate appropriate uic and moc rules for the files in UI_LIST,
### and add the resulting source files to the VIEW sources and to their
### source group
MACRO(ADD_BALL_UIFILES GROUP UI_LIST)
	SET(DIRECTORY source/${GROUP})

	### for out of source builds, the output directory might not yet exist ###
	FILE(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/${DIRECTORY})

	### iterate over the ui files ###
	FOREACH(i ${UI_LIST})
		SET(UI_FILE ${DIRECTORY}/${i})

		### Generate the corresponding ui file ###
		SET(OUTFILES)
		QT4_WRAP_UI_BALL(OUTFILES ${UI_FILE})

		### and add them to the sources ###
		SET(VIEW_sources ${VIEW_sources} "${OUTFILES}" ${UI_FILE})

		### source group definition ###
		STRING(REGEX REPLACE "/" "\\\\" S_GROUP ${GROUP})
		SOURCE_GROUP("Source Files\\\\${S_GROUP}" FILES ${OUTFILES})
		SOURCE_GROUP("UI Files" FILES ${UI_FILE})
	ENDFOREACH()
ENDMACRO()

MACRO(ADD_BALL_MOCFILES GROUP INPUT_DIRECTORY MOC_LIST)
	SET(DIRECTORY source/${GROUP})

	### for out of source builds, the output directory might not yet exist ###
	FILE(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/${DIRECTORY})

	### iterate over the moc files ###
	FOREACH(i ${MOC_LIST})
		GET_FILENAME_COMPONENT(NAME_BASE ${i} NAME_WE)

		SET(MOC_FILE ${INPUT_DIRECTORY}/${NAME_BASE}.h)
		SET(OUTFILE  ${PROJECT_BINARY_DIR}/${DIRECTORY}/moc_${NAME_BASE}.C)

		### generate the corresponding moc files ###
		QT4_GENERATE_MOC(${MOC_FILE} ${OUTFILE})

		### and add them to the sources ###
		SET(VIEW_sources ${VIEW_sources} ${OUTFILE})

		### source group definition ###
		STRING(REGEX REPLACE "/" "\\\\" S_GROUP ${GROUP})
		SOURCE_GROUP("Source Files\\\\${S_GROUP}" FILES ${OUTFILE})
	ENDFOREACH()
ENDMACRO()

MACRO(BALL_COMBINE_LIBS RESULT OPTIMIZED_LIBS DEBUG_LIBS)
	SET(OPTIMIZED_LIBS "${OPTIMIZED_LIBS}")
	SET(DEBUG_LIBS "${DEBUG_LIBS}")
	IF (OPTIMIZED_LIBS AND DEBUG_LIBS)
		SET(${RESULT} optimized "${OPTIMIZED_LIBS}" debug "${DEBUG_LIBS}")
	ELSEIF(OPTIMIZED_LIBS)
		SET(${RESULT} "${OPTIMIZED_LIBS}")
	ELSEIF(DEBUG_LIBS)
		SET(${RESULT} "${DEBUG_LIBS}")
	ENDIF()
ENDMACRO()

MACRO(BALL_LIB_TO_DLL RESULT_DEBUG RESULT_OPTIMZED LIBS)
	SET(NEXT_LIB_IS_OPT TRUE)
	FOREACH(LIB ${LIBS})
		STRING(TOLOWER "${LIB}" LOWER_LIB)
		IF (${LOWER_LIB} STREQUAL "debug")
			SET(NEXT_LIB_IS_OPT FALSE)
		ELSEIF(${LOWER_LIB} STREQUAL "optimized")
			SET(NEXT_LIB_IS_OPT TRUE) 
		ELSE()
			GET_FILENAME_COMPONENT(LIBPATH "${LIB}" PATH)
			GET_FILENAME_COMPONENT(LIBNAME "${LIB}" NAME_WE)
			SET(DLLPATH "DLLPATH-NOTFOUND" CACHE STRING "" FORCE)
			FIND_FILE(DLLPATH 
				NAMES  "${LIBNAME}.dll ${LIBNAME}.pyd"
				PATHS "${LIBPATH}"
				PATH_SUFFIXES ../bin ../dlls
			)
			IF (DLLPATH)
				IF (NEXT_LIB_IS_OPT)
					LIST(APPEND ${RESULT_OPTIMZED} "${DLLPATH}")
				ELSE()
					LIST(APPEND ${RESULT_DEBUG} "${DLLPATH}")
				ENDIF()
			ENDIF()
			SET(NEXT_LIB_IS_OPT TRUE)
		ENDIF()
	ENDFOREACH()
ENDMACRO()
