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

				/**
				 * Set the receiver of the events emited by the
				 * input device.
				 */
				virtual void setReceiver(QWidget* receiver) = 0;

				/**
				 * Starts the input device driver and makes it send events.
				 *
				 * @return A pointer to the started driver
				 */
				virtual InputDeviceDriver* startDriver() = 0;
		};
	}
}

Q_DECLARE_INTERFACE(BALL::VIEW::InputPlugin, "org.ball-project.Plugin.InputPlugin/1.0")

#endif //INPUTPLUGIN_H

