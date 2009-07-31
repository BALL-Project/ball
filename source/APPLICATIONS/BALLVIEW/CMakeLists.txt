set(path APPLICATIONS/BALLVIEW)

set(BALLVIEW_SOURCES "")


add_custom_command(OUTPUT ${path}/demoTutorialDialog_moc.C COMMAND ${QT_MOC_EXECUTABLE} ARGS -o ${path}/demoTutorialDialog_moc.C ${path}/demoTutorialDialog.h DEPENDS ${path}/demoTutorialDialog.h)
list(APPEND BALLVIEW_SOURCES ${path}/demoTutorialDialog_moc.C)

add_custom_command(OUTPUT ${path}/mainframe_moc.C COMMAND ${QT_MOC_EXECUTABLE} ARGS -o ${path}/mainframe_moc.C ${path}/mainframe.h DEPENDS ${path}/mainframe.h)
list(APPEND BALLVIEW_SOURCES ${path}/mainframe_moc.C)


SET(UIC_SOURCES
	aboutDialog.ui
	demoTutorialDialog.ui
)

foreach(i ${UIC_SOURCES})
	wrap_UIC(BALLVIEW ${path} ${i})
endforeach(i)


list(APPEND BALLVIEW_SOURCES 
	${path}/main.C
	${path}/demoTutorialDialog.C
	${path}/icons.C
	${path}/mainframe.C
)


link_directories(. ${LIB_DIRS})
add_executable(BALLView ${BALLVIEW_SOURCES})

set(ballview_libs "")
list(APPEND ballview_libs VIEW BALL ${LIBS} QtOpenGL QtXml QtTest)
if(USE_glew)
	list(APPEND ballview_libs GLEW)
endif(USE_glew)
target_link_libraries(BALLView ${ballview_libs})

