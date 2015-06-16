PROJECT("Lig3D_Tools")

# set build target dir and include dir:
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/TOOLS/Lig3DGen)

# Set BALL specific includes and compilerflags
INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR})
ADD_DEFINITIONS(${BALL_DEFINITIONS})
ADD_DEFINITIONS(${BALL_ADD_CXXFLAGS})

### list all filenames of the directory this CMakeList resides in ###
FILE(GLOB BIN_PATHES_LIST "*.C")

### derive target names from file pathes and save in TOOL_NAMES:
SET(TOOL_NAMES)

FOREACH(i ${BIN_PATHES_LIST})
    GET_FILENAME_COMPONENT(i ${i} NAME_WE) ### remove absolute paths and extension
    LIST(APPEND TOOL_NAMES ${i})
ENDFOREACH()

### generate exec-targets from list of target names
FOREACH(i ${TOOL_NAMES})
    add_executable(${i} ${i}.C)
    TARGET_LINK_LIBRARIES(${i} BALL) ### link with the super project
ENDFOREACH(i)

### add Ligand3DGenerator executable targets to the general LIG3DGEN target:
ADD_DEPENDENCIES(LIG3DGEN ${TOOL_NAMES})




