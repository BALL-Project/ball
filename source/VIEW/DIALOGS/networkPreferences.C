// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: networkPreferences.C,v 1.4.2.4 2006/10/22 14:08:47 amoll Exp $
//

#include <BALL/VIEW/DIALOGS/networkPreferences.h>
#include <BALL/VIEW/DIALOGS/downloadPDBFile.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/mainControl.h>

#include <QtGui/qcheckbox.h>
#include <QtGui/qlineedit.h>

namespace BALL
{
	namespace VIEW
	{

NetworkPreferences::NetworkPreferences(QWidget* parent, const char* name, Qt::WFlags fl)
	throw()
	: QWidget(parent, fl),
		Ui_NetworkPreferencesData(),
		PreferencesEntry()
{
	setINIFileSectionName("NETWORK");
	setupUi(this);
	
  // signals and slots connections
  connect( enable_proxy, SIGNAL( toggled(bool) ), host_edit, SLOT( setEnabled(bool) ) );
  connect( enable_proxy, SIGNAL( toggled(bool) ), port_edit, SLOT( setEnabled(bool) ) );

	setObjectName(name);
	registerObject_(port_edit);
	registerObject_(host_edit);
	registerObject_(enable_proxy);
	registerObject_(pdb_prefix);
	registerObject_(pdb_suffix);

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
		port = ascii(port_edit->text()).toUnsignedInt();
	}
	catch(...)
	{
		return;
	}
		
	mc->setProxy(ascii(host_edit->text()), port);

	DownloadPDBFile* df = DownloadPDBFile::getInstance(0);
	if (df == 0) return;

	df->setPrefix(ascii(pdb_prefix->text()));
	df->setSuffix(ascii(pdb_suffix->text()));
}

} } // namespaces
