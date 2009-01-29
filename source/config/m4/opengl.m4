AC_DEFUN(CF_VIEW_OPENGL, [
dnl
dnl		Fix up the OpenGL stuff for MacOS X -- here we need to use OpenGL and AGL frameworks
dnl
if test "${OS}" = "Darwin" ; then
	VIEW_PLATFORM="OpenGL-Darwin"
	OPENGL_LIBOPTS="-framework Carbon -framework OpenGL -framework AGL"
	BALL_INCLUDES="${BALL_INCLUDES} -I/usr/X11R6/include"
	X11_LIBPATHOPT=""
	X11_INCPATH=""
	dnl OPENGL_INCPATH="-I/System/Library/Frameworks/OpenGL.framework/Versions/A/Headers -I/System/Library/Frameworks/AGL.framework/Versions/A/Headers"
	dnl VIEW_INCLUDES="${VIEW_INCLUDES} ${OPENGL_INCPATH}"
fi

if test "${VIEW_PLATFORM}" = Mesa ; then
	AC_MSG_CHECKING(for Mesa includes)
	CF_FIND_HEADER(MESA_INCLUDES,GL/gl.h, ${OPENGL_INCPATH} ${X11_INCPATH})
	if test "${MESA_INCLUDES}" = "" ; then
		AC_MSG_RESULT((not found!))
		AC_MSG_RESULT()
		AC_MSG_RESULT(No Mesa headers found! Please specify the path to the directory)
		AC_MSG_RESULT(containing the Mesa headers using --with-opengl-incl=DIR.)
		AC_MSG_RESULT(Mesa can be obtained from www.mesa3d.org.)
		CF_ERROR
	else
		AC_MSG_RESULT(${MESA_INCLUDES})
	fi

	if test "${USE_VIEW}" = true ; then
		AC_MSG_CHECKING(for Mesa library)
		CF_FIND_LIB(MESA_LIBS,libMesaGL, ${OPENGL_LIBPATH} ${X11_LIBPATH})
		if test "${MESA_LIBS}" = "" ; then
			CF_FIND_LIB(MESA_LIBS,libGL, ${OPENGL_LIBPATH} ${X11_LIBPATH})
		fi
		if test "${MESA_LIBS}" = "" ; then
			AC_MSG_RESULT((not found!))
			AC_MSG_RESULT()
			AC_MSG_RESULT(No Mesa library libMesaGL or libGL found! Please specify the path)
			AC_MSG_RESULT(to the directory containing the library using the --with-opengl-libs=DIR.)
			AC_MSG_RESULT(Mesa can be obtained from www.mesa3d.org.)
			AC_MSG_RESULT(Aborted.)
		else
			AC_MSG_RESULT((${MESA_LIBS}))
		fi

		dnl prevent the use of -L/usr/lib - this may lead to problems with different
		dnl binary formats (e.g. SGI O32/N32 format)
		if test "${MESA_INCLUDES}" != /usr/include -a "${MESA_INCLUDES}" != "" ; then
			VIEW_INCLUDES="${VIEW_INCLUDES} -I${MESA_INCLUDES}"
		fi
	fi
fi

if test ${VIEW_PLATFORM} = OpenGL ; then
	AC_MSG_CHECKING(for OpenGL includes)
	CF_FIND_HEADER(OPENGL_INCPATH,GL/gl.h)
	if test "${OPENGL_INCPATH}" = "" ; then
		AC_MSG_RESULT((not found!))
		AC_MSG_RESULT()
		AC_MSG_RESULT(no OpenGL headers found! Please use the option --with-opengl-incl=DIR)
		AC_MSG_RESULT(of configure to specify the correct path to these headers.)
		CF_ERROR
	else
		AC_MSG_RESULT((${OPENGL_INCPATH}))
	fi

	AC_MSG_CHECKING(for OpenGL library)
	CF_FIND_LIB(OPENGL_LIBPATH,libGL)
	if test "${OPENGL_LIBPATH}" = "" ; then
		AC_MSG_RESULT((not found!))
		AC_MSG_RESULT()
		AC_MSG_RESULT(no OpenGL lib found! Please use the option --with-opengl-libs=DIR)
		AC_MSG_RESULT(of configure to specify the correct path to these libraries.)
		CF_ERROR
	else
		AC_MSG_RESULT((${OPENGL_LIBPATH}))
	fi

	if test "${OPENGL_INCPATH}" != /usr/include && test "${OPENGL_INCPATH}" != "" ; then
		VIEW_INCLUDES="${VIEW_INCLUDES} -I${OPENGL_INCPATH}"
	fi
fi

])
dnl end of opengl tests


