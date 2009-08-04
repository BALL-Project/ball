set(path APPLICATIONS/BALLVIEW)

set(BALLVIEW_SOURCES "")


add_custom_command(OUTPUT ${path}/demoTutorialDialog_moc.C COMMAND ${QT_MOC_EXECUTABLE} ARGS -o ${path}/demoTutorialDialog_moc.C ${path}/demoTutorialDialog.h DEPENDS ${path}/demoTutorialDialog.h)
list(APPEND BALLVIEW_SOURCES ${path}/demoTutorialDialog_moc.C)

add_custom_command(OUTPUT ${path}/mainframe_moc.C COMMAND ${QT_MOC_EXECUTABLE} ARGS -o ${path}/mainframe_moc.C ${path}/mainframe.h DEPENDS ${path}/mainframe.h)
list(APPEND BALLVIEW_SOURCES ${path}/mainframe_moc.C)

add_custom_command(OUTPUT ${path}/aboutDialog.h COMMAND ${QT_UIC_EXECUTABLE} ARGS -o ${path}/aboutDialog.h ${path}/aboutDialog.ui MAIN_DEPENDENCY ${path}/aboutDialog.ui)
list(APPEND BALLVIEW_SOURCES ${path}/aboutDialog.ui)
	
add_custom_command(OUTPUT ${path}/demoTutorialDialogData.h COMMAND ${QT_UIC_EXECUTABLE} ARGS -o ${path}/demoTutorialDialogData.h ${path}/demoTutorialDialog.ui MAIN_DEPENDENCY ${path}/demoTutorialDialog.ui)
list(APPEND BALLVIEW_SOURCES ${path}/demoTutorialDialog.ui)


list(APPEND BALLVIEW_SOURCES 
	${path}/main.C
	${path}/demoTutorialDialog.C
	${path}/icons.C
	${path}/mainframe.C
)


link_directories(. ${LIB_DIRS})
add_executable(${path}/BALLView ${BALLVIEW_SOURCES})

set(ballview_libs "")
list(APPEND ballview_libs VIEW BALL ${LIBS} QtOpenGL QtXml QtTest)
if(USE_glew)
	list(APPEND ballview_libs GLEW)
endif(USE_glew)
target_link_libraries(${path}/BALLView ${ballview_libs})

