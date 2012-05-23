// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_PYTHONSETTINGS_H
#define BALL_VIEW_DIALOGS_PYTHONSETTINGS_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
#endif

#ifndef BALL_VIEW_WIDGETS_PYWIDGET_H
# include <BALL/VIEW/WIDGETS/pyWidget.h>
#endif

#include <BALL/VIEW/UIC/ui_pythonSettings.h>

#include <QtWidgets/QWidget>
#include <QtGui/QFont>

namespace BALL
{
	namespace VIEW
	{
		/** Dialog for setting the Python preferences.
		 		Currently only a startup script can be selected, that will be called,
				when the application has loaded and a PyWidget instance is added.
				\ingroup ViewDialogs
		*/
		class BALL_VIEW_EXPORT PythonSettings 
			: public QWidget,
				public Ui_PythonSettingsData,
				public PreferencesEntry
		{ 
			Q_OBJECT

			public:

			/// Constructor
			PythonSettings(QWidget* parent = 0, const char* name = 0);

			/// Destructor
			~PythonSettings() {}

			///
			QFont getEditorFont() const { return font_;}

			/// Set the filename of the startup script
			void setFilename(const String& filename);

			/// Get the filename of the startup script
			String getFilename() const;

			///
			const std::list<Hotkey> getContent() const;
			
			///
			void setContent(const std::list<Hotkey>& hotkeys);

			///
			void writePreferenceEntries(INIFile& inifile);

			///
			void readPreferenceEntries(const INIFile& inifile);

			public slots:

			/// Open a filedialog to select the startup script
			void fileSelected();

			///
			void clearStartupScript();

			///
			virtual void rowSelected();

			///
			void selectFont();

			protected:

			QFont				font_;
		};
} }

#endif
