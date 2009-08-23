set(path APPLICATIONS/QuEasyRun)


include_directories(${path})
link_directories(. ${LIB_DIRS})
add_executable(${path}/QuEasyRun ${path}/main.C)
target_link_libraries(${path}/QuEasyRun BALL ${QUEASY_LIBS} ${CORE_LIBS})

if(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
	include_directories(${path})
	link_directories(. ${LIB_DIRS})
	add_executable(QuEasyRun MACOSX_BUNDLE ${path}/main.C)
	target_link_libraries(QuEasyRun BALL ${QUEASY_LIBS} ${CORE_LIBS})
endif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")

