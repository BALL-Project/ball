// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DIALOGS/networkPreferences.h>
#include <BALL/VIEW/DIALOGS/downloadPDBFile.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/mainControl.h>

#include <QtGui/QCheckBox>
#include <QtGui/QLineEdit>

#include <QtNetwork/QNetworkProxy>

namespace BALL
{
	namespace VIEW
	{

		NetworkPreferences::NetworkPreferences(QWidget* parent, const char* name, Qt::WFlags fl)
			: QWidget(parent, fl),
				Ui_NetworkPreferencesData(),
				PreferencesEntry()
		{
			setINIFileSectionName("NETWORK");
			setupUi(this);

			setObjectName(name);

			// signals and slots connections
			connect(proxy_type_box, SIGNAL(currentIndexChanged(int)), this, SLOT(proxyModeChanged(int)));

			setWidgetStackName((String)tr("Network"));
			registerWidgets_();

			if (!proxyEnabled())
				proxy_settings_group->setEnabled(false);	
		}

		NetworkPreferences::~NetworkPreferences()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << (void *)this 
										<< " of class NetworkPreferences" << endl;
			#endif 
		}

		void NetworkPreferences::proxyModeChanged(int proxy_mode)
		{
			if (proxy_mode == 0)
			{
				proxy_settings_group->setEnabled(false);
			}
			else
			{
				proxy_settings_group->setEnabled(true);
			}
		}

		void NetworkPreferences::applySettings()
		{
			DownloadPDBFile* df = DownloadPDBFile::getInstance(0);
			if (df == 0) return;

			df->setPrefix(ascii(pdb_prefix->text()));
			df->setSuffix(ascii(pdb_suffix->text()));

			MainControl* mc = getMainControl();
			if (mc == 0) return;

			bool valid_port;
			unsigned int port = port_edit->text().toUInt(&valid_port);

			if (!valid_port && proxyEnabled())
			{
				Log.error() << (String)tr("Proxy port invalid!") << std::endl;
				return;
			}

			QNetworkProxy new_proxy((QNetworkProxy::ProxyType)proxy_type_box->currentIndex(),
															host_edit->text(), port, login_edit->text(), password_edit->text());

			QNetworkProxy::setApplicationProxy(new_proxy);
		}

		bool NetworkPreferences::proxyEnabled()
		{
			return (proxy_type_box->currentIndex() != 0);
		}

	}
} // namespaces
