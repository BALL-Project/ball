#######################################################################
#create Doxyfiles for html documentation
SET(CF_BALL_BIN_PATH ${PROJECT_BINARY_DIR})
SET(CF_BALL_SRC_PATH ${PROJECT_SOURCE_DIR})

FIND_PACKAGE(Doxygen)
FIND_PACKAGE(LATEX)

IF (BALL_ENABLE_PACKAGING)
	SET(AUTO_BUILD_DOC "ALL")
ELSE()
	SET(AUTO_BUILD_DOC "")
ENDIF()

IF (DOXYGEN_FOUND)
	CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/doc/Doxyfile.in ${PROJECT_BINARY_DIR}/doc/Doxyfile)
	#CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/doc/doxygen/Doxyfile_internal.in ${PROJECT_BINARY_DIR}/doc/doxygen/Doxyfile_internal)
	CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/doc/Doxyfile_dot.in ${PROJECT_BINARY_DIR}/doc/Doxyfile_dot)
	#CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/doc/doxygen/Doxyfile_noclass.in ${PROJECT_BINARY_DIR}/doc/doxygen/Doxyfile_noclass)
	#CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/doc/doxygen/Doxyfile_xml.in ${PROJECT_BINARY_DIR}/doc/doxygen/Doxyfile_xml)
	#CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/doc/BALLView/Doxyfile.in ${PROJECT_BINARY_DIR}/doc/BALLView/Doxyfile)
	#CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/doc/TUTORIAL/Doxyfile.in ${PROJECT_BINARY_DIR}/doc/TUTORIAL/Doxyfile)
	
	#######################################################################
	## doc
	ADD_CUSTOM_TARGET(doc ${AUTO_BUILD_DOC}
										COMMAND ${CMAKE_COMMAND} -E echo ""
										COMMAND ${CMAKE_COMMAND} -E echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
										COMMAND ${CMAKE_COMMAND} -E echo "Creating html documentation";
										COMMAND ${CMAKE_COMMAND} -E echo "";   
										COMMAND ${CMAKE_COMMAND} -E remove_directory doc/html
										COMMAND ${CMAKE_COMMAND} -E chdir doc doxygen Doxyfile
										COMMAND ${CMAKE_COMMAND} -E echo "";
										COMMAND ${CMAKE_COMMAND} -E echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
										COMMAND ${CMAKE_COMMAND} -E echo "The documentation has been successfully created.";
										COMMAND ${CMAKE_COMMAND} -E echo "You can now open 'doc/html/index.html' in a web browser.";
										COMMAND ${CMAKE_COMMAND} -E echo ""
										COMMENT "Build the doxygen documentation"
										COMPONENT "${COMPONENT_DOCUMENTATION_HTML}"
										VERBATIM)

	INSTALL(DIRECTORY   "${PROJECT_BINARY_DIR}/doc/html/"
	        DESTINATION "${BALL_DOCUMENTATION_INSTALL_DIRECTORY}/BALL"
					COMPONENT   "${COMPONENT_DOCUMENTATION_HTML}"
  )
	
	#######################################################################
	## doc_tidy
	ADD_CUSTOM_TARGET(doc_tidy
										COMMAND ${CMAKE_COMMAND} -E echo ""
										COMMAND ${CMAKE_COMMAND} -E echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
										COMMAND ${CMAKE_COMMAND} -E echo "Creating html documentation";
										COMMAND ${CMAKE_COMMAND} -E echo "";   
										COMMAND ${CMAKE_COMMAND} -E remove_directory doc/html
										COMMAND ${CMAKE_COMMAND} -E chdir doc doxygen Doxyfile
										COMMAND ${CMAKE_COMMAND} -E echo "Running tidy...";
										COMMAND ${CMAKE_COMMAND} -E chdir doc/html tidy -m -i -asxml -q -bare -clean *htm* 2>/dev/null | true
										COMMAND ${CMAKE_COMMAND} -E echo "Finshed Tunning tidy...";
										COMMAND ${CMAKE_COMMAND} -E echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
										COMMAND ${CMAKE_COMMAND} -E echo "The documentation has been successfully created.";
										COMMAND ${CMAKE_COMMAND} -E echo "You can now open 'doc/html/index.html' in a web browser.";
										COMMAND ${CMAKE_COMMAND} -E echo ""
										COMMENT "Build the doxygen documentation"
										VERBATIM)
	
	IF (DOXYGEN_DOT_FOUND OR DOXYGEN_DOT_EXECUTABLE)
		#######################################################################
		## doc_dot target
		ADD_CUSTOM_TARGET(doc_dot
											COMMAND ${CMAKE_COMMAND} -E echo ""
											COMMAND ${CMAKE_COMMAND} -E echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
											COMMAND ${CMAKE_COMMAND} -E echo "Creating DOT html documentation";
											COMMAND ${CMAKE_COMMAND} -E echo "";   
											COMMAND ${CMAKE_COMMAND} -E remove_directory doc/html-dot
											COMMAND ${CMAKE_COMMAND} -E chdir doc doxygen Doxyfile_dot
											COMMAND ${CMAKE_COMMAND} -E echo "";
											COMMAND ${CMAKE_COMMAND} -E echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
											COMMAND ${CMAKE_COMMAND} -E echo "The documentation has been successfully created.";
											COMMAND ${CMAKE_COMMAND} -E echo "You can now open 'doc/html-dot/index.html' in a web browser.";
											COMMAND ${CMAKE_COMMAND} -E echo ""
											COMMENT "Build the doxygen documentation"
											VERBATIM)
	ELSE()
		MESSAGE(STATUS "DOT not found. Disabling target 'doc_dot'!")
	ENDIF()
		

ELSE()
	MESSAGE(STATUS "Doxygen not found. Disabling all documentation targets!")
ENDIF()

IF (DOXYGEN_FOUND AND LATEX_COMPILER AND DVIPS_CONVERTER)
	#######################################################################
	# doc_tutorials target
	ADD_CUSTOM_TARGET(doc_tutorial ${AUTO_BUILD_DOC}
										COMMAND ${CMAKE_COMMAND} -E echo ""
										COMMAND ${CMAKE_COMMAND} -E echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
										COMMAND ${CMAKE_COMMAND} -E echo "Creating BALL pdf tutorial";
										COMMAND ${CMAKE_COMMAND} -E echo "";   
										COMMAND ${CMAKE_COMMAND} -E chdir ${PROJECT_SOURCE_DIR}/doc/TUTORIAL/	pdflatex tutorial.tex
										COMMAND ${CMAKE_COMMAND} -E chdir ${PROJECT_SOURCE_DIR}/doc/TUTORIAL/	bibtex tutorial
										COMMAND ${CMAKE_COMMAND} -E chdir ${PROJECT_SOURCE_DIR}/doc/TUTORIAL/	makeindex tutorial.idx
										COMMAND ${CMAKE_COMMAND} -E chdir ${PROJECT_SOURCE_DIR}/doc/TUTORIAL/	pdflatex tutorial.tex
										COMMAND ${CMAKE_COMMAND} -E chdir ${PROJECT_SOURCE_DIR}/doc/TUTORIAL/	pdflatex tutorial.tex
										COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/doc/TUTORIAL/tutorial.pdf doc/tutorial.pdf
										COMMAND ${CMAKE_COMMAND} -E echo "";
										COMMAND ${CMAKE_COMMAND} -E echo "The BALL tutorial in PDF format has been successfully created:"; 
										COMMAND ${CMAKE_COMMAND} -E echo "doc/tutorial.pdf"; 
										COMMAND ${CMAKE_COMMAND} -E echo ""; 
										COMMENT "Build the BALL pdf tutorial"
										VERBATIM)

	INSTALL(FILES   ${PROJECT_BINARY_DIR}/doc/tutorial.pdf
	        DESTINATION "${BALL_DOCUMENTATION_INSTALL_DIRECTORY}/doc"
					COMPONENT   "${COMPONENT_DOCUMENTATION_PDF}"
  )

ELSE()
	MESSAGE(STATUS "Doxygen or Latex missing. Disabling 'tutorial' target!")
ENDIF()
