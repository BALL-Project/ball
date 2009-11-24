MESSAGE(WARNING "TODO: Implement!")

IF (MSVC)
	ADD_CUSTOM_TARGET(targets
										COMMAND ${CMAKE_COMMAND} -E echo ""
										COMMAND ${CMAKE_COMMAND} -E echo "=========================================================================="
										COMMAND ${CMAKE_COMMAND} -E echo ""
										COMMAND ${CMAKE_COMMAND} -E echo "The following make targets are available:"
#										COMMAND ${CMAKE_COMMAND} -E echo "    ALL_BUILD       [Visual Studio only] builds the OpenMS library, TOPP tools and UTILS tools"
#										COMMAND ${CMAKE_COMMAND} -E echo "    [no target]     [NMake only]         builds the OpenMS library, TOPP tools and UTILS tools"
#										COMMAND ${CMAKE_COMMAND} -E echo "    OpenMS          builds the OpenMS library"
#										COMMAND ${CMAKE_COMMAND} -E echo "    TOPP            builds the TOPP tools"
#										COMMAND ${CMAKE_COMMAND} -E echo "    UTILS           builds the UTILS tools"
#										COMMAND ${CMAKE_COMMAND} -E echo "    RUN_TESTS       [Visual Studio only] executes OpenMS and TOPP tests (*)"
#										COMMAND ${CMAKE_COMMAND} -E echo "    test            [NMake only]         executes OpenMS and TOPP tests (*)"
#										COMMAND ${CMAKE_COMMAND} -E echo "                    *) make sure they are built using the 'test_build' target (see below)"
#										COMMAND ${CMAKE_COMMAND} -E echo "    Tutorials_build builds the tutorials in source/EXAMPLES"
#										COMMAND ${CMAKE_COMMAND} -E echo "    Tutorials_exec  executes the tutorials in source/EXAMPLES"
#										COMMAND ${CMAKE_COMMAND} -E echo "    doc             builds the doxygen documentation and tutorials"
#										COMMAND ${CMAKE_COMMAND} -E echo "    doc_tutorials   builds the pdf tutorials"
#										COMMAND ${CMAKE_COMMAND} -E echo ""
#										COMMAND ${CMAKE_COMMAND} -E echo "Single TOPP tools and UTILS have their own target, e.g. TOPPView"
#										COMMAND ${CMAKE_COMMAND} -E echo "The class tests have their own project in ./source/TEST (project test_build)."
#										COMMAND ${CMAKE_COMMAND} -E echo ""
#										COMMAND ${CMAKE_COMMAND} -E echo "=========================================================================="
#										COMMAND ${CMAKE_COMMAND} -E echo ""
										COMMENT "The most important targets for BALL"
										VERBATIM
		)
ELSE()
	ADD_CUSTOM_TARGET(targets
										COMMAND ${CMAKE_COMMAND} -E echo ""
										COMMAND ${CMAKE_COMMAND} -E echo "=========================================================================="
										COMMAND ${CMAKE_COMMAND} -E echo ""
										COMMAND ${CMAKE_COMMAND} -E echo "The following make targets are available:"
#										COMMAND ${CMAKE_COMMAND} -E echo "    [no target]     builds the OpenMS library, TOPP tools and UTILS tools"
#										COMMAND ${CMAKE_COMMAND} -E echo "    OpenMS          builds the OpenMS library"
#										COMMAND ${CMAKE_COMMAND} -E echo "    TOPP            builds the TOPP tools"
#										COMMAND ${CMAKE_COMMAND} -E echo "    UTILS           builds the UTILS tools"
#										COMMAND ${CMAKE_COMMAND} -E echo "    test_build      builds the OpenMS tests"
#										COMMAND ${CMAKE_COMMAND} -E echo "    test            executes OpenMS and TOPP tests"
#										COMMAND ${CMAKE_COMMAND} -E echo "                    make sure they are built using the 'test_build' target"
#										COMMAND ${CMAKE_COMMAND} -E echo "    Tutorials_build builds the tutorials in source/EXAMPLES"
#										COMMAND ${CMAKE_COMMAND} -E echo "    Tutorials_exec  executes the tutorials in source/EXAMPLES"
 										COMMAND ${CMAKE_COMMAND} -E echo "    doc             builds the doxygen documentation and tutorials"
#										COMMAND ${CMAKE_COMMAND} -E echo "    doc_tutorials   builds the pdf tutorials"
#										COMMAND ${CMAKE_COMMAND} -E echo "    help            list all available targets (very long)"
#										COMMAND ${CMAKE_COMMAND} -E echo ""
#										COMMAND ${CMAKE_COMMAND} -E echo "Single TOPP tools and UTILS have their own target, e.g. TOPPView"
#										COMMAND ${CMAKE_COMMAND} -E echo "The class tests have their own project in ./source/TEST."
										COMMAND ${CMAKE_COMMAND} -E echo ""
										COMMAND ${CMAKE_COMMAND} -E echo "=========================================================================="
										COMMAND ${CMAKE_COMMAND} -E echo ""
										COMMENT "The most important targets for BALL"
										VERBATIM
		)
ENDIF()
