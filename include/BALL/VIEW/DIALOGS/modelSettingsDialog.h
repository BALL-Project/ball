// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: modelSettingsDialog.h,v 1.10 2004/01/20 16:48:03 amoll Exp $
//

#ifndef BALL_VIEW_DIALOGS_MODELSETTINGSDIALOG_H
#define BALL_VIEW_DIALOGS_MODELSETTINGSDIALOG_H

#include <BALL/VIEW/UIC/modelSettingsDialogData.h>

class QSlider;
class QLabel;

namespace BALL
{
	class INIFile;
	class String;

	namespace VIEW
	{
		/** Dialog for the molecular model settings
				\ingroup ViewDialogs
		*/
		class ModelSettingsDialog 
			: public ModelSettingsDialogData
		{ 
			Q_OBJECT

			public:

			/// Constructor
			ModelSettingsDialog( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );

			/// Destructor
			~ModelSettingsDialog() {}

			///
			void writePreferences(INIFile& file)
				throw();

			///
			void fetchPreferences(const INIFile& file)
				throw();

			///
			void setDefaults(bool all = true)
				throw();
			
			/// Called when defaults is pressed in Preferences, calls setDefaults
			virtual void setDefaultValues()
				throw();

			///
			float getStickStickRadius() const
				throw() {return getFloatValue_(stick_radius_slider);}

			///
			float getBallAndStickStickRadius() const
				throw() {return getFloatValue_(ball_stick_cylinder_radius_slider);}

			///
			float getBallRadius() const
				throw() {return getFloatValue_(ball_stick_sphere_radius_slider);}

			///
			float getVDWRadiusFactor() const
				throw() {return getFloatValue_(vdw_radius_factor_slider);}

			///
			float getSurfaceProbeRadius() const
				throw() {return getFloatValue_(surface_probe_radius_slider);}

			///
			float getTubeRadius() const
				throw() {return getFloatValue_(tube_radius_slider);}

			///
			float getCartoonTubeRadius() const
				throw() {return getFloatValue_(cartoon_tube_radius_slider);}

			///
			float getCartoonHelixRadius() const
				throw() {return getFloatValue_(cartoon_helix_radius_slider);}

			///
			float getCartoonArrowWidth() const
				throw() {return getFloatValue_(cartoon_arrow_width_slider);}

			///
			float getCartoonArrowHeight() const
				throw() {return getFloatValue_(cartoon_arrow_height_slider);}

			///
			float getHBondsRadius() const
				throw() {return getFloatValue_(hbonds_radius_slider);}

			///
			void setStickStickRadius(float value) 
				throw() { setValue_(stick_radius_slider,value);}

			///
			void setBallAndStickStickRadius(float value) 
				throw() { setValue_(ball_stick_cylinder_radius_slider,value);}

			///
			void setBallRadius(float value) 
				throw() { setValue_(ball_stick_sphere_radius_slider,value);}

			///
			void setVDWRadiusFactor(float value) 
				throw() { setValue_(vdw_radius_factor_slider,value);}

			///
			void setSurfaceProbeRadius(float value) 
				throw() { setValue_(surface_probe_radius_slider,value);}

			///
			void setTubeRadius(float value) 
				throw() { setValue_(tube_radius_slider,value);}

			///
			void setCartoonTubeRadius(float value) 
				throw() { setValue_(cartoon_tube_radius_slider,value);}

			///
			void setCartoonHelixRadius(float value) 
				throw() { setValue_(cartoon_helix_radius_slider,value);}

			///
			void setCartoonArrowWidth(float value) 
				throw() { setValue_(cartoon_arrow_height_slider,value);}

			///
			void setCartoonArrowHeight(float value) 
				throw() { setValue_(cartoon_arrow_height_slider,value);}

			///
			void setHBondRadius(float value) 
				throw() { setValue_(hbonds_radius_slider,value);}

			public slots:
			
			///
			void showPage(int nr);

			protected slots:
			void stickRadiusChanged() {setLabelText_(stick_radius_label, stick_radius_slider);}
			void VDWfactorChanged() {setLabelText_(vdw_radius_factor_label, vdw_radius_factor_slider);}
			void ballStickSphereRadiusChanged() {setLabelText_(ball_stick_sphere_radius_label, ball_stick_sphere_radius_slider);}
			void ballStickCylinderRadiusChanged() {setLabelText_(ball_stick_cylinder_radius_label, ball_stick_cylinder_radius_slider);}
			void probeRadiusChanged() {setLabelText_(surface_probe_radius_label, surface_probe_radius_slider);}
			void tubeRadiusChanged() {setLabelText_(tube_radius_label, tube_radius_slider);}
			void cartoonTubeRadiusChanged(){setLabelText_(cartoon_tube_radius_label, cartoon_tube_radius_slider);}
			void cartoonHelixRadiusChanged(){setLabelText_(cartoon_helix_radius_label, cartoon_helix_radius_slider);}
			void cartoonArrowWidthChanged(){setLabelText_(cartoon_arrow_width_label, cartoon_arrow_width_slider);}
			void cartoonArrowHeightChanged(){setLabelText_(cartoon_arrow_height_label, cartoon_arrow_height_slider);}
			void hbondsRadiusChanged(){setLabelText_(hbonds_radius_label, hbonds_radius_slider);}

			protected:
			
			float getFloatValue_(const QSlider* const& le) const
				throw();

			void setValue_(QSlider* le, float value)
				throw();

			void setLabelText_(QLabel* label, const QSlider* const from)
				throw();

			void fetchPreference_(const INIFile& inifile, const String& entry, QSlider& slider)
				throw();

			void writePreference_(INIFile& inifile, const String& entry, const QSlider& slider) const
				throw();
		};

} }

#endif
