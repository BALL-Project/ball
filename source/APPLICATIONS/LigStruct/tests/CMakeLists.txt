PROJECT("LigStruct_tests")

# set build target dir and include dir:
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/LigStruct/tests)
INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR})

### list all filenames of the directory this CMakeList resides in ###
FILE(GLOB TEST_PATHES_LIST "*.C")

### derive target names from file pathes and save in LIGSTRUCT_TESTS:
SET(LIGSTRUCT_TESTS)

FOREACH(i ${TEST_PATHES_LIST})
    GET_FILENAME_COMPONENT(i ${i} NAME_WE) ### remove absolute paths and extension
    LIST(APPEND LIGSTRUCT_TESTS ${i})
ENDFOREACH()

### generate testing-targets from list of target names
FOREACH(i ${LIGSTRUCT_TESTS})
    add_executable(${i} ${i}.C)
    TARGET_LINK_LIBRARIES(${i} BALL) ### link with the super project
ENDFOREACH(i)

### add LigStruct testing targets to the general LIGSTRUCT target:
ADD_DEPENDENCIES(LIGSTRUCT ${LIGSTRUCT_TESTS})






