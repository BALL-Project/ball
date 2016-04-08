# The name of the project to build
PROJECT(ballaxy)

# We want a specific structure for our binary directories which suits galaxy...
SET(BALLAXY_TOOLS_DIR ${CMAKE_BINARY_DIR}/ballaxy/${CF_BALL_PACKAGE_VERSION}
	  CACHE PATH "Installation directory for ballaxy tools.")

#SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/TOOLS/
#	  CACHE PATH "Single output directory for building all executables.")

# Set BALL specific includes and compilerflags
INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR})
ADD_DEFINITIONS(${BALL_DEFINITIONS})
ADD_DEFINITIONS(${BALL_ADD_CXXFLAGS})

###### add TOOLS targets ########
SET(TOOLS_EXECUTABLES)
INCLUDE(sources.cmake)

FOREACH(i ${TOOLS_EXECUTABLES})
	ADD_EXECUTABLE(${i} ${i}.C)
        TARGET_LINK_LIBRARIES(${i} BALL Qt5::Core)
	SET_PROPERTY(TARGET ${i} PROPERTY FOLDER TOOLS)
        SET_TARGET_PROPERTIES(${i} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/TOOLS")
	INSTALL(TARGETS ${i}
		COMPONENT "${COMPONENT_TOOLS}"
		RUNTIME DESTINATION "${BALL_TOOLS_INSTALL_DIRECTORY}"
	)
ENDFOREACH(i)

# before finding CTD2Galaxy, let's see if the variable CTD2GALAXY_DIR has been set
# if not, we can infer it based on the value of BALL_CONTRIB_PATH
IF (BALL_CONTRIB_PATH AND NOT CTD2GALAXY_DIR)
	SET(CTD2GALAXY_DIR "${BALL_CONTRIB_PATH}/install/lib/python/CTD2Galaxy")
ENDIF()
FIND_PACKAGE(CTD2Galaxy)
MESSAGE(STATUS "PYTHONPATH = $ENV{PYTHONPATH}")
IF (NOT CTD2GALAXY)
	MESSAGE(STATUS "CTD2Galaxy or one of its requirements could not be found. The BALL-based galaxy tools will not be built.")
ELSE()
	# store the location of the configuration files in a variable
	FIND_FILE(CTD2GALAXY_MACROS_FILE
		macros.xml
		PATHS ${CMAKE_SOURCE_DIR}/cmake/ballaxy
	)       
	FIND_FILE(CTD2GALAXY_FORMATS_FILE
		file_formats.txt
		PATHS ${CMAKE_SOURCE_DIR}/cmake/ballaxy
	)
               
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
ENDIF()

