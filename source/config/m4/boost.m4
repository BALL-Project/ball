dnl
dnl		general boost support
dnl
AC_DEFUN([CF_BOOST], [
	AC_SUBST(BOOST_LIBS)

	AX_BOOST_BASE()
	AX_BOOST_SYSTEM()

	AX_BOOST_THREAD()
	if test "$ax_cv_boost_thread" = yes; then
		AC_DEFINE(PROJECT[]_HAS_BOOST_THREAD)
	fi

	AX_BOOST_IOSTREAMS()
	if test "$ax_cv_boost_iostreams" = yes; then
		AC_DEFINE(PROJECT[]_HAS_BOOST_IOSTREAMS)
	fi

	PROJECT[]_INCLUDES="${PROJECT[]_INCLUDES}"
	BOOST_CPPFLAGS=$(echo ${BOOST_CPPFLAGS} | ${SED} "s_-I\s*/usr/include\(\s\|$\)_ _g")
	CXXFLAGS="${CXXFLAGS} ${BOOST_CPPFLAGS}"
	BOOST_LIBS="${BOOST_LDFLAGS} ${BOOST_SYSTEM_LIB} ${BOOST_THREAD_LIB} ${BOOST_IOSTREAMS_LIB}"
])

dnl
dnl		asio support
dnl
AC_DEFUN([CF_ASIO], [
	dnl
	dnl configure arguments
	dnl
	AC_ARG_WITH([asio-incl],
		AS_HELP_STRING([--with-asio-incl@<:@=DIR@:>@], [path to asio includes]),
		[
				ASIO_INCPATH="$withval"
		],
		[
				ASIO_INCPATH=
		])

	dnl
	dnl	variable substitutions for config.mak
	dnl
	AC_SUBST(ASIO_INCLUDES)
	AC_SUBST(ASIO_LIBS)
	AC_SUBST(PROJECT[]_HAS_ASIO)

	PROJECT[]_HAS_ASIO=

	dnl
	dnl this call does all the hard work
	dnl
	AX_BOOST_ASIO()
	
	dnl
	dnl do we have to search further?
	dnl
	if test "$ax_cv_boost_asio" = yes; then
		dnl
		dnl set the namespace accordingly
		dnl
		AC_DEFINE(PROJECT[]_HAS_ASIO)
		AC_DEFINE(PROJECT[]_ASIO_NAMESPACE,boost::asio)
		AC_DEFINE(PROJECT[]_HAS_BOOST_ASIO,,[define whether to take asio from boost or not])
		PROJECT[]_HAS_ASIO=true
	else
		dnl
		dnl let's see if we can find a boost-less asio
		dnl
		AC_MSG_CHECKING(for asio header files)
		CF_FIND_HEADER(ASIO_INCDIR, asio.hpp, ${ASIO_INCPATH})
		if test "${ASIO_INCDIR}" = "" ; then
			AC_MSG_RESULT((not found!))
		  AC_MSG_RESULT()
		  AC_MSG_RESULT([The asio headers could not be found. Please specify the path to <asio.hpp>])
		  AC_MSG_RESULT([by passing the option --with-asio-incl=DIR to configure.])
		  AC_MSG_RESULT()
		  AC_MSG_RESULT([asio is needed for network related functionality and is not currently strictly necessary.])
		  AC_MSG_RESULT([You can continue building the library, but the BALLView - server will be disabled.])
		  AC_MSG_RESULT()
		else
			AC_MSG_RESULT((${ASIO_INCDIR}))
			AC_DEFINE(PROJECT[]_HAS_ASIO)
			AC_DEFINE(PROJECT[]_ASIO_NAMESPACE,asio)
			PROJECT[]_INCLUDES="${PROJECT[]_INCLUDES} -I${ASIO_INCDIR}"
			PROJECT[]_HAS_ASIO=true
		fi
	fi
])


