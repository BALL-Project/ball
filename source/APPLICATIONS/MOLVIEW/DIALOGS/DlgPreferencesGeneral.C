/**********************************************************************

	--- Qt Architect generated file ---

	File: DlgPreferencesGeneral.C
	Last generated: Thu Jan 13 15:01:42 2000

 *********************************************************************/

#include "DlgPreferencesGeneral.h"

#define Inherited DlgPreferencesGeneralData

#include <qplatinumstyle.h>
#include <qwindowsstyle.h>
#include <qmotifstyle.h>
#include <qcdestyle.h>
#include <qapplication.h>

DlgPreferencesGeneral::DlgPreferencesGeneral
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
}

DlgPreferencesGeneral::~DlgPreferencesGeneral()
{
}

QStyle* DlgPreferencesGeneral::getStyle()
{
	QStyle* new_style = 0;
	if (is_platinum_style->isChecked())
	{
		new_style = new QPlatinumStyle();
	}
	if (is_windows_style->isChecked())
	{
		new_style = new QWindowsStyle();
	}
	if (is_motif_style->isChecked())
	{
		new_style = new QMotifStyle();
	}
	if (is_cde_style->isChecked())
	{
		new_style = new QCDEStyle();
	}

	return new_style;
}

void DlgPreferencesGeneral::setPreferences(INIFile& inifile)
{
	String style = "platinum";
	if (is_platinum_style->isChecked())
	{
		style = "platinum";
	}
	else if (is_windows_style->isChecked())
	{
		style = "windows";
	}
	else if (is_motif_style->isChecked())
	{
		style = "motif";
	}
	else if (is_cde_style->isChecked())
	{
		style = "cde";
	}	

	// save the style settings
	inifile.setValue("WINDOWS", "style", style);
}

void DlgPreferencesGeneral::getPreferences(INIFile& inifile)
{
	String style = "platinum";
  if (inifile.hasEntry("WINDOWS", "style"))
  {
		style = inifile.getValue("WINDOWS", "style");
	}
	
	if (style == "motif")
	{
		is_motif_style->setChecked(TRUE);
	} 
	else if (style == "windows")
	{
		is_windows_style->setChecked(TRUE);
	}
	else if (style == "cde")
	{
		is_cde_style->setChecked(TRUE);
	} 
	else 
	{
		is_platinum_style->setChecked(TRUE);
	}
	
	// retrieve the current style and apply it
	// to the whole application
	QApplication::setStyle(getStyle());
}
