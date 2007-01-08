// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#ifndef BALL_VIEW_DIALOGS_PYTHONSETTINGS_H
#define BALL_VIEW_DIALOGS_PYTHONSETTINGS_H

#include <BALL/VIEW/UIC/pythonSettingsData.h>

#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
#endif

#ifndef BALL_VIEW_WIDGETS_PYWIDGET_H
 #include <BALL/VIEW/WIDGETS/pyWidget.h>
#endif

#include <qtable.h>

namespace BALL
{
	namespace VIEW
	{
		///
		class BALL_VIEW_EXPORT HotkeyTable
			:	public QTable,
				public PreferencesEntry::ExtendedPreferencesObject
		{
			Q_OBJECT

			public:

				///
				enum Modifier
				{
					NONE = 0,
					SHIFT
					// ALT // currently not working!
				};

				///
				HotkeyTable(QWidget* parent = 0, const char* name = 0)
					throw();
				
				///
				virtual bool getValue(String& value) const;

				///
				virtual bool setValue(const String& value);

				///
				List<Hotkey> getContent() const
					throw();

				///
				void setContent(const List<Hotkey>& hotkeys)
					throw();

				public slots:
				
				///
				virtual void addEmptyRow();
				
				///
				virtual void removeSelection();

				/** Append a hotkey
						F_key: 1-12 for the 12 F-keys
				*/
				virtual void appendHotkey(Modifier mod, Position F_key, const String& command);
				
			private:
				QStringList modifier_, keys_;
		};


		/** Dialog for setting the Python preferences.
		 		Currently only a startup script can be selected, that will be called,
				when the application has loaded and a PyWidget instance is added.
				\ingroup ViewDialogs
		*/
		class BALL_VIEW_EXPORT PythonSettings 
			: public PythonSettingsData,
				public PreferencesEntry
		{ 
			Q_OBJECT

			public:

			/// Constructor
			PythonSettings( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );

			/// Destructor
			~PythonSettings() {}

			/// Set the filename of the startup script
			void setFilename(const String& filename)
				throw();

			/// Get the filename of the startup script
			String getFilename() const
				throw();

			///
			const List<Hotkey> getContent() const
				throw();
			
			///
			void setContent(const List<Hotkey>& hotkeys);

			public slots:

			/// Open a filedialog to select the startup script
			void fileSelected();

			///
			void clearStartupScript();

			///
			virtual void rowSelected();

			protected:

			HotkeyTable*  table;
		};
} }

#endif
