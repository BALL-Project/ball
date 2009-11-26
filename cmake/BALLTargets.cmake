MESSAGE(WARNING "TODO: Implement!")

IF (MSVC)
	ADD_CUSTOM_TARGET(targets
										COMMAND ${CMAKE_COMMAND} -E echo ""
										COMMAND ${CMAKE_COMMAND} -E echo "=========================================================================="
										COMMAND ${CMAKE_COMMAND} -E echo ""
										COMMAND ${CMAKE_COMMAND} -E echo "The following make targets are available:"
										COMMAND ${CMAKE_COMMAND} -E echo "    doc             builds the html documentation"
										COMMAND ${CMAKE_COMMAND} -E echo "    doc_tidy        builds the xhtml cmpliant documentation"
										COMMAND ${CMAKE_COMMAND} -E echo "    doc_dot         builds the doxygen DOT documentation"
										COMMAND ${CMAKE_COMMAND} -E echo "    doc_tutorial		builds the BALL pdf tutorial"
										COMMAND ${CMAKE_COMMAND} -E echo ""
										COMMAND ${CMAKE_COMMAND} -E echo "=========================================================================="
										COMMAND ${CMAKE_COMMAND} -E echo ""
										COMMENT "The most important targets for BALL"
										VERBATIM
		)
ELSE()
	ADD_CUSTOM_TARGET(targets
										COMMAND ${CMAKE_COMMAND} -E echo ""
										COMMAND ${CMAKE_COMMAND} -E echo "=========================================================================="
										COMMAND ${CMAKE_COMMAND} -E echo ""
										COMMAND ${CMAKE_COMMAND} -E echo "The following make targets are available:"
										COMMAND ${CMAKE_COMMAND} -E echo "    doc             builds the html documentation"
										COMMAND ${CMAKE_COMMAND} -E echo "    doc_tidy        builds the xhtml cmpliant documentation"
										COMMAND ${CMAKE_COMMAND} -E echo "    doc_dot         builds the doxygen DOT documentation"
										COMMAND ${CMAKE_COMMAND} -E echo "    doc_tutorial		builds the BALL pdf tutorial"
										COMMAND ${CMAKE_COMMAND} -E echo ""
										COMMAND ${CMAKE_COMMAND} -E echo "=========================================================================="
										COMMAND ${CMAKE_COMMAND} -E echo ""
										COMMENT "The most important targets for BALL"
										VERBATIM
		)
ENDIF()
