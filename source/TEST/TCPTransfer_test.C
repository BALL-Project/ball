// $Id: TCPTransfer_test.C,v 1.3 2001/09/10 12:49:04 amoll Exp $

#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/SYSTEM/TCPTransfer.h>
#include <BALL/SYSTEM/file.h>
///////////////////////////

START_TEST(TCPTransfer, "$Id: TCPTransfer_test.C,v 1.3 2001/09/10 12:49:04 amoll Exp $")

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

CHECK(set(ofstream& file, const String& address))
	TCPTransfer tcp_t;
	ofstream out;
	tcp_t.set(out, "http://postino.mpi-sb.mpg.de/index.html");
	TEST_EQUAL(tcp_t.getHostAddress(), "postino.mpi-sb.mpg.de")
	TEST_EQUAL(tcp_t.getFileAddress(), "/index.html")
	TEST_EQUAL(tcp_t.getPort(), 80)
	TEST_EQUAL(tcp_t.getStatusCode(), TCPTransfer::NO_ERROR)
	TEST_EQUAL(tcp_t.getReceivedBytes(), 0)
	TEST_EQUAL(tcp_t.getLogin(), "")
	TEST_EQUAL(tcp_t.getPassword(), "")
	TEST_EQUAL(tcp_t.getStream(), &out)
RESULT

/*
CHECK(esoterik)
	String filename;
	File::createTemporaryFilename(filename);
	ofstream out(filename.c_str(), ios::out);
	out << "ASDFGHJKL";
	out.close();
	File f(filename);
	TEST_EQUAL(f.getSize(), 9);
RESULT

CHECK(esoterik2)
	String filename;
	File::createTemporaryFilename(filename);
	ofstream out(filename.c_str(), ios::out);
	out << "ASDFGHJKLM";
	out.close();
	File f(filename);
	TEST_EQUAL(f.getSize(), 10);
RESULT

*/
CHECK(http)
	String filename;
	File::createTemporaryFilename(filename);
	ofstream out(filename.c_str(), ios::out);
	TCPTransfer tcp_t(out ,"http://tucows.uni-erlangen.de:80/index.html");
	TEST_EQUAL(tcp_t.getHostAddress(), "tucows.uni-erlangen.de")
	TEST_EQUAL(tcp_t.getFileAddress(), "/index.html")
	TEST_EQUAL(tcp_t.getPort(), 80)
	TEST_EQUAL(tcp_t.getStatusCode(), TCPTransfer::NO_ERROR)
	TEST_EQUAL(tcp_t.getReceivedBytes(), 2238)
	TEST_EQUAL(tcp_t.getLogin(), "")
	TEST_EQUAL(tcp_t.getPassword(), "")
	out.close();
	
	//File f(filename);
	//TEST_EQUAL(f.getSize(), 2238)
	//f.close();
	File::createTemporaryFilename(filename);
	ofstream out2(filename.c_str(), ios::out);
	tcp_t.set(out2 ,"http://BALL:test@www.mpi-sb.mpg.de/BALL/INTERNAL/internal.html");
	TEST_EQUAL(tcp_t.getHostAddress(), "www.mpi-sb.mpg.de")
	TEST_EQUAL(tcp_t.getFileAddress(), "/BALL/INTERNAL/internal.html")
	TEST_EQUAL(tcp_t.getPort(), 80)
	TEST_EQUAL(tcp_t.getStatusCode(), TCPTransfer::NO_ERROR)
	TEST_EQUAL(tcp_t.getReceivedBytes(), 2238)
	TEST_EQUAL(tcp_t.getLogin(), "BALL")
	TEST_EQUAL(tcp_t.getPassword(), "test")
	out2.close();
RESULT

CHECK(ftp)
  String filename;
	File::createTemporaryFilename(filename);
	ofstream out(filename.c_str(), ios::out);
	TCPTransfer tcp_t(out, "ftp://anonymous:nobody@asd.de@ftp.mpi-sb.mpg.de:21/pub/welcome.msg");
	TEST_EQUAL(tcp_t.getHostAddress(), "ftp.mpi-sb.mpg.de")
	TEST_EQUAL(tcp_t.getFileAddress(), "/pub/welcome.msg")
	TEST_EQUAL(tcp_t.getPort(), 21)
	TEST_EQUAL(tcp_t.getStatusCode(), TCPTransfer::NO_ERROR)
	TEST_EQUAL(tcp_t.getReceivedBytes(), 961)
	TEST_EQUAL(tcp_t.getLogin(), "anonymous")
	TEST_EQUAL(tcp_t.getPassword(), "nobody@asd.de")
	TEST_EQUAL(tcp_t.getStream(), &out)
	out.close();

	File f(filename);
	TEST_EQUAL(f.getSize(), 961)
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
