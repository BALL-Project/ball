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
	shortcutRegistry.C
	snapshotManagerInterface.C
	stage.C
	threads.C
	UIOperationMode.C
)

ADD_VIEW_SOURCES("VIEW/KERNEL" "${SOURCES_LIST}")
