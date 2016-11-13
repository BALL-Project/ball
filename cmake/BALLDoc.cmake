#######################################################################
#create Doxyfiles for html documentation
SET(CF_BALL_BIN_PATH ${PROJECT_BINARY_DIR})
SET(CF_BALL_SRC_PATH ${PROJECT_SOURCE_DIR})

FILE(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/doc/html)

FIND_PACKAGE(Doxygen)
FIND_PACKAGE(LATEX)

IF (BALL_ENABLE_PACKAGING)
	SET(AUTO_BUILD_DOC "ALL")
ELSE()
	SET(AUTO_BUILD_DOC "")
ENDIF()

########################################################################
## Examples
########################################################################

IF(NOT APPLE)
	INSTALL(DIRECTORY   "${PROJECT_SOURCE_DIR}/doc/examples/"
		DESTINATION "${BALL_DOCUMENTATION_INSTALL_DIRECTORY}/EXAMPLES"
		COMPONENT   "${COMPONENT_DOCUMENTATION_EXAMPLES}")
ENDIF()

IF (DOXYGEN_FOUND)
	CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/doc/doxygen/Doxyfile_inc.in ${PROJECT_BINARY_DIR}/doc/doxygen/Doxyfile_inc)
	CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/doc/doxygen/Doxyfile.in ${PROJECT_BINARY_DIR}/doc/doxygen/Doxyfile)
	CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/doc/doxygen/Doxyfile_dot.in ${PROJECT_BINARY_DIR}/doc/doxygen/Doxyfile_dot)
	CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/doc/doxygen/template/balldoc-header.html.in ${PROJECT_BINARY_DIR}/doc/doxygen/template/balldoc-header.html)
	CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/doc/doxygen/template/balldoc-footer.html.in ${PROJECT_BINARY_DIR}/doc/doxygen/template/balldoc-footer.html)
	FILE(COPY ${PROJECT_SOURCE_DIR}/doc/doxygen/template/balldoc.css DESTINATION ${PROJECT_BINARY_DIR}/doc/doxygen/template)
	FILE(COPY ${PROJECT_SOURCE_DIR}/doc/doxygen/images/balldoc-header.jpg DESTINATION ${PROJECT_BINARY_DIR}/doc/doxygen/images)
	
	#######################################################################
	## doc
	ADD_CUSTOM_TARGET(doc ${AUTO_BUILD_DOC}
										COMMAND ${CMAKE_COMMAND} -E echo ""
										COMMAND ${CMAKE_COMMAND} -E echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
										COMMAND ${CMAKE_COMMAND} -E echo "Creating html documentation"
										COMMAND ${CMAKE_COMMAND} -E echo ""   
										COMMAND ${CMAKE_COMMAND} -E remove_directory doc/html
										COMMAND ${CMAKE_COMMAND} -E chdir doc/doxygen ${DOXYGEN_EXECUTABLE} Doxyfile
										COMMAND ${CMAKE_COMMAND} -E echo ""
										COMMAND ${CMAKE_COMMAND} -E echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
										COMMAND ${CMAKE_COMMAND} -E echo "The documentation has been successfully created."
										COMMAND ${CMAKE_COMMAND} -E echo "You can now open 'doc/html/index.html' in a web browser."
										COMMAND ${CMAKE_COMMAND} -E echo ""
										COMMENT "Build the doxygen documentation"
										COMPONENT "${COMPONENT_DOCUMENTATION_HTML}"
										VERBATIM)

	IF(NOT APPLE)
		INSTALL(DIRECTORY   "${PROJECT_BINARY_DIR}/doc/html/"
			DESTINATION "${BALL_DOCUMENTATION_INSTALL_DIRECTORY}/BALL"
			COMPONENT   "${COMPONENT_DOCUMENTATION_HTML}"
		)
	ENDIF()
	
	IF (DOXYGEN_DOT_FOUND OR DOXYGEN_DOT_EXECUTABLE)
		#######################################################################
		## doc_dot target
		ADD_CUSTOM_TARGET(doc_dot
											COMMAND ${CMAKE_COMMAND} -E echo ""
											COMMAND ${CMAKE_COMMAND} -E echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
											COMMAND ${CMAKE_COMMAND} -E echo "Creating DOT html documentation"
											COMMAND ${CMAKE_COMMAND} -E echo ""   
											COMMAND ${CMAKE_COMMAND} -E remove_directory doc/html-dot
											COMMAND ${CMAKE_COMMAND} -E chdir doc/doxygen ${DOXYGEN_EXECUTABLE} Doxyfile_dot
											COMMAND ${CMAKE_COMMAND} -E echo ""
											COMMAND ${CMAKE_COMMAND} -E echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
											COMMAND ${CMAKE_COMMAND} -E echo "The documentation has been successfully created."
											COMMAND ${CMAKE_COMMAND} -E echo "You can now open 'doc/html-dot/index.html' in a web browser."
											COMMAND ${CMAKE_COMMAND} -E echo ""
											COMMENT "Build the doxygen documentation"
											VERBATIM)
	ELSE()
		MESSAGE(STATUS "DOT not found. Disabling target 'doc_dot'!")
	ENDIF()
		

ELSE()
	MESSAGE(STATUS "Doxygen not found. Disabling all documentation targets!")
ENDIF()

IF (LATEX_COMPILER AND DVIPS_CONVERTER)
	#######################################################################
	# doc_tutorials target
	ADD_CUSTOM_TARGET(doc_tutorial ${AUTO_BUILD_DOC}
										COMMAND ${CMAKE_COMMAND} -E echo ""
										COMMAND ${CMAKE_COMMAND} -E echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
										COMMAND ${CMAKE_COMMAND} -E echo "Creating BALL pdf tutorial"
										COMMAND ${CMAKE_COMMAND} -E echo ""   
										COMMAND ${CMAKE_COMMAND} -E chdir ${PROJECT_SOURCE_DIR}/doc/TUTORIAL/	${PDFLATEX_COMPILER} -interaction=nonstopmode tutorial.tex
										COMMAND ${CMAKE_COMMAND} -E chdir ${PROJECT_SOURCE_DIR}/doc/TUTORIAL/	${BIBTEX_COMPILER} tutorial
										COMMAND ${CMAKE_COMMAND} -E chdir ${PROJECT_SOURCE_DIR}/doc/TUTORIAL/	${MAKEINDEX_COMPILER} tutorial.idx
										COMMAND ${CMAKE_COMMAND} -E chdir ${PROJECT_SOURCE_DIR}/doc/TUTORIAL/	${PDFLATEX_COMPILER} -interaction=nonstopmode tutorial.tex
										COMMAND ${CMAKE_COMMAND} -E chdir ${PROJECT_SOURCE_DIR}/doc/TUTORIAL/	${PDFLATEX_COMPILER} -interaction=nonstopmode tutorial.tex
										COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/doc/TUTORIAL/tutorial.pdf doc/tutorial.pdf
										COMMAND ${CMAKE_COMMAND} -E echo ""
										COMMAND ${CMAKE_COMMAND} -E echo "The BALL tutorial in PDF format has been successfully created:" 
										COMMAND ${CMAKE_COMMAND} -E echo "doc/tutorial.pdf" 
										COMMAND ${CMAKE_COMMAND} -E echo "" 
										COMMENT "Build the BALL pdf tutorial"
										VERBATIM)

	IF(NOT APPLE)
		INSTALL(FILES   ${PROJECT_BINARY_DIR}/doc/tutorial.pdf
			DESTINATION "${BALL_DOCUMENTATION_INSTALL_DIRECTORY}/TUTORIAL"
			COMPONENT   "${COMPONENT_DOCUMENTATION_PDF}"
			OPTIONAL
		)
	ENDIF()

ELSE()
	MESSAGE(STATUS "Doxygen or Latex missing. Disabling 'tutorial' target!")
ENDIF()
