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
	general_tab_ = new DlgPreferencesGeneral;
	CHECK_PTR(general_tab_);

	network_tab_ = new DlgPreferencesNetwork;
	CHECK_PTR(network_tab_);

	display_tab_ = new DlgPreferencesDisplay;
	CHECK_PTR(display_tab_);

	addTab(general_tab_, "General");
	addTab(network_tab_, "Network");
	addTab(display_tab_, "Display");

	setApplyButton();
	setCancelButton();
	setOKButton();

	connect(this,
					SIGNAL(cancelButtonPressed()),
					SLOT(hide()));
}

DlgPreferences::~DlgPreferences()
{
}

void DlgPreferences::setPreferences(INIFile& inifile)
{
	//	
	// the display window position
	//
	inifile.setValue
		("WINDOWS", "Preferences::x", String(x()));
	inifile.setValue
		("WINDOWS", "Preferences::y", String(y()));

	general_tab_->setPreferences(inifile);
	network_tab_->setPreferences(inifile);
	display_tab_->setPreferences(inifile);
}

void DlgPreferences::getPreferences(INIFile& inifile)
{
	// 
	// the geometry of the preferences window
	//
	int x_pos = x();
	int y_pos = y();

	if (inifile.hasEntry("WINDOWS", "Preferences::x"))
	{
		x_pos = inifile.getValue("WINDOWS", "Preferences::x").toInt();
	}
	if (inifile.hasEntry("WINDOWS", "Preferences::y"))
	{
		y_pos = inifile.getValue("WINDOWS", "Preferences::y").toInt();
	}

	move(x_pos, y_pos);

	general_tab_->getPreferences(inifile);
	network_tab_->getPreferences(inifile);
	display_tab_->getPreferences(inifile);
}

DlgPreferencesGeneral* DlgPreferences::getGeneralTab()
{
	return general_tab_;
}

DlgPreferencesNetwork* DlgPreferences::getNetworkTab()
{
	return network_tab_;
}

DlgPreferencesDisplay* DlgPreferences::getDisplayTab()
{
	return display_tab_;
}



