// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//


#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/SYSTEM/networking.h>
#include <BALL/SYSTEM/systemCalls.h> // for sleepFor()

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

START_TEST(Socket)
using namespace BALL;

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
	  
CHECK([EXTRA] simple asynchronous socket transmission to/from threaded server)
	SimpleTCPServerTestThread server(0, true, false);
	server.start();

	Size port = server.getPort();

	// starting up can take a few moments, so we wait up to 1 second
	Size retries = 0;
	while ((port == 0) && (retries < 10))
	{
		sleepFor(100);
		port = server.getPort();
		++retries;
	}

	TEST_NOT_EQUAL(port, 0);
	if (port > 0)
	{
		String sent("Hello, world!");

		TCPIOStream stream("localhost", port);
		stream << sent << std::endl;

		String received;
		stream >> received;

		sleepFor(10);
		TEST_EQUAL(sent, server.received);
		TEST_EQUAL(received, server.sent_async);	
	}
	server.deactivate();
	server.terminate();
	server.wait();
RESULT
	
CHECK([EXTRA] simple synchronous socket transmission to/from threaded server)
	SimpleTCPServerTestThread server(0, false, false);
	server.start();

	Size port = server.getPort();

	// starting up can take a few moments, so we wait up to 1 second
	Size retries = 0;
	while ((port == 0) && (retries < 10))
	{
		sleepFor(100);
		port = server.getPort();
		++retries;
	}

	TEST_NOT_EQUAL(port, 0);
	if (port > 0)
	{
		String sent("Hello, world!");

		TCPIOStream stream("localhost", port);
		stream << sent << std::endl;

		String received;
		stream >> received;
		sleepFor(10);
		TEST_EQUAL(sent, server.received);
		TEST_EQUAL(received, server.sent_sync);	
	}

	server.deactivate();
	server.terminate();
	server.wait();
RESULT
	

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
