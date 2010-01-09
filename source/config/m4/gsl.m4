dnl
dnl	GSL -- GNU Scientific Library
dnl
AC_DEFUN([CF_GSL], [
	AC_MSG_CHECKING(Checking for GSL support)
	dnl
  dnl  variable substitutions required for GSL support
  dnl
  AC_SUBST(GSL_SUPPORT)
  AC_SUBST(GSL_INCLUDES)
  AC_SUBST(GSL_LIBS)

	dnl
	dnl Check for the GSL headers
	dnl
	AC_MSG_RESULT(enabled)

	AC_DEFINE(PROJECT[]_HAS_GSL)
	AC_DEFINE(PROJECT[]_HAS_GSL_H)

	AC_MSG_CHECKING(for GSL header files)
	CF_FIND_HEADER(GSL_INCDIR, gsl/gsl_version.h, ${GSL_INCPATH})
	if test "${GSL_INCDIR}" = "" ; then
		AC_MSG_RESULT((not found!))
		AC_MSG_RESULT()
		AC_MSG_RESULT([The GSL headers could not be found. Please specify the path to <gsl/gsl_version.h>])
		AC_MSG_RESULT([by passing the option --with-gsl-incl=DIR to configure.])
		AC_MSG_RESULT()
		AC_MSG_RESULT([GSL is needed for signal processing.])
		AC_MSG_RESULT([Please install the library on your system, or disable it with --disable-gsl.])
		AC_MSG_RESULT()
		CF_ERROR
		CF_ERROR
	else
		dnl  remove trailing '/gsl' from the include path
		GSL_INCDIR=`echo ${GSL_INCDIR} | sed "s/gsl$//"` 
		AC_MSG_RESULT((${GSL_INCDIR}))
		PROJECT[]_INCLUDES="${PROJECT[]_INCLUDES} -I${GSL_INCDIR}"
	fi

	dnl
	dnl Check for the GSL lib
	dnl

	AC_MSG_CHECKING(for libgsl)
	CF_FIND_LIB(GSL_LIBDIR, libgsl, ${GSL_LIBPATH})
	
	if test "${GSL_LIBDIR}" = "" ; then
		AC_MSG_RESULT((not found!))
		AC_MSG_RESULT()
		AC_MSG_RESULT([The GSL library could not be found. Please specify the path to libgsl.${SHARED_LIBRARY_SUFFIX}])
		AC_MSG_RESULT([by passing the option --with-gsl-lib=DIR to configure.])
		AC_MSG_RESULT([You may also set the environment variable GSL_LIBDIR to the correct])
		AC_MSG_RESULT([path - configure will recognize this, too.])
		AC_MSG_RESULT()
		AC_MSG_RESULT([Please install the library on your system, or disable it with --disable-gsl.])
		AC_MSG_RESULT()
		CF_ERROR
	else
		AC_MSG_CHECKING(for libgslcblas)
		if test "${GSL_LIBDIR}" != "" ; then
			CF_FIND_LIB(GSLCBLAS_LIBDIR, libgslcblas, ${GSL_LIBPATH})

			if test "${GSLCBLAS_LIBDIR}" = "" ; then
				AC_MSG_RESULT((not found!))
				AC_MSG_RESULT()
				AC_MSG_RESULT([The GSL library could not be found. Please specify the path to <libgsl.${SHARED_LIB_SUFFIX}>])
				AC_MSG_RESULT([by passing the option --with-gsl-lib=DIR to configure.])
				AC_MSG_RESULT([You may also set the environment variable GSL_LIB_DIR to the correct])
				AC_MSG_RESULT([path - configure will recognize this, too.])
				AC_MSG_RESULT()
				AC_MSG_RESULT([GSL is needed for signal processing.])
				AC_MSG_RESULT([Please install the library on your system, or disable it with --disable-gsl.])
				AC_MSG_RESULT()
				CF_ERROR
			fi
		fi		
		GSL_LIBS="${GSL_LIBS} -L${GSL_LIBDIR} -lgsl -lgslcblas"	
	fi

	PROJECT[]_HAS_GSL=true
	AC_SUBST(PROJECT[]_HAS_GSL)
])

