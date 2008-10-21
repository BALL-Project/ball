#ifndef SPACENAVIGATORPLUGIN_H
#define SPACENAVIGATORPLUGIN_H

#include <QtCore/QObject>
#include <QtGui/QPixmap>

#include <BALL/VIEW/PLUGIN/VIEWPlugin.h>
#include <BALL/VIEW/INPUT/inputPlugin.h>

namespace BALL
{
	namespace VIEW
	{

		class SpaceNavigatorPlugin : public QObject, public VIEWPlugin, public InputPlugin
		{
			Q_OBJECT
			Q_INTERFACES(BALL::VIEW::VIEWPlugin BALL::VIEW::InputPlugin)

			public:
				SpaceNavigatorPlugin();

				const QPixmap* getIcon() const;
				QString getName() const;
				QString getDescription() const;

				QDialog* getConfigDialog();

				bool isActive() { return is_active_; }

				InputDeviceDriver* startDriver(QWidget* receiver);

				void activate();
				void deactivate();

			private:
				QPixmap icon_;
				bool is_active_;
		};
	}
}

#endif //SPACENAVIGATORPLUGIN_H

