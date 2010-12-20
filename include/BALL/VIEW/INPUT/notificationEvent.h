#ifndef NOTIFICATIONEVENT_H
#define NOTIFICATIONEVENT_H

#include <QtCore/QEvent>

#include <BALL/VIEW/KERNEL/message.h>

namespace BALL
{
	namespace VIEW
	{
		class NotificationEvent : public QEvent
		{
			public:
				NotificationEvent(EventsIDs type, Message* message);

				Message* getMessage() { return message_; }
			private:
				Message* message_;
		};
	}
}

#endif //NOTIFICATIONEVENT_H
