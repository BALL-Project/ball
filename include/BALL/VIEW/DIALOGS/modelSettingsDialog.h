// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: modelSettingsDialog.h,v 1.6 2003/11/15 01:39:28 amoll Exp $
//

#ifndef BALL_VIEW_DIALOGS_MODELSETTINGSDIALOG_H
#define BALL_VIEW_DIALOGS_MODELSETTINGSDIALOG_H

#include <BALL/VIEW/UIC/modelSettingsDialogData.h>

class QLineEdit;

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
			void setDefaults()
				throw();

			/// Apply the new values to the stage
			void apply()
				throw();

			///
			float getStickStickRadius() const
				throw() {return getFloatValue_(stick_radius);}

			///
			float getBallAndStickStickRadius() const
				throw() {return getFloatValue_(ball_stick_cylinder_radius);}

			///
			float getBallRadius() const
				throw() {return getFloatValue_(ball_stick_sphere_radius);}

			///
			float getVDWRadiusFactor() const
				throw() {return getFloatValue_(vdw_radius_factor);}

			///
			float getSurfaceProbeRadius() const
				throw() {return getFloatValue_(surface_probe_radius);}

			///
			float getTubeRadius() const
				throw() {return getFloatValue_(tube_radius);}

			///
			float getCartoonTubeRadius() const
				throw() {return getFloatValue_(cartoon_tube_radius);}

			///
			float getCartoonHelixRadius() const
				throw() {return getFloatValue_(cartoon_helix_radius);}

			///
			float getCartoonArrowWidth() const
				throw() {return getFloatValue_(cartoon_arrow_width);}

			///
			float getCartoonArrowHeight() const
				throw() {return getFloatValue_(cartoon_arrow_height);}

			///
			float getHBondsRadius() const
				throw() {return getFloatValue_(hbonds_radius);}

			///
			void setStickStickRadius(float value) 
				throw() { setValue_(stick_radius,value);}

			///
			void setBallAndStickStickRadius(float value) 
				throw() { setValue_(ball_stick_cylinder_radius,value);}

			///
			void setBallRadius(float value) 
				throw() { setValue_(ball_stick_sphere_radius,value);}

			///
			void setVDWRadiusFactor(float value) 
				throw() { setValue_(vdw_radius_factor,value);}

			///
			void setSurfaceProbeRadius(float value) 
				throw() { setValue_(surface_probe_radius,value);}

			///
			void setTubeRadius(float value) 
				throw() { setValue_(tube_radius,value);}

			///
			void setCartoonTubeRadius(float value) 
				throw() { setValue_(cartoon_tube_radius,value);}

			///
			void setCartoonHelixRadius(float value) 
				throw() { setValue_(cartoon_helix_radius,value);}

			///
			void setCartoonArrowWidth(float value) 
				throw() { setValue_(cartoon_arrow_height,value);}

			///
			void setCartoonArrowHeight(float value) 
				throw() { setValue_(cartoon_arrow_height,value);}

			///
			void setHBondRadius(float value) 
				throw() { setValue_(hbonds_radius,value);}

			protected:
			
			float getFloatValue_(const QLineEdit* const & le) const
				throw();

			void setValue_(QLineEdit* le, float value)
				throw();

			void fetchPreference_(const INIFile& inifile, const String& entry, 
														QLineEdit& lineedit)
				throw();

			void writePreference_(INIFile& inifile, const String& entry, 
														const QLineEdit& lineedit) const
				throw();
		};

} }

#endif
