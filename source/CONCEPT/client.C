// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: client.C,v 1.1.4.1 2007/03/25 22:00:05 oliver Exp $
//

#include <BALL/CONCEPT/client.h>

using namespace std;

namespace BALL
{

	Client::InvalidClient::InvalidClient(const char* file, int line)
		:	Exception::GeneralException(file, line, string("InvalidClient"), string("client not valid!"))
	{
	}

	Client::NoPersistentObject::NoPersistentObject(const char* file, int line)
		:	Exception::GeneralException(file, line, string("NoPersistentObject"), string("object is not a persistent object!"))
	{
	}


	Client::Client()
		:	host_(),
			port_(20000),
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
		#ifdef BALL_DEBUG
			cout << "Destructing object " << (void *)this 
				<< " of class " << RTTI::getName<Client>() << endl;
		#endif 
	}

	void Client::clear()
	{
	}

	void Client::connect(const String& host, int port)
	{
		host_ = host;
		port_ = port;
	}

	void Client::insert(Composite &composite)
	{
		if (!isValid())
		{
			throw InvalidClient(__FILE__, __LINE__);
		}	
			
		if (!RTTI::isKindOf<PersistentObject>(composite))
		{
			throw NoPersistentObject(__FILE__, __LINE__);
		}

		TCPIOStream tcp_iostream(host_.c_str(), port_);	

		tcp_iostream << (Size)COMMAND__SEND_OBJECT << " ";
		tcp_iostream << static_cast<LongSize>(reinterpret_cast<PointerSizeUInt>(&composite)) << endl;

		pm_.setOstream(tcp_iostream);
		composite >> pm_;

		tcp_iostream.close();
	}

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

} // namespace BALL
