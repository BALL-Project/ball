### list all filenames of the directory here ###
SET(SOURCES_LIST
	renderer.C
	POVRenderer.C
	STLRenderer.C
	tilingRenderer.C
)

IF (BALL_HAS_RTFACT)
	SET(SOURCES_LIST ${SOURCES_LIST} rtfactRenderer.C)
ENDIF()

ADD_VIEW_SOURCES("VIEW/RENDERING/RENDERERS" "${SOURCES_LIST}")
