// $Id: networkTest.h,v 1.4.4.1 2002/12/05 16:48:26 crauser Exp $

// workaround for Solaris -- this should be caught by configure -- OK
#define BSD_COMP 
#ifdef BALL_USE_WINSOCK
#include <windows.h>
#include<winsock.h>
#endif
#ifdef BALL_HAS_SYS_SOCKET_H
#include <sys/socket.h>	  // socket
#endif
#ifdef BALL_HAS_NETDB_H
#include <netdb.h>	  // gethostbyname
#endif
#ifdef BALL_HAS_NETINET_IN_H
#include <netinet/in.h>	  // sockaddr_in
#endif
#include <unistd.h>	  // close
//#include <sys/ioctl.h>
#include <iostream>	  // cout, endl

class NetworkTest
{
	public:
	
	enum ConnectionType
	{
		FTP,
		HTTP
	};
	
	static bool test(const String& hostname, ConnectionType type)
	{
		bool result = false;
		typedef int Socket;
		Socket my_socket;
		struct sockaddr_in host;  

		if (type == HTTP)
		{
			// check for HTTP transfer
			//first we will test if we get http access to the host
			struct hostent* ht = gethostbyname(hostname.c_str());
			if (ht == NULL)
			{
				return false;
			}  

			my_socket = socket(AF_INET, SOCK_STREAM, 0); 
			if (my_socket == -1)
			{
				return false;
			}  

			host.sin_family = AF_INET;
			host.sin_port	  = htons(80);
			host.sin_addr 	= *(struct in_addr*)ht->h_addr;  
			
			result = (connect(my_socket, (struct sockaddr*)&host, sizeof(struct sockaddr)) != -1);
#ifdef BALL_USE_WINSOCK
			closesocket(my_socket);
#else
			close(my_socket);
#endif
		}
		else
		{

			// now testing if we get FTP access to the host
			struct hostent* ht = gethostbyname(hostname.c_str());
			if (ht == NULL)
			{
				return 0;
			}  

			my_socket = socket(AF_INET, SOCK_STREAM, 0); 
			if (my_socket == -1)
			{
				my_socket = 0;
				return 0;
			}  

			host.sin_family = AF_INET;
			host.sin_port	  = htons(21);
			host.sin_addr 	= *(struct in_addr*)ht->h_addr;  
			
			result = (connect(my_socket, (struct sockaddr*)&host, sizeof(struct sockaddr)) != -1);
		}

		return result;
	}

	private:

	// don't allow instantiation
	NetworkTest();
};
