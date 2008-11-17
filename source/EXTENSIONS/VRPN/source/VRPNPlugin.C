#include <VRPNPlugin.h>

#include <VRPNDriver.h>


Q_EXPORT_PLUGIN2(pluginVRPN, BALL::VIEW::VRPNPlugin)

namespace BALL
{
	namespace VIEW
	{
		VRPNPlugin::VRPNPlugin()
			: receiver_(), icon_(":logo.png"), is_active_(false)
		{
		}

		void VRPNPlugin::setReceiver(QWidget* receiver)
		{
			receiver_ = receiver;
		}

		QString VRPNPlugin::getName() const
		{
			return QString("VRPN");
		}

		QString VRPNPlugin::getDescription() const
		{
			return QString("A driver for the 3DConnexion VRPN input device via VRPN.");
		}

		const QPixmap* VRPNPlugin::getIcon() const
		{
			return &icon_;
		}

		QDialog* VRPNPlugin::getConfigDialog()
		{
			return NULL;
		}

		InputDeviceDriver* VRPNPlugin::startDriver()
		{
			VRPNDriver* driver = new VRPNDriver(receiver_); 
			driver->setServer("device0@localhost");
		  driver->setUp();
			driver->setEnabled(true);
			return(driver);
		}

		void VRPNPlugin::activate()
		{
			is_active_ = true;

			startDriver();
		}

		void VRPNPlugin::deactivate()
		{
			is_active_ = false;
		}

	}
}

