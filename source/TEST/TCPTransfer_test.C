// $Id: TCPTransfer_test.C,v 1.1 2001/09/09 23:52:12 amoll Exp $

#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/SYSTEM/TCPTransfer.h>
///////////////////////////

START_TEST(TCPTransfer, "$Id: TCPTransfer_test.C,v 1.1 2001/09/09 23:52:12 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;


CHECK(all)
  ofstream out("output", ios::out);
	TCPTransfer tcp_t(out ,"http://tucows.uni-erlangen.de:80/index.html");
	//TCPTransfer tcp_t(out, "http://postino.mpi-sb.mpg.de/index.html");
	//TCPTransfer tcp_t(out, "http://www.winmx.com/index.html");
	//TCPTransfer tcp_t(out, "http://download.nullsoft.com/winamp/client/winamp276_full.exe");
	//TCPTransfer tcp_t(out, "ftp://ftp.gwdg.de:21/pub/newfiles.gz");
	//TCPTransfer tcp_t(out, "ftp://ftp.uni-mainz.de/pub/misc/.cache");
	//TCPTransfer tcp_t(out, "ftp://ftp.gwdg.de:21/pub/mpg/service/lt.db.txt");
	//cout << tcp_t.getHostAddress() << "|"<< tcp_t.getFileAddress() <<"|"<< tcp_t.getPort() << endl;
	//cout << "status code "<< tcp_t.getStatusCode() << "   bytes:" << tcp_t.getReceivedBytes()<< endl;
	//TEST_EQUAL(tcp_t.getStatusCode(), TCPTransfer::NO_ERROR)
	//TEST_EQUAL(tcp_t.getReceivedBytes(), 1234)
	TEST_EQUAL(false, true)
	out.close();
RESULT


/*
int main()
{
	ofstream out("output", ios::out);
//	TCPTransfer tcp_t(out ,"http://tucows.uni-erlangen.de:80/index.html");
	TCPTransfer tcp_t(out, "ftp://ftp.gwdg.de:21/pub/newfiles.gz");
	cout << tcp_t.getHostAddress() << "|"<< tcp_t.getFileAddress() <<"|"<< tcp_t.getPort() << endl;
	cout << "status code "<< tcp_t.getStatusCode() << "   bytes:" << tcp_t.getReceivedBytes()<< endl;
	out.close();
}
*/

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
