// $Id: universe.h,v 1.1 1999/08/26 07:53:19 oliver Exp $

#ifndef BALL_KERNEL_UNIVERSE_H
#define BALL_KERNEL_UNIVERSE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_OBJECT_H
#	include <BALL/CONCEPT/object.h>
#endif

#ifdef BALL_SUPPORT_OBJECT_MANAGER
	namespace BALL {
	  typedef ObjectManager Universe;
	}
#endif

#endif // BALL_KERNEL_UNIVERSE_H
