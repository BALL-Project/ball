dnl    $Id: configure.ac,v 1.1.2.3 2002/11/29 11:01:44 oliver Exp $
dnl    Process this file with autoconf to produce a configure script.
dnl   

dnl 
dnl    some initialization stuff...
dnl 
AC_INIT(BALL, 1.0, ball-bugs@bioinf.uni-sb.de)
AC_CONFIG_HEADER([
	config.h:config/config.h.in
])
AC_LANG(C++)

dnl   Require the user to accept the license (once)
CF_CHECK_LICENSE

dnl		
dnl    define command line arguments for QT, Mesa, and OpenGL
dnl    includes and libraries
dnl
AC_ARG_ENABLE(optimization,    [  --enable-optimization   optimize the library for speed. Omits debug info!])
AC_ARG_ENABLE(debuginfo,       [  --enable-debuginfo      add -g to the compiler flags (to create debug information)])
AC_ARG_ENABLE(BALLVIEW,        [  --disable-BALLVIEW      disable the compilation of BALLVIEW, the visualization component])
AC_ARG_ENABLE(64,              [  --enable-64             create 64 bit binaries (if allowed by the compiler)])
AC_ARG_WITH(compiler,          [  --with-compiler=CXX     use CXX as C++ compiler])
AC_ARG_WITH(cxxflags,          [  --with-cxxflags=FLAGS   add FLAGS to the C++ compiler flags (commas are converted to blanks)])
AC_ARG_WITH(ldflags,           [  --with-ldflags=FLAGS    add FLAGS to the linker flags (commas are converted to blanks)])
AC_ARG_WITH(arflags,           [  --with-arflags=FLAGS    add FLAGS to the flags for the creation of the static libraries])
AC_ARG_WITH(dynarflags,        [  --with-dynarflags=FLAGS add FLAGS to the flags for the creation of the shared libraries])
AC_ARG_WITH(qt-incl,           [  --with-qt-incl=DIR      QT header files are in DIR])
AC_ARG_WITH(qt-libs,           [  --with-qt-libs=DIR      QT libraries are in DIR])
AC_ARG_WITH(qt-mt,             [  --with-qt-mt            use a threadsafe version of the QT lib (libqt-mt)])
AC_ARG_WITH(moc,               [  --with-moc=MOC          use MOC as the absolute path to the QT meta object compiler (moc)])
AC_ARG_WITH(opengl-incl,       [  --with-opengl-incl=DIR  OpenGL/Mesa header files are in DIR/GL])
AC_ARG_WITH(opengl-libs,       [  --with-opengl-libs=DIR  OpenGL/Mesa libraries are in DIR/GL])
AC_ARG_WITH(mesa,              [  --with-mesa             use MESA instead of OpenGL])
AC_ARG_WITH(libxnet,           [  --without-libxnet       use -lsocket -lnsl rather than -lxnet (under Solaris)])
AC_ARG_ENABLE(python,          [  --enable-python         enable Python support and search for executable])
AC_ARG_WITH(python,            [  --with-python=EXE       enable Python support and use Python executable in EXE])
AC_ARG_WITH(python-incl,       [  --with-python-incl=DIR  Python includes (Python.h) is in DIR])
AC_ARG_WITH(python-libs,       [  --with-python-libs=DIR  Python library (libpython*.a) is in DIR])
AC_ARG_WITH(python-ldopts,     [  --with-python-ldopts=X  Use additional options X when linking with the Python library])
AC_ARG_WITH(sip,   	           [  --with-sip=DIR          the SIP executable resides in DIR])
AC_ARG_WITH(sip-lib,   	       [  --with-sip-lib=DIR      the SIP library resides in DIR])
AC_ARG_WITH(sip-incl,   	     [  --with-sip-incl=DIR     the SIP header file resides in DIR])
AC_ARG_WITH(xdr,               [  --without-xdr           no RPC/XDR headers available - do not build portable binary persistence support])
AC_ARG_ENABLE(fftw,            [  --enable-fftw           enable the build of BALL with support for FFTW (a FFT library)])
AC_ARG_WITH(fftw-lib,          [  --with-fftw-lib=DIR     libfftw is in DIR])
AC_ARG_WITH(fftw-incl,         [  --with-fftw-incl=DIR    fftw includes are in DIR])
AC_ARG_ENABLE(double-cpx,      [  --enable-double-cplx    use double precision floats for Complex type])

dnl
dnl    interpret switches...
dnl
if test "$enable_optimization" = "yes" ; then
	DEBUG=false
else
	DEBUG=true
fi

if test "$enable_debuginfo" = "yes" ; then
	DEBUG_INFO=true
else
	DEBUG_INFO=false
fi

if test "$enable_64" = "yes" ; then
	BINFMT_64_BIT=true
else
	BINFMT_64_BIT=false
fi

if test "$with_compiler" != "" ; then
	CXX=$with_compiler
fi

if test "$with_cxxflags" != "" ; then
	CXXFLAGS="${CXXFLAGS}`echo $with_cxxflags|tr , \  `"
fi

if test "$with_ldflags" != "" ; then
	LDFLAGS="${LDFLAGS}`echo $with_ldflags|tr ,  \  `"
fi

if test "$with_dynarflags" != "" ; then
	DYNAROPTS="${DYNAROPTS}`echo $with_dynarflags|tr ,  \  `"
fi

if test "$with_arflags" != "" ; then
	AROPTS="${AROPTS}`echo $with_arflags|tr ,  \  `"
fi

if test "$with_qt_incl" != "" ; then
	QT_INCPATH=$with_qt_incl
fi
if test "$with_qt_libs" != "" ; then
	QT_LIBPATH=$with_qt_libs
fi
if test "$with_qt_mt" = "yes" ; then
	USE_THREADSAFE_QT=true
	QT_MT_SUFFIX="-mt"
else
	USE_THREADSAFE_QT=false
	QT_MT_SUFFIX=""
fi
if test "$with_opengl_incl" != "" ; then
	OPENGL_INCPATH=$with_opengl_incl
fi
if test "$with_opengl_libs" != "" ; then
	OPENGL_LIBPATH=$with_opengl_libs
fi

if test "$with_mesa" != "yes" ; then	
	BALLVIEW_PLATFORM=OpenGL
else
 	BALLVIEW_PLATFORM=Mesa
fi

if test "$with_moc" != "" ; then	
	MOC=$with_moc
else
 	MOC=moc
fi

if test "$with_libxnet" != "no" ; then	
	USE_LIBXNET=true
else
	USE_LIBXNET=false
fi

if test "$with_BALLVIEW" = "no" ; then
	USE_BALLVIEW=false
else	
	USE_BALLVIEW=true
fi

if test "$with_xdr" = "no" ; then
	BALL_NO_XDR=true
else
	BALL_NO_XDR=false
fi

# use this switch to enable the python support
# this also builds libBALLc.so and requires
# Python.h and libsip.a
if test "$with_python" != "" -a "$with_python" != "no" -o "$enable_python" = yes ; then
	PYTHON_SUPPORT=true
	if test "${with_python}" != "" -a "${with_python}" != "yes" ; then
		PYTHON_EXE=${with_python}
	else
		PYTHON_EXE=python
	fi
else
	PYTHON_SUPPORT=
fi

# the path to the python includes (Python.h)
PYTHON_INCLUDE_PATH=$with_python_incl

# the path to the python library (libpython*.a)
# and additional linker options (e.g. -ltermcap etc.)
PYTHON_LIBPATH=$with_python_libs
PYTHON_LDOPTS=$with_python_ldopts


# the paths to the SIP library, header, and executable
SIP_LIBPATH=$with_sip_lib
SIP_INCLUDE_PATH=$with_sip_incl
SIP=$with_sip

# support for fftw (FFT lib)
if test "$with_fftw" != "" -a "$with_fftw" != "no" -o "$enable_fftw" = yes ; then
	FFTW_SUPPORT=true
	if test "${with_fftw_lib}" != "" ; then
		FFTW_LIBPATH=${with_fftw_lib}
	else
		FFTW_LIBPATH=""
	fi
	FFTW_INCL=${with_fftw_incl}
else
	FFTW_SUPPORT=
fi

# size of Complex type (if given)
if test "$enable_double_cplx" = yes ; then
	FFTW_PRECISION=double
else
	FFTW_PRECISION=float
fi

dnl
dnl 	global definitions
dnl

dnl		the file containing the list of supported (configure`d) 
dnl   binary formats. configure will add a line with ${BINFMT} to this file
BINFORMAT_FILE=config/binary_formats
BALL_PATH=`cd ..; pwd`


dnl
dnl   check for programs used to determine architecture
dnl

AC_PROG_LN_S
CF_MSG_PATH_PROG(UNAME,uname)
CF_MSG_PATH_PROG(CUT,cut)
CF_MSG_PATH_PROG(TR,tr)
CF_MSG_PATH_PROG(AR,ar)
CF_MSG_PATH_PROG(CP,cp,no)
CF_MSG_PATH_PROG(RM,rm,no)
CF_MSG_PATH_PROG(MV,mv,no)
CF_MSG_PATH_PROG(LN,ln,no)
CF_MSG_PATH_PROG(SED,sed,no)
CF_MSG_PATH_PROG(FIND,find,no)
CF_MSG_PATH_PROG(DIFF,diff,no)
CF_MSG_PATH_PROG(TAIL,tail,no)
CF_MSG_PATH_PROG(GREP,grep,no)
CF_MSG_PATH_PROG(EGREP,egrep,no)
CF_MSG_PATH_PROG(CAT,cat,no)

dnl
dnl 		Check whether the default shell (/bin/sh) understands "echo -e".
dnl
CF_CHECK_ECHO
CF_CHECK_FIND

dnl
dnl   	Determine OS and architecture and all this stuff.
dnl
CF_DETECT_OS


dnl
dnl			Search for the C++ compiler in the (OS-determined) search order.
CF_SEARCH_CXX

dnl
dnl  		Try to identify the compiler and determine its version number.
dnl
CF_IDENTIFY_GXX
if test "${CXX_IDENTIFIED}" != true ; then CF_IDENTIFY_KAI fi
if test "${CXX_IDENTIFIED}" != true ; then CF_IDENTIFY_SUN fi
if test "${CXX_IDENTIFIED}" != true ; then CF_IDENTIFY_SGI fi
if test "${CXX_IDENTIFIED}" != true ; then CF_IDENTIFY_COMPAQ fi
if test "${CXX_IDENTIFIED}" != true ; then CF_IDENTIFY_INTEL fi
CF_BUILD_FULL_CXX_NAME

dnl
dnl		Default (fall-back) for make depend
dnl
CXX_MAKEDEPEND="makedepend"
MAKEDEP_CXX_OPTS="-f.Dependencies -D__cplusplus --"

dnl	
dnl		Set the compiler-specific options, archiver, linker, etc.
dnl
case "${CXX_NAME}" in
	g++ )				CF_GXX_OPTIONS ;;
	KAI ) 			CF_KAI_OPTIONS ;;
	Intel ) 		CF_INTEL_OPTIONS ;;
	Compaq )		CF_COMPAQ_OPTIONS ;;
	MIPSpro )		CF_MIPSPRO_OPTIONS ;;
	SunCC )			CF_SUNCC_OPTIONS ;;
esac

dnl
dnl   checking for DEBUG-Flag
dnl
CF_CHECK_DEBUG_FLAG

dnl
dnl		checks for header files
dnl

dnl
dnl		some platforms need to include ieeefp.h
dnl		for the definition of finite
dnl
if test "$OS" = IRIX -o "$OS" = Solaris ; then
	INCLUDE_IEEEFP=true
	BALL_INCLUDE_IEEEFP=true
fi

dnl 	check for standard C headers
dnl 	and how to run the preprocessor
AC_HEADER_STDC



dnl
dnl		Check for some C++/STL features
dnl		  -- the numeric_limits class
dnl     -- null template arguments (whether <> is required)
dnl 		-- ANSI-compliant iostreams 
dnl			-- ARM vs. ANSI-style access modifications
dnl
CF_CHECK_NUM_LIMITS
CF_CHECK_TPL_NULL_ARGS
CF_CHECK_ANSI_IOSTREAM
CF_CHECK_ARM_ACCESS_MODIFICATION

dnl
dnl		Check for the word lengths of different C/C++ types, define
dnl			the correct replacement types, and determine the endianness of 
dnl			the machine.
dnl
CF_GET_TYPE_SIZES
CF_C_BIGENDIAN

dnl
dnl		Checks for a couple of headers files and whether they are required.
dnl
AC_TYPE_SIZE_T
AC_HEADER_TIME
CF_CHECK_STDINT_H
CF_CHECK_IEEEFP_H
CF_CHECK_VALUES_H
CF_CHECK_REGEX_H

dnl
dnl		Check whether vsnprintf is defined and working correctly.
dnl
CF_CHECK_VSNPRINTF

dnl   
dnl   Check for some network stuff needed for socket class
dnl   this test tries to identify the neccessary libraries
dnl		solaris needs xnet or socket/nsl to link the tcp stuff
dnl  	

dnl
dnl		Check whether we need sysinfo or gethostname, in which libraries
dnl			the network stuff is hidden, and whether which arguments types
dnl			the functions accept.
dnl
CF_CHECK_GETHOSTNAME
CF_CHECK_NETLIBS
CF_CHECK_SOCKET_ARGS_AND_TYPES

dnl
dnl   Check for the XDR functions: their interface and the libraries they're hidden in.
dnl			XDR is required for a portable, platform-independent data exchange.
CF_CHECK_XDR

dnl
dnl		We need Flex and Bison for a couple of parsers hideen in BALL (SMILES, Expression, ParsedFct)
dnl
AC_PROG_LEX
AC_PROG_YACC
if test "${LEX}" != "flex" -o "${YACC}" = "" ; then
	BALL_HAS_FLEX_YACC=false
	AC_DEFINE(BALL_HAS_FLEX_YACC, false)
	AC_MSG_RESULT(flex and yacc/bison required!)
	AC_MSG_RESULT(Please install them from your closest GNU mirror.)	
	AC_MSG_RESULT()
	AC_MSG_ERROR(Aborted.)
else
	BALL_HAS_FLEX_YACC=true
	AC_DEFINE(BALL_HAS_FLEX_YACC, true)
	LIBS="${LIBS}"
fi
AC_SUBST(BALL_HAS_FLEX_YACC)
AC_SUBST(LEX)
AC_SUBST(YACC)


dnl
dnl		Check for optional support libraries:
dnl

dnl
dnl FFTW -- Fastest Fourier Transform in the West
dnl
CF_CHECK_FFTW_SUPPORT



dnl
dnl		BALLVIEW
dnl

dnl
dnl    search for X-libs and includes and BALLView (OpenGL/MESA) stuff
dnl 
if test "${USE_BALLVIEW}" = true ; then
	AC_PATH_X
	X11_INCPATH=${x_includes}
	X11_LIBPATH=${x_libraries}

	if test "${no_x}" = "yes" ; then
		USE_BALLVIEW=false
	fi

	if test "${USE_BALLVIEW}" = true ; then
		if test "${X11_LIBPATH}" = "/usr/lib" -o "${X11_LIBPATH}" = "" ; then
			X11_LIBPATH=""
			X11_LIBPATHOPT=""
		else
			X11_LIBPATHOPT="-L${X11_LIBPATH}"
		fi
	fi

	if test "${USE_BALLVIEW}" = true ; then
		if test "${BALLVIEW_PLATFORM}" = Mesa ; then
			AC_MSG_CHECKING(for Mesa includes)
			CF_FIND_HEADER(MESA_INCLUDES,GL/gl.h, ${OPENGL_INCPATH} ${X11_INCPATH})
			if test "${MESA_INCLUDES}" = "" ; then
				AC_MSG_RESULT((not found!))
				AC_MSG_RESULT()
				AC_MSG_RESULT(No Mesa headers found! Please specify the path to the directory)
				AC_MSG_RESULT(containing the Mesa headers using --with-opengl-incl=DIR.)
				AC_MSG_RESULT(Mesa can be obtained from www.mesa3d.org.)
				AC_MSG_ERROR(Aborted.)
			else
				AC_MSG_RESULT(${MESA_INCLUDES})
			fi

			if test "${USE_BALLVIEW}" = true ; then
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
			fi
			
			dnl prevent the use of -L/usr/lib - this may lead to problems with different
			dnl binary formats (e.g. SGI O32/N32 format)
			if test "${MESA_INCLUDES}" != /usr/include -a "${MESA_INCLUDES}" != "" ; then
				BALLVIEW_INCLUDES="${BALLVIEW_INCLUDES} -I${MESA_INCLUDES}"
			fi
		fi

		if test ${BALLVIEW_PLATFORM} = OpenGL ; then
			AC_MSG_CHECKING(for OpenGL includes)
			CF_FIND_HEADER(OPENGL_INCPATH,GL/gl.h)
			if test "${OPENGL_INCPATH}" = "" ; then
				AC_MSG_RESULT((not found!))
				AC_MSG_RESULT()
				AC_MSG_RESULT(no OpenGL headers found! Please use the option --with-opengl-incl=DIR)
				AC_MSG_RESULT(of configure to specify the correct path to these headers.)
				AC_MSG_ERROR(aborted)
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
				AC_MSG_ERROR(aborted)
			else
				AC_MSG_RESULT((${OPENGL_LIBPATH}))
			fi
			
			if test "${OPENGL_INCPATH}" != /usr/include && test "${OPENGL_INCPATH}" != "" ; then
				BALLVIEW_INCLUDES="${BALLVIEW_INCLUDES} -I${OPENGL_INCPATH}"
			fi
		fi

		if test "${USE_BALLVIEW}" = true ; then
			AC_MSG_CHECKING(for QT headers)
			if test "${QTDIR}" != "" ; then
				CF_FIND_HEADER(QT_INCPATH,qgl.h,${QTDIR}/include ${BALL_PATH}/contrib/qt/include)
			else
				CF_FIND_HEADER(QT_INCPATH,qgl.h,${BALL_PATH}/contrib/qt/include)
			fi

			if test "${QT_INCPATH}" = "" ; then
				AC_MSG_RESULT((not found!))
				AC_MSG_RESULT()
				AC_MSG_RESULT(No QT header files found! Please specify the path to the QT headers)
				AC_MSG_RESULT(by passing the option --with-qt-incl=DIR to configure.)
				AC_MSG_RESULT(You may also set the environment variable QTDIR to the correct)
				AC_MSG_RESULT(path - configure will recognize this, too.)
				AC_MSG_RESULT(The QT package can be found under the following URL:)
				AC_MSG_RESULT(  http://www.troll.no/qt)
				AC_MSG_ERROR(Aborted.)
			else
				AC_MSG_RESULT((${QT_INCPATH}))	
			fi

			AC_MSG_CHECKING(for libqt${QT_MT_SUFFIX})
			if test "${QTDIR}" != "" ; then
				CF_FIND_LIB(QT_LIBPATH, libqt${QT_MT_SUFFIX}, ${QTDIR}/lib ${QTDIR}/lib/${BINFMT} ${BALL_PATH}/contrib/qt/include)
			else
				CF_FIND_LIB(QT_LIBPATH, libqt${QT_MT_SUFFIX}, ${BALL_PATH}/contrib/qt/lib ${BALL_PATH}/contrib/qt/lib/${BINFMT})
			fi

			if test "${QT_LIBPATH}" = "" ; then
				AC_MSG_RESULT((not found!))
				AC_MSG_RESULT()
				AC_MSG_RESULT([The QT library could not be found. Please specify the path to libqt])
				AC_MSG_RESULT([by passing the option --with-qt-libs=DIR to configure.])
				AC_MSG_RESULT([You may also set the environment variable QTDIR to the correct])
				AC_MSG_RESULT([path - configure will recognize this, too.])
				AC_MSG_RESULT([If the QT library was built with thread support enabled (liqt-mt])
				AC_MSG_RESULT([instead of libqt), please specify the option --with-threadsafe-qt.])
				AC_MSG_RESULT([The QT package can be found under the following URL:])
				AC_MSG_RESULT(  http://www.troll.no/qt)
				AC_MSG_RESULT()
				AC_MSG_ERROR(Aborted.)
			else
				AC_MSG_RESULT((${QT_LIBPATH}))	
			fi

			
			dnl
			dnl extract the QT version number and version number string from include/qglobal.h
			dnl
			QT_VERSION=`${GREP} "#define QT_VERSION[^_]" ${QT_INCPATH}/qglobal.h | ${TR} '\011' ' ' | ${TR} -s ' ' | ${CUT} -d\  -f3`
			QT_VERSION_STR=`${GREP} "#define QT_VERSION_STR" ${QT_INCPATH}/qglobal.h | ${TR} '\011' ' ' | ${TR} -s ' ' | ${CUT} -d\  -f3`
			AC_MSG_CHECKING(for QT version number in qglobal.h)
			if test "${QT_VERSION}" = "" ; then
				AC_MSG_RESULT([<unknown>])
				AC_MSG_RESULT()
				AC_MSG_RESULT([  Could not determine version number of QT library -- please])
				AC_MSG_RESULT([  check config.log for details.])
				AC_MSG_RESULT([  You might have a problem with your LD_LIBRARY_PATH.])
				AC_MSG_RESULT([  Please check the settings of QTDIR as well or specify])
				AC_MSG_RESULT([  the path to the library/headers with])
				AC_MSG_RESULT([    --with-qt-libs=<DIR> / --with-qt-incl=<DIR>])
				AC_MSG_RESULT()
				AC_MSG_ERROR(Aborted.)
			else
				AC_MSG_RESULT([${QT_VERSION} (${QT_VERSION_STR})])
			fi			
	
			dnl
			dnl  QT libraries before release 2.0 contained the OGL support in a separate
			dnl   librarie: libqgl.a, so we hav to look for that one as well....
			dnl
			if test `echo ${QT_VERSION} | ${CUT} -c1-2` != "0x" ; then
				if test "${QT_VERSION}" -lt 200 ; then
					QGL_LIBPATH=${QT_LIBPATH}
					AC_MSG_CHECKING(for libqgl)
					if test "${QTDIR}" != "" ; then
						CF_FIND_LIB(QGL_LIBPATH,libqgl,${QTDIR}/lib ${QTDIR}/lib/${BINFMT} ${BALL_PATH}/contrib/qt/include)
					else
						CF_FIND_LIB(QGL_LIBPATH,libqgl,${BALL_PATH}/contrib/qt/lib ${BALL_PATH}/contrib/qt/lib/${BINFMT})
					fi

					if test "${QGL_LIBPATH}" = "" ; then
						AC_MSG_RESULT((not found!))
						AC_MSG_RESULT()
						AC_MSG_RESULT([The QT Opengl library libqgl.a could not be found. Please specify])
						AC_MSG_RESULT([the path to libqt/libqgl by passing the option --with-qt-libs=DIR])
						AC_MSG_RESULT([to configure.])
						AC_MSG_RESULT([You may also set the environment variable QTDIR to the correct])
						AC_MSG_RESULT([path - configure will recognize this, too.])
						AC_MSG_RESULT([Perhaps you simple forgot to compile the OpenGl extsions, too?])
						AC_MSG_RESULT([You find these extensions in ${QTDIR}/extensions/opengl])
						AC_MSG_RESULT()
						AC_MSG_RESULT([The complete QT package can be found under the following URL:])
						AC_MSG_RESULT([  http://www.troll.no/qt])
						AC_MSG_RESULT()
						AC_MSG_ERROR(Aborted.)
					else
						AC_MSG_RESULT((${QT_LIBPATH}))	
					fi
					if test "${QGL_PATH}" != "${QT_PATH}" ; then
						QT_PATH=${QGL_PATH}
						AC_MSG_RESULT(using ${QT_PATH} to look for liqt.so and libqgl.s)
					fi
				fi
			fi

			if test "${QT_INCPATH}" != /usr/include && test "${QT_INCPATH}" != "" ; then
				BALLVIEW_INCLUDES="${BALLVIEW_INCLUDES} -I${QT_INCPATH}"
			fi	
		fi
	fi
fi


dnl
dnl   verify libraries needed for BALLVIEW
dnl   (X, QT, Mesa/OpenGL)
dnl

if test "${USE_BALLVIEW}" = true ; then		
	dnl  
	dnl
	dnl  identify the X11 libraries needed to link agains
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
		AC_MSG_RESULT(Built of visualization component BALLVIEW disabled.)
		AC_MSG_RESULT()
		USE_BALLVIEW=false
	fi

	dnl		
	dnl  define some variables: X11_LIBOPTS and BALLVIEW_LIBS
	dnl
	X11_LIBOPTS="${X11_LIBPATHOPT} ${X11_LIBS}"
fi

if test "${USE_BALLVIEW}" = true ; then
	if test "${BALLVIEW_PLATFORM}" = OpenGL ; then
		if test "${OPENGL_LIBPATH}" != "/usr/lib" -a "${OPENGL_LIBPATH}" != "" ; then
			OPENGL_LIBOPTS="-L${OPENGL_LIBPATH} -lGLU -lGL"
		else
			OPENGL_LIBPATH=""
			OPENGL_LIBOPTS="-lGLU -lGL"
		fi

		dnl make sure we have OpenGL libs and no Mesa libs!
		dnl
		SAVE_LIBS=${LIBS}
		SAVE_LDFLAGS=${LDFLAGS}
		LIBS="${LIBS} ${X11_LIBOPTS}"
		if test "${OPENGL_LIBPATH}" != "" ; then
			LDFLAGS="${LDFLAGS} -L${OPENGL_LIBPATH}"
		fi
		AC_CHECK_LIB(GL, XMesaGarbageCollect, BALLVIEW_PLATFORM=Mesa)
		LIBS=${SAVE_LIBS}
		LDFLAGS=${SAVE_LDFLAGS}
		if test "${BALLVIEW_PLATFORM}" != Mesa ; then
			AC_MSG_CHECKING(linking against OpenGL libraries)
			SAVE_LIBS=${LIBS}
			LIBS="${OPENGL_LIBOPTS} ${LIBS}"
			AC_TRY_LINK([],[],OPENGL_LINKING_OK=1)
			LIBS=${SAVE_LIBS}
			if test "${OPENGL_LINKING_OK+set}" != set ; then
				AC_MSG_RESULT(no)
				AC_MSG_RESULT()
				AC_MSG_RESULT(Cannot link against libGL/GLU - disabling visualization support!)
				AC_MSG_RESULT(Please specify the path to OpenGL libraries using --with-opengl-libs=DIR)
				AC_MSG_RESULT()
				AC_MSG_ERROR(Aborted)
			else
				AC_MSG_RESULT(yes)
			fi
		fi
	fi
fi

if test "${USE_BALLVIEW}" = true ; then
	if test "${BALLVIEW_PLATFORM}" = Mesa ; then
		dnl
		dnl  strip default path
		dnl
	
		if test "${MESA_LIBS}" = "" ; then 
			MESA_LIBS=${OPENGL_LIBPATH}
		fi
		if test "${MESA_LIBS}" != "/usr/lib" -a "${MESA_LIBS}" != "" ; then
			OPENGL_LIBPATH="${MESA_LIBS}"
			OPENGL_LIBPATHOPT="-L${MESA_LIBS}"			
		else
			OPENGL_LIBPATH=""
			OPENGL_LIBPATHOPT=""
		fi
		
		dnl
		dnl  out first guess for the names of the Mesa libraries
		dnl
		OPENGL_LIBS="-lGLU -lGL"

		dnl
		dnl  try to link against mesa libraries
		dnl
		AC_MSG_CHECKING(linking against Mesa libs)
		SAVE_LIBS=${LIBS}
		LIBS="${OPENGL_LIBPATHOPT} ${OPENGL_LIBS} ${X11_LIBOPTS} ${LIBS} "
		AC_TRY_LINK([],[], HAVE_MESALIBS=1)
		LIBS=${SAVE_LIBS}

		dnl
		dnl  could not link against libGLU/libGL,
    dnl  so try libMesaGLU/libMesaGL
		dnl
		if test "${HAVE_MESALIBS+set}" != set ; then
			OPENGL_LIBS="-lMesaGLU -lMesaGL"
			SAVE_LIBS=${LIBS}
			LIBS="${OPENGL_LIBPATHOPT} ${OPENGL_LIBS} ${X11_LIBOPTS} ${LIBS} "
			AC_TRY_LINK([],[], HAVE_MESALIBS=1)
			LIBS=${SAVE_LIBS}
		fi

		if test "${HAVE_MESALIBS+set}" != set ; then
			AC_MSG_RESULT(no)
			AC_MSG_RESULT()
			AC_MSG_RESULT(Cannot link against libMesaGL/GLU - disabling visualization support!)
			AC_MSG_RESULT(Please specify the path to libMesaGL using --with-opengl-libs=DIR)
			AC_MSG_RESULT()
			AC_MSG_ERROR(Aborted.)
		else
			AC_MSG_RESULT(yes)
			OPENGL_LIBOPTS="${OPENGL_LIBPATHOPT} ${OPENGL_LIBS}"
		fi
	fi
fi

if test "${USE_BALLVIEW}" = true ; then
	if test "${QT_LIBPATH}" != "/usr/lib" ; then
		QTQGL_LIBOPTS="-L${QT_LIBPATH} -lqgl -lqt${QT_MT_SUFFIX}"
		QT_LIBOPTS="-L${QT_LIBPATH} -lqt${QT_MT_SUFFIX}"
	else 
		QT_LIBPATH=""
		QTQGL_LIBOPTS="-lqgl -lqt${QT_MT_SUFFIX}"
		QT_LIBOPTS="-lqt${QT_MT_SUFFIX}"
	fi
fi

if test "${USE_BALLVIEW}" = true ; then
	AC_MSG_CHECKING(linking against QT libraries)

	SAVE_LIBS=${LIBS}
	LIBS="${QTQGL_LIBOPTS} ${OPENGL_LIBOPTS} ${X11_LIBOPTS} ${LIBS} ${BALLVIEW_INCLUDES}"
	AC_TRY_LINK([#include <qgl.h>], [QGLWidget widget;], QT_LINKING_OK=1)
	LIBS=${SAVE_LIBS}

	if test "${QT_LINKING_OK+set}" != set ; then
		SAVE_LIBS=${LIBS}
		LIBS="${QT_LIBOPTS} ${OPENGL_LIBOPTS} ${X11_LIBOPTS} ${LIBS} ${BALLVIEW_INCLUDES}"
		AC_TRY_LINK([#include <qgl.h>], [QGLWidget wid;], QT_LINKING_OK=1)
		LIBS=${SAVE_LIBS}
	else
		dnl link against qgl as well (for qt <= 2.0)
		QT_LIBOPTS="${QTQGL_LIBOPTS}"
	fi

	if test "${QT_LINKING_OK+set}" != set ; then
		AC_MSG_RESULT(no)
		AC_MSG_RESULT()
		AC_MSG_RESULT([Cannot link against libqgl/qt - disabling support for visualization!])
		AC_MSG_RESULT([If QT is installed, please specify the path to the library])
		AC_MSG_RESULT([using the option --with-qt-libs=DIR])
		AC_MSG_RESULT()
		AC_MSG_ERROR(Aborted.)
	else
		AC_MSG_RESULT(yes)
		
		dnl  
		dnl  identify the version of the library
		dnl
		AC_MSG_CHECKING(QT library version)
		SAVE_LIBS=${LIBS}
		LIBS="${QT_LIBOPTS} ${OPENGL_LIBOPTS} ${X11_LIBOPTS} ${LIBS}"
		LD_LIBRARY_PATH="${X11_LIBPATH}:${OPENGL_LIBPATH}:${QT_LIBPATH}:${LD_LIBRARY_PATH}"
		export LD_LIBRARY_PATH
		echo "LD_LIBRARY_PATH = ${LD_LIBRARY_PATH}" 1>&5
		AC_TRY_RUN(
			[
				#include <stdio.h> 
				const char* qVersion();
				int main()
				{
					FILE* f = fopen("qt.version", "w");
					fprintf(f, "%s\n", qVersion());
					fclose(f);
					return 0;
				}
			], 
			QT_VERSION_OK=1,
			DUMMY=0,
			DUMMY=0
		)
		LIBS=${SAVE_LIBS}
		
		dnl
		dnl	if the program compiled and ran successfully,
		dnl extract the QT version number
		dnl
		if test "${QT_VERSION_OK+set}" != set; then
			AC_MSG_RESULT(no)
			AC_MSG_RESULT()
			AC_MSG_RESULT(The execution of a program linked against the QT)
			AC_MSG_RESULT(library failed. Please have a look at config.log)
			AC_MSG_RESULT((the last few lines) to find out what happened.)
			AC_MSG_RESULT(Perhaps you specified the wrong library or the)
			AC_MSG_RESULT(X11 libraries are in conflict with any other library.)
			AC_MSG_RESULT(You might also want to check your LD_LIBRARY_PATH.)
			AC_MSG_RESULT(Support for visualization is disabled.)
			AC_MSG_RESULT()
			USE_BALLVIEW=false
		else
			QT_VERSION_STRING=`cat qt.version`
			AC_MSG_RESULT(${QT_VERSION_STRING})

			dnl
			dnl  test whether this version is the right one
			dnl  (2.x.y and at least 2.0.2
			dnl
			${RM} qt.version 2>/dev/null
			QT_MAJOR=`echo ${QT_VERSION_STRING} | ${CUT} -d. -f1`
			if test "${QT_MAJOR}" -lt 2 ; then
				AC_MSG_RESULT()
				AC_MSG_RESULT(QT version 2.x is required.)
				AC_MSG_RESULT(Please install version QT Version 2 (at least 2.0.2))
				AC_MSG_RESULT(which can be obtained from)
				AC_MSG_RESULT()
				AC_MSG_RESULT(  www.troll.no/qt)
				AC_MSG_RESULT()
				AC_MSG_RESULT(Support for visualization is disabled.)
				AC_MSG_RESULT()
				USE_BALLVIEW=false
			else
				dnl
				dnl  give a hint if somebody is using 2.00 which
				dnl  has a serious bug in QListView: multiple selection
        dnl  does not work as expected!
				dnl
				if test "${QT_VERSION_STRING}" = "2.00" ; then
					AC_MSG_RESULT()
					AC_MSG_RESULT(QT verison 2.00 is known to contain a bug)
					AC_MSG_RESULT(that affects some features of MOLVIEW.)
					AC_MSG_RESULT(Please install at least version 2.0.2)
					AC_MSG_RESULT(which can be obtained from)
					AC_MSG_RESULT()
					AC_MSG_RESULT(  www.troll.no/qt)
					AC_MSG_RESULT()
					AC_MSG_RESULT()
				fi
			fi
		fi
	fi
fi

dnl
dnl	try to find the MOC (QT meta object compiler)
dnl It is usually installed in ${QTDIR}/bin/moc
dnl
if test "${USE_BALLVIEW}" = true ; then
	if test "${MOC}" = moc ; then
		if test "${QTDIR}" != "" ; then
			MOC=${QTDIR}/bin/moc
		fi
	fi

	dnl
	dnl  try to find that damned moc
	dnl
	AC_PATH_PROG(MOC,moc,moc)
	if test "${MOC}" = moc ; then
		AC_MSG_RESULT()
		AC_MSG_RESULT([Could not find the QT Meta Object Compiler (moc)!])
		AC_MSG_RESULT([You might run into trouble if you want to compile MolVIEW.])
		AC_MSG_RESULT([Please include the correct path to moc into your])
		AC_MSG_RESULT([PATH environment variable or specify the path to moc])
		AC_MSG_RESULT([using the option --with-moc=PATH to rerun configure.])
		AC_MSG_RESULT()
	fi
fi

if test "${USE_BALLVIEW}" = "true" ; then
	LIBBALLVIEW="libVIEW.a libMOLVIEW.a"
	BALLVIEW="VIEW MOLVIEW"
else
	BALLVIEW=
fi

dnl
dnl		Python extension support
dnl

dnl
dnl  variable subsitutions for config.mak
dnl
AC_SUBST(SIP)
AC_SUBST(SIP_LIB)
AC_SUBST(SIP_INCLUDES)
AC_SUBST(PYTHON_INCLUDES)
AC_SUBST(PYTHON_SUPPORT)
AC_SUBST(PYTHON_LIBS)

if test "${PYTHON_SUPPORT}" = true ; then
	dnl
	dnl Python support won't work without BALLVIEW!
  dnl (at least for the moment...)
	dnl
	if test "${USE_BALLVIEW}" = false ; then
		AC_MSG_RESULT()
		AC_MSG_RESULT(BALL Python support requires the visualization component)
		AC_MSG_RESULT(BALLVIEW. Please reconfigure without --without-BALLVIEW.)
		AC_MSG_RESULT()
		AC_MSG_ERROR(Aborted)
	fi
	AC_DEFINE(BALL_PYTHON_SUPPORT)

	dnl 
	dnl Find the python executable (specified via --with-python)
	dnl 
 
	dnl
	dnl  If a complete path is specified, assume it is correct.
  dnl  Otherwise, search in the current PATH for a suitable executable.
	dnl
	if test "`basename ${PYTHON_EXE}`" = "${PYTHON_EXE}" ; then 
		AC_PATH_PROG(PYTHON_EXECUTABLE, ${PYTHON_EXE})
	else
		PYTHON_EXECUTABLE="${PYTHON_EXE}"
	fi
 
	if test "${PYTHON_EXECUTABLE}" = "" ; then
		AC_MSG_RESULT()
		AC_MSG_RESULT([Could not find Python interpreter ]${PYTHON_EXCUTABLE})
		AC_MSG_RESULT([Please use --with-python=EXE to specify its location.])
    AC_MSG_ERROR(Aborted)
	fi

	dnl
	dnl	 Run python to retrieve some useful configuration information
	dnl	
	AC_MSG_CHECKING(for Python version)
 	PYTHON_VERSION=`${PYTHON_EXECUTABLE} -c 'import sys;print sys.version' | ${SED} -n 1p | ${CUT} -d\  -f1`
	AC_MSG_RESULT(${PYTHON_VERSION})
	PYTHON_VERSION_NUMBER_1=`echo ${PYTHON_VERSION} | ${CUT} -d. -f1`
	PYTHON_VERSION_NUMBER_2=`echo ${PYTHON_VERSION} | ${CUT} -d. -f2`
	PYTHON_VERSION_NUMBER_3=`echo ${PYTHON_VERSION} | ${CUT} -d. -f3`
	dnl
	dnl	shorten the release number to Major.minor (only those are used to construct
  dnl include and lib paths)
  dnl
  PYTHON_VERSION="${PYTHON_VERSION_NUMBER_1}.${PYTHON_VERSION_NUMBER_2}"
	
	dnl
	dnl  We need at least Python 2.0
	dnl
	if test "${PYTHON_VERSION_NUMBER_1}" -le 1 ; then
		AC_MSG_RESULT()
		AC_MSG_RESULT([Python verison 2.0 or above required!])
		AC_MSG_RESULT([Please donwload and install Python from])
		AC_MSG_RESULT([  http://www.python.org])
		AC_MSG_ERROR(Aborted)
	fi
	
	AC_MSG_CHECKING(for Python installation paths)
	PYTHON_PREFIX=`${PYTHON_EXECUTABLE} -c 'import sys;print sys.prefix'`
	AC_MSG_RESULT(${PYTHON_PREFIX})


	dnl
	dnl  Python include path
	dnl
	AC_MSG_CHECKING(for Python.h)
	if test "${PYTHON_INC_PATH}" = "" ; then
		PYTHON_INC_PATH="${PYTHON_PREFIX}/include/python${PYTHON_VERSION}"
	fi
	CF_FIND_HEADER(PYTHON_INC_PATH, Python.h, ${PYTHON_INCLUDE_PATH})
	if test "${PYTHON_INC_PATH}" = "" ; then
		AC_MSG_RESULT(not found!)
		AC_MSG_RESULT()
		AC_MSG_RESULT([Please specify the path to the directory that contains])
		AC_MSG_RESULT([Python.h using the option --with-python-incl=DIR])
		AC_MSG_RESULT([or ensure that Python is installed in the correct directory])
		AC_MSG_RESULT([(sys.prefix is ${PYTHON_PREFIX})])
		AC_MSG_ERROR(Aborted.)
	else
		AC_MSG_RESULT(${PYTHON_INC_PATH})
		PYTHON_INCLUDES="-I${PYTHON_INC_PATH}"
	fi

	dnl
	dnl	Python library path
	dnl
	AC_MSG_CHECKING(for libpython)
	if test "${PYTHON_LIBPATH}" = "" ; then
		PYTHON_LIBPATH="${PYTHON_PREFIX}/lib/python${PYTHON_VERSION}/config/"
	fi
	PYTHON_LIBS=`${FIND} ${PYTHON_LIBPATH} -name libpython*.a 2>/dev/null`
	if test "${PYTHON_LIBS}" = "" ; then
		AC_MSG_RESULT()
		AC_MSG_RESULT(No libpython*a found in ${PYTHON_LIBPATH}. Please specify)
		AC_MSG_RESULT(the path where your Python library resides using --with-python-libs=DIR)
		AC_MSG_RESULT(or ensure that libpython is installed in the correct directory)
		AC_MSG_RESULT([(sys.prefix is ]${PYTHON_PREFIX}[)])
		AC_MSG_RESULT()
		AC_MSG_ERROR(Aborted.)
	fi
	AC_MSG_RESULT(${PYTHON_LIBS})

	if test "${PYTHON_LDOPTS}" = "" ; then
		PYTHON_MAKEFILE=`${FIND} ${PYTHON_LIBPATH} -name Makefile 2>/dev/null`
		if test "${PYTHON_MAKEFILE}" = "" ; then
			AC_MSG_RESULT()
			AC_MSG_RESULT(Makefile in the Python lib/config directory not found!)
			AC_MSG_RESULT(Please specify the correct options needed to link)
			AC_MSG_RESULT(against the Python library using)
			AC_MSG_RESULT( --with-python-ldopts=OPTIONS)
			AC_MSG_RESULT([(e.g. --with-python-ldopts="-ltermcap -lm")])
			AC_MSG_RESULT()
			AC_MSG_ERROR(Aborted.)
		fi
		PYTHON_LIBS="${PYTHON_LIBS} `${GREP} \^LIBS= ${PYTHON_MAKEFILE} | ${CUT} -d=  -f2-`"
		PYTHON_LIBS="${PYTHON_LIBS} `${GREP} \^BASEMODLIBS= ${PYTHON_MAKEFILE} | ${CUT} -d=  -f2-`"
		PYTHON_LIBS="${PYTHON_LIBS} `${GREP} \^LOCALMODLIBS= ${PYTHON_MAKEFILE} | ${CUT} -d=  -f2-` -lm"
		PYTHON_LIBS=`echo ${PYTHON_LIBS} | ${TR} -s " "`
	fi
	AC_MSG_RESULT(Linker options for Python library: ${PYTHON_LIBS})
		
	dnl
	dnl	 SIP
	dnl
	AC_MSG_CHECKING(sip executable)
	AC_MSG_RESULT(${SIP})

	dnl
	dnl libsip.a 
	dnl
	AC_MSG_CHECKING(libsip.a)
	if test -r "${SIP_LIBPATH}/libsip.a" ; then
		SIP_LIB=${SIP_LIBPATH}/libsip.a
		AC_MSG_RESULT(${SIP_LIB})
	else
		AC_MSG_RESULT(not found in ${SIP_LIBPATH})
		AC_MSG_RESULT()
		AC_MSG_RESULT(Please specify the path to the directory that contains)
		AC_MSG_RESULT(libsip.a using the option --with-sip-lib=DIR.)
		AC_MSG_RESULT([If you do not have that file, you should obtain SIP])
		AC_MSG_RESULT(from)
		AC_MSG_RESULT(  www.thekompany.com/projects/pykde)
		AC_MSG_ERROR(Aborted.)
	fi
	
	dnl
	dnl	sip.h
	dnl
	AC_MSG_CHECKING(sip headers (sip.h))
	CF_FIND_HEADER(SIP_INC_PATH, sip.h, ${SIP_INCLUDE_PATH})
	if test "${SIP_INC_PATH}" = "" ; then
		AC_MSG_RESULT(not found!)
		AC_MSG_RESULT()
		AC_MSG_RESULT(Please specify the path to the directory that contains)
		AC_MSG_RESULT(sip.h using the option --with-sip-incl=DIR.)
		AC_MSG_RESULT([If you do not have that file, you should obtain SIP])
		AC_MSG_RESULT(from)
		AC_MSG_RESULT(  www.thekompany.com/projects/pykde)
		AC_MSG_ERROR(Aborted.)
	else
		AC_MSG_RESULT(${SIP_INC_PATH})
		SIP_INCLUDES="-I${SIP_INC_PATH}"
	fi
fi

if test "${PYTHON_SUPPORT}" = true ; then
	PYTHON=PYTHON
else
	PYTHON=
fi


dnl
dnl		include the default BALL include path
dnl
BALL_INCLUDES="-I$BALL_PATH/include ${BALL_INCLUDES}"

dnl
dnl		expand BINFMT to include the compiler name
dnl
BINFMT="${BINFMT}-${CXX_NAME}"


dnl		MULTI-BUILD MODE ONLY:
dnl 	add the binary format to the list of supported binary formats
dnl 	held in config/binary_formats. Avoid double entries
dnl
if test "${MULTI_BUILD}" = "true" ; then
	touch ${BINFORMAT_FILE}
	if test "`${GREP} \^${BINFMT}\\$ ${BINFORMAT_FILE}`" = "" ; then
		echo ${BINFMT} >> ${BINFORMAT_FILE}
	fi
fi

dnl
dnl   create the global config.h (the one including the platform specific
dnl   config.h.${BINFMT})
dnl
if test "${MULTI_BUILD}" = "true" ; then
	${CAT} config/config.h.header | ${SED} 1,2d > config.h

	dnl
	dnl add an error line to catch all compilations without -DBMFT=
	dnl (this is usually a problem with a missing "include config.mak" in the makefile)
	dnl
	echo "#ifndef BFMT" >> config.h
	echo "# error BALL was configured in MULTI BUILD mode! Please specify -DBMFT=<BINFMT_INDEX> or rerun configure" >> config.h
	echo "#endif" >> config.h
	echo "" >> config.h

	LINES=`cat config/binary_formats | wc -l`
	i=1
	while test $i -le $LINES ; do
		BFMT=`cat ${BINFORMAT_FILE} | ${SED} -n ${i}p`
		echo "#if ( BFMT == $i )" >> config.h
		echo "#	include <BALL/CONFIG/config.h.${BFMT}>" >> config.h
		echo "#endif" >> config.h
		echo " " >> config.h
		i=`expr $i + 1`
	done
	${CAT} config/config.h.footer | ${SED} 1,2d >> config.h
	${MKDIR} ${BALL_PATH}/include/BALL/CONFIG 2>/dev/null
	if test -f ${BALL_PATH}/include/BALL/CONFIG/config.h ; then
		if test "`${DIFF} ${BALL_PATH}/include/BALL/CONFIG/config.h config.h`" != "" ; then
			${RM} ${BALL_PATH}/include/BALL/CONFIG/config.h
			${MV} config.h  ${BALL_PATH}/include/BALL/CONFIG/config.h
		else 
			${RM} config.h
		fi
	else
		${MV} config.h  ${BALL_PATH}/include/BALL/CONFIG/config.h
	fi

	BINFMT_INDEX="-DBFMT="`${GREP} -n ${BINFMT} ${BINFORMAT_FILE} | ${CUT} -d: -f1 | ${TAIL} -1`
else
	BINFMT_INDEX=""
fi

LIBS="${LIBS} -lm"
BALLVIEW_LIBS="${BALLVIEW_LIBS} ${QT_LIBOPTS} ${OPENGL_LIBOPTS} ${X11_LIBOPTS} ${LIBS}"

BALL_LIBS="${BALL_LIBS} -L${BALL_PATH}/lib/${BINFMT} -lBALL ${LIBS}"
if test "${USE_BALLVIEW}" = true ; then
	BALLVIEW_INCLUDES="${BALL_INCLUDES} ${BALLVIEW_INCLUDES}"
	BALLVIEW_LIBS="-L${BALL_PATH}/lib/${BINFMT} -lMOLVIEW -lVIEW -lBALL ${BALLVIEW_LIBS}"
fi

LDFLAGS="$LDFLAGS"
LIBS="$BALL_LIBS"


dnl
dnl   joining all flags
dnl
CXXFLAGS="$CXXFLAGS $DEFINES"

dnl
dnl  we try to remove dependencies from non-BALL files
dnl  to speed up compilation
dnl  This works only for compilers that create one-line dependencies
dnl  (most compilers except for g++). We simply "grep" away all foreign stuff.
dnl  We can only insert ou egrep filter, if the compiler needs
dnl  a redirection of the output (MAKEDEP_CXX_SUFFIX != "")
dnl
dnl  btw: escaping is FUN! the expression below uses \ to escape the damned shell,
dnl  [] to escape m4, and finally $$ to escape make - the correct expression
dnl  is something like "(${BALL_PATH}|^[^/]*$)". Send me an email if got that immediately...
dnl
if test "${HAS_GPLUSPLUS}" = false -a "${MAKEDEP_CXX_SUFFIX}" != "" ; then
	MAKEDEP_CXX_SUFFIX="|egrep \"(${BALL_PATH}|^[[^/]]*\$\$)\" ${MAKEDEP_CXX_SUFFIX}"
fi



dnl  
dnl   neccessary variable substitutions in *.mak
dnl


dnl         PATHS

AC_SUBST(BALL_PATH)         dnl path to top level directory
AC_SUBST(BALL_INCLUDES)     dnl includes 


dnl					DEPENDENCIES

MAKEDEP_CXX_OPTS="${MAKEDEP_CXX_OPTS} ${CXXFLAGS}"
if test ${CXX_MAKEDEPEND} = "makedepend" ; then
	MAKEDEP_CXX_OPTS="${MAKEDEP_CXX_OPTS} -- "
fi
	
AC_SUBST(CXX_MAKEDEPEND)		dnl path to C++ makedepend
AC_SUBST(MAKEDEP_CXX_OPTS)	dnl options to C++ makedepend
AC_SUBST(MAKEDEP_CXX_SUFFIX)dnl optional suffix (for redirection) for C++ makedepend
AC_SUBST(DEPENDENCY_GREPS)  dnl command line to remove non-BALL headers from the dependencies


dnl         COMPILER

AC_SUBST(CXX)               dnl C++ compiler (absolute path)
AC_SUBST(CXXFLAGS)          dnl C++ compiler flags
AC_SUBST(ADD_CXXFLAGS)      dnl additional C++ compiler falgs (not passed on to makedepend)
AC_SUBST(LIB_CXXFLAGS)			dnl C++ compiler flags used during compilation of the library only
AC_SUBST(NONLIB_CXXFLAGS)		dnl C++ compiler flags used during compilation of applications, tests, examples only
AC_SUBST(HAS_GPLUSPLUS)     dnl true, if C++ compiler is g++
AC_SUBST(STD_CPP_INCLUDES)  dnl standard include paths of C++ compiler
AC_SUBST(MAKEDEP_OPTS)      dnl options for makedep (needed for Linux)
AC_SUBST(BALL_TYPENAME)			dnl if the compiler requires the use of typename
                            dnl this variable is set to "typename". "" otherwise.
AC_SUBST(BINFMT_INDEX)      dnl BFMT: binformat index of the current BINFMT - passed to the compiler
                            dnl   as -DBFMT=${BINFMT_INDEX} to decide which config.h to use
                            dnl   used in multi-platform build only!

dnl         LINKER

AC_SUBST(BALLVIEW_LIBS)     dnl BALLVIEW libs
AC_SUBST(OPENGL_LIBOPTS)    dnl compiler options for OpenGL libs (-Lxxx -lGLU -lGL)
AC_SUBST(OPENGL_LIBPATH)    dnl path to OpenGL libs (without -L)
AC_SUBST(QT_LIBOPTS)        dnl copiler options for QT libs (-Lxxx -lqgl -lqt)
AC_SUBST(QT_LIBPATH)        dnl path to QT libs (without -L)
AC_SUBST(MOC)        				dnl path to QT meta object compiler
AC_SUBST(X11_LIBOPTS)       dnl compiler options for X11 libs (-Lxx -lXmu -lXt -lX11 [-lSM -lICE])
AC_SUBST(X11_LIBPATH)       dnl path to X11 libs (without -L)
AC_SUBST(X11_LIBS)          dnl the X11 libraries only (-lXmu -lXt...)
AC_SUBST(BALL_LIBS)         dnl libraries
AC_SUBST(LDFLAGS)           dnl linker flags
AC_SUBST(LIBS)              dnl linker libraries
AC_SUBST(DEFINES)           dnl defines (part of LDFLAGS)
AC_SUBST(CXXFLAGS_D)        dnl debug flags for compiler 
AC_SUBST(CXXFLAGS_O)        dnl optimization flags for compiler 
AC_SUBST(CPP_MODE_FLAGS)    dnl set to CXXFLAGS_D|CXXFLAGS_O, depending on BALL_DEBUG
AC_SUBST(CPP_MODE_FLAGS_NO_OPTIMIZATION)    
														dnl set to CXXFLAGS_D|"", depending on BALL_DEBUG





dnl					ARCHIVER

dnl if nothing has been defined for AR/AROPTS,
dnl use default

if test "${AR}" = "no" ; then
	AR="ar"
fi
if test "${AROPTS}" = "" ; then
	AROPTS="cru"
fi

AC_SUBST(AR)
AC_SUBST(AROPTS)
AC_SUBST(DYNAR)
AC_SUBST(DYNAROPTS)


dnl				  MISC

dnl   ADDTIONAL_COMMENTS is substituted at the end of the
dnl   information text in Makefile and will be printed after
dnl 	the library has been built.
dnl   Any hints concerning a special OS/compiler combination 
dnl   might want to print some information here (as is the
dnl   case for the IRIX LD_LBRARY_PATH/LD_LIBRARY64_PATH/LD_LIBRARYN32_PATH)
dnl   If BALLVIEW is to be built, we add a hint for MolVIEW.
if test "${ADDITIONAL_COMMENTS}" = "" ; then
	ADDITIONAL_COMMENTS=""
fi
AC_SUBST(ADDITIONAL_COMMENTS)

dnl	    BALLVIEW

AC_SUBST(USE_BALLVIEW)				dnl bool: use ballview (default=true)
AC_SUBST(BALLVIEW_PLATFORM)		dnl Mesa or OpenGL
AC_SUBST(BALLVIEW_INCLUDES)		dnl BALLVIEW includes
AC_SUBST(LIBBALLVIEW)					dnl static library name


dnl    	define the standard installation path
dnl     and the binary format string
AC_DEFINE_UNQUOTED(BALL_PATH, "${BALL_PATH}")
AC_DEFINE_UNQUOTED(BALL_BINFMT, "${BINFMT}")

dnl			define BALL_NULL_TEMPLATE_ARGS
AC_DEFINE_UNQUOTED(BALL_NULL_TEMPLATE_ARGS, ${BALL_NULL_TEMPLATE_ARGS})

dnl			define BALL_HAS_ANSI_IOSTREAM
if test "$BALL_HAS_ANSI_IOSTREAM" = yes ; then
	AC_DEFINE(BALL_HAS_ANSI_IOSTREAM)
fi

dnl	
dnl		 make target "clean" should also remove 
dnl    template repositories. The name of this directory 
dnl    depends on the compiler and ist set in TEMPLATE_DIR
dnl
AC_SUBST(TEMPLATE_DIR)

dnl
dnl    create files
dnl

SUBDIRS="COMMON CONCEPT DATATYPE ENERGY FORMAT KERNEL MATHS MOLMEC NMR QSAR SOLVATION STRUCTURE SYSTEM ${BALLVIEW} ${PYTHON}"
AC_SUBST(SUBDIRS)

if test "${MULTI_BUILD}" = "true" ; then
	AC_MSG_RESULT(creating shadow directories...)
	config/shadowsource.sh `pwd`"/${BINFMT}" `pwd` "${SUBDIRS} TEST BENCHMARKS EXAMPLES TUTORIAL APPLICATIONS"
	${RM} -fr `pwd`/${BINFMT}/TEST/data 2>/dev/null
	${RM} -fr `pwd`/${BINFMT}/BENCHMARKS/data 2>/dev/null
	${LN} -s `pwd`/TEST/data `pwd`/${BINFMT}/TEST 2>/dev/null
	${LN} -s `pwd`/TEST/runtests `pwd`/${BINFMT}/TEST 2>/dev/null
	${LN} -s `pwd`/BENCHMARKS/data `pwd`/${BINFMT}/BENCHMARKS 2>/dev/null
	${LN} -s `pwd`/BENCHMARKS/runbenchmarks `pwd`/${BINFMT}/BENCHMARKS 2>/dev/null

	${CP} config/Makefile.multiplatform Makefile

	dnl   define the string to substitute in common.mak
	BINFMT_PATH="/${BINFMT}"
else
	BINFMT_PATH=""
fi

AC_OUTPUT([
	Makefile.tmp:config/Makefile.in
	config.mak.tmp:config/config.mak.in 
	common.mak.tmp:config/common.mak.in 
])

if test "${MULTI_BUILD}" = "true" ; then
	${MV} Makefile.tmp ${BINFMT}/Makefile
	${MV} common.mak.tmp ${BINFMT}/common.mak
	${MV} config.mak.tmp ${BINFMT}/config.mak
else 
	${MV} Makefile.tmp Makefile
	${MV} common.mak.tmp common.mak
	${MV} config.mak.tmp config.mak
fi

dnl
dnl    copy config.h to the includes directory
dnl    remove obsolete dependency file
dnl

if test "${MULTI_BUILD}" = "true" ; then
	mkdir ${BALL_PATH}/include/BALL/CONFIG 2>/dev/null
	${MV} -f config.h $BALL_PATH/include/BALL/CONFIG/config.h.${BINFMT}
else
	dnl	
	dnl move that damned file only if it differs from the previous
	dnl version. Otherwise we have to rebuild _everything_ after each configure
	dnl
	if test -f $BALL_PATH/include/BALL/CONFIG/config.h ; then
		if test "`${DIFF} config.h $BALL_PATH/include/BALL/CONFIG/config.h`" != "" ; then
			${MV} -f config.h $BALL_PATH/include/BALL/CONFIG/config.h
		fi
	else 
		dnl
		dnl  create the directory BALL/include/CONFIG
		dnl  and move config.h to that directory
		dnl
		mkdir ${BALL_PATH}/include/BALL/CONFIG 2>/dev/null
		${MV} -f config.h $BALL_PATH/include/BALL/CONFIG/config.h
	fi
fi
	


dnl
dnl		make sure the dependencies and object lists are (re)built
dnl
if test "${MULTI_BUILD}" = "true" ; then
	${RM}  ${BINFMT}/.Dependencies 2>/dev/null
	${RM}  ${BINFMT}/lib*.objects 2>/dev/null
else
	${RM}  .Dependencies 2>/dev/null
	${RM}  lib*.objects 2>/dev/null
fi

dnl
dnl Just give another helpful message and exit
dnl

AC_MSG_RESULT()
AC_MSG_RESULT()
AC_MSG_RESULT(Your Makefiles have been created. If everything went OK: just type)
AC_MSG_RESULT()
AC_MSG_RESULT(		make)
AC_MSG_RESULT()
AC_MSG_RESULT(to build BALL.)
AC_MSG_RESULT()
