class Client
{
%TypeHeaderCode
	#include <BALL/CONCEPT/client.h>
%End
	public:

	class InvalidClient
		: GeneralException
	{
		public:
		InvalidClient(const char*, int) throw();
	};

	class NoPersistentObject
		: GeneralException
	{
		public:
		NoPersistentObject(const char*, int) throw();
	};

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
};
