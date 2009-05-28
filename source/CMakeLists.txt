cmake_minimum_required(VERSION 2.6)

include(AddFileDependencies)

# use g++ -g
#SET(CMAKE_BUILD_TYPE Debug)

# use g++ -O3 -DNDEBUG
SET(CMAKE_BUILD_TYPE Release)

SET(QT_MOC_EXECUTABLE /usr/bin/moc)
SET(QT_UIC_EXECUTABLE /usr/bin/uic)
SET(YACC_EXECUTABLE bison -y )
SET(FLEX_EXECUTABLE /usr/bin/flex)
#include_directories(../include /usr/include/qt4 /media/sda3/BALL-git/contrib/gsl-1.8/gsl-1.8/include /media/sda3/BALL-git/contrib/glew/include)

include_directories(../include /usr/include/qt4)
SET(LIB_DIRS "")
SET(SYS_LIBDIR "/usr/lib")
SET(SYS_INCLDIR "/usr/include")
SET(USE_gsl 0)
SET(Added_contrIncl 0)
SET(Added_contrLib 0)

function(check_headers package_name header)
	if(EXISTS ${SYS_INCLDIR}/${ARGV1})
		SET(USE_${ARGV0} 1)
	elseif(EXISTS ../contrib/${ARGV0}/include/${ARGV1})
		SET(USE_${ARGV0} 1)
		include_directories(../contrib/${ARGV0}/include/)
	elseif(EXISTS ../contrib/include/${ARGV1})
		SET(USE_${ARGV0} 1)
		if(NOT ${Added_contrIncl})
			include_directories(../contrib/include/)
			SET(Added_contrIncl 1)
		endif(NOT ${Added_contrIncl})
	else(EXISTS ${SYS_INCLDIR}/${ARGV1})
		message(STATUS "${ARGV0} headers not found!")
		SET(USE_${ARGV0} 0)
	endif(EXISTS ${SYS_INCLDIR}/${ARGV1})
endfunction(check_headers name)

function(check_lib package_name libname)
	if(EXISTS ${SYS_LIBDIR}/lib${ARGV1}.so)
		SET(USE_${ARGV0} 1)
	elseif(EXISTS ../contrib/${ARGV0}/lib/lib${ARGV1}.so)
		SET(USE_${ARGV0} 1)
	elseif(EXISTS ../contrib/lib/lib${ARGV1}.so)
		SET(USE_${ARGV0} 1)
	else(EXISTS ${SYS_LIBDIR}/lib${ARGV1}.so)
		message(STATUS "lib${ARGV1} not found!")
		SET(USE_${ARGV0} 0)
	endif(EXISTS ${SYS_LIBDIR}/lib${ARGV1}.so)
endfunction(check_lib name)


# search necessary libraries and headers
check_headers(gsl gsl/gsl_blas.h)
check_lib(gsl gsl)
check_lib(gsl gslcblas)
check_headers(glew GL/glew.h)
check_lib(glew GLEW)
check_headers(Qt qt4/Qt/qstring.h)
check_lib(Qt QtCore)
check_lib(Qt QtGui)

set(BALL_SOURCES "")
set(VIEW_SOURCES "")



macro(wrap_YACC libname path input_filename) # input= .y-files
	string(LENGTH ${input_filename} size) 
	math(EXPR size ${size}-2)
	string(SUBSTRING ${input_filename} 0 ${size} name)
	#message(STATUS "name=${name}")
	
	# YACC-prefix = first char upper-case, remove terminal 'Parser'
	string(SUBSTRING ${input_filename} 0 1 fc)
	string(TOUPPER ${fc} fc)
	math(EXPR size ${size}-7)
	string(SUBSTRING ${input_filename} 1 ${size} lc)
	set(prefix ${fc}${lc})
	#message(STATUS "prefix=${prefix}")

	add_custom_command(OUTPUT ${path}/${name}.C ${path}/${name}.h COMMAND ${YACC_EXECUTABLE} ARGS -p ${prefix} -d ${path}/${input_filename} COMMAND mv ARGS y.tab.c ${path}/${name}.C COMMAND mv ARGS y.tab.h ${path}/${name}.h MAIN_DEPENDENCY ${path}/${input_filename})
	list(APPEND ${libname}_SOURCES ${path}/${name}.C)
endmacro(wrap_YACC input_folder input_filename)


macro(wrap_FLEX libname path input_filename) # input= .l-files
	string(LENGTH ${input_filename} size) 
	math(EXPR size ${size}-2)
	string(SUBSTRING ${input_filename} 0 ${size} name)
	
	# FLEX-prefix = first char upper-case, remove terminal 'Lexer'
	string(SUBSTRING ${input_filename} 0 1 fc)
	string(TOUPPER ${fc} fc)
	math(EXPR size ${size}-6)
	string(SUBSTRING ${input_filename} 1 ${size} lc)
	set(prefix ${fc}${lc})
	#message(STATUS "prefix=${prefix}")
	
	add_custom_command(OUTPUT ${path}/${name}.C COMMAND ${FLEX_EXECUTABLE} ARGS -P ${prefix} -o ${path}/${name}.C ${path}/${input_filename} MAIN_DEPENDENCY ${path}/${input_filename})
	list(APPEND ${libname}_SOURCES ${path}/${name}.C)	
endmacro(wrap_FLEX path input_filename)


macro(wrap_UIC libname path input_filename) # input= .ui-files
	string(LENGTH ${input_filename} size) 
	math(EXPR size ${size}-3)
	string(SUBSTRING ${input_filename} 0 ${size} name)
	add_custom_command(OUTPUT ../include/BALL/VIEW/UIC/${name}Data.h COMMAND ${QT_UIC_EXECUTABLE} ARGS -o ../include/BALL/VIEW/UIC/${name}Data.h ${path}/${name}.ui MAIN_DEPENDENCY ${path}/${name}.ui)
	list(APPEND ${libname}_SOURCES ${path}/${name}.ui)
endmacro(wrap_UIC path input_filename)


macro(wrap_MOC libname path input_filename) # input= .h-files
	string(LENGTH ${input_filename} size) 
	math(EXPR size ${size}-2)
	string(SUBSTRING ${input_filename} 0 ${size} name)
	add_custom_command(OUTPUT ${path}/${name}_moc.C COMMAND ${QT_MOC_EXECUTABLE} ARGS -o ${path}/${name}_moc.C ../include/BALL/${path}/${i} DEPENDS ../include/BALL/${path}/${input_filename}) 
	list(APPEND ${libname}_SOURCES ${path}/${name}_moc.C)	
endmacro(wrap_MOC path input_filename)




#include_directories(/media/sda3/BALL-git/contrib/boost_1_39_0/include)
include_directories(/media/sda6/boost_1_39_0/include)


message(STATUS "${USE_gsl}")

# FIND_PACKAGE(Qt4)
# if(Qt4_FOUND)
# 	message(STATUS "found Qt4")
# endif(Qt4_FOUND)
# FIND_PACKAGE(GSL)
# if(GSL_FOUND)
# 	message(STATUS "found gsl")
# endif(GSL_FOUND)


#include(BENCHMARKS/CMakeLists.txt)
include(COMMON/CMakeLists.txt)
include(CONCEPT/CMakeLists.txt)
include(DATATYPE/CMakeLists.txt)
#include(DOCKING/CMakeLists.txt)
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

ADD_LIBRARY(VIEW STATIC ${VIEW_SOURCES})
#ADD_LIBRARY(VIEW SHARED ${VIEW_SOURCES})
