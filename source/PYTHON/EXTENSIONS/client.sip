// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: client.sip,v 1.2 2003/08/26 09:18:14 oliver Exp $
//

class Client
{
%HeaderCode
	#include <BALL/VIEW/KERNEL/client.h>
	using namespace BALL;
	using namespace VIEW;
%End
	public:
	class InvalidClient
		: GeneralException
	{
%HeaderCode
	#include <BALL/VIEW/KERNEL/client.h>
	using namespace BALL;
	using namespace VIEW;
%End
		public:
		InvalidClient(const char* /file/, int /line/)
			throw();
	};
	class NoPersistentObject
		: GeneralException
	{
%HeaderCode
	#include <BALL/VIEW/KERNEL/client.h>
	using namespace BALL;
	using namespace VIEW;
%End
		public:
		NoPersistentObject(const char* /file/, int /line/)
			throw();
	};

	public:
	Client() throw();
	Client(const String& /host/, int /port/ = VIEW_DEFAULT_PORT) throw();

	~Client() throw();
	virtual void clear() throw();
	virtual void destroy() throw();

	void connect(const String& /host/, int /port/ = VIEW_DEFAULT_PORT) throw();

	void insert(Composite& /composite/)
					throw(GeneralException);

	virtual bool isValid() const
					throw();
	//virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
	//				throw();
	//virtual void read(std::istream& s)
	//				throw();
	//virtual void write(std::ostream& s) const
	//				throw();
};
