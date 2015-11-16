### This macro is a terrible ugly hack and if life was fair, it would never exist.
### However, the QT4_WRAP_UI - macro from FindQt4.cmake does not allow changing the
### output directory, and the default choice is unacceptable.
###
### So this macro is a nearly one-to-one copy of the FindQt4 - version with only
### minor modifications (marked with ## BALL ###)
###
function(QT5_WRAP_UI_BALL outfiles)
    set(options)
    set(oneValueArgs)
    set(multiValueArgs OPTIONS)

    cmake_parse_arguments(_WRAP_UI "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(ui_files ${_WRAP_UI_UNPARSED_ARGUMENTS})
    set(ui_options ${_WRAP_UI_OPTIONS})

		FILE(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/include/BALL/VIEW/UIC/)

    foreach(it ${ui_files})
        get_filename_component(outfile ${it} NAME_WE)
        get_filename_component(infile ${it} ABSOLUTE)
        set(outfile ${PROJECT_BINARY_DIR}/include/BALL/VIEW/UIC/ui_${outfile}.h)
        add_custom_command(OUTPUT ${outfile}
          COMMAND ${Qt5Widgets_UIC_EXECUTABLE}
          ARGS ${ui_options} -o ${outfile} ${infile}
          MAIN_DEPENDENCY ${infile} VERBATIM)
        list(APPEND ${outfiles} ${outfile})
    endforeach()
    set(${outfiles} ${${outfiles}} PARENT_SCOPE)
endfunction()

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

	## Oh, what an ugly hack...
	IF (NOT WIN32)
		BISON_TARGET(${BASENAME}Parser ${PARSERINPUT} ${PARSEROUTPUT} COMPILE_FLAGS "--defines=${PARSERHEADER} -p${PREFIX}")
	ELSE()
		BISON_TARGET(${BASENAME}Parser ${PARSERINPUT} ${PARSEROUTPUT} COMPILE_FLAGS "${BALL_BISON_BAT} --defines=${PARSERHEADER} -p${PREFIX}")
	ENDIF()
	FLEX_TARGET(${BASENAME}Lexer ${LEXERINPUT} ${LEXEROUTPUT} COMPILE_FLAGS "-P${PREFIX}")

	ADD_FLEX_BISON_DEPENDENCY(${BASENAME}Lexer ${BASENAME}Parser)

	SET(BALL_sources ${BALL_sources} ${PARSERINPUT} ${PARSEROUTPUT} ${LEXERINPUT} ${LEXEROUTPUT})

	SOURCE_GROUP("Source Files\\\\${GROUP}" FILES ${PARSEROUTPUT} ${LEXEROUTPUT})
	SOURCE_GROUP("Parser Files\\\\${GROUP}" FILES ${PARSERINPUT})
	SOURCE_GROUP("Lexer Files\\\\${GROUP}" FILES ${LEXERINPUT})
ENDMACRO()

## Add the header files in HEADERS_LIST to the list
## of files compiled into libBALL, and mark them as
## part of source group GROUP
MACRO(ADD_VIEW_HEADERS GROUP HEADERS_LIST)
	SET(DIRECTORY include/BALL/${GROUP})

	### add full path to the filenames ###
	SET(HEADERS)
	FOREACH(i ${HEADERS_LIST})
		### make sure we do not have absolute paths flying around...
		GET_FILENAME_COMPONENT(i ${i} NAME)
		LIST(APPEND HEADERS ${DIRECTORY}/${i})
	ENDFOREACH()

	### pass source file list to the upper instance ###
	LIST(APPEND VIEW_headers ${HEADERS})

	### source group definition ###
	STRING(REGEX REPLACE "/" "\\\\" S_GROUP ${GROUP})
	SOURCE_GROUP("Header Files\\\\${S_GROUP}" FILES ${HEADERS})
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
		QT5_WRAP_UI_BALL(OUTFILES ${UI_FILE})

		### and add them to the sources ###
		SET(VIEW_sources ${VIEW_sources} "${OUTFILES}" ${UI_FILE})

		### source group definition ###
		STRING(REGEX REPLACE "/" "\\\\" S_GROUP ${GROUP})
		SOURCE_GROUP("Source Files\\\\${S_GROUP}" FILES ${OUTFILES})
		SOURCE_GROUP("UI Files" FILES ${UI_FILE})
	ENDFOREACH()
ENDMACRO()

MACRO(BALL_COMBINE_LIBS RESULT OPTIMIZED_LIBS DEBUG_LIBS)
	SET(OPTIMIZED_LIBS "${OPTIMIZED_LIBS}")
	SET(DEBUG_LIBS "${DEBUG_LIBS}")
	IF (OPTIMIZED_LIBS AND DEBUG_LIBS)
		SET(${RESULT} optimized "${OPTIMIZED_LIBS}" debug "${DEBUG_LIBS}" CACHE STRING "" FORCE)
	ELSEIF(OPTIMIZED_LIBS)
		SET(${RESULT} "${OPTIMIZED_LIBS}" CACHE STRING ${RESULT} FORCE)
	ELSEIF(DEBUG_LIBS)
		SET(${RESULT} "${DEBUG_LIBS}" CACHE STRING ${RESULT} FORCE)
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
				NAMES  "${LIBNAME}.dll"
				HINTS "${LIBPATH}"
				PATH_SUFFIXES ../bin ../dlls
				NO_DEFAULT_PATH
			)
			IF (NOT DLLPATH)
				FIND_FILE(DLLPATH 
					NAMES  "${LIBNAME}.pyd"
					HINTS "${LIBPATH}"
					PATH_SUFFIXES ../bin ../dlls
				)
			ENDIF()
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
