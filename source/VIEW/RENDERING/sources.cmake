INCLUDE(source/VIEW/RENDERING/RENDERERS/sources.cmake)

### list all filenames of the directory here ###
SET(SOURCES_LIST
	camera.C
	glDisplayList.C
	renderSetup.C
	vertexBuffer.C
	glRenderWindow.C
	glOffscreenTarget.C
	rendererPlugin.C
	renderWindow.C
	pixelFormat.C
)

ADD_VIEW_SOURCES("VIEW/RENDERING" "${SOURCES_LIST}")
