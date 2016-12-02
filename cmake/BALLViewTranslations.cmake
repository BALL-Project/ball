IF(NOT Qt5LinguistTools_FOUND)
	RETURN (0)
ENDIF()


# Collect TS files from translations directory
SET(TRANSLATION_DIR ${CMAKE_SOURCE_DIR}/data/BALLView/translations/)
FILE(GLOB PROJECT_TS_FILES "${TRANSLATION_DIR}/*.ts")

IF(UPDATE_TRANSLATIONS)

	# Update and compile translations
	QT5_CREATE_TRANSLATION(PROJECT_QM_FILES
		"${CMAKE_SOURCE_DIR}/source"
		${PROJECT_TS_FILES}
		OPTIONS -I "${CMAKE_SOURCE_DIR}/include"
		-extensions C,ui,c,c++,cc,cpp,cxx,ch,h,h++,hh,hpp,hxx
		-source-language en_US
	)

ELSE()

	# Compile translations
	QT5_ADD_TRANSLATION(PROJECT_QM_FILES
		${PROJECT_TS_FILES}
	)

ENDIF()

# Add a custom target to create and/or compile translations
ADD_CUSTOM_TARGET(translations DEPENDS ${PROJECT_QM_FILES})

# Copy compiled .qm files into translations directory
# Copying multiple files with a single 'cmake -E copy' works from CMake 3.5+
FOREACH(PROJECT_QM_FILE ${PROJECT_QM_FILES})
	ADD_CUSTOM_COMMAND(TARGET translations
			COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_QM_FILE} ${TRANSLATION_DIR}
		)
ENDFOREACH()
