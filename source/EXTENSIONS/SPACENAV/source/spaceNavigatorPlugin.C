#include <spaceNavigatorPlugin.h>

#include <spaceNavigatorDriver.h>

Q_EXPORT_PLUGIN2(pluginSpaceNavigator, BALL::VIEW::SpaceNavigatorPlugin)

namespace BALL
{
	namespace VIEW
	{
		SpaceNavigatorPlugin::SpaceNavigatorPlugin()
			: receiver_(), icon_(":logo.png"), is_active_(false)
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
			driver->setEnabled(true);
			return driver;
		}

		bool SpaceNavigatorPlugin::activate()
		{
			return (is_active_ = (bool)startDriver());
		}

		bool SpaceNavigatorPlugin::deactivate()
		{
			return is_active_ = false;
		}

	}
}

