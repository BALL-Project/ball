PROJECT(Jupyter)

IF(NOT BALL_HAS_VIEW)
	MESSAGE(STATUS "BALL is configured to not build the VIEW components. Disabling the Jupyter plugin.")
	RETURN()
ENDIF()

IF(NOT USE_QTWEBENGINE)
	MESSAGE(STATUS "QtWebEngine support has not been enabled. Disabling the Jupyter plugin.")
	RETURN()
ENDIF()

SET(DIRECTORY source)
### list all filenames of the directory here ###
SET(UI_SOURCES_LIST
	${DIRECTORY}/jupyterPreferences.ui
	${DIRECTORY}/jupyterServerTab.ui
)

SET(SOURCES_LIST
	${DIRECTORY}/jupyterPlugin.C
	${DIRECTORY}/jupyterPreferences.C
	${DIRECTORY}/jupyterServer.C
	${DIRECTORY}/jupyterServerTab.C
	${DIRECTORY}/jupyterTab.C
	${DIRECTORY}/jupyterWidget.C
)

SET(RCC_SOURCES_LIST
	${DIRECTORY}/logo.qrc
)

SET(DIRECTORY include)
### the list of all files requiring a moc run ###
SET(MOC_SOURCES_LIST
	${DIRECTORY}/jupyterPlugin.h
	${DIRECTORY}/jupyterPreferences.h
	${DIRECTORY}/jupyterServer.h
	${DIRECTORY}/jupyterServerTab.h
	${DIRECTORY}/jupyterTab.h
	${DIRECTORY}/jupyterWidget.h
)

QT5_WRAP_CPP(MOC_OUTFILES ${MOC_SOURCES_LIST})
QT5_ADD_RESOURCES(RCC_OUTFILES ${RCC_SOURCES_LIST})
QT5_WRAP_UI(UIC_OUTFILES ${UI_SOURCES_LIST})

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR})

ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_SHARED)

ADD_LIBRARY(pluginJupyter SHARED
	${SOURCES_LIST}
	${MOC_OUTFILES}
	${RCC_OUTFILES}
	${UIC_OUTFILES}
)

SET_TARGET_PROPERTIES(pluginJupyter PROPERTIES
	PREFIX ""
)

INSTALL(TARGETS pluginJupyter
	COMPONENT "${COMPONENT_PLUGINS}"
	RUNTIME DESTINATION "${BALL_PLUGIN_INSTALL_DIRECTORY}"
	LIBRARY DESTINATION "${BALL_PLUGIN_INSTALL_DIRECTORY}"
)

TARGET_LINK_LIBRARIES(pluginJupyter BALL VIEW Qt5::WebEngineWidgets)
