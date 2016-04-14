#include "../include/XML3DSettings.h"

#include <QtWidgets/QLineEdit>

#include <BALL/VIEW/KERNEL/common.h>

namespace BALL
{
	namespace VIEW
	{
		XML3DSettings::XML3DSettings(XML3DPlugin* plugin, QWidget* parent, const char* name, Qt::WindowFlags fl)
			: ConfigDialog(parent, fl),
			  Ui_XML3DSettingsData(),
			  plugin_(plugin)
		{
			setupUi(this);
			setObjectName(name);
			setINIFileSectionName("XML3D_PLUGIN");
			registerWidgets_();
		}

		void XML3DSettings::setHostName(const String& hostname)
		{
			host_edit->setText(hostname.c_str());
		}

		void XML3DSettings::setPort(const String& port)
		{
			port_edit->setText(port.c_str());
		}

		void XML3DSettings::setSensitivity(const Size sensitivity)
		{
			sensitivity_slider->setValue(sensitivity);
		}

		void XML3DSettings::accept()
		{
			plugin_->setHostName(ascii(host_edit->text()));
			plugin_->setPort(ascii(port_edit->text()));
			plugin_->setSensitivity(sensitivity_slider->value());

			QDialog::accept();
		}

	}
}
