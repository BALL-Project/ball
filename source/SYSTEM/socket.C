// $Id: socket.C,v 1.23 2001/05/18 16:54:04 anker Exp $

// ORIGINAL COPYRIGHT DISCLAIMER
// /////////////////////////////
// sockstream.C -*- C++ -*- socket library
// Copyright (C) 1992,1993,1994 Gnanasekaran Swaminathan <gs4t@virginia.edu>
// 
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided the above copyright
// notice and this paragraph are preserved on all copies.
// This software is provided "as is" with no express or implied warranty.
//
// Version: 17Oct95 1.10

#include <BALL/SYSTEM/socket.h>

#include <sys/time.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>

#ifndef BUFSIZ
#  define BUFSIZ 1024
#endif

#ifdef FD_ZERO
#  undef FD_ZERO    // bzero causes so much trouble to us
#endif
#define FD_ZERO(p) (memset ((p), 0, sizeof *(p)))


// if configure detects gethostname but there's no
// appropriate definition in unistd.h (e.g. Solaris 2.5.1)
// we define it
#ifdef BALL_DEFINE_GETHOSTNAME
	extern "C" int gethostname(char* name, int len);
#endif

// if we have no gethostname, try sysinfo(SI_HOSTNAME,..)
#ifdef BALL_HAVE_GETHOSTNAME
#	define BALL_GETHOSTNAME(buf, size) ::gethostname(buf, size)
#else
#	ifdef	BALL_HAVE_SYSINFO
#		include <sys/systeminfo.h>
#		define BALL_GETHOSTNAME(buf, size) ::sysinfo(SI_HOSTNAME, buf, (long)size)
#	endif
#endif

using std::endl;

namespace BALL 
{

	void errnoError_(const char* method_name)
	{
		// retrieve the error message
		const char* error_message = const_cast<const char*>(strerror(errno));

		if (error_message == 0)
		{
			error_message = "unknown error";
		}

		// print the mesage
		Log.error() << "Error in " << method_name << ": " << error_message << endl;

		// reset the error flag
		errno = 0;
	}

	SockAddr::SockAddr()
		throw()
	{
	}

	SockAddr::SockAddr(const SockAddr& /* sockaddr */)
		throw()
	{
	}

	SockAddr::~SockAddr()
		throw()
	{
	}

	SockAddr::operator void* () const
		throw()
	{
		Log.error() << "SockAddr::operator void* () "
			<< "This method should not be called." << endl;
		return 0;
	}

	int SockAddr::getSize() const
		throw()
	{
		Log.error() << "SockAddr::getSize() "
			<< "This method should not be called." << endl;
		return 0;
	}

	int SockAddr::getFamily() const
		throw()
	{
		Log.error() << "SockAddr::getFamily() "
			<< "This method should not be called." << endl;
		return 0;
	}

	sockaddr* SockAddr::getAddr() const
		throw()
	{
		Log.error() << "SockAddr::getAddr() "
			<< "This method should not be called." << endl;
		return 0;
	}


	SocketBuf::SocketBuf(int soc)
		throw()
		: rep(new sockcnt(soc, 1)),
			stmo(-1), 
			rtmo(-1)
	{
		xflags(0);
		xsetflags(_S_LINE_BUF);
	}


	SocketBuf::SocketBuf(int domain, SocketBuf::type socket_type, int proto)
		throw()
		: rep(0), 
			stmo(-1), 
			rtmo(-1)
	{
		int soc = ::socket(domain, socket_type, proto);
		rep = new sockcnt(soc, 1);
#	ifdef BALL_HAS_ANSI_IOSTREAM
		xflags_ (0);
#	endif
		if (rep->sock == -1)
		{
			errnoError_("SocketBuf::SocketBuf");
		}

		xsetflags(_S_LINE_BUF);
	}


	SocketBuf::SocketBuf(const SocketBuf& socket_buf)
		throw()
		: std::streambuf(),
			rep(socket_buf.rep), 
			stmo(socket_buf.stmo), 
			rtmo(socket_buf.rtmo)
	{
#	ifdef BALL_HAS_ANSI_IOSTREAM
		xflags_(0);
#	endif
		rep->cnt++;
		xsetflags(_S_LINE_BUF);
	}


	SocketBuf& SocketBuf::operator = (const SocketBuf& socket_buf)
		throw()
	{
		if (this != &socket_buf 
				&& rep != socket_buf.rep 
				&& rep->sock != socket_buf.rep->sock) 
		{
			this->SocketBuf::~SocketBuf();
			rep = socket_buf.rep; 
			stmo = socket_buf.stmo; 
			rtmo = socket_buf.rtmo;
			rep->cnt++;
#	ifdef BALL_HAS_ANSI_IOSTREAM
				xflags(socket_buf.xflags());
#	else
				xflags(((SocketBuf&)socket_buf).xflags());
#	endif
		}

		return *this;
	}


	SocketBuf::~SocketBuf()
		throw()
	{
		overflow(EOF);

		if (rep->cnt == 1 
				&& !(xflags() & _S_DELETE_DONT_CLOSE))
		{
			close();
		}

		if (--rep->cnt == 0) 
		{
			delete rep;
		}
	}


	SocketBuf* SocketBuf::close()
		throw()
	{
		if (rep->sock >= 0) 
		{
			if (::close (rep->sock) == -1) 
			{
				return this;
			}
			rep->sock = -1;
		}
		
		return 0;
	}

	// return 0 when there is nothing to flush or when the flush is a success
	// return EOF when it could not flush
	int SocketBuf::flush_output()
		throw()
	{
		if (pptr() <= pbase()) 
		{
			return 0;
		}

		if (!(xflags() & _S_NO_WRITES)) 
		{
			int wlen = (int)(pptr() - pbase());
			int wval = (int)sys_write(pbase(), wlen);
			int status = (wval == wlen) ? 0: EOF;

			if (unbuffered()) 
			{
				setp(pbase(), pbase());
			}
			else 
			{
				setp(pbase(), pbase() + BUFSIZ);
			}

			return status;
		}

		return EOF;
	}

	int SocketBuf::sync()
		throw()
	{
		return flush_output();
	}

	// return 1 on allocation and 0 if there is no need
	int SocketBuf::doallocate()
		throw()
	{
		if (!pbase()) 
		{
			char*	buf = new char[2 * BUFSIZ];
			setg(buf, buf, buf + BUFSIZ);
			
			buf += BUFSIZ;
			setp(buf, buf + BUFSIZ);

			return 1;
		}
		return 0;
	}

	int SocketBuf::underflow()
		throw()
	{
		if (xflags() & _S_NO_READS) 
		{
			return EOF;
		}
		
		if (gptr() < egptr())
		{
			return *(unsigned char*)gptr();
		}
		
		if (pbase() == 0 && doallocate() == 0)
		{
			return EOF;
		}
		
		int bufsz = unbuffered() ? 1: BUFSIZ;
		int rval = (int)sys_read(pbase(), bufsz);

		if (rval == EOF) 
		{
			xsetflags(_S_EOF_SEEN);
			return EOF;
		}
		else 
		{
			if (rval == 0) 
			{
				return EOF;
			}
		}
		setg(eback(), pbase(), pbase() + rval);

		return *(unsigned char*)gptr();
	}

	// if c == EOF, return flush_output();
	// if c == '\n' and linebuffered, insert c and
	// return (flush_output()==EOF)? EOF: c;     
	// otherwise insert c into the buffer and return c
	int SocketBuf::overflow(int c)
		throw()
	{
		if (c == EOF) 
		{
			return flush_output();
		}
		
		if (xflags() & _S_NO_WRITES)
		{
			return EOF;
		}
		
		if (pbase() == 0 && doallocate() == 0) 
		{
			return EOF;
		}
		
		if (pptr() >= epptr() && flush_output() == EOF)
		{
			return EOF;
		}

		xput_char (c);
		if ((unbuffered() || linebuffered() && c == '\n' || pptr() >= epptr()) && flush_output() == EOF)
		{
			return EOF;
		}
		
		return c;
	}

	std::streamsize SocketBuf::xsputn(const char* s, std::streamsize n)
		throw()
	{
		if (n <= 0) 
		{
			return 0;
		}
		const unsigned char* p = (const unsigned char*)s;
		
		for (int i=0; i<n; i++, p++) 
		{
			if (*p == '\n') 
			{
				if (overflow (*p) == EOF) 
				{
					return i;
				}
			} 
			else 
			{
				if (sputc (*p) == EOF) 
				{
					return i;
				}
			}
		}

		return n;
	}

	int SocketBuf::bind(SockAddr& sa)
		throw()
	{
		if (::bind(rep->sock, sa.getAddr(), sa.getSize()) == -1) 
		{
			return errno;
		}

		return 0;
	}

	int SocketBuf::connect(SockAddr& sa)
		throw()
	{
		int result = 0;
		if (::connect(rep->sock, sa.getAddr(), sa.getSize()) == -1) 
		{
			result = errno;
			errnoError_("SocketBuf::connect");
		}
		return result;
	}

	void SocketBuf::listen(int num)
		throw()
	{
		if (::listen(rep->sock, num) == -1)
		{
			errnoError_("SocketBuf::listen");
		}
	}

	SocketBuf	SocketBuf::accept(SockAddr& sa)
		throw()
	{
		BALL_SOCKLEN_TYPE len = sa.getSize();
		int soc = -1;

		while ((soc = ::accept (rep->sock, sa.getAddr(), &len)) == -1 && errno == EINTR)
		{
			errno = 0;
		}

		if (soc == -1)
		{
			errnoError_("SocketBuf::accept(SockAddr&)");
		}

		return soc;
	}

	SocketBuf	SocketBuf::accept()
		throw()
	{
		int soc = -1;
		while ((soc = ::accept (rep->sock, 0, 0)) == -1 && errno == EINTR)
		{
			errno = 0;
		}

		if (soc == -1)
		{
			errnoError_("SocketBuf::accept()");
		}

		return soc;
	}

	int SocketBuf::read(void* buf, int len)
		throw()
	{
		if (rtmo != -1 && is_readready(rtmo)==0)
		{
			return 0;
		}
		
		int	rval;
		if ((rval = (int)::read(rep->sock, (char*) buf, len)) == -1)
		{
			errnoError_("SocketBuf::read()");
		}

		return (rval==0) ? EOF: rval;
	}

	int SocketBuf::recv(void* buf, int len, int msgf)
		throw()
	{
		if (rtmo != -1 && is_readready (rtmo)==0) 
		{
			return 0;
		}
		
		int	rval;

		if ((rval = ::recv (rep->sock, (char*) buf, len, msgf)) == -1)
		{
			errnoError_("SocketBuf::recv()");
		}

		return (rval==0) ? EOF: rval;
	}

	int SocketBuf::recvfrom(SockAddr& sa, void* buf, int len, int msgf)
		throw()
	{
		if (rtmo != -1 && is_readready (rtmo)==0) 
		{
			return 0;
		}
		
		int	rval;
		BALL_SOCKLEN_TYPE	sockaddr_len = sa.getSize();
		
		if ((rval = ::recvfrom (rep->sock, (char*) buf, len, msgf, sa.getAddr(), &sockaddr_len)) == -1)
		{
			errnoError_("SocketBuf::recvfrom()");
		}

		return (rval==0) ? EOF: rval;
	}

	int SocketBuf::write(const void* buf, int len)
		throw()
	{
		if (stmo != -1 && is_writeready (stmo)==0) 
		{
			return 0;
		}
		
		int	wlen=0;
		while(len>0) 
		{
			int	wval;
			if ((wval = (int)::write (rep->sock, (char*) buf, len)) == -1) 
			{
				errnoError_("SocketBuf::write()");
				return wval;
			}

			len -= wval;
			wlen += wval;
		}

		return wlen; // == len if every thing is all right
	}

	int SocketBuf::send(const void* buf, int len, int msgf)
		throw()
	{
		if (stmo != -1 && is_writeready (stmo)==0) 
		{
			return 0;
		}
		
		int	wlen=0;
		while(len>0) 
		{
			int	wval;
			if ((wval = ::send (rep->sock, (char*) buf, len, msgf)) == -1) 
			{
				errnoError_("SocketBuf::send");
				return wval;
			}

			len -= wval;
			wlen += wval;
		}

		return wlen;
	}

	int SocketBuf::sendto(SockAddr& sa, const void* buf, int len, int msgf)
		throw()
	{
		if (stmo != -1 && is_writeready (stmo)==0) 
		{
			return 0;
		}
		
		int	wlen=0;
		while(len>0) 
		{
			int	wval;
			if ((wval = ::sendto (rep->sock, (char*) buf, len, msgf, sa.getAddr(), sa.getSize())) == -1) 
			{
				errnoError_("SocketBuf::sendto");
				return wval;
			}

			len -= wval;
			wlen += wval;
		}

		return wlen;
	}

	int SocketBuf::sendtimeout(int wp)
		throw()
	{
		int oldstmo = stmo;
		stmo = (wp < 0) ? -1: wp;
		return oldstmo;
	}

	int SocketBuf::recvtimeout(int wp)
		throw()
	{
		int oldrtmo = rtmo;
		rtmo = (wp < 0) ? -1: wp;
		return oldrtmo;
	}

	int SocketBuf::is_readready(int wp_sec, int wp_usec) const
		throw()
	{
		fd_set fds;
		FD_ZERO (&fds);
		FD_SET (rep->sock, &fds);
		
		timeval tv;
		tv.tv_sec  = wp_sec;
		tv.tv_usec = wp_usec;
		
		int ret = select(rep->sock + 1, &fds, 0, 0, (wp_sec == -1) ? 0: &tv);
		if (ret == -1) 
		{
			errnoError_("SocketBuf::readready");
			return 0;
		}

		return ret;
	}

	int SocketBuf::is_writeready(int wp_sec, int wp_usec) const
		throw()
	{
		fd_set fds;
		FD_ZERO (&fds);
		FD_SET (rep->sock, &fds);
		
		timeval tv;
		tv.tv_sec  = wp_sec;
		tv.tv_usec = wp_usec;
		
		int ret = select (rep->sock+1, 0, &fds, 0, (wp_sec == -1) ? 0: &tv);
		if (ret == -1) 
		{
			errnoError_("Select::operator()");
			return 0;
		}

		return ret;
	}

	int SocketBuf::is_exceptionpending(int wp_sec, int wp_usec) const
		throw()
	{
		fd_set fds;
		FD_ZERO (&fds);
		FD_SET  (rep->sock, &fds);
		
		timeval tv;
		tv.tv_sec = wp_sec;
		tv.tv_usec = wp_usec;
		
		int ret = select (rep->sock+1, 0, 0, &fds, (wp_sec == -1) ? 0: &tv);
		if (ret == -1) 
		{
			errnoError_("Select::operator()");
			return 0;
		}

		return ret;
	}

	void SocketBuf::shutdown(shuthow sh)
		throw()
	{
		switch (sh) 
		{
			case shut_read:
				xsetflags(_S_NO_READS);
				break;
			case shut_write:
				xsetflags(_S_NO_WRITES);
				break;
			case shut_readwrite:
				xsetflags(_S_NO_READS|_S_NO_WRITES);
				break;
		}

		if (::shutdown(rep->sock, sh) == -1)
		{
			errnoError_("SocketBuf::shutdown");
		}
	}

	int SocketBuf::getopt(option op, void* buf, int len, level l) const
		throw()
	{
		BALL_SOCKLEN_TYPE	rlen = len;
		if (::getsockopt (rep->sock, l, op, (char*) buf, &rlen) == -1)
		{
			errnoError_("SocketBuf::getopt");
		}

		return rlen;
	}

	void SocketBuf::setopt(option op, void* buf, int len, level l) const
		throw()
	{
		if (::setsockopt (rep->sock, l, op, (char*) buf, len) == -1)
		{
			errnoError_("SocketBuf::setopt");
		}
	}

	SocketBuf::type SocketBuf::gettype() const
		throw()
	{
		int	ty=0;
		getopt (so_type, &ty, sizeof (ty));

		return SocketBuf::type(ty);
	}

	int SocketBuf::clearerror() const
		throw()
	{
		int 	err=0;
		getopt (so_error, &err, sizeof (err));

		return err;
	}

	int SocketBuf::debug (int opt) const
		throw()
	{
		int old=0;
		getopt (so_debug, &old, sizeof (old));
		if (opt != -1)
		{
			setopt (so_debug, &opt, sizeof (opt));
		}

		return old;
	}

	int SocketBuf::reuseaddr(int opt) const
		throw()
	{
		int old=0;
		getopt (so_reuseaddr, &old, sizeof (old));

		if (opt != -1)
		{
			setopt (so_reuseaddr, &opt, sizeof (opt));
		}

		return old;
	}

	int SocketBuf::keepalive(int opt) const
		throw()
	{
		int old=0;
		getopt (so_keepalive, &old, sizeof (old));
		
		if (opt != -1)
		{
			setopt (so_keepalive, &opt, sizeof (opt));
		}

		return old;
	}

	int SocketBuf::dontroute(int opt) const
		throw()
	{
		int old=0;
		getopt (so_dontroute, &old, sizeof (old));

		if (opt != -1)
		{
			setopt (so_dontroute, &opt, sizeof (opt));
		}

		return old;
	}

	int SocketBuf::broadcast(int opt) const
		throw()
	{
		int old=0;
		getopt (so_broadcast, &old, sizeof (old));

		if (opt != -1)
		{
			setopt (so_broadcast, &opt, sizeof (opt));
		}

		return old;
	}

	int SocketBuf::oobinline(int opt) const
		throw()
	{
		int old=0;
		getopt (so_oobinline, &old, sizeof (old));
		
		if (opt != -1)
		{
			setopt (so_oobinline, &opt, sizeof (opt));
		}

		return old;
	}

	int SocketBuf::linger(int opt) const
		throw()
	{
		socklinger old (0, 0);
		getopt(so_linger, &old, sizeof(old));

		if (opt > 0) 
		{
			socklinger nw (1, opt);
			setopt(so_linger, &nw, sizeof(nw));
		}
		else 
		{
			if (opt == 0) 
			{
				socklinger nw (0, old.l_linger);
				setopt(so_linger, &nw, sizeof(nw));
			}
		}

		return old.l_onoff ? old.l_linger: -1;
	}

	int SocketBuf::sendbufsz (int  sz) const
		throw()
	{
		int old=0;
		getopt (so_sndbuf, &old, sizeof (old));
		if (sz >= 0)
		{
			setopt (so_sndbuf, &sz, sizeof (sz));
		}

		return old;
	}

	int SocketBuf::recvbufsz(int sz) const
		throw()
	{
		int old=0;
		getopt (so_rcvbuf, &old, sizeof (old));

		if (sz >= 0)
		{
			setopt (so_rcvbuf, &sz, sizeof (sz));
		}

		return old;
	}


	IOSockStream::~IOSockStream()
		throw()
	{
		delete rdbuf();
		init(0);
	}


	SockInetAddr::SockInetAddr() 
		throw()
	{
		sin_family = SockInetBuf::af_inet;
		sin_addr.s_addr = htonl(INADDR_ANY);
		sin_port = 0;
	}


	// addr and port_no are in host byte order
	SockInetAddr::SockInetAddr(unsigned long addr, int port_no)
		throw()
	{
		if (addr == 0)
		{
			sin_addr.s_addr = htonl(INADDR_ANY);
		}
		else 
		{
			sin_addr.s_addr = (unsigned int)htonl(addr);
		}
		sin_family = SockInetBuf::af_inet;
		sin_port = htons(port_no);
	}


	// addr is in host byte order
	SockInetAddr::SockInetAddr(unsigned long addr, const String& service_name,
			const String& protocol_name)
		throw()
	{
		sin_family = SockInetBuf::af_inet;
		// next line added by cgay@cs.uoregon.edu May 29, 1993
		sin_addr.s_addr = (unsigned int)htonl(addr); 
		setport_(service_name, protocol_name);
	}


	// port_no is in host byte order
	SockInetAddr::SockInetAddr(const String& host_name, int port_no)
		throw()
	{
		setaddr_(host_name);
		sin_port = htons(port_no);
	}


	SockInetAddr::SockInetAddr(const String& host_name,
			const String& service_name, const String& protocol_name)
		throw()
	{
		setaddr_(host_name);
		setport_(service_name, protocol_name);
	}


	SockInetAddr::SockInetAddr(const SockInetAddr& sina)
		throw()
		: SockAddr(sina)
	{
		sin_family = SockInetBuf::af_inet;
		sin_addr.s_addr = sina.sin_addr.s_addr;
		sin_port = sina.sin_port;
	}   


	void SockInetAddr::setport_(const String& service_name,
			const String& protocol_name)
		throw()
	{
		servent* sp = getservbyname(service_name.c_str(), protocol_name.c_str());
		if (sp == 0) 
		{
			errnoError_(service_name.c_str());
			return;
		}

		sin_port = sp->s_port;
	}


	int SockInetAddr::getPort() const
		throw()
	{
		return ntohs (sin_port);
	}


	void SockInetAddr::setaddr_(const String& host_name)
		throw()
	{
		if ((int)(sin_addr.s_addr = inet_addr(host_name.c_str())) == -1) 
		{
			hostent* hp = gethostbyname(host_name.c_str());
			if (hp == 0) 
			{
				String message = "SockInetAddr::setaddr(" + String(host_name) + ")";
				errnoError_(message.c_str());
				return;
			}
			memcpy(&sin_addr, hp->h_addr, hp->h_length);
			sin_family = hp->h_addrtype;
		} else {
			sin_family = SockInetBuf::af_inet;
		}
	}

	
	const String& SockInetAddr::getHostname() const
		throw()
	{
		static String hostname;
		static char buf[256];
		
		hostname = "";
		if (sin_addr.s_addr == htonl(INADDR_ANY)) 
		{
			if (BALL_GETHOSTNAME(buf, 255) == -1) 
			{
				errnoError_("SockInetAddr::getHostname");
				hostname = "";
			}
			else 
			{
				hostname = buf;
			}

			return hostname;		
		}
		
		hostent* hp = gethostbyaddr((const char*) &sin_addr, sizeof(sin_addr), getFamily());
		if (hp == 0) 
		{
			errnoError_("SockInetAddr::getHostname()");
		} 
		else 
		{
			if (hp->h_name) 
			{
				hostname = hp->h_name;
			}
		}

		return hostname;
	}


	SockInetBuf::SockInetBuf(SocketBuf::type type, int proto)
		throw()
		: SocketBuf(af_inet, type, proto)
	{
	}


	SockInetBuf& SockInetBuf::operator = (const SocketBuf& socket_buf)
		throw()
	{
		this->SocketBuf::operator = (socket_buf);
		return *this;
		
	}


	SockInetBuf& SockInetBuf::operator = (const SockInetBuf& socket_inet_buf)
		throw()
	{
		this->SocketBuf::operator = (socket_inet_buf);
		return *this;
	}


	SocketBuf* SockInetBuf::open(SocketBuf::type socket_type, int proto)
		throw()
	{
		*this = SockInetBuf(socket_type, proto);
		return this;
	}


	SockInetAddr SockInetBuf::localaddr() const
		throw()
	{
		SockInetAddr sin;
		BALL_SOCKLEN_TYPE len = sin.getSize();
		if (::getsockname(rep->sock, sin.getAddr(), &len) == -1)
		{
			errnoError_("SockInetBuf::localaddr()");
		}

		return sin;
	}


	int SockInetBuf::localport() const
		throw()
	{
		SockInetAddr sin = localaddr();
		if (sin.getFamily() != af_inet) 
		{
			return -1;
		}

		return sin.getPort();
	}


	const String& SockInetBuf::localhost() const
		throw()
	{
		static String empty;
		SockInetAddr sin = localaddr();
		if (sin.getFamily() != af_inet) 
		{
			return empty;
		}

		return sin.getHostname();
	}


	SockInetAddr SockInetBuf::peeraddr() const
		throw()
	{
		SockInetAddr sin;
		BALL_SOCKLEN_TYPE len = sin.getSize();
		if (::getpeername(rep->sock, sin.getAddr(), &len) == -1)
		{
			errnoError_("SockInetBuf::peeraddr()");
		}

		return sin;
	}


	int SockInetBuf::peerport() const
		throw()
	{
		SockInetAddr sin = peeraddr();
		if (sin.getFamily() != af_inet) 
		{
			return -1;
		}

		return sin.getPort();
	}


	const String& SockInetBuf::peerhost() const
		throw()
	{
		static String empty;
		SockInetAddr sin = peeraddr();
		if (sin.getFamily() != af_inet) 
		{
			return empty;
		}

		return sin.getHostname();
	}


	// a. bind to (INADDR_ANY, portno)
	// b. if success return 0
	// c. if failure and errno is EADDRINUSE, portno++ and go to step a.
	// d. return errno.
	int SockInetBuf::bind_until_success(int portno)
		throw()
	{
		while(true)
		{
			SockInetAddr sa((unsigned long) INADDR_ANY, portno++);
			int eno = bind (sa);
			if (eno == 0)
			{
				return 0;
			}

			if (eno != EADDRINUSE)
			{
				return eno;
			}
		}
	}


	int SockInetBuf::bind(SockAddr& sa)
		throw()
	{
		return SocketBuf::bind(sa);
	}

	int SockInetBuf::bind()
		throw()
	{
		SockInetAddr sa;
		return bind(sa);
	}

	// address and portno are in host byte order
	int SockInetBuf::bind(unsigned long addr, int port_no)
		throw()
	{
		SockInetAddr sa(addr, port_no);

		return bind (sa);
	}

	int SockInetBuf::bind(const String& host_name, int port_no)
		throw()
	{
		SockInetAddr sa (host_name, port_no);

		return bind (sa);
	}

	int SockInetBuf::bind (unsigned long addr, const String& service_name, const String& protocol_name)
		throw()
	{
		SockInetAddr sa (addr, service_name, protocol_name);

		return bind (sa);
	}

	int SockInetBuf::bind (const String& host_name, const String& service_name, const String& protocol_name)
		throw()
	{
		SockInetAddr sa(host_name, service_name, protocol_name);

		return bind (sa);
	}

	int SockInetBuf::connect(SockAddr& sa)
		throw()
	{
		return SocketBuf::connect (sa);
	}

	// address and portno are in host byte order
	int SockInetBuf::connect(unsigned long addr, int port_no)
		throw()
	{
		SockInetAddr sa (addr, port_no);

		return connect (sa);
	}

	int SockInetBuf::connect(const String& host_name, int port_no)
		throw()
	{
		SockInetAddr sa (host_name, port_no);

		return connect (sa);
	}

	int SockInetBuf::connect (unsigned long addr, const String& service_name, const String& protocol_name)
		throw()
	{
		SockInetAddr sa (addr, service_name, protocol_name);

		return connect (sa);
	}

	int SockInetBuf::connect (const String& host_name, const String& service_name, const String& protocol_name)
		throw()
	{
		SockInetAddr sa (host_name, service_name, protocol_name);

		return connect(sa);
	}

	IOStreamSocket::IOStreamSocket(SocketBuf::type ty, int proto)
		throw()
		:	IOSockStream(new SockInetBuf(ty, proto))
	{
		if (rdbuf() == 0)
		{
			throw Exception::NullPointer(__FILE__, __LINE__);
		}
	}

	IOStreamSocket::IOStreamSocket(const SocketBuf& sb)
		throw()
		: IOSockStream(new SockInetBuf(sb))
	{
		if (rdbuf() == 0)
		{
			throw Exception::NullPointer(__FILE__, __LINE__);
		}
	}

	IOStreamSocket::~IOStreamSocket()
		throw()
	{
		delete rdbuf();
		init(0);
	}

} // namespace BALL
