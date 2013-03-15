#ifndef SPACENAVIGATORPLUGIN_H
#define SPACENAVIGATORPLUGIN_H

#include <QtCore/QObject>
#include <QtGui/QPixmap>

#include <BALL/PLUGIN/BALLPlugin.h>
#include <BALL/VIEW/PLUGIN/VIEWPlugin.h>
#include <BALL/VIEW/INPUT/inputPlugin.h>

namespace BALL
{
	namespace VIEW
	{

		class SpaceNavigatorPlugin : public QObject, public BALLPlugin, public VIEWPlugin, public InputPlugin
		{
			Q_OBJECT
			Q_PLUGIN_METADATA(IID "org.ball-project.Plugin.Input.SpaceNavigator")
			Q_INTERFACES(BALL::BALLPlugin BALL::VIEW::VIEWPlugin BALL::VIEW::InputPlugin)

			public:
				SpaceNavigatorPlugin();
				virtual ~SpaceNavigatorPlugin();

				const QPixmap* getIcon() const;
				QString getName() const;
				QString getDescription() const;

				ConfigDialog* getConfigDialog();

				bool isActive() { return driver_ != 0; }

				void setReceiver(QWidget* receiver);
				InputDeviceDriver* startDriver();

				bool activate();
				bool deactivate();

			private:
				QWidget* receiver_;
				QPixmap icon_;
				InputDeviceDriver* driver_;
		};
	}
}

#endif //SPACENAVIGATORPLUGIN_H

