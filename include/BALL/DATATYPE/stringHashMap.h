// $Id: stringHashMap.h,v 1.2 1999/09/18 19:08:04 oliver Exp $

#ifndef BALL_DATATYPE_STRINGHASHMAP_H
#define BALL_DATATYPE_STRINGHASHMAP_H

#ifndef BALL_DATATYPE_STRING_H
#	include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_DATATYPE_HASHMAP_H
#	include <BALL/DATATYPE/hashMap.h>
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
