#ifndef INPUTPLUGIN_H
#define INPUTPLUGIN_H

#include <QtCore/QtPlugin>

namespace BALL
{
	namespace VIEW
	{
		class InputDeviceDriver;

		class InputPlugin
		{
			public:
				virtual ~InputPlugin() {}

				virtual void setReceiver(QWidget* receiver) = 0;
				virtual InputDeviceDriver* startDriver() = 0;
		};
	}
}

Q_DECLARE_INTERFACE(BALL::VIEW::InputPlugin, "org.ball-project.Plugin.InputPlugin/1.0")

#endif //INPUTPLUGIN_H

