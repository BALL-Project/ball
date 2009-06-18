dnl
dnl	lpsolve
dnl
AC_DEFUN([CF_LPSOLVE], [
AC_MSG_CHECKING(Checking for lpsolve support)
dnl
dnl  variable substitutions required for lpsolve support
dnl
AC_SUBST(LPSOLVE_SUPPORT)
AC_SUBST(LPSOLVE_INCLUDES)
AC_SUBST(LPSOLVE_LIBS)

dnl
dnl Check for the lpsolve headers
dnl
if test "${LPSOLVE_SUPPORT}" = "true" ; then	
	AC_MSG_RESULT(enabled)

	AC_DEFINE(PROJECT[]_HAS_LPSOLVE)

	AC_MSG_CHECKING(for lpsolve header files)
	CF_FIND_HEADER(LPSOLVE_INCDIR, lpsolve/lp_lib.h, ${LPSOLVE_INCPATH})
	if test "${LPSOLVE_INCDIR}" = "" ; then
		AC_MSG_RESULT((not found!))
		AC_MSG_RESULT()
		AC_MSG_RESULT([The lpsolve headers could not be found. Please specify the path to <lpsolve/lp_lib.h>])
		AC_MSG_RESULT([by passing the option --with-lpsolve-incl=DIR to configure.])
		AC_MSG_RESULT()
		AC_MSG_RESULT([lpsolve is needed for integer linear programming.])
		AC_MSG_RESULT([Please install the library on your system, or disable it with --disable-lpsolve.])
		AC_MSG_RESULT()
		CF_ERROR
	else
		dnl  remove trailing '/lpsolve' from the include path
		LPSOLVE_INCDIR=`echo ${LPSOLVE_INCDIR} | sed "s/lpsolve$//"` 
		AC_MSG_RESULT((${LPSOLVE_INCDIR}))
		if test "${LPSOLVE_INCDIR}" != /usr/include; then
			PROJECT[]_INCLUDES="${PROJECT[]_INCLUDES} -I${LPSOLVE_INCDIR}"
		fi
	fi

	dnl
	dnl Check for the lpsolve lib. 
	dnl

	AC_MSG_CHECKING(for liblpsolve55.a)
	if test "${LPSOLVE_LIBDIR}" != "" ; then
		if test -a "${LPSOLVE_LIBDIR}/liblpsolve55.a" ; then
			LPSOLVE_LIBDIR="${LPSOLVE_LIBDIR}/"
		fi
	fi
	CF_FIND_LIB(LPSOLVE_LIBDIR, liblpsolve55, ${LPSOLVE_LIBPATH})

	if test "${LPSOLVE_LIBDIR}" = "" ; then
		AC_MSG_RESULT((not found!))
		AC_MSG_RESULT()
		AC_MSG_RESULT([The lpsolve library, version 5.5, could not be found. Please specify the path to liblpsolve55.a])
		AC_MSG_RESULT([by passing the option --with-lpsolve-lib=DIR to configure.])
		AC_MSG_RESULT([You may also set the environment variable LPSOLVE_LIBDIR to the correct])
		AC_MSG_RESULT([path - configure will recognize this, too.])
		AC_MSG_RESULT()
		AC_MSG_RESULT([Please install the library on your system, or disable it with --disable-lpsolve.])
		AC_MSG_RESULT()
		CF_ERROR
	else
		dnl
		dnl Prevent inclusion of /usr/lib
		dnl
		if test "${LPSOLVE_LIBDIR}" != /usr/lib ; then
			LPSOLVE_LIBS="-L${LPSOLVE_LIBDIR} -llpsolve55"
		else
			LPSOLVE_LIBS="-llpsolve55"
		fi
		AC_MSG_CHECKING(libraries needed for liblpsolve55)
		SAVE_LIBS=${LIBS}
		SAVE_LDFLAGS=${LDFLAGS}
		LIBS=${LPSOLVE_LIBS}
		LDFLAGS="${LDFLAGS} -I${LPSOLVE_INCDIR}"
		LPSOLVE_LINKS=0
		AC_TRY_LINK([
			#include <lpsolve/lp_lib.h>
			],
			[
			int major, minor, release, build;
			lp_solve_version(&major, &minor, &release, &build);
			],
			LPSOLVE_LINKS=1)
		LIBS=${SAVE_LIBS}
		LDFLAGS=${SAVE_LDFLAGS}
		if test "${LPSOLVE_LINKS}" = 1; then
			AC_MSG_RESULT((liblpsolve55 suffices!))
		else
			SAVE_LIBS=${LIBS}
			SAVE_LDFLAGS=${LDFLAGS}
			LPSOLVE_LIBS="${LPSOLVE_LIBS} -lm -ldl"
			LIBS=${LPSOLVE_LIBS}
			LDFLAGS="${LDFLAGS} -I${LPSOLVE_INCDIR}"
			LPSOLVE_LINKS=0
			AC_TRY_LINK([
				#include <lpsolve/lp_lib.h>
				],
				[
				int major, minor, release, build;
				lp_solve_version(&major, &minor, &release, &build);
				],
				LPSOLVE_LINKS=1)
			LIBS=${SAVE_LIBS}
			LDFLAGS=${SAVE_LDFLAGS}
			if test "${LPSOLVE_LINKS}" != 1; then
				AC_MSG_RESULT(could not link against liblpsolve55!)
				AC_MSG_RESULT()
				AC_MSG_RESULT([Please check your lpsolve installation or disable lpsolve support])
				AC_MSG_RESULT([by passing --disable-lpsolve to configure!])
				CF_ERROR
			else
				AC_MSG_RESULT((${LPSOLVE_LIBS}))
				PROJECT[]_HAS_LPSOLVE=true
				AC_SUBST(PROJECT[]_HAS_LPSOLVE)
			fi
		fi
	fi
else
	AC_MSG_RESULT(disabled)
fi

])
