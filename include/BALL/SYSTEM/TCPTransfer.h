// $Id: TCPTransfer.h,v 1.2 2001/09/10 12:49:28 amoll Exp $

#ifndef BALL_SYSTEM_TCPTRANSFER
#define BALL_SYSTEM_TCPTRANSFER

#ifndef BALL_COMMON_H
# include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
# include <BALL/DATATYPE/string.h>
#endif

#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h> 
#include <unistd.h>

#include <sys/ioctl.h>
#include <fstream>
//#include <stdio.h>

#define BUFFER_SIZE 1024

namespace BALL
{

	class TCPTransfer
	{
		public:

			/// typdef for Socket
			typedef int Socket;

			/** Enumeration of some status codes.
					In addition to these TCPTransfer can also return the
					status codes of HTTP and FTP.
			*/
			enum Status
			{
				NO_ERROR 								= 0,
				GETHOSTBYNAME_ERROR 		= 1,
				SOCKET_ERROR 						= 2,
				CONNECT_ERROR 					= 3,
				RECV_ERROR 							= 4,
				OUTOFMEMORY_ERROR 			= 5,
				BODY_ERROR 							= 6,
				UNKNOWN_ERROR 					= 7,
				ADDRESS_ERROR 					= 8,
				UNINITIALIZED_ERROR 		= 9,
				TRANSFER_ERROR					= 10,
				SEND_ERROR							= 11,
				PORT_ERROR							= 12,
				UNKNOWN_PROTOCOL_ERROR 	= 13,
				FILENOTFOUND_ERROR 			= 404
			};

			/** Enumeration of the protocols.
			 */
			enum Protocol
			{
				UNKNOWN_PROTOCOL 	= 0,
				HTTP_PROTOCOL			= 1,
				FTP_PROTOCOL			= 2
			};
			
			/** Default constructor.
					The instance is set to a non valid state, by setting
					its state to UNINITIALIZED_ERROR.
			*/
			TCPTransfer()
				throw()
			{	
				clear();
			}

			/** Detailled constructor.
			 *	The file from the given address is downloaded and
			 *	stored in the ofstream instance.
			 *	You can specify protocol, port and a login/pass by using
			 *	the syntax: \\
			 *	http[ftp]://login:pass@server.com:port/fileaddress
			 */
			TCPTransfer(ofstream& file, const String& address)
				throw();
			
			/** Clear method.
			 *  All attributes are set to default values and the status is set
			 *  to UNINITIALIZED_ERROR.
			 */
			void clear()
				throw();
		
			/** Set method.
			 * 	You can set a new file and address, but the transfer is not
			 * 	yet done. To do that, use transfer() afterwards.
			 */
			bool set(ofstream& file, const String& address)
				throw();
			
			/** Detailled set method.
			 *  @see set(ofstream& file, const String& address)
			 */
			void set(ofstream& file, 
							 Protocol protocol, 
							 const String& host_address, 
							 const String& file_address,
							 const String& login,
							 const String& password,
							 Position port = 80)
				throw()
			{
				clear();
				protocol_ 		= protocol;
				host_address_ = host_address;
				file_address_ = file_address;
				login_ 				= login;
				password_			= password;
				port_ 				= port;
				fstream_ 			= &file;
			}

			/** Return the host address.
			*/
			const String& getHostAddress() const
				throw()
			{
				return host_address_;
			}

			/// Return the file address.
			const String& getFileAddress() const
				throw()
			{
				return file_address_;
			}

			/// Return the port number.
			Position getPort() const
				throw()
			{
				return port_;
			}

			/// Return the status code.
			Position getStatusCode() const
				throw()
			{
				return status_;
			}
				
			/// Return the amount of received bytes.
			Position getReceivedBytes() const
				throw()
			{
				return received_bytes_;
			}
			
			/// Return the protocol.
			Protocol getProtocol() const
				throw()
			{
				return protocol_;
			}
			
			/// Return the login.
			const String& getLogin() const
				throw()
			{
				return login_;
			}

			/// Return the password.
			const String& getPassword() const
				throw()
			{
				return password_;
			}

			/// Get a pointer to the stream.
			const ofstream* getStream() const
				throw()
			{
				return fstream_;
			}
					
			/** Return a pointer to the buffer.
			 		Remember that the buffer may contain NULL-chars.
					The length of the buffer is less than BUFFER_LENGTH.
					This method should only be used for debuging.
			*/
			const char* getBuffer() const
				throw()
			{
				return buffer_;
			}

			/** Transfer method.
					If the address is specified, the file can be transfered
					whith this method.
					@return Status the status of the transfer/instance
					@see Status
			*/
			Status transfer()
				throw();
			
			protected:
				
				String 			host_address_;
				String 			file_address_;
				Position 		port_;
				String			login_;
				String 			password_;
				Status			status_;
				int			 		received_bytes_;
				Protocol 		protocol_;
				char* 			buffer_;
				ofstream*		fstream_;
				Socket			socket_;
				
				//_ Specified method for transfering per FTP-protocol
				Status	 		getFTP_();
				
				//_ Specified method for transfering per HTTP-protocol
				Status	 		getHTTP_();

				/*_ Logon to a server.
				 *  @param query string to send to the server as first contact
				 */
				Status 			logon_(const String& query);
				
				//_ Compute the status of a ftp server from its response
				Status			getFTPStatus_();

				//_ Set a socket to blocking or nonblocking mode.
				Status			setBlock_(Socket socket, bool block = true);

				//_ Wait a given time for output from the Socket.
				bool				waitForOutput_(const String& key, Size seconds);
				
				//_ Debug method
				void				output_();
	};

}

#endif // BALL_SYSTEM_TCPTRANSFER_H 
