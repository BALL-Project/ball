dnl
dnl FFTW -- Fastest Fourier Transform in the West
dnl
AC_DEFUN([CF_CHECK_FFTW_SUPPORT], [
if test "${FFTW_SUPPORT}" = true ; then
	FFTW_SUPPORT=true

	AC_MSG_CHECKING(for FFTW headers)
	CF_FIND_HEADER(FFTW_INCL_PATH, fftw3.h, ${FFTW_INCL})

	if test "${FFTW_INCL_PATH}" = "" ; then
		AC_MSG_RESULT((not found!))
		AC_MSG_RESULT()
		AC_MSG_RESULT(No FFTW header files found! Please specify the path to the FFTW header fftw3.h)
		AC_MSG_RESULT(by passing the option --with-fftw-incl=DIR to configure.)
		AC_MSG_RESULT(The FFTW package can be found under the following URL:)
		AC_MSG_RESULT(  http://www.fftw.org)
		CF_ERROR
	else
		AC_MSG_RESULT((${FFTW_INCL_PATH}))
	fi

	if test "${FFTW_INCL_PATH}" != /usr/include ; then
		BALL_INCLUDES="${BALL_INCLUDES} -I${FFTW_INCL_PATH}"
	fi

	if test "${FFTW_DISABLE_FFTW_FLOAT}" = "false" ; then
		AC_MSG_CHECKING(for FFTW library with float support)
		CF_FIND_LIB(FFTW_LIB_F,libfftw3f, ${FFTW_LIBPATH})

		if test "${FFTW_LIB_F}" = "" ; then
			AC_MSG_RESULT((not found!))
			FFTW_DISABLE_FFTW_FLOAT=true
		else
			AC_MSG_RESULT((${FFTW_LIB_F}))
			FFTW_DISABLE_FFTW_FLOAT=false
		fi
	fi

	if test "${FFTW_DISABLE_FFTW_DOUBLE}" = "false" ; then
		AC_MSG_CHECKING(for FFTW library with double support)
		CF_FIND_LIB(FFTW_LIB_D,libfftw3, ${FFTW_LIBPATH})

		if test "${FFTW_LIB_D}" = "" ; then
			AC_MSG_RESULT((not found!))
			FFTW_DISABLE_FFTW_DOUBLE=true
		else
			AC_MSG_RESULT((${FFTW_LIB_D}))
			FFTW_DISABLE_FFTW_DOUBLE=false
		fi
	fi

	if test "${FFTW_DISABLE_FFTW_LONGDBL}" = false ; then
		AC_MSG_CHECKING(for FFTW library with long double support)
		CF_FIND_LIB(FFTW_LIB_L,libfftw3l, ${FFTW_LIBPATH})

		if test "${FFTW_LIB_L}" = "" ; then
			AC_MSG_RESULT((not found!))
			FFTW_DISABLE_FFTW_LONGDBL=true
		else
			AC_MSG_RESULT((${FFTW_LIB_L}))
			FFTW_DISABLE_FFTW_LONGDBL=false
		fi
	fi

	if test "${FFTW_LIB_F}" = "" -a "${FFTW_LIB_D}" = "" -a "${FFTW_LIB_L}" = "" ; then
		AC_MSG_RESULT(Please install it in a standard location or specify the path)
		AC_MSG_RESULT(with --with-fftw-lib=DIR on the command line.)
		CF_ERROR
	fi

	dnl prevent the use of -L/usr/lib - this may lead to problems with different
	dnl binary formats (e.g. SGI O32/N32 format)
	if test "${FFTW_INCL_PATH}" != /usr/include -a "${FFTW_INCL_PATH}" != "" ; then
		PROJECT[]_INCLUDES="${PROJECT[]_INCLUDES} -I${FFTW_INCL_PATH}"
	fi

	AC_DEFINE(PROJECT[]_HAS_FFTW, true)
	AC_DEFINE(PROJECT[]_HAS_FFTW_H, true)
	PROJECT[]_HAS_FFTW=true
	AC_SUBST(PROJECT[]_HAS_FFTW)
	AC_SUBST(PROJECT[]_HAS_FFTW_H)

	PROJECT[]_HAS_FFTW_FLOAT=""
	if test "${FFTW_DISABLE_FFTW_FLOAT}" = "false" ; then
		AC_MSG_CHECKING(linking against libfftw3f)
		SAVE_LIBS=${LIBS}
		SAVE_LDFLAGS=${LDFLAGS}
		LIBS="${FFTW_LIB_F}/libfftw3f.a ${LIBS}"
		LDFLAGS="$LDFLAGS -I${FFTW_INCL_PATH}"
		FFTW_LINKING_OK=0
		AC_TRY_LINK([
		#include <fftw3.h>
		],
		[
		 fftwf_plan f = fftwf_plan_dft_1d(1,0,0,1,FFTW_FORWARD);
		], FFTW_LINKING_OK=1)
		LIBS=${SAVE_LIBS}
		LDFLAGS=${SAVE_LDFLAGS}
		if test "${FFTW_LINKING_OK}" != "1" ; then
			AC_MSG_RESULT(no)
			AC_MSG_RESULT()
			AC_MSG_RESULT([Cannot link against libfftw3f. Please check config.log and])
			AC_MSG_RESULT([specify appropriate options to configure (e.g. --with-fftw-lib/incl).])
			CF_ERROR
		else
			AC_MSG_RESULT(yes)
			PROJECT[]_HAS_FFTW_FLOAT=true
		fi
	fi

	PROJECT[]_HAS_FFTW_DOUBLE=""
	if test "${FFTW_DISABLE_FFTW_DOUBLE}" = "false" ; then
		AC_MSG_CHECKING(linking against libfftw3)
		SAVE_LIBS=${LIBS}
		SAVE_LDFLAGS=${LDFLAGS}
		LIBS="${FFTW_LIB_D}/libfftw3.a ${LIBS}"
		LDFLAGS="$LDFLAGS -I${FFTW_INCL_PATH}"
		FFTW_LINKING_OK=0
		AC_TRY_LINK([
							#include <fftw3.h>
						],
						[
							 fftw_plan f = fftw_plan_dft_1d(1,0,0,1,FFTW_FORWARD);
						], FFTW_LINKING_OK=1)
		LIBS=${SAVE_LIBS}
		LDFLAGS=${SAVE_LDFLAGS}
		if test "${FFTW_LINKING_OK}" != "1" ; then
			AC_MSG_RESULT(no)
			AC_MSG_RESULT()
			AC_MSG_RESULT([Cannot link against libfftw3. Please check config.log and])
			AC_MSG_RESULT([specify appropriate options to configure (e.g. --with-fftw-lib/incl).])
			CF_ERROR
		else
			AC_MSG_RESULT(yes)
			PROJECT[]_HAS_FFTW_DOUBLE=true
		fi
	fi

	PROJECT[]_HAS_FFTW_LONG_DOUBLE=""
	if test "${FFTW_DISABLE_FFTW_LONGDBL}" = "false" ; then
		AC_MSG_CHECKING(linking against libfftw3l)
		SAVE_LIBS=${LIBS}
		SAVE_LDFLAGS=${LDFLAGS}
		LIBS="${FFTW_LIB_D}/libfftw3l.a ${LIBS}"
		LDFLAGS="$LDFLAGS -I${FFTW_INCL_PATH}"
		FFTW_LINKING_OK=0
		AC_TRY_LINK([
							#include <fftw3.h>
						],
						[
							 fftwl_plan f = fftwl_plan_dft_1d(1,0,0,1,FFTW_FORWARD);
						], FFTW_LINKING_OK=1)
		LIBS=${SAVE_LIBS}
		LDFLAGS=${SAVE_LDFLAGS}
		if test "${FFTW_LINKING_OK}" != "1" ; then
			AC_MSG_RESULT(no)
			AC_MSG_RESULT()
			AC_MSG_RESULT([Cannot link against libfftw3l. Please check config.log and])
			AC_MSG_RESULT([specify appropriate options to configure (e.g. --with-fftw-lib/incl).])
			CF_ERROR
		else
			AC_MSG_RESULT(yes)
			PROJECT[]_HAS_FFTW_LONG_DOUBLE=true
		fi
	fi

	AC_DEFINE_UNQUOTED(PROJECT[]_COMPLEX_TYPE, ${PROJECT[]_COMPLEX_TYPE})
	if test "${PROJECT[]_HAS_FFTW_FLOAT}" != "" ; then
		FFTW_LIBS="${FFTW_LIB_F}/libfftw3f.a"
		AC_DEFINE(PROJECT[]_HAS_FFTW_FLOAT,)
	fi
	if test "${PROJECT[]_HAS_FFTW_DOUBLE}" != "" ; then
		FFTW_LIBS="$FFTW_LIBS ${FFTW_LIB_D}/libfftw3.a"
		AC_DEFINE(PROJECT[]_HAS_FFTW_DOUBLE,)
	fi
	if test "${PROJECT[]_HAS_FFTW_LONG_DOUBLE}" != "" ; then
		FFTW_LIBS="$FFTW_LIBS ${FFTW_LIB_L}/libfftw3l.a"
		AC_DEFINE(PROJECT[]_HAS_FFTW_LONG_DOUBLE,)
	fi
		if test "${PROJECT[]_HAS_FFTW_H}" != "" ; then
		LDFLAGS="$LDFLAGS -I${FFTW_INCL_PATH}"
	fi

	dnl
	dnl Now, determine the default FFTW precision using the PROJECT[]_COMPLEX_PRECISION
	dnl determined earlier. If this FFTW lib corresponding to this type is not
	dnl available, we throw an error
	dnl
	if test "${PROJECT[]_COMPLEX_PRECISION}" == "float" ; then
		if test "${PROJECT[]_HAS_FFTW_FLOAT}" != "" ; then
			PROJECT[]_FFTW_DEFAULT_TRAITS="FloatTraits"
		else	
			AC_MSG_RESULT([The fftw3 library needed for the default complex type (float) is not available!])
			AC_MSG_RESULT([Please specify the desired default FFTW precision (--enable-...-cplx) and/or])
			AC_MSG_RESULT([the path to the fftw3 lib/headers needed for this type (--with--fftw-lib/incl).])
			CF_ERROR
		fi
	fi
	if test "${PROJECT[]_COMPLEX_PRECISION}" == "double" ; then
		if test "${PROJECT[]_HAS_FFTW_DOUBLE}" != "" ; then
			PROJECT[]_FFTW_DEFAULT_TRAITS="DoubleTraits"
		else	
			AC_MSG_RESULT([The fftw3 library needed for the default complex type (double) is not available!])
			AC_MSG_RESULT([Please specify the desired default FFTW precision (--enable-...-cplx) and/or])
			AC_MSG_RESULT([the path to the fftw3 lib/headers needed for this type (--with--fftw-lib/incl).])
			CF_ERROR
		fi
	fi
	if test "${PROJECT[]_COMPLEX_PRECISION}" == "long double" ; then
		if test "${PROJECT[]_HAS_FFTW_LONG_DOUBLE}" != "" ; then
			PROJECT[]_FFTW_DEFAULT_TRAITS="LongDoubleTraits"
		else	
			AC_MSG_RESULT([The fftw3 library needed for the default complex type (long double) is not available!])
			AC_MSG_RESULT([Please specify the desired default FFTW precision (--enable-...-cplx) and/or])
			AC_MSG_RESULT([the path to the fftw3 lib/headers needed for this type (--with--fftw-lib/incl).])
			CF_ERROR
		fi
	fi
fi 

AC_DEFINE_UNQUOTED(PROJECT[]_FFTW_DEFAULT_TRAITS, ${PROJECT[]_FFTW_DEFAULT_TRAITS})
AC_SUBST(PROJECT[]_HAS_FFTW_FLOAT,)
AC_SUBST(PROJECT[]_HAS_FFTW_DOUBLE,)
AC_SUBST(PROJECT[]_HAS_FFTW_LONG_DOUBLE,)
AC_SUBST(FFTW_LIBS)
])


