// $Id: server.C,v 1.6 2001/05/13 16:29:09 hekl Exp $

#include <BALL/VIEW/GUI/KERNEL/server.h>
#include <BALL/COMMON/logStream.h>

#include <qstatusbar.h>
#include <qpixmap.h>
#include <qtooltip.h>
#include <qstring.h>

using namespace std;

namespace BALL
{
  
	namespace VIEW
	{
		const char* Server::mini_ray_xpm_[] =
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
  
		Server::NotCompositeObject::NotCompositeObject(const char* file, int line)
			throw()
			:	Exception::GeneralException(file, line, string("NotCompositeObject"), string("received an non composite object!"))
		{
		}

		Server::Server(QWidget* parent, const char* name)
				throw()
			:	QTTimer(parent, name),
				ModularWidget(name),
				object_creator_(0),
				composite_hashmap_(),
				iostream_socket_(0),
				sock_inet_buf_(0),
				port_(VIEW_DEFAULT_PORT),
				server_preferences_(0),
				server_icon_(0)
		{
			// register ModularWidget
			registerWidget(this);
			
			unregisterObjectCreator();
		}

		Server::~Server()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<Server>() << endl;
			#endif 

			destroy();
		}

		void Server::clear()
			throw()
		{
			QTTimer::clear();
			ConnectionObject::clear();
		}

		void Server::destroy()
			throw()
		{
			QTTimer::destroy();
			ConnectionObject::destroy();
		}

		// initializes a new socket and starts the timer
		void Server::activate()
				throw()
		{
			static SockInetBuf sock_inet_buf(SocketBuf::sock_stream);
			
			// if the timer is already running, clear it and close the
			// socket first
			if (isTimerEnabled())
			{
				stopTimer();
				sock_inet_buf.close();
			}
			
			if (sock_inet_buf.bind(0, port_) != 0)
			{
				Log.error() << "VIEW::Server: cannot bind to port " << port_ << endl;
				return;
			}

			sock_inet_buf_ = &sock_inet_buf;

			Log.info() << "VIEW::Server: listening at port " 
								 << sock_inet_buf_->localhost() << ":"
								 << sock_inet_buf_->localport() << endl;

			sock_inet_buf_->listen();

			// check once per second
			setInterval(1000);
			startTimer();
		}

		void Server::deactivate()
				throw()
		{
			if (isTimerEnabled())
			{
				Log.info() << "VIEW::Server: stopped." << endl;
				stopTimer();
				sock_inet_buf_->close();
			}
		}
			
		int Server::getPort() const
				throw()
		{
			return port_;
		}
		
		void Server::setPort(const int port)
				throw()
		{
			port_ = port;
		}
		
		void Server::initializeWidget(MainControl& main_control)
				throw()
		{
			server_icon_ = new QLabel(main_control.statusBar());
			main_control.statusBar()->addWidget(server_icon_, 1, TRUE );
			QToolTip::add(server_icon_, "VIEW server status");
			QPixmap icon(mini_ray_xpm_);

			server_icon_->setPixmap(icon);
			server_icon_->show();
 		}

		void Server::finalizeWidget(MainControl& main_control)
				throw()
		{
			main_control.statusBar()->removeWidget(server_icon_);	
			delete server_icon_;
		}
			
		void Server::checkMenu(MainControl& /* main_control */)
				throw()
		{
		}

		void Server::initializePreferencesTab(Preferences &preferences)
				throw()
		{
			server_preferences_ = new ServerPreferences();
			CHECK_PTR(server_preferences_);

			preferences.insertTab(server_preferences_, "Server");
		}

		void Server::finalizePreferencesTab(Preferences &preferences)
				throw()
		{
			if (server_preferences_ != 0)
			{
				preferences.removeTab(server_preferences_);
		
				delete server_preferences_;
				server_preferences_ = 0;
			}
		}
		
		void Server::applyPreferences(Preferences & /* preferences */)
				throw()
		{
			if (server_preferences_ != 0)
			{
				// get server mode
				bool start_server = server_preferences_->getServerStatus();
		
				if (start_server)
				{
					// retrieve the port number
					int port = server_preferences_->getPort();

					// set the port and active the server
					setPort(port);
					activate();
	
					// adjust the tool tip and update the server icon
					QString tip;
					tip.sprintf("VIEW Server listening on port %d", port); 
					QToolTip::add(server_icon_, tip);
					server_icon_->show();
				}
				else
				{
					// stop the server
					deactivate();

					// hide the icon
					server_icon_->hide();
					QToolTip::add(server_icon_, "VIEW Server disabled");
				}

				// notify the main window
				WindowMessage *window_message_2 = new WindowMessage;
				window_message_2->setStatusBar("");
				window_message_2->setDeletable(true);
				notify_(window_message_2);
 			}
		}
		
		void Server::fetchPreferences(INIFile &inifile)
				throw()
		{
			// the default preferences tab (if existent)
			if (server_preferences_ != 0)
			{
				server_preferences_->fetchPreferences(inifile);
			}
		}
		
		void Server::writePreferences(INIFile &inifile)
				throw()
		{
			// the default preferences tab (if existent)
			if (server_preferences_ != 0)
			{
				server_preferences_->writePreferences(inifile);
			}
		}

		bool Server::isValid() const
			throw()
		{
			return (ConnectionObject::isValid());
		}

		void Server::dump(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			QTTimer::dump(s, depth + 1);
			ConnectionObject::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void Server::read(istream & /* s */)
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void Server::write(ostream & /* s */) const
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		// main event loop
	  void Server::timer()
		{
			unsigned int command;

			// wait until socket is read ready
			if (!sock_inet_buf_->is_readready(0,20))
			{
				return;
			}

			// open stream socket
			IOStreamSocket iostream_socket(sock_inet_buf_->accept());

			// reading command
			iostream_socket >> command;
			
			// process commands
			switch (command)
			{
  			case COMMAND__SEND_OBJECT:
					sendObject(iostream_socket);
				break;

   			default:
					Log.info() << "Server: unkown command." << endl;
					break;
			}
		}

	  void Server::sendObject(IOStreamSocket &iostream_socket)
				throw(NotCompositeObject)
    {
			Log.info() << "Server: receiving object ... " << endl;

			unsigned long object_handle;
			
			// get object handle
			iostream_socket >> object_handle;
			
			Composite *new_composite_ptr = 0;

			Log.info() << "creating object: object_creator_ = " << object_creator_ << endl;

			// use specified object creator for inserting the object in the scene
			new_composite_ptr = object_creator_->operator()(iostream_socket);

			if (new_composite_ptr == 0)
			{
				throw NotCompositeObject(__FILE__, __LINE__);
			}

			received_composite_ = new_composite_ptr;

			// get composite with handle
 			CompositeHashMap::Iterator iterator = composite_hashmap_.find(object_handle);

	 		Composite* inserted_composite_ptr = 0;

		 	// already in hashmap ?
			if (iterator != composite_hashmap_.end())
			{
			 	// get it
				inserted_composite_ptr = iterator->second;
 			}
			
			// composite already exists ?
			if (inserted_composite_ptr != 0)
			{
				try
				{
					// remove old composite
					RemovedCompositeMessage message;
					message.setComposite(*inserted_composite_ptr);
					notify_(message);
					
					// remove composite from hashmap
					composite_hashmap_.erase(object_handle);
					
					// delete old composite
					delete inserted_composite_ptr;
				}
				catch (...)
				{
					Log.info() << "> Server: error deleting old composite!" << endl;
				}
			}
			/*
			else // composite is new
 			{
	 			// insert into hashmap
		 		composite_hashmap_.
					insert(CompositeHashMap::ValueType(object_handle, new_composite_ptr));
 			
				// notify main window
				NewCompositeMessage new_message;
				new_message.setComposite(*new_composite_ptr);
				
				notify_(new_message);
			}		
			*/
			
			// insert into hashmap
			composite_hashmap_.
				insert(CompositeHashMap::ValueType(object_handle, new_composite_ptr));
 			
			// notify main window
			NewCompositeMessage new_message;
			new_message.setComposite(*new_composite_ptr);
			
			notify_(new_message);
    }


#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/KERNEL/server.iC>
#		endif 

	} // namespace VIEW

} // namespace BALL
