dnl Try to identify the X11 libraries to link against
AC_DEFUN([CF_VIEW_X_LINK_TEST], [

	dnl  
	dnl
	dnl  identify the X11 libraries needed to link against
	dnl
	dnl
	
	AC_MSG_CHECKING(linking against X11 libraries)
	dnl 
	dnl   if the user specified X libraries, try these first
	dnl
	if test "${X11_LIBS}" != "" ; then
		SAVE_LIBS=${LIBS}
		LIBS="${X11_LIBPATHOPT} ${X11_LIBS} ${LIBS}"
		AC_TRY_LINK([],[],X_LINKING_OK=1)
		LIBS=${SAVE_LIBS}
	fi

	dnl
	dnl  Special treatment for MacOS X -- we just ignore everything.
	dnl		The OpenGL and AGL frameworks will take care of it...
	dnl
	if test "${OS}" = "Darwin" ; then
		X11_LIBS=""
		X11_LIBPATHOPTS=""
		X_LINKING_OK="true"
	fi

	dnl 		
	dnl  now try the default guess: Xmu, Xext, Xt, and X11 
	dnl
	if test "${X_LINKING_OK+set}" != set ; then
		X11_LIBS="-lXmu -lXext -lXt -lX11 -lm"
		SAVE_LIBS=${LIBS}
		LIBS="${X11_LIBPATHOPT} ${X11_LIBS} ${LIBS}"
		AC_TRY_LINK([],[],X_LINKING_OK=1)
		LIBS=${SAVE_LIBS}
	fi
	
	dnl 		
	dnl  second guess: add SM and ICE
	dnl
	if test "${X_LINKING_OK+set}" != set ; then
		X11_LIBS="-lXmu -lXext -lXt -lX11 -lSM -lICE -lm"
		SAVE_LIBS=${LIBS}
		LIBS="${X11_LIBPATHOPT} ${X11_LIBS} ${LIBS}"
		AC_TRY_LINK([],[],X_LINKING_OK=1)
		LIBS=${SAVE_LIBS}
	fi
	
	dnl 		
	dnl  now try the default guess: Xmu, Xext, Xt, and X11 
	dnl
	if test "${X_LINKING_OK+set}" != set ; then
		X11_LIBS="-lXmu -lXt -lX11 -lm"
		SAVE_LIBS=${LIBS}
		LIBS="${X11_LIBPATHOPT} ${X11_LIBS} ${LIBS}"
		AC_TRY_LINK([],[],X_LINKING_OK=1)
		LIBS=${SAVE_LIBS}
	fi
	
	dnl 		
	dnl  second guess: add SM and ICE
	dnl
	if test "${X_LINKING_OK+set}" != set ; then
		X11_LIBS="-lXmu -lXt -lX11 -lSM -lICE -lm"
		SAVE_LIBS=${LIBS}
		LIBS="${X11_LIBPATHOPT} ${X11_LIBS} ${LIBS}"
		AC_TRY_LINK([],[],X_LINKING_OK=1)
		LIBS=${SAVE_LIBS}
	fi
	
	dnl 
	dnl  if we could not link - complain about it!
	dnl
	if test "${X_LINKING_OK+set}" = set ; then
		AC_MSG_RESULT(yes)	
	else
		AC_MSG_RESULT(no)
		AC_MSG_RESULT()
		AC_MSG_RESULT(Don't know how to link with X11 libraries.)
		AC_MSG_RESULT(Please specify the correct libraries (e.g. -lXmu -lXt -lX11) in the)
		AC_MSG_RESULT(environment variable X11_LIBS)
		AC_MSG_RESULT(If you are running Solaris 2.x you might also try the option --without-libxnet)
		AC_MSG_RESULT(if your X libraries were linked against libsocket and libnsl instead of libxnet.)
		AC_MSG_RESULT(Built of visualization component VIEW disabled.)
		AC_MSG_RESULT()
		USE_VIEW=false
	fi

	dnl		
	dnl  define some variables: X11_LIBOPTS
	dnl
	X11_LIBOPTS="${X11_LIBPATHOPT} ${X11_LIBS}"
])

