// $Id: stringMap.h,v 1.1 1999/08/26 07:53:13 oliver Exp $

#ifndef BALL_DATATYPE_STRINGMAP_H
#define BALL_DATATYPE_STRINGMAP_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#include <map>

namespace BALL {

	template <class ValueType>
	class TStringMap
		: public map<String, ValueType>
	{
		public:

			// --- CONSTRUCTORS

			TStringMap(void);

			TStringMap(const TStringMap& map, bool deep = true);

			bool has(const String& key) const
			{
				return (find(key) != end());
			}

			pair<iterator, bool>	insert(const String& key, const ValueType& value)
			{
				return map<String, ValueType>::insert(map<String, ValueType>::value_type(key, value));
			}

			// --- DESTRUCTORS

			virtual ~TStringMap(void);

		protected:


		private:
	};


} // namespace BALL

#endif // BALL_DATATYPE_STRINGMAP_H
