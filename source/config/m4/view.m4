dnl
dnl		VIEW support
dnl
AC_DEFUN(CF_VIEW, [
dnl
dnl    search for X-libs and includes, QT and 3D stuff (OpenGL/MESA)
dnl 
if test "${USE_VIEW}" = true ; then
	if test "${OS}" != "Darwin" ; then
		AC_PATH_X
		X11_INCPATH=${x_includes}
		X11_LIBPATH=${x_libraries}

		if test "${no_x}" = "yes" ; then
			USE_VIEW=false
		fi
	fi

	if test "${X11_LIBPATH}" = "/usr/lib" -o "${X11_LIBPATH}" = "" ; then
		X11_LIBPATH=""
		X11_LIBPATHOPT=""
	else
		X11_LIBPATHOPT="-L${X11_LIBPATH}"
	fi

	dnl Check for OpenGL/Mesa
	CF_VIEW_OPENGL

	dnl Check for GLEW
	if test "${GLEW}" = "true"; then
		CF_VIEW_GLEW
	fi

	dnl Check for QT basics (version, headers, existence of library)
	CF_VIEW_QT_BASICS

	dnl Check for X11 libraries to link against
	CF_VIEW_X_LINK_TEST

  dnl Check whether we can link against OpenGL/Mesa
	CF_VIEW_OPENGL_LINK_TEST
	
	dnl Check whether we can link against all libraries together 
  dnl (build a simple QT executable and determine QT version number)
	CF_VIEW_QT_LINK_TEST

	dnl Check for QT executables required to build the dialogs (MOC, UIC)
	CF_VIEW_QT_EXECUTABLES

	dnl Check for special features, e.g. SQL drivers
	CF_VIEW_QT_SQL

	AC_DEFINE(PROJECT[]_HAS_VIEW,)
	LIBVIEW="libVIEW.a"
	VIEW="VIEW"
fi
])

