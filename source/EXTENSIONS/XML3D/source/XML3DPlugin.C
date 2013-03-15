#include <XML3DPlugin.h>

#include <XML3DDriver.h>
#include <XML3DSettings.h>
#include <XMPPWidget.h>

#include <QtWidgets/QDialog>

#ifndef BALL_VIEW_WIDGETS_DOCKWIDGET_H
# include <BALL/VIEW/WIDGETS/dockWidget.h>
#endif

#include <BALL/VIEW/KERNEL/mainControl.h>

// TODO: make settings changeable while driver already running
namespace BALL
{
	namespace VIEW
	{
		XML3DPlugin::XML3DPlugin()
			: receiver_(), 
			  icon_(":pluginXML3D.png"),
				is_active_(false),
				hostname_("localhost"),
				port_("51967"),
				sensitivity_(500),
				settings_(0)
		{
		}

		void XML3DPlugin::setReceiver(QWidget* receiver)
		{
			receiver_ = receiver;
		}

		QString XML3DPlugin::getName() const
		{
			return QString("XML3D");
		}

		QString XML3DPlugin::getDescription() const
		{
			return QString("A driver for XML3D input devices.");
		}

		const QPixmap* XML3DPlugin::getIcon() const
		{
			return &icon_;
		}

		ConfigDialog* XML3DPlugin::getConfigDialog()
		{
			if(!settings_) {
				settings_ = new XML3DSettings(this, receiver_);
				settings_->setHostName(hostname_);
				settings_->setPort(port_);
				settings_->setSensitivity(sensitivity_);
			}

			return settings_;
		}

		InputDeviceDriver* XML3DPlugin::startDriver()
		{
			XML3DDriver* driver = new XML3DDriver(receiver_);
			driver->setHostName(hostname_);
			driver->setPort(port_);

			driver->setSensitivity(sensitivity_);

			if(!driver->setUp()) 
			{
				delete driver;
				return NULL;
			}

			driver->setEnabled(true);

			return driver;
		}

		bool XML3DPlugin::activate()
		{
			is_active_ = (startDriver() != 0);

			if(!is_active_)
			{
				return false;
			}

			XMPPWidget* chat_widget = new XMPPWidget;
			DockWidget* dock_widget = new DockWidget(MainControl::getInstance(0), "Chat Window");

			dock_widget->setGuest(*chat_widget);
			dock_widget->registerWidget(dock_widget);

			MainControl::getInstance(0)->addDockWidget(Qt::BottomDockWidgetArea, dock_widget);

			return true;
		}

		// TODO
		bool XML3DPlugin::deactivate()
		{
			is_active_ = false;
			return true;
		}

	}
}

