// $Id: TCPTransfer.C,v 1.1 2001/09/09 23:51:42 amoll Exp $

#include <BALL/SYSTEM/TCPTransfer.h>
#include <BALL/SYSTEM/timer.h>

#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h> 
#include <unistd.h>

#include <sys/ioctl.h>
#include <fstream>
#include <stdio.h>

//using namespace std;

namespace BALL
{

// no support for passworts yet
TCPTransfer::TCPTransfer(ofstream& file, const String& address)
	throw() 
{
	buffer_ = new char[BUFFER_SIZE];
	if (!set(file, address))
	{
		return; 
	}

	status_ = transfer();
}

TCPTransfer::Status TCPTransfer::transfer()
	throw()
{
	if (protocol_ == FTP_PROTOCOL)
	{
		return getFTP_();
	}
	
	if (protocol_ == HTTP_PROTOCOL)
	{
		return getHTTP_();
	}

	close(socket_);
	return UNINITIALIZED_ERROR;
}

bool TCPTransfer::set(ofstream& file, const String& address)
	throw()
{
	clear();
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
		
	file_address_ = host_address_.getSubstring(host_address_.getField(0, "/").size()); 
	host_address_ = host_address_.getField(0, "/");
	
	if (file_address_.isEmpty() || host_address_.isEmpty())
	{
		return false;
	}
	
	if (host_address_.has(':'))
	{
		try
		{
			port_	= host_address_.getField(1, ":").toUnsignedInt();
		}
		catch(Exception::InvalidFormat)
		{
			return false;
		}
		host_address_ = host_address_.getField(0, ":");
	}

	status_ = NO_ERROR;
	
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
	
	close(socket_);
}

TCPTransfer::Status TCPTransfer::getHTTP_()
{
	String query;
	query  = "GET /" + file_address_ + " HTTP/1.0\n\r";
	query += "Accept: */*\n\r";
	query += "User-Agent: inet.c\n\r\n\r";
	Status status = logon_(query);
	if (status != NO_ERROR)
	{
		return status;
	}

	int bytes = received_bytes_;
	// ==================================== get the status from the server
	String first_line;
	for (Position pos = 0; pos < (Position) bytes; pos++)
	{
		if (buffer_[pos] == '\n')
		{
			break;
		}
		first_line += buffer_[pos];
	}
	
	try
	{
		status = (Status) first_line.getField(1).toUnsignedInt();
		if (status != 200)
		{
			return status;
		}
	}
	catch(Exception::InvalidFormat)
	{
		return UNKNOWN_ERROR;
	}

	// ===================================== now cutting of the head
	Position pos = 0; 
	for (;pos < (Position)bytes; pos++)
	{
		if (buffer_[pos] == '\n' && buffer_[pos + 1] == '\r')
		{
			break;
		}
	}
	if (pos == 0)
	{
		return BODY_ERROR;
	}
	
	// ==================================== receive the rest
	pos += 3;
	for (Position i = pos; i < (Position)bytes; i ++)
	{
		(*fstream_) << buffer_[i];
	}
	received_bytes_ = bytes - pos;

	do
	{
		bytes = read(socket_, buffer_, BUFFER_SIZE);
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

	return NO_ERROR;
}
	
TCPTransfer::Status TCPTransfer::setBlock_(Socket socket, bool block)
{
	int temp = !block;
	if (ioctl(socket, FIONBIO, &temp) == -1)
	{
		return CONNECT_ERROR;
	}

	return NO_ERROR;
}

TCPTransfer::Status TCPTransfer::logon_(const String& query)
{
	status_ = UNKNOWN_ERROR;
	
	struct hostent* ht = gethostbyname(host_address_.c_str());
	if (ht == NULL)
	{
		status_ = GETHOSTBYNAME_ERROR;
		return status_;
	}  

	socket_ = socket(AF_INET, SOCK_STREAM, 0); 
	if (socket_ == -1)
	{
		status_ = SOCKET_ERROR;
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
		if (send(socket_, query.c_str(), query.size(), 0) != (int) query.size())
		{
			status_ = SEND_ERROR;
			return status_;
		}
	}

	received_bytes_ = read(socket_, buffer_, BUFFER_SIZE);
	if (received_bytes_ < 0)
	{
		status_ = RECV_ERROR;
		return status_;
	}
	buffer_[received_bytes_] = '\0';
	
	status_ = NO_ERROR;
	return status_;
}	

TCPTransfer::Status TCPTransfer::getFTPStatus_()
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
{
	for (Position pos = 0; pos < (Position) received_bytes_; pos++)
	{
		cout << buffer_[pos];
	}
}

bool TCPTransfer::waitForOutput_(const String& key, Size seconds)
{
	setBlock_(socket_, false);
	Timer timer;
	timer.start();
	while (timer.getClockTime() < seconds)
	{
		String temp(buffer_);
		if (key.size() > 0 && temp.hasSubstring(key))
		{
			setBlock_(socket_, true);
			return true;
		}
	
		for (Position pos = 0; pos < 5; pos ++)
		{
			received_bytes_ = read(socket_, buffer_, BUFFER_SIZE);
			sleep(1);
			if (received_bytes_ > 0)
			{
				buffer_[received_bytes_] = '\0';
				break;
			}
		}	
	}

	setBlock_(socket_, true);
	return false;
}

TCPTransfer::Status TCPTransfer::getFTP_()
{
	Status status = logon_("");
	if (status != NO_ERROR) 
	{
		return status;
	}

	if (getFTPStatus_() != 220)
	{
		return status_;
	}

	if (!waitForOutput_("ready.", 20))
	{
		status_ = BODY_ERROR;
		return status_;
	}

	//================================================== login
	String query;
	if (login_.isEmpty())
	{
		query = "USER anonymous\n";
	}
	else
	{
		query = "USER "+ login_ + "\n";
	}
	
	if (send(socket_, query.c_str(), query.size(), 0) != (int) query.size())
	{
		return SEND_ERROR;
	}
	
	received_bytes_ = read(socket_, buffer_, BUFFER_SIZE);
	if (received_bytes_ < 0)
	{
		return RECV_ERROR;
	}
	
	if (getFTPStatus_() != 331)
	{
		return status_;
	}
	
	//================================================ password
	if (password_.isEmpty())
	{
		query = "PASS nobody@nowhereland.com\n";
	}
	else
	{	
	 query = "PASS " + password_ + "\n";
	}
	
	if (send(socket_, query.c_str(), query.size(), 0) != (int) query.size())
	{
		return SEND_ERROR;
	}
	
	received_bytes_ = read(socket_, buffer_, BUFFER_SIZE);
	if (received_bytes_ < 0)
	{
		return RECV_ERROR;
	}

	if (getFTPStatus_() != 230)
	{
		return status_;
	}

	//================================================ opening passive connection
	waitForOutput_("", 5);
	query = "PASV\n";
	if (send(socket_, query.c_str(), query.size(), 0) != (int) query.size())
	{
		return SEND_ERROR;
	}

	received_bytes_ = read(socket_, buffer_, BUFFER_SIZE);
	if (received_bytes_ < 0)
	{
		return RECV_ERROR;
	}
	buffer_[received_bytes_] = '\0';
	if (getFTPStatus_() != 227)
	{
		return status_;
	}

	Position passv_port = 0;
	String 	 passv_host;
	try
	{
		String temp(buffer_);
		temp = temp.getField(1, "(");
		temp = temp.getField(0, ")");

		passv_host = temp.getField(0, ",") + "." +
								 temp.getField(1, ",") + "." +
								 temp.getField(2, ",") + "." +
								 temp.getField(3, ",");
		
		passv_port = temp.getField(4, ",").toUnsignedInt() * 256 + 
								 temp.getField(5, ",").toUnsignedInt();
	}
	catch(Exception::InvalidFormat)
	{
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
		status_ = SOCKET_ERROR;
		return status_;
	}  

	struct sockaddr_in host;  
	host.sin_family = AF_INET;
	host.sin_port	  = htons(passv_port);
	host.sin_addr 	= *(struct in_addr*)ht->h_addr;  
	
	if(connect(socket2, (struct sockaddr*)&host, sizeof(struct sockaddr)) == -1)
	{
		status_ = CONNECT_ERROR;
		return status_;
	}
	setBlock_(socket2, false);
	
	query = "RETR " + file_address_ + '\n';
	if (send(socket_, query.c_str(), query.size(), 0) != (int) query.size())
	{
		close(socket2);
		return SEND_ERROR;
	}
	
	// ----------------------------------- test if server will send file
	setBlock_(socket_, true);
	received_bytes_ = read(socket_, buffer_, BUFFER_SIZE);

	if (received_bytes_ < 1)
	{
		close(socket2);
		status_ = RECV_ERROR;
		return status_;
	}

	buffer_[received_bytes_] = '\0';
	String temp(buffer_);
	temp = temp.getSubstring(0, 3);
	if (temp != "150")
	{ 
		close(socket2);
		return (Status) temp.toUnsignedInt();
	}
	
	// ----------------------------------- receive the file
	received_bytes_ = 0;
	int control_bytes = -1;
	setBlock_(socket_, false);

	int bytes = -1;
	while (control_bytes < 1 && bytes != 0)
	{			
		bytes = read(socket2, buffer_, BUFFER_SIZE);
		if (bytes > 0)
		{
			for (Position i = 0; i < (Position)bytes; i++)
			{
				(*fstream_) << buffer_[i];
			}
			received_bytes_ += bytes;
		}
		control_bytes = read(socket_, buffer_, BUFFER_SIZE);
	}
	// im moment noch kein zeitabbruch	
	
	close(socket2);
	
	if (bytes == 0)
	{
		return NO_ERROR;
	}
	
	if (control_bytes < 1)
	{
		status_ = RECV_ERROR;
		return status_;
	}

	buffer_[control_bytes] = '\0';
	temp = buffer_;
	temp = temp.getSubstring(0, 3);


	if (temp != "226")
	{
		return (Status) temp.toUnsignedInt();
	}

	return NO_ERROR;
}

} // namespace BALL
