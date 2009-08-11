INCLUDE (CheckIncludeFiles)


SET(BALL_PATH "BALL_PATH \"${PROJECT_SOURCE_DIR}/..\"")

if(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
	SET(BALL_OS_DARWIN 1)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "SOLARIS")
	SET(BALL_OS_SOLARIS 1)
endif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")

SET(CMAKE_INCLUDE_PATH ${INCLUDE_DIRS})



# === macro-definitions for header- and compiling-checks ===

macro(check_file filename variable)
	set(RES "RES-NOTFOUND")
	find_file(RES ${ARGV0} INCLUDE_DIRS)
	if(NOT ${RES} STREQUAL "RES-NOTFOUND")
		set(${ARGV1} 1)
		message(STATUS "checking whether ${ARGV0} can be used ... yes")
	else(NOT ${RES} STREQUAL "RES-NOTFOUND")
		set(${ARGV1} 0)
		message(STATUS "checking whether ${ARGV0} can be used ... no")
	endif(NOT ${RES} STREQUAL "RES-NOTFOUND")
endmacro(check_file filename variable)

macro(check_compile filename variable body)
	set(text "#include<${ARGV0}>\n\n int main() {\n")
 	set(text ${text}${ARGV2})
	set(t2 "return 0; \n}; \n\n")
	set(text ${text}${t2})
	file(WRITE test.C "${text}")
	set(RES 0)
	try_compile(RES ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/test.C COMPILE_DEFINITIONS ${try_flag})
	if(RES)
		message(STATUS "checking whether ${ARGV0} can be used ... yes")
		set(${ARGV1} 1)
	else(RES)
		message(STATUS "checking whether ${ARGV0} can be used ... no")
		set(${ARGV1} 0)
	endif(RES)
endmacro(check_compile filename variable body)


# ===== check which of those header-files exist =======

check_file(regex.h BALL_HAS_REGEX_H)
check_file(pwd.h BALL_HAS_PWD_H)
check_file(sys/types.h BALL_HAS_SYS_TYPES_H)
check_file(sys/time.h BALL_HAS_SYS_TIME_H)
check_file(sys/times.h BALL_HAS_SYS_TIMES_H)
check_file(sys/ioctl.h BALL_HAS_SYS_IOCTL_H)
check_file(sys/stat.h BALL_HAS_SYS_STAT_H)
check_file(sys/param.h BALL_HAS_SYS_PARAM_H)
check_file(sys/socket.h BALL_HAS_SYS_SOCKET_H)
check_file(sys/sysinfo.h BALL_HAS_SYS_SYSINFO_H)
check_file(netdb.h BALL_HAS_NETDB_H)
check_file(GL/glew.h BALL_USE_GLEW)
check_file(QtCore/qthread.h BALL_QT_HAS_THREADS)
check_file(boost/asio/version.hpp BALL_HAS_BOOST_ASIO)
check_file(dirent.h BALL_HAS_DIRENT_H)
check_file(direct.h BALL_HAS_DIRECT_H)
check_file(unistd.h BALL_HAS_UNISTD_H)
check_file(stdint.h BALL_HAS_STDINT_H)

if(BALL_HAS_BOOST_ASIO)
	SET(BALL_HAS_ASIO 1)
endif(BALL_HAS_BOOST_ASIO)

if(NOT BALL_HAS_SYS_TYPES_H)
	SET(pid_t 1)
	SET(size_t 1)
endif(NOT BALL_HAS_SYS_TYPES_H)


# ==== try to compile in order to see what can be used ... ====

check_compile(sstream BALL_HAS_SSTREAM "")
check_compile(float.h BALL_HAS_FLOAT_H "")
check_compile(iostream BALL_HAS_ANSI_IOSTREAM "")

set(text "#include <rpc/types.h>\n#include <rpc/xdr.h>\nint main ()\n
	{	xdr_u_hyper(0, 0);\n return 0;\n}\n\n")
file(WRITE test.C "${text}")
set(RES 0)
try_compile(RES ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/test.C)
if(RES)
	SET(BALL_HAS_XDR_U_HYPER 1)
	message(STATUS "checking whether xdr_u_hyper() can be used ... yes")
else(RES)
	message(STATUS "checking whether xdr_u_hyper() can be used ... no")
endif(RES)


# ==== determine platform-dependend types =============

set(text "int main()\n{\nreturn sizeof(long);\n};\n\n")
file(WRITE test.C "${text}")
set(RES 0)
try_run(RES LONG_SIZE ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/test.C)
if(LONG_SIZE EQUAL 4)
	set(BALL_SIZE_TYPE "BALL_SIZE_TYPE long")
	set(BALL_INDEX_TYPE "BALL_INDEX_TYPE long")
	message(STATUS "checking size type ... long")
	message(STATUS "checking index type ... long")
else(LONG_SIZE EQUAL 4)
	set(BALL_SIZE_TYPE "BALL_SIZE_TYPE unsigned int")
	set(BALL_INDEX_TYPE "BALL_INDEX_TYPE int")
	message(STATUS "checking size type ... unsigned int")
	message(STATUS "checking index type ... int")
endif(LONG_SIZE EQUAL 4)

set(text "int main()\n{\nreturn sizeof(unsigned long);\n};\n\n")
file(WRITE test.C "${text}")
set(RES 0)
try_run(ULONG_SIZE RES ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/test.C)
if(RES AND ULONG_SIZE EQUAL 8)
	set(BALL_LONG64_TYPE "BALL_LONG64_TYPE long")
	set(BALL_ULONG64_TYPE "BALL_ULONG64_TYPE unsigned long")
	set(ULONG64_TYPE "unsigned long")
	message(STATUS "checking ulong type ... unsigned long")
	message(STATUS "checking long type ... long")
else(RES AND ULONG_SIZE EQUAL 8)
	set(text "int main()\n{\nreturn sizeof(unsigned long long);\n};\n\n")
	set(RES 0)
	file(WRITE test.C "${text}")
	try_run(ULONG_SIZE RES ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/test.C)
	if(RES AND ULONG_SIZE EQUAL 8)
		set(BALL_LONG64_TYPE "BALL_LONG64_TYPE long long")
		set(BALL_ULONG64_TYPE "BALL_ULONG64_TYPE unsigned long long")
		set(ULONG64_TYPE "unsigned long long")
		message(STATUS "checking ulong type ... unsigned long long")
		message(STATUS "checking long type ... long long ")
	endif(RES AND ULONG_SIZE EQUAL 8)
endif(RES AND ULONG_SIZE EQUAL 8)
if(NOT ULONG_SIZE EQUAL 8)
	message(FATAL_ERROR "\nUnsigned 64bit type cannot be found!\n")
endif(NOT ULONG_SIZE EQUAL 8)


# ==== check whether 'using' has to be used ===========

foreach(i RANGE 1)
	set(text "class A\n	{\nprotected: void foo(){};\n};\n\n
	class B : public A\n{\npublic: ")
	if(${i} EQUAL 0)
		set(t2 "using ")
		set(text ${text}${t2})
	endif(${i} EQUAL 0) 
	set(t2 "A::foo;\n};\n\nint main () {\nB b;\nb.foo();\nreturn 0;\n}\n\n")
	set(text ${text}${t2})
	file(WRITE test.C "${text}")
	set(RES 0)
	try_compile(RES ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/test.C)
	if(RES)
		if(${i} EQUAL 0)
			set(BALL_CFG_USING_METHOD_DIRECTIVE "BALL_CFG_USING_METHOD_DIRECTIVE ANSI")
			message(STATUS "'using'-style: ANSI")
		else(${i} EQUAL 1)
			set(BALL_CFG_USING_METHOD_DIRECTIVE "BALL_CFG_USING_METHOD_DIRECTIVE ARM")
			message(STATUS "'using'-style: ARM")
		endif(${i} EQUAL 0)
		break(RES)
	endif(RES)
endforeach(i)



# ===== check which hash_map to use ===================

set(try_flag "-std=c++0x")
check_compile(tr1/unordered_map BALL_HAS_UNORDERED_MAP "")
if(BALL_HAS_UNORDERED_MAP)
	set(BALL_MAP_NAME "BALL_MAP_NAME std::tr1::unordered_map<Key,T>")
	set(BALL_MAP_NAMESPACE "BALL_MAP_NAMESPACE tr1")
	set(BALL_EXTEND_HASH_IN_STD_NS 1)
	add_definitions(-std=c++0x)
	
	message(STATUS "BALL_ULONG64_TYPE = "${BALL_ULONG64_TYPE})
	
	# check whether hash-definition for long is needed
	set(text "#include <tr1/unordered_map>\n\nnamespace std::tr1 {\n")
	set(text "${text} template<>\n struct hash<${ULONG64_TYPE}>\n{\n")
	set(text "${text} size_t operator()(${ULONG64_TYPE} x) const { return (size_t)x; }\n};\n}\n\n int main (){ return 0; } \n\n")
	file(WRITE test.C "${text}")
	set(RES 0)
	try_compile(RES ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/test.C)
	if(RES)
		set(BALL_NEEDS_LONGSIZE_HASH 1)
		message(STATUS "checking whether longsize hash is neeed ... yes")
	else(RES)
		message(STATUS "checking whether longsize hash is neeed ... no")
	endif(RES)
	
	
else(BALL_HAS_UNORDERED_MAP)
	set(try_flag "")

	# first, find include-prefix
	check_compile(ext/hash_map BALL_EXT_INCLUDE_PREFIX "")
	if(BALL_EXT_INCLUDE_PREFIX)
		set(incl_pre "ext/")
	else(BALL_EXT_INCLUDE_PREFIX)
		set(incl_pre "")
	endif(BALL_EXT_INCLUDE_PREFIX)

	# now check to which namespace hash_map belongs
	check_compile(${incl_pre}hash_map BALL_HAS_HASH_MAP "std::hash_map<int, int> a;\n")
	if(BALL_HAS_HASH_MAP)
		set(BALL_MAP_NAMESPACE "BALL_MAP_NAMESPACE std")
		set(MAP_NAMESPACE "std")
		set(BALL_MAP_NAME "BALL_MAP_NAME std::hash_map<Key,T>")
	else(BALL_HAS_HASH_MAP)
		check_compile(${incl_pre}hash_map BALL_HAS_HASH_MAP "stdext::hash_map<int, int> a;\n")
		if(BALL_HAS_HASH_MAP)
			set(BALL_MAP_NAMESPACE "BALL_MAP_NAMESPACE stdext")
			set(MAP_NAMESPACE "stdext")
			set(BALL_MAP_NAME "BALL_MAP_NAME stdext::hash_map<Key,T>")
		endif(BALL_HAS_HASH_MAP)
	endif(BALL_HAS_HASH_MAP)
	if(NOT BALL_HAS_HASH_MAP)
		check_compile(${incl_pre}hash_map BALL_HAS_HASH_MAP "__gnu_cxx::hash_map<int, int> a;\n")
		if(BALL_HAS_HASH_MAP)
			set(BALL_MAP_NAMESPACE "BALL_MAP_NAMESPACE __gnu_cxx")
			set(MAP_NAMESPACE "__gnu_cxx")
			set(BALL_MAP_NAME "BALL_MAP_NAME __gnu_cxx::hash_map<Key,T>")
		endif(BALL_HAS_HASH_MAP)
	endif(NOT BALL_HAS_HASH_MAP)
	
	# check whether hash-definition for long is needed
	set(text "#include <${incl_pre}hash_map>\n\nnamespace ${MAP_NAMESPACE}{\n")
	set(text "${text} template<>\n struct hash<${ULONG64_TYPE}>\n{\n")
	set(text "${text} size_t operator()(${ULONG64_TYPE} x) const { return (size_t)x; }\n};\n}\n\n int main (){ return 0; } \n\n")
	file(WRITE test.C "${text}")
	set(RES 0)
	try_compile(RES ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/test.C)
	if(RES)
		set(BALL_NEEDS_LONGSIZE_HASH 1)
		message(STATUS "checking whether longsize hash is neeed ... yes")
	else(RES)
		message(STATUS "checking whether longsize hash is neeed ... no")
	endif(RES)
endif(BALL_HAS_UNORDERED_MAP)


# ==== check which kind of xdrrec_create() function to use =======

foreach(i RANGE 4)
	set(text "#include<rpc/types.h>\n#include <rpc/types.h>\n#include <rpc/xdr.h>\n\nextern \"C\" int dummy(")
	if(${i} EQUAL 0)
		set(t2 "void*, void*, unsigned int")
	elseif(${i} EQUAL 1)
		set(t2 "void*, char*, int")
	elseif(${i} EQUAL 2)
		set(t2 "char*, char*, int")
	elseif(${i} EQUAL 3)
		set(t2 "")
	elseif(${i} EQUAL 4)
		set(t2 "void*, void*, int")
	endif(${i} EQUAL 0)

	set(t3 ") {return 0;}\n\nvoid foo(){\nXDR xdrs;\nxdrrec_create(&xdrs, 0, 0, 0, dummy, dummy);\n}\n\nint main()\n{\nreturn 0;\n}") 
	set(text ${text}${t2}${t3})
	file(WRITE test.C "${text}")
	set(RES 0)
	try_compile(RES ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/test.C)
	if(RES)
		if(${i} EQUAL 0)
			set(BALL_XDRREC_CREATE_VOID_VOID_INT 1)
			message(STATUS "xdrrec_create type: XDRREC_CREATE_VOID_VOID_UINT")
		elseif(${i} EQUAL 1)
			set(BALL_XDRREC_CREATE_VOID_CHAR_INT 1)
			message(STATUS "xdrrec_create type: XDRREC_CREATE_VOID_CHAR_INT")
		elseif(${i} EQUAL 2)
			SET(BALL_XDRREC_CREATE_CHAR_CHAR_INT 1)
			message(STATUS "xdrrec_create type: XDRREC_CREATE_CHAR_CHAR_INT")
		elseif(${i} EQUAL 3)
			SET(BALL_XDRREC_CREATE_VOID 1)
			message(STATUS "xdrrec_create type: XDRREC_CREATE_VOID")
		elseif(${i} EQUAL 4)
			SET(BALL_XDRREC_CREATE_VOID_VOID_INT 1)
			message(STATUS "xdrrec_create type: XDRREC_CREATE_VOID_VOID_INT")
		endif(${i} EQUAL 0)
		break(RES)
	endif(RES)
endforeach(i)


# ==== set define's for other optional packages =======

SET(BALL_PYTHON_SUPPORT 0)
SET(BALL_HAS_VIEW 1)


# =====================================================



SET(BALL_NO_INLINE_FUNCTIONS 0)

SET(BALL_COMPILER_GXX 1)
SET(BALL_COMPILER "BALL_COMPILER GXX")
message(STATUS "COMPILER="${CMAKE_SYSTEM_NAME}-${CMAKE_CXX_COMPILER}-${CMAKE_CXX_COMPILER_VERSION})
SET(BALL_COMPILER_VERSION_MAJOR "BALL_COMPILER_VERSION_MAJOR 4")

SET(BALL_HAS_INLINE_TPL_ARGS 1)


file(REMOVE test.C)

# create config.h from config.h.in
CONFIGURE_FILE(cmake/config.h.in ../include/BALL/CONFIG/config.h)
