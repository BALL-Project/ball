// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: materialSettings.h,v 1.1 2004/06/19 14:30:26 amoll Exp $
//

#ifndef BALL_VIEW_DIALOGS_MATERIALSETTINGS_H
#define BALL_VIEW_DIALOGS_MATERIALSETTINGS_H

#include <BALL/VIEW/UIC/materialSettingsData.h>

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

namespace BALL
{
	namespace VIEW
	{
		class Scene;

		/** Dialog for setting the Material, which is added to the Preferences.
		 		This dialog allows the control of the Materials of a Scene.
				Materials can be added, removed and adjusted.
				\ingroup ViewDialogs
		*/
		class BALL_EXPORT MaterialSettings 
			: public MaterialSettingsData
		{ 
			Q_OBJECT

			public:

			/// Constructor
			MaterialSettings( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );

			/// Destructor
			~MaterialSettings() {}

			/// Update the display of all fields
			void update()
				throw();

			/// Get the values for Materialing from the stage
			void updateFromStage()
				throw();

			/// Apply the new values to the stage
			void apply()
				throw();

			/// Called when defaults is pressed in Preferences, calls setDefaults
			virtual void setDefaultValues()
				throw();

			public slots:

			virtual void redChanged();
			virtual void greenChanged();
			virtual void blueChanged();
			virtual void alphaChanged();
			virtual void shininessChanged();
			virtual void materialSelected(int button_id);

			private:

			void setValues_(const QSlider& slider, QLabel& label, Position pos);
			void setValues_();

			Scene* scene_;
			float material_values_[3][4];
			float shininess_;
		};

	}
}

#endif
