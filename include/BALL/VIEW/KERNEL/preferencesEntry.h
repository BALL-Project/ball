// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: preferencesEntry.h,v 1.1 2004/09/28 16:44:44 amoll Exp $
//

#ifndef BALL_VIEW_KERNEL_PREFERENCES_ENTRY
#define BALL_VIEW_KERNEL_PREFERENCES_ENTRY

#ifndef BALL_FORMAT_INIFILE_H
# include <BALL/FORMAT/INIFile.h>
#endif

class QWidget;

namespace BALL
{
	class INIFile;

	namespace VIEW
	{

		/**
		 */
		class BALL_EXPORT PreferencesEntry
		{
			friend class PreferencesObject;

			public:

			///
			PreferencesEntry()
				throw();

			///
			virtual ~PreferencesEntry()
				throw();

			///
			void writePreferences(INIFile& inifile);

			///
			void readPreferences(const INIFile& inifile);

			private:

			String 					inifile_section_name_;
			List<QWidget*> 	preferences_objects_;

		};
  
	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_PREFERENCES_ENTRY
