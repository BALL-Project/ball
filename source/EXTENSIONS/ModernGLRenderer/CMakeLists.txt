PROJECT(ModernGLRenderer)

SET(DIRECTORY source)

### list all filenames of the directory here
SET(SOURCES_LIST
	${DIRECTORY}/Camera.C
	${DIRECTORY}/ClippingPlanes.C
	${DIRECTORY}/Global.C
	${DIRECTORY}/ShaderManager.C
	${DIRECTORY}/Shader.C
	${DIRECTORY}/renderObjects/Geometry.C
	${DIRECTORY}/renderObjects/Compositing.C
	${DIRECTORY}/renderObjects/Opaque.C
	${DIRECTORY}/renderObjects/Transparent.C
	${DIRECTORY}/renderObjects/Cylinder.C
	${DIRECTORY}/renderObjects/Mesh.C
	${DIRECTORY}/renderObjects/RenderObject.C
	${DIRECTORY}/renderObjects/Sphere.C
	${DIRECTORY}/renderObjects/Line.C
	${DIRECTORY}/renderObjects/TwoColoredLine.C
	${DIRECTORY}/renderObjects/Point.C
	${DIRECTORY}/renderObjects/SSAO.C
	${DIRECTORY}/renderObjects/Volume.C
	${DIRECTORY}/renderObjects/Box.C
	${DIRECTORY}/renderObjects/PointVolume.C
	${DIRECTORY}/renderObjects/LightSources.C
	${DIRECTORY}/renderObjects/ClippingPlane.C
	${DIRECTORY}/vbo/VBObject.C
	${DIRECTORY}/vbo/VBOQuad.C
	${DIRECTORY}/vbo/VBOPoint.C
	${DIRECTORY}/vbo/VBOTriangle.C
	${DIRECTORY}/vbo/VBOLine.C
	${DIRECTORY}/modernGLConfigDialog.C
	${DIRECTORY}/modernGLPlugin.C
	${DIRECTORY}/modernGLRenderer.C
	${DIRECTORY}/modernGLRenderSetup.C
	${DIRECTORY}/Material.C
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
	${DIRECTORY}/Camera.h
	${DIRECTORY}/renderObjects/Geometry.h
	${DIRECTORY}/renderObjects/Compositing.h
	${DIRECTORY}/renderObjects/Opaque.h
	${DIRECTORY}/renderObjects/Transparent.h
	${DIRECTORY}/renderObjects/Cylinder.h
	${DIRECTORY}/renderObjects/Mesh.h
	${DIRECTORY}/renderObjects/RenderObject.h
	${DIRECTORY}/renderObjects/Sphere.h
	${DIRECTORY}/renderObjects/Line.h
	${DIRECTORY}/renderObjects/TwoColoredLine.h
	${DIRECTORY}/renderObjects/Point.h
	${DIRECTORY}/renderObjects/SSAO.h
	${DIRECTORY}/renderObjects/Volume.h
	${DIRECTORY}/renderObjects/Box.h
	${DIRECTORY}/renderObjects/PointVolume.h
	${DIRECTORY}/renderObjects/LightSources.h
	${DIRECTORY}/renderObjects/ClippingPlane.h
	${DIRECTORY}/modernGLConfigDialog.h
	${DIRECTORY}/modernGLPlugin.h
	${DIRECTORY}/modernGLRenderer.h
	${DIRECTORY}/Material.h
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
