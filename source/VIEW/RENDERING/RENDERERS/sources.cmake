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

# THROWAWAY SPIKE -- Phase 5 prototype only (Plan 05-06).
# Gated by the BALL_SPIKE_BACKEND CMake option AND on Qt 6.7+ (the
# -DBALL_SPIKE_BACKEND_QRHI compile defn is only appended in
# CMakeLists.txt when Qt6Core_VERSION >= 6.7). Production builds never
# compile this file.
IF (BALL_SPIKE_BACKEND STREQUAL "QRhi" AND Qt6Core_VERSION VERSION_GREATER_EQUAL 6.7)
	SET(SOURCES_LIST ${SOURCES_LIST} rhiRenderer.C)
ENDIF()

ADD_VIEW_SOURCES("VIEW/RENDERING/RENDERERS" "${SOURCES_LIST}")
