// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: stringHashSet.h,v 1.9 2003/03/14 11:48:56 sturm Exp $

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
			
	*/
	typedef HashSet<String> StringHashSet;
  /** @} */
} // namespace BALL

#endif // BALL_DATATYPE_STRINGHASHSET_H
