// $Id: stringHashMap.h,v 1.1 1999/08/26 07:53:13 oliver Exp $

#ifndef BALL_DATATYPE_STRINGHASHMAP_H
#define BALL_DATATYPE_STRINGHASHMAP_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_HASHMAP_H
#	include <BALL/DATATYPE/hashMap.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
#	include <BALL/DATATYPE/string.h>
#endif

namespace BALL 
{

	template <class Value>
	class StringHashMap
		: public HashMap<String, Value>
	{
		public:

		StringHashMap()
			:	HashMap<String, Value>()
		{
		}

		StringHashMap(const StringHashMap& string_hash_map, bool deep = true)
			:	HashMap<String, Value> (string_hash_map, deep)
		{
		}

	};

} // namespace BALL

#endif // BALL_DATATYPE_STRINGHASHMAP_H
