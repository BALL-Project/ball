// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: preferencesEntry.h,v 1.9.2.4 2005/10/01 14:20:20 amoll Exp $
//

#ifndef BALL_VIEW_KERNEL_PREFERENCES_ENTRY
#define BALL_VIEW_KERNEL_PREFERENCES_ENTRY

#ifndef BALL_FORMAT_INIFILE_H
# include <BALL/FORMAT/INIFile.h>
#endif

#ifndef BALL_DATATYPE_HASHSET_H
# include <BALL/DATATYPE/hashSet.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLORRGBA_H
# include <BALL/VIEW/DATATYPE/colorRGBA.h>
#endif

class QWidget;
class QLabel;
class QWidgetStack;

namespace BALL
{
	class INIFile;

	namespace VIEW
	{
		/** Base class for a dialogs or a Preferences page.
		 		It allows to register all individual GUI elements in a widget
				to support reading and writing of its settings to a INIFile.
				Also restoring of the default value and the last applied value
				before cancel was pressed is supported.
				Currently the following QT objects are supported:
				- QLineEdit
				- QLabel
				- QSlider
				- QCheckBox
				- QCombobox
				- QButtonGroup
				To support more sophisticated items, e.g. color tables, a base class
				is provided to derive from (see ExtendedPreferencesObject).
				\\
				Furthermore this class allows to use QWidgetStack items, like e.g.
				in the Preferences, where all individual child widgets are than
				also entries in a QListView. This is than also use to support
				e.g. applying default values for the currently shown page in the
				QWidgetStack.\\
				\\
				How to use this class:\\
				1. Derive from it.\\
				2. Modify the constructor of the derived class:
 				3. Make sure that every GUI element is set to its default value.\\
				4. Call registerObject_() for every GUI element to be restored.\\
				5. Call setINIFileSectionName(String) for the section in the INIFile, where
				   the settins are to be stored.
				6. If the dialog is a page in a QWidgetStack (like in the Preferences dialog) 
					 call insertEntry(this, "Name to appear in the listview").\\
				7. If the dialog itself has a QWidgetStack call setWidgetStack() for it and 
					 set the names of the QWidgetStack pages accordingly.
					 But: setWidgetStack() has to be called after all registerObject_() calls!\\
				\\
				@see QModelSettings for an example
		 */
		class BALL_VIEW_EXPORT PreferencesEntry
		{
			friend class PreferencesObject;

			public:

			/** Base class for more sophisticated GUI elements in a dialog,
					to support e.g. reading and (re)storing of its state.
					Make sure to name all derived objects, so that (re-)storing the
					values from the INIFile works!
					@see QColorTable
			*/
			class BALL_VIEW_EXPORT ExtendedPreferencesObject
			{
				public:

				///
				ExtendedPreferencesObject() {};
				
				///
				virtual ~ExtendedPreferencesObject() {};

				/// Overload this in derived classes!
				virtual bool getValue(String&) const { return true;}

				/// Overload this in derived classes!
				virtual bool setValue(const String&) { return true;}
			};
			
			///
			typedef List<std::pair<QWidget*, String> > StackPages;
			
			/// 
			typedef HashMap<const QWidget*, String>    ValueMap;
			
			/// Typedef for all QWidgets in the individual WidgetStack items
			typedef vector<vector<QWidget*> > StackEntries;

			///
			PreferencesEntry();

			///
			virtual ~PreferencesEntry();

			/// Store the settings of all registered objects.
			virtual void writePreferenceEntries(INIFile& inifile);

			/// Restore the settings of all registered objects.
			virtual void readPreferenceEntries(const INIFile& inifile);

			/// Set the name for the section in the INIFile
			void setINIFileSectionName(const String& name) { inifile_section_name_ = name;}

			/// Get the name for the section in the INIFile
			const String& getINIFileSectionName() const { return inifile_section_name_;}
	
			/** If the derived class is to be shown in a QWidgetStack, 
			    call this method to set the name for its entry.
			*/
			void setWidgetStackName(const String& name);
			
			/// Return all pages, that are to be shown in a parent QWidgetStack.
			StackPages& getStackPages() { return stack_pages_;}
	
			/** Call this method in the constructor of the derived class
			    has its own QWidgetStack.
			*/
			void setWidgetStack(QWidgetStack* stack);

			/// Show the specified page in the QWidgetStack.
			virtual void showStackPage(Position nr);
			
			/// Show the specified page in the QWidgetStack.
			virtual void showStackPage(QWidget* widget);

			/// Get the currently shown page in the QWidgetStack.
			virtual Position currentStackPage() const;

			/// Set all registered objects to their default values.
			virtual void restoreDefaultValues(bool all = false);

			///
			virtual void storeValues();

			///
			virtual void restoreValues(bool all = false);

			protected:

			//_ Call this method in the constructor of the derived class for all GUI elements.
			void registerObject_(QWidget* widget);
			
			///
			bool getValue_(const QWidget* widget, String& value);

			///
			bool setValue_(QWidget* widget, const String& value);

			void registerWidgetForHelpSystem_(const QWidget* widget, const String& url);

			void storeStackEntries_();

			void restoreValues_(bool all, const ValueMap& map);

			void insertEntry_(QWidget*, const String& name);

			ColorRGBA getLabelColor_(const QLabel* label) const;

			void setLabelColor_(QLabel* label, const ColorRGBA& color);

			// name for the section in the INIFile
			String 							inifile_section_name_;
			
			// all registered QUI objects
			HashSet<QWidget*> 	registered_objects_;
			
			// stored default and last values for each registered object
			ValueMap 						default_values_, last_values_;
			
			// if the derived class has its own QWidgetStack, it is stored here
			QWidgetStack*				widget_stack_;

			StackPages 					stack_pages_;

			// registered objects per QWidgetStack page
			StackEntries 				stack_entries_;
		};
  
	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_PREFERENCES_ENTRY
