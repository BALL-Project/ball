INCLUDE(source/VIEW/RENDERING/RENDERERS/sources.cmake)

### list all filenames of the directory here ###
SET(SOURCES_LIST
	camera.C
	glDisplayList.C
	renderSetup.C
	rendererFactory.C
	vertexBuffer.C
	glRenderWindow.C
	glOffscreenTarget.C
	renderWindow.C
	pixelFormat.C
)

# THROWAWAY SPIKE -- Phase 5 prototype only (Plan 05-06).
# Gated by the BALL_SPIKE_BACKEND CMake option AND Qt 6.7+; mirrors the
# coreGLRenderer.C gating in RENDERERS/sources.cmake. Production builds
# never compile this file.
IF (BALL_SPIKE_BACKEND STREQUAL "QRhi" AND Qt6Core_VERSION VERSION_GREATER_EQUAL 6.7)
	SET(SOURCES_LIST ${SOURCES_LIST} qtRhiSurface.C)
ENDIF()

ADD_VIEW_SOURCES("VIEW/RENDERING" "${SOURCES_LIST}")
