// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: preferencesEntry.h,v 1.10.16.1 2007/03/25 21:26:02 oliver Exp $
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
class QObject;
class QStackedWidget;

namespace BALL
{
	class INIFile;

	namespace VIEW
	{
		/** Base class for a dialog or a Preferences page.
		 		It allows to register child widgets to
				support reading and writing their data into a INIFile.
				Also restoring of a default value and the last applied value
				is supported.
				Currently the following Qt widgets are supported:
				<ul>
				<li>QLineEdit
				<li>QSlider
				<li>QSpinBox
				<li>QCheckBox
				<li>QCombobox
				<li>QButtonGroup
				<li>QLabels without text (for storing colors)
				</ul>
				To support more sophisticated items, e.g. color tables, a base class
				is provided to derive from (see ExtendedPreferencesObject). This object
		    only needs to be a QObject with the PreferencesEntry as parent.
				\\
				Furthermore this class allows to use QStackedWidget items, like e.g.
				in the Preferences, where all individual child widgets are then
				also entries in a QListView.
				\\
				\\
				How to use this class:
				<ol>
				<li>Derive from it.
				<li>Ensure, that all child widgets have a unique name (see QWidget::setObjectName)
				<li>Modify the constructor of the derived class:
 				<li>Make sure that every GUI element is set to its default value.
				<li>Call registerWidgets_(), which automatically registeres all supported child widgets
				<li>Call setINIFileSectionName(String) 
				<li>If the dialog is a page in a QStackedWidget (like in the Preferences dialog) 
					 call insertEntry(this, "Name to appear in the listview").
				<li>If the dialog itself has a QStackedWidget call setWidgetStack() for it and 
					 set the names of the QStackedWidget pages accordingly.
				</ol>
				@see ModelSettings for an example
		 */
		class BALL_VIEW_EXPORT PreferencesEntry
		{
			public:

			/** Base class for the more sophisticated GUI elements in a dialog,
					to support reading and (re)storing their state.
					Make sure to name all derived objects, by calling setObjectName()
					Otherwise (re-)storing the values wont work.
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
				virtual bool getValue(String&) const = 0;

				/// Overload this in derived classes!
				virtual bool setValue(const String&) = 0;
			};

			///
			typedef std::list<std::pair<QWidget*, String> > StackPages;

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

			/** If the derived class is to be shown in a QStackedWidget, 
			    call this method to set the name for its entry.
			*/
			void setWidgetStackName(const String& name);

			/// Return all pages, that are to be shown in a parent QStackedWidget.
			StackPages& getStackPages() { return stack_pages_;}

			/** Call this method in the constructor of the derived class
			    has its own QStackedWidget.
			*/
			void setWidgetStack(QStackedWidget* stack);

			/// Show the specified page in the QStackedWidget.
			virtual void showStackPage(Position nr);

			/// Show the specified page in the QStackedWidget.
			virtual void showStackPage(QWidget* widget);

			/// Get the currently shown page in the QStackedWidget.
			virtual Position currentStackPage() const;

			/// Set all registered objects to their default values.
			virtual void restoreDefaultValues(bool all = false);

			/** Store the values for later restoration by the restoreValues method.
			 		Called e.g. by the Preferences dialog apply button.
			*/
			virtual void storeValues();

			/** Restore the values of the child widgets. 
			 		Called e.g. when a user presses a Cancel button.
			*/
			virtual void restoreValues(bool all = false);

			/** Internal state dump.
					Dump the current internal state of this mainControl to 
					the output ostream <b>s</b> with dumping depth <b>depth</b>.
					\param   s output stream where to output the internal state 
					\param   depth the dumping depth
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;

			/// Allows to prevent restoring of widgets
			virtual bool setValueAllowed(QObject* /*widget*/) { return true; }

			protected:

			/// 
			typedef HashMap<const QObject*, String> ValueMap;

			/** Register all supported child objects for the storing of their data.
			 		Must be called at the end of the child classes constructor.
			*/
			void registerWidgets_();

			/*_ Registration for objects, that are not automatically supported.
			 		Should no be needed, but if the need should arive, call it in the constructor of the derived class.
			*/
			void registerObject_(QObject* widget);

			/*_ Unregistration for objects, that are automatically registered.
			 		Should no be needed, but if the need should arive, call it in the constructor of the derived class
					after registerWidgets_()
			*/
			void unregisterObject_(QObject* widget);

			/** Register a widget for the internal help system
			 		@see HelpViewer
			*/
			void registerWidgetForHelpSystem_(const QWidget* widget, const String& url);

			//_ Helper function to restore the values of the current stack widget or all values
			void restoreValues_(bool all, const ValueMap& map);

			//_ Add a stack entry
			void insertStackEntry_(QWidget*, const String& name);

			//_ Check if the object's data can be transformed into a string
			bool isSupported_(QObject& widget);

			//_ Transform the object's data into a string
			bool getValue_(const QObject* widget, String& value);

			//_ Restore the widget's data from a string
			bool setValue_(QObject* widget, const String& value);

			// name for the section in the INIFile
			String inifile_section_name_;

			// all registered child objects
			HashSet<QObject*> registered_objects_;

			// stored default and last values for each registered object
			ValueMap default_values_, last_values_;

			// if the derived class has its own QStackedWidget, it is stored here
			QStackedWidget* widget_stack_;

			StackPages stack_pages_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_PREFERENCES_ENTRY
