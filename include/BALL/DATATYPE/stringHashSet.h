// $Id: stringHashSet.h,v 1.2 2000/01/10 15:50:59 oliver Exp $

#ifndef BALL_DATATYPE_STRINGHASHSET_H
#define BALL_DATATYPE_STRINGHASHSET_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_HASHSET_H
#	include <BALL/DATATYPE/hashSet.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
#	include <BALL/DATATYPE/string.h>
#endif

namespace BALL 
{

	template <class ValueType>
	class StringHashSet
		: public HashSet<String, ValueType>
	{
		public:

		StringHashSet()
			:	HashSet<String, ValueType>()
		{
		}

		StringHashSet(const StringHashSet& string_hash_map, bool deep = true)
			:	HashSet<String, ValueType> (string_hash_map, deep)
		{
		}

	};

} // namespace BALL

#endif // BALL_DATATYPE_STRINGHASHSET_H
