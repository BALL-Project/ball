dnl
dnl   check for the XDR functions: their interface and the libraries they're hidden in.
dnl
AC_DEFUN(CF_CHECK_XDR, [
if test "${PROJECT[]_NO_XDR}" = "true" ; then
AC_MSG_RESULT([No XDR headers available - building of XDR persistence support disabled])
AC_DEFINE(PROJECT[]_HAS_XDR, )
PROJECT[]_HAS_XDR=""
AC_SUBST(PROJECT[]_HAS_XDR)
else
AC_CHECK_HEADER(rpc/types.h, HAS_RPC_TYPES_H=true, HAS_RPC_TYPES_H=false)
if test "${HAS_RPC_TYPES_H}" = false ; then
AC_MSG_RESULT()
AC_MSG_RESULT([Cannot find RPC headers (rpc/types.h).])
AC_MSG_RESULT([If your system does not provide an RPC/XDR implementation (e.g., CYGWIN),])
AC_MSG_RESULT([please specify the option --without-xdr to avoid this error.])
CF_ERROR
fi

AC_CHECK_HEADER(rpc/xdr.h, HAS_XDR_H=true, HAS_XDR_H=false)
if test "${HAS_XDR_H}" = false ; then
AC_MSG_RESULT()
AC_MSG_RESULT([Cannot find XDR headers (rpc/xdr.h).])
CF_ERROR
fi

AC_MSG_CHECKING([arg types for xdrrec_create])
AC_TRY_COMPILE(
[
	#include <rpc/types.h>
	#include <rpc/xdr.h>
	extern "C" int dummy(void*, void*, unsigned int) {return 0;}
	void foo(){ 
		XDR xdrs;
		xdrrec_create(&xdrs, 0, 0, 0, dummy, dummy);
	}
],
[	
],
PROJECT[]_XDRREC_VOID_VOID_UINT=true,
PROJECT[]_XDRREC_VOID_VOID_UINT=false
)
if test "${PROJECT[]_XDRREC_VOID_VOID_UINT}" = true ; then
AC_MSG_RESULT([(void*, void*, unsigned int)])
else
AC_TRY_COMPILE(
	[
		 #include <rpc/types.h>
		 #include <rpc/xdr.h>
		 extern "C" int dummy(void*, char*, int) {return 0;}
		 void foo(){ 
			XDR xdrs;
			xdrrec_create(&xdrs, 0, 0, 0, dummy, dummy);
		 }
	 ],
	 [	
	 ],
	 PROJECT[]_XDRREC_VOID_CHAR_INT=true,
	 PROJECT[]_XDRREC_VOID_CHAR_INT=false
)
if test "${PROJECT[]_XDRREC_VOID_CHAR_INT}" = true ; then
	AC_MSG_RESULT([(void*, char*, int)])
else
	AC_TRY_COMPILE(
		[
			#include <rpc/types.h>
			#include <rpc/xdr.h>
			extern "C" int dummy(char*, char*, int) {return 0;}
			void foo(){ 
				XDR xdrs;
				xdrrec_create(&xdrs, 0, 0, 0, dummy, dummy);
			}
		],
		[	
		],
		PROJECT[]_XDRREC_CHAR_CHAR_INT=true,
		PROJECT[]_XDRREC_CHAR_CHAR_INT=false
	)
	if test "${PROJECT[]_XDRREC_CHAR_CHAR_INT}" = true ; then
		 AC_MSG_RESULT([(char*, char*, int)])
	else
		 AC_TRY_COMPILE(
			 [
				 #include <rpc/types.h>
				 #include <rpc/xdr.h>
				 extern "C" int dummy() {return 0;}
				 void foo(){
					XDR xdrs;
					xdrrec_create(&xdrs, 0, 0, 0, dummy, dummy);
				}
			 ],
			 [
			 ],
			 PROJECT[]_XDRREC_VOID=true,
			 PROJECT[]_XDRREC_VOID=false
		 )
		 if test "${PROJECT[]_XDRREC_VOID}" = true ; then
				AC_MSG_RESULT(())
		 else
	 AC_TRY_COMPILE(
		[
		 #include <rpc/types.h>
		 #include <rpc/xdr.h>
		extern "C" int dummy(void*, void*, int) {return 0;}
		void foo(){
		 XDR xdrs;
		 xdrrec_create(&xdrs, 0, 0, 0, dummy, dummy);
		}
	 ],
	 [
	 ],
	 PROJECT[]_XDRREC_VOID_VOID_INT=true,
	 PROJECT[]_XDRREC_VOID_VOID_INT=false
	 )
	if test "${PROJECT[]_XDRREC_VOID_VOID_INT}" = true ; then
		AC_MSG_RESULT((void*, void*, int))
	else
		AC_MSG_RESULT(not found!)
	fi
		fi 
fi
fi
fi

dnl
dnl  check whether there is a function to store 64bit
dnl  unsigned ints (xdr_u_hyper)
dnl
AC_MSG_CHECKING(for xdr_u_hyper function)
PROJECT[]_HAS_XDR_U_HYPER=false
AC_TRY_COMPILE(
[
#include <rpc/types.h>
#include <rpc/xdr.h>
],
[
xdr_u_hyper(0, 0);
],	
PROJECT[]_HAS_XDR_U_HYPER=true
)	

if test "${PROJECT[]_HAS_XDR_U_HYPER}" = "true" ; then
AC_MSG_RESULT(found)

AC_MSG_CHECKING([for 64-bit XDR type (for xdr_u_hyper)])
PROJECT[]_U_QUAD_TYPE=""
AC_TRY_COMPILE(
[
	#include <rpc/types.h>
	#include <rpc/xdr.h>
],
[	u_quad_t   q;
	XDR xdrs;
		xdr_u_hyper(&xdrs, &q);
],
PROJECT[]_U_QUAD_TYPE=u_quad_t
)	

if test "${PROJECT[]_U_QUAD_TYPE}" = "" ; then
AC_TRY_COMPILE(
	[
		#include <rpc/types.h>
		#include <rpc/xdr.h>
	],
	[	u_longlong_t   q;
		XDR xdrs;
		xdr_u_hyper(&xdrs, &q);
	],
	PROJECT[]_U_QUAD_TYPE=u_longlong_t
)	
fi

if test "${PROJECT[]_U_QUAD_TYPE}" = "" ; then
AC_TRY_COMPILE(
	[
		#include <rpc/types.h>
		#include <rpc/xdr.h>
	],
	[	unsigned long long int  q;
		XDR xdrs;
		xdr_u_hyper(&xdrs, &q);
	],
	PROJECT[]_U_QUAD_TYPE="unsigned long long int"
)	
fi

if test "${PROJECT[]_U_QUAD_TYPE}" = "" ; then
AC_TRY_COMPILE(
	[
		#include <rpc/types.h>
		#include <rpc/xdr.h>
	],
	[	__uint64_t  q;
		XDR xdrs;
		xdr_u_hyper(&xdrs, &q);
	],
	PROJECT[]_U_QUAD_TYPE=__uint64_t
)	
fi
if test "${PROJECT[]_U_QUAD_TYPE}" = "" ; then
AC_MSG_RESULT([Could not identify an appropriate type for xdr_u_hyper.])
CF_ERROR
fi

AC_MSG_RESULT(${PROJECT[]_U_QUAD_TYPE})
AC_DEFINE_UNQUOTED(PROJECT[]_XDR_UINT64_TYPE, ${PROJECT[]_U_QUAD_TYPE})
AC_DEFINE(PROJECT[]_HAS_XDR_U_HYPER)

else

dnl
dnl	we do not have xdr_u_hyper, so PROJECT[] has to use two 
dnl	calls to xdr_u_int instead. 
dnl	However, we have to identify whether the system supports
dnl	64bit unsigned types at all
dnl	
AC_TRY_COMPILE(
[
],
[	
	unsigned long long int  q = 1234567890123456789LL;
],
PROJECT[]_U_QUAD_TYPE="unsigned long long int"
)	
if test "${PROJECT[]_U_QUAD_TYPE}" = "" ; then
AC_MSG_RESULT([Could not identify an 64 bit unsigned type (long long).])
CF_ERROR
fi

AC_DEFINE_UNQUOTED(PROJECT[]_XDR_UINT64_TYPE, ${PROJECT[]_U_QUAD_TYPE})
AC_MSG_RESULT(unsigned long long int)

fi


dnl
dnl Define appropriate symbols in config.h.
dnl The symbols are used in CONCEPT/XDRPersistenceManager.C only.
dnl
if test "${PROJECT[]_XDRREC_VOID_CHAR_INT}" = true ; then
AC_DEFINE(PROJECT[]_XDRREC_CREATE_VOID_CHAR_INT)
fi
if test "${PROJECT[]_XDRREC_CHAR_CHAR_INT}" = true ; then
AC_DEFINE(PROJECT[]_XDRREC_CREATE_CHAR_CHAR_INT)
fi
if test "${PROJECT[]_XDRREC_VOID}" = true ; then
AC_DEFINE(PROJECT[]_XDRREC_CREATE_VOID)
fi
if test "${PROJECT[]_XDRREC_VOID_VOID_UINT}" = true ; then
AC_DEFINE(PROJECT[]_XDRREC_CREATE_VOID_VOID_UINT)
fi
if test "${PROJECT[]_XDRREC_VOID_VOID_INT}" = true ; then
AC_DEFINE(PROJECT[]_XDRREC_CREATE_VOID_VOID_INT)
fi

dnl
dnl		Try to guess the library required for the XDR stuff.
dnl   It is often in libc, but Solaris hides it in libnsl.
dnl
AC_MSG_CHECKING(for XDR symbols in libc)
AC_TRY_LINK([
#include <rpc/types.h>
#include <rpc/xdr.h>
],
[
XDR xdrs;
int i;
xdr_int(&xdrs, &i);
],
XDR_IN_LIBC=true
)
if test "${XDR_IN_LIBC}" != true ; then
AC_MSG_RESULT(not found!)
AC_MSG_CHECKING(for XDR symbols in libnsl)

SAVE_LIBS=${LIBS}
LIBS="-lnsl ${LIBS}"
AC_TRY_LINK([
	#include <rpc/types.h>
	#include <rpc/xdr.h>
],
[
	XDR xdrs;
	int i;
	xdr_int(&xdrs, &i);
],
XDR_IN_LIBNSL=true
)
if test "${XDR_IN_LIBNSL}" = true ; then
AC_MSG_RESULT(yes)
else
AC_MSG_RESULT(no)
AC_MSG_RESULT()
AC_MSG_RESULT(Did not find XDR symbols in libc or libnsl.)
CF_ERROR
fi
else
dnl
dnl	XDR symbols are in libc.
dnl
AC_MSG_RESULT(yes)
fi
AC_DEFINE(PROJECT[]_HAS_XDR, true)
PROJECT[]_HAS_XDR=true
AC_SUBST(PROJECT[]_HAS_XDR)
fi
])


