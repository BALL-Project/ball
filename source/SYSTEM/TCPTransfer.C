// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: TCPTransfer.C,v 1.27 2003/05/25 21:38:10 oliver Exp $
//

// workaround for Solaris -- this should be caught by configure -- OK / 15.01.2002
#define BSD_COMP

// stupid workaround for Intel C++ 7.0/Linux w/ optimization enabled -- OK / 23.12.2002
#ifdef __OPTIMIZE__
# undef __OPTIMIZE__
#endif

#include <BALL/SYSTEM/TCPTransfer.h>
#include <BALL/SYSTEM/timer.h>

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
#	define GLOBAL_CLOSE	_close
#	define sleep(a) _sleep(1000 * a)
#else
#	define GLOBAL_CLOSE ::close
#endif

#include <fstream>				// ofstream
#include <stdio.h>


namespace BALL
{

	TCPTransfer::TransferFailed::TransferFailed(const char* file, int line, Index error_code)
		throw()
		: Exception::GeneralException(file, line, string("TransferFailed"), string("Error code: ") + String(error_code))
	{
	}


	TCPTransfer::TCPTransfer(std::ofstream& file, const String& address)
		throw(TCPTransfer::TransferFailed) 
	:	host_address_(""),
		file_address_(""),
		port_(0),
		login_(""),
		password_(""),
		status_(UNINITIALIZED_ERROR),
		received_bytes_(0),
		protocol_(UNKNOWN_PROTOCOL),
		buffer_(),
		socket_(0),
		fstream_(0)
	{
		set(file, address);

		status_ = transfer();
		if (status_ != OK)
		{
			throw(TransferFailed(__FILE__, __LINE__, status_));
		}
	}


	TCPTransfer::~TCPTransfer()
		throw()
	{
		if (socket_ != 0)
		{
			GLOBAL_CLOSE(socket_);
			socket_ = 0;
		}
	}
				
				
	TCPTransfer::TCPTransfer()
		throw()
	:	host_address_(""),
		file_address_(""),
		port_(0),
		login_(""),
		password_(""),
		status_(UNINITIALIZED_ERROR),
		received_bytes_(0),
		protocol_(UNKNOWN_PROTOCOL),
		buffer_(),
		socket_(0),
		fstream_(0)
	{	
	}


	void TCPTransfer::set(std::ofstream&  file, 
													 Protocol 			protocol, 
													 const String& 	host_address, 
													 const String& 	file_address,
													 const String& 	login,
													 const String& 	password,
													 Position 			port)
		throw()
	{
		protocol_ 			= protocol;
		host_address_ 	= host_address;
		file_address_ 	= file_address;
		login_ 					= login;
		password_				= password;
		port_ 					= port;
		fstream_ 				= &file;
		status_					= (Status)OK;
		received_bytes_ = 0;

		if (socket_ != 0)
		{
			close(socket_);
			socket_ = 0;
		}
	}


	TCPTransfer::Status TCPTransfer::transfer()
		throw()
	{
		if (protocol_ == FTP_PROTOCOL)
		{
			status_ =  getFTP_();
			return status_;
		}
		
		if (protocol_ == HTTP_PROTOCOL)
		{
			status_ = getHTTP_();
			return status_;
		}

		close(socket_);
		return UNKNOWN_PROTOCOL_ERROR;
	}


	bool TCPTransfer::set(std::ofstream& file, const String& address)
		throw()
	{
		if (socket_ != 0)
		{
			close(socket_);
			socket_ = 0;
		}

		status_ = ADDRESS_ERROR;

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
				status_ 	= UNKNOWN_PROTOCOL_ERROR;
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
				catch(Exception::InvalidFormat)
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
		
		status_ = (Status)OK;
		
		return true;
	}


	void TCPTransfer::clear()
		throw()
	{
		host_address_.clear();
		file_address_.clear();
					 login_.clear();
				password_.clear();
				
		port_ 					= 0;
		fstream_ 				= 0;
		received_bytes_ = 0;
		status_				  = UNINITIALIZED_ERROR;
		protocol_ 			= UNKNOWN_PROTOCOL;
		
		if (socket_ != 0)
		{
			close(socket_);
			socket_ = 0;
		}
	}


	TCPTransfer::Status TCPTransfer::getHTTPStatus_()
		throw()
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
			if (status != 200)
			{
				return status;
			}
		}
		catch(Exception::InvalidFormat)
		{
			return UNKNOWN_ERROR;
		}

		return (Status)OK;
	}

		
	TCPTransfer::Status TCPTransfer::getHTTP_()
		throw()
	{
		String query;
		query  = "GET " + file_address_ + " HTTP/1.0\n";
		query += "Accept: */*\n";
		query += "User-Agent: Mozilla/4.76\n";
		
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
		Size bytes = received_bytes_;
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
			return BODY_ERROR;
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
#			ifdef BALL_USE_WINSOCK
			bytes = (int)::recv(socket_, buffer_, BUFFER_SIZE, 0);
#			else
			bytes = read(socket_, buffer_, BUFFER_SIZE);
#			endif
			if (bytes < 0)
			{
				return RECV_ERROR;
			}
			for (Position i = 0; i < (Position)bytes; i++)
			{
				(*fstream_) << buffer_[i];
			}
			received_bytes_ += bytes;
		}
		while (bytes > 0);

		return (Status)OK;
	}
		

	TCPTransfer::Status TCPTransfer::setBlock_(Socket socket, bool block)
		throw()
	{
		
		// WIN port
		#ifndef BALL_USE_WINSOCK
			int temp = !block;

			if (ioctl(socket, FIONBIO, &temp) == -1)
			{
				return (Status)CONNECT_ERROR;
			}
		#else
			u_long temp = !block;

			if (ioctlsocket(socket, FIONBIO, &temp) == -1)
			{
				return (Status)CONNECT_ERROR;
			}
		#endif

		return (Status)OK;
	}


	TCPTransfer::Status TCPTransfer::logon_(const String& query)
		throw()
	{
		#ifdef BALL_USE_WINSOCK
			WORD    wsa_vers = 0x0101;
			WSADATA wsa_data;
		
			if (WSAStartup(wsa_vers, &wsa_data) != 0)
			{
				Log.error() << "SockInetBuf::open: No WINSOCK.DLL found!" << std::endl;
			}	 
		#endif

		status_ = UNKNOWN_ERROR;
		
		struct hostent* ht = gethostbyname(host_address_.c_str());
		if (ht == NULL)
		{
			status_ = GETHOSTBYNAME_ERROR;
			return status_;
		}  
		if (socket_ != 0)
		{
			close(socket_);
		}
		socket_ = socket(AF_INET, SOCK_STREAM, 0); 
		if (socket_ == -1)
		{
			socket_ = 0;
			status_ = (Status)SOCKET_ERROR;
			return status_;
		}  

		struct sockaddr_in host;  
		host.sin_family = AF_INET;
		host.sin_port	  = htons(port_);
		host.sin_addr 	= *(struct in_addr*)ht->h_addr;  

		if(connect(socket_, (struct sockaddr*)&host, sizeof(struct sockaddr)) == -1)
		{
			status_ = CONNECT_ERROR;
			return status_;
		}
		if (!query.isEmpty())
		{
			sendData_(query, socket_);
		}
#			ifdef BALL_USE_WINSOCK
			received_bytes_ = (int)::recv(socket_, buffer_, BUFFER_SIZE, 0);
#			else
			received_bytes_ = read(socket_, buffer_, BUFFER_SIZE);
#			endif
		if (received_bytes_ < 0)
		{
			status_ = RECV_ERROR;
			return status_;
		}
		buffer_[received_bytes_] = '\0';
		#ifdef DEBUG
			output_();
		#endif
		
		status_ = (Status)OK;
		return status_;
	}	


	TCPTransfer::Status TCPTransfer::getFTPStatus_()
		throw()
	{
		status_ = UNKNOWN_ERROR;
		String temp;
		for (Position pos = 0; pos < 3 && pos < (Position) received_bytes_; pos++)
		{
			temp += buffer_[pos];
		}
		try
		{
			status_ = (Status)temp.toUnsignedInt();
		}
		catch(Exception::InvalidFormat)
		{
		}	
		
		return status_;
	}


	void TCPTransfer::output_()
		throw()
	{
		Log.info() << "<<";
		for (Position pos = 0; pos < (Position) received_bytes_; pos++)
		{
			if (buffer_[pos] == '\0' || buffer_[pos] == '\n' || buffer_[pos] == '\r') 
			{
				break;
			}
			Log.info() << buffer_[pos];
		}
		Log.info() << "|" << received_bytes_;
	}


	TCPTransfer::Status TCPTransfer::sendData_(const String& query, Socket socket)
		throw()
	{
		#ifdef DEBUG
			Log.info() << "\n>>" << query << std::endl;
		#endif
				
		if (send(socket, query.c_str(), query.size(), 0) != (int) query.size())
		{
			status_ = SEND_ERROR;
			return status_;
		}
		return (Status)OK;
	}


	bool TCPTransfer::waitForOutput_(const String& key, Size seconds)
		throw()
	{
		setBlock_(socket_, false);
		Timer timer;
		timer.start();
		while (timer.getClockTime() < seconds)
		{
#			ifdef BALL_USE_WINSOCK
			received_bytes_ = (int)::recv(socket_, buffer_, BUFFER_SIZE, 0);
#			else
			received_bytes_ = read(socket_, buffer_, BUFFER_SIZE);
#			endif
			if (received_bytes_ > 0)
			{
				buffer_[received_bytes_] = '\0';
				String temp(buffer_);
				
				#ifdef DEBUG
					output_();			
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
		throw()
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
#			ifdef BALL_USE_WINSOCK
			received_bytes_ = (int)::recv(socket_, buffer_, BUFFER_SIZE, 0);
#			else
			received_bytes_ = read(socket_, buffer_, BUFFER_SIZE);
#			endif
			if (received_bytes_ > 0)
			{
				buffer_[received_bytes_] = '\0';
				#ifdef DEBUG
					output_();			
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
		while (timer.getClockTime() < 20);
		
		if (!last_line2)
		{
			status_ = RECV_ERROR;
			return false; 
		}
		
		return true;
	}

	TCPTransfer::Status TCPTransfer::getFTP_()
		throw()
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
		catch(Exception::InvalidFormat)
		{
			// we dont need anything to do here
			// passv_port == 0
		}

		if (passv_port == 0)
		{
			return PORT_ERROR;
		}

		struct hostent* ht = gethostbyname(passv_host.c_str());
		if (ht == NULL)
		{
			status_ = GETHOSTBYNAME_ERROR;
			return status_;
		}  

		Socket socket2 = socket(AF_INET, SOCK_STREAM, 0); 
		if (socket2 == -1)
		{
			status_ = (Status)SOCKET_ERROR;
			return status_;
		}  

		struct sockaddr_in host;  
		host.sin_family = AF_INET;
		host.sin_port	  = htons(passv_port);
		host.sin_addr 	= *(struct in_addr*)ht->h_addr;  
		
		// now connecting to passive ftp port
		if(connect(socket2, (struct sockaddr*)&host, sizeof(struct sockaddr)) == -1)
		{
			close(socket2);
			status_ = CONNECT_ERROR;
			return status_;
		}
		setBlock_(socket2, false);
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
			close(socket2);
			return SEND_ERROR;
		}
		

		if (!getFTPMessage_(150)) 
		{
			close(socket2);	
			return status_;	
		}		
		
		// ----------------------------------- receive the file
		received_bytes_ = 0;
		int control_bytes = -1;
		setBlock_(socket_, false);

		int bytes = -1;
		while (control_bytes < 1 && bytes != 0)
		{			
#			ifdef BALL_USE_WINSOCK
			bytes = (int)::recv(socket2, buffer_, BUFFER_SIZE, 0);
#			else
			bytes = read(socket2, buffer_, BUFFER_SIZE);
#			endif

			if (bytes > 0)
			{
				for (Position i = 0; i < (Position)bytes; i++)
				{
					(*fstream_) << buffer_[i];
				}
				received_bytes_ += bytes;
			}
#			ifdef BALL_USE_WINSOCK
			control_bytes = (int)::recv(socket_, buffer_, BUFFER_SIZE, 0);
#			else
			control_bytes = read(socket_, buffer_, BUFFER_SIZE);
#			endif			
			#ifdef DEBUG
				if (control_bytes > 0)
				{	
					Log.info() << "\n<<\n";			
					for (Position i = 0; i < (Position)control_bytes; i++) { Log.info() <<  buffer_[i]; }
					Log.info() << "\n";			
				}
			#endif
		}
		// im moment noch kein zeitabbruch	
		
		// we dont need the second socket anymore
		close(socket2);
		
		
		if (bytes == 0)
		{
			return (Status)OK;
		}
		
		if (control_bytes < 1)
		{
			status_ = RECV_ERROR;
			return status_;
		}

		buffer_[control_bytes] = '\0';

		if (!getFTPMessage_(226)) return status_;

		return (Status)OK;
	}

} // namespace BALL

