dnl
dnl	Test for RTfact (Real Time Raytracing Library)
dnl
AC_DEFUN([CF_VIEW_RTFACT], [
	AC_MSG_CHECKING(for RTFact library)

	dnl
	dnl variable subsitutions required for RTfact support
	dnl
	AC_SUBST(RTFACT_SUPPORT)
	AC_SUBST(RTFACT_INCLUDES)
	AC_SUBST(RTFACT_LIBS)
	AC_SUBST(PROJECT[]_HAS_RTFACT)

	dnl
	dnl Check for the rtfact headers
	dnl
	if test "${RTFACT_SUPPORT}" = "true" ; then
		AC_MSG_RESULT(enabled)

		AC_DEFINE(PROJECT[]_HAS_RTFACT)

		AC_MSG_CHECKING(for RTfact header files)
		CF_FIND_HEADER(RTFACT_INCDIR, RTremote/Renderer.hpp, ${RTFACT_INCPATH})
		if test "${RTFACT_INCDIR}" = "" ; then
			AC_MSG_RESULT((not found!))
			AC_MSG_RESULT()
			AC_MSG_RESULT([The RTfact headers could not be found. Please specify the path to <RTremote/Renderer.hpp>])
			AC_MSG_RESULT([by passing the option --with-rtfact-incl=DIR to configure.])
			AC_MSG_RESULT()
			AC_MSG_RESULT([RTfact is needed for real time raytracing.])
			AC_MSG_RESULT([Please install the library on your system, or disable it with --disable-rtfact.])
			AC_MSG_RESULT()
			CF_ERROR
		else
			dnl remove trailing '/RTremote' from the include path
			RTFACT_INCDIR=$(echo ${RTFACT_INCDIR} | sed "s/RTremote$//")
			AC_MSG_RESULT((${RTFACT_INCDIR}))
			if test "${RTFACT_INCDIR}" != /usr/include; then
				PROJECT[]_INCLUDES="${PROJECT[]_INCLUDES} -I${RTFACT_INCDIR}"
			fi
		fi

		dnl
		dnl Check for the RTFactRemote lib.
		dnl
		AC_MSG_CHECKING(for libRTfactRemote.so)
		CF_FIND_LIB(RTFACT_LIBDIR, libRTfactRemote.so, ${RTFACT_LIBPATH})

		if test "${RTFACT_LIBDIR}" = "" ; then
			AC_MSG_RESULT((not found!))
			AC_MSG_RESULT()
			AC_MSG_RESULT([The RTfact library could not be found. Please specify the path to libRTfactRemote.so])
			AC_MSG_RESULT([by passing the option --with-rtfact-lib=DIR to configure.])
			AC_MSG_RESULT([You may also set the environment variable RTFACT_LIBDIR to the correct])
			AC_MSG_RESULT([path - configure will recognize this, too.])
			AC_MSG_RESULT()
			AC_MSG_RESULT([Please install the library on your system, or disable it with --disable-rtfact.])
			AC_MSG_RESULT()
			CF_ERROR
		else
			dnl
			dnl Prevent inclusion of /usr/lib
			dnl
			if test "${RTFACT_LIBDIR}" != /usr/lib ; then
				RTFACT_LIBS="-L${RTFACT_LIBDIR} -lRTfactRemote"
			else
				RTFACT_LIBS="-lRTfactRemote"
			fi
			AC_MSG_RESULT(${RTFACT_LIBS})

			AC_DEFINE(PROJECT[]_HAS_RTFACT)
			PROJECT[]_HAS_RTFACT=true
		fi
	else
		AC_MSG_RESULT(disabled)
	fi
])
