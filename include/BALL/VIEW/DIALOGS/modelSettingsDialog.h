// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: modelSettingsDialog.h,v 1.18 2004/09/16 14:36:58 amoll Exp $
//

#ifndef BALL_VIEW_DIALOGS_MODELSETTINGSDIALOG_H
#define BALL_VIEW_DIALOGS_MODELSETTINGSDIALOG_H

#ifndef BALL_VIEW_MODELS_MODELPROCESSOR_H
# include <BALL/VIEW/MODELS/modelProcessor.h>
#endif

#ifndef BALL_VIEW_KERNEL_COMMON_H
# include <BALL/VIEW/KERNEL/common.h>
#endif


#include <BALL/VIEW/UIC/modelSettingsDialogData.h>

#include <qcheckbox.h>

class QSlider;
class QLabel;

namespace BALL
{
	class INIFile;
	class String;

	namespace VIEW
	{
		/** Dialog for the molecular model settings, it is inserted into the Preferences.
				\ingroup ViewDialogs
		*/
		class BALL_EXPORT ModelSettingsDialog 
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
			void applySettingsTo(ModelProcessor& cp) const
				throw();
				
			///
			ModelProcessor* createModelProcessor(ModelType type) const
				throw(Exception::InvalidOption);

			///
			void getSettings(const ModelProcessor& mp)
				throw();

			///
			void showPage(ModelType type)
				throw();

			///
			float getStickStickRadius() const
				throw() {return getFloatValue_(stick_radius_slider);}

			///
			float getBallAndStickStickRadius() const
				throw() {return getFloatValue_(ball_stick_cylinder_radius_slider);}

			///
			bool ballAndStickDashedBondsEnabled() const
				throw() {return ball_stick_dashed_bonds->isChecked();}

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
			float getForceMaxLength() const
				throw() {return getFloatValue_(force_max_length_slider);}

			///
			float getForceScaling() const
				throw() {return getFloatValue_(force_scaling_slider);}

			///
			float getDNALadderRadius() const
				throw() {return getFloatValue_(cartoon_dna_ladder_radius_slider);}

			///
			float getDNABaseRadius() const
				throw() {return getFloatValue_(cartoon_dna_base_radius_slider);}

			///
			float getDNAHelixRadius() const
				throw() {return getFloatValue_(cartoon_dna_helix_radius_slider);}

			///
			void setStickStickRadius(float value) 
				throw() { setValue_(stick_radius_slider,value);}

			///
			void setBallAndStickStickRadius(float value) 
				throw() { setValue_(ball_stick_cylinder_radius_slider,value);}

			void setBallAndStickStickDashedBondsEnabled(bool state)
				throw() { ball_stick_dashed_bonds->setChecked(state);}

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

			///
			void setForceMaxLenght(float value) 
				throw() { setValue_(force_max_length_slider,value);}

			///
			void setForceScaling(float value) 
				throw() { setValue_(force_scaling_slider,value / 10.0);}

			///
			void setCartoonDNALadderRadius(float value)
				throw() { setValue_(cartoon_dna_ladder_radius_slider, value / 10.);}

			///
			void setCartoonDNAHelixRadius(float value)
				throw() { setValue_(cartoon_dna_helix_radius_slider, value / 10.);}

			///
			void setCartoonDNABaseRadius(float value)
				throw() { setValue_(cartoon_dna_base_radius_slider, value / 10.);}

			public slots:
			
			///

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
			void cartoonDNAHelixRadiusChanged(){setLabelText_(cartoon_dna_helix_radius_label, cartoon_dna_helix_radius_slider);}
			void cartoonDNABaseRadiusChanged(){setLabelText_(cartoon_dna_base_radius_label, cartoon_dna_base_radius_slider);}
			void cartoonDNALadderRadiusChanged(){setLabelText_(cartoon_dna_ladder_radius_label, cartoon_dna_ladder_radius_slider);}
			void hbondsRadiusChanged(){setLabelText_(hbonds_radius_label, hbonds_radius_slider);}
			void forceScalingChanged(){setLabelText_(force_scaling_label, force_scaling_slider);}
			void forceMaxLengthChanged(){setLabelText_(force_max_length_label, force_max_length_slider);}
			void showPage_(int nr);

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
