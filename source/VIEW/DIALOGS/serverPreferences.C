// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/DIALOGS/serverPreferences.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/COMMON/rtti.h>

#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

namespace BALL
{
	namespace VIEW
	{

		ServerPreferences::ServerPreferences(QWidget* parent, const char* name)
			: QWidget(parent),
				PreferencesEntry()
		{
			setObjectName(name);
			QLabel* port_label = new QLabel(this);
			port_label->setGeometry(30, 20, 100, 27);
			port_label->setFocusPolicy(Qt::NoFocus);
			port_label->setFrameStyle(0);
			port_label->setLineWidth(1);
			port_label->setMidLineWidth(0);
			port_label->setText(tr("server port"));
			port_label->setMargin(0);
			
			port_ = new QLineEdit(this);
			port_->setObjectName("port");
			port_->setGeometry(180, 20, 90, 30);
			port_->setFocusPolicy(Qt::StrongFocus);
			port_->setMaxLength(5);
			port_->setFrame(QLineEdit::Normal);
			port_->setFrame(true);
			port_->setAlignment(Qt::AlignRight);
			
			server_status_ = new QCheckBox(this);
			server_status_->setObjectName("server_status");
			server_status_->setGeometry(40, 80, 180, 21);
			server_status_->setFocusPolicy(Qt::TabFocus);
			server_status_->setText(tr("accept clients"));
			server_status_->setAutoRepeat(false);
			
			resize(380,210);
			setMinimumSize(0, 0);
			setMaximumSize(32767, 32767);

			port_->setText(String(VIEW_DEFAULT_PORT).c_str());
			server_status_->setChecked(false);
			
			setINIFileSectionName("NETWORK");
			setWidgetStackName((String)tr("Client/Server"));
			registerWidgets_();
		}

		ServerPreferences::~ServerPreferences()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << (void *)this 
				 					 << " of class ServerPreferences" << std::endl;
			#endif 
		}
		

		int ServerPreferences::getPort()
		{
			return ascii(port_->text()).toInt();
		}
		
		bool ServerPreferences::getServerStatus()
		{
			return server_status_->isChecked();
		}

	} // namespace VIEW
} // namespace BALL
