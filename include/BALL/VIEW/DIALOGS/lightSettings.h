// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: lightSettings.h,v 1.3 2003/09/04 22:17:45 amoll Exp $
//

#ifndef BALL_VIEW_DIALOGS_LIGHTSETTINGS_H
#define BALL_VIEW_DIALOGS_LIGHTSETTINGS_H

#include <BALL/VIEW/UIC/lightSettingsData.h>

#ifndef BALL_VIEW_KERNEL_STAGE_H
# include <BALL/VIEW/KERNEL/stage.h>
#endif

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

namespace BALL
{
	namespace VIEW
	{
		class Stage;
		class Scene;

		/** Dialog for setting the lighting.
		 		This dialog allows the control of the lighting of a scene.
				Lights can be added, removed and adjusted. Also a standard value
				to reset the lighting can be stored.
				\ingroup ViewDialogs
		*/
		class LightSettings 
			: public LightSettingsData
		{ 
			Q_OBJECT

			public:

			/// Constructor
			LightSettings( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );

			/// Destructor
			~LightSettings() {}

			/// Update the display of all fields
			void update()
				throw();

			/// Get the values for lighting from the stage
			void updateFromStage()
				throw();

			/// Apply the new values to the stage
			void apply()
				throw();

			/// Store the default values of lighting for later usage
			void getDefaultLights()
				throw();

			public slots:

			/// Slot for the AddLight button
	    virtual void addLightPressed();

			/// Slot for the EditColor button, opens a color dialog
  	  virtual void colorPressed();
   	 	
			/// Resets the lighting
			virtual void defaultsPressed();
   		
			/// Slot for selection in the listbox with lights
			virtual void lightSelected();
   		
			/// Slot for the RemoveLight button
			virtual void removeLightPressed();

			/// Slot for type selection changed
   	 	virtual void typeSelected();

			/// Slot for changes of the intensity slider
			virtual void intensityChanged();

			private:

			//_ apply values to a light
			void saveSettingsToLight_()
				throw();

			//_ show the values of a light, after selecting an other one
			void getValues_()
				throw();

			//_ empty all fields
			void clearFields_()
				throw();

			Index current_light_;
			
			Scene* scene_;

			Stage* stage_;

			//__ temporary copy of all lights
			vector<LightSource> lights_;

			//__ default lights
			vector<LightSource> default_lights_;
		};

	}
}

#endif
