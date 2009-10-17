### the directory name ###
SET(GROUP STRUCTURE)

INCLUDE(include/BALL/STRUCTURE/DOCKING/sources.cmake)

FILE(GLOB HEADERS_LIST "include/BALL/${GROUP}/*.h" "include/BALL/${GROUP}/*.iC")	

ADD_BALL_HEADERS("${GROUP}" "${HEADERS_LIST}")
