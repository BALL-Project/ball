// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: stageSettings.h,v 1.5 2004/02/26 08:41:36 anhi Exp $
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
