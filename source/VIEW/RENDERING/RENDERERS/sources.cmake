### list all filenames of the directory here ###
SET(SOURCES_LIST
	glRenderer.C
	renderer.C
	POVRenderer.C
	STLRenderer.C
	tilingRenderer.C
	VRMLRenderer.C
	raytracingRenderer.C
	XML3DRenderer.C
)
### the list of all files requiring a moc run ###
SET(MOC_SOURCES_LIST "")

IF (BALL_HAS_RTFACT)
	SET(SOURCES_LIST ${SOURCES_LIST} rtfactRenderer.C)
ENDIF()

ADD_VIEW_SOURCES("VIEW/RENDERING/RENDERERS" "${SOURCES_LIST}")

ADD_BALL_MOCFILES("VIEW/RENDERING/RENDERERS" "include/BALL/VIEW/RENDERING/RENDERERS" "${MOC_SOURCES_LIST}")
