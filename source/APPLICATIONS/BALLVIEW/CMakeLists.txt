PROJECT(BALLView)

### list all filenames of the directory here ###
SET(SOURCES_LIST
	demoTutorialDialog.C
	icons.C
	main.C
	mainframe.C
)

### the list of all files requiring a moc run ###
SET(MOC_SOURCES_LIST
	demoTutorialDialog.h
	mainframe.h
)

SET(UI_SOURCES_LIST
	aboutDialog.ui
	demoTutorialDialog.ui
)

QT6_WRAP_UI(UIC_OUTFILES ${UI_SOURCES_LIST})

SET(BALLVIEW_DEP_LIBRARIES BALL VIEW Qt6::Widgets)

IF(APPLE)
	SET(BALLVIEW_EXEC_PROPERTY MACOSX_BUNDLE)
	SET(MACOSX_BUNDLE_ICON_FILE BALLView.icns)
	SET(RESOURCES_LIST ${PROJECT_SOURCE_DIR}/BALLView.icns ${CMAKE_SOURCE_DIR}/LICENSE)
	SET_SOURCE_FILES_PROPERTIES(${RESOURCES_LIST} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
ELSEIF(WIN32)
	SET(BALLVIEW_EXEC_PROPERTY WIN32)
	SET(RESOURCES_LIST BALLView.rc)
ELSEIF(UNIX)
	LIST(APPEND BALLVIEW_DEP_LIBRARIES "pthread" "X11")
ENDIF()

INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR})
ADD_DEFINITIONS(${Qt6Widgets_EXECUTABLE_COMPILE_FLAGS})

ADD_EXECUTABLE(BALLView
	${BALLVIEW_EXEC_PROPERTY}
	${SOURCES_LIST}
	${MOC_OUTFILES}
	${UIC_OUTFILES}
	${RESOURCES_LIST}
)

TARGET_LINK_LIBRARIES(BALLView ${BALLVIEW_DEP_LIBRARIES})

# macOS bundle identity — populates BALLView.app/Contents/Info.plist via the
# cmake/MacOSXBundleInfo.plist.in template (Phase 05.1 plan 14, BACKLOG D5;
# CFBundleIdentifier per CONTEXT.md D-10).
# Required for Apple notarization (Phase 8): notarytool rejects bundles with
# empty CFBundleIdentifier; Developer ID code-signing validates the identifier
# against the certificate's authorized bundle IDs. The identifier is also the
# stable key for macOS preferences, sandbox identity, and LaunchServices
# "Open With" entries — changing it post-distribution requires user-pref
# migration, so it is fixed here at v1.6 release time.
IF(APPLE)
	SET_TARGET_PROPERTIES(BALLView PROPERTIES
		MACOSX_BUNDLE                      TRUE
		MACOSX_BUNDLE_GUI_IDENTIFIER       "de.uni-tuebingen.ball.ballview"
		MACOSX_BUNDLE_BUNDLE_NAME          "BALLView"
		MACOSX_BUNDLE_BUNDLE_VERSION       "${CMAKE_PROJECT_VERSION}"
		MACOSX_BUNDLE_SHORT_VERSION_STRING "${CMAKE_PROJECT_VERSION}"
		MACOSX_BUNDLE_LONG_VERSION_STRING  "BALLView ${CMAKE_PROJECT_VERSION}"
		MACOSX_BUNDLE_COPYRIGHT            "© BALL maintainers. LGPL-2.1."
		MACOSX_BUNDLE_INFO_STRING          "BALLView ${CMAKE_PROJECT_VERSION} — molecular visualization (LGPL-2.1)"
		MACOSX_BUNDLE_ICON_FILE            "BALLView.icns"
	)
ENDIF()

#Setup the translations
INCLUDE(BALLViewTranslations)

IF(APPLE)
	IF(QT_DEPLOY_EXECUTABLE)
		CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/BALLViewInstallFix.cmake.in ${PROJECT_BINARY_DIR}/BALLViewInstallFix.cmake @ONLY)
		INSTALL(SCRIPT ${PROJECT_BINARY_DIR}/BALLViewInstallFix.cmake)
	ENDIF()
ELSE()
	INSTALL(TARGETS BALLView
		COMPONENT "${COMPONENT_BALLVIEW}"
		RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
		BUNDLE DESTINATION "${BALL_BUNDLE_INSTALL_DIRECTORY}"
	)

	IF(WIN32)
		IF(QT_DEPLOY_EXECUTABLE)
			INSTALL(CODE "EXECUTE_PROCESS(COMMAND ${QT_DEPLOY_EXECUTABLE} \$\{CMAKE_INSTALL_PREFIX\}/${CMAKE_INSTALL_BINDIR}/BALLView.exe)"
				COMPONENT "${COMPONENT_BALLVIEW}")
		ENDIF()
	ENDIF()
ENDIF()

