// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: stringHashSet.h,v 1.8 2003/03/03 14:17:41 anhi Exp $

#ifndef BALL_DATATYPE_STRINGHASHSET_H
#define BALL_DATATYPE_STRINGHASHSET_H

#ifndef BALL_DATATYPE_HASHSET_H
#	include <BALL/DATATYPE/hashSet.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
#	include <BALL/DATATYPE/string.h>
#endif

namespace BALL 
{
    /**  \addtogroup  GenericHash
     *  @{
     */
	/**	A string hash set.
			This is just a convenience class.
			 \par
			<b>Definition:</b>BALL/DATATYPE/stringHashSet.h
	*/
	typedef HashSet<String> StringHashSet;
  /** @} */
} // namespace BALL

#endif // BALL_DATATYPE_STRINGHASHSET_H
