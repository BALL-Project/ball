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
}

void DlgPreferencesGeneral::getPreferences(INIFile& inifile)
{
}
