// $Id: client.C,v 1.8 2001/02/04 16:14:27 hekl Exp $

#include <BALL/VIEW/KERNEL/client.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{


		Client::Client()
			:	host_(),
				port_(VIEW_DEFAULT_PORT),
				pm_()
		{
		}

		Client::Client(const String& host, int port)
			:	pm_()
		{
			connect(host, port);
		}

		Client::~Client()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<Client>() << endl;
			#endif 

			destroy();
		}

		void Client::clear()
		{
		}

		void Client::destroy()
		{
		}

		void Client::connect(const String& host, int port)
		{
			host_ = host;
			port_ = port;
		}

	  void Client::insert(Composite &composite)
    {
			#ifdef BALL_VIEW_DEBUG
					
			if (!isValid())
			{
				throw InvalidClient(__FILE__, __LINE__);
			}	
				
			if (!RTTI::isKindOf<PersistentObject*>(&composite))
			{
				throw NoPersistentObject(__FILE__, __LINE__);
			}

			#endif

			IOStreamSocket iostream_socket;	
			iostream_socket->connect(host_.c_str(), port_);

			iostream_socket << (int)COMMAND__SEND_OBJECT << " ";
			iostream_socket << (unsigned long)(&composite) << endl;

			pm_.setOstream(iostream_socket);
			composite >> pm_;

			iostream_socket->close();
    }

		/*
		void Client::setCreatorValue(int address, int value)
    {
			#ifdef BALL_VIEW_DEBUG
					
			if (!isValid())
			{
				throw InvalidClient(__FILE__, __LINE__);
			}
				
			#endif

			IOStreamSocket iostream_socket;	
			iostream_socket->connect(host_.c_str(), port_);

			iostream_socket << (int)COMMAND__SET_CREATOR_VALUE << " ";
			iostream_socket << address << " ";
			iostream_socket << value << endl;
		
			iostream_socket->close();
		}

		int Client::getCreatorValue(int address)
    {
			#ifdef BALL_VIEW_DEBUG
					
			if (!isValid())
			{
				throw InvalidClient(__FILE__, __LINE__);
			}
				
			#endif

			IOStreamSocket iostream_socket;	
			iostream_socket->connect(host_.c_str(), port_);

			iostream_socket << (int)COMMAND__GET_CREATOR_VALUE << " ";
			iostream_socket << address << endl;

			int value = -1;

			iostream_socket >> value;

			iostream_socket->close();

			return value;
		}

		bool Client::hasCreatorValue(int address, int value)
    {
			#ifdef BALL_VIEW_DEBUG
					
			if (!isValid())
			{
				throw InvalidClient(__FILE__, __LINE__);
			}
				
			#endif

			IOStreamSocket iostream_socket;	
			iostream_socket->connect(host_.c_str(), port_);

			iostream_socket << (int)COMMAND__HAS_CREATOR_VALUE << " ";
			iostream_socket << address << " ";
			iostream_socket << value << endl;

			bool has_value = false;

			iostream_socket >> has_value;

			iostream_socket->close();

			return has_value;
		}
		*/

		bool Client::isValid() const
		{
			return (host_ != "" && port_ != 0);
		}

		void Client::dump(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "host: " << host_ << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "port: " << port_ << endl;
			
			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void Client::read(istream & /* s */)
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void Client::write(ostream & /* s */) const
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/client.iC>
#		endif 
		
	} // namespace VIEW

} // namespace BALL
