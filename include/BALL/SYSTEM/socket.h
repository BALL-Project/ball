// $Id: socket.h,v 1.10 2000/01/14 20:42:41 oliver Exp $

#ifndef BALL_SYSTEM_SOCKET_H
#define BALL_SYSTEM_SOCKET_H


// ORIGINAL COPYRIGHT DISCLAIMER
////////////////////////////////
// sockinet.h -*- C++ -*- socket library
// Copyright (C) 1992,1993,1994 Gnanasekaran Swaminathan <gs4t@virginia.edu>
//
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided the above copyright
// notice and this paragraph are preserved on all copies.
// This software is provided "as is" with no express or implied warranty.
//
// Version: 17Oct95 1.10
 
#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif
#ifndef BALL_DATATYPE_STRING_H
#	include <BALL/DATATYPE/string.h>
#endif

#include <iostream>
#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/socket.h>
#include <netinet/in.h>

#ifndef _G_config_h
	// non libg++ or g++ compilation
#	define _S_NOLIBGXX

#	define _S_USER_BUF		0x0001
# define _S_UNBUFFERED	0x0002
# define _S_NO_READS		0x0004
# define _S_NO_WRITES		0x0008
# define _S_EOF_SEEN		0x0010
# define _S_ERR_SEEN		0x0020
# define _S_DELETE_DONT_CLOSE	0x0040
# define _S_LINKED		0x0080
# define _S_LINE_BUF		0x0200
# define _G_ssize_t size_t
#else
# ifndef _S_USER_BUF // libg++ 2.5.x
#   define _S_USER_BUF		_IO_USER_BUF		
#		define _S_UNBUFFERED		_IO_UNBUFFERED		
#		define _S_NO_READS		_IO_NO_READS		
#		define _S_NO_WRITES		_IO_NO_WRITES		
#		define _S_EOF_SEEN		_IO_EOF_SEEN		
#		define _S_ERR_SEEN		_IO_ERR_SEEN		
#		define _S_DELETE_DONT_CLOSE	_IO_DELETE_DONT_CLOSE	
#		define _S_LINKED		_IO_LINKED		
#		define _S_LINE_BUF		_IO_LINE_BUF		
#	endif //  !_S_USER_BUF
#endif //  !_G_config_h

// linux leaves these things out. We define it for compatitbility
// not for their use.
#ifdef BALL_OS_LINUX
#	define SO_ACCEPTCONN	0x0002
#	define SO_USELOOPBACK	0x0040
#	define SO_SNDLOWAT	0x1003
#	define SO_RCVLOWAT	0x1004
#	define SO_SNDTIMEO	0x1005
#	define SO_RCVTIMEO	0x1006
#	define MSG_MAXIOVLEN	16
#	ifndef SOMAXCONN
#		define SOMAXCONN	5
#	endif // SOMAXCONN
#endif // LINUX


extern void sock_error (const char* classname, const char* error_message);

namespace BALL 
{

	/**	@name	Network socket support */
	//@{

	typedef int sockexcept;

	/**	
	*/
	class SockAddr 
	{
		public:
		
		/** @name	Constructors and Destructors */
		//@{

		///
		virtual	~SockAddr() 
		{
		}
			
		//@}

		/**	@name Converters */
		//@{
		
		///
		virtual operator void* () const = 0;

		///
		operator sockaddr* () const 
		{ 
			return getAddr(); 
		}
		//@}

		/**	@name	Accessors */
		//@{

		///
		virtual int getSize() const = 0;

		///
		virtual int getFamily() const = 0;

		///
		virtual sockaddr* getAddr() const = 0;
		//@}

		void error (const char* errmsg) const;
	};

	/**	
	*/
	class SocketBuf
		: public std::streambuf 
	{
		public:

		/**	@name	Enumerations */
		//@{
			
		///
		enum type 
		{
			///
			sock_stream	= SOCK_STREAM,
			///
			sock_dgram	= SOCK_DGRAM,
			///
			sock_raw	= SOCK_RAW,
			///
			sock_rdm	= SOCK_RDM,
			///
			sock_seqpacket  = SOCK_SEQPACKET
		};

		///
		enum option 
		{
			///
			so_debug	= SO_DEBUG,
			///
			so_acceptconn	= SO_ACCEPTCONN,
			///
			so_reuseaddr	= SO_REUSEADDR,
			///
			so_keepalive	= SO_KEEPALIVE,
			///
			so_dontroute	= SO_DONTROUTE,
			///
			so_broadcast	= SO_BROADCAST,
			///
			so_useloopback	= SO_USELOOPBACK,
			///
			so_linger	= SO_LINGER,
			///
			so_oobinline	= SO_OOBINLINE,
			///
			so_sndbuf	= SO_SNDBUF,
			///
			so_rcvbuf	= SO_RCVBUF,
			///
			so_sndlowat	= SO_SNDLOWAT,
			///
			so_rcvlowat	= SO_RCVLOWAT,
			///
			so_sndtimeo	= SO_SNDTIMEO,
			///
			so_rcvtimeo	= SO_RCVTIMEO,
			///
			so_error	= SO_ERROR,
			///
			so_type		= SO_TYPE
		};	

		///
		enum level 
		{ 
			///
			sol_socket = SOL_SOCKET 
		};

		///
		enum msgflag 
		{
			///
			msg_oob		= MSG_OOB,
			///
			msg_peek	= MSG_PEEK,
			///
			msg_dontroute	= MSG_DONTROUTE,
			///
			msg_maxiovlen	= MSG_MAXIOVLEN
		};

		///
		enum shuthow 
		{
			///
			shut_read,
			///
			shut_write,
			///
			shut_readwrite
		};

		///
		enum 
		{ 
			///
			somaxconn	= SOMAXCONN 
		};
		
		///
		struct socklinger 
		{
			///
			int	l_onoff;	// option on/off
			///
			int	l_linger;	// linger time
			///
			socklinger (int a, int b): l_onoff (a), l_linger (b) {}
		};

		//@}

		protected:

		struct sockcnt 
		{
			int	sock;
			int	cnt;

			sockcnt(int s, int c): 
				sock(s), cnt(c) {}
		};
			
		sockcnt* 	rep;  // counts the # refs to sock
		int				stmo; // -1==block, 0==poll, >0 == waiting time in secs
		int				rtmo; // -1==block, 0==poll, >0 == waiting time in secs
			
		virtual int underflow();
		virtual int	overflow(int c = EOF);
		virtual int	doallocate();
		int flush_output();

#	ifdef _S_NOLIBGXX
		int	x_flags; // port to USL iostream

		int xflags() const 
		{ 
			return x_flags; 
		}

		int xsetflags(int f) 
		{ 
			return x_flags |= f; 
		}

		int xflags(int f)
		{ 
			int ret = x_flags; 
			x_flags = f; 
			return ret; 
		}

		void xput_char(char c) 
		{ 
			*pptr() = c; 
			pbump(1); 
		}

		int unbuffered() const
		{
			return x_flags & _S_UNBUFFERED;
		}

		int linebuffered() const 
		{ 
			return x_flags & _S_LINE_BUF; 
		}

#	endif // _S_NOLIBGXX
			
		public:

		/**	@name	Constructors and Destructors */
		//@{
			
		///
		SocketBuf(int soc = -1);

		///
		SocketBuf(int, type, int proto = 0);

		///
		SocketBuf(const SocketBuf&);

		///
		virtual ~SocketBuf();
		//@}

		/**	@name	Converters */
		//@{

		///
		SocketBuf& operator = (const SocketBuf&);

		///
		operator int() const 
		{ 
			return rep->sock; 
		}
		//@}
			
		virtual SocketBuf* open(type, int proto=0);

		virtual SocketBuf* close();

		virtual int sync();

		virtual _G_ssize_t sys_read (char* buf, _G_ssize_t len);

		virtual _G_ssize_t sys_write (const void* buf, long len);

		virtual std::streamsize xsputn(const char* s, std::streamsize n);

		int is_open() const 
		{ 
			return rep->sock >= 0; 
		}

		int is_eof()       
		{ 
			return xflags() & _S_EOF_SEEN; 
		}
		
		virtual int bind(SockAddr&);

		virtual int connect(SockAddr&);
		
		void listen(int num = somaxconn);

		virtual SocketBuf	accept();
		virtual SocketBuf	accept(SockAddr& sa);
		
		int read(void* buf, int len);
		int recv(void* buf, int len, int msgf = 0);
		int recvfrom(SockAddr& sa, void* buf, int len, int msgf = 0);

#	ifndef BALL_OS_LINUX
		int recvmsg (msghdr* msg, int msgf = 0);
		int sendmsg	(msghdr* msg, int msgf = 0);
#	endif
		
		int write(const void* buf, int len);
		int send(const void* buf, int len, int msgf = 0);
		int sendto(SockAddr& sa, const void* buf, int len, int msgf = 0);
		
		int	sendtimeout (int wp = -1);
		int	recvtimeout (int wp = -1);
		int	is_readready (int wp_sec, int wp_usec = 0) const;
		int	is_writeready (int wp_sec, int wp_usec = 0) const;
		int	is_exceptionpending (int wp_sec, int wp_usec = 0) const;
		
		void shutdown (shuthow sh);
		
		int	getopt(option op, void* buf,int len, level l = sol_socket) const;
		void setopt(option op, void* buf,int len, level l = sol_socket) const;
		
		type gettype() const;
		int clearerror() const;
		int debug(int opt = -1) const;
		int reuseaddr(int opt = -1) const;
		int keepalive(int opt = -1) const;
		int dontroute(int opt = -1) const;
		int broadcast(int opt = -1) const;
		int oobinline(int opt = -1) const;
		int linger(int tim = -1) const;
		int sendbufsz(int sz = -1)   const;
		int recvbufsz(int sz = -1)   const;
		
		void error (const char* errmsg) const;
	};

#ifdef BALL_HAS_ANSI_IOSTREAM
#	define BALL_IOS std::basic_ios<char>
#	define BALL_ISTREAM std::istream
#	define BALL_OSTREAM std::ostream
#	define BALL_IOSTREAM std::iostream
#else
#	define BALL_IOS ios
#	define BALL_ISTREAM istream
#	define BALL_OSTREAM ostream
#	define BALL_IOSTREAM iostream
#endif

	/**	
	*/
	class ISockStream
		: public std::istream 
	{
		protected:

		ISockStream()
			: BALL_IOS(0),
				BALL_ISTREAM(0)
		{
		}

		public:
						
		ISockStream(SocketBuf* sb)
			: BALL_IOS(sb),
				BALL_ISTREAM(sb)
		{
			if (rdbuf() == 0)
			{
				throw Exception::NullPointer(__FILE__, __LINE__);
			}
		}

		~ISockStream();
							

		SocketBuf* rdbuf() 
		{ 
			return (SocketBuf*)BALL_ISTREAM::rdbuf(); 
		}

		SocketBuf* operator -> () 
		{ 
			return rdbuf(); 
		}

		void error(const char* errmsg) const;
	};

	/**	
	*/
	class OSockStream
		: public std::ostream 
	{
		protected:

		OSockStream()
			: BALL_IOS(0),
				BALL_OSTREAM(0)
		{
		}

		public:

		/**	@name	Constructors and Destructors */
		//@{

		///
		OSockStream(SocketBuf* sb)
			: BALL_IOS(sb),
				BALL_OSTREAM(sb)
		{
			if (rdbuf() == 0)
			{
				throw Exception::NullPointer(__FILE__, __LINE__);
			}
		}

		/// Destructor
		virtual ~OSockStream();

		//@}

		/**	@name	Miscellaneous */
		//@{
		///
		SocketBuf* rdbuf() 
		{ 
			return(SocketBuf*)BALL_OSTREAM::rdbuf(); 
		}

		///
		SocketBuf* operator -> () 
		{ 
			return rdbuf(); 
		}

		//@}

		void error (const char* errmsg) const;
	};

	/**	
	*/
	class SockInetAddr
		: public SockAddr, 
			public sockaddr_in 
	{
		protected:

		void setport (const String& service_name, const String& protocol_name= "tcp");

		void setaddr (const String& hostname);


		public:

		/**	@name	Construtors and Destructors */
		//@{

		/// Default constructor
		SockInetAddr();

		/// Creates a SockInetAddr object from an unsigned int (containing the IP address) and a port number
		SockInetAddr(unsigned long addr, int port_no = 0);

		/// Creates a SockInetAddr object from a given hostname and port number
		SockInetAddr(const String& host_name, int port_no = 0);

		/// 
		SockInetAddr(unsigned long addr, const String& service_name, const String& protocol_name = "tcp");

		///
		SockInetAddr(const String& host_name, const String& service_name, const String& protocol_name = "tcp");

		///
		SockInetAddr(const SockInetAddr& sina);    

		/// Destructor
		~SockInetAddr() 
		{
		}
		//@}

		/**	@name	Converters */
		//@{

		///
		operator void* () const 
		{ 
			return (sockaddr_in*)this; 
		}
		//@}
			
		/**	@name	Accessors */
		//@{

		///
		int getSize() const 
		{ 
			return sizeof (sockaddr_in); 
		}

		///
		int getFamily() const 
		{ 
			return sin_family; 
		}
	
		///
		sockaddr* getAddr() const 
		{
			return (sockaddr*)((sockaddr_in*)this); 
		}
			
		///
		int	getPort() const;

		///
		const String&	getHostname() const;

		//@}
	};

	/**	
	*/
	class SockInetBuf
		: public SocketBuf 
	{
		protected:

		SockInetBuf& operator = (const SocketBuf& si); // needs to be fixed

		SockInetBuf& operator = (const SockInetBuf& si); // needs fixing

		public:

		/**	@name	Enumerations */
		//@{

		///
		enum domain 
		{ 
			af_inet = AF_INET 
		};
		//@}
			
		/**	@name	Constructors and Destructors */
		//@{

		///
		SockInetBuf(const SocketBuf& si)
			: SocketBuf(si) 
		{
		}

		///
		SockInetBuf(const SockInetBuf& si)
			: SocketBuf (si) 
		{
		}
		//@}

		///
		SockInetBuf(SocketBuf::type ty, int proto = 0);
		
		///
		SocketBuf* open(SocketBuf::type, int proto=0);
		
		///
		SockInetAddr localaddr() const;

		///
		int localport() const;

		///
		const String& localhost() const;
		
		///
		SockInetAddr peeraddr() const;

		///
		int	peerport() const;

		///
		const String&	peerhost() const;


		/**	@name	Binding */
		//@{

		///
		int bind_until_success(int portno);

		///
		virtual int	bind(SockAddr& sa);

		///
		int	bind();

		///
		int	bind(unsigned long addr, int port_no = 0);

		///
		int	bind(const String& host_name, int port_no = 0);

		///
		int	bind(unsigned long addr, const String& service_name, const String& protocol_name = "tcp");

		///
		int	bind(const String& host_name, const String& service_name, const String& protocol_name = "tcp");

		//@}


		/**	@name	Connecting */
		//@{

		///
		virtual int	connect(SockAddr& sa);

		///
		int connect(unsigned long addr, int port_no = 0);

		///
		int connect(const String& host_name, int port_no = 0);

		///
		int connect(unsigned long addr, const String& service_name,  const String& protocol_name = "tcp");

		///
		int connect (const String& host_name, const String& service_name, const String& protocol_name = "tcp");
		//@}
	};

	/**	
	*/
	class IOSockStream
		: public std::iostream 
	{
		protected:

		IOSockStream()
			: BALL_IOSTREAM(0)
		{
		}

		public:
						
		/**	@name	Constructors and Destructors */
		//@{

		/// Constructor
		IOSockStream(SocketBuf* sb)
			: std::iostream(sb)
		{
			BALL_IOS::rdbuf(sb);
			if (rdbuf() == 0)
			{
				throw Exception::NullPointer(__FILE__, __LINE__);
			}
		}
		
		/// Destructor
		virtual ~IOSockStream();
		//@}

		/**	@name	Miscellaneous */
		//@{

		///
		SocketBuf* rdbuf() 
		{ 
			return (SocketBuf*)BALL_IOSTREAM::rdbuf(); 
		}
		
		///
		SocketBuf* operator -> () 
		{ 
			return rdbuf(); 
		}

		//@}
		void error (const char* errmsg) const;
	};

	/**	
	*/
	class IOStreamSocket
		: public IOSockStream
	{
		public:

		/**	@name	Constructors and Destructors */
		//@{

		///
		IOStreamSocket(const SocketBuf& sb);
				
		///
		IOStreamSocket(SocketBuf::type ty=SocketBuf::sock_stream, int proto = 0);
		
		/// Destructor 
		virtual ~IOStreamSocket();
		//@}

		/**	@name	Miscellaneous */
		//@{

		///
		SockInetBuf* rdbuf() 
		{ 
			return (SockInetBuf*)IOSockStream::rdbuf(); 
		}

		///
		SockInetBuf* operator -> () 
		{
			return rdbuf(); 
		}
		//@}

	};

	//@}

} // namespace BALL

#endif	// BALL_SYSTEM_SOCKET_H
