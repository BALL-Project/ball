dnl
dnl  Test for GLEW (GL Extensions Wrangler) library used 
dnl  to handle OpenGL extensions in a more or less portable fashion
dnl
AC_DEFUN([CF_VIEW_GLEW], [
if test ${VIEW_PLATFORM} = OpenGL ; then
	AC_MSG_CHECKING(for GLEW library)
	CF_FIND_LIB(LIBPATH,libGLEW, ${GLEW_LIBPATH})
	if test "${LIBPATH}" = "" ; then
		AC_MSG_RESULT((not found!))
	else
		AC_MSG_RESULT((${LIBPATH}))
	fi
	GLEW_LIBPATH="${LIBPATH}"

	if test "${GLEW_LIBPATH}" != "" ; then
		AC_MSG_CHECKING(for GLEW headers)
		CF_FIND_HEADER(INCPATH,GL/glew.h, ${GLEW_INCPATH})

		if test "${INCPATH}" = "" ; then
			AC_MSG_RESULT((not found!))
			CF_ERROR(Could not find GLEW headers (GL/glew.h). Please specify their location with --with-glew-incl=DIR)
		else
			if test "${INCPATH}" != "" ; then
				AC_MSG_RESULT((${INCPATH}))
				AC_DEFINE(PROJECT[]_HAS_GLEW,)
				GLEW_INCPATH="${INCPATH}"
	
				dnl
				dnl  Define a few options used later to link/inlucde GLEW stuff.
 				dnl
				BALL_HAS_GLEW=true
				GLEW_LIBOPTS="-lGLEW"
				if test "${GLEW_LIBPATH}" != /usr/lib -a "${GLEW_LIBPATH}" != /lib -a "${GLEW_LIBPATH}" != "" ; then
					GLEW_LIBOPTS="-L${GLEW_LIBPATH} ${GLEW_LIBOPTS}"
				fi

				if test "${GLEW_INCPATH}" != /usr/include/GL && test "${GLEW_INCPATH}" != "" ; then
					GLEW_INCLUDES="-I${GLEW_INCPATH}"
					PROJECT[]_INCLUDES="${PROJECT[]_INCLUDES} ${GLEW_INCLUDES}"
				fi
			fi
		fi
	fi
fi
])


