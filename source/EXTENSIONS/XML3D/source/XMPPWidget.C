#include <XMPPWidget.h>

#include <BALL/COMMON/logStream.h>
#include <BALL/VIEW/WIDGETS/pyWidget.h>

namespace BALL
{
	namespace VIEW
	{

		XMPPWidget* XMPPWidget::instance = NULL;

		XMPPWidget::XMPPWidget(QWidget* parent, const char* name, Qt::WindowFlags fl)
			: QWidget(parent, fl),
				Ui_XMPPWidget()
		{
			XMPPWidget::instance = this;

			setupUi(this);
			setObjectName(name);
			
			gchar server[1024] = "pinguin.bioinf.uni-sb.de";
			gchar user[1024] = "testuser@pinguin.bioinf.uni-sb.de";
			gchar login[1024] = "testuser";
			gchar pass[1024] = "test123";
			gint  port = 5222;

			xmpp_connection_ = lm_connection_new(server);

			lm_connection_set_port(xmpp_connection_, port);	
			lm_connection_set_jid(xmpp_connection_, user);	

			GError* error = NULL;

			if (!lm_connection_open_and_block(xmpp_connection_, &error))
			{
				Log.error() << "Failed to open XMPP connection: " << error->message << std::endl;
			}
			else
			{
				history->append(QString("connected to xmpp server"));
			}

			if (!lm_connection_authenticate_and_block(xmpp_connection_, login, pass, "BALLView XMPP", &error))
			{
				Log.error() << "Failed to authenticate " << error->message << std::endl;
			}
			else
			{
				history->append(QString("Successfully authenticated with xmpp server"));
			}

			LmMessage *m;
			m = lm_message_new_with_sub_type(NULL,
				LM_MESSAGE_TYPE_PRESENCE,
				LM_MESSAGE_SUB_TYPE_AVAILABLE);
			lm_connection_send(xmpp_connection_, m, NULL);	

			history->append(QString("Send presence message"));
//			history->append(QString(lm_message_node_to_string(m->node)));

			lm_message_unref(m);

			LmMessageHandler* handler = lm_message_handler_new(XMPPWidget::handleMessages_, NULL, NULL);
			lm_connection_register_message_handler(xmpp_connection_, handler, LM_MESSAGE_TYPE_MESSAGE, LM_HANDLER_PRIORITY_NORMAL);
			lm_message_handler_unref(handler);

			message->installEventFilter(this);
		}

		bool XMPPWidget::eventFilter(QObject* object, QEvent* event)
		{
			bool filter_out = false;

			switch (event->type())
			{
				case QEvent::KeyPress:
					if (((QKeyEvent*)event)->key() == Qt::Key_Return)
					{
						sendMessage();
						filter_out = true;
					}
					break;
				default:
					break;
			}

			if (filter_out)
				return true;
			else
				return QWidget::eventFilter(object, event);
		}

		void XMPPWidget::sendMessage()
		{
			QString current_message = message->toPlainText();
			history->append(current_message);
			message->clear();

			LmMessage* m;
			GError *error = NULL;

			QString recipient_qs = recipient->text();

			m = lm_message_new_with_sub_type(((String)recipient_qs).c_str(), LM_MESSAGE_TYPE_MESSAGE, LM_MESSAGE_SUB_TYPE_CHAT);
			lm_message_node_add_child(m->node, "body", ((String)current_message).c_str());

			lm_connection_send(xmpp_connection_, m, &error);

			lm_message_unref(m);
		}

		void XMPPWidget::unescapeHTML(String& text)
		{
			while (text.substitute("&lt;", "<") != String::EndPos) {};
			while (text.substitute("&gt;", ">") != String::EndPos) {};
			while (text.substitute("&quot;", "\"") != String::EndPos) {};
			while (text.substitute("&amp;", "&") != String::EndPos) {};
			while (text.substitute("&apos;", "&") != String::EndPos) {};
		}

		void XMPPWidget::checkPython_(String& text)
		{
			unescapeHTML(text);
			if (text.hasSubstring("<py>"))
			{
				String python_command = text.after("<py>");
				
				if (python_command.hasSubstring("</py>"))
				{
					python_command = python_command.before("</py>");

					PyWidget::getInstance(0)->runString(python_command);
				}
			}
		}

		LmHandlerResult XMPPWidget::handleMessages_(LmMessageHandler* handler, LmConnection *connection, LmMessage *m, gpointer user_data)
		{
			LmMessageNode* body = lm_message_node_get_child(m->node, "body");

			String to_insert = String("<")+lm_message_node_get_attribute(m->node, "from") + "> ";
			if (body)
			{
				to_insert += String(lm_message_node_to_string(body));
				to_insert.substitute("<body>", "");
				to_insert.substitute("</body>", "");

				checkPython_(to_insert);

				XMPPWidget::instance->history->append(QString(to_insert.c_str()));
			}

		}
	}
}
