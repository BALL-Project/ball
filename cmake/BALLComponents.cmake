MACRO(SET_COMPONENT_NAME COMPONENT NAME DESCRIPTION)
	STRING(TOUPPER ${COMPONENT} CURRENT_COMPONENT)
	SET(CPACK_COMPONENT_${CURRENT_COMPONENT}_DISPLAY_NAME ${NAME})
	SET(CPACK_COMPONENT_${CURRENT_COMPONENT}_DESCRIPTION  ${DESCRIPTION})
ENDMACRO()

MACRO(SET_COMPONENT_DEPENDENCIES COMPONENT DEPENDENCIES)
	## Dependencies of components seem to be currently broken for PackageMaker.
	## Or we do something completely wrong...
	IF (NOT APPLE)
		STRING(TOUPPER ${COMPONENT} CURRENT_COMPONENT)
		SET(CPACK_COMPONENT_${CURRENT_COMPONENT}_DEPENDS ${DEPENDENCIES})
	ENDIF()
ENDMACRO()

SET(BALL_PACKAGE_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}")

IF (UNIX AND NOT APPLE)
	## libraries
	SET(COMPONENT_LIBBALL "libBALL${BALL_PACKAGE_VERSION}")
	SET(COMPONENT_LIBVIEW "libVIEW${BALL_PACKAGE_VERSION}")

	## development
	SET(COMPONENT_LIBBALL_DEV "libBALL${BALL_PACKAGE_VERSION}-dev")
	SET(COMPONENT_LIBVIEW_DEV "libVIEW${BALL_PACKAGE_VERSION}-dev")

	## Python bindings
	SET(COMPONENT_PYTHON_BINDINGS_BALL "python-ball${BALL_PACKAGE_VERSION}")
	SET(COMPONENT_PYTHON_BINDINGS_VIEW "python-ballview${BALL_PACKAGE_VERSION}")

	## Documentation
	SET(COMPONENT_DOCUMENTATION_EXAMPLES "libball${BALL_PACKAGE_VERSION}-doc")
	SET(COMPONENT_DOCUMENTATION_HTML "libball${BALL_PACKAGE_VERSION}-doc")
	SET(COMPONENT_DOCUMENTATION_PDF  "libball${BALL_PACKAGE_VERSION}-doc")

	## applications
	SET(COMPONENT_BALLVIEW "BALLView${BALL_PACKAGE_VERSION}")
ELSEIF(APPLE)
	## libraries
	SET(COMPONENT_LIBBALL "libBALL${BALL_PACKAGE_VERSION}")
	SET(COMPONENT_LIBVIEW "libVIEW${BALL_PACKAGE_VERSION}")

	## development
	SET(COMPONENT_LIBBALL_DEV "libBALL${BALL_PACKAGE_VERSION}-dev")
	SET(COMPONENT_LIBVIEW_DEV "libBALL${BALL_PACKAGE_VERSION}-dev")

	## Python bindings
	SET(COMPONENT_PYTHON_BINDINGS_BALL "python-ball${BALL_PACKAGE_VERSION}")
	SET(COMPONENT_PYTHON_BINDINGS_VIEW "python-ballview${BALL_PACKAGE_VERSION}")

	## Documentation
	SET(COMPONENT_DOCUMENTATION_EXAMPLES "libball${BALL_PACKAGE_VERSION}-doc-examples")
	SET(COMPONENT_DOCUMENTATION_HTML "libball${BALL_PACKAGE_VERSION}-doc-html")
	SET(COMPONENT_DOCUMENTATION_PDF  "libball${BALL_PACKAGE_VERSION}-doc-pdf")

	## applications
	SET(COMPONENT_BALLVIEW "BALLView${BALL_PACKAGE_VERSION}")
ELSEIF(WIN32)
	## libraries
	SET(COMPONENT_LIBBALL "Runtime")
	SET(COMPONENT_LIBVIEW "Runtime")

	## development
	SET(COMPONENT_LIBBALL_DEV "Development")
	SET(COMPONENT_LIBVIEW_DEV "Development")

	## Python bindings
	SET(COMPONENT_PYTHON_BINDINGS_BALL "PythonBindings")
	SET(COMPONENT_PYTHON_BINDINGS_VIEW "PythonBindings")

	## Documentation
	SET(COMPONENT_DOCUMENTATION_EXAMPLES "DocumentationExamples")
	SET(COMPONENT_DOCUMENTATION_HTML "DocumentationHTML")
	SET(COMPONENT_DOCUMENTATION_PDF  "DocumentationPDF")

	## applications
	SET(COMPONENT_BALLVIEW "BALLView")
ENDIF()

INCLUDE(source/EXTENSIONS/BALLPluginComponents.cmake)

## Note: On MacOS X, it is necessary to have the applications (COMPONENT_BALLVIEW) at the
##       *end* of this list; otherwise, the automatic fixing of import library names will omit all components 
##       that come after it
SET(BALL_COMPONENTS
 ${COMPONENT_LIBBALL}
 ${COMPONENT_LIBVIEW}
 ${COMPONENT_LIBBALL_DEV}
 ${COMPONENT_LIBVIEW_DEV}
 "Unspecified"
 ${COMPONENT_PLUGINS}
)

# Phase 8b post-mortem (v1.7.0-rc1, 2026-05-18) — NSIS-installer
# clean-component gating:
#
# Modern NSIS 3.x treats unknown component-IDs in
# Deselect_required_by_<X> macros as a hard error and aborts
# makensis with "warning 6000: unknown variable/constant {<X>}".
# Components listed in BALL_COMPONENTS but never installed-into
# (because their gating OPTION is OFF, or the producing target
# is missing in the CI build) generate those macros, killing
# the installer compile.
#
# History of this gate cascade (per re-roll):
#   - v1.7.0-rc1 #5 — d5e268d290: gate PythonBindings on
#     BALL_PYTHON_SUPPORT (SIP disabled by default in v1.7).
#   - v1.7.0-rc1 #6 — this commit: gate Documentation*
#     components on BUILD_DOCUMENTATION (Doxygen never runs on
#     CI release runners, so the doc components are always
#     empty here).
#
# Custom builds that DO produce docs (developer machines with
# Doxygen + a `cmake --build . --target doc` invocation before
# packaging) can opt in via -DBUILD_DOCUMENTATION=ON.
OPTION(BUILD_DOCUMENTATION "Build + install API documentation (Doxygen HTML/PDF + examples)" OFF)

IF(BALL_PYTHON_SUPPORT)
	LIST(APPEND BALL_COMPONENTS
		${COMPONENT_PYTHON_BINDINGS_BALL}
		${COMPONENT_PYTHON_BINDINGS_VIEW}
	)
ENDIF()

IF(BUILD_DOCUMENTATION)
	LIST(APPEND BALL_COMPONENTS
		${COMPONENT_DOCUMENTATION_EXAMPLES}
		${COMPONENT_DOCUMENTATION_HTML}
		${COMPONENT_DOCUMENTATION_PDF}
	)
ENDIF()

LIST(APPEND BALL_COMPONENTS ${COMPONENT_BALLVIEW})

LIST(REMOVE_DUPLICATES BALL_COMPONENTS)

SET_COMPONENT_NAME(${COMPONENT_LIBBALL} 
	"BALL libraries and runtime"
	"The Biochemical Algorithms Library (BALL) is a rapid application development framework written in C++. It offers a variety of data structures and algorithms for all kinds of applications arising in molecular modelling."
)

IF (NOT COMPONENT_LIBBALL STREQUAL ${COMPONENT_LIBVIEW})

	SET_COMPONENT_NAME(${COMPONENT_LIBVIEW} 
		"VIEW library"
		"The VIEW library contains BALL's functionality for molecular visualization."
	)
	SET_COMPONENT_DEPENDENCIES(${COMPONENT_LIBVIEW} ${COMPONENT_LIBBALL})

ENDIF()

SET_COMPONENT_NAME(${COMPONENT_LIBBALL_DEV}
	"Development components for BALL"
	"Files required for developing with BALL (C++ - header files, ...)"
)
SET_COMPONENT_DEPENDENCIES(${COMPONENT_LIBBALL_DEV} ${COMPONENT_LIBBALL})

IF (NOT COMPONENT_LIBBALL_DEV STREQUAL ${COMPONENT_LIBVIEW_DEV})

	SET_COMPONENT_NAME(${COMPONENT_LIBVIEW_DEV}
		"Development components for VIEW"
		"Files required for developing with VIEW (C++ - header files, ...)"
	)

	SET_COMPONENT_DEPENDENCIES(${COMPONENT_LIBVIEW_DEV} "${COMPONENT_LIBVIEW} ${COMPONENT_LIBBALL_DEV}")

ENDIF()

# Phase 8b post-mortem (v1.7.0-rc1, 2026-05-18): see BALL_COMPONENTS
# guard above. Gate the per-component registration on BALL_PYTHON_SUPPORT
# so the NSIS template doesn't emit Deselect_required_by_Runtime macros
# referencing the undefined {PythonBindings} component.
IF(BALL_PYTHON_SUPPORT)
	SET_COMPONENT_NAME(${COMPONENT_PYTHON_BINDINGS_BALL}
		"BALL Python bindings"
		"The Python bindings can be used to develop fully functional BALL-based applications completely in Python. In addition, they allow to freely script BALLView"
	)
	SET_COMPONENT_DEPENDENCIES(${COMPONENT_PYTHON_BINDINGS_BALL} ${COMPONENT_LIBBALL})

	IF (NOT COMPONENT_PYTHON_BINDINGS_BALL STREQUAL ${COMPONENT_PYTHON_BINDINGS_VIEW})

		SET_COMPONENT_NAME(${COMPONENT_PYTHON_BINDINGS_VIEW}
			"BALL Python bindings"
			"The Python bindings can be used to develop fully functional BALL-based applications completely in Python. In addition, they allow to freely script BALLView"
		)
		SET_COMPONENT_DEPENDENCIES(${COMPONENT_PYTHON_BINDINGS_VIEW} "${COMPONENT_LIBVIEW} ${COMPONENT_PYTHON_BINDINGS_BALL}")

	ENDIF()
ENDIF()

# Phase 8b post-mortem (v1.7.0-rc1 #6, 2026-05-18): same gate as
# the BALL_COMPONENTS list above. Doc components register CPack
# dependencies on COMPONENT_LIBBALL — those dependency macros land
# in the NSIS Deselect_required_by_Runtime expansion and fail
# makensis if the component itself was never installed-into.
IF(BUILD_DOCUMENTATION)
	SET_COMPONENT_NAME(${COMPONENT_DOCUMENTATION_HTML}
		"BALL html Documentation"
		"This package contains the full documentation to the BALL and VIEW libraries in html format"
	)
	SET_COMPONENT_DEPENDENCIES(${COMPONENT_DOCUMENTATION_HTML} ${COMPONENT_LIBBALL})

	IF (NOT COMPONENT_DOCUMENTATION_HTML STREQUAL ${COMPONENT_DOCUMENTATION_PDF})

		SET_COMPONENT_NAME(${COMPONENT_DOCUMENTATION_PDF}
			"BALL PDF Documentation"
			"This package contains the full documentation to the BALL and VIEW libraries in PDF format"
		)
		SET_COMPONENT_DEPENDENCIES(${COMPONENT_DOCUMENTATION_PDF} ${COMPONENT_LIBBALL})

	ENDIF()

	IF (NOT COMPONENT_DOCUMENTATION_EXAMPLES STREQUAL ${COMPONENT_DOCUMENTATION_PDF})

		SET_COMPONENT_NAME(${COMPONENT_DOCUMENTATION_EXAMPLES}
			"BALL Usage Examples"
			"This package contains a number of examples showing the use of BALL as a library and python module"
		)
		SET_COMPONENT_DEPENDENCIES(${COMPONENT_DOCUMENTATION_EXAMPLES} ${COMPONENT_LIBBALL})

	ENDIF()
ENDIF()

SET_COMPONENT_NAME(${COMPONENT_BALLVIEW} 
	"BALLView"
	"BALLView is a feature-rich molecular viewing and modelling application based on BALL."
)
SET_COMPONENT_DEPENDENCIES(${COMPONENT_BALLVIEW} "${COMPONENT_LIBVIEW}")

SET(CPACK_COMPONENTS_ALL ${BALL_COMPONENTS})
