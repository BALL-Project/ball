// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: networkPreferences.C,v 1.1.2.3 2005/09/29 14:01:25 amoll Exp $
//

#include <BALL/VIEW/DIALOGS/networkPreferences.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/mainControl.h>

#include <qcheckbox.h>
#include <qlineedit.h>

namespace BALL
{
	namespace VIEW
	{

NetworkPreferences::NetworkPreferences(QWidget* parent, const char* name, WFlags fl)
	throw()
	: NetworkPreferencesData(parent, name, fl),
		PreferencesEntry()
{
	setINIFileSectionName("NETWORK");
	registerObject_(port_edit);
	registerObject_(host_edit);
	registerObject_(enable_proxy);

	setWidgetStackName("Network");
}

NetworkPreferences::~NetworkPreferences()
	throw()
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "Destructing object " << (void *)this 
								<< " of class NetworkPreferences" << endl;
	#endif 
}

void NetworkPreferences::getSettings()
{
	MainControl* mc = getMainControl();
	if (mc == 0) return;

	if (mc->getProxy() == "")
	{
		enable_proxy->setChecked(false);
		return;
	}

	host_edit->setText(mc->getProxy().c_str());
	port_edit->setText(String(mc->getProxyPort()).c_str());

	bool use_proxy = host_edit->text() != "" && port_edit->text() != "";

	enable_proxy->setChecked(use_proxy);
}

void NetworkPreferences::applySettings()
{
	MainControl* mc = getMainControl();
	if (mc == 0) return;

	if (!enable_proxy->isChecked() ||
			host_edit->text() == "" ||
			port_edit->text() == "")
	{
		mc->setProxy("", 0);
		return;
	}

	Position port = 0;
	try
	{
		port = String(port_edit->text().ascii()).toUnsignedInt();
	}
	catch(...)
	{
		return;
	}
		
	mc->setProxy(host_edit->text().ascii(), port);
}

} } // namespaces
