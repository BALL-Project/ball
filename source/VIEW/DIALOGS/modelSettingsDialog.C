// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/VIEW/DIALOGS/modelSettingsDialog.h>
#include <BALL/DATATYPE/string.h>

#include <qlineedit.h>

namespace BALL
{
	namespace VIEW
	{

ModelSettingsDialog::ModelSettingsDialog( QWidget* parent,  const char* name, WFlags fl )
    : ModelSettingsDialogData( parent, name, fl )
{
	setDefaults();
	show();
}

void ModelSettingsDialog::apply()
	throw()
{
}

void ModelSettingsDialog::setDefaults()
	throw()
{
								stick_radius->setText("0.2");
	ball_stick_cylinder_radius->setText("0.2");
		ball_stick_sphere_radius->setText("0.4");
					 vdw_radius_factor->setText("1.0");
				surface_probe_radius->setText("1.5");
							 	 tube_radius->setText("0.4");
				 cartoon_tube_radius->setText("0.4");
 			 cartoon_sphere_radius->setText("2.4");
				 cartoon_arrow_width->setText("0.6");
				cartoon_arrow_height->setText("1.5");
						   hbonds_radius->setText("0.3");
}

float ModelSettingsDialog::getFloatValue_(const QLineEdit* const& te) const
	throw()
{
	try
	{
		return String(te->text().ascii()).toFloat();
	}
	catch(...)
	{
		Log.error() << "Invalid value: " << te->text() << std::endl;
	}
	return 1;
}

} } // NAMESPACE
