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

IF (BALL_HAS_RTFACT)
	SET(SOURCES_LIST ${SOURCES_LIST} rtfactRenderer.C)
ENDIF()

ADD_VIEW_SOURCES("VIEW/RENDERING/RENDERERS" "${SOURCES_LIST}")
