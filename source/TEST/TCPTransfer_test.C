// $Id: TCPTransfer_test.C,v 1.11 2002/01/03 01:24:42 oliver Exp $

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/SYSTEM/TCPTransfer.h>
#include <BALL/SYSTEM/file.h>

///////////////////////////

using namespace BALL;
using namespace std;

// include a helper class testing for network availability
// (or more precisely: the ability to perform HTTP or FTP transfers)

#include "networkTest.h"

START_TEST(TCPTransfer, "$Id: TCPTransfer_test.C,v 1.11 2002/01/03 01:24:42 oliver Exp $")


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

TCPTransfer* tcp_ptr;
CHECK(cstr)
	tcp_ptr = new TCPTransfer;
	TEST_NOT_EQUAL(tcp_ptr, 0)
	TEST_EQUAL(tcp_ptr->getStatusCode(), TCPTransfer::UNINITIALIZED_ERROR)
RESULT

CHECK(~TCPTransfer_test)
	delete tcp_ptr;
RESULT

CHECK(set(ofstream& file, const String& address))
	ABORT_IF(!NetworkTest::test("www.mpi-sb.mpg.de", NetworkTest::HTTP))
	TCPTransfer tcp_t;
	ofstream out;
	tcp_t.set(out, "http://www.mpi-sb.mpg.de/BALL/test/http_test.txt");
	TEST_EQUAL(tcp_t.getHostAddress(), "www.mpi-sb.mpg.de")
	TEST_EQUAL(tcp_t.getFileAddress(), "/BALL/test/http_test.txt")
	TEST_EQUAL(tcp_t.getPort(), 80)
	TEST_EQUAL(tcp_t.getStatusCode(), TCPTransfer::NO_ERROR)
	TEST_EQUAL(tcp_t.getReceivedBytes(), 0)
	TEST_EQUAL(tcp_t.getLogin(), "")
	TEST_EQUAL(tcp_t.getPassword(), "")
	TEST_EQUAL(tcp_t.getStream(), &out)
RESULT

CHECK(http/no login)
	ABORT_IF(!NetworkTest::test("www.mpi-sb.mpg.de", NetworkTest::HTTP))
	String filename;
	File::createTemporaryFilename(filename);
	ofstream out(filename.c_str(), std::ios::out);
	
	TCPTransfer tcp_t(out ,"http://www.mpi-sb.mpg.de/BALL/test/http_test.txt" , false);
	TEST_EQUAL(tcp_t.getHostAddress(), "www.mpi-sb.mpg.de")
	TEST_EQUAL(tcp_t.getFileAddress(), "/BALL/test/http_test.txt")
	TEST_EQUAL(tcp_t.getPort(), 80)
	TEST_EQUAL(tcp_t.getStatusCode(), TCPTransfer::NO_ERROR)
	TEST_EQUAL(tcp_t.getReceivedBytes(), 3048)
	TEST_EQUAL(tcp_t.getLogin(), "")
	TEST_EQUAL(tcp_t.getPassword(), "")
	out.close();
	
	TEST_FILE(filename.c_str(), "data/http_test.txt", false)
RESULT

CHECK(http/login)
	ABORT_IF(!NetworkTest::test("www.www.mpi-sb.mpg.de", NetworkTest::HTTP))
	String filename;
	File::createTemporaryFilename(filename);
	ofstream out(filename.c_str(), std::ios::out);
	TCPTransfer tcp_t2(out ,"http://BALL:test@www.mpi-sb.mpg.de/BALL/INTERNAL/internal.html", true);
	TEST_EQUAL(tcp_t2.getHostAddress(), "www.mpi-sb.mpg.de")
	TEST_EQUAL(tcp_t2.getFileAddress(), "/BALL/INTERNAL/internal.html")
	TEST_EQUAL(tcp_t2.getPort(), 80)
	TEST_EQUAL(tcp_t2.getStatusCode(), TCPTransfer::NO_ERROR)
	//?????, waiting for www-server config
	//TEST_EQUAL(tcp_t2.getReceivedBytes(), 11908)
	TEST_EQUAL(tcp_t2.getLogin(), "BALL")
	TEST_EQUAL(tcp_t2.getPassword(), "test")
	out.close();
RESULT

CHECK(ftp)
	ABORT_IF(!NetworkTest::test("ftp.mpi-sb.mpg.de", NetworkTest::FTP))
  String filename;
	File::createTemporaryFilename(filename);
	ofstream out(filename.c_str(), std::ios::out);
	TCPTransfer tcp_t(out, "ftp://ftp.mpi-sb.mpg.de/pub/outgoing/BALL/ftp_test.txt");
	TEST_EQUAL(tcp_t.getHostAddress(), "ftp.mpi-sb.mpg.de")
	TEST_EQUAL(tcp_t.getFileAddress(), "/pub/outgoing/BALL/ftp_test.txt")
	TEST_EQUAL(tcp_t.getPort(), 21)
	TEST_EQUAL(tcp_t.getStatusCode(), TCPTransfer::NO_ERROR)
	TEST_EQUAL(tcp_t.getReceivedBytes(), 2312)
	TEST_EQUAL(tcp_t.getLogin(), "")
	TEST_EQUAL(tcp_t.getPassword(), "")
	TEST_EQUAL(tcp_t.getStream(), &out)
	out.close();

	TEST_FILE(filename.c_str(), "data/ftp_test.txt", false)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
