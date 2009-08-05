
set(path APPLICATIONS/QSAR_GUI)


set(QSARGUI_SOURCES "")
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
	wrap_MOC(QSARGUI ${path} ${i})
endforeach(i)


list(APPEND QSARGUI_SOURCES 
	APPLICATIONS/QSAR_GUI/bayesPlotter.C
	APPLICATIONS/QSAR_GUI/coefficientPlotter.C
	APPLICATIONS/QSAR_GUI/componentPlotter.C
	APPLICATIONS/QSAR_GUI/connectionManager.C
	APPLICATIONS/QSAR_GUI/CSVInputDataItem.C
	APPLICATIONS/QSAR_GUI/CSVInputDialog.C
	APPLICATIONS/QSAR_GUI/dataItem.C
	APPLICATIONS/QSAR_GUI/dataItemScene.C
	APPLICATIONS/QSAR_GUI/dataItemView.C
	APPLICATIONS/QSAR_GUI/dottedEdge.C
	APPLICATIONS/QSAR_GUI/edge.C
	APPLICATIONS/QSAR_GUI/exception.C
	APPLICATIONS/QSAR_GUI/featurePlotter.C
	APPLICATIONS/QSAR_GUI/featureSelectionDialog.C
	APPLICATIONS/QSAR_GUI/featureSelectionItem.C
	APPLICATIONS/QSAR_GUI/fileBrowser.C
	APPLICATIONS/QSAR_GUI/inputDataDialog.C
	APPLICATIONS/QSAR_GUI/inputDataItem.C
	APPLICATIONS/QSAR_GUI/inputDataItemIO.C
	APPLICATIONS/QSAR_GUI/inputPartitionItem.C
	APPLICATIONS/QSAR_GUI/inputPlotter.C
	APPLICATIONS/QSAR_GUI/main.C
	APPLICATIONS/QSAR_GUI/mainWindow.C
	APPLICATIONS/QSAR_GUI/modelConfigurationDialog.C
	APPLICATIONS/QSAR_GUI/modelConfigurationDialogPages.C
	APPLICATIONS/QSAR_GUI/modelItem.C
	APPLICATIONS/QSAR_GUI/partitioningItem.C
	APPLICATIONS/QSAR_GUI/pipeline.C
	APPLICATIONS/QSAR_GUI/plotter.C
	APPLICATIONS/QSAR_GUI/predictionItem.C
	APPLICATIONS/QSAR_GUI/predictionPlotter.C
	APPLICATIONS/QSAR_GUI/predictionResultDialog.C
	APPLICATIONS/QSAR_GUI/SDFInputDataItem.C
	APPLICATIONS/QSAR_GUI/SDFInputDialog.C
	APPLICATIONS/QSAR_GUI/validationDialog.C
	APPLICATIONS/QSAR_GUI/validationItem.C
	APPLICATIONS/QSAR_GUI/validationResultDialog.C
)


link_directories(. ${LIB_DIRS})
add_executable(APPLICATIONS/QSAR_GUI/QuEasyViz ${QSARGUI_SOURCES})
target_link_libraries(APPLICATIONS/QSAR_GUI/QuEasyViz BALL ${LIBS})

