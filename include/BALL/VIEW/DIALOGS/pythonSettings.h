// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#ifndef BALL_VIEW_DIALOGS_PYTHONSETTINGS_H
#define BALL_VIEW_DIALOGS_PYTHONSETTINGS_H

#include <BALL/VIEW/UIC/pythonSettingsData.h>

#ifndef BALL_DATATYPE_STRING_H
# include <BALL/DATATYPE/string.h>
#endif

namespace BALL
{
	namespace VIEW
	{
		/** Dialog for setting the Python preferences.
				\ingroup ViewDialogs
		*/
		class BALL_EXPORT PythonSettings 
			: public PythonSettingsData
		{ 
			Q_OBJECT

			public:

			/// Constructor
			PythonSettings( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );

			/// Destructor
			~PythonSettings() {}

			///
			void setFilename(const String& filename)
				throw();

			///
			String getFilename() const
				throw();

			public slots:

			///
			void fileSelected();
		};
} }

#endif
