// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_COMMON_DEBUG_H
#define BALL_COMMON_DEBUG_H

#ifndef BALL_CONFIG_CONFIG_H
#	include <BALL/CONFIG/config.h>
#endif

#ifdef BALL_NO_INLINE_FUNCTIONS
#	define BALL_INLINE
#else
#	define BALL_INLINE inline
#endif

#include <string>
#include <cstring>

#ifdef BALL_DEBUG

#	define BALL_PRECONDITION_EXCEPTION(condition, message)\
	if (!(condition))\
	{\
		Exception::Precondition e(__FILE__, __LINE__, #condition);\
		if (strcmp(message, "") != 0)\
		{\
      ::std::string tmp(e.getMessage());\
			tmp += ::std::string(message);\
			e.setMessage(tmp);\
		}\
		throw e;\
	}\

#	define BALL_POSTCONDITION_EXCEPTION(condition, message)\
	if (!(condition))\
	{\
		Exception::Postcondition e(__FILE__, __LINE__, #condition);\
		if (strcmp(message, "") != 0)\
		{\
      std::string tmp(e.getMessage());\
			tmp += std::string(message);\
			e.setMessage(tmp);\
		}\
		throw e;\
	}\

# if defined(BALL_COMPILER_GXX) || defined(BALL_COMPILER_LLVM)
# include <execinfo.h>
# include <cxxabi.h>

# define BALL_PRINT_BACKTRACE()\
	{\
		void *bt_addresses[100];\
		char **bt;\
		\
		int bt_size = backtrace(bt_addresses, 100);\
		bt = backtrace_symbols(bt_addresses, bt_size);\
		\
		char* demangled=0;\
		size_t num_demangled;\
		int status;\
		std::vector<String> split;\
		\
		for(int i = 0; i < bt_size; i++)\
		{\
			String mangled(bt[i]);\
			\
			Log.warn() << i << ": " << bt[i] << " ";\
			mangled = mangled.getField(1, "()");\
			mangled = mangled.getField(0, "+");\
			\
			char* out = abi::__cxa_demangle(mangled.c_str(), demangled, &num_demangled, &status);\
			if (status == 0) Log.warn() << "demangled: " << String(out);\
			if (num_demangled > 0) free(demangled);\
			demangled = 0;\
			Log.warn() << std::endl;\
		}\
		free(bt);\
	}\

# else
# define BALL_PRINT_BACKTRACE()
# endif

#else

# define BALL_PRECONDITION_EXCEPTION(condition, message)
# define BALL_POSTCONDITION_EXCEPTION(condition, message)
# define BALL_PRINT_BACKTRACE()

#endif // BALL_DEBUG

#endif // BALL_COMMON_DEBUG_H
