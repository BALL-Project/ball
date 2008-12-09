#include <VRPNHDPlugin.h>
#include <VRPNHDDriver.h>

#include <BALL/VIEW/WIDGETS/scene.h>


Q_EXPORT_PLUGIN2(pluginVRPNHD, BALL::VIEW::VRPNHDPlugin)

namespace BALL
{
	namespace VIEW
	{
		VRPNHDPlugin::VRPNHDPlugin()
			: receiver_(), icon_(":logo.png"), is_active_(false)
		{
		}

		QString VRPNHDPlugin::getName() const
		{
			return QString("VRPN-HD");
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

		QDialog* VRPNHDPlugin::getConfigDialog()
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

		void VRPNHDPlugin::activate()
		{
			is_active_ = true;

			receiver_->resetTracking();
			startDriver();
		}

		void VRPNHDPlugin::deactivate()
		{
			is_active_ = false;
		}

	}
}

