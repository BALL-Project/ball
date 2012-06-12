#include <spaceNavigatorPlugin.h>

#include <spaceNavigatorDriver.h>


Q_EXPORT_PLUGIN2(pluginSpaceNavigator, BALL::VIEW::SpaceNavigatorPlugin)

namespace BALL
{
	namespace VIEW
	{
		SpaceNavigatorPlugin::SpaceNavigatorPlugin()
			: receiver_(), 
				icon_(":pluginSpaceNavigator.png"),
				is_active_(false)
		{
		}

		SpaceNavigatorPlugin::~SpaceNavigatorPlugin()
		{
		}

		void SpaceNavigatorPlugin::setReceiver(QWidget* receiver)
		{
			receiver_ = receiver;
		}

		QString SpaceNavigatorPlugin::getName() const
		{
			return QString("SpaceNavigator");
		}

		QString SpaceNavigatorPlugin::getDescription() const
		{
			return QString("A driver for the 3DConnexion SpaceNavigator input device.");
		}

		const QPixmap* SpaceNavigatorPlugin::getIcon() const
		{
			return &icon_;
		}

		QDialog* SpaceNavigatorPlugin::getConfigDialog()
		{
			return NULL;
		}

		InputDeviceDriver* SpaceNavigatorPlugin::startDriver()
		{
			SpaceNavigatorDriver* driver = new SpaceNavigatorDriver(receiver_);
			if(!driver->setUp()) {
				delete driver;
				return NULL;
			}

			is_active_ = true;

			driver->setEnabled(true);
			return driver;
		}

		bool SpaceNavigatorPlugin::activate()
		{
			driver_ = startDriver();
			return (bool)driver_;
		}

		bool SpaceNavigatorPlugin::deactivate()
		{
			driver_->tearDown();

			is_active_ = false;
			return false;
		}

	}
}

