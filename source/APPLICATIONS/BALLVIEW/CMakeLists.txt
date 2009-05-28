cmake_minimum_required(VERSION 2.6)

# use g++ -g
#SET(CMAKE_BUILD_TYPE Debug)

# use g++ -O3 -DNDEBUG
SET(CMAKE_BUILD_TYPE Release)

SET(QT_MOC_EXECUTABLE /usr/bin/moc)
SET(QT_UIC_EXECUTABLE /usr/bin/uic)

include(AddFileDependencies)
set(BALLVIEW_SOURCES "")


include_directories(../../../include /usr/include/qt4 ../../../contrib/gsl-1.8/gsl-1.8/include ../../../contrib/glew/include)
include_directories(/media/sda6/boost_1_39_0/include)



add_custom_command(OUTPUT aboutDialog.h COMMAND ${QT_UIC_EXECUTABLE} ARGS -o aboutDialog.h aboutDialog.ui DEPENDS aboutDialog.ui)

add_custom_command(OUTPUT demoTutorialDialogData.h COMMAND ${QT_UIC_EXECUTABLE} ARGS -o demoTutorialDialogData.h demoTutorialDialog.ui DEPENDS demoTutorialDialog.ui)

add_custom_command(OUTPUT demoTutorialDialogData.C COMMAND ${QT_UIC_EXECUTABLE} ARGS -impl -o demoTutorialDialogData.C demoTutorialDialog.ui demoTutorialDialog.h DEPENDS demoTutorialDialog.h)

add_custom_command(OUTPUT demoTutorialDialog_moc.C COMMAND ${QT_MOC_EXECUTABLE} ARGS -o demoTutorialDialog_moc.C demoTutorialDialog.h DEPENDS demoTutorialDialog.h)
list(APPEND BALLVIEW_SOURCES demoTutorialDialog_moc.C)

add_custom_command(OUTPUT mainframe_moc.C COMMAND ${QT_MOC_EXECUTABLE} ARGS -o mainframe_moc.C mainframe.h DEPENDS mainframe.h)
list(APPEND BALLVIEW_SOURCES mainframe_moc.C)

list(APPEND BALLVIEW_SOURCES main.C)
list(APPEND BALLVIEW_SOURCES demoTutorialDialog.C)
list(APPEND BALLVIEW_SOURCES icons.C)
list(APPEND BALLVIEW_SOURCES mainframe.C)
#list(APPEND BALLVIEW_SOURCES splash.C)


link_directories(/media/sda7/BALL-git/source /usr/lib /usr/lib/qt4  ../../../contrib/gsl/lib ../../../contrib/glew/lib /media/sda6/boost_1_39_0/lib)
add_executable(BALLView ${BALLVIEW_SOURCES})
target_link_libraries(BALLView VIEW BALL QtCore QtGui QtOpenGL QtXml gsl gslcblas GLEW boost_system-mt QtTest mysqlclient)


#/usr/bin/c++   -O3 -DNDEBUG   CMakeFiles/BALLView.dir/demoTutorialDialog_moc.C.o CMakeFiles/BALLView.dir/mainframe_moc.C.o CMakeFiles/BALLView.dir/main.C.o CMakeFiles/BALLView.dir/demoTutorialDialog.C.o CMakeFiles/BALLView.dir/icons.C.o CMakeFiles/BALLView.dir/mainframe.C.o CMakeFiles/BALLView.dir/splash.C.o -o BALLView -rdynamic -L/media/sda3/BALL-git/source -L/usr/lib -L/usr/lib/qt4 -L/media/sda3/BALL-git/contrib/gsl-1.8/gsl-1.8/lib -L/media/sda3/BALL-git/contrib/glew/lib -L/media/sda3/BALL-git/contrib/boost_1_39_0/lib -lVIEW -lBALL -lQtCore -lQtGui -lQtOpenGL -lgsl -lgslcblas -lGLEW -lboost_system-mt -lQtTest -lmysqlclient
