// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: stageSettings.h,v 1.5.2.1 2004/05/18 15:04:27 amoll Exp $
//

#ifndef BALL_VIEW_DIALOGS_STAGESETTINGS_H
#define BALL_VIEW_DIALOGS_STAGESETTINGS_H

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

		/** Dialog for the Stage setup.
		 		Following options can be set:
				- background color of the Scene
				- if a coordinate system is shown in the Scene
				- mouse sensitivity in the Scene
				- mouse wheel sensitivity in the Scene
				\ingroup ViewDialogs
		*/
		class BALL_EXPORT StageSettings 
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

			/// 
			void setDefaultValues()
				throw();

			public slots:

			/// Show a QColorDialog to select a new background color for the Scene
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
