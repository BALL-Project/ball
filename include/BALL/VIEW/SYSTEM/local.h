#ifdef HAVE_BUILTIN_H
#  include <builtin.h>
#endif

EXTERN_C_BEGIN
#ifdef HAVE_STRING_H
#  include <string.h>
#else
#  ifdef HAVE_MEMORY_H
#    include <memory.h>
#  endif
#endif

#ifdef _ALL_SOURCE
#  define _BSD 44  // AIX
#  include <sys/select.h>
#  ifndef _POSIX_SOURCE
#    define _POSIX_SOURCE
#  endif
#  undef _ALL_SOURCE
#endif

#include <sys/types.h>
#ifdef HAVE_SYS_WAIT
#  include <sys/wait.h>
#endif
#include <sys/signal.h>
#include <netinet/in.h>

#ifndef SA_RESTART
#  define SA_RESTART 0
#endif

EXTERN_C_END

#if defined (__sun__) && !defined (__svr4__) && defined (_S_LIBGXX)
// libg++-2.6.x has stopped providing prototypes for the following
// for sunos 4.1.x

extern "C" {
  int socketpair (int domain, int typ, int protocol, int* sockpair);
  int socket (int domain, int typ, int protocol);
  int bind   (int sock, void* addr, int addrlen);
  int connect (int sock, void* addr, int addrlen);
  int listen (int sock, int num);
  int accept (int sock, void* addr, int* addrlen);
  
  int recv (int sock, void* buf, int buflen, int msgflag);
  int recvfrom (int sock, void* buf, int buflen, int msgflag,
		void* addr, int* addrlen);
  int send (int sock, void* buf, int buflen, int msgflag);
  int sendto (int sock, void* buf, int buflen, int msgflag,
	      void* addr, int addrlen);
  
  int recvmsg (int sock, struct msghdr* msg, int msgflag);
  int sendmsg (int sock, struct msghdr* msg, int msgflag);

  int select (int sock, void* rd, void* wr, void* ex, struct timeval* tv);

  int getsockopt (int sock, int level, int option, void* val, int* vallen);
  int setsockopt (int sock, int level, int option, void* val, int vallen);

  int getsockname (int sock, void* addr, int* addrlen);
  int getpeername (int sock, void* addr, int* addrlen);
}
#endif 

extern "C" int shutdown (int, int); // they have forgotten this

// <arpa/inet.h> does not have a prototype for inet_addr () and gethostname()
extern "C" unsigned long inet_addr (const char*);

// arpa/in.h does not provide a protype for the following
extern "C" char* inet_ntoa (in_addr ina);

#if !defined (__linux__)
  extern "C" int gethostname (char* hostname, int len);
  extern char* SYS_SIGLIST [];
#endif

#ifdef __osf__
  extern "C" {
    int select (int, fd_set*, fd_set*, fd_set*, timeval*);
    unsigned short ntohs (unsigned short);
    unsigned short htons (unsigned short);
    unsigned long  ntohl (unsigned long);
    unsigned long  htonl (unsigned long);
  }
#endif

typedef RETSIGTYPE (*funcptr) (...);
