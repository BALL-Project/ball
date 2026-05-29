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
	# Phase 999.59-01 — base Controller class (NEW) for the command
	# contract (ARCHITECTURE-CONTRACT.md §2). Q_OBJECT header with a
	# paired controller.C of the same name → CMAKE_AUTOMOC scans it
	# automatically (no MOC_SOURCES_LIST entry needed). The companion
	# headers controller.h / applyPayload.h / controllerApplyGuard.h are
	# picked up by the FILE(GLOB ...) in include/BALL/VIEW/KERNEL/sources.cmake.
	controllers/controller.C
	# Phase 999.59-05 — minimal async-job base (§6). Q_OBJECT header
	# controllerJob.h is MOC'd automatically (companion to this .C).
	controllers/controllerJob.C
	# Phase 999.44 — BALLView Refresh: *Controller extraction subtree.
	# The exemplar StageController landed in Plan 01; Plan 04 adds 5
	# more read-only mirror Controllers for the Representation tab
	# (Model/Coloring/Material/Label/Clipping). Body gated by
	# #ifdef BALL_UI_V2 — empty TU when OFF.
	controllers/stageController.C
	controllers/modelController.C
	controllers/coloringController.C
	controllers/materialController.C
	controllers/labelController.C
	controllers/clippingController.C
	# Phase 999.44 Plan 05 — Scene-tab remainder Controllers.
	controllers/cameraController.C
	controllers/lightController.C
	controllers/stereoController.C
	# Phase 999.44 — Tools › Legacy Settings interim home helper.
	# Static data — compiles in both BALL_UI_V2 cells. Mainframe wires
	# the submenu under the ifdef in a follow-up plan.
	legacySettingsHelper.C
	# Phase 999.46 (Handover §6.1) — CommandRegistry. Body is wholly
	# gated behind #ifdef BALL_UI_V2; OFF cell produces an empty TU.
	commandRegistry.C
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
