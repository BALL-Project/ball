#######################################################################
#create Doxyfiles for html documentation
set(CF_BALL_BIN_PATH ${PROJECT_BINARY_DIR})
set(CF_BALL_SRC_PATH ${PROJECT_SOURCE_DIR})

find_package(Doxygen)
find_package(LATEX)

if (DOXYGEN_FOUND)
	configure_file(${PROJECT_SOURCE_DIR}/doc/Doxyfile.in ${PROJECT_BINARY_DIR}/doc/Doxyfile)
	#configure_file(${PROJECT_SOURCE_DIR}/doc/doxygen/Doxyfile_internal.in ${PROJECT_BINARY_DIR}/doc/doxygen/Doxyfile_internal)
	configure_file(${PROJECT_SOURCE_DIR}/doc/Doxyfile_dot.in ${PROJECT_BINARY_DIR}/doc/Doxyfile_dot)
	#configure_file(${PROJECT_SOURCE_DIR}/doc/doxygen/Doxyfile_noclass.in ${PROJECT_BINARY_DIR}/doc/doxygen/Doxyfile_noclass)
	#configure_file(${PROJECT_SOURCE_DIR}/doc/doxygen/Doxyfile_xml.in ${PROJECT_BINARY_DIR}/doc/doxygen/Doxyfile_xml)
	#configure_file(${PROJECT_SOURCE_DIR}/doc/BALLView/Doxyfile.in ${PROJECT_BINARY_DIR}/doc/BALLView/Doxyfile)
	#configure_file(${PROJECT_SOURCE_DIR}/doc/TUTORIAL/Doxyfile.in ${PROJECT_BINARY_DIR}/doc/TUTORIAL/Doxyfile)
	
	#######################################################################
	## doc
	add_custom_target(doc
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
										VERBATIM)
	
	#######################################################################
	## doc_tidy
	add_custom_target(doc_tidy
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
	
	if (DOXYGEN_DOT_FOUND OR DOXYGEN_DOT_EXECUTABLE)
		#######################################################################
		## doc_dot target
		add_custom_target(doc_dot
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
	else()
		Message(STATUS "DOT not found. Disabling target 'doc_dot'!")
	endif()
		

else()
	Message(STATUS "Doxygen not found. Disabling all documentation targets!")
endif()

if (DOXYGEN_FOUND AND LATEX_COMPILER AND DVIPS_CONVERTER)
	#######################################################################
	# doc_tutorials target
	add_custom_target(doc_tutorial
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
else()
	Message(STATUS "Doxygen or Latex missing. Disabling 'tutorial' target!")
endif()
