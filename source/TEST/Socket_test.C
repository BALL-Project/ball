// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: Socket_test.C,v 1.6 2002/02/27 12:24:56 sturm Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/SYSTEM/socket.h>
#include <unistd.h>
///////////////////////////

START_TEST(Socket, "$Id: Socket_test.C,v 1.6 2002/02/27 12:24:56 sturm Exp $")
using namespace BALL;


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////


CHECK(simple socket transmission)
	SockInetBuf sock_inet_buf(SocketBuf::sock_stream);
	sock_inet_buf.bind();
	char c;
	int port = sock_inet_buf.localport();
	STATUS("localport = " << sock_inet_buf.localport())
	STATUS("localhost = " << sock_inet_buf.localhost())

	if (fork())
	{	
		STATUS("A:listen...")
		sock_inet_buf.listen();
		STATUS("A:accept...")
		IOStreamSocket  s(sock_inet_buf.accept());
		STATUS("A:get...")
		s.get(c);
		s.get(c);
		STATUS("A:done")
	} 
	else
	{
		sleep(1);
		IOStreamSocket  sio(SocketBuf::sock_stream);
		int result = sio->connect(sock_inet_buf.localhost(), port);
		STATUS("B:connect = " << result)
		sio.put((char)123);
		STATUS("B:done.")
		return 0;
	}

	TEST_EQUAL((int)c, (int)123);
RESULT
	

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
