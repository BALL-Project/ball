// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_DATATYPE_FOREACH_H
#define BALL_DATATYPE_FOREACH_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif
/**	Container Iteration Macro.
		Use this macro for convenient iteration over BALL container 
		classes. \par
		<b>Example:</b> \par
\verbatim
		HashMap<int,int> hm;
		HashMap<int,int>::Iterator it;
		BALL_FOREACH(p,it)
		{
			cout << it->first << endl;
		}
\endverbatim
		
		@param container an instance of a BALL container class
		@param element_iterator an iterator of the container class

    \ingroup  DatatypeMiscellaneous
*/
#define BALL_FOREACH(container, element_iterator) \
for (element_iterator = container.begin(); \
     !element_iterator.isEnd(); \
     ++element_iterator)

#endif // BALL_DATATYPE_FOREACH_H
