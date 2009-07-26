cmake_minimum_required(VERSION 2.6)

include(AddFileDependencies)

# use g++ -g
#SET(CMAKE_BUILD_TYPE Debug)

# use g++ -O3 -DNDEBUG
SET(CMAKE_BUILD_TYPE Release)

SET(QT_MOC_EXECUTABLE moc)
SET(QT_UIC_EXECUTABLE uic)
SET(YACC_EXECUTABLE bison -y )
SET(FLEX_EXECUTABLE flex)

# temporary hack only
include(cmake/simpleContribDetection.cmake)

include(cmake/createConfigH.cmake)

set(BALL_SOURCES "")
set(VIEW_SOURCES "")

# rules for uic, moc, yacc, flex
include(cmake/rules.cmake)


#include(BENCHMARKS/CMakeLists.txt)
include(COMMON/CMakeLists.txt)
include(CONCEPT/CMakeLists.txt)
include(DATATYPE/CMakeLists.txt)
include(ENERGY/CMakeLists.txt)
include(FORMAT/CMakeLists.txt)
include(KERNEL/CMakeLists.txt)
include(MATHS/CMakeLists.txt)
include(MOLMEC/CMakeLists.txt)
include(NMR/CMakeLists.txt)
#include(PYTHON/CMakeLists.txt)
include(QSAR/CMakeLists.txt)
include(SOLVATION/CMakeLists.txt)
include(STRUCTURE/CMakeLists.txt)
include(STRUCTURE/DOCKING/CMakeLists.txt)
include(SYSTEM/CMakeLists.txt)
include(PLUGIN/CMakeLists.txt)
include(VIEW/CMakeLists.txt)


ADD_LIBRARY(BALL STATIC ${BALL_SOURCES})
#ADD_LIBRARY(BALL_shared SHARED ${BALL_SOURCES})

#ADD_LIBRARY(VIEW STATIC ${VIEW_SOURCES})
#ADD_LIBRARY(VIEW SHARED ${VIEW_SOURCES})



# Qwt needs include-path to QtGui and QtCore
include_directories(/usr/include/qt4/QtGui /usr/include/qt4/QtCore)
include(APPLICATIONS/QSAR_GUI/CMakeLists.txt)
