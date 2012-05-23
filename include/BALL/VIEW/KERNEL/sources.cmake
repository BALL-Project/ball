### list all filenames of the directory here ###
SET(GROUP VIEW/KERNEL)

FILE(GLOB HEADERS_LIST "include/BALL/${GROUP}/*.h" "include/BALL/${GROUP}/*.iC")

ADD_VIEW_HEADERS("${GROUP}" "${HEADERS_LIST}")

include(${CMAKE_SOURCE_DIR}/include/BALL/VIEW/KERNEL/MODES/sources.cmake)
