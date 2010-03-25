// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: debug.h,v 1.10 2004/02/23 21:35:17 anhi Exp $
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

#else

# define BALL_PRECONDITION_EXCEPTION(condition, message)
# define BALL_POSTCONDITION_EXCEPTION(condition, message)

#endif // BALL_DEBUG

#endif // BALL_COMMON_DEBUG_H
