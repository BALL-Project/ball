// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: stageSettings.h,v 1.2 2003/08/26 08:04:59 oliver Exp $
//

#ifndef BALL_VIEW_GUI_DIALOGS_STAGESETTINGS_H
#define BALL_VIEW_GUI_DIALOGS_STAGESETTINGS_H

#include <BALL/VIEW/UIC/stageSettingsData.h>

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

namespace BALL
{
	namespace VIEW
	{
		class Stage;
		class Scene;

		/** Dialog for setting the Stage.
		*/
		class StageSettings 
			: public StageSettingsData
		{ 
			Q_OBJECT

			public:

			/// Constructor
			StageSettings( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );

			/// Destructor
			~StageSettings() {}

			/// Get the values for Stageing from the stage
			void updateFromStage()
				throw();

			/// Apply the new values to the stage
			void apply()
				throw();

			public slots:

			///
			void colorPressed();

			private:

			//_ apply values to a Stage
			void saveSettingsToStage_()
				throw();

			Scene* scene_;

			Stage* stage_;
		};

} }

#endif
