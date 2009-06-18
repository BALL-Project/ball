#include "../include/multitouchSettings.h"

#include <QtGui/qlineedit.h> 

#include <BALL/VIEW/KERNEL/common.h>

namespace BALL
{
	namespace VIEW
	{
		MultitouchSettings::MultitouchSettings(MultitouchPlugin* plugin, QWidget* parent, const char* name, Qt::WFlags fl)
			: QDialog(parent, fl),
				Ui_multitouchSettingsData(),
				PreferencesEntry(),
				plugin_(plugin)
		{
			setupUi(this);
			setObjectName(name);
			setINIFileSectionName("MULTITOUCH_PLUGIN");
			registerWidgets_();
		}

		void MultitouchSettings::setHostName(const String& hostname)
		{
			host_edit->setText(hostname.c_str());
		}

		void MultitouchSettings::setPort(const String& port)
		{
			port_edit->setText(port.c_str());
		}

		void MultitouchSettings::setSensitivity(const Size sensitivity)
		{
			sensitivity_slider->setValue(sensitivity);
		}

		void MultitouchSettings::accept()
		{
			plugin_->setHostName(ascii(host_edit->text()));
			plugin_->setPort(ascii(port_edit->text()));
			plugin_->setSensitivity(sensitivity_slider->value());

			QDialog::accept();
		}

	}
}
