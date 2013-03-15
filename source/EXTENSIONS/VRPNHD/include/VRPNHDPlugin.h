#ifndef VRPNHDPLUGIN_H
#define VRPNHDPLUGIN_H

#include <QtCore/QObject>
#include <QtGui/QPixmap>

#include <BALL/PLUGIN/BALLPlugin.h>
#include <BALL/VIEW/PLUGIN/VIEWPlugin.h>
#include <BALL/VIEW/INPUT/inputPlugin.h>

namespace BALL
{
	namespace VIEW
	{

		class Scene;
		class VRPNHDPlugin : public QObject, public BALLPlugin, public VIEWPlugin, public InputPlugin
		{
			Q_OBJECT
			Q_PLUGIN_METADATA(IID "org.ball-project.Plugin.Input.VRPNHD")
			Q_INTERFACES(BALL::BALLPlugin BALL::VIEW::VIEWPlugin BALL::VIEW::InputPlugin)

			public:
				VRPNHDPlugin();

				const QPixmap* getIcon() const;
				QString getName() const;
				QString getDescription() const;

				ConfigDialog* getConfigDialog();

				bool isActive() { return is_active_; }

				InputDeviceDriver* startDriver();

				bool activate();
				bool deactivate();

				void setReceiver(QWidget* receiver);


			private:
				QPixmap icon_;
				bool is_active_;
				Scene* receiver_;
		};
	}
}

#endif //VRPNHDPLUGIN_H

