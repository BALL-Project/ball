// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: modelSettingsDialog.h,v 1.1 2003/11/13 17:47:18 amoll Exp $
//

#ifndef BALL_VIEW_DIALOGS_MODELSETTINGSDIALOG_H
#define BALL_VIEW_DIALOGS_MODELSETTINGSDIALOG_H

#include <BALL/VIEW/UIC/modelSettingsDialogData.h>

namespace BALL
{
	namespace VIEW
	{
		/** Dialog for the molecular model settings
				\ingroup ViewDialogs
		*/
		class ModelSettingsDialog 
			: public ModelSettingsDialogData
		{ 
			Q_OBJECT

			public:

			/// Constructor
			ModelSettingsDialog( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );

			/// Destructor
			~ModelSettingsDialog() {}

			/// Apply the new values to the stage
			void apply()
				throw();
		};

} }

#endif
