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

# THROWAWAY SPIKE -- Phase 5 prototype only (Plan 05-05).
# Gated by the BALL_SPIKE_BACKEND CMake option (default OFF). Production
# builds never compile this file.
IF (BALL_SPIKE_BACKEND STREQUAL "GLCore")
	SET(SOURCES_LIST ${SOURCES_LIST} coreGLRenderer.C)
ENDIF()

ADD_VIEW_SOURCES("VIEW/RENDERING/RENDERERS" "${SOURCES_LIST}")
