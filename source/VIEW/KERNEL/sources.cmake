INCLUDE(source/VIEW/KERNEL/MODES/sources.cmake)

### list all filenames of the directory here ###
SET(SOURCES_LIST
	clippingPlane.C
	common.C
	compositeManager.C
	connectionObject.C
	editOperation.C
	geometricObject.C
	iconLoader.C
	mainControl.C
	message.C
	modelInformation.C
	modularWidget.C
	preferencesEntry.C
	representationManager.C
	representation.C
	serverWidget.C
	shortcutRegistry.C
	snapshotManagerInterface.C
	stage.C
	threads.C
	UIOperationMode.C
	theme/themeManager.C
	theme/themedIconEngine.C
	theme/iconRegistry.C
)

SET(MOC_SOURCES_LIST ${MOC_SOURCES_LIST} serverWidget.C)

ADD_VIEW_SOURCES("VIEW/KERNEL" "${SOURCES_LIST}")

# Phase 999.40 — BALLView Refresh: compile the theme QRC into the VIEW
# shared library so ThemeManager can load `:/theme/theme-neutral.qss`.
# The generated _qrc.cpp lives in the binary dir; append it to the
# VIEW_sources list using the same mechanism the macro does, so the file
# is added even though it lives outside source/VIEW/KERNEL/.
QT6_ADD_RESOURCES(VIEW_THEME_RCC
	${CMAKE_SOURCE_DIR}/source/VIEW/KERNEL/theme/theme.qrc
)
SET(VIEW_sources ${VIEW_sources} ${VIEW_THEME_RCC})
