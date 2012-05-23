// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/KERNEL/serverWidget.h>
#include <BALL/CONCEPT/client.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/DIALOGS/preferences.h>
#include <BALL/VIEW/DIALOGS/serverPreferences.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/FORMAT/INIFile.h>
#include <BALL/SYSTEM/systemCalls.h>

#include <QtWidgets/QStatusBar>
#include <QtGui/QPixmap>
#include <QtWidgets/QToolTip>

using namespace std;

namespace BALL
{
	namespace VIEW
	{
		const char* ServerWidget::mini_ray_xpm_[] =
		{
        "16 14 4 1",
        "   c None",
        ".  c black",
        "X  c yellow",
        "o  c gray50",
        "     .........  ",
        "     .XXXXXX.o  ",
        "    .XXXXXX.o   ",
        "    .XXXXX.o    ",
        "   .XXXXX.o     ",
        "   .XXXX.....   ",
        "  .XXXXXXXX.o   ",
        "  .....XXX.o    ",
        "   oo.XXX.o     ",
        "     .XX.o      ",
        "    .XX.o       ",
        "    .X.o        ",
        "   .X.o         ",
        "   ..o          "
		};        
  
		ServerWidget::NotCompositeObject::NotCompositeObject(const char* file, int line)
			:	Exception::GeneralException(file, line, string("NotCompositeObject"), string("received an non composite object!"))
		{
		}

		ServerWidget::ServerWidget(QWidget* parent, const char* name)
			:	QObject(parent),
				ModularWidget(name),
				server_(this, VIEW_DEFAULT_PORT, true),
				object_creator_(0),
				port_(VIEW_DEFAULT_PORT),
				server_preferences_(0),
				server_icon_(0)
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "new ServerWidget " << this << std::endl;
			#endif

			// register ModularWidget
			registerWidget(this);
			
			unregisterObjectCreator();

			server_.setMainControl(getMainControl());

			connect(this, SIGNAL(lockRequested(bool)), this, SLOT(handleLocking(bool)));
		}

		ServerWidget::ServerWidget(const ServerWidget& /*s*/)
			: QObject(),
				ModularWidget(),
				server_(this, VIEW_DEFAULT_PORT, true)
		{
			// register ModularWidget
			registerWidget(this);
			
			unregisterObjectCreator();

			server_.setMainControl(getMainControl());
		}

		ServerWidget::~ServerWidget()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << this << " ServerWidget" << std::endl;
			#endif 

			server_.deactivate();
			server_.terminate();
			server_.wait();

			ConnectionObject::destroy();

			if (object_creator_ != 0) delete object_creator_;
		}

		void ServerWidget::clear()
		{
			ConnectionObject::clear();

			server_.deactivate();
		}

		int ServerWidget::getPort() const
		{
			return port_;
		}
		
		void ServerWidget::setPort(const int port)
		{
			port_ = port;
			server_.setPort(port_);
		}
		
		void ServerWidget::initializeWidget(MainControl& main_control)
		{
			server_icon_ = new QLabel(main_control.statusBar());
			main_control.statusBar()->addPermanentWidget(server_icon_);
//   			QToolTip::add(server_icon_, "VIEW server status");
			QPixmap icon(mini_ray_xpm_);

  		server_icon_->setFrameShape(QLabel::NoFrame);
			server_icon_->setPixmap(icon);
			server_icon_->setMaximumSize(14,20);
			server_icon_->setMinimumSize(14,20);
			server_icon_->show();
 		}

		void ServerWidget::finalizeWidget(MainControl& main_control)
		{
			main_control.statusBar()->removeWidget(server_icon_);	
			delete server_icon_;
		}
			
		void ServerWidget::initializePreferencesTab(Preferences &preferences)
		{
			server_preferences_ = new ServerPreferences();

			preferences.insertEntry(server_preferences_);
		}

		void ServerWidget::finalizePreferencesTab(Preferences &preferences)
		{
			if (server_preferences_ != 0)
			{
				preferences.removeEntry(server_preferences_);
				server_preferences_ = 0;
			}
		}
		
		void ServerWidget::applyPreferences()
		{
			if (server_preferences_ == 0) return;

			// get server mode
			if (server_preferences_->getServerStatus())
			{
				// retrieve the port number
				int port = server_preferences_->getPort();

				// set the port and active the server
				setPort(port);
				server_.start();

				if (server_.isRunning())
				{
					// adjust the tool tip and update the server icon
					QString tip;
					tip.sprintf("VIEW Server listening on port %d", port); 
					server_icon_->setToolTip(tip);
					server_icon_->show();
				}
			}
			else
			{
				// stop the server
				server_.deactivate();

				// hide the icon
				server_icon_->hide();
			}
		}

		bool ServerWidget::isValid() const
		{
			return (ConnectionObject::isValid());
		}

		void ServerWidget::dump(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			ConnectionObject::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void ServerWidget::changeLock(bool lock)
		{
			emit lockRequested(lock);
		}

		void ServerWidget::handleLocking(bool lock)
		{
			if (lock)
			{
				// try releasing a lock if we already hold one
				unlockComposites();
				if (lockComposites())
					server_.setLocked(true);
				server_.setLocked(true);
			}
			else
			{
				unlockComposites();
				server_.setLocked(false);
			}
		}

		ServerWidget::BALLViewServer::BALLViewServer(ServerWidget* parent, Size port, bool restart)
			: BALLThread(),
				TCPServerThread(port, true, restart),
				parent_widget_(parent)
		{
		}

		void ServerWidget::BALLViewServer::run()
		{
			try {
				activate();
			} catch (std::exception& e) {
				output_(e.what());
			}
		}

		void ServerWidget::BALLViewServer::handleConnection()
		{
			unsigned int command;

			// reading command
			connected_stream_ >> command;
			
			// process commands
			switch (command)
			{
				case Client::COMMAND__SEND_OBJECT:
					sendObject();
				break;

   			default:
					Log.info() << "Server: unkown command." << endl;
					connected_stream_.close();
					break;
			}
		}

	  void ServerWidget::BALLViewServer::sendObject()
				throw(ServerWidget::NotCompositeObject)
    {
			output_("Server: receiving object ... \n");

			unsigned long object_handle;
			
			// get object handle
			connected_stream_ >> object_handle;
			
			ObjectCreator* object_creator = &(parent_widget_->getObjectCreator());
			output_("creating object\n");

			// use specified object creator for inserting the object in the scene
			Composite* new_composite_ptr = object_creator->operator()(connected_stream_);
			if (new_composite_ptr == 0)
			{
				throw NotCompositeObject(__FILE__, __LINE__);
			}

			received_composite_ = new_composite_ptr;

			// get composite with handle
 			CompositeHashMap::Iterator iterator = composite_hashmap_.find(object_handle);

			is_locked_ = false;
			parent_widget_->changeLock(true);
			
			while (!is_locked_)
			{
				// give the GUI thread time to update widgets, etc., and try
				// again in 2 seconds
				msleep(2000);
				parent_widget_->changeLock(true);
			}

		 	// already in hashmap ?
			if (iterator != composite_hashmap_.end())
			{
				// tell the main control to get rid of it
				sendMessage_(new CompositeMessage(*iterator->second,
																					CompositeMessage::REMOVED_COMPOSITE,
																					true));

				// remove old composite from hashmap
				composite_hashmap_.erase(object_handle);
			}

			// insert new composite 
			composite_hashmap_.insert(CompositeHashMap::ValueType(object_handle, new_composite_ptr));
			sendMessage_(new CompositeMessage(*new_composite_ptr,
																			 CompositeMessage::NEW_COMPOSITE,
																			 true));

			parent_widget_->changeLock(false);
    }

		void ServerWidget::BALLViewServer::setLocked(bool is_locked)
		{
			is_locked_ = is_locked;
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/serverWidget.iC>
#		endif 
	} 
} // namespaces
