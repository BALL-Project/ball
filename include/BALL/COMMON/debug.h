// $Id: debug.h,v 1.1 1999/08/26 07:53:13 oliver Exp $

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
  if (!(a)) {\
    cerr << "Failed: " << #a << "/" << #b << endl << "(" << __FILE__ << ":" << __LINE__ << ")" << endl;\
    throw Exception::GeneralException(__FILE__, __LINE__);\
  }\

#define BALL_NO_CONDITION true

#define BALL_PRECOND(a)
 
#endif // BALL_COMMON_DEBUG_H
