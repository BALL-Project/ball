// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class Client
{
%TypeHeaderCode
	#include <BALL/CONCEPT/client.h>
	#include <BALL/COMMON/exception.h>

	using namespace BALL;
	using namespace BALL::Exception;
%End
	public:
	class InvalidClient
		: GeneralException
	{
%TypeHeaderCode
	#include <BALL/CONCEPT/client.h>
	using namespace BALL;
%End
		public:
		InvalidClient(const char*, int)
			throw();
	};
	class NoPersistentObject
		: GeneralException
	{
%TypeHeaderCode
	#include <BALL/CONCEPT/client.h>
	using namespace BALL;
%End
		public:
		NoPersistentObject(const char*, int)
			throw();
	};

	public:

	enum Command
	{
		COMMAND__UNKOWN              = 0,
		COMMAND__SEND_OBJECT         = 1,
		NUMBER_OF_COMMANDS
	};

	Client() throw();
	Client(const String&, int port = DEFAULT_PORT) throw();
  ~Client() throw();
	virtual void clear() throw();
	void connect(const String&, int port = DEFAULT_PORT) throw();
	void insert(Composite&) throw(GeneralException);
	virtual bool isValid() const throw();
	//virtual void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
};
