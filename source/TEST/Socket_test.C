// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: Socket_test.C,v 1.7 2002/12/12 11:34:44 oliver Exp $

#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/SYSTEM/socket.h>
#include <unistd.h>

#ifdef BALL_COMPILER_MSVC
#include<windows.h>
#include<process.h>
#endif

///////////////////////////
using namespace BALL;

SockInetBuf sock_inet_buf(SocketBuf::sock_stream);
char c;
void socket_listener(void*)
{
		sock_inet_buf.listen();
		IOStreamSocket  s(sock_inet_buf.accept());
		s.get(c);
		s.get(c);
}

START_TEST(Socket, "$Id: Socket_test.C,v 1.7 2002/12/12 11:34:44 oliver Exp $")
using namespace BALL;

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
	  
CHECK(simple socket transmission)
	
	sock_inet_buf.bind();
	int port = sock_inet_buf.localport();
	STATUS("localport = " << sock_inet_buf.localport())
	STATUS("localhost = " << sock_inet_buf.localhost())

#ifdef BALL_COMPILER_MSVC
	_beginthread(socket_listener,0,NULL);
	Sleep(1);
	IOStreamSocket  sio(SocketBuf::sock_stream);
	int result = sio->connect(sock_inet_buf.localhost(), port);
	STATUS("B:connect = " << result)
	sio.put((char)123);
	STATUS("B:done.")
#else
	if (fork())
	{	
		socket_listener(0);
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
#endif

	TEST_EQUAL((int)c, (int)123);
RESULT
	

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
