// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: preferencesEntry.h,v 1.5 2004/09/29 21:14:02 amoll Exp $
//

#ifndef BALL_VIEW_KERNEL_PREFERENCES_ENTRY
#define BALL_VIEW_KERNEL_PREFERENCES_ENTRY

#ifndef BALL_FORMAT_INIFILE_H
# include <BALL/FORMAT/INIFile.h>
#endif

#ifndef BALL_DATATYPE_HASHSET_H
# include <BALL/DATATYPE/hashSet.h>
#endif

class QWidget;
class QLabel;

namespace BALL
{
	class INIFile;

	namespace VIEW
	{
		class ColorRGBA;

		/**
		 */
		class BALL_EXPORT PreferencesEntry
		{
			friend class PreferencesObject;

			public:

			///
			PreferencesEntry();

			///
			virtual ~PreferencesEntry();

			///
			virtual void writePreferenceEntries(INIFile& inifile);

			///
			virtual void readPreferenceEntries(const INIFile& inifile);

			///
			virtual void setDefaultValues(bool /*all*/ = false) {}

			///
			virtual void showPage(Position /*nr*/) {};

			///
			virtual Position currentPage() const { return 0;}

			///
			void setINIFileSectionName(const String& name) { inifile_section_name_ = name;}

			protected:

			//_
			void registerObject_(QWidget* widget);

			//_
			void setLabelColor_(QLabel* label, const ColorRGBA& color);
			
			//_
			ColorRGBA getLabelColor_(QLabel* label) const;
			
			//_
			ColorRGBA chooseColor_(QLabel* label);

			//_
      bool fetchPreference_(const INIFile& inifile, const String& entry, ColorRGBA& color)
				        throw();

			String 							inifile_section_name_;
			HashSet<QWidget*> 	preferences_objects_;
		};
  
	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_PREFERENCES_ENTRY
