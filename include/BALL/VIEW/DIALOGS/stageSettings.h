// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_STAGESETTINGS_H
#define BALL_VIEW_DIALOGS_STAGESETTINGS_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
#endif

#ifndef BALL_MATH_VECTOR3
# include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_VIEW_RENDERING_RENDERSETUP_H
# include <BALL/VIEW/RENDERING/renderSetup.h>
#endif

#ifndef BALL_VIEW_RENDERING_RENDERERS_RENDERER_H
# include <BALL/VIEW/RENDERING/RENDERERS/renderer.h>
#endif

#include <BALL/VIEW/UIC/ui_stageSettings.h>

#include <QtGui/QWidget>

class QSplashScreen;

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
			void updateFromStage();

			/// Apply the new values to the stage
			void apply();

			///
			void getGLSettings();

			/** Return the screen number of the control screen in our stereo setup.
			 */
			int getControlScreenNumber() const;

			/** Return the screen number of the right eye in our stereo setup.
			 */
			int getLeftEyeScreenNumber() const;

			/** Return the screen number of the right eye in our stereo setup.
			 */
			int getRightEyeScreenNumber() const;

			/** Return the chosen stereo mode
			 */
			Renderer::StereoMode getStereoMode() const;

			/** Return the selected renderer for the control screen
			 */
			RenderSetup::RendererType getControlScreenRendererType() const;

			/** Return the selected renderer for the stereo screens
			 */
			RenderSetup::RendererType getStereoScreensRendererType() const;

			/** Return the screen geometry for the left eye
       */
			QRect getLeftEyeGeometry() const;

			/** Return the screen geometry for the right eye
       */
			QRect getRightEyeGeometry() const;

			virtual bool setValueAllowed(QObject* widget);

			public slots:

			///
			void computeDefaultPressed();

			/// Show a QFileDialog to select a new environment texture map for the Scene
			void loadEnvironmentMapPressed();
			
			///
			void environmentMapChanged(bool active);
			
			///
			void fogBoxChanged(bool active);

			///
			void screenCountChanged(int number);

			///
			void stereoScreenChanged(int new_screen);

			///
			void identifyDisplays();

			///
			void stereoModeChanged();

			private slots:
				
			///
			void eyeDistanceChanged();

			///
			void focalDistanceChanged();

			///
			void projectionTransformationChanged();
			
			///
			void downsamplingSliderChanged();

			///
			void killIdentificationLabels_();

			private:
			
			///
			Vector3 getTextureUpDirection_()
				throw(Exception::InvalidFormat);
			
			///
			void setTextureUpDirection_(const Vector3& tud);

			///
			float getUser2ScreenDistance_()
				throw(Exception::InvalidFormat);
			
			///
			void setUser2ScreenDistance_(const float& s2u);

			///
			float getUserEyeLevel_()
				throw(Exception::InvalidFormat);
			
			///
			void setUserEyeLevel_(const float& s2u);
			
			///
			float getUserEyeDistance_()
				throw(Exception::InvalidFormat);
			
			///
			void setUserEyeDistance_(const float& s2u);
			///
			void setDefaultValues_();

			//_ apply values to a Stage
			void saveSettingsToStage_();

			Scene* scene_;

			VIEW::Stage* stage_;

			std::vector<QSplashScreen*> identification_labels_;
		};

} }

#endif
