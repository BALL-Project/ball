// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_LIGHTSETTINGS_H
#define BALL_VIEW_DIALOGS_LIGHTSETTINGS_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_CONCEPT_EMBEDDABLE_H
#	include <BALL/CONCEPT/embeddable.h>
#endif

#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
#endif

#ifndef BALL_VIEW_KERNEL_STAGE_H
# include <BALL/VIEW/KERNEL/stage.h>
#endif

#include <BALL/VIEW/UIC/ui_lightSettings.h>

namespace BALL
{
	namespace VIEW
	{
		class Scene;

		/** Dialog for setting the lighting, which is added to the Preferences.
		 		This dialog allows the control of the lighting of a scene.
				Lights can be added, removed and adjusted. Also a standard value
				to reset the lighting can be stored.
				\ingroup ViewDialogs
		*/
		class BALL_VIEW_EXPORT LightSettings 
			: public QWidget,
				public Ui_LightSettingsData,
				public PreferencesEntry,
				public Embeddable
		{ 
			Q_OBJECT

			public:

			BALL_EMBEDDABLE(LightSettings,Embeddable)

			/// Constructor
			LightSettings( QWidget* parent = 0, const char* name = "LightSettings", 
										Qt::WindowFlags fl = 0 );

			/// Destructor
			~LightSettings() {}

			/// Update the display of all fields
			void update();

			/// Get the values for lighting from the stage
			void updateFromStage();

			/// Apply the new values to the stage
			void apply();

			/// Called when defaults is pressed in Preferences, calls setDefaults
			virtual void restoreDefaultValues(bool /*all*/ = false);

			///
			void restoreValues(bool all);
			
			public slots:

			/// Slot for the AddLight button
	    virtual void addLightPressed();

			/// Slot for the EditColor button, opens a color dialog
  	  virtual void colorPressed();
   	 	
			/// Resets the lighting
			virtual void defaultsPressed();
   		
			/// Slot for selection in the listbox with lights
			virtual void lightSelected(QListWidgetItem* current_item, QListWidgetItem* previous_item);
   		
			/// Slot for the RemoveLight button
			virtual void removeLightPressed();

			/// Slot for type selection changed
   	 	virtual void typeSelected();

			/// Slot for changes of the intensity slider
			virtual void intensityChanged();
			
			/// Slot for changes of the intensity max factor 
			virtual void intensityMaxChanged(const QString& text);

			///
			virtual void positionTypeChanged();
			
			///	
			virtual void updateDirectlyBoxChanged();

			protected:

			void setPosition_(const Vector3& v);
			void setDirection_(const Vector3& v);
			void setAttenuation_(const Vector3& a);
			Vector3 getDirection_() throw(Exception::InvalidFormat);
			Vector3 getPosition_() throw(Exception::InvalidFormat);
			Vector3 getAttenuation_() throw(Exception::InvalidFormat);
			void setControlsEnabled_(bool state);
			Index getCurrentLightNumber_() const;

			void typeSelected_(Position type);
		
			//_ apply values to a light
			void saveSettingsToLight_();

			//_ show the values of a light, after selecting an other one
			void getValues_(Index light = -1);

			//_ empty all fields
			void clearFields_();

			Stage* stage_;

			//__ temporary copy of all lights
			vector<LightSource> lights_;

			//__ default lights
			vector<LightSource> default_lights_;
			
			bool ignore_;
			Index current_light_;

			Scene* scene_;
		};

	}
}

#endif
