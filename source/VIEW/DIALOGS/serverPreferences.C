// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: serverPreferences.C,v 1.5 2004/07/27 13:01:27 amoll Exp $
//

#include <BALL/VIEW/DIALOGS/serverPreferences.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/FORMAT/INIFile.h>
#include <BALL/COMMON/rtti.h>

#include <qlabel.h>
#include <qpushbutton.h>

namespace BALL
{
	namespace VIEW
	{

		ServerPreferences::ServerPreferences(QWidget* parent, const char* name)
			throw()
			: QWidget(parent, name, 0)
		{
			QLabel* port_label = new QLabel(this, "Label_1");
			port_label->setGeometry(30, 20, 73, 27);
			port_label->setMinimumSize(0, 0);
			port_label->setMaximumSize(32767, 32767);
			port_label->setFocusPolicy(QWidget::NoFocus);
			port_label->setBackgroundMode(QWidget::PaletteBackground);
			port_label->setFrameStyle(0);
			port_label->setLineWidth(1);
			port_label->setMidLineWidth(0);
			port_label->QFrame::setMargin(0);
			port_label->setText("server port");
			port_label->setAlignment( AlignLeft|AlignVCenter|ExpandTabs );
			port_label->setMargin(0);
			
			port_ = new QLineEdit(this, "LineEdit_1");
			port_->setGeometry(120, 20, 50, 30);
			port_->setMinimumSize(0, 0);
			port_->setMaximumSize(32767, 32767);
			port_->setFocusPolicy(QWidget::StrongFocus);
			port_->setBackgroundMode(QWidget::PaletteBase);
			port_->setMaxLength( 5 );
			port_->setFrame( QLineEdit::Normal );
			port_->setFrame( TRUE );
			port_->setAlignment( AlignRight );
			
			server_status_ = new QCheckBox(this, "CheckBox_1");
			server_status_->setGeometry(40, 80, 118, 21);
			server_status_->setMinimumSize(0, 0);
			server_status_->setMaximumSize(32767, 32767);
			server_status_->setFocusPolicy(QWidget::TabFocus);
			server_status_->setBackgroundMode(QWidget::PaletteBackground);
			server_status_->setText( tr( "accept clients" ) );
			server_status_->setAutoRepeat( FALSE );
			server_status_->setAutoResize( TRUE );
			
			resize(380,210);
			setMinimumSize(0, 0);
			setMaximumSize(32767, 32767);
			setDefaultValues();
		}

		ServerPreferences::~ServerPreferences()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << (void *)this 
				 					 << " of class " << RTTI::getName<ServerPreferences>() << std::endl;
			#endif 
		}
		
		void ServerPreferences::writePreferences(INIFile& inifile)
			throw()
		{
			// retrieve the network settings from the dialog
			bool server_status = server_status_->isChecked();
			String port = String(port_->text().ascii());
			
			inifile.appendSection("NETWORK");

			// save them to the inifile
			if (server_status)
			{
				inifile.insertValue("NETWORK", "start_server", "true");
			} else {
				inifile.insertValue("NETWORK", "start_server", "false");
			}
			inifile.insertValue("NETWORK", "server_port", port);
		}

		void ServerPreferences::fetchPreferences(INIFile& inifile)
			throw()
		{
			bool server_status = false;
			int port = VIEW_DEFAULT_PORT;
			if (inifile.hasEntry("NETWORK", "start_server"))
			{
				server_status = (inifile.getValue("NETWORK", "start_server") == "true");
			}
			if (inifile.hasEntry("NETWORK", "server_port"))
			{
				port = inifile.getValue("NETWORK", "server_port").toInt();
			}
			
			// set the values of the widgets
			server_status_->setChecked(server_status);
			port_->setText(String(port).c_str());
		}
		
		int ServerPreferences::getPort()
			throw()
		{
			return String(port_->text().ascii()).toInt();
		}
		
		bool ServerPreferences::getServerStatus()
			throw()
		{
			return server_status_->isChecked();
		}

		void ServerPreferences::setDefaultValues()
			throw()
		{
			port_->setText(String(VIEW_DEFAULT_PORT).c_str());
			server_status_->setChecked(false);
		}

	} // namespace VIEW
} // namespace BALL
