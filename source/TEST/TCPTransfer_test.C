// $Id: TCPTransfer_test.C,v 1.9 2001/12/30 00:09:43 oliver Exp $

#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/SYSTEM/TCPTransfer.h>
#include <BALL/SYSTEM/file.h>
///////////////////////////

#include <sys/socket.h>		// socket
#include <netdb.h>				// gethostbyname
#include <netinet/in.h> 	// sockaddr_in
#include <unistd.h>  			// close
#include <iostream.h> 		// cout, endl

#if defined(__hpux__) || defined(__linux__)
# include <sys/ioctl.h>		//ioctl, FIONBIO
#else
# include <sys/filio.h>		//ioctl, FIONBIO
#endif

using namespace BALL;
using namespace std;

//test if networking is available
bool testNetwork()
{
	typedef int Socket;
	Socket socket_;
	//first we will test if we get http access to the ZBI
	struct hostent* ht = gethostbyname("www.zbi-saar.de");
	if (ht == NULL)
	{
		return 0;
	}  

	socket_ = socket(AF_INET, SOCK_STREAM, 0); 
	if (socket_ == -1)
	{
		return 0;
	}  

	struct sockaddr_in host;  
	host.sin_family = AF_INET;
	host.sin_port	  = htons(80);
	host.sin_addr 	= *(struct in_addr*)ht->h_addr;  
	
	if(connect(socket_, (struct sockaddr*)&host, sizeof(struct sockaddr)) == -1)
	{
		close(socket_);
		return 0;
	}
	close(socket_);

	// now testing if we get FTP access to the MPI
	ht = gethostbyname("ftp.mpi-sb.mpg.de");
	if (ht == NULL)
	{
		return 0;
	}  

	socket_ = socket(AF_INET, SOCK_STREAM, 0); 
	if (socket_ == -1)
	{
		socket_ = 0;
		return 0;
	}  

	host.sin_family = AF_INET;
	host.sin_port	  = htons(21);
	host.sin_addr 	= *(struct in_addr*)ht->h_addr;  
	
	if(connect(socket_, (struct sockaddr*)&host, sizeof(struct sockaddr)) == -1)
	{
		close(socket_);
		return 0;
	}
	close(socket_);

	return true;
}



START_TEST(TCPTransfer, "$Id: TCPTransfer_test.C,v 1.9 2001/12/30 00:09:43 oliver Exp $")


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

bool network = false;
CHECK(network)
	network = testNetwork();
	// if no network-connection available, stop test
	ABORT_IF(!network)
RESULT	


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
	ABORT_IF(!network)
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
	ABORT_IF(!network)
	String filename;
	File::createTemporaryFilename(filename);
	ofstream out(filename.c_str(), ::std::ios::out);
	
	TCPTransfer tcp_t(out ,"http://www.zbi.uni-saarland.de/zbi/BALL/test/TCPTransferTest.txt" , false);
	TEST_EQUAL(tcp_t.getHostAddress(), "www.zbi.uni-saarland.de")
	TEST_EQUAL(tcp_t.getFileAddress(), "/zbi/BALL/test/TCPTransferTest.txt")
	TEST_EQUAL(tcp_t.getPort(), 80)
	TEST_EQUAL(tcp_t.getStatusCode(), TCPTransfer::NO_ERROR)
	TEST_EQUAL(tcp_t.getReceivedBytes(), 3665)
	TEST_EQUAL(tcp_t.getLogin(), "")
	TEST_EQUAL(tcp_t.getPassword(), "")
	out.close();
	
	File f(filename);
	TEST_EQUAL(f.getSize(), 3665)
	f.close();

	File::createTemporaryFilename(filename);
	ofstream out2(filename.c_str(), ::std::ios::out);
	TCPTransfer tcp_t2(out2 ,"http://BALL:test@www.mpi-sb.mpg.de/BALL/INTERNAL/internal.html", true);
	TEST_EQUAL(tcp_t2.getHostAddress(), "www.mpi-sb.mpg.de")
	TEST_EQUAL(tcp_t2.getFileAddress(), "/BALL/INTERNAL/internal.html")
	TEST_EQUAL(tcp_t2.getPort(), 80)
	TEST_EQUAL(tcp_t2.getStatusCode(), TCPTransfer::NO_ERROR)
	//BAUSTELLE, waiting for www-server config
	//TEST_EQUAL(tcp_t2.getReceivedBytes(), 11908)
	TEST_EQUAL(tcp_t2.getLogin(), "BALL")
	TEST_EQUAL(tcp_t2.getPassword(), "test")
	out2.close();
RESULT

CHECK(ftp)
	ABORT_IF(!network)
  String filename;
	File::createTemporaryFilename(filename);
	ofstream out(filename.c_str(), ::std::ios::out);
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
