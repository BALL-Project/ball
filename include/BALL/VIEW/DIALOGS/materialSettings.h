// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: materialSettings.h,v 1.5.4.2 2005/09/29 14:01:30 amoll Exp $
//

#ifndef BALL_VIEW_DIALOGS_MATERIALSETTINGS_H
#define BALL_VIEW_DIALOGS_MATERIALSETTINGS_H

#include <BALL/VIEW/UIC/materialSettingsData.h>

#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
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
		class BALL_VIEW_EXPORT MaterialSettings 
			: public MaterialSettingsData,
				public PreferencesEntry
		{ 
			Q_OBJECT

			public:

			/// Constructor
			MaterialSettings( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );

			/// Destructor
			~MaterialSettings() {}

			/// Apply the new values to the stage
			void apply()
				throw();

			public slots:

			virtual void ambientChanged();
			virtual void diffuseChanged();
			virtual void specularChanged();
			virtual void shininessChanged();

			private:

			void setValues_(const QSlider& slider, QLabel& label);
		};

	}
}

#endif
