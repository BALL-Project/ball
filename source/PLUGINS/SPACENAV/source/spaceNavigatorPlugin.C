#include <spaceNavigatorPlugin.h>

#include <spaceNavigatorDriver.h>


Q_EXPORT_PLUGIN2(pluginSpaceNavigator, BALL::VIEW::SpaceNavigatorPlugin)

namespace BALL
{
	namespace VIEW
	{
		SpaceNavigatorPlugin::SpaceNavigatorPlugin()
			: icon_(":logo.png"), is_active_(false)
		{
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

		InputDeviceDriver* SpaceNavigatorPlugin::startDriver(QWidget* receiver)
		{
			return new SpaceNavigatorDriver(receiver);
		}

		void SpaceNavigatorPlugin::activate()
		{
			is_active_ = true;
		}

		void SpaceNavigatorPlugin::deactivate()
		{
			is_active_ = false;
		}

	}
}

