#include <VRPNHDPlugin.h>
#include <VRPNHDDriver.h>

#include <BALL/VIEW/WIDGETS/scene.h>

namespace BALL
{
	namespace VIEW
	{
		VRPNHDPlugin::VRPNHDPlugin()
			: receiver_(), icon_(":pluginVRPNHD.png"), is_active_(false)
		{
		}

		QString VRPNHDPlugin::getName() const
		{
			return QString("VRPN-DTrack");
		}

		void VRPNHDPlugin::setReceiver(QWidget* receiver)
		{
			receiver_ = (Scene*)receiver;
		}

		QString VRPNHDPlugin::getDescription() const
		{
			return QString("A driver for a head tracking device via VRPN.");
		}

		const QPixmap* VRPNHDPlugin::getIcon() const
		{
			return &icon_;
		}

		ConfigDialog* VRPNHDPlugin::getConfigDialog()
		{
			return NULL;
		}

		InputDeviceDriver* VRPNHDPlugin::startDriver()
		{
			VRPNHDDriver* driver = new VRPNHDDriver(receiver_); 
			driver->setServer("DTrack@localhost");
		  driver->setUp();
			driver->setEnabled(true);
			return(driver);
		}

		bool VRPNHDPlugin::activate()
		{
			is_active_ = startDriver();

			if(!is_active)
			{
				return false;
			}

			receiver_->resetTracking();
			return true;
		}

		bool void VRPNHDPlugin::deactivate()
		{
			is_active_ = false;

			return true;
		}

	}
}

