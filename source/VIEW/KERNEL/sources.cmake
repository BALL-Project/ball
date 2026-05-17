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
	# Phase 999.45 — BALLView Refresh: Workspace consolidation. Both
	# classes compile in BALL_UI_V2=ON and OFF cells (ifdef-free);
	# Mainframe only wires them in under the BALL_UI_V2 ifdef.
	workspaceManager.C
	configMigration.C
	# Phase 999.44 — BALLView Refresh: *Controller extraction subtree.
	# The exemplar StageController lands in this plan; 8 more controllers
	# (Light/Camera/Stereo/Model/Coloring/Material/Label/Clipping) are
	# enumerated in controllers/PATTERN.md and follow in the v1.7 RC
	# patch cycle. Body gated by #ifdef BALL_UI_V2 — empty TU when OFF.
	controllers/stageController.C
	# Phase 999.44 — Tools › Legacy Settings interim home helper.
	# Static data — compiles in both BALL_UI_V2 cells. Mainframe wires
	# the submenu under the ifdef in a follow-up plan.
	legacySettingsHelper.C
)

# Phase 999.45 — workspaceManager.h has Q_OBJECT (both WorkspaceManager
# itself AND the inline WorkspaceStatusLabel companion). CMAKE_AUTOMOC is
# project-wide ON for VIEW, but Q_OBJECT in a header without a paired .C
# of the same name needs to be listed explicitly so moc is invoked on the
# header.
SET(MOC_SOURCES_LIST ${MOC_SOURCES_LIST} workspaceManager.C)

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
