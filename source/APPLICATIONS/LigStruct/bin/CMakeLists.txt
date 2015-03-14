PROJECT("LigStruct_bins")

# set build target dir and include dir:
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/LigStruct)
INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR})

### list all filenames of the directory this CMakeList resides in ###
FILE(GLOB BIN_PATHES_LIST "*.C")

### derive target names from file pathes and save in LIGSTRUCT_BINS:
SET(LIGSTRUCT_BINS)

FOREACH(i ${BIN_PATHES_LIST})
    GET_FILENAME_COMPONENT(i ${i} NAME_WE) ### remove absolute paths and extension
    LIST(APPEND LIGSTRUCT_BINS ${i})
ENDFOREACH()

### generate exec-targets from list of target names
FOREACH(i ${LIGSTRUCT_BINS})
    add_executable(${i} ${i}.C)
    TARGET_LINK_LIBRARIES(${i} BALL) ### link with the super project
ENDFOREACH(i)
 
### add LigStruct executable targets to the general LIGSTRUCT target:
ADD_DEPENDENCIES(LIGSTRUCT ${LIGSTRUCT_BINS})






