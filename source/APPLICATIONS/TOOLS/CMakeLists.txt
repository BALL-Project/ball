# The name of the project to build
PROJECT(ballaxy)

# We want a specific structure for our binary directories which suits galaxy...
SET(BALLAXY_TOOLS_DIR ${CMAKE_BINARY_DIR}/ballaxy/${CF_BALL_PACKAGE_VERSION}
	  CACHE PATH "Installation directory for ballaxy tools.")

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/TOOLS/
	  CACHE PATH "Single output directory for building all executables.")

# Set BALL specific includes and compilerflags
INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR})
ADD_DEFINITIONS(${BALL_DEFINITIONS})
ADD_DEFINITIONS(${BALL_ADD_CXXFLAGS})

###### add TOOLS targets ########
SET(TOOLS_EXECUTABLES)
INCLUDE(sources.cmake)

FOREACH(i ${TOOLS_EXECUTABLES})
	ADD_EXECUTABLE(${i} ${i}.C)
	TARGET_LINK_LIBRARIES(${i} BALL)
	SET_PROPERTY(TARGET ${i} PROPERTY FOLDER TOOLS)
ENDFOREACH(i)

CONFIGURE_FILE(
		"${CMAKE_SOURCE_DIR}/cmake/ballaxy/env.sh.in"
		"${CMAKE_BINARY_DIR}/ballaxy/${CF_BALL_PACKAGE_VERSION}/env.sh"
		@ONLY
)

CONFIGURE_FILE(
		"${CMAKE_SOURCE_DIR}/cmake/ballaxy/setupBALLAXYDirectories.cmake.in"
		"${PROJECT_BINARY_DIR}/cmake/setupBALLAXYDirectories.cmake"
		@ONLY
)

EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E create_symlink 
	                 "${CMAKE_BINARY_DIR}/ballaxy/${CF_BALL_PACKAGE_VERSION}"
									 "${CMAKE_BINARY_DIR}/ballaxy/default")


ADD_CUSTOM_TARGET(ballaxy
	COMMAND ${CMAKE_COMMAND} -P "${PROJECT_BINARY_DIR}/cmake/setupBALLAXYDirectories.cmake"
)
ADD_DEPENDENCIES(ballaxy ${TOOLS_EXECUTABLES})

