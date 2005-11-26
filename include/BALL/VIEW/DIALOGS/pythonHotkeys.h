// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pythonHotkeys.h,v 1.3.6.7 2005/11/26 17:31:07 amoll Exp $
//

#ifndef BALL_VIEW_DIALOGS_PYTHONHOTKEYS_H
#define BALL_VIEW_DIALOGS_PYTHONHOTKEYS_H

#include <BALL/VIEW/UIC/pythonHotkeysData.h>

#ifndef BALL_VIEW_WIDGETS_PYWIDGET_H
 #include <BALL/VIEW/WIDGETS/pyWidget.h>
#endif
			
#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
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


		/** Dialog for setting up the Hotkeys for Python commands.
		 		It is inserted to the Preferences.
				\ingroup ViewDialogs
		*/
		class BALL_VIEW_EXPORT PythonHotkeys 
			: public PythonHotkeysData,
				public PreferencesEntry
		{ 
			Q_OBJECT

			public:

			/// Constructor
			PythonHotkeys( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );

			/// Destructor
			~PythonHotkeys() {}

			///
			const List<Hotkey> getContent() const
				throw();
			
			///
			void setContent(const List<Hotkey>& hotkeys);

			public slots:

			///
			virtual void rowSelected();

			protected:

			HotkeyTable*  table;
		};

} }

#endif
