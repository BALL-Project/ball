// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: modelSettingsDialog.h,v 1.3 2003/11/13 21:46:50 amoll Exp $
//

#ifndef BALL_VIEW_DIALOGS_MODELSETTINGSDIALOG_H
#define BALL_VIEW_DIALOGS_MODELSETTINGSDIALOG_H

#include <BALL/VIEW/UIC/modelSettingsDialogData.h>
#include <qlineedit.h>
#include <BALL/DATATYPE/string.h>

class QLineEdit;

namespace BALL
{
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
			float getCartoonSphereRadius() const
				throw() {return getFloatValue_(cartoon_sphere_radius);}

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
				throw() { stick_radius->setText(String(value).c_str());}

			///
			void setBallAndStickStickRadius(float value) 
				throw() { ball_stick_cylinder_radius->setText(String(value).c_str());}

			///
			void setBallRadius(float value) 
				throw() { ball_stick_sphere_radius->setText(String(value).c_str());}

			///
			void setVDWRadiusFactor(float value) 
				throw() { vdw_radius_factor->setText(String(value).c_str());}

			///
			void setSurfaceProbeRadius(float value) 
				throw() { surface_probe_radius->setText(String(value).c_str());}

			///
			void setTubeRadius(float value) 
				throw() { tube_radius->setText(String(value).c_str());}

			///
			void setCartoonTubeRadius(float value) 
				throw() { cartoon_tube_radius->setText(String(value).c_str());}

			///
			void setCartoonSphereRadius(float value) 
				throw() { cartoon_sphere_radius->setText(String(value).c_str());}

			///
			void setCartoonArrowWidth(float value) 
				throw() { cartoon_arrow_height->setText(String(value).c_str());}

			///
			void setCartoonArrowHeight(float value) 
				throw() { cartoon_arrow_height->setText(String(value).c_str());}

			///
			void setHBondRadius(float value) 
				throw() { hbonds_radius->setText(String(value).c_str());}

			protected:
			
			float getFloatValue_(const QLineEdit* const & te) const
				throw();
		};

} }

#endif
