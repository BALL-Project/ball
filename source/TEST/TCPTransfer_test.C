// $Id: TCPTransfer_test.C,v 1.5 2001/10/17 21:25:37 amoll Exp $

#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/SYSTEM/TCPTransfer.h>
#include <BALL/SYSTEM/file.h>
///////////////////////////

START_TEST(TCPTransfer, "$Id: TCPTransfer_test.C,v 1.5 2001/10/17 21:25:37 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

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

CHECK(http)
	String filename;
	File::createTemporaryFilename(filename);
	ofstream out(filename.c_str(), ios::out);
	
	TCPTransfer tcp_t(out ,"http://tucows.uni-erlangen.de:80/index.html" , false);
	TEST_EQUAL(tcp_t.getHostAddress(), "tucows.uni-erlangen.de")
	TEST_EQUAL(tcp_t.getFileAddress(), "/index.html")
	TEST_EQUAL(tcp_t.getPort(), 80)
	TEST_EQUAL(tcp_t.getStatusCode(), TCPTransfer::NO_ERROR)
	TEST_EQUAL(tcp_t.getReceivedBytes(), 2238)
	TEST_EQUAL(tcp_t.getLogin(), "")
	TEST_EQUAL(tcp_t.getPassword(), "")
	out.close();
	
	File f(filename);
	TEST_EQUAL(f.getSize(), 2238)
	f.close();

	File::createTemporaryFilename(filename);
	ofstream out2(filename.c_str(), ios::out);
	TCPTransfer tcp_t2(out2 ,"http://BALL:test@www.mpi-sb.mpg.de/BALL/INTERNAL/internal.html", true);
	TEST_EQUAL(tcp_t2.getHostAddress(), "www.mpi-sb.mpg.de")
	TEST_EQUAL(tcp_t2.getFileAddress(), "/BALL/INTERNAL/internal.html")
	TEST_EQUAL(tcp_t2.getPort(), 80)
	TEST_EQUAL(tcp_t2.getStatusCode(), TCPTransfer::NO_ERROR)
	TEST_EQUAL(tcp_t2.getReceivedBytes(), 11908)
	TEST_EQUAL(tcp_t2.getLogin(), "BALL")
	TEST_EQUAL(tcp_t2.getPassword(), "test")
	out2.close();
RESULT

CHECK(ftp1)
  String filename;
	File::createTemporaryFilename(filename);
	ofstream out(filename.c_str(), ios::out);
	TCPTransfer tcp_t(out, "ftp://anonymous:nobody@asd.de@ftp.mpi-sb.mpg.de:21/pub/welcome.msg", false);
	TEST_EQUAL(tcp_t.getHostAddress(), "ftp.mpi-sb.mpg.de")
	TEST_EQUAL(tcp_t.getFileAddress(), "/pub/welcome.msg")
	TEST_EQUAL(tcp_t.getPort(), 21)
	TEST_EQUAL(tcp_t.getStatusCode(), TCPTransfer::NO_ERROR)
	TEST_EQUAL(tcp_t.getReceivedBytes(), 932)
	TEST_EQUAL(tcp_t.getLogin(), "anonymous")
	TEST_EQUAL(tcp_t.getPassword(), "nobody@asd.de")
	TEST_EQUAL(tcp_t.getStream(), &out)
	out.close();

	File f(filename);
	TEST_EQUAL(f.getSize(), 932)
	f.close();
RESULT

CHECK(ftp2)
  String filename;
	File::createTemporaryFilename(filename);
	ofstream out(filename.c_str(), ios::out);
	TCPTransfer tcp_t(out, "ftp://ftp.vim.org/pub/vim/pc/vim60w32.zip");
	TEST_EQUAL(tcp_t.getHostAddress(), "ftp.vim.org")
	TEST_EQUAL(tcp_t.getFileAddress(), "/pub/vim/pc/vim60w32.zip")
	TEST_EQUAL(tcp_t.getPort(), 21)
	TEST_EQUAL(tcp_t.getStatusCode(), TCPTransfer::NO_ERROR)
	TEST_EQUAL(tcp_t.getReceivedBytes(), 591521)
	TEST_EQUAL(tcp_t.getLogin(), "")
	TEST_EQUAL(tcp_t.getPassword(), "")
	TEST_EQUAL(tcp_t.getStream(), &out)
	out.close();

	File f(filename);
	TEST_EQUAL(f.getSize(), 591521)
	f.close();
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
