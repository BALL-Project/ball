// $Id: socket.h,v 1.24 2001/12/30 13:28:44 sturm Exp $

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

#ifdef BALL_HAS_ANSI_IOSTREAM
#	ifndef _S_USER_BUF
#		define _S_USER_BUF		0x0001
#		define _S_UNBUFFERED	0x0002
#		define _S_NO_READS		0x0004
#		define _S_NO_WRITES		0x0008
#		define _S_EOF_SEEN		0x0010
#		define _S_ERR_SEEN		0x0020
#		define _S_DELETE_DONT_CLOSE	0x0040
#		define _S_LINKED		0x0080
#		define _S_LINE_BUF		0x0200
#	endif
# ifndef _G_ssize_t
#		define _G_ssize_t size_t
#	endif
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
#if (defined(BALL_OS_LINUX) || defined(__CYGWIN__))
# ifndef SO_ACCEPTCONN
#		define SO_ACCEPTCONN	0x0002
# endif
# ifndef SO_USELOOPBACK
#		define SO_USELOOPBACK	0x0040
#	endif
#	ifndef SO_SNDLOWAT
#		define SO_SNDLOWAT	0x1003
#	endif
#	ifndef SO_RCVLOWAT
#		define SO_RCVLOWAT	0x1004
#	endif
#	ifndef SO_SNDTIMEO
#		define SO_SNDTIMEO	0x1005
#	endif
#	ifndef SO_RCVTIMEO
#		define SO_RCVTIMEO	0x1006
#	endif
#	define MSG_MAXIOVLEN	16
#	ifndef SOMAXCONN
#		define SOMAXCONN	5
#	endif // SOMAXCONN
#endif // LINUX


namespace BALL 
{
	
	void errnoError_(const char* method_name);

	/**	@name	Network socket support 
	*/
	//@{

	typedef int sockexcept;


	/**	Socket addresses. This is an interface definition only.
			{\bf Definition:} \URL{BALL/SYSTEM/socket.h}
	*/
	class SockAddr 
	{
		public:
		
		BALL_CREATE(SockAddr)

		/** @name	Constructors and Destructors 
		*/
		//@{

		/// Default constructor
		SockAddr()
			throw();

		/// Copy constructor
		SockAddr(const SockAddr& sock_addr)
			throw();

		/// Destructor
		virtual	~SockAddr()
			throw();
			
		//@}
		/**	@name Converters 
		*/
		//@{
		
		/// Convert a SockAddr into void*
		virtual operator void* () const
			throw();

		/// Convert a SockAddr to standard sockaddr*
		operator sockaddr* () const 
			throw();

		//@}
		/**	@name	Accessors 
		*/
		//@{

		/// Get the size (of what?)
		virtual int getSize() const
			throw();

		/// Get the socket family
		virtual int getFamily() const
			throw();

		/// Get the actual address as a standard sockaddr
		virtual sockaddr* getAddr() const
			throw();

		//@}
	};



	/**	Socket buffer class.
			{\bf Definition:} \URL{BALL/SYSTEM/socket.h}
	*/
	class SocketBuf
		: public std::streambuf 
	{
		public:

		/**	@name	Enumerations 
		*/
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
			/// option on/off
			int	l_onoff;	
			/// linger time
			int	l_linger;	
			/// ?????
			socklinger (int a, int b)
				: l_onoff (a), 
					l_linger (b)
			{
			}
		};

		//@}

		protected:

		struct sockcnt
		{
			//_
			int	sock;
			//_
			int	cnt;

			//_
			sockcnt(int s, int c)
				: sock(s),
					cnt(c)
			{
			}

		};

		//_ counts the # refs to sock	
		sockcnt* rep;

		/*_ Send timeout (I assume)
				-1==block, 0==poll, >0 == waiting time in secs
		*/
		int	stmo; 

		/*_ Receive timeout (I assume)
				-1==block, 0==poll, >0 == waiting time in secs
		*/
		int	rtmo; 
			
		//_
		virtual int underflow()
			throw();

		//_ 
		virtual int	overflow(int c = EOF)
			throw();

		//_ 
		virtual int	doallocate()
			throw();

		//_
		int flush_output()
			throw();

#	ifdef BALL_HAS_ANSI_IOSTREAM

		//_ port to USL iostream
		int	x_flags; 

		// ?????:
		// The following names don't follow our naming convention!

		//_
		int xflags() const
			throw()
		{ 
			return x_flags; 
		}

		//_
		int xflags(int f)
		{ 
			int ret = x_flags; 
			x_flags = f; 
			return ret; 
		}

		//_
		int xsetflags(int f) 
		{ 
			return x_flags |= f; 
		}

		//_
		void xput_char(char c) 
		{ 
			*pptr() = c; 
			pbump(1); 
		}

		//_
		int unbuffered() const
		{
			return x_flags & _S_UNBUFFERED;
		}

		//_
		int linebuffered() const 
		{ 
			return x_flags & _S_LINE_BUF; 
		}

#	endif // BALL_HAS_ANSI_IOSTREAM
			
		public:

		/**	@name	Constructors and Destructors 
		*/
		//@{
			
		/// Default Constructor
		//SocketBuf()
		//	throw();

		///
		SocketBuf(int soc = -1)
			throw();

		/// Detailed constructor
		SocketBuf(int domain, type socket_type, int proto = 0)
			throw();

		/// Copy constructor
		SocketBuf(const SocketBuf& socket_buf)
			throw();

		/// Destructor
		virtual ~SocketBuf()
			throw();

		//@}
		/** @name Assignment
		*/
		//@{

		/// Assignment operator
		SocketBuf& operator = (const SocketBuf&)
			throw();

		//@}
		/**	@name	Converters 
		*/
		//@{

		/// Convert this SocketBuf to int.
		operator int() const 
			throw();

		//@}
			
		///
		virtual SocketBuf* open(type socket_type, int proto = 0)
			throw();

		///
		virtual SocketBuf* close()
			throw();

		///
		virtual int sync()
			throw();

		/// return EOF on eof, 0 on timeout, and number of chars read on success
		virtual _G_ssize_t sys_read (char* buf, _G_ssize_t len)
			throw();

		/// return written_length; < len indicates error
		virtual _G_ssize_t sys_write (const void* buf, long len)
			throw();

		///
		virtual std::streamsize xsputn(const char* s, std::streamsize n)
			throw();

		///
		int is_open() const
			throw();
		///
		int is_eof()
			throw();
		
		///
		virtual int bind(SockAddr&)
			throw();

		///
		virtual int connect(SockAddr&)
			throw();
		
		///
		void listen(int num = somaxconn)
			throw();

		///
		virtual SocketBuf	accept()
			throw();
		///
		virtual SocketBuf	accept(SockAddr& sa)
			throw();
		
		///
		int read(void* buf, int len)
			throw();
		///
		int recv(void* buf, int len, int msgf = 0)
			throw();
		///
		int recvfrom(SockAddr& sa, void* buf, int len, int msgf = 0)
			throw();

		///
		int write(const void* buf, int len)
			throw();
		///
		int send(const void* buf, int len, int msgf = 0)
			throw();
		///
		int sendto(SockAddr& sa, const void* buf, int len, int msgf = 0)
			throw();
		
		///
		int	sendtimeout (int wp = -1)
			throw();
		///
		int	recvtimeout (int wp = -1)
			throw();
		///
		int	is_readready (int wp_sec, int wp_usec = 0) const
			throw();
		///
		int	is_writeready (int wp_sec, int wp_usec = 0) const
			throw();
		///
		int	is_exceptionpending (int wp_sec, int wp_usec = 0) const
			throw();
		
		///
		void shutdown (shuthow sh)
			throw();
		
		///
		int	getopt(option op, void* buf,int len, level l = sol_socket) const
			throw();
		///
		void setopt(option op, void* buf,int len, level l = sol_socket) const
			throw();
		
		///
		type gettype() const
			throw();
		///
		int clearerror() const
			throw();
		///
		int debug(int opt = -1) const
			throw();
		///
		int reuseaddr(int opt = -1) const
			throw();
		///
		int keepalive(int opt = -1) const
			throw();
		///
		int dontroute(int opt = -1) const
			throw();
		///
		int broadcast(int opt = -1) const
			throw();
		///
		int oobinline(int opt = -1) const
			throw();
		///
		int linger(int tim = -1) const
			throw();
		///
		int sendbufsz(int sz = -1)   const
			throw();
		///
		int recvbufsz(int sz = -1)   const
			throw();

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

	/**	Input stream from a socket
			{\bf Definition:} \URL{BALL/SYSTEM/socket.h}
	*/
	class ISockStream
		: public std::istream 
	{
		// ?????:
		// should this really be protected here?
		protected:

		//_
		ISockStream()
			: BALL_IOS(0),
				BALL_ISTREAM(0)
		{
		}

		public:

		// ?????
		// /KM/usr1/anker/BALL/include/BALL/SYSTEM/socket.h: In method
		// `BALL::ISockStream::ISockStream(const BALL::ISockStream &)':
		// /opt/gnu/lib/gcc-lib/i686-pc-linux-gnu/2.95.3/../../../../include/g++-3/streambuf.h:128:
		// `ios::ios(const ios &)' is private
		// /KM/usr1/anker/BALL/include/BALL/SYSTEM/socket.h:643: within this
		// context
		// BALL_CREATE(ISockStream)
						
		/** @name Constructors and destructor
		*/
		//@{

		/// Detailed constructur
		ISockStream(SocketBuf* sb)
			throw(Exception::NullPointer);

		/// Destructor
		~ISockStream()
			throw();
							
		//@}
		/** @name ?????: Accessors 
		*/
		//@{

		/// 
		SocketBuf* rdbuf()
			throw();

		///
		SocketBuf* operator -> ()
			throw();

		//@}
	};



	/**	Output stream from a socket.
			{\bf Definition:} \URL{BALL/SYSTEM/socket.h}
	*/
	class OSockStream
		: public std::ostream 
	{
		// ?????:
		// see above
		protected:

		OSockStream()
			: BALL_IOS(0),
				BALL_OSTREAM(0)
		{
		}

		public:

		// ?????
		// see above
		// BALL_CREATE(OSockStream)

		/**	@name	Constructors and Destructors 
		*/
		//@{

		/// Detailed constructur
		OSockStream(SocketBuf* socket_buf)
			throw(Exception::NullPointer);

		/// Destructor
		virtual ~OSockStream()
			throw();

		//@}
		/**	@name	Miscellaneous 
		*/
		//@{

		///
		SocketBuf* rdbuf()
			throw();

		///
		SocketBuf* operator -> ()
			throw();

		//@}
	};



	/**	INET socket address.
			{\bf Definition:} \URL{BALL/SYSTEM/socket.h}
	*/
	class SockInetAddr
		: public SockAddr, 
			public sockaddr_in 
	{

		protected:

		//_
		void setport_(const String& service_name, 
				const String& protocol_name= "tcp")
			throw();

		//_
		void setaddr_(const String& hostname)
			throw();


		public:

		/**	@name	Construtors and Destructors 
		*/
		//@{

		/// Default constructor
		SockInetAddr()
			throw();

		/** Creates a SockInetAddr object from an unsigned int (containing the
				IP address) and a port number
		*/
		SockInetAddr(unsigned long addr, int port_no = 0)
			throw();

		/// Creates a SockInetAddr object from a given hostname and port number
		SockInetAddr(const String& host_name, int port_no = 0)
			throw();

		/// 
		SockInetAddr(unsigned long addr, const String& service_name,
				const String& protocol_name = "tcp")
			throw();

		///
		SockInetAddr(const String& host_name, const String& service_name,
				const String& protocol_name = "tcp")
			throw();

		///
		SockInetAddr(const SockInetAddr& sina)
			throw();

		/// Destructor
		~SockInetAddr() 
			throw();

		//@}
		/**	@name	Converters 
		*/
		//@{

		/// Convert a SockInetAddr to void*.
		operator void* () const
			throw();

		//@}
		/**	@name	Accessors 
		*/
		//@{

		///
		int getSize() const
			throw();

		///
		int getFamily() const 
			throw();
	
		///
		sockaddr* getAddr() const 
			throw();
			
		///
		int	getPort() const
			throw();

		///
		const String&	getHostname() const
			throw();

		//@}
	};


	/**	INET socket buffer.
			{\bf Definition:} \URL{BALL/SYSTEM/socket.h}
	*/
	class SockInetBuf
		: public SocketBuf 
	{

		protected:

		// needs to be fixed
		SockInetBuf& operator = (const SocketBuf& si)
			throw();

		// needs fixing
		SockInetBuf& operator = (const SockInetBuf& si)
			throw();

		public:

		/**	@name	Enumerations 
		*/
		//@{

		///
		enum domain 
		{ 
			af_inet = AF_INET 
		};

		//@}
		/**	@name	Constructors and Destructor
		*/
		//@{

		/// Detailed constructor
		SockInetBuf(const SocketBuf& socket_buf)
			throw();

		/// Detailed constructor
		SockInetBuf(const SockInetBuf& socket_inet_buf)
			throw();

		/// Detailed constructor
		SockInetBuf(SocketBuf::type type, int proto = 0)
			throw();
		
		/// Destructor
		~SockInetBuf()
			throw();

		//@}

		///
		SocketBuf* open(SocketBuf::type, int proto=0)
			throw();
		
		///
		SockInetAddr localaddr() const
			throw();

		///
		int localport() const
			throw();

		///
		const String& localhost() const
			throw();
		
		///
		SockInetAddr peeraddr() const
			throw();

		///
		int	peerport() const
			throw();

		///
		const String&	peerhost() const
			throw();


		/**	@name	Binding 
		*/
		//@{

		///
		int bind_until_success(int portno)
			throw();

		///
		virtual int	bind(SockAddr& sa)
			throw();

		///
		int	bind()
			throw();

		///
		int	bind(unsigned long addr, int port_no = 0)
			throw();

		///
		int	bind(const String& host_name, int port_no = 0)
			throw();

		///
		int	bind(unsigned long addr, const String& service_name, const String& protocol_name = "tcp")
			throw();

		///
		int	bind(const String& host_name, const String& service_name, const String& protocol_name = "tcp")
			throw();

		//@}
		/**	@name	Connecting 
		*/
		//@{

		///
		virtual int	connect(SockAddr& sa)
			throw();

		///
		int connect(unsigned long addr, int port_no = 0)
			throw();

		///
		int connect(const String& host_name, int port_no = 0)
			throw();

		///
		int connect(unsigned long addr, const String& service_name,  const String& protocol_name = "tcp")
			throw();

		///
		int connect (const String& host_name, const String& service_name, const String& protocol_name = "tcp")
			throw();

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
						
		/**	@name	Constructors and Destructors 
		*/
		//@{

		/// Constructor
		IOSockStream(SocketBuf* sb)
			: BALL_IOS(0),
				BALL_IOSTREAM(0)
		{
			BALL_IOS::rdbuf(sb);
			init(sb);
			if (rdbuf() == 0)
			{
				throw Exception::NullPointer(__FILE__, __LINE__);
			}
		}
		
		/// Destructor
		virtual ~IOSockStream()
			throw();
		
		//@}
		/**	@name	Miscellaneous 
		*/
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
	};

	/**	
	*/
	class IOStreamSocket
		: public IOSockStream
	{
		public:

		/**	@name	Constructors and Destructors 
		*/
		//@{

		///
		IOStreamSocket(const SocketBuf& sb)	
			throw();
				
		///
		IOStreamSocket(SocketBuf::type ty=SocketBuf::sock_stream, int proto = 0)
			throw();
		
		/// Destructor 
		virtual ~IOStreamSocket()
			throw();

		//@}
		/**	@name	Miscellaneous 
		*/
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

#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/SYSTEM/socket.iC>
#	endif
	
} // namespace BALL

#endif	// BALL_SYSTEM_SOCKET_H
