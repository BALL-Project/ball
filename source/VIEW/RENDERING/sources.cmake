INCLUDE(source/VIEW/RENDERING/RENDERERS/sources.cmake)

### list all filenames of the directory here ###
SET(SOURCES_LIST
	camera.C
	glDisplayList.C
	renderSetup.C
	vertexBuffer.C
	glRenderWindow.C
	glOffscreenTarget.C
	renderWindow.C
	pixelFormat.C
)

### the list of all files requiring a moc run ###
SET(MOC_SOURCES_LIST "")

ADD_VIEW_SOURCES("VIEW/RENDERING" "${SOURCES_LIST}")

ADD_BALL_MOCFILES("VIEW/RENDERING" "include/BALL/VIEW/RENDERING" "${MOC_SOURCES_LIST}")
