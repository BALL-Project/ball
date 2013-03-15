#ifndef BALL_EXTENSIONS_XMPPWIDGET_H
#define BALL_EXTENSIONS_XMPPWIDGET_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
# include <BALL/DATATYPE/string.h>
#endif

#include "ui_XMPPWidget.h"

#include <loudmouth/loudmouth.h>

#include <QtCore/QThread>

namespace BALL
{
	namespace VIEW
	{
		class XMPPWidget
			: public QWidget,
				public Ui_XMPPWidget
		{
			Q_OBJECT

			public:
				XMPPWidget(QWidget* parent = 0, const char* name = "XMPPWidget", Qt::WindowFlags fl = 0);
				static void unescapeHTML(String& text);

				virtual bool eventFilter(QObject* object, QEvent* event);

			public slots:
				virtual void sendMessage();

			protected:
				static void checkPython_(String& text);
				static LmHandlerResult handleMessages_(LmMessageHandler* handler, LmConnection *connection, LmMessage *m, gpointer user_data);

				LmConnection* xmpp_connection_;	

				static XMPPWidget* instance;
		};
	}
}

#endif
