#ifndef VRPNPLUGIN_H
#define VRPNPLUGIN_H

#include <QtCore/QObject>
#include <QtGui/QPixmap>

#include <BALL/PLUGIN/BALLPlugin.h>
#include <BALL/VIEW/PLUGIN/VIEWPlugin.h>
#include <BALL/VIEW/INPUT/inputPlugin.h>

namespace BALL
{
	namespace VIEW
	{

		class VRPNPlugin : public QObject, public BALLPlugin, public VIEWPlugin, public InputPlugin
		{
			Q_OBJECT
			Q_PLUGIN_METADATA(IID "org.ball-project.Plugin.Input.VRPN")
			Q_INTERFACES(BALL::BALLPlugin BALL::VIEW::VIEWPlugin BALL::VIEW::InputPlugin)

			public:
				VRPNPlugin();

				const QPixmap* getIcon() const;
				QString getName() const;
				QString getDescription() const;

				ConfigDialog* getConfigDialog();

				bool isActive() { return is_active_; }

				void setReceiver(QWidget* receiver);
				InputDeviceDriver* startDriver();

				bool activate();
				bool deactivate();

			private:
				QWidget* receiver_;
				QPixmap icon_;
				bool is_active_;
		};
	}
}

#endif //VRPNPLUGIN_H

