// $Id: TCPTransfer_test.C,v 1.2 2001/09/10 00:04:21 amoll Exp $

#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/SYSTEM/TCPTransfer.h>
///////////////////////////

START_TEST(TCPTransfer, "$Id: TCPTransfer_test.C,v 1.2 2001/09/10 00:04:21 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

TCPTransfer* tcp_ptr;
CHECK(cstr)
	tcp_ptr = new TCPTransfer;
	TEST_NOT_EQUAL(tcp_ptr, 0)
	TEST_EQUAL(tcp_ptr->getStatusCode(), TCPTransfer::UNINITIALIZED_ERROR)
RESULT

CHECK(~TCPTransfer_test)
	delete tcp_ptr;
RESULT

CHECK(http)
  ofstream out("output", ios::out);
	TCPTransfer tcp_t(out ,"http://tucows.uni-erlangen.de:80/index.html");
	TEST_EQUAL(tcp_t.getHostAddress(), "tucows.uni-erlangen.de")
	TEST_EQUAL(tcp_t.getFileAddress(), "/index.html")
	TEST_EQUAL(tcp_t.getPort(), 80)
	TEST_EQUAL(tcp_t.getStatusCode(), TCPTransfer::NO_ERROR)
	TEST_EQUAL(tcp_t.getReceivedBytes(), 2238)
	out.close();
RESULT
	
	//TCPTransfer tcp_t(out, "http://postino.mpi-sb.mpg.de/index.html");
	//TCPTransfer tcp_t(out, "http://www.winmx.com/index.html");
	//TCPTransfer tcp_t(out, "http://download.nullsoft.com/winamp/client/winamp276_full.exe");
	//TCPTransfer tcp_t(out, "ftp://ftp.gwdg.de:21/pub/newfiles.gz");
	//TCPTransfer tcp_t(out, "ftp://ftp.uni-mainz.de/pub/misc/.cache");
	//TCPTransfer tcp_t(out, "ftp://ftp.gwdg.de:21/pub/mpg/service/lt.db.txt");

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
