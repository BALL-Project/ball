// $Id: networkTest.h,v 1.2 2002/01/04 03:48:29 oliver Exp $

#include <sys/socket.h>	  // socket
#include <netdb.h>	  // gethostbyname
#include <netinet/in.h>	  // sockaddr_in
#include <unistd.h>	  // close
#include <iostream>	  // cout, endl

#if defined(__hpux__) || defined(__linux__)
# include <sys/ioctl.h>	  //ioctl, FIONBIO
#else
# include <sys/filio.h>   //ioctl, FIONBIO
#endif

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
			close(my_socket);
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
