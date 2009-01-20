dnl -*- Mode: C++; tab-width: 2; -*-
dnl vi: set ts=2:
dnl
dnl $Id: configure.ac,v 1.39.6.3 2007/05/10 20:05:02 oliver Exp $
dnl
dnl Author:
dnl   Oliver Kohlbacher
dnl   Andreas Hildebrandt
dnl   Marc Sturm
dnl

dnl
dnl    Process this file with autoconf to produce a configure script.
dnl   

dnl 
dnl    Some initialization stuff...
dnl 
AC_INIT([BALL],[1.3.2],[ball-bugs@bioinf.uni-sb.de])
AC_CONFIG_MACRO_DIR([config/m4])
AC_CONFIG_HEADER([
	config.h:config/config.h.in
])
AC_LANG(C++)
PROJECT=BALL
define(PROJECT, BALL)
dnl this project define is required by (portable) aclocal.m4 file
dnl being used for BN++, BALL, and OpenMS

dnl
dnl retrieve the BALL version and print it to the console
dnl for debugging purposes (people sending in config.log and the like).
dnl
BALL_VERSION_STRING=`grep define\ BALL_RELEASE_STRING ../include/BALL/COMMON/version.h | cut -d\" -f2 `
BALL_CURRENT_VERSION=`echo ${BALL_VERSION_STRING}| tr -d "a-zA-Z"`
BALL_COMPATIBILITY_VERSION=`echo ${BALL_CURRENT_VERSION} | cut -d. -f1-2`
CF_VERSION_CHECK

dnl   Require the user to accept the license (once)
CF_CHECK_LICENSE

dnl		
dnl    define command line arguments for QT, Mesa, and OpenGL
dnl    includes and libraries
dnl
AC_ARG_ENABLE(optimization,    [  --disable-optimization  Do not optimize the library for speed. Might be required for debugging.])
AC_ARG_ENABLE(debuginfo,       [  --enable-debuginfo      add -g to the compiler flags (to create debug information)])
AC_ARG_ENABLE(VIEW,            [  --disable-VIEW          disable the compilation of the visualization component])
AC_ARG_ENABLE(64,              [  --enable-64             create 64 bit binaries (if allowed by the compiler)])
AC_ARG_ENABLE(libtool,         [  --enable-libtool        use libtool for compiling and linking the libraries.])
AC_ARG_ENABLE(libdir-detect,   [  --enable-libdir-detect  try to decide automatically whether to install into lib64 or lib])
AC_ARG_WITH(libtool-rpath,     [  --with-libtool-rpath    use libtool-rpath instead of the standard (i.e. --prefix)])
AC_ARG_WITH(compiler,          [  --with-compiler=CXX     use CXX as C++ compiler])
AC_ARG_WITH(cxxflags,          [  --with-cxxflags=FLAGS   add FLAGS to the C++ compiler flags (commas are converted to blanks)])
AC_ARG_WITH(ldflags,           [  --with-ldflags=FLAGS    add FLAGS to the linker flags (commas are converted to blanks)])
AC_ARG_WITH(arflags,           [  --with-arflags=FLAGS    add FLAGS to the flags for the creation of the static libraries])
AC_ARG_WITH(dynarflags,        [  --with-dynarflags=FLAGS add FLAGS to the flags for the creation of the shared libraries])
AC_ARG_WITH(libxnet,           [  --without-libxnet       use -lsocket -lnsl rather than -lxnet (under Solaris)])
AC_ARG_WITH(xdr,               [  --without-xdr           no RPC/XDR headers available - do not build portable binary persistence support])
AC_ARG_WITH(ball-path,         [  --with-ball-path=PATH   use PATH as the compiled-in BALL_PATH (required, e.g., for finding the data directory])

dnl  ** QT/X11 **
AC_ARG_WITH(qt,                [  --with-qt=QTDIR         QT is installed in QTDIR. Prefer this over the following options.])
AC_ARG_WITH(qt-incl,           [  --with-qt-incl=DIR      QT header files are in DIR])
AC_ARG_WITH(qt-libs,           [  --with-qt-libs=DIR      QT libraries are in DIR])
AC_ARG_WITH(qt-mt,             [  --with-qt-mt            use a threadsafe version of the QT lib (libqt-mt)])
AC_ARG_WITH(moc,               [  --with-moc=MOC          use MOC as the absolute path to the QT meta object compiler (moc)])
AC_ARG_WITH(uic,               [  --with-uic=UIC          use UIC as the absolute path to the QT user interface compiler (uic)])
AC_ARG_WITH(rcc,               [  --with-rcc=RCC          use RCC as the absolute path to the QT resource compiler (rcc)])
AC_ARG_WITH(opengl-incl,       [  --with-opengl-incl=DIR  OpenGL/Mesa header files are in DIR/GL])
AC_ARG_WITH(opengl-libs,       [  --with-opengl-libs=DIR  OpenGL/Mesa libraries are in DIR/GL])
AC_ARG_WITH(mesa,              [  --with-mesa             use MESA instead of OpenGL])

dnl  ** GLEW **
AC_ARG_ENABLE(glew,						 [  --disable-glew          disable check for GLEW library (default:enabled)])
AC_ARG_WITH(glew-incl,         [  --with-glew-incl=DIR    GLEW header files are in DIR])
AC_ARG_WITH(glew-libs,         [  --with-glew-libs=DIR    GLEW libraries are in DIR])

AC_ARG_WITH(glew-incl,         [  --with-glew-incl=DIR    GLEW header files are in DIR])
AC_ARG_WITH(glew-libs,         [  --with-glew-libs=DIR    GLEW libraries are in DIR])

dnl ** Python support **
AC_ARG_ENABLE(python,          [  --enable-python         enable Python support and search for executable])
AC_ARG_WITH(python,            [  --with-python=EXE       enable Python support and use Python executable in EXE])
AC_ARG_WITH(python-incl,       [  --with-python-incl=DIR  Python includes (Python.h) is in DIR])
AC_ARG_WITH(python-libs,       [  --with-python-libs=DIR  Python library (libpython*.a) is in DIR])
AC_ARG_WITH(python-ldopts,     [  --with-python-ldopts=X  Use additional options X when linking with the Python library])
AC_ARG_WITH(sip,   	           [  --with-sip=DIR          the SIP executable resides in DIR])
AC_ARG_WITH(sip-lib,   	       [  --with-sip-lib=DIR      the SIP library resides in DIR])
AC_ARG_WITH(sip-incl,   	     [  --with-sip-incl=DIR     the SIP header file resides in DIR])

dnl ** FFTW ** 
AC_ARG_ENABLE(fftw,            [  --enable-fftw           enable the build of BALL with support for FFTW (a FFT library)])
AC_ARG_WITH(fftw-lib,          [  --with-fftw-lib=DIR     libfftw is in DIR])
AC_ARG_WITH(fftw-incl,         [  --with-fftw-incl=DIR    fftw includes are in DIR])
AC_ARG_ENABLE(fftw-float,      [  --disable-fftw-float    disable support for float precision in FFT code])
AC_ARG_ENABLE(fftw-double,     [  --disable-fftw-double   disable support for double precision in FFT code])
AC_ARG_ENABLE(fftw-longdbl,    [  --disable-fftw-longdbl  disable support for long double precision in FFT code])
AC_ARG_ENABLE(double-cplx,     [  --enable-double-cplx    use double precision floats for Complex type])
AC_ARG_ENABLE(longdbl-cplx,    [  --enable-longdbl-cplx   use long double precision floats for Complex type])

dnl ** GSL **
#AC_ARG_ENABLE(gsl,             [  --enable-gsl            enable the build of BALL with support for GSL (GNU Scientific Library)])
AC_ARG_WITH(gsl-lib,           [  --with-gsl-lib=DIR      libgsl is in DIR])
AC_ARG_WITH(gsl-incl,          [  --with-gsl-incl=DIR     gsl includes are in DIR])

dnl ** LPSOLVE **
AC_ARG_ENABLE(lpsolve,				 [  --enable-lpsolve        enable the build of BALL with support for lpsolve55)])
AC_ARG_WITH(lpsolve-lib,			 [  --with-lpsolve-lib=DIR  liblpsolve55 is in DIR])
AC_ARG_WITH(lpsolve-incl,			 [  --with-lpsolve-incl=DIR lpsolve includes are in DIR])

dnl ** Stuff for the plugins **
AC_ARG_ENABLE(spacenavigator,    [  --enable-spacenavigator    enable the build of the SpaceNav plugin for Space Navigator input devices])
AC_ARG_WITH(spacenavigator-lib,  [  --with-spacenavigator-lib  the directory in which libspnav can be found])
AC_ARG_WITH(spacenavigator-incl, [  --with-spacenavigator-incl the directory in which the headers of libspnav can be found])

dnl
dnl    interpret switches...
dnl
if test "x$prefix" = xNONE ; then
	PREFIX=${ac_default_prefix}
else
	PREFIX=${prefix}
fi
AC_SUBST(PREFIX)

if test "$enable_optimization" = "no" ; then
	DEBUG=true
else
	DEBUG=false
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

if test "$enable_libtool" = "yes" ; then
	USE_LIBTOOL=true
	INSTALL_TARGET=install.libtool
else
	USE_LIBTOOL=false
	INSTALL_TARGET=install.shared
fi
AC_SUBST(INSTALL_TARGET)

if test "$with_libtool_rpath" != "" ; then
	LIBTOOL_RPATH=$with_libtool_rpath
else
	LIBTOOL_RPATH=${PREFIX}
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

dnl we will require this later when we AC_SUBST the BALL_PATH that is compiled in into config.h
if test "$with_ball_path" != "" ; then
	FINAL_BALL_PATH="${with_ball_path}"
else
	FINAL_BALL_PATH=`cd ..; pwd`
fi

if test "$with_qt" != "" ; then
	QTDIR=$with_qt
fi
if test "$with_qt_incl" != "" ; then
	QT_INCPATH=$with_qt_incl
fi
if test "$with_qt_libs" != "" ; then
	QT_LIBPATH=$with_qt_libs
fi

if test "$with_qt_mt" = "yes" -o "${with_qt_mt+set}" != set; then
	USE_THREADSAFE_QT=true
	QT_MT_SUFFIX="-mt"
else
	USE_THREADSAFE_QT=false
	QT_MT_SUFFIX=""
fi

if test "$enable_glew" != "no"; then
	GLEW="true"
else
	GLEW="false"
fi

if test "$with_glew_incl" != "" ; then
	GLEW_INCPATH=$with_glew_incl
fi
if test "$with_glew_libs" != "" ; then
	GLEW_LIBPATH=$with_glew_libs
fi

if test "$with_opengl_incl" != "" ; then
	OPENGL_INCPATH=$with_opengl_incl
fi
if test "$with_opengl_libs" != "" ; then
	OPENGL_LIBPATH=$with_opengl_libs
fi

if test "$with_mesa" != "yes" ; then	
	VIEW_PLATFORM=OpenGL
else
	VIEW_PLATFORM=Mesa
fi

if test "$with_moc" != "" ; then	
	MOC=$with_moc
else
	MOC=moc
fi

if test "$with_uic" != "" ; then	
	UIC=$with_uic
else
	UIC=uic
fi

if test "$with_rcc" != "" ; then
	RCC=$with_rcc
else
	RCC=rcc
fi

if test "$with_libxnet" != "no" ; then	
	USE_LIBXNET=true
else
	USE_LIBXNET=false
fi

if test "$with_VIEW" = "no" -o "$enable_VIEW" = "no"; then
	USE_VIEW=false
	BALL_HAS_VIEW=""
else	
	USE_VIEW=true
	BALL_HAS_VIEW=true
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
	if test "${with_fftw_incl}" != "" ; then
		FFTW_INCL=${with_fftw_incl}
	else
		FFTW_INCL=""
	fi
	if test "$enable_fftw_float" = "no" ; then
		FFTW_DISABLE_FFTW_FLOAT=true
	else
		FFTW_DISABLE_FFTW_FLOAT=false
	fi
	if test "$enable_fftw_double" = "no" ; then
		FFTW_DISABLE_FFTW_DOUBLE=true
	else
		FFTW_DISABLE_FFTW_DOUBLE=false
	fi
	if test "$enable_fftw_longdbl" = "no" ; then
		FFTW_DISABLE_FFTW_LONGDBL=true
	else
		FFTW_DISABLE_FFTW_LONGDBL=false
	fi
else
	FFTW_SUPPORT=
fi

# support for GSL (GNU Scientific Library)
if test "${with_gsl_lib}" != "" ; then
	GSL_LIBDIR=${with_gsl_lib}
else
	GSL_LIBDIR=""
fi
if test "${with_gsl_incl}" != "" ; then
	GSL_INCDIR=${with_gsl_incl}
else
	GSL_INCDIR=""
fi

# support for lpsolve 
if test "$with_lpsolve" != "" -a "$with_lpsolve" != "no" -o "$enable_lpsolve" = yes ; then
	LPSOLVE_SUPPORT=true
	if test "${with_lpsolve_lib}" != "" ; then
		LPSOLVE_LIBDIR=${with_lpsolve_lib}
	else
		LPSOLVE_LIBDIR=""
	fi
	if test "${with_lpsolve_incl}" != "" ; then
		LPSOLVE_INCDIR=${with_lpsolve_incl}
	else
		LPSOLVE_INCDIR=""
	fi
else
	LPSOLVE_SUPPORT=
fi

if test "$USE_VIEW" == "true" -a "$enable_spacenavigator" = "yes" ; then
	SPACENAV_SUPPORT=true
	SPACENAV_LIBDIR=${with_spacenavigator_lib}
	SPACENAV_INCLDIR=${with_spacenavigator_incl}
else
	SPACENAV_SUPPORT=false
	SPACENAV_LIBDIR=""
	SPACENAV_INCLDIR=""
fi

dnl
dnl 	global definitions
dnl

dnl		the file containing the list of supported (configure`d) 
dnl   binary formats. configure will add a line with ${BINFMT} to this file
BINFORMAT_FILE=config/binary_formats
BALL_PATH=`cd ..; pwd`


if test "${PREFIX}" = "${BALL_PATH}" ; then
	INSTALL_HEADERS_TARGET=
	INSTALL_DATA_TARGET=
else
	INSTALL_HEADERS_TARGET=install.headers
	INSTALL_DATA_TARGET=install.data
fi
AC_SUBST(INSTALL_HEADERS_TARGET)
AC_SUBST(INSTALL_DATA_TARGET)

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

AC_SUBST(MV)
AC_SUBST(RM)

dnl
dnl 		Check whether the default shell (/bin/sh) understands "echo -e".
dnl
CF_CHECK_ECHO
CF_CHECK_FIND

AC_SUBST(ECHO)
dnl
dnl   	Determine OS and architecture and all this stuff.
dnl
CF_DETECT_OS


dnl
dnl			Search for the C++ compiler in the (OS-determined) search order.
CF_SEARCH_CXX

dnl
dnl		Determine the object suffix and executable suffix
dnl
SAVE_CXXFLAGS="${CXXFLAGS}"
AC_PROG_CXX
AC_OBJEXT
AC_EXEEXT
AC_SUBST(EXEEXT,$EXEEXT)
CXXFLAGS="${SAVE_CXXFLAGS}"

dnl
dnl  		Try to identify the compiler and determine its version number.
dnl
CXX_IDENTIFIED=false
if test "${CXX_IDENTIFIED}" != true ; then CF_IDENTIFY_INTEL  fi
if test "${CXX_IDENTIFIED}" != true ; then CF_IDENTIFY_GXX  fi
if test "${CXX_IDENTIFIED}" != true ; then CF_IDENTIFY_KAI  fi
if test "${CXX_IDENTIFIED}" != true ; then CF_IDENTIFY_SUN  fi
if test "${CXX_IDENTIFIED}" != true ; then CF_IDENTIFY_SGI  fi
if test "${CXX_IDENTIFIED}" != true ; then CF_IDENTIFY_COMPAQ  fi

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

CF_BUILD_FULL_CXX_NAME


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
dnl			-- inline functions as template function arguments
dnl 		-- ANSI-compliant iostreams 
dnl			-- sstream vs. strstream
dnl
CF_CHECK_NUM_LIMITS
CF_CHECK_TPL_NULL_ARGS
CF_CHECK_INLINE_TPL_ARGS
CF_CHECK_ANSI_IOSTREAM
CF_CHECK_HAS_SSTREAM

dnl
dnl		Check for the word lengths of different C/C++ types, define
dnl			the correct replacement types, and determine the endianness of 
dnl			the machine.
dnl
CF_GET_TYPE_SIZES
CF_C_BIGENDIAN

dnl Check for the presence of hash_map. This check depends on the types determined
dnl in CF_GET_TYPE_SIZES. It _must_ be called afterwards!
CF_CHECK_MAP

dnl
dnl		Checks for a couple of headers files and whether they are required.
dnl
AC_TYPE_SIZE_T
AC_HEADER_TIME
CF_CHECK_STDINT_H
CF_CHECK_IEEEFP_H
CF_CHECK_VALUES_H
CF_CHECK_LIMITS_H
CF_CHECK_REGEX_H
CF_CHECK_UNISTD_H
CF_CHECK_PROCESS_H
CF_CHECK_TIME_H
CF_CHECK_DIRENT_H
CF_CHECK_DIRECT_H
CF_CHECK_PWD_H
CF_CHECK_IO_H
CF_CHECK_SYS_TIME_H
CF_CHECK_SYS_STAT_H
CF_CHECK_SYS_TIMES_H
CF_CHECK_SYS_TYPES_H
CF_CHECK_SYS_IOCTL_H
CF_CHECK_SYS_SOCKET_H
CF_CHECK_SYS_PARAM_H
CF_CHECK_SYS_SYSINFO_H
CF_CHECK_NETINET_IN_H
CF_CHECK_NETDB_H
CF_CHECK_ARPA_INET_H

CF_CHECK_SYSCONF
CF_CHECK_KILL
CF_CHECK_HYPOT

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
if test "${FFTW_SUPPORT}" == true ; then
	CF_CHECK_FFTW_SUPPORT
fi

dnl
dnl GSL -- The GNU Scientific Library
dnl
CF_GSL

dnl Spacenavigator input plugin support
dnl
if test "${SPACENAV_SUPPORT}" == true ; then
	CF_SPACENAV
fi

dnl
dnl   boost
dnl
CF_BOOST()

dnl
dnl   asio
dnl
CF_ASIO()

dnl
dnl LPSOLVE 
dnl
if test "${LPSOLVE_SUPPORT}" == true ; then
	CF_LPSOLVE
fi

dnl Spacenavigator input plugin support
dnl
if test "${SPACENAV_SUPPORT}" == true ; then
	CF_SPACENAV
fi

dnl
dnl		VIEW
dnl
dnl Currently, we require QT >= 4.2. QT 3.x isn't supported any more.
QT_MIN_VERSION=430
QT_MAX_VERSION=499
QT_RECOMMENDED_VERSION=4.5.0
CF_VIEW

dnl
dnl		Python extension support
dnl
CF_PYTHON

dnl
dnl		valgrind support (leak cehcking for the BALL tests)
dnl
CF_VALGRIND

dnl
dnl		include the default BALL include path
dnl
BALL_INCLUDES=$(echo ${BALL_INCLUDES} | ${SED} "s_-I\s*/usr/include\(\s\|$\)_ _g")
BALL_INCLUDES="-I$BALL_PATH/include ${BALL_INCLUDES}"

dnl
dnl		expand BINFMT to include the compiler name
dnl
BINFMT="${BINFMT}-${CXX_NAME}"
BINFMT_INDEX=""

BALL_INCLUDES="${BALL_INCLUDES} ${VIEW_INCLUDES}"

LIBS="${LIBS} ${FFTW_LIBS} ${GSL_LIBS} ${LPSOLVE_LIBS} ${BOOST_LIBS} -lm"
VIEW_LIBS="${VIEW_LIBS} ${QT_VIEW_LIBOPTS} ${OPENGL_LIBOPTS} ${X11_LIBOPTS} ${LIBS}"
BALL_LIBS="${BALL_LIBS} -L${BALL_PATH}/lib -lBALL ${LIBS}"
if test "${USE_VIEW}" = true ; then
	BALL_LIBS="-L${BALL_PATH}/lib -lVIEW -lBALL ${VIEW_LIBS}"
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

OBJECT_EXTENSION=o
MAKEDEP_LIBTOOL_FIX=
USE_LIBTOOL_FOR_BALL=false
AC_PROG_LIBTOOL
if test "${USE_LIBTOOL}" = "true" ; then
	AC_FIX_LIBTOOL
	CXX="${BALL_PATH}/source/libtool --tag=CXX --mode=compile ${CXX}"
	OBJECT_EXTENSION=lo
	MAKEDEP_LIBTOOL_FIX=" | ${SED} s/.o:/.lo:/g"
	USE_LIBTOOL_FOR_BALL=true
fi
AC_SUBST(OBJECT_EXTENSION)
AC_SUBST(MAKEDEP_LIBTOOL_FIX)
AC_SUBST(USE_LIBTOOL_FOR_BALL)

AC_SUBST(CXX)               dnl C++ compiler (absolute path)
CC=CXX
AC_SUBST(CC)
AC_SUBST(CXXFLAGS)          dnl C++ compiler flags
AC_SUBST(ADD_CXXFLAGS)      dnl additional C++ compiler falgs (not passed on to makedepend)
AC_SUBST(LIB_CXXFLAGS)			dnl C++ compiler flags used during compilation of the library only
AC_SUBST(NONLIB_CXXFLAGS)		dnl C++ compiler flags used during compilation of applications, tests, examples only
AC_SUBST(HAS_GPLUSPLUS)     dnl true, if C++ compiler is g++
AC_SUBST(STD_CPP_INCLUDES)  dnl standard include paths of C++ compiler
AC_SUBST(MAKEDEP_OPTS)      dnl options for makedep (needed for Linux)
AC_SUBST(BALL_TYPENAME)			dnl if the compiler requires the use of typename
														dnl this variable is set to "typename". "" otherwise.

dnl         LINKER

AC_SUBST(OPENGL_LIBOPTS)    dnl compiler options for OpenGL libs (-Lxxx -lGLU -lGL)
AC_SUBST(OPENGL_LIBPATH)    dnl path to OpenGL libs (without -L)
AC_SUBST(QT_BALL_LIBOPTS)        dnl copiler options for QT libs (-Lxxx -lqgl -lqt)
AC_SUBST(QT_VIEW_LIBOPTS)        dnl copiler options for QT libs (-Lxxx -lqgl -lqt)
AC_SUBST(QT_LIBPATH)        dnl path to QT libs (without -L)
AC_SUBST(QT_MT_SUFFIX)      dnl Either "" or "-mt" if multithreading is enabled.
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

dnl
dnl  Some platforms (e.g. Darwin) require special flags to build
dnl  loadable modules. If no specific flags for this were set,
dnl  we just use the standard settings to create dynamic libraries.
dnl
if test "${DYNAROPTS_PYMODULE}" = "" ; then
  DYNAROPTS_PYMODULE="${DYNAROPTS}"
fi

dnl
dnl  Some platforms (e.g. Darwin) require special flags to build
dnl  loadable modules. If no specific flags for this were set,
dnl  we just use the standard settings to create dynamic libraries.
dnl
if test "${DYNAROPTS_PYMODULE}" = "" ; then
  DYNAROPTS_PYMODULE="${DYNAROPTS}"
fi

dnl
dnl Try to determine into which libdir we need to install
dnl
LIBDIR=lib

if test "$enable_libdir_detect" = "yes" ; then
	if test ${ARCHITECTURE} = x86_64; then
		LIBDIR=lib64
	fi
fi

dnl
dnl We should switch to version-info sooner or later, but for now, we really
dnl cannot guarantee binary compatibility between releases, so lets rather err
dnl on the safe side...
dnl
if test "${USE_LIBTOOL}" = "true" ; then
	LINKER="${BALL_PATH}/source/libtool --tag=CXX --mode=link ${DYNAR}"
	DYNAR="${BALL_PATH}/source/libtool --tag=CXX --mode=link ${DYNAR} --shared -export-dynamic -release ${BALL_VERSION_STRING} -rpath ${LIBTOOL_RPATH}/${LIBDIR}"
	SHARED_LIB_SUFFIX=la
	AC_SUBST(SHARED_LIB_SUFFIX)
else
	LINKER="${DYNAR}"
fi

AC_SUBST(AR)
AC_SUBST(AROPTS)
AC_SUBST(DYNAR)
AC_SUBST(DYNAROPTS)
AC_SUBST(DYNAROPTS_PYMODULE)
AC_SUBST(ADD_DYNAROPTS_LIBBALL)
AC_SUBST(ADD_DYNAROPTS_LIBVIEW)
AC_SUBST(RANLIB)
AC_SUBST(LINKER)
AC_SUBST(LIBDIR)

dnl				  MISC

dnl   ADDTIONAL_COMMENTS is substituted at the end of the
dnl   information text in Makefile and will be printed after
dnl 	the library has been built.
dnl   Any hints concerning a special OS/compiler combination 
dnl   might want to print some information here (as is the
dnl   case for the IRIX LD_LBRARY_PATH/LD_LIBRARY64_PATH/LD_LIBRARYN32_PATH)
dnl   If VIEW is to be built, we add a hint for MolVIEW.
if test "${ADDITIONAL_COMMENTS}" = "" ; then
	ADDITIONAL_COMMENTS=""
fi
AC_SUBST(ADDITIONAL_COMMENTS)

dnl	    VIEW

AC_SUBST(USE_VIEW)				dnl bool: use ballview (default=true)
AC_SUBST(BALL_HAS_VIEW)		dnl true if VIEW is enabled, "" otherwise
AC_SUBST(VIEW_PLATFORM)		dnl Mesa or OpenGL
AC_SUBST(LIBVIEW)					dnl static library name


dnl    	define the standard installation path
dnl     and the binary format string
AC_DEFINE_UNQUOTED(BALL_PATH, "${FINAL_BALL_PATH}")
AC_DEFINE_UNQUOTED(BALL_BINFMT, "${BINFMT}")

dnl			define BALL_NULL_TEMPLATE_ARGS
AC_DEFINE_UNQUOTED(BALL_NULL_TEMPLATE_ARGS, ${BALL_NULL_TEMPLATE_ARGS})

dnl			define BALL_HAS_ANSI_IOSTREAM
if test "$BALL_HAS_ANSI_IOSTREAM" = yes ; then
	AC_DEFINE(BALL_HAS_ANSI_IOSTREAM)
fi

dnl			define BALL_HAS_SSTREAM
if test "$BALL_HAS_SSTREAM" = yes ; then
	AC_DEFINE(BALL_HAS_SSTREAM)
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

SUBDIRS="COMMON CONCEPT DATATYPE ENERGY FORMAT KERNEL MATHS MOLMEC NMR QSAR SCORING SOLVATION STRUCTURE SYSTEM XRAY ${VIEW} ${PYTHON}"
if test "$BALL_HAS_VIEW" = true ; then
	SUBDIRS+=" PLUGIN EXTENSIONS"
fi
AC_SUBST(SUBDIRS)

AC_PREFIX_DEFAULT(`cd ..; pwd`)

dnl
dnl 	Create output files. Move them to temporary location
dnl		only, because some of them will require additional
dnl		modification
dnl
AC_CONFIG_FILES([
 	config.mak.tmp:config/config.mak.in 
 	rules.mak.tmp:config/rules.mak.in 
 	targets.mak.tmp:config/targets.mak.in 
	common.mak.tmp:config/common.mak.in 
	Makefile.tmp:config/Makefile.in
	BALLPythonConfig.h.tmp:config/BALLPythonConfig.h.in
	BALLTestConfig.h.tmp:config/BALLTestConfig.h.in
])
AC_OUTPUT
CF_MOVE_CONFIG_FILES

dnl
dnl		Make sure the dependencies and object lists are (re)built.
dnl
CF_CLEAR_DEP_FILES

dnl
dnl Just give another helpful message and exit.
dnl

AC_MSG_RESULT()
AC_MSG_RESULT()
AC_MSG_RESULT(Your Makefiles have been created. If everything went OK: just type)
AC_MSG_RESULT()
AC_MSG_RESULT(		make)
AC_MSG_RESULT()
AC_MSG_RESULT(to build BALL.)
AC_MSG_RESULT()
