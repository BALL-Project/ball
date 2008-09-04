#include <BALL/VIEW/INPUT/inputDeviceDriver.h>

#include <BALL/VIEW/INPUT/transformationEvent6D.h>
#include <BALL/VIEW/WIDGETS/scene.h>

#include <QtCore/QMutexLocker>
#include <QtCore/QCoreApplication>

namespace BALL
{
	namespace VIEW
	{
		InputDeviceDriver::InputDeviceDriver(Scene* scene)
			: receiver_(scene), enabled_(false)
		{
		}

		InputDeviceDriver::~InputDeviceDriver()
		{
		}

		Scene* InputDeviceDriver::getReceiver()
		{
			QMutexLocker locker(&receiverLock);
			return receiver_;
		}

		void InputDeviceDriver::setReceiver(Scene* s)
		{
			receiverLock.lock();
			receiver_ = s;
			receiverLock.unlock();
		}

		void InputDeviceDriver::setEnabled(bool enabled)
		{
			enabled_ = enabled;
		}

		void InputDeviceDriver::emitPositionChange(double x, double y, double z,
		                                           double a, double b, double c)
		{
			qApp->postEvent(static_cast<QObject*>(receiver_),
			                new TransformationEvent6D(x, y, z, a, b, c));
		}
	}
}

