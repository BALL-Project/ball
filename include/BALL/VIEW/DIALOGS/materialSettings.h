// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DIALOGS_MATERIALSETTINGS_H
#define BALL_VIEW_DIALOGS_MATERIALSETTINGS_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
#endif

#include <BALL/VIEW/UIC/ui_materialSettings.h>

#include <QtGui/QWidget>
#include <QtGui/QLabel>

namespace BALL
{
	namespace VIEW
	{
		class Scene;
		class Representation;

		/** Dialog for setting the Material, which is added to the Preferences.
		 		This dialog allows the control of the Materials of a Scene.
				Materials can be added, removed and adjusted.
				\ingroup ViewDialogs
		*/
		class BALL_VIEW_EXPORT MaterialSettings 
			: public QWidget,
				public Ui_MaterialSettingsData,
				public PreferencesEntry
		{ 
			Q_OBJECT

			public:

			/// Constructor
			MaterialSettings( QWidget* parent = 0, const char* name = "MaterialSettings", Qt::WFlags fl = 0 );

			/// Destructor
			~MaterialSettings() {}

			/// Apply the new values to the stage
			void apply();

			/// set the current representation
			void setCurrentRepresentation(Representation* representation);

			/** Set the values according to the ones stored in the scene
			 *  TODO: currently, this only works for RTfact materials!
			 *  OpenGL and POVRay are not yet supported!
			 */
			void updateDefaultMaterialsFromStage();

			public slots:

			virtual void ambientFactorChanged();
			virtual void specularityFactorChanged();
			virtual void reflectivenessFactorChanged();
			virtual void shininessFactorChanged();
			virtual void transparencyFactorChanged();
			virtual void updateDirectlyBoxChanged();
			virtual void editAmbientColor();
			virtual void editSpecularityColor();
			virtual void editReflectivenessColor();
			private:

			void setLabel_(QLabel& label, float value);
			void setValues_(const QSlider& slider, QLabel& label, int divisor);
			void setQuadraticValues_(const QSlider& slider, QLabel& label, int divisor);

			Representation* current_representation_;
		};

	}
}

#endif
