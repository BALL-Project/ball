#include <XML3DPlugin.h>

#include <XML3DDriver.h>
#include <XML3DSettings.h>
#include <XMPPWidget.h>

#include <QtGui/QDialog>

#ifndef BALL_VIEW_WIDGETS_DOCKWIDGET_H
# include <BALL/VIEW/WIDGETS/dockWidget.h>
#endif

#include <BALL/VIEW/KERNEL/mainControl.h>

Q_EXPORT_PLUGIN2(pluginXML3D, BALL::VIEW::XML3DPlugin)

// TODO: make settings changeable while driver already running
namespace BALL
{
	namespace VIEW
	{
		XML3DPlugin::XML3DPlugin()
			: receiver_(), 
			  icon_(":logo.png"), 
				is_active_(false),
				hostname_("localhost"),
				port_("51967"),
				sensitivity_(500)
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

		QDialog* XML3DPlugin::getConfigDialog()
		{
			XML3DSettings *settings = new XML3DSettings(this, receiver_);

			settings->setHostName(hostname_);
			settings->setPort(port_);
			settings->setSensitivity(sensitivity_);

			return static_cast<QDialog*>(settings);
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
			bool is_active_ = (bool)startDriver();

			XMPPWidget* chat_widget = new XMPPWidget;
			DockWidget* dock_widget = new DockWidget(MainControl::getInstance(0), "Chat Window");

			dock_widget->setGuest(*chat_widget);
			dock_widget->registerWidget(dock_widget);

			MainControl::getInstance(0)->addDockWidget(Qt::BottomDockWidgetArea, dock_widget);

			return is_active_;
		}

		// TODO
		bool XML3DPlugin::deactivate()
		{
			return is_active_ = false;
		}

	}
}

