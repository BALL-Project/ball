// $Id: forEach.h,v 1.2 1999/12/28 21:29:13 oliver Exp $

#ifndef BALL_DATATYPE_FOREACH_H
#define BALL_DATATYPE_FOREACH_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

/**	Container Iteration Macro.
		Use this macro for convenient iteration over BALL container 
		classes.\\
		{\bf Example:}\\	
\begin{verbatim}
		HashMap<int,int> hm;
		HashMap<int,int>::Iterator it;
		BALL_FOREACH(p,it)
		{
			cout << it->first << endl;
		}
\end{verbatim}
		{\bf Definition:} \URL{BALL/DATATYPE/forEach.h}
		@param container an instance of a BALL container class
		@param element_iterator an iterator of the container class
*/
#define BALL_FOREACH(container, element_iterator) \
for (element_iterator = container.begin(); \
     !element_iterator.isEnd(); \
     ++element_iterator)

#endif // BALL_DATATYPE_FOREACH_H
