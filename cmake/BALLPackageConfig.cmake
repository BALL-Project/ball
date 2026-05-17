SET(BALLVIEW_SOURCE_DIR ${PROJECT_SOURCE_DIR}/source/APPLICATIONS/BALLVIEW)

SET(CPACK_PACKAGE_NAME "BALL")
SET(CPACK_PACKAGE_VENDOR "ball-project.org")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "BALL - The Biochemical Algorithms Library")
SET(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
SET(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
SET(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
SET(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "BALL-${CPACK_PACKAGE_VERSION}")
# Phase 999.47 §7 — Welcome.rtf retired in favour of the in-product
# WelcomeScreen widget + the 1.7 What's New card. The CPack installer
# welcome page now reads a Markdown-derived plain-text file
# (Welcome.txt) authored to match the new product copy.
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${BALLVIEW_SOURCE_DIR}/Welcome.txt")
SET(CPACK_RESOURCE_FILE_WELCOME "${BALLVIEW_SOURCE_DIR}/Welcome.txt")
SET(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/COPYRIGHT")

IF (WIN32 AND NOT UNIX)

	# Phase 8b modernization (2026-05-17): vcpkg + MSVC 2022 is the ONLY
	# supported Windows toolchain. Drop the MSVC 2010/2012/2013/2015
	# hardcoding (historical; targets predate vcpkg manifest mode and
	# Windows 10/11 era). Compiler suffix removed from the file name —
	# the toolchain is implicit in v1.7+.
	SET(BALL_PLATFORM_TAG "windows-x64")
	SET(CPACK_PACKAGE_FILE_NAME "BALLView-${PROJECT_VERSION}-${BALL_PLATFORM_TAG}-installer")

	# CPack NSIS generator (Decision #1 in 08-INSTALLER-FORMATS-RESEARCH.md).
	SET(CPACK_GENERATOR "NSIS")

	# Decision #3: per-user install, no admin/UAC prompt.
	# $LOCALAPPDATA\Programs is the modern Windows per-user install root
	# (mirrors what apps like VS Code, GitHub Desktop, Discord do).
	# CPACK_NSIS_MODIFY_PATH is intentionally NOT set — leaving it unset
	# means the installer does not need to mutate the per-user / system
	# PATH, which is what triggers a UAC admin prompt on machine-wide
	# installs.
	SET(CPACK_NSIS_INSTALL_ROOT "$LOCALAPPDATA\\\\Programs")

	# Decision #4: English-only chrome (no localised installer strings).
	# This is the default for CPack NSIS; we just note it explicitly here.

	SET(CPACK_NSIS_DISPLAY_NAME "BALLView ${CPACK_PACKAGE_VERSION}")
	SET(CPACK_NSIS_PACKAGE_NAME "BALLView ${CPACK_PACKAGE_VERSION}")
	SET(CPACK_NSIS_HELP_LINK "https://ball-project.org")
	SET(CPACK_NSIS_URL_INFO_ABOUT "https://github.com/BALL-Project/ball")
	SET(CPACK_NSIS_CONTACT "https://github.com/BALL-Project/ball/issues")

	# Start Menu shortcuts (BALLView app + project homepage).
	SET(CPACK_NSIS_MENU_LINKS
		"bin\\\\BALLView.exe" "BALLView"
		"https://ball-project.org" "BALL Project Homepage")

	# Installer icons (existing assets under data/graphics/).
	SET(CPACK_NSIS_MUI_ICON "${PROJECT_SOURCE_DIR}\\\\data\\\\graphics\\\\ballview.ico")
	SET(CPACK_NSIS_MUI_UNIICON "${PROJECT_SOURCE_DIR}\\\\data\\\\graphics\\\\ballview.ico")
	SET(CPACK_PACKAGE_ICON "${PROJECT_SOURCE_DIR}\\\\data\\\\graphics\\\\nsis_installer_header.bmp")
	SET(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\BALLView.exe")

	# RTfact path retained for back-compat with research builds (BALL_HAS_RTFACT
	# is unset in v1.7+; legacy users on the rtfact branch still benefit).
	IF (BALL_HAS_RTFACT)
		SET(CPACK_PACKAGE_NAME "BALL-RT")
		SET(CPACK_NSIS_DISPLAY_NAME "BALLView-RT ${CPACK_PACKAGE_VERSION}")
	ENDIF()

	SET(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "NOT-FOUND")
	SET(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "NOT-FOUND")
	FILE(READ "${PROJECT_SOURCE_DIR}/cmake/NSISExtraInstallCommands.cmake" CPACK_NSIS_EXTRA_INSTALL_COMMANDS)
	FILE(READ "${PROJECT_SOURCE_DIR}/cmake/NSISExtraUninstallCommands.cmake" CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS)
	IF (PYTHON_INSTALLER)
		CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/cmake/NSISEmbeddedInstallers.cmake.in ${PROJECT_BINARY_DIR}/NSISEmbeddedInstallers.nsh)
		SET(CPACK_NSIS_EMBEDDED_INSTALLERS "!Include ..\\\\..\\\\..\\\\NSISEmbeddedInstallers.nsh")
	ENDIF()

	# Phase 8b: drop the legacy `RequestExecutionLevel highest` directives
	# (historical machine-wide install assumption). Per-user install means
	# the default `RequestExecutionLevel user` is correct — no UAC prompt.
	# CPack auto-emits the user-level directive when CPACK_NSIS_MODIFY_PATH
	# is unset and CPACK_NSIS_INSTALL_ROOT points at $LOCALAPPDATA.

	SET(CPACK_PACKAGE_EXECUTABLES ${BALL_EXECUTABLES})
ELSEIF(APPLE)
	EXECUTE_PROCESS(COMMAND sw_vers -productVersion OUTPUT_VARIABLE SW_VERS_OUTPUT)
	STRING(FIND ${SW_VERS_OUTPUT} "." PATCH_VERSION_DELIM REVERSE)
	STRING(SUBSTRING ${SW_VERS_OUTPUT} 0 ${PATCH_VERSION_DELIM} MACOS_PRODUCT_VER)
	SET(CPACK_PACKAGE_FILE_NAME "BALLView-${PROJECT_VERSION}-macOS-${MACOS_PRODUCT_VER}")

	SET(CPACK_GENERATOR DragNDrop)
ELSE()
	# ====================================================================
	# Linux (Phase 8c part-2): TGZ + DEB + RPM generators side-by-side.
	# AppImage is NOT a CPack generator — it's produced out-of-band via
	# linuxdeploy + linuxdeploy-plugin-qt in .github/workflows/release.yml
	# from the CMake `install` tree. The TGZ generator is retained as the
	# portable fallback (Phase 8c part-1, PKG-LINUX-01).
	#
	# Dependency lists are derived from BUILD-linux.md's apt install line.
	# CPACK_DEBIAN_PACKAGE_DEPENDS is a literal string (not auto-resolved
	# via dpkg-shlibdeps) — predictable across runner-vs-target dpkg
	# versions; the SHLIBDEPS auto-mode was rejected during plan review
	# because the CI runner's installed Qt 6.8 (jurplel/install-qt-action)
	# is NOT an apt-managed package and dpkg-shlibdeps cannot resolve its
	# Qt 6 SONAMEs to apt package names. The explicit DEPENDS line below
	# names the apt packages a user must have installed on the target.
	# ====================================================================
	SET(CPACK_STRIP_FILES "bin/BALLView bin/libBALL.so bin/libVIEW.so")
	SET(CPACK_GENERATOR "TGZ;DEB;RPM")

	# Architecture: derive via `dpkg --print-architecture` for the DEB
	# generator (amd64 / arm64 / etc.); CPack auto-fills the RPM
	# architecture from CMAKE_SYSTEM_PROCESSOR.
	EXECUTE_PROCESS(
		COMMAND dpkg --print-architecture
		OUTPUT_VARIABLE BALL_DEB_ARCH
		OUTPUT_STRIP_TRAILING_WHITESPACE
		ERROR_QUIET
	)
	IF (BALL_DEB_ARCH)
		SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "${BALL_DEB_ARCH}")
	ENDIF()

	# Package file-name suffix: BALLView-lgpl-1.7.0-linux-x86_64.{tar.gz,...}
	# Matches the workflow's expected output filenames.
	SET(CPACK_PACKAGE_FILE_NAME "BALLView-lgpl-${PROJECT_VERSION}-linux-${CMAKE_SYSTEM_PROCESSOR}")

	# --- CPack DEB generator (Debian / Ubuntu) -------------------------
	SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "BALL Project <ball-project@example.org>")
	SET(CPACK_DEBIAN_PACKAGE_SECTION "science")
	SET(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
	SET(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://ball-project.org")
	# Runtime dependencies derived from BUILD-linux.md's apt install line.
	# Boost is given as an `|`-alternation between 1.83 (Ubuntu 24.04) and
	# 1.74 (Debian 12) so the .deb installs cleanly on both.
	SET(CPACK_DEBIAN_PACKAGE_DEPENDS
		"libqt6core6 (>= 6.8), libqt6gui6 (>= 6.8), libqt6widgets6 (>= 6.8), libqt6opengl6 (>= 6.8), libqt6openglwidgets6 (>= 6.8), libboost-system1.83.0 | libboost-system1.74.0, libtbb12, libeigen3-dev, libopenbabel7 | libopenbabel-dev")

	# --- CPack RPM generator (Fedora / RHEL / openSUSE) ----------------
	SET(CPACK_RPM_PACKAGE_LICENSE "LGPLv2+")
	SET(CPACK_RPM_PACKAGE_GROUP "Sciences/Chemistry")
	SET(CPACK_RPM_PACKAGE_URL "https://ball-project.org")
	SET(CPACK_RPM_PACKAGE_REQUIRES
		"qt6-qtbase >= 6.8, qt6-qtbase-gui >= 6.8, qt6-qtopengl >= 6.8, boost, tbb, eigen3, openbabel")
	# Non-relocatable: BALL_DATA_PATH discovery assumes a fixed prefix.
	SET(CPACK_RPM_PACKAGE_RELOCATABLE FALSE)
ENDIF()

INCLUDE(CPack)
