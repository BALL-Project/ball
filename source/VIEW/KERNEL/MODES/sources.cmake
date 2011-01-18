SET(SOURCES_LIST
	editMode.C
	interactionMode.C
	interactionModeManager.C
	moveMode.C
	pickingMode.C
	rotateMode.C
)

SET(MOC_SOURCES_LIST
	moveMode.C
	editMode.C
	interactionMode.C
	interactionModeManager.C
)

ADD_VIEW_SOURCES("VIEW/KERNEL/MODES" "${SOURCES_LIST}")
ADD_BALL_MOCFILES("VIEW/KERNEL/MODES" "include/BALL/VIEW/KERNEL/MODES" "${MOC_SOURCES_LIST}")
