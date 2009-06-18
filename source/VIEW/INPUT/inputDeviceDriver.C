#include <BALL/VIEW/INPUT/inputDeviceDriver.h>

#include <BALL/VIEW/INPUT/transformationEvent6D.h>
#include <BALL/VIEW/INPUT/headTrackingEvent.h>
#include <BALL/VIEW/INPUT/motionTrackingEvent.h>
#include <BALL/VIEW/WIDGETS/scene.h>

#include <QtCore/QMutexLocker>
#include <QtCore/QCoreApplication>

namespace BALL
{
	namespace VIEW
	{
		InputDeviceDriver::InputDeviceDriver(QWidget* scene)
			: receiver_(scene), enabled_(false)
		{
		}

		InputDeviceDriver::~InputDeviceDriver()
		{
		}

		QWidget* InputDeviceDriver::getReceiver()
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
			QCoreApplication::instance()->postEvent(
			                      static_cast<QObject*>(receiver_),
			                      new TransformationEvent6D(this, x, y, z, a, b, c));
		}
		
		void InputDeviceDriver::emitPositionChange(double x, double y, double z,
		                                           double q1, double q2, double q3, double q4)
		{
			QCoreApplication::instance()->postEvent(
		                      	static_cast<QObject*>(receiver_),
		                      	new MotionTrackingEvent(this, x, y, z, q1, q2, q3, q4));
		}
		
		void InputDeviceDriver::emitHeadChange(double x, double y, double z,
	                                               double q1, double q2, double q3, double q4)
		{
			QCoreApplication::instance()->postEvent(
		                      	static_cast<QObject*>(receiver_),
		                      	new HeadTrackingEvent(this, x, y, z, q1, q2, q3, q4));
		}
	}
}

