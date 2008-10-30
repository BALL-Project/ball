dnl
dnl	spacenav support
dnl
AC_DEFUN([CF_SPACENAV], [
	AC_MSG_CHECKING(Checking for spacenavigator support)
	dnl
  dnl  variable substitutions required for spacenavigator support
  dnl
  AC_SUBST(SPACENAV_SUPPORT)
  AC_SUBST(SPACENAV_INCLUDES)
  AC_SUBST(SPACENAV_LIBS)

	dnl
	dnl Check for the spacenav headers
	dnl
	if test "${SPACENAV_SUPPORT}" = "true" ; then	
		AC_MSG_RESULT(enabled)

		AC_DEFINE(PROJECT[]_HAS_SPACENAV)
		AC_DEFINE(PROJECT[]_HAS_SPACENAV_H)

		AC_MSG_CHECKING(for spacenavigator header files)
   	CF_FIND_HEADER(SPACENAV_INCDIR, spnav.h, ${SPACENAV_INCPATH})
		if test "${SPACENAV_INCDIR}" = "" ; then
      AC_MSG_RESULT((not found!))
		  AC_MSG_RESULT()
		  AC_MSG_RESULT([The header files for the spacenavigator support could not be found. Please specify the path to <spnav.h>])
		  AC_MSG_RESULT([by passing the option --with-spacenavigator-incl=DIR to configure.])
		  AC_MSG_RESULT()
		  AC_MSG_RESULT([These files are needed for the spacenavigator input plugin.])
		  AC_MSG_RESULT([Please install the library on your system, or disable it with --disable-spacenavigator.])
		  AC_MSG_RESULT()
			CF_ERROR
		  CF_ERROR
	  else
  		AC_MSG_RESULT((${SPACENAV_INCDIR}))
  		SPACENAV_INCLUDES="-I${SPACENAV_INCDIR}"
  	fi

		dnl
		dnl Check for the spacenavigator lib
		dnl
	
	  AC_MSG_CHECKING(for libspnav.so)
  	if test "${SPACENAV_LIBDIR}" != "" ; then
      if test -a "${SPACENAV_LIBDIR}/libspnav.so" ; then
		  	SPACENAV_LIBDIR="${SPACENAV_LIBDIR}/"
  		fi
  	fi	
		CF_FIND_LIB(SPACENAV_LIBDIR, libspnav, ${SPACENAV_LIBPATH})
		
  	if test "${SPACENAV_LIBDIR}" = "" ; then
      AC_MSG_RESULT((not found!))
		  AC_MSG_RESULT()
		  AC_MSG_RESULT([The spacenavigator library could not be found. Please specify the path to libspnav.so])
		  AC_MSG_RESULT([by passing the option --with-spacenavigator-lib=DIR to configure.])
		  AC_MSG_RESULT([You may also set the environment variable SPACENAV_LIBDIR to the correct])
		  AC_MSG_RESULT([path - configure will recognize this, too.])
		  AC_MSG_RESULT()
		  AC_MSG_RESULT([These files are needed for the spacenavigator input plugin.])
		  AC_MSG_RESULT([Please install the library on your system, or disable it with --disable-spacenavigator.])
		  AC_MSG_RESULT()
			CF_ERROR
	  else
			AC_MSG_RESULT(${SPACENAV_LIBDIR}/libspnav.so)
  		SPACENAV_LIBS="-L${SPACENAV_LIBDIR} -lspnav"
  	fi

		PROJECT[]_HAS_SPACENAV=true
		AC_SUBST(PROJECT[]_HAS_SPACENAV)

	else
		AC_MSG_RESULT(disabled)
	fi
	
])

