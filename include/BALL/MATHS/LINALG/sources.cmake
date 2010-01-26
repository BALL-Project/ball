### list all filenames of the directory here ###
SET(GROUP MATHS/LINALG)

FILE(GLOB HEADERS_LIST "include/BALL/${GROUP}/*.h" "include/BALL/${GROUP}/*.iC" "include/BALL/${GROUP}/*.ih")	

ADD_BALL_HEADERS("${GROUP}" "${HEADERS_LIST}")
