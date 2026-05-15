# Skip translation handling when Qt6LinguistTools is unavailable. RETURN()
# takes no positional argument — RETURN(0) errors on CMake >= 3.25 unless
# CMP0140 is OLD. This branch was previously never exercised because every
# CI/dev environment shipped Qt6LinguistTools; the Windows vcpkg build does
# not (qttools is not in the manifest), which surfaced the bug.
IF(NOT Qt6LinguistTools_FOUND)
	RETURN()
ENDIF()


# Collect TS files from translations directory
SET(TRANSLATION_DIR ${CMAKE_SOURCE_DIR}/data/BALLView/translations/)
FILE(GLOB PROJECT_TS_FILES "${TRANSLATION_DIR}/*.ts")

IF(UPDATE_TRANSLATIONS)

	# Update and compile translations
	QT6_CREATE_TRANSLATION(PROJECT_QM_FILES
		"${CMAKE_SOURCE_DIR}/source"
		${PROJECT_TS_FILES}
		OPTIONS -I "${CMAKE_SOURCE_DIR}/include"
		-extensions C,ui,c,c++,cc,cpp,cxx,ch,h,h++,hh,hpp,hxx
		-source-language en_US
	)

ELSE()

	# Compile translations
	QT6_ADD_TRANSLATION(PROJECT_QM_FILES
		${PROJECT_TS_FILES}
	)

ENDIF()

# Add a custom target to create and/or compile translations
ADD_CUSTOM_TARGET(translations DEPENDS ${PROJECT_QM_FILES})

# Copy compiled .qm files into translations directory
# Copying multiple files with a single 'cmake -E copy' works from CMake 3.5+
FOREACH(PROJECT_QM_FILE ${PROJECT_QM_FILES})
	ADD_CUSTOM_COMMAND(TARGET translations POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_QM_FILE} ${TRANSLATION_DIR}
		)
ENDFOREACH()
