// $Id: hashSet.h,v 1.1 1999/08/26 07:53:13 oliver Exp $

#ifndef BALL_DATATYPE_NEWHASHSET_H
#define BALL_DATATYPE_NEWHASHSET_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_COMMON_HASH_H
#	include <BALL/COMMON/hash.h>
#endif

#include <BALL/DATATYPE/internal_hash_set.h>

namespace BALL {

	template 
		<typename Key, 
		 typename Hasher = HashFunction<Key>, 
	   typename EqualKey = equal_to<Key> >
	class HashSet
		:	public hash_set<Key, Hasher, EqualKey>
	{
		public:

		HashSet()
			: hash_set<Key, Hasher, EqualKey>()
		{
		}

		HashSet(const HashSet& set, bool /* deep = true */)
			: hash_set<Key, Hasher, EqualKey>()
		{
			(*this) = set;
		}

		bool has(const Key& key) const
		{
			return !(find(key) == end());
		}

		void destroy()
		{
			clear();
		}
	
	};


} // namespace BALL

#endif // BALL_DATATYPE_NEWHASHSET_H
