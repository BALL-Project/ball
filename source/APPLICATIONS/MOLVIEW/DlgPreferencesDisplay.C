/**********************************************************************

	--- Qt Architect generated file ---

	File: DlgPreferencesDisplay.C
	Last generated: Fri Jan 14 13:15:10 2000

 *********************************************************************/

#include "DlgPreferencesDisplay.h"
#include <qcolordialog.h>
#define Inherited DlgPreferencesDisplayData

using namespace BALL::VIEW;

DlgPreferencesDisplay::DlgPreferencesDisplay
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
	depth_cueing_check_->setEnabled(false);
	coordinate_axes_check_->setEnabled(false);
	edit_button_->setEnabled(false);
}

DlgPreferencesDisplay::~DlgPreferencesDisplay()
{
}

void DlgPreferencesDisplay::editColor()
{
  color_sample_->setBackgroundColor(QColorDialog::getColor(color_sample_->backgroundColor()));
}

void DlgPreferencesDisplay::setPreferences(INIFile& inifile)
{
	// 
	// the checkbox values
	// 
	inifile.setValue
		("WINDOWS", 
		 "Preferences::Display::Depth_Cueing", 
		 String(depth_cueing_check_->isChecked()));

	inifile.setValue
		("WINDOWS", 
		 "Preferences::Display::Coordinate_Axes", 
		 String(coordinate_axes_check_->isChecked()));

	// 
	// the color value
	// 
	ColorRGBA color;

	QColor qcolor = color_sample_->backgroundColor();
 	color.set((float)qcolor.red() / 255.0,
						(float)qcolor.green() / 255.0,
						(float)qcolor.blue() / 255.0);

	inifile.setValue
		("WINDOWS", "Preferences::Display::customcolor", color);
}

void DlgPreferencesDisplay::getPreferences(INIFile& inifile)
{
	// 
	// the checkbox values
	// 
	if (inifile.hasEntry("WINDOWS", "Preferences::Display::Depth_Cueing"))
	{
		depth_cueing_check_
			->setChecked((bool)
									 (inifile.getValue("WINDOWS", 
																		 "Preferences::Display::Depth_Cueing").toInt()));
	}

	if (inifile.hasEntry("WINDOWS", "Preferences::Display::Coordinate_Axes"))
	{
		coordinate_axes_check_
			->setChecked((bool)
									 (inifile.getValue("WINDOWS", 
																		 "Preferences::Display::Coordinate_Axes").toInt()));
	}

	// 
	// the color value
	// 
	if (inifile.hasEntry("WINDOWS", "Preferences::Display::customcolor"))
	{
		ColorRGBA color;
		color.set(inifile.getValue("WINDOWS", "Preferences::Display::customcolor"));

		QColor qcolor(color.red(), 
									color.green(), 
									color.blue());

		color_sample_->setBackgroundColor(qcolor);
	}
}

const QColor& DlgPreferencesDisplay::getQColor() const
{
	return color_sample_->backgroundColor();
}

ColorRGBA DlgPreferencesDisplay::getColor() const
{
	ColorRGBA color;

	QColor qcolor = color_sample_->backgroundColor();
 	color.set((float)qcolor.red() / 255.0,
						(float)qcolor.green() / 255.0,
						(float)qcolor.blue() / 255.0);

	return color;
}

bool DlgPreferencesDisplay::isDepthCueingEnabled() const
{
	return depth_cueing_check_->isChecked();
}

bool DlgPreferencesDisplay::isCoordinateAxesEnabled() const
{
	return coordinate_axes_check_->isChecked();
}
