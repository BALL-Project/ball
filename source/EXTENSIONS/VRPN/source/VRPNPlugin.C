#include <VRPNPlugin.h>

#include <VRPNDriver.h>

namespace BALL
{
	namespace VIEW
	{
		VRPNPlugin::VRPNPlugin()
			: receiver_(), icon_(":pluginVRPN.png"), is_active_(false)
		{
		}

		void VRPNPlugin::setReceiver(QWidget* receiver)
		{
			receiver_ = receiver;
		}

		QString VRPNPlugin::getName() const
		{
			return QString("VRPN-SpaceNavigator");
		}

		QString VRPNPlugin::getDescription() const
		{
			return QString("A driver for the 3DConnexion VRPN input device via VRPN.");
		}

		const QPixmap* VRPNPlugin::getIcon() const
		{
			return &icon_;
		}

		ConfigDialog* VRPNPlugin::getConfigDialog()
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

		bool VRPNPlugin::activate()
		{
			is_active_ = true;

			return startDriver();
		}

		bool VRPNPlugin::deactivate()
		{
			is_active_ = false;

			return true;
		}

	}
}

