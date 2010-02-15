// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: TCPTransfer.C,v 1.39.16.2 2007/05/18 20:42:22 anhi Exp $
//

// workaround for Solaris -- this should be caught by configure -- OK / 15.01.2002
#define BSD_COMP

// stupid workaround for Intel C++ 7.0/Linux w/ optimization enabled -- OK / 23.12.2002
#ifdef __OPTIMIZE__
# undef __OPTIMIZE__
#endif

#include <BALL/SYSTEM/TCPTransfer.h>
#include <BALL/SYSTEM/timer.h>
#include <BALL/SYSTEM/systemCalls.h>

#ifdef BALL_HAS_SYS_SOCKET_H
#	include <sys/socket.h>		// socket
#endif
#ifdef BALL_HAS_NETDB_H
#	include <netdb.h>				// gethostbyname
#endif
#ifdef BALL_HAS_NETINET_IN_H
#	include <netinet/in.h> 	// sockaddr_in
#endif
#ifdef BALL_HAS_UNISTD_H
#	include <unistd.h>  			// close, ioctl
#endif
#ifdef BALL_HAS_SYS_IOCTL_H
#	include <sys/ioctl.h>		// FIONBIO
#endif

#ifdef BALL_USE_WINSOCK
#	include <winsock2.h>
#	include <io.h>
#	define GLOBAL_CLOSE	closesocket
#	define sleep(a) _sleep(1000 * a)
#else
#	define GLOBAL_CLOSE ::close
#endif

#include <fstream>				// ostream
#include <stdio.h>

// DEBUG will lead to Logs, which will crash all QThreads, so only change this if you really 
// know what you are doing:
#undef DEBUG

namespace BALL
{
	QNetworkAccessManager global_network_manager;

	const int TCPTransfer::TIMEOUT = 3;  // 3 seconds timeout
	
	
	TCPTransfer::TransferFailed::TransferFailed(const char* file, int line, Index error_code)
		: Exception::GeneralException(file, line, string("TransferFailed"), string("Error code: ") + String(error_code))
	{
	}

	TCPTransfer::TCPTransfer(std::ostream& file, const String& address)
		throw(TCPTransfer::TransferFailed) 
	:	host_address_(""),
		file_address_(""),
		port_(0),
		login_(""),
		password_(""),
		status_(UNINITIALIZED__ERROR),
		received_bytes_(0),
		protocol_(UNKNOWN_PROTOCOL),
		socket_(0),
		fstream_(0),
		proxy_address_(""),
		proxy_port_(0),
		abort_(false)
	{
		set(file, address);

		status_ = transfer();
		if (status_ != OK)
		{
			throw(TransferFailed(__FILE__, __LINE__, status_));
		}
	}


	TCPTransfer::~TCPTransfer()
	{
		if (socket_ > 0)
		{
			GLOBAL_CLOSE(socket_);
			socket_ = 0;
		}
	}
				
				
	TCPTransfer::TCPTransfer()
	:	host_address_(""),
		file_address_(""),
		port_(0),
		login_(""),
		password_(""),
		status_(UNINITIALIZED__ERROR),
		received_bytes_(0),
		protocol_(UNKNOWN_PROTOCOL),
		socket_(0),
		fstream_(0),
		proxy_address_(""),
		proxy_port_(0)
	{	
	}


	void TCPTransfer::set(std::ostream&  file, 
													 Protocol 			protocol, 
													 const String& 	host_address, 
													 const String& 	file_address,
													 const String& 	login,
													 const String& 	password,
													 Position 			port)
	{
		protocol_ 			= protocol;
		host_address_ 	= host_address;
		file_address_ 	= file_address;
		login_ 					= login;
		password_				= password;
		port_ 					= port;
		fstream_ 				= &file;
		status_					= OK;
		received_bytes_ = 0;

		if (socket_ > 0)
		{
			GLOBAL_CLOSE(socket_);
			socket_ = 0;
		}
	}


	TCPTransfer::Status TCPTransfer::transfer()
	{
		abort_ = false;
		status_ = UNKNOWN_PROTOCOL__ERROR;

		if (protocol_ == FTP_PROTOCOL)
		{
			status_ =  getFTP_();
		}
		else if (protocol_ == HTTP_PROTOCOL)
		{
			status_ = getHTTP_();
		}

		if (socket_ > 0) 
		{
			GLOBAL_CLOSE(socket_);
			socket_ = 0;
		}
		return status_;
	}


	bool TCPTransfer::set(std::ostream& file, const String& address)
	{
		if (socket_ > 0)
		{
			GLOBAL_CLOSE(socket_);
			socket_ = 0;
		}

		status_ = ADDRESS__ERROR;

		fstream_ = &file;
		if (address.getSubstring(0, 7) == "http://")
		{
			protocol_ = HTTP_PROTOCOL;
			host_address_ = address.getSubstring(7);
			port_ = 80;
		}
		else
		{
			if (address.getSubstring(0, 6) == "ftp://")
			{
				protocol_ = FTP_PROTOCOL;
				host_address_ = address.getSubstring(6);
				port_ = 21;
			}
			else
			{
				protocol_ = UNKNOWN_PROTOCOL;
				status_ 	= UNKNOWN_PROTOCOL__ERROR;
				return false;
			}
		}

		// ----------- resolve address
		// example:   ftp://anonymous:nobody@asd.de@ftp.gwdg.de:21/pub/newfiles.gz		
		
		file_address_ = host_address_.getSubstring(host_address_.getField(0, "/").size()); 
		host_address_ = host_address_.left(host_address_.size() - file_address_.size());

		if (host_address_.has(':'))
		{
			String temp = host_address_.getField(-1, ":");
			if (!temp.isEmpty() && !temp.has('@'))
			{
				try
				{
					port_	= temp.toUnsignedInt();
				}
				catch(Exception::InvalidFormat&)
				{
					return false;
				}
				host_address_ = host_address_.left(host_address_.size() - temp.size() - 1);
			}
		}
		if (host_address_.has(':'))
		{		
			login_ = host_address_.getField(0, ":");
			host_address_ = host_address_.right(host_address_.size() - login_.size() - 1);
		}

		if (host_address_.has('@'))
		{
			String temp = host_address_.getField(-1, "@");
			password_ = host_address_.left(host_address_.size() - temp.size() - 1);
			host_address_ = temp;
		}
		if (file_address_.isEmpty() || host_address_.isEmpty())
		{
			return false;
		}
		
		status_ = OK;
		
		return true;
	}


	void TCPTransfer::clear()
	{
		host_address_.clear();
		file_address_.clear();
					 login_.clear();
				password_.clear();
				
		port_ 					= 0;
		fstream_ 				= 0;
		received_bytes_ = 0;
		status_				  = UNINITIALIZED__ERROR;
		protocol_ 			= UNKNOWN_PROTOCOL;
		
		if (socket_ > 0)
		{
			GLOBAL_CLOSE(socket_);
			socket_ = 0;
		}
	}


	TCPTransfer::Status TCPTransfer::getHTTPStatus_()
	{	
		// get the status from the server
		String first_line;
		for (Position pos = 0; pos < (Position) received_bytes_; pos++)
		{
			if (buffer_[pos] == '\n')
			{
				break;
			}
			first_line += buffer_[pos];
		}
		
		try
		{
			Status status = (Status) first_line.getField(1).toUnsignedInt();

			if (usingProxy() && status == 403)
			{
				return PROXY__ERROR;
			}

			if(status==302 || status==404)
			{
				return FILENOTFOUND__ERROR;
			}

			if (status != 200)
			{
				return status;
			}
		}
		catch(Exception::InvalidFormat&)
		{
			return UNKNOWN__ERROR;
		}

		return OK;
	}

		
	TCPTransfer::Status TCPTransfer::getHTTP_()
	{
		String query = "GET ";
		
		if (usingProxy()) query += "http://" + host_address_;

		query += file_address_ + " HTTP/1.0\n";
		query += "Accept: */*\n";
		query += "User-Agent: Mozilla/4.76\n";
		query += "Host: " + host_address_ + "\r\n";
		
		if (usingProxy()) 
		{
			query += "Proxy-Connection: close\r\n";
		}

		// HTTP authentification
		if (!login_.isEmpty() && !password_.isEmpty())
		{
			String auth(login_ + ":" + password_);
			query += "Authorization: Basic "+ auth.encodeBase64() + "\n";
		}
		query += "\n";

	
		// Logon to webserver and send GET-request
		Status status = logon_(query);
		if (status != OK)
		{
			return status;
		}
		
		// read Status from HTTP-Request-Response
		status = getHTTPStatus_();

		if (status != OK)
		{
			return status;
		}

		// now cutting of the head
		int bytes = received_bytes_;
		Position pos = 0; 
		for (; pos < (Position)bytes; pos++)
		{
			if ((buffer_[pos] == '\n') && (buffer_[pos + 1] == '\r'))
			{
				break;
			}
		}
		if (pos == 0)
		{
			return BODY__ERROR;
		}
		
		// write the file
		// skip the carriage returns
		pos += 3;
		for (Position i = pos; i < (Position)bytes; i ++)
		{
			(*fstream_) << buffer_[i];
		}
		received_bytes_ = bytes - pos;

		// receive the rest
		do
		{		
			bytes = readInputFromSocket_(socket_);

			if (bytes < 0)
			{
				return RECV__ERROR;
			}
			for (Position i = 0; i < (Position)bytes; i++)
			{
				(*fstream_) << buffer_[i];
			}
			received_bytes_ += bytes;
		}
		while (!abort_ && bytes > 0);

		return OK;
	}
		

	TCPTransfer::Status TCPTransfer::setBlock_(Socket socket, bool block)
	{
		// WIN port
		#ifndef BALL_USE_WINSOCK
			int temp = !block;

			if (ioctl(socket, FIONBIO, &temp) == -1)
			{
				return CONNECT__ERROR;
			}
		#else
			u_long temp = !block;

			if (ioctlsocket(socket, FIONBIO, &temp) == -1)
			{
				return CONNECT__ERROR;
			}
		#endif

		return OK;
	}


	TCPTransfer::Status TCPTransfer::logon_(const String& query)
	{
		#ifdef BALL_USE_WINSOCK
			WORD    wsa_vers = 0x0101;
			WSADATA wsa_data;
		
			if (WSAStartup(wsa_vers, &wsa_data) != 0)
			{
				Log.error() << "SockInetBuf::open: No WINSOCK.DLL found!" << std::endl;
			}	 
		#endif

		status_ = UNKNOWN__ERROR;

		// ============ do we use a proxy ? =====================
		Position port = 0;
		String host_address = "";

		if (usingProxy())
		{
			port = proxy_port_;
			host_address = proxy_address_;
		}
		else
		{
			port = port_;
			host_address = host_address_;
		}
	
		// ok, start the connection
		struct hostent* ht = gethostbyname(host_address.c_str());
		if (ht == NULL)
		{
			status_ = GETHOSTBYNAME__ERROR;
			return status_;
		}  

		if (socket_ > 0)
		{
			GLOBAL_CLOSE(socket_);
			socket_ = 0;
		}
		socket_ = socket(AF_INET, SOCK_STREAM, 0); 
		if (socket_ == -1)
		{
			socket_ = 0;
			status_ = SOCKET__ERROR;
			return status_;
		}  

		struct sockaddr_in host;  
		host.sin_family = AF_INET;
		host.sin_port	  = htons(port);
		host.sin_addr 	= *(struct in_addr*)ht->h_addr;

		setBlock_(socket_, false);
		Timer timer;
		timer.start();
		bool connected, try_connect;

		// try to connect in non-blocking mode; usually, this will not directly succeed
		if (connect(socket_, (struct sockaddr*)&host, sizeof(struct sockaddr)) == -1)
		{
			connected = false;
			try_connect = false;
			// if we have to wait for a bit, errno will tell us
#ifndef BALL_OS_WINDOWS
			if (errno == EINPROGRESS)
#else
			int error = WSAGetLastError();
			if (error == WSAEWOULDBLOCK || error == WSAEINPROGRESS)
#endif
				try_connect = true;
		}
		else 
		{
			connected = true;
			try_connect = false;
		}

		struct timeval timeout;
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;

		fd_set socket_set;
		FD_ZERO(&socket_set);
		FD_SET(socket_, &socket_set);

		while (try_connect && timer.getClockTime()<TIMEOUT)
		{
			int result = select(FD_SETSIZE, NULL, &socket_set, NULL, &timeout);
			if (result == -1)
			{
				connected = false;
				try_connect = false;
			}
			else if (result == 1)
			{
				connected = true;
				try_connect = false;
			}
		}
		setBlock_(socket_, true);

		if(!connected)
		{
			if (!usingProxy()) status_ = CONNECT__ERROR;
			else 					     status_ = PROXY__ERROR;

			return status_;
		}

		if (!query.isEmpty())
		{
			sendData_(query, socket_);
		}

		received_bytes_ = readInputFromSocket_(socket_);

		if (received_bytes_ < 0)
		{
			status_ = RECV__ERROR;
			return status_;
		}

		buffer_[received_bytes_] = '\0';

		#ifdef DEBUG
			dump();
 		#endif

		status_ = OK;
		return status_;
	}	


	TCPTransfer::Status TCPTransfer::getFTPStatus_()
	{
		status_ = UNKNOWN__ERROR;
		String temp;
		for (Position pos = 0; pos < 3 && pos < (Position) received_bytes_; pos++)
		{
			temp += buffer_[pos];
		}
		try
		{
			status_ = (Status)temp.toUnsignedInt();
		}
		catch(Exception::InvalidFormat&)
		{
		}	
		
		return status_;
	}


	void TCPTransfer::dump(std::ostream& s, Size depth) const
	{
		BALL_DUMP_STREAM_PREFIX(s);

		BALL_DUMP_DEPTH(s, depth);

		s << std::endl;
		for (Position pos = 0; pos < (Position) received_bytes_; pos++)
		{
			if (buffer_[pos] == '\0') 
			{
				break;
			}
			s << buffer_[pos];
		}
		s << std::endl;
		s << "received bytes: " << received_bytes_ << std::endl;

		BALL_DUMP_STREAM_SUFFIX(s);
	}


	TCPTransfer::Status TCPTransfer::sendData_(const String& query, Socket socket)
	{
		#ifdef DEBUG
			Log.info() << "\n>>" << query << std::endl;
		#endif
				
		if (send(socket, query.c_str(), query.size(), 0) != (int) query.size())
		{
			status_ = SEND__ERROR;
			return status_;
		}
		return OK;
	}

	bool TCPTransfer::waitForOutput_(const String& key, Size seconds)
	{
		setBlock_(socket_, false);
		Timer timer;
		timer.start();
		while (timer.getClockTime() < seconds)
		{
			received_bytes_ = getReceivedBytes_(socket_);

			if (received_bytes_ > 0)
			{
				buffer_[received_bytes_] = '\0';
				String temp(buffer_);
				
				#ifdef DEBUG
					dump();
				#endif
						
				if (key.size() == 0 || temp.hasSubstring(key))
				{
					setBlock_(socket_, true);
					return true;
				}
			}
			sleep(1);
		}

		setBlock_(socket_, true);
		return false;
	}


	bool TCPTransfer::getFTPMessage_(Index status)
	{
		// read all lines from the server
		// last line starts with "nr "
		setBlock_(socket_, false);
		
		bool got_data = false;

		// got "nr " (important: space)  -> last line has arrived
		bool last_line1 = false;
		
		// last line has ended
		bool last_line2 = false;
		
		// try to read login message, abort after 20 seconds
		Timer timer;
		timer.start();
		do		
		{	
			received_bytes_ = readInputFromSocket_(socket_);

			if (received_bytes_ > 0)
			{
				buffer_[received_bytes_] = '\0';
				#ifdef DEBUG
					dump();
				#endif
								
				if (!got_data)
				{
					Status got_status = getFTPStatus_();
					if (got_status != status)
					{
						status_ = got_status;
						return false;
					}
					got_data = true;
				}
							
				String temp(buffer_);
				if (temp.hasSubstring(String(status) + " "))
				{
					last_line1 = true;
				}
				if (last_line1 && temp.has('\n'))
				{
					last_line2 = true;
					break;
				}
			}
		}
		while (!abort_ && timer.getClockTime() < 20);
		
		if (!last_line2)
		{
			status_ = RECV__ERROR;
			return false; 
		}
		
		return true;
	}

	TCPTransfer::Status TCPTransfer::getFTP_()
	{
		// connect to FTP-server
		Status status = logon_("");
		if (status != OK) 
		{
			return status;
		}

		if (getFTPStatus_() != 220)
		{
			return status_;
		}

		// login
		String query;
		if (login_.isEmpty())
		{
			query = "USER anonymous\n";
		}
		else
		{
			query = "USER "+ login_ + "\n";
		}
		
		sendData_(query, socket_);
		
		if (!getFTPMessage_(331)) return status_;		
		
		// password 
		// if password is empty try a common type email-address as password
		if (password_.isEmpty())
		{
			query = "PASS nobody@nowhereland.com\n";
		}
		else
		{	
		 query = "PASS " + password_ + "\n";
		}
		
		sendData_(query, socket_);
		
		if (!getFTPMessage_(230)) 
		{
			return status_;
		}
		// we will use a passive connection
		query = "PASV\n";
		sendData_(query, socket_);	
		
		// we get a code 227 if FTP-server will open a passive connection
		if (!getFTPMessage_(227)) return status_;	

		// port for passive connection
		Position passv_port = 0;
		// host address for passive connection
		String 	 passv_host;
		try
		{
			String temp(buffer_);
			temp = temp.getField(1, "(");
			temp = temp.getField(0, ")");

			// extract IP for passive FTP-server
			passv_host = temp.getField(0, ",") + "." +
									 temp.getField(1, ",") + "." +
									 temp.getField(2, ",") + "." +
									 temp.getField(3, ",");
			
			// calculate port
			passv_port = temp.getField(4, ",").toUnsignedInt() * 256 + 
									 temp.getField(5, ",").toUnsignedInt();
		}
		catch(Exception::InvalidFormat&)
		{
			// we dont need anything to do here
			// passv_port == 0
		}

		if (passv_port == 0)
		{
			return PORT__ERROR;
		}

		struct hostent* ht = gethostbyname(passv_host.c_str());
		if (ht == NULL)
		{
			status_ = GETHOSTBYNAME__ERROR;
			return status_;
		}  

		Socket socket2 = socket(AF_INET, SOCK_STREAM, 0); 
		if (socket2 == -1)
		{
			status_ = SOCKET__ERROR;
			return status_;
		}  

		struct sockaddr_in host;  
		host.sin_family = AF_INET;
		host.sin_port	  = htons(passv_port);
		host.sin_addr 	= *(struct in_addr*)ht->h_addr;  

		// now connecting to passive ftp port
		setBlock_(socket2, false);
		Timer timer;
		timer.start();
		bool connected, try_connect;

		// try to connect in non-blocking mode; usually, this will not directly succeed
		if (connect(socket2, (struct sockaddr*)&host, sizeof(struct sockaddr)) == -1)
		{
			connected = false;
			try_connect = false;

			// if we have to wait for a bit, errno will tell us
#ifndef BALL_OS_WINDOWS
			if (errno == EINPROGRESS)
#else
			if (errno == WSAEWOULDBLOCK || errno == WSAEINPROGRESS)
#endif
				try_connect = true;
		}
		else 
		{
			connected = true;
			try_connect = false;
		}

		struct timeval timeout;
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;

		fd_set socket_set;
		FD_ZERO(&socket_set);
		FD_SET(socket2, &socket_set);

		while (try_connect && timer.getClockTime()<TIMEOUT)
		{
			int result = select(FD_SETSIZE, NULL, &socket_set, NULL, &timeout);
			if (result == -1)
			{
				connected = false;
				try_connect = false;
			}
			else if (result == 1)
			{
				connected = true;
				try_connect = false;
			}
		}

		if(!connected)
		{
			GLOBAL_CLOSE(socket2);
			status_ = CONNECT__ERROR;
			return status_;
		}

		// ----------------------------------- setting binary mode
		query = "TYPE I\n";
		sendData_(query, socket_);	

		// we get a code 200 if FTP-server will use binary mode
		if (!getFTPMessage_(200)) return status_;
		
		// ----------------------------------- test if server will send file
		query = "RETR " + file_address_ + '\n';
		
		#ifdef DEBUG
			Log.info() << ">>" << query << std::endl;
		#endif

		if (send(socket_, query.c_str(), query.size(), 0) != (int) query.size())
		{
			GLOBAL_CLOSE(socket2);
			return SEND__ERROR;
		}
		

		if (!getFTPMessage_(150)) 
		{
			GLOBAL_CLOSE(socket2);	
			return status_;	
		}		
		
		// ----------------------------------- receive the file
		received_bytes_ = 0;
		int control_bytes = -1;
		setBlock_(socket_, false);

		int bytes = -1;
		String control;

		while (!abort_ && control_bytes < 1 && bytes != 0)
		{			
			bytes = readInputFromSocket_(socket2);

			if (bytes > 0)
			{
				for (Position i = 0; i < (Position)bytes; i++)
				{
					(*fstream_) << buffer_[i];
				}
				received_bytes_ += bytes;
			}

			control_bytes = readInputFromSocket_(socket_);
			if (control_bytes > 0)
			{	
				buffer_[control_bytes] = '\0';
				control = buffer_;
			#ifdef DEBUG
					Log.info() << "\n<<\n";			
					for (Position i = 0; i < (Position)control_bytes; i++) { Log.info() <<  buffer_[i]; }
					Log.info() << "\n";	Log.info() << std::endl;
			#endif
			}
		}
		// im moment noch kein zeitabbruch	
		
		if (control.hasPrefix("226") || control.hasPrefix("250")) 
		{
			// due to a race condition between the control socket and the data socket, we might have
			// missed some data so far
			bytes = readInputFromSocket_(socket2);

			while (bytes > 0)
			{
				for (Position i = 0; i < (Position)bytes; i++)
				{
					(*fstream_) << buffer_[i];
				}
				received_bytes_ += bytes;

				bytes = readInputFromSocket_(socket2);
			}

			GLOBAL_CLOSE(socket2);
				
			return OK;
		}

		// we dont need the second socket anymore
		GLOBAL_CLOSE(socket2);

		if (bytes == 0) return OK;

			if (control_bytes < 1)
		{
			status_ = RECV__ERROR;
			return status_;
		}

		buffer_[control_bytes] = '\0';

		if (!getFTPMessage_(226)) return status_;

		return OK;
	}

	int TCPTransfer::getReceivedBytes_(Socket& socket)
	{
#		ifdef BALL_USE_WINSOCK
			return (int)::recv(socket, buffer_, BUFFER_SIZE, 0);
#		else
			return read(socket, buffer_, BUFFER_SIZE);
#		endif
	}
	
	int TCPTransfer::readInputFromSocket_(Socket& socket)
	{	
		setBlock_(socket, false);
	
		int received_bytes = -1;
		Timer timer;
		timer.start();
		while (received_bytes<0 && timer.getClockTime()<TIMEOUT)
		{
			received_bytes = getReceivedBytes_(socket); // returns -1 if there was an error (e.g. no connection)
			if(received_bytes<0) sleepFor(200); // sleep 0.2 seconds
		}
		
		setBlock_(socket, true);
		return received_bytes;
	}

	void TCPTransfer::setProxy(const String proxy_address, Position port)
	{
		proxy_address_ = proxy_address;
		proxy_port_    = port;
	}

	bool TCPTransfer::usingProxy() const
	{
		return proxy_address_ != "" && proxy_port_    != 0;
	}

	String TCPTransfer::getErrorCode() const
	{
		switch(status_)
		{
			case OK: return "No Error";
			case GETHOSTBYNAME__ERROR: return "HostByName Error";
			case SOCKET__ERROR: return "Socket Error";
			case CONNECT__ERROR: return "Connect Error";
			case RECV__ERROR: return "Receive Error";
			case OUTOFMEMORY__ERROR: return "OutOfMemory Error";
			case BODY__ERROR: return "Body Error";
			case UNKNOWN__ERROR: return "Unknown Error";
			case ADDRESS__ERROR: return "Address Error";
			case UNINITIALIZED__ERROR: return "Unitialized Error";
			case TRANSFER__ERROR: return "Transfer Error";
			case SEND__ERROR: return "Send Error";
			case PORT__ERROR: return "Port Error";
			case UNKNOWN_PROTOCOL__ERROR: return "UnknownProtocol Error";
			case LOGON__ERROR: return "Logon Error";
			case PROXY__ERROR: return "Proxy Error";
			case FILENOTFOUND__ERROR: return "FileNotFound Error";
		}
		return "Unknown Error";
	}

} // namespace BALL

