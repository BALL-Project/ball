PROJECT(BALLView)

### list all filenames of the directory here ###
SET(SOURCES_LIST
	aboutDialog.C
	demoTutorialDialog.C
	# Phase 999.43: IconBrowser dev tool. The file compiles to an empty TU
	# when BALL_UI_V2 is OFF (via #ifdef wrapping in iconBrowser.C) so it
	# is safe to keep in the unconditional source list — the matrix's
	# UI-V2-OFF cells still link the same TU set.
	iconBrowser.C
	main.C
	mainframe.C
)
# Phase 999.42: icons.C (XPM bucky_64x64_xpm array) removed; mainframe.C
# now uses VIEW::Icons::get("ball-app") which loads the SVG from the
# theme.qrc resource bundle (see source/VIEW/KERNEL/theme/icons/lucide/ball-app.svg).
# Phase 999.43: aboutDialog.ui deleted; aboutDialog.{h,C} hand-coded against
# the shared widget set (SectionHeader + FormRow) and authors.json. The .ui
# is gone — there is no fallback path needed since this compiles in both
# BALL_UI_V2 ON and OFF builds.

# === Phase 999.8-full: Sparkle (macOS) auto-update integration ===============
# Detect vendored Sparkle.xcframework. The vendor script
# packaging/macos/vendor-sparkle.sh downloads + extracts it (see
# .planning/phases/999.8-auto-update-sparkle-winsparkle/999.8-SPIKE.md §1).
# When absent (local dev clones, unsigned CI fallback path), BALL_HAS_SPARKLE
# is OFF and the sparkle_bridge.mm shim compiles a no-op stub — BALLView
# builds and runs without auto-update.
SET(BALL_SPARKLE_XCFRAMEWORK "${CMAKE_SOURCE_DIR}/packaging/macos/Sparkle.xcframework")
IF(APPLE AND EXISTS "${BALL_SPARKLE_XCFRAMEWORK}")
	SET(BALL_HAS_SPARKLE ON)
	MESSAGE(STATUS "BALLView: Sparkle auto-update ENABLED (vendored XCFramework found)")
ELSE()
	SET(BALL_HAS_SPARKLE OFF)
	IF(APPLE)
		MESSAGE(STATUS "BALLView: Sparkle auto-update DISABLED (run packaging/macos/vendor-sparkle.sh to enable)")
	ENDIF()
ENDIF()

IF(APPLE)
	# Compile sparkle_bridge.mm as Objective-C++ on macOS only. The .mm
	# extension triggers OBJCXX automatically; we explicitly enable the
	# language to keep CMake's compiler detection deterministic.
	ENABLE_LANGUAGE(OBJCXX)
	LIST(APPEND SOURCES_LIST sparkle_bridge.mm)
ENDIF()
# === end Phase 999.8-full ===================================================

### the list of all files requiring a moc run ###
SET(MOC_SOURCES_LIST
	aboutDialog.h
	demoTutorialDialog.h
	# Phase 999.43: iconBrowser.h has Q_OBJECT inside a BALL_UI_V2 ifdef;
	# CMAKE_AUTOMOC is ON project-wide so moc will be invoked on the
	# header when the macro expands, and produce an empty moc output
	# (no Q_OBJECT visible) when the flag is OFF. Listed here for
	# explicitness / discoverability.
	iconBrowser.h
	mainframe.h
)

SET(UI_SOURCES_LIST
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

# === Phase 999.8-full: Sparkle (macOS) link + framework install =============
IF(APPLE AND BALL_HAS_SPARKLE)
	TARGET_COMPILE_DEFINITIONS(BALLView PRIVATE BALL_HAS_SPARKLE=1)
	# Add the XCFramework's macOS slice to the framework search path. The
	# .xcframework bundle contains macos-arm64_x86_64/Sparkle.framework
	# which is what the linker resolves. Sparkle ships pre-signed; the
	# Phase 8a `codesign --deep --force` pass re-signs it under BALL's
	# Developer ID + hardened runtime (R-sparkle-helper-sign mitigation —
	# see 999.8-SPIKE.md §5).
	TARGET_LINK_OPTIONS(BALLView PRIVATE
		-F${BALL_SPARKLE_XCFRAMEWORK}/macos-arm64_x86_64
	)
	TARGET_LINK_LIBRARIES(BALLView "-framework Sparkle")
	# Install Sparkle.framework into Contents/Frameworks/ AFTER macdeployqt
	# runs (release.yml orders these correctly — macdeployqt first, then
	# Sparkle is copied in by the post-build step driven from
	# BALLViewInstallFix.cmake.in's downstream hook).
ENDIF()
# === Phase 999.8-full: WinSparkle (Windows) link =============================
IF(WIN32)
	# winsparkle vcpkg port exposes the unofficial::winsparkle::winsparkle
	# imported target. find_package is best-effort: when the port isn't
	# installed (older vcpkg manifests, or local dev without --vcpkg), we
	# fall through and BALL_HAS_WINSPARKLE stays OFF — main.C's stub
	# compiles in and BALLView ships without auto-update on that build.
	FIND_PACKAGE(unofficial-winsparkle CONFIG QUIET)
	IF(TARGET unofficial::winsparkle::winsparkle)
		SET(BALL_HAS_WINSPARKLE ON)
		TARGET_COMPILE_DEFINITIONS(BALLView PRIVATE BALL_HAS_WINSPARKLE=1)
		TARGET_LINK_LIBRARIES(BALLView unofficial::winsparkle::winsparkle)
		MESSAGE(STATUS "BALLView: WinSparkle auto-update ENABLED (vcpkg port found)")
	ELSE()
		SET(BALL_HAS_WINSPARKLE OFF)
		MESSAGE(STATUS "BALLView: WinSparkle auto-update DISABLED (winsparkle vcpkg port not installed)")
	ENDIF()
ENDIF()
# === end Phase 999.8-full ===================================================

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

