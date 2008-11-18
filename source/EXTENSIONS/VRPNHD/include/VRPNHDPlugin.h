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

		class VRPNHDPlugin : public QObject, public BALLPlugin, public VIEWPlugin, public InputPlugin
		{
			Q_OBJECT
			Q_INTERFACES(BALL::BALLPlugin BALL::VIEW::VIEWPlugin BALL::VIEW::InputPlugin)

			public:
				VRPNHDPlugin();

				const QPixmap* getIcon() const;
				QString getName() const;
				QString getDescription() const;

				QDialog* getConfigDialog();

				bool isActive() { return is_active_; }

				void setReceiver(QWidget* receiver);
				InputDeviceDriver* startDriver();

				void activate();
				void deactivate();

			private:
				QWidget* receiver_;
				QPixmap icon_;
				bool is_active_;
		};
	}
}

#endif //VRPNHDPLUGIN_H

