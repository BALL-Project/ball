// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#ifndef BALL_VIEW_DIALOGS_PYTHONSETTINGS_H
#define BALL_VIEW_DIALOGS_PYTHONSETTINGS_H

#include <BALL/VIEW/UIC/pythonSettingsData.h>

#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
#endif

namespace BALL
{
	namespace VIEW
	{
		/** Dialog for setting the Python preferences.
		 		Currently only a startup script can be selected, that will be called,
				when the application has loaded and a PyWidget instance is added.
				\ingroup ViewDialogs
		*/
		class BALL_EXPORT PythonSettings 
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

			public slots:

			/// Open a filedialog to select the startup script
			void fileSelected();
		};
} }

#endif
