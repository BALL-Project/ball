#include <BALL/VIEW/INPUT/notificationEvent.h>
#include <BALL/VIEW/KERNEL/common.h>


namespace BALL
{
	namespace VIEW
	{
		NotificationEvent::NotificationEvent(EventsIDs type, Message* message)
			: QEvent(static_cast<QEvent::Type>(type)),
			  message_(message)
		{
		}

	}
}

