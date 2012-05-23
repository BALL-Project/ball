SET(SOURCES_LIST
	editMode.C
	interactionMode.C
	interactionModeManager.C
	moveMode.C
	pickingMode.C
	rotateMode.C
)

ADD_VIEW_SOURCES("VIEW/KERNEL/MODES" "${SOURCES_LIST}")
