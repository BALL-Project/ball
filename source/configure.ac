dnl    $Id: configure.ac,v 1.1.2.11 2002/12/01 13:40:47 oliver Exp $
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
CF_CHECK_NETINET_IN_H
CF_CHECK_NETDB_H
CF_CHECK_ARPA_INET_H

CF_CHECK_SYSCONF
CF_CHECK_KILL

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
CF_BALLVIEW

dnl
dnl		Python extension support
dnl
CF_PYTHON

dnl
dnl		include the default BALL include path
dnl
BALL_INCLUDES="-I$BALL_PATH/include ${BALL_INCLUDES}"

dnl
dnl		expand BINFMT to include the compiler name
dnl
BINFMT="${BINFMT}-${CXX_NAME}"
BINFMT_INDEX=""

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
