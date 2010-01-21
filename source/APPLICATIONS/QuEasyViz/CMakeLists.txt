PROJECT(QuEasyViz)

INCLUDE_DIRECTORIES(. ${Qwt5_INCLUDE_DIR})

### list all filenames of the directory here ###
SET(SOURCES_LIST
	bayesPlotter.C
	coefficientPlotter.C
	componentPlotter.C
	connectionManager.C
	CSVInputDataItem.C
	CSVInputDialog.C
	dataItem.C
	dataItemScene.C
	dataItemView.C
	dottedEdge.C
	edge.C
	exception.C
	featurePlotter.C
	featureSelectionDialog.C
	featureSelectionItem.C
	fileBrowser.C
	inputDataDialog.C
	inputDataItem.C
	inputDataItemIO.C
	inputPartitionItem.C
	inputPlotter.C
	main.C
	mainWindow.C
	modelConfigurationDialog.C
	modelConfigurationDialogPages.C
	modelItem.C
	partitioningItem.C
	pipeline.C
	plotter.C
	predictionItem.C
	predictionPlotter.C
	predictionResultDialog.C
	SDFInputDataItem.C
	SDFInputDialog.C
	validationDialog.C
	validationItem.C
	validationResultDialog.C
)

### the list of all files requiring a moc run ###
SET(MOC_SOURCES_LIST
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

#SET(RESOURCES_LIST
#	QuEasyViz.rc
#)

IF (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	# TODO: use individual icon for QuEasy
	LIST(APPEND RESOURCES_LIST ${CMAKE_SOURCE_DIR}/cmake/installer/BALLView.icns)
	SET(MACOSX_BUNDLE_ICON_FILE BALLView.icns)
	SET_SOURCE_FILES_PROPERTIES(${CMAKE_SOURCE_DIR}/cmake/installer/BALLView.icns
			PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
ENDIF()

QT4_WRAP_CPP(MOC_OUTFILES ${MOC_SOURCES_LIST})
#QT4_ADD_RESOURCES(RCC_OUTFILES ${RCC_SOURCES_LIST})
#QT4_WRAP_UI(UIC_OUTFILES ${UI_SOURCES_LIST})

INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR})

IF (NOT Qwt5_Qt4_LIBRARY)
	MESSAGE(SEND_ERROR "Could not find Qwt5 library linked against Qt4!")
ENDIF()

ADD_EXECUTABLE(QuEasyViz
	${SOURCES_LIST}
	${MOC_OUTFILES}
	${RCC_OUTFILES}
	${UIC_OUTFILES}
	${RESOURCES_LIST}
)

IF (WIN32)
	SET_TARGET_PROPERTIES(QuEasyViz PROPERTIES WIN32_EXECUTABLE TRUE)
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	SET_TARGET_PROPERTIES(QuEasyViz PROPERTIES MACOSX_BUNDLE TRUE)
ENDIF()

INSTALL(TARGETS QuEasyViz
	COMPONENT "${COMPONENT_QUEASY_VIZ}"
	RUNTIME DESTINATION "${BALL_RUNTIME_INSTALL_DIRECTORY}"
	BUNDLE DESTINATION "${BALL_BUNDLE_INSTALL_DIRECTORY}"
)

#INSTALL(DIRECTORY ${CMAKE_SOURCE_DIR}/doc/QuEasyViz 
#	DESTINATION "${BALL_DOCUMENTATION_INSTALL_DIRECTORY}" 
#	COMPONENT "${COMPONENT_QUEASY_VIZ}"
#)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	GET_FILENAME_COMPONENT(MACDEPLOYQT_EXECUTABLE "${QT_QMAKE_EXECUTABLE}" PATH)
	SET(MACDEPLOYQT_EXECUTABLE "${MACDEPLOYQT_EXECUTABLE}/macdeployqt")
	INSTALL(CODE "SET(MACDEPLOYQT_EXECUTABLE \"${MACDEPLOYQT_EXECUTABLE}\")" COMPONENT "${COMPONENT_QUEASY_VIZ}")
	INSTALL(CODE "SET(FIXBUNDLE ${CMAKE_SOURCE_DIR}/MacOSX/fixbundleframeworks.sh)" COMPONENT "${COMPONENT_QUEASY_VIZ}")
	INSTALL(CODE "SET(BALLVIEW_BUNDLE \"\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/${BALL_BUNDLE_INSTALL_DIRECTORY}/QuEasyViz.app\")" COMPONENT "${COMPONENT_QUEASY_VIZ}")
	INSTALL(CODE "SET(CMAKE_SOURCE_DIR ${CMAKE_SOURCE_DIR})" COMPONENT "${COMPONENT_QUEASY_VIZ}")
	INSTALL(SCRIPT "${CMAKE_SOURCE_DIR}/cmake/BALLViewCreateBundle.cmake" COMPONENT "${COMPONENT_QUEASY_VIZ}")
ENDIF()

TARGET_LINK_LIBRARIES(QuEasyViz BALL VIEW ${Qwt5_Qt4_LIBRARY})
