/**********************************************************************

	--- Qt Architect generated file ---

	File: DlgPreferencesNetwork.C
	Last generated: Fri Jan 14 11:14:45 2000

 *********************************************************************/

#include "DlgPreferencesNetwork.h"

#include <BALL/VIEW/COMMON/global.h>

#define Inherited DlgPreferencesNetworkData

DlgPreferencesNetwork::DlgPreferencesNetwork
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
}

DlgPreferencesNetwork::~DlgPreferencesNetwork()
{
}

void DlgPreferencesNetwork::getPreferences(INIFile& inifile)
{
	bool server_status = true;
	int port = VIEW_DEFAULT_PORT;
	if (inifile.hasEntry("NETWORK", "start_server"))
	{
		server_status = (inifile.getValue("NETWORK", "start_server") == "true");
	}
	if (inifile.hasEntry("NETWORK", "start_server"))
	{
		port = inifile.getValue("NETWORK", "server_port").toInt();
	}
	
	// set the values of the widgets
	server_status_->setChecked(server_status);
	port_->setText(String(port).c_str());
}

void DlgPreferencesNetwork::setPreferences(INIFile& inifile)
{
	// retrieve the network settings from the dialog
	bool server_status = server_status_->isChecked();
	String port = String(port_->text().ascii());
	
	// save them to the inifile
	if (server_status)
	{
		inifile.setValue("NETWORK", "start_server", "true");
	} else {
		inifile.setValue("NETWORK", "start_server", "false");
	}
	inifile.setValue("NETWORK", "server_port", port);
}

void DlgPreferencesNetwork::resetPort()
{
	port_->setText(String(VIEW_DEFAULT_PORT).c_str());
}

int DlgPreferencesNetwork::getPort()
{
	return String(port_->text()).toInt();
}

bool DlgPreferencesNetwork::getServerStatus()
{
	return server_status_->isChecked();
}

