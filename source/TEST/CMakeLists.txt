### Build rules for BALL's class tests
PROJECT("BALL_tests")

ENABLE_TESTING()

## TODO: we want to set up a CDash or Dart ASAP...
INCLUDE(Dart)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

INCLUDE(cmake/BALLTestExecutables.cmake)

## BASIC
ADD_CUSTOM_TARGET(BASIC_TEST)
ADD_DEPENDENCIES(BASIC_TEST ${BALL_BASIC_TESTS})

# Now, add all tests as targets
FOREACH(t ${BALL_TESTS})
	ADD_EXECUTABLE(${t} ${t}.C)
	TARGET_LINK_LIBRARIES(${t} BALL)
	MESSAGE(STATUS "Adding test ${t}")
	ADD_TEST(${t} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${t})
ENDFOREACH()

# Create a suitable target
ADD_CUSTOM_TARGET(build_tests)
ADD_DEPENDENCIES(build_tests ${BALL_TESTS})

