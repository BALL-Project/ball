// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: client.C,v 1.17 2005/02/06 09:45:01 oliver Exp $
//

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
		}

		void Client::clear()
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
			throw(Client::InvalidClient, Client::NoPersistentObject)
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

			iostream_socket << (Size)COMMAND__SEND_OBJECT << " ";
			iostream_socket << static_cast<LongSize>(reinterpret_cast<PointerSizeUInt>(&composite)) << endl;

			pm_.setOstream(iostream_socket);
			composite >> pm_;

			iostream_socket->close();
    }

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

	} // namespace VIEW

} // namespace BALL
