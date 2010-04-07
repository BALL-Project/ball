#include "../include/OpenSimPluginConfiguration.h"

#include <QtGui/qlineedit.h>

#include <BALL/VIEW/KERNEL/common.h>

namespace BALL
{
	namespace VIEW
	{
		OpenSimPluginConfiguration::OpenSimPluginConfiguration(QWidget* parent, const char* name, Qt::WFlags fl)
			: QDialog(parent, fl),
				Ui_OpenSimPluginConfigurationData(),
				PreferencesEntry()
		{
			setupUi(this);
			setObjectName(name);
			setINIFileSectionName("OPENSIM_PLUGIN");
			registerWidgets_();
		}

		OpenSimPluginConfiguration::~OpenSimPluginConfiguration()
		{
		}

		void OpenSimPluginConfiguration::setRemoteServer(const String& address, Size port)
		{
			remote_address->setText(address.c_str());
			remote_port->setText(QString::number(port, 10));
		}

		void OpenSimPluginConfiguration::setLocalServer(const String& address, Size port)
		{
			local_address->setText(address.c_str());
			local_port->setText(QString::number(port, 10));
		}
	}
}
