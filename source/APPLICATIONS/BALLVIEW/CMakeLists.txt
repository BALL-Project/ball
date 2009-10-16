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

SET(RCC_SOURCES_LIST
	splashscreen.qrc
)

QT4_WRAP_CPP(MOC_OUTFILES ${MOC_SOURCES_LIST})
QT4_ADD_RESOURCES(RCC_OUTFILES ${RCC_SOURCES_LIST})
QT4_WRAP_UI(UIC_OUTFILES ${UI_SOURCE_LIST})

ADD_EXECUTABLE(BALLView
	${SOURCES_LIST}
	${MOC_OUTFILES}
	${RCC_OUTFILES}
	${UIC_OUTFILES}
)

TARGET_LINK_LIBRARIES(BALLView BALL VIEW)
