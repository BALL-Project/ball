#ifndef SPACENAVIGATORPLUGIN_H
#define SPACENAVIGATORPLUGIN_H

#include <BALL/VIEW/INPUT/inputPlugin.h>

class QWidget;

namespace BALL
{
	namespace VIEW
	{

		class SpaceNavigatorPlugin : InputPlugin
		{
			Q_OBJECT
			Q_INTERFACES(VIEWPlugin)

			public:
				const QPixmap* getIcon() const;
				QString getName() const;
				QString getDescription() const;

				QDialog* getConfigDialog();

				bool isActive() { return is_active_; }

				InputDeviceDriver* startDriver(QWidget* receiver);

			public slots:
				virtual void activate();
				virtual void deactivate();

			private:
				bool is_active_;
		};
	}
}

#endif //SPACENAVIGATORPLUGIN_H

