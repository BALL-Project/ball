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
	
	add_custom_command(OUTPUT ${path}/${name}.C COMMAND ${FLEX_EXECUTABLE} ARGS -P${prefix} -o${path}/${name}.C ${path}/${input_filename} MAIN_DEPENDENCY ${path}/${input_filename})
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

