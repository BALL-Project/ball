// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: client.sip,v 1.5.20.1 2007/03/25 21:37:04 oliver Exp $
//

class Client
{
%TypeHeaderCode
	#include <BALL/CONCEPT/client.h>
	using namespace BALL;
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
		InvalidClient(const char* /file/, int /line/)
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
		NoPersistentObject(const char* /file/, int /line/)
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
	Client(const String& /host/, int /port/ = VIEW_DEFAULT_PORT) throw();
  ~Client() throw();
	virtual void clear() throw();
	void connect(const String& /host/, int /port/ = VIEW_DEFAULT_PORT) throw();
	void insert(Composite& /composite/) throw(GeneralException);
	virtual bool isValid() const throw();
	//virtual void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
};
