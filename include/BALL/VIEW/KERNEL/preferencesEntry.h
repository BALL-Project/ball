// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: preferencesEntry.h,v 1.9 2005/02/15 12:36:08 amoll Exp $
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
class QWidgetStack;

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
			typedef List<std::pair<QWidget*, String> > EntriesList;
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
			virtual void showEntry(Position nr);
			
			///
			virtual void showEntry(QWidget* widget);

			///
			virtual Position currentEntry() const;

			///
			void setINIFileSectionName(const String& name) { inifile_section_name_ = name;}

			///
			const String& getINIFileSectionName() const { return inifile_section_name_;}

			///
			void insertEntry(QWidget* widget, const String& name);

			///
			EntriesList& getEntries() { return entries_;}

			///
			void setWidgetStack(QWidgetStack* stack);

			protected:

			//_
			void registerObject_(QWidget* widget);

			//_
			void setLabelColor_(QLabel* label, const ColorRGBA& color);
			
			//_
			ColorRGBA getLabelColor_(QLabel* label) const;
			
			//_
      bool fetchPreference_(const INIFile& inifile, const String& entry, ColorRGBA& color)
				        throw();

			String 							inifile_section_name_;
			HashSet<QWidget*> 	preferences_objects_;
			EntriesList 				entries_;
			QWidgetStack*				widget_stack_;
		};
  
	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_PREFERENCES_ENTRY
