// $Id: client.C,v 1.9 2001/05/13 14:28:37 hekl Exp $

#include <BALL/VIEW/KERNEL/client.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		Client::InvalidClient::InvalidClient(const char* file, int line)
				throw()
			:	Exception::GeneralException(file, line, string("InvalidClient"), string("client not valid!"))
		{
		}

		Client::NoPersistentObject::NoPersistentObject(const char* file, int line)
				throw()
			:	Exception::GeneralException(file, line, string("NoPersistentObject"), string("object is not a persistent object!"))
		{
		}


		Client::Client()
			throw()
			:	host_(),
				port_(VIEW_DEFAULT_PORT),
				pm_()
		{
		}

		Client::Client(const String& host, int port)
			throw()
			:	pm_()
		{
			connect(host, port);
		}

		Client::~Client()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<Client>() << endl;
			#endif 

			destroy();
		}

		void Client::clear()
			throw()
		{
		}

		void Client::destroy()
			throw()
		{
		}

		void Client::connect(const String& host, int port)
			throw()
		{
			host_ = host;
			port_ = port;
		}

	  void Client::insert(Composite &composite)
			throw(InvalidClient, NoPersistentObject)
    {
			if (!isValid())
			{
				throw InvalidClient(__FILE__, __LINE__);
			}	
				
			if (!RTTI::isKindOf<PersistentObject>(composite))
			{
				throw NoPersistentObject(__FILE__, __LINE__);
			}

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
			throw()
		{
			return (host_ != "" && port_ != 0);
		}

		void Client::dump(ostream& s, Size depth) const
			throw()
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
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void Client::write(ostream & /* s */) const
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/client.iC>
#		endif 
		
	} // namespace VIEW

} // namespace BALL
