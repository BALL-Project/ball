// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: stageSettings.h,v 1.15.16.1 2007/03/25 21:25:57 oliver Exp $
//

#ifndef BALL_VIEW_DIALOGS_STAGESETTINGS_H
#define BALL_VIEW_DIALOGS_STAGESETTINGS_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifdef BALL_COMPILER_MSVC
# include <BALL/VIEW/UIC/ui_stageSettings.h>
#else
# include <BALL/VIEW/UIC/stageSettingsData.h>
#endif


#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
#endif

#ifndef BALL_MATH_VECTOR3
#  include <BALL/MATHS/vector3.h>
#endif

#include <QtGui/QWidget>

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
		class BALL_VIEW_EXPORT StageSettings 
			: public QWidget,
				public Ui_StageSettingsData,
				public PreferencesEntry
		{ 
			Q_OBJECT

			public:

			/// Constructor
			StageSettings( QWidget* parent = 0, const char* name = "StageSettings", Qt::WFlags fl = 0 );

			/// Destructor
			~StageSettings() {}

			/// Get the values for Stageing from the stage
			void updateFromStage()
				throw();

			/// Apply the new values to the stage
			void apply()
				throw();

			///
			void getGLSettings()
				throw();

			public slots:

			/// Show a QColorDialog to select a new background color for the Scene
			void colorPressed();

			/// Show a QFileDialog to select a new environment texture map for the Scene
			void loadEnvironmentMapPressed();
			
			///
			void environmentMapChanged(bool active);

			///
			void cappingColorPressed();


			private slots:
				
			///
			void eyeDistanceChanged();

			///
			void focalDistanceChanged();

			///
			void projectionTransformationChanged();

			private:
			
			///
			Vector3 getTextureUpDirection_()
				throw(Exception::InvalidFormat);
			
			///
			void setTextureUpDirection_(const Vector3& tud);

			///
			void setDefaultValues_();

			//_ apply values to a Stage
			void saveSettingsToStage_()
				throw();

			Scene* scene_;

			VIEW::Stage* stage_;
		};

} }

#endif
