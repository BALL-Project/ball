// $Id: forEach.h,v 1.1 1999/08/26 07:53:13 oliver Exp $

#ifndef BALL_DATATYPE_FOREACH_H
#define BALL_DATATYPE_FOREACH_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

/// 
#define BALL_FOREACH(container, element_iterator) \
for (element_iterator = container.begin(); \
     !element_iterator.isEnd(); \
     ++element_iterator)

#endif // BALL_DATATYPE_FOREACH_H
