#######################################################################
#create Doxyfiles for html documentation
set(CF_BALL_BIN_PATH ${PROJECT_BINARY_DIR})
set(CF_BALL_SRC_PATH ${PROJECT_SOURCE_DIR})

find_package(Doxygen)
find_package(LATEX)

if (DOXYGEN_FOUND)
	configure_file(${PROJECT_SOURCE_DIR}/doc/Doxyfile.in ${PROJECT_BINARY_DIR}/doc/Doxyfile)
	#configure_file(${PROJECT_SOURCE_DIR}/doc/doxygen/Doxyfile_internal.in ${PROJECT_BINARY_DIR}/doc/doxygen/Doxyfile_internal)
	#configure_file(${PROJECT_SOURCE_DIR}/doc/doxygen/Doxyfile_dot.in ${PROJECT_BINARY_DIR}/doc/doxygen/Doxyfile_dot)
	#configure_file(${PROJECT_SOURCE_DIR}/doc/doxygen/Doxyfile_noclass.in ${PROJECT_BINARY_DIR}/doc/doxygen/Doxyfile_noclass)
	#configure_file(${PROJECT_SOURCE_DIR}/doc/doxygen/Doxyfile_xml.in ${PROJECT_BINARY_DIR}/doc/doxygen/Doxyfile_xml)
	#configure_file(${PROJECT_SOURCE_DIR}/doc/BALLView/Doxyfile.in ${PROJECT_BINARY_DIR}/doc/BALLView/Doxyfile)
	#configure_file(${PROJECT_SOURCE_DIR}/doc/TUTORIAL/Doxyfile.in ${PROJECT_BINARY_DIR}/doc/TUTORIAL/Doxyfile)
	
	#######################################################################
	## doc_minimal target
	add_custom_target(doc
										COMMAND ${CMAKE_COMMAND} -E echo ""
										COMMAND ${CMAKE_COMMAND} -E echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
										COMMAND ${CMAKE_COMMAND} -E echo "Creating html documentation without class/TOPP/UTILS documentation";
										COMMAND ${CMAKE_COMMAND} -E echo "";   
										COMMAND ${CMAKE_COMMAND} -E remove_directory doc/html
										COMMAND ${CMAKE_COMMAND} -E chdir doc doxygen Doxyfile
										COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/doc/index.html doc/index.html
										COMMAND ${CMAKE_COMMAND} -E echo "";
										COMMAND ${CMAKE_COMMAND} -E echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
										COMMAND ${CMAKE_COMMAND} -E echo "The documentation has been successfully created.";
										COMMAND ${CMAKE_COMMAND} -E echo "You can now open 'doc/index.html' in a web browser.";
										COMMAND ${CMAKE_COMMAND} -E echo ""
										COMMENT "Build the doxygen documentation"
										VERBATIM)
else()
	Message(STATUS "Doxygen or Latex missing. Disabling 'tutorials' target!")
endif()
