// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: modelSettingsDialog.C,v 1.6 2003/11/14 20:49:04 oliver Exp $

#include <BALL/VIEW/DIALOGS/modelSettingsDialog.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/FORMAT/INIFile.h>

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
			cartoon_helix_radius->setText("2.0");
			cartoon_arrow_width->setText("0.4");
			cartoon_arrow_height->setText("0.8");
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

		void ModelSettingsDialog::setValue_(QLineEdit* le, float value)
			throw()
		{
			String s(value);
			s.trimRight("0");
			if (s.hasSuffix(".")) s+= "0";
			le->setText(s.c_str());
		}

		void ModelSettingsDialog::fetchPreference_(const INIFile& inifile, const String& entry, 
																							 QLineEdit& lineedit)
			throw()
		{
			if (!inifile.hasEntry("MODEL_OPTIONS", entry)) return;
			lineedit.setText(inifile.getValue("MODEL_OPTIONS", entry).c_str());
		}

		void ModelSettingsDialog::writePreference_(INIFile& inifile, const String& entry, 
																			const QLineEdit& lineedit) const
			throw()
		{
			inifile.insertValue("MODEL_OPTIONS", entry, lineedit.text().ascii());
		}


		void ModelSettingsDialog::writePreferences(INIFile& file)
			throw()
		{
			file.appendSection("MODEL_OPTIONS");
			writePreference_(file, "stick_radius", *stick_radius);
			writePreference_(file, "ball_stick_cylinder_radius", *ball_stick_cylinder_radius);
			writePreference_(file, "ball_stick_sphere_radius", *ball_stick_sphere_radius);
			writePreference_(file, "vdw_radius_factor", *vdw_radius_factor);
			writePreference_(file, "surface_probe_radius", *surface_probe_radius);
			writePreference_(file, "tube_radius", *tube_radius);
			writePreference_(file, "cartoon_tube_radius", *cartoon_tube_radius);
			writePreference_(file, "cartoon_helere_radius", *cartoon_helere_radius);
			writePreference_(file, "cartoon_arrow_height", *cartoon_arrow_height);
			writePreference_(file, "cartoon_arrow_width", *cartoon_arrow_width);
		}

		void ModelSettingsDialog::fetchPreferences(const INIFile& file)
			throw()
		{
			fetchPreference_(file, "stick_radius", *stick_radius);
			fetchPreference_(file, "ball_stick_cylinder_radius", *ball_stick_cylinder_radius);
			fetchPreference_(file, "ball_stick_sphere_radius", *ball_stick_sphere_radius);
			fetchPreference_(file, "vdw_radius_factor", *vdw_radius_factor);
			fetchPreference_(file, "surface_probe_radius", *surface_probe_radius);
			fetchPreference_(file, "tube_radius", *tube_radius);
			fetchPreference_(file, "cartoon_tube_radius", *cartoon_tube_radius);
			fetchPreference_(file, "cartoon_helere_radius", *cartoon_helere_radius);
			fetchPreference_(file, "cartoon_arrow_height", *cartoon_arrow_height);
			fetchPreference_(file, "cartoon_arrow_width", *cartoon_arrow_width);
		}
	
	} // namespace VIEW

} // namespace BALL
