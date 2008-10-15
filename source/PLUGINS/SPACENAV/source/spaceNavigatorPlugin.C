#include <spaceNavigatorPlugin.h>

#include <spaceNavigatorDriver.h>

#include <QtCore/QString>
#include <QtGui/QPixmap>

namespace BALL
{
	namespace VIEW
	{
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
			return NULL;
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

