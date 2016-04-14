### list all filenames of the directory here ###
SET(GROUP VIEW/KERNEL)

FILE(GLOB HEADERS_LIST "include/BALL/${GROUP}/*.h" "include/BALL/${GROUP}/*.iC")

IF(NOT BALL_HAS_ASIO)
	LIST(REMOVE_ITEM HEADERS_LIST "${CMAKE_CURRENT_SOURCE_DIR}/include/BALL/${GROUP}/serverWidget.h")
ENDIF()

ADD_VIEW_HEADERS("${GROUP}" "${HEADERS_LIST}")

include(${CMAKE_SOURCE_DIR}/include/BALL/VIEW/KERNEL/MODES/sources.cmake)
