// $Id: debug.h,v 1.2 1999/10/30 12:53:14 oliver Exp $

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

#define BALL_PRECONDITION(a,b)\
  if (!(a))\
	{\
    ::std::cerr << "Failed: " << #a << "/" << #b << ::std::endl << "(" << __FILE__ << ":" << __LINE__ << ")" << ::std::endl;\
    throw Exception::GeneralException(__FILE__, __LINE__);\
  }\

#define BALL_NO_CONDITION true

#define BALL_PRECOND(a)
 
#endif // BALL_COMMON_DEBUG_H
