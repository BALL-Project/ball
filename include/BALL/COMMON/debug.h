// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: debug.h,v 1.6 2003/06/12 13:44:09 anker Exp $
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

#ifdef BALL_DEBUG

#	define BALL_PRECONDITION_EXCEPTION(condition, message)\
	if (!(condition))\
	{\
		Exception::Precondition e(__FILE__, __LINE__);\
		if (message != "")\
		{\
			e.setMessage(std::string(e.getMessage()) + std::string(message), #condition);\
		}\
		throw e;\
	}\

#	define BALL_POSTCONDITION_EXCEPTION(condition, message)\
	if (!(condition))\
	{\
		Exception::PostCondition e(__FILE__, __LINE__, #condition);\
		if (message != "")\
		{\
			e.setMessage(std::string(e.getMessage()) + std::string(message));\
		}\
		throw e;\
	}\

#else

# define BALL_PRECONDITION_EXCEPTION(condition, message)
# define BALL_POSTCONDITION_EXCEPTION(condition, message)

#endif // BALL_DEBUG

#endif // BALL_COMMON_DEBUG_H
