// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: forEach.h,v 1.7 2003/03/03 14:17:39 anhi Exp $

#ifndef BALL_DATATYPE_FOREACH_H
#define BALL_DATATYPE_FOREACH_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif
    /**  \addtogroup  Miscellaneous
     *  @{
     */
/**	Container Iteration Macro.
		Use this macro for convenient iteration over BALL container 
		classes. \par
		<b>Example:</b> \par
\begin{verbatim}
		HashMap<int,int> hm;
		HashMap<int,int>::Iterator it;
		BALL_FOREACH(p,it)
		{
			cout << it->first << endl;
		}
\end{verbatim}
		<b>Definition:</b> BALL/DATATYPE/forEach.h
		@param container an instance of a BALL container class
		@param element_iterator an iterator of the container class
*/
  /** @} */
#define BALL_FOREACH(container, element_iterator) \
for (element_iterator = container.begin(); \
     !element_iterator.isEnd(); \
     ++element_iterator)

#endif // BALL_DATATYPE_FOREACH_H
