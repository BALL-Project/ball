// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: modelSettingsDialog.C,v 1.12 2003/11/21 01:22:52 amoll Exp $
//

#include <BALL/VIEW/DIALOGS/modelSettingsDialog.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/FORMAT/INIFile.h>

#include <qslider.h>
#include <qlabel.h>
#include <qtabwidget.h>

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

		void ModelSettingsDialog::setDefaults(bool all)
			throw()
		{
			if (all || tabwidget->currentPageIndex() == 0)
			{
				stick_radius_slider->setValue(2);
			}
			
			if (all || tabwidget->currentPageIndex() == 1)
			{
				ball_stick_cylinder_radius_slider->setValue(2);
				ball_stick_sphere_radius_slider->setValue(4);
			}
			
			if (all || tabwidget->currentPageIndex() == 2)
			{
				vdw_radius_factor_slider->setValue(10);
			}
			
			if (all || tabwidget->currentPageIndex() == 3)
			{
				surface_probe_radius_slider->setValue(15);
			}
			
			if (all || tabwidget->currentPageIndex() == 4)
			{
				tube_radius_slider->setValue(4);
			}
			
			if (all || tabwidget->currentPageIndex() == 5)
			{
				cartoon_tube_radius_slider->setValue(4);
				cartoon_helix_radius_slider->setValue(20);
				cartoon_arrow_width_slider->setValue(4);
				cartoon_arrow_height_slider->setValue(8);
			}
			
			if (all || tabwidget->currentPageIndex() == 6)
			{
				hbonds_radius_slider->setValue(3);
			}
		}

		float ModelSettingsDialog::getFloatValue_(const QSlider* const& slider) const
			throw()
		{
			return (float(slider->value())) / 10.0;
		}

		void ModelSettingsDialog::setValue_(QSlider* le, float value)
			throw()
		{
			le->setValue((Size)(value * 10.0));
		}

		void ModelSettingsDialog::setLabelText_(QLabel* label, const QSlider* const from)
			throw()
		{
			String s((float)from->value() / 10.0);
			s.trimRight("0");
			if (s.hasSuffix(".")) s+= "0";
			label->setText(s.c_str());
		}

		void ModelSettingsDialog::fetchPreference_(const INIFile& inifile, const String& entry, QSlider& slider)
			throw()
		{
			if (!inifile.hasEntry("MODEL_OPTIONS", entry)) return;
			setValue_(&slider, inifile.getValue("MODEL_OPTIONS", entry).toFloat());
		}

		void ModelSettingsDialog::writePreference_(INIFile& inifile, const String& entry, const QSlider& slider) const
			throw()
		{
			inifile.insertValue("MODEL_OPTIONS", entry, getFloatValue_(&slider));
		}


		void ModelSettingsDialog::writePreferences(INIFile& file)
			throw()
		{
			file.appendSection("MODEL_OPTIONS");
			writePreference_(file, "stick_radius", *stick_radius_slider);
			writePreference_(file, "ball_stick_cylinder_radius", *ball_stick_cylinder_radius_slider);
			writePreference_(file, "ball_stick_sphere_radius", *ball_stick_sphere_radius_slider);
			writePreference_(file, "vdw_radius_factor", *vdw_radius_factor_slider);
			writePreference_(file, "surface_probe_radius", *surface_probe_radius_slider);
			writePreference_(file, "tube_radius", *tube_radius_slider);
			writePreference_(file, "cartoon_tube_radius", *cartoon_tube_radius_slider);
			writePreference_(file, "cartoon_helix_radius", *cartoon_helix_radius_slider);
			writePreference_(file, "cartoon_arrow_height", *cartoon_arrow_height_slider);
			writePreference_(file, "cartoon_arrow_width", *cartoon_arrow_width_slider);
			writePreference_(file, "cartoon_arrow_width", *cartoon_arrow_width_slider);
			writePreference_(file, "hbonds_radius", *hbonds_radius_slider);
		}

		void ModelSettingsDialog::fetchPreferences(const INIFile& file)
			throw()
		{
			fetchPreference_(file, "stick_radius", *stick_radius_slider);
			fetchPreference_(file, "ball_stick_cylinder_radius", *ball_stick_cylinder_radius_slider);
			fetchPreference_(file, "ball_stick_sphere_radius", *ball_stick_sphere_radius_slider);
			fetchPreference_(file, "vdw_radius_factor", *vdw_radius_factor_slider);
			fetchPreference_(file, "surface_probe_radius", *surface_probe_radius_slider);
			fetchPreference_(file, "tube_radius", *tube_radius_slider);
			fetchPreference_(file, "cartoon_tube_radius", *cartoon_tube_radius_slider);
			fetchPreference_(file, "cartoon_helix_radius", *cartoon_helix_radius_slider);
			fetchPreference_(file, "cartoon_arrow_height", *cartoon_arrow_height_slider);
			fetchPreference_(file, "cartoon_arrow_width", *cartoon_arrow_width_slider);
			fetchPreference_(file, "hbonds_radius", *hbonds_radius_slider);
		}

	void ModelSettingsDialog::setDefaultValues()
		throw()
	{
		setDefaults(false);
	}


	} // namespace VIEW
} // namespace BALL
