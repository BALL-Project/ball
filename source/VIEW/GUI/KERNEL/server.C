// $Id: server.C,v 1.1 2000/09/23 13:28:35 hekl Exp $

#include <BALL/VIEW/GUI/KERNEL/server.h>

#include <BALL/COMMON/logStream.h>


using namespace std;

namespace BALL
{
  
	namespace VIEW
	{
  
		Server::Server()
			:	QTTimer(),
				ConnectionObject(),
				object_creator_(0),
				composite_hashmap_(),
				iostream_socket_(0),
				sock_inet_buf_(0),
				port_(VIEW_DEFAULT_PORT)
		{
			deregisterObjectCreator();
		}

		Server::Server(const Server& server, bool deep)
			:	QTTimer(server, deep),
				ConnectionObject(server, deep),
				object_creator_(server.object_creator_),
				composite_hashmap_(),
				iostream_socket_(0),
				sock_inet_buf_(0),
				port_(server.port_)
		{
		}

		Server::~Server()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<Server>() << endl;
			#endif 

			clear();
		}

		void Server::clear()
		{
			QTTimer::clear();
			ConnectionObject::clear();
		}

		// initializes a new socket and starts the timer
		void Server::activate()
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
			setIntervall(1000);
			startTimer();
		}

		void Server::deactivate()
		{
			if (isTimerEnabled())
			{
				Log.info() << "VIEW::Server: stopped." << endl;
				stopTimer();
				sock_inet_buf_->close();
			}
		}
			
		int Server::getPort() const
		{
			return port_;
		}
		
		void Server::setPort(const int port)
		{
			port_ = port;
		}
		
		bool Server::isValid() const
		{
			return (bool)(QTTimer::isValid() == true
										&& ConnectionObject::isValid() == true);
		}

		void Server::dump
			(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			QTTimer::dump(s, depth + 1);
			ConnectionObject::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void Server::read(istream & /* s */)
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void Server::write(ostream & /* s */) const
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

  			case COMMAND__SET_CREATOR_VALUE:
					setCreatorValue(iostream_socket);
				break;

  			case COMMAND__GET_CREATOR_VALUE:
					getCreatorValue(iostream_socket);
				break;

  			case COMMAND__HAS_CREATOR_VALUE:
					hasCreatorValue(iostream_socket);
				break;

   			default:
					Log.info() << "Server: unkown command." << endl;
					break;
			}
		}

	  void Server::sendObject(IOStreamSocket &iostream_socket)
    {
			Log.info() << "Server: receiving object ... " << endl;

			unsigned long object_handle;
			
			// get object handle
			iostream_socket >> object_handle;
			
			Composite *new_composite_ptr = 0;

			Log.info() << "creating object: object_creator_ = " << object_creator_ << endl;

			// use specified object creator for inserting the object in the scene
			new_composite_ptr = object_creator_->operator()(iostream_socket);

			#ifdef BALL_VIEW_DEBUG
					
			if (!isValid())
			{
				throw CompositePointerIsNull(__FILE__, __LINE__);
			}
				
			#endif	

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
    }

	  void Server::setCreatorValue(IOStreamSocket &iostream_socket)
    {
			Log.info() << "Server: setting ObjectCreator value... " << flush;
			
			int address;
			int value;
			
			iostream_socket >> address;
			iostream_socket >> value;
			
			object_creator_->setValue(address, value);
			
			Log.info() << "finished." << endl;
    }
 
	  void Server::getCreatorValue(IOStreamSocket &iostream_socket)
    {
			Log.info() << "Server: getting ObjectCreator value... " << flush;

			int value;
			int address;
			
			iostream_socket >> address;
			
			value = object_creator_->getValue(address);

			iostream_socket << value << endl;
			
			Log.info() << "finished." << endl;
		}

	  void Server::hasCreatorValue(IOStreamSocket &iostream_socket)
    {
			Log.info() << "Server: has ObjectCreator value... " << flush;

			int value;
			int address;
			bool has_value;
			
			iostream_socket >> address;
			iostream_socket >> value;
			
			has_value = object_creator_->hasValue(address, value);

			iostream_socket << has_value << endl;
			
			Log.info() << "finished." << endl;
		}


#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/KERNEL/server.iC>
#		endif 

	} // namespace VIEW

} // namespace BALL
