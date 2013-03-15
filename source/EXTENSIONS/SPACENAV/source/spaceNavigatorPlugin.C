#include <spaceNavigatorPlugin.h>

#include <spaceNavigatorDriver.h>

namespace BALL
{
	namespace VIEW
	{
		SpaceNavigatorPlugin::SpaceNavigatorPlugin()
			: icon_(":pluginSpaceNavigator.png"),
			  driver_(0)
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

		ConfigDialog* SpaceNavigatorPlugin::getConfigDialog()
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

			driver->setEnabled(true);
			return driver;
		}

		bool SpaceNavigatorPlugin::activate()
		{
			driver_ = startDriver();
			return driver_ != 0;
		}

		bool SpaceNavigatorPlugin::deactivate()
		{
			driver_->tearDown();

			delete driver_;
			driver_ = 0;

			return true;
		}

	}
}

