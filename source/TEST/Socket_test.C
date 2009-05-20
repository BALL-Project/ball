// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//


#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/SYSTEM/networking.h>

///////////////////////////

using namespace BALL;

class SimpleTCPServerTestThread
	: public TCPServerThread
{
	public:
		SimpleTCPServerTestThread(Size port, bool async, bool restart)
			: TCPServerThread(port, async, restart),
				sent_async("HelloAsync!"),
				sent_sync("HelloSync!")
		{}

		void handleConnection() 
		{
			getline(connected_stream_, received);

			connected_stream_ << sent_sync << std::endl;
		}

		void handleAsyncConnection() 
		{
			getline(connected_stream_, received);

			connected_stream_ << sent_async << std::endl;
		}

		String received;
		String sent_async;
		String sent_sync;
};

START_TEST(Socket, "")
using namespace BALL;

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
	  
CHECK([EXTRA] simple asynchronous socket transmission to/from threaded server)
	SimpleTCPServerTestThread server(0, true, false);
	server.start();

	Size port = server.getPort();
	ABORT_IF(port <= 0)

	String sent("Hello, world!");

	TCPIOStream stream("localhost", port);
	stream << sent << std::endl;
	TEST_EQUAL(sent, server.received);

	String received;
	stream >> received;
	TEST_EQUAL(received, server.sent_async);	
RESULT
	
CHECK([EXTRA] simple synchronous socket transmission to/from threaded server)
	SimpleTCPServerTestThread server(0, false, false);
	server.start();

	Size port = server.getPort();
	ABORT_IF(port <= 0)

	String sent("Hello, world!");

	TCPIOStream stream("localhost", port);
	stream << sent << std::endl;
	TEST_EQUAL(sent, server.received);

	String received;
	stream >> received;
	TEST_EQUAL(received, server.sent_sync);	
RESULT
	

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
