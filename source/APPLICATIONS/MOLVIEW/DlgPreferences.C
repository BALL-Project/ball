/**********************************************************************

	--- Qt Architect generated file ---

	File: DlgPreferences.C
	Last generated: Thu Jan 13 14:59:16 2000

 *********************************************************************/

#include "DlgPreferences.h"
#include "DlgPreferencesGeneral.h"
#include "DlgPreferencesNetwork.h"
#include "DlgPreferencesDisplay.h"
#include <BALL/FORMAT/INIFile.h>

#define Inherited DlgPreferencesData

DlgPreferences::DlgPreferences
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
	addTab(new DlgPreferencesGeneral, "General");
	addTab(new DlgPreferencesNetwork, "Network");
	addTab(new DlgPreferencesDisplay, "Display");
	setApplyButton();
	setCancelButton();
	setOKButton();
}

DlgPreferences::~DlgPreferences()
{
}


void DlgPreferences::setPreferences(INIFile& inifile)
{
}

void DlgPreferences::getPreferences(INIFile& inifile)
{
}

