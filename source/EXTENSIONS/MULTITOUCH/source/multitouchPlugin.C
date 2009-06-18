#include <multitouchPlugin.h>

#include <multitouchDriver.h>
#include <multitouchSettings.h>

#include <QtGui/QDialog>

Q_EXPORT_PLUGIN2(pluginMultitouch, BALL::VIEW::MultitouchPlugin)

// TODO: make settings changeable while driver already running
namespace BALL
{
	namespace VIEW
	{
		MultitouchPlugin::MultitouchPlugin()
			: receiver_(), 
			  icon_(":logo.png"), 
				is_active_(false),
				hostname_("134.96.248.72"),
				port_("51967"),
				sensitivity_(500)
		{
		}

		void MultitouchPlugin::setReceiver(QWidget* receiver)
		{
			receiver_ = receiver;
		}

		QString MultitouchPlugin::getName() const
		{
			return QString("Multitouch");
		}

		QString MultitouchPlugin::getDescription() const
		{
			return QString("A driver for multitouch input devices.");
		}

		const QPixmap* MultitouchPlugin::getIcon() const
		{
			return &icon_;
		}

		QDialog* MultitouchPlugin::getConfigDialog()
		{
			MultitouchSettings *settings = new MultitouchSettings(this, receiver_);

			settings->setHostName(hostname_);
			settings->setPort(port_);
			settings->setSensitivity(sensitivity_);

			return static_cast<QDialog*>(settings);
		}

		InputDeviceDriver* MultitouchPlugin::startDriver()
		{
			MultitouchDriver* driver = new MultitouchDriver(receiver_);
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

		bool MultitouchPlugin::activate()
		{
			bool is_active_ = (bool)startDriver();

			return is_active_;
		}

		// TODO
		bool MultitouchPlugin::deactivate()
		{
			return is_active_ = false;
		}

	}
}

