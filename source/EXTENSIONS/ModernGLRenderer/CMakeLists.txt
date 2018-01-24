PROJECT(ModernGLRenderer)

SET(DIRECTORY source)

### list all filenames of the directory here
SET(SOURCES_LIST
	${DIRECTORY}/renderObjects/box.C
	${DIRECTORY}/renderObjects/clippingPlane.C
	${DIRECTORY}/renderObjects/compositing.C
	${DIRECTORY}/renderObjects/cylinder.C
	${DIRECTORY}/renderObjects/geometry.C
	${DIRECTORY}/renderObjects/lightSources.C
	${DIRECTORY}/renderObjects/line.C
	${DIRECTORY}/renderObjects/mesh.C
	${DIRECTORY}/renderObjects/opaque.C
	${DIRECTORY}/renderObjects/point.C
	${DIRECTORY}/renderObjects/pointVolume.C
	${DIRECTORY}/renderObjects/renderObject.C
	${DIRECTORY}/renderObjects/sphere.C
	${DIRECTORY}/renderObjects/SSAO.C
	${DIRECTORY}/renderObjects/transparent.C
	${DIRECTORY}/renderObjects/twoColoredLine.C
	${DIRECTORY}/renderObjects/volume.C
	${DIRECTORY}/vbo/VBObject.C
	${DIRECTORY}/vbo/VBOLine.C
	${DIRECTORY}/vbo/VBOPoint.C
	${DIRECTORY}/vbo/VBOQuad.C
	${DIRECTORY}/vbo/VBOTriangle.C
	${DIRECTORY}/camera.C
	${DIRECTORY}/clippingPlanes.C
	${DIRECTORY}/global.C
	${DIRECTORY}/material.C
	${DIRECTORY}/modernGLConfigDialog.C
	${DIRECTORY}/modernGLPlugin.C
	${DIRECTORY}/modernGLRenderer.C
	${DIRECTORY}/modernGLRenderSetup.C
	${DIRECTORY}/shader.C
	${DIRECTORY}/shaderManager.C
)

SET(RCC_SOURCES_LIST
	${DIRECTORY}/logo.qrc
)

SET(UI_SOURCES_LIST
	${DIRECTORY}/modernGLConfigDialog.ui
)

SET(DIRECTORY include)

### the list of all files requiring a moc run
SET(MOC_SOURCES_LIST
	${DIRECTORY}/renderObjects/box.h
	${DIRECTORY}/renderObjects/clippingPlane.h
	${DIRECTORY}/renderObjects/compositing.h
	${DIRECTORY}/renderObjects/cylinder.h
	${DIRECTORY}/renderObjects/geometry.h
	${DIRECTORY}/renderObjects/lightSources.h
	${DIRECTORY}/renderObjects/line.h
	${DIRECTORY}/renderObjects/mesh.h
	${DIRECTORY}/renderObjects/opaque.h
	${DIRECTORY}/renderObjects/point.h
	${DIRECTORY}/renderObjects/pointVolume.h
	${DIRECTORY}/renderObjects/renderObject.h
	${DIRECTORY}/renderObjects/sphere.h
	${DIRECTORY}/renderObjects/SSAO.h
	${DIRECTORY}/renderObjects/transparent.h
	${DIRECTORY}/renderObjects/twoColoredLine.h
	${DIRECTORY}/renderObjects/volume.h
	${DIRECTORY}/camera.h
	${DIRECTORY}/material.h
	${DIRECTORY}/modernGLConfigDialog.h
	${DIRECTORY}/modernGLPlugin.h
	${DIRECTORY}/modernGLRenderer.h
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

ADD_LIBRARY(pluginModernGLRenderer SHARED
	${SOURCES_LIST}
	${MOC_OUTFILES}
	${RCC_OUTFILES}
	${UIC_OUTFILES}
)

SET_TARGET_PROPERTIES(pluginModernGLRenderer PROPERTIES PREFIX "")

INSTALL(TARGETS pluginModernGLRenderer
	COMPONENT "${COMPONENT_PLUGINS}"
	RUNTIME DESTINATION "${BALL_PLUGIN_INSTALL_DIRECTORY}"
	LIBRARY DESTINATION "${BALL_PLUGIN_INSTALL_DIRECTORY}"
)

TARGET_LINK_LIBRARIES(pluginModernGLRenderer BALL VIEW)
