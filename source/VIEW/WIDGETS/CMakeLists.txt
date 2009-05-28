set(path VIEW/WIDGETS)


SET(MOC_SOURCES "")
SET(MOC_SOURCES 
	datasetControl.h
	dockingController.h
	dockWidget.h
	editableScene.h
	fileObserver.h
	genericControl.h
	geometricControl.h
	helpViewer.h
	logView.h
	molecularControl.h
	molecularStructure.h
	scene.h
	shortcutTableView.h
	testFramework.h
)

foreach(i ${MOC_SOURCES})
	wrap_MOC(VIEW ${path} ${i})
endforeach(i)


list(APPEND VIEW_SOURCES 
	VIEW/WIDGETS/datasetControl.C
	VIEW/WIDGETS/dockingController.C
	VIEW/WIDGETS/dockWidget.C
	VIEW/WIDGETS/editableScene.C
	VIEW/WIDGETS/fileObserver.C
	VIEW/WIDGETS/genericControl.C
	VIEW/WIDGETS/geometricControl.C
	VIEW/WIDGETS/helpViewer.C
	VIEW/WIDGETS/logView.C
	VIEW/WIDGETS/molecularControl.C
	VIEW/WIDGETS/molecularStructure.C
	VIEW/WIDGETS/scene.C
	VIEW/WIDGETS/SDWidget.C
	VIEW/WIDGETS/shortcutTableView.C
	VIEW/WIDGETS/testFramework.C
)

