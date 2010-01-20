set(path APPLICATIONS/QuEasyViz)

set(QUEASYVIZ_SOURCES "")
SET(MOC_SOURCES "")
SET(MOC_SOURCES 
	componentPlotter.h
	connectionManager.h
	CSVInputDialog.h
	dataItem.h
	dataItemScene.h
	dataItemView.h
	featurePlotter.h
	featureSelectionDialog.h
	inputDataDialog.h
	inputDataItem.h
	inputPlotter.h
	mainWindow.h
	modelConfigurationDialog.h
	modelConfigurationDialogPages.h
	modelItem.h
	plotter.h
	predictionItem.h
	predictionResultDialog.h
	SDFInputDialog.h
	validationDialog.h
	validationItem.h
	validationResultDialog.h
)

foreach(i ${MOC_SOURCES})
	wrap_MOC(QUEASYVIZ ${path} ${i})
endforeach(i)


list(APPEND QUEASYVIZ_SOURCES 
	${path}/bayesPlotter.C
	${path}/coefficientPlotter.C
	${path}/componentPlotter.C
	${path}/connectionManager.C
	${path}/CSVInputDataItem.C
	${path}/CSVInputDialog.C
	${path}/dataItem.C
	${path}/dataItemScene.C
	${path}/dataItemView.C
	${path}/dottedEdge.C
	${path}/edge.C
	${path}/exception.C
	${path}/featurePlotter.C
	${path}/featureSelectionDialog.C
	${path}/featureSelectionItem.C
	${path}/fileBrowser.C
	${path}/inputDataDialog.C
	${path}/inputDataItem.C
	${path}/inputDataItemIO.C
	${path}/inputPartitionItem.C
	${path}/inputPlotter.C
	${path}/main.C
	${path}/mainWindow.C
	${path}/modelConfigurationDialog.C
	${path}/modelConfigurationDialogPages.C
	${path}/modelItem.C
	${path}/partitioningItem.C
	${path}/pipeline.C
	${path}/plotter.C
	${path}/predictionItem.C
	${path}/predictionPlotter.C
	${path}/predictionResultDialog.C
	${path}/SDFInputDataItem.C
	${path}/SDFInputDialog.C
	${path}/validationDialog.C
	${path}/validationItem.C
	${path}/validationResultDialog.C
)


link_directories(. ${LIB_DIRS})
add_executable(APPLICATIONS/QuEasyViz/QuEasyViz ${QUEASYVIZ_SOURCES})
target_link_libraries(APPLICATIONS/QuEasyViz/QuEasyViz QuEasy BALL ${QUEASYVIZ_LIBS} ${VIEW_LIBS} ${QUEASY_LIBS} ${CORE_LIBS})

if(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
	link_directories(. ${LIB_DIRS})
	add_executable(QuEasyViz MACOSX_BUNDLE ${QUEASYVIZ_SOURCES})
	target_link_libraries(QuEasyViz QuEasy BALL ${QUEASYVIZ_LIBS} ${VIEW_LIBS} ${QUEASY_LIBS} ${CORE_LIBS})
endif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")


