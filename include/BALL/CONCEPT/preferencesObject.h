#ifndef BALL_CONCEPT_PREFERENCESOBJECT_H
#define BALL_CONCEPT_PREFERENCESOBJECT_H

#ifndef BALL_DATATYPE_STRING_H
# include <BALL/DATATYPE/string.h>
#endif

#include <QtCore/QObject>

namespace BALL
{
	/** Base class for objects that can store settings in an INIFile. 
			To support storing of a classes' state to an INIFile, derive 
			from this class and implement the getValue/setValue-methods.

			Note that a more sophisticated and simpler to use scheme for
			storing state that is accessible through GUI elements, e.g.,
			LineEdits, already exists in libVIEW. Use this class if you
			want/need to be independent of the view functionality, or if
			the elements supported by VIEW::PreferencesEntry do not suffice.

			Make sure to name all derived objects, by calling setObjectName()
			Otherwise (re-)storing the values wont work.
	*/
	class BALL_EXPORT PreferencesObject
		: public virtual QObject
	{
		public:

		///
		PreferencesObject() {};

		///
		virtual ~PreferencesObject() {};

		/// Overload this in derived classes!
		virtual bool getValue(String&) const = 0;

		/// Overload this in derived classes!
		virtual bool setValue(const String&) = 0;
	};
}

#endif // BALL_CONCEPT_PREFERENCESOBJECT_H
