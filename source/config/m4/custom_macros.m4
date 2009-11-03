dnl -*- Mode: C++; tab-width: 1; -*-
dnl vi: set ts=2:
dnl
dnl		$Id: aclocal.m4,v 1.89.10.6 2007/08/07 17:48:28 oliver Exp $
dnl
dnl Author:
dnl   Oliver Kohlbacher
dnl   Andreas Hildebrandt
dnl
dnl		Autoconf M4 macros used by configure.ac.
dnl

dnl
dnl Determine the current version of the code and print it
dnl to the console to simplify debugging of submitted configure
dnl output.
dnl
AC_DEFUN([CF_VERSION_CHECK],[
	MACHINE=`uname -a`
	AC_MSG_RESULT(This is PROJECT $PROJECT[]_VERSION_STRING compiling.)
	AC_MSG_RESULT(Host: $MACHINE)
])

dnl
dnl		display the license and abort if not accepted
dnl		we create the file config.lic if the license was
dnl		accepted and do not show the license the second time
dnl

AC_DEFUN([CF_CHECK_LICENSE],[
	dnl deprecated -- no check required
])

dnl		define a macro to remove the directory name
dnl		from a fully specified path
dnl
AC_DEFUN([CF_BASENAME],[
 	TMP__DIR="$1/"
	while test "${TMP__DIR}" != "" ; do
		TMP__NAME=`echo ${TMP__DIR}|${CUT} -d/ -f1`
		TMP__DIR=`echo ${TMP__DIR}|${CT} -d/ -f2-`
		if test "${TMP__DIR}" = "${TMP__NAME}" ; then
			TMP__DIR=""
		fi
	done
])

dnl    define a macro to abort configure, print an appropriate error message
dnl    and package up the current stuff relevant to diagnosis into a tar
dnl    file.
AC_DEFUN([CF_ERROR],[
	AC_MSG_RESULT()
	AC_MSG_RESULT([Configure failed. If you cannot solve your problem with the aid])
	AC_MSG_RESULT([of the above error message, please contact the ]PROJECT[ mailing list])
	AC_MSG_RESULT([or the ]PROJECT[ developers. Please enclose the file 'conf.diag.tar'])
	AC_MSG_RESULT([which has been created in source. It contains the relevant])
	AC_MSG_RESULT([files from this configure run. In most cases, the information])
	AC_MSG_RESULT([is necessary to diagnose what went wrong. This file contains])
	AC_MSG_RESULT([information about your system setup and versions of compilers])
	AC_MSG_RESULT([and other tools installed in your system.])
	AC_MSG_RESULT()
	CF_CONF_DIAG_TAR
	AC_ERROR(Aborted.)
])

AC_DEFUN([CF_CONF_DIAG_TAR],[
	TARFILE=conf.diag.tar
	if test -f $TARFILE ; then 
		${RM} $TARFILE ; 
	fi
	FILES="configure configure.ac aclocal.m4 config.log"
	if test -f config.h ; then FILES="$FILES config.h" ; fi
	if test -f config.mak ; then FILES="$FILES config.mak" ; fi
	if test -f common.mak ; then FILES="$FILES common.mak" ; fi
	tar cf $TARFILE $FILES
])

dnl    define a macro to inform the user about failed tests for programs
dnl    it checks for the unix command given as second parameter and
dnl    sets the shell variable given as second parameter to its absolute path
dnl 
AC_DEFUN([CF_MSG_PATH_PROG],[
	AC_PATH_PROG($1,$2,no)
	if test $$1 = no ; then
		AC_MSG_RESULT()
		AC_MSG_RESULT([This script requires the unix command $2, but cannot find it.])
		AC_MSG_RESULT([Please add the correct path to $2 to your \$PATH variable])
		AC_MSG_RESULT([and restart configure.])
		CF_ERROR
	fi
])

dnl
dnl    define macro to search for header files that may be somewhere in the filesystem
dnl    if ${FIND}!=- (i.e. it has been set BEFORE py AC_PATH_PROG!) find will be used
dnl    too if everything fails - this may take some time...
dnl
dnl    syntax: AC_FIND_HEADER(<PATH_VAR>,<header.h>,<additional dirnames>)
dnl    
dnl        PATH_VAR will be set to the include path or to empty string (if not found)
dnl        header.h is the header file name (e.g. wait.h, GL/gl.h)
dnl        additional dirnames are included in searches these should be absolute names.
dnl 

AC_DEFUN([CF_FIND_HEADER],[
	_INCLUDES=

	dnl    immediate return on predefined directory (read from file?)
	if test "${$1}" != "" ; then
	_INCLUDES=${$1}
	fi

	if test "${_INCLUDES}" = "" ; then
		for i in /usr/include /opt/include $3 ; do
			if test -f "$i/$2" -a "${_INCLUDES}" = ""; then
				_INCLUDES="$i"
			fi
		done
	fi

	if test "${_INCLUDES}" = "" ; then
		for i in /usr/*/include /opt/*/include ; do
			if test -f "$i/$2" -a "${_INCLUDES}" = ""; then
				_INCLUDES="$i"
			fi
		done
	fi

	if test "${_INCLUDES}" = "" ; then
		for i in /opt/*/*/include /usr/*/*/include /usr/local/*/*/include ; do
			if test -f "$i/$2" -a "${_INCLUDES}" = ""; then
				_INCLUDES="$i"
			fi
		done
	fi

	if test "${_INCLUDES}" = "" -a "${FIND}" != "-" ; then
		if test "${FIND_KNOWS_PATH}" = false ; then
			FIND_OPT="-name"
			_TMP_FIND_NAME="$2"
			while test _`${EGREP} / $_TMP_FIND_NAME`_ != __ ; do
				_TMP_FIND_NAME=`echo ${_TMP_FIND_NAME}|${CUT} -d/ -f2-`
			done

			FIND_ARG="\*${_TMP_FIND_NAME}\*"
		else
			FIND_OPT="-path"
			FIND_ARG="\*$2\*"
		fi

		_TMP=`${FIND} /usr ${FIND_OPT} ${FIND_ARG} -print 2>/dev/null`
		for j in ${_TMP} ; do
			if test "${_INCLUDES}" = "" ; then
				_INCLUDES=`echo $j|${SED} "s/include\/.\*\$/include/"`
			fi
		done

		if test "${_INCLUDES}" = "" ; then
			_TMP=`${FIND} /opt ${FIND_OPT} ${FIND_ARG} -print 2>/dev/null`
			for j in ${_TMP} ; do
				if test "${_INCLUDES}" = "" ; then
					_INCLUDES=`echo $j|${SED} "s/include\/.\*\$/include/"`
				fi
			done
		fi

		if test "${_INCLUDES}" = "" -a "$3" != ""; then
			for i in $3 /dev/null ; do
				_TMP=`${FIND} $i ${FIND_OPT} ${FIND_ARG} -print 2>/dev/null`
				for j in ${_TMP} ; do
					if test "${_INCLUDES}" = "" ; then
						_INCLUDES=`echo $j|${SED} "s/include\/.\*\$/include/"`
					fi
				done
			done
		fi
	fi

	$1="${_INCLUDES}"
])


dnl
dnl    define macro to search for libraries that may be somewhere in the filesystem
dnl    if ${FIND}!=- (i.e. it has been set BEFORE py AC_PATH_PROG!) find will be used
dnl    too if everything fails - this may take some time...
dnl
dnl    syntax: CF_FIND_LIB(<PATH_VAR>,<libXXX>,<additional dirnames>)
dnl    
dnl        PATH_VAR will be set to the library path or to empty string (if not found)
dnl        libXXX is the header file name (e.g. libGLUT, libGL) .a, .so etc. should be omitted
dnl        additional dirnames are included in searches these should be absolute names.
dnl 

AC_DEFUN([CF_FIND_LIB],[
	_LIBS=

	dnl   immediate "return" on preset directory (read from file?)
	if test "${$1}" != "" ; then
		_LIBS=${$1}
	fi

	if test "$3" = "" ; then
		_LIBDIRS="/usr/lib /opt/lib /usr/local/lib"
	else
		_LIBDIRS="$3"
	fi

	if test "${_LIBS}" = "" ; then
		for i in ${_LIBDIRS} ; do
			for j in $i/$2.* ; do
				if test -f "$j" -a "${_LIBS}" = ""; then
					_LIBS="$i"
				fi
			done
		done
	fi

	if test "${_LIBS}" = "" ; then
		for i in /usr/lib /opt/lib /usr/*/lib /opt/*/lib ; do
			for j in $i/$2.* ; do
				if test -f "$j" -a "${_LIBS}" = ""; then
					_LIBS="$i"
				fi
			done
		done
	fi

	if test "${_LIBS}" = "" ; then
		for i in /opt/*/*/lib /usr/*/*/lib /usr/local/*/*/lib ; do
			for j in $i/$2.* ; do
				if test -f "$j" -a "${_LIBS}" = ""; then
					_LIBS="$i"
				fi
			done
		done
	fi

	if test "${_LIBS}" = "" -a "${FIND}" != "-" ; then
		if test "${_LIBS}" = "" -a "$3" != ""; then
			for i in $3 /dev/null; do
				if test "${_LIBS}" = "" ; then
					_TMP=`${FIND} $i -name "$2.*" -print 2>/dev/null`
					for j in ${_TMP} ; do
						if test "${_LIBS}" = "" ; then
							_LIBS=`echo $j|${SED} "s/\/$2\\..*/\//"`
						fi
					done
				fi
			done
		fi

		if test "${_LIBS}" = "" ; then
			_TMP=`${FIND} /opt -name "$2.*" -print 2>/dev/null`
			for j in ${_TMP} ; do
				if test "${_LIBS}" = "" ; then
					_LIBS=`echo $j|${SED} "s/\/$2\\..*/\//"`
				fi
			done
		fi

		if test "${_LIBS}" = "" ; then
			_TMP=`${FIND} /usr -name "$2.*" -print 2>/dev/null`
			for j in ${_TMP} ; do
				if test "${_LIBS}" = "" ; then
					_LIBS=`echo $j|${SED} "s/\/$2\\..*/\//"`
				fi
			done
		fi

	fi

	$1="${_LIBS}"
])

dnl
dnl   check whether "echo" understands "-n" (required on some
dnl   platforms to expand '\n')
dnl
AC_DEFUN([CF_CHECK_ECHO],[
AC_MSG_CHECKING(whether echo accepts -e)
if `/bin/sh -c "echo -e \"\n\"" >/dev/null 2>&1` ; then
	if test "`/bin/sh -c echo -e 2>&1`" = "" -a "`/bin/sh -c echo -e OK`" = "OK" ; then
		ECHO_COMMAND="echo -e"
		AC_MSG_RESULT(yes)	
	else
		ECHO_COMMAND="echo"
		AC_MSG_RESULT(no)	
	fi
else
	ECHO_COMMAND="echo"
	AC_MSG_RESULT(no)
fi
AC_SUBST(ECHO_COMMAND)
])


dnl
dnl   check whether find can be called with the parameter -path
dnl   (needed to find headers in a certain path like GL/libgl.h
dnl
AC_DEFUN([CF_CHECK_FIND],[
if test "${FIND}" != "no" ; then
	RESULT=`${FIND} KERNEL -path . -print 2>&1`
	if test "${RESULT}" != "" ; then     dnl    did get an error message ... bad.
		FIND_KNOWS_PATH=false
	else
		FIND_KNOWS_PATH=true
	fi
fi
])

dnl
dnl    determine OS and architecture and all this stuff
dnl
AC_DEFUN([CF_DETECT_OS],[
AC_SUBST(OSMAJOR)
AC_SUBST(OS)
AC_SUBST(OSREV)
AC_SUBST(ARCHITECTURE)
RANLIB="echo"

AC_MSG_CHECKING(your OS)
OS=`${UNAME} -s`
OSREV=`${UNAME} -r`
OSMAJOR=`echo $OSREV|${CUT} -d"." -f1`

dnl		default...
BINFMT="${OS}"

if test "$OS" = SunOS ; then
	if test "$OSMAJOR" = 5 ; then
		OS=Solaris
		ARCHITECTURE=`${UNAME} -p`
		BINFMT="${OS}-${OSREV}-${ARCHITECTURE}"
	else
		OS=SunOS
	fi
fi

if test "$OS" = Linux ; then
	PROCESSOR=`${UNAME} -m`
	ARCHITECTURE=unknown
	if test "${PROCESSOR}" = sparc -o "${PROCESSOR}" = SPARC ; then
		ARCHITECTURE=sparc
		BINFMT=Linux-sparc
	fi
	if test `echo $PROCESSOR|${CUT} -c1` = i ; then
		ARCHITECTURE=i386
		BINFMT=Linux-i386
	fi
	if test `echo $PROCESSOR` = alpha ; then
		ARCHITECTURE=alpha
		BINFMT=Linux-alpha
	fi
	if test `echo $PROCESSOR` = x86_64 ; then
		ARCHITECTURE=x86_64
		BINFMT=Linux-x86_64
	fi
	if test `echo $PROCESSOR` = ppc ; then
    ARCHITECTURE=ppc32
    BINFMT=Linux-ppc32
	fi
	if test `echo $PROCESSOR` = ppc64 ; then
    ARCHITECTURE=ppc64
    BINFMT=Linux-ppc64
	fi
	if test `echo $PROCESSOR` = s390 ; then
		ARCHITECTURE=s390
		BINFMT=s390
	fi

	if test "${ARCHITECTURE}" = "unknown" -a "${PROJECT[]_IGNORE_ARCH}" = ""; then
		AC_MSG_RESULT(OS: ${OS} / hardware: ${PROCESSOR})
		AC_MSG_RESULT(Sorry - this architecture is currently not supported...)
		CF_ERROR
	fi
fi

if test "${OS}" = IRIX64 ; then
	OS=IRIX
fi

if test "${OS}" = IRIX ; then
	BINFMT=IRIX-${OSREV}
fi

if test "${OS}" = OSF1 ; then
	BINFMT="OSF1-${OSREV}"
	PROCESSOR=`${UNAME} -m`
	ARCHITECTURE=unknown
	if test `echo $PROCESSOR` = alpha ; then
		ARCHITECTURE=alpha
	fi
fi

if test "${OS}" = Darwin ; then
	BINFMT="Darwin-${OSREV}"
	PROCESSOR=`${UNAME} -p`
	ARCHITECTURE=`${UNAME} -m`
fi

if test "`echo $OS | ${CUT} -d_ -f1`" = "CYGWIN" ; then
	OS="CYGWIN"
	PROJECT[]_NO_XDR=true
fi

if test "$OS" != Linux -a "$OS" != Solaris -a "$OS" != IRIX \
	-a  "$OS" != OSF1 -a "$OS" != FreeBSD -a "$OS" != "CYGWIN" \
	-a "${OS}" != Darwin -a "${PROJECT[]_IGNORE_ARCH}" = "" ; then
	AC_MSG_RESULT(Sorry - your OS ($OS) is currently not supported...)
	CF_ERROR
fi

dnl
dnl 	create OS defines in config.h:
dnl
if test "${OS}" = Linux ; then
	AC_DEFINE(PROJECT[]_OS_LINUX,LINUX)
fi
if test "${OS}" = Solaris ; then
	AC_DEFINE(PROJECT[]_OS_SOLARIS,SOLARIS)
fi
if test "${OS}" = IRIX ; then
	AC_DEFINE(PROJECT[]_OS_IRIX,IRIX)
fi
if test "${OS}" = OSF1 ; then
	AC_DEFINE(PROJECT[]_OS_OSF1,OSF1)
fi
if test "${OS}" = FreeBSD ; then
	AC_DEFINE(PROJECT[]_OS_FREEBSD,FREEBSD)
fi
if test "${OS}" = Darwin ; then
	AC_DEFINE(PROJECT[]_OS_DARWIN,DARWIN)
fi

dnl
dnl		create ARCHITECTURE defines
dnl
if test "$ARCHITECTURE" = sparc ; then
	AC_DEFINE(PROJECT[]_ARCH_SPARC,SPARC)
fi
if test "$ARCHITECTURE" = i386 ; then
	AC_DEFINE(PROJECT[]_ARCH_I386,I386)
fi
if test "$ARCHITECTURE" = mips ; then
	AC_DEFINE(PROJECT[]_ARCH_MIPS,MIPS)
fi
if test "$ARCHITECTURE" = alpha ; then
	AC_DEFINE(PROJECT[]_ARCH_ALPHA,ALPHA)
fi
if test "$ARCHITECTURE" = ppc64 ; then
	AC_DEFINE(PROJECT[]_ARCH_POWERPC64,POWERPC64)
fi
if test "$ARCHITECTURE" = ppc32 ; then
	AC_DEFINE(PROJECT[]_ARCH_POWERPC32,POWERPC32)
fi


AC_MSG_RESULT($OS $OSREV (BINFMT=$BINFMT))

dnl
dnl	some definitions the depend solely on the OS
dnl
SHARED_LIB_SUFFIX=so
if test "${OS}" = HP-UX ; then
	SHARED_LIB_SUFFIX=sl
fi
if test "${OS}" = Darwin ; then
	SHARED_LIB_SUFFIX=dylib
fi
if test "${OS}" = CYGWIN ; then
	SHARED_LIB_SUFFIX=dll
fi
AC_SUBST(SHARED_LIB_SUFFIX)
])


dnl
dnl
dnl   Declare compiler search order
dnl     1) look for compiler defined in configure
dnl     2) look for vendor supplied compilers (CC)
dnl     3) check for g++, egcs, eg++, gcc
dnl   Except for Solaris, where the vendor supplied compiler
dnl   CC (at least releases 5.0 and below) is not usable.
AC_DEFUN([CF_SEARCH_CXX],[
CXX_NAME=""
case "${OS}" in
	Solaris )		CXX_SEARCH_ORDER="g++ CC ";;
	IRIX ) 			CXX_SEARCH_ORDER="CC g++ ";;
	OSF1 )			CXX_SEARCH_ORDER="cxx CC g++ ";;
	* ) 				CXX_SEARCH_ORDER="g++ CC cxx ";;
esac

dnl
dnl   Search for the C++ compiler
dnl
AC_MSG_CHECKING(searching for C++ compiler)
if test "${CXX}" != "" ; then
	if test -x "${CXX}" ; then
		AC_MSG_RESULT(from the command line: ${CXX})
	else
		AC_PATH_PROG(CXXPATH,${CXX},no)
		if test "${CXXPATH}" = no ; then
			AC_MSG_RESULT()
			AC_MSG_RESULT(Cannot find ${CXX}. Please add it to your PATH)
			AC_MSG_RESULT(or specify an absolute path in configure.)
			CF_ERROR
		else
			CXX=${CXXPATH}
		fi
	fi
else
	AC_MSG_RESULT()
	CXXPATH=""
	while test "${CXXPATH}" = "" ; do
	CXX=`echo ${CXX_SEARCH_ORDER}|${CUT} -d\  -f1`
	if test _`echo ${CXX} | ${TR} -d " "`_ = __ ; then
		CXXPATH="END"
	fi
	if test "${CXXPATH}" != "END" ; then
		AC_PATH_PROG(CXXPATH,${CXX},no)
		if test "${CXXPATH}" = no ; then
			CXXPATH=""
			unset ac_cv_path_CXXPATH
		else
			CXX=${CXXPATH}
		fi
	fi

	CXX_SEARCH_ORDER=`echo "${CXX_SEARCH_ORDER} " |${CUT} -d\  -f2-`
	done

	if test "${CXXPATH}" = "end" ; then
		AC_MSG_RESULT()
		AC_MSG_RESULT(Could not find a C++ compiler. Please change the settings)
		AC_MSG_RESULT(of your PATH environment variable (using setenv export))
		AC_MSG_RESULT(or specify an absolute path in configure by setting the variable)
		AC_MSG_RESULT(CXX=<pathname> or specify the compiler by passing the option)
		AC_MSG_RESULT(--with-compiler=<compiler> to configure.)
		CF_ERROR
	fi
fi

dnl
dnl   extract the executable name of the compiler
dnl   as default compiler name (CXX_NAME is needed
dnl   to name the default directory the libraries
dnl   reside in)
dnl

if test "${CXX_PATH}" = "" ; then
	if test "${CXX}" = "" ; then
		CXX_NAME=unknown
	else
		CXX_NAME="${CXX}"
	fi
else
	CXX_NAME="${CXX_PATH}"
fi

while test "`echo ${CXX_NAME}|  ${GREP} /`" != "" ; do
	CXX_NAME=`echo ${CXX_NAME} |  ${CUT} -d/ -f2-`
done
])



dnl
dnl		Break up the compiler version into its major and minor release numbers
dnl
AC_DEFUN([CF_DIGEST_CXX_VERSION],[
CXX_VERSION_1=`echo ${CXX_VERSION} | ${CUT} -d. -f1`
CXX_VERSION_LENGTH=`echo ${CXX_VERSION} | sed "s/[^.]//g" | wc -c`
if test "${CXX_VERSION_LENGTH}" -ge 2 ; then
	CXX_VERSION_2=`echo ${CXX_VERSION} | ${CUT} -d. -f2`
fi
if test "${CXX_VERSION_LENGTH}" -ge 3 ; then
	CXX_VERSION_3=`echo ${CXX_VERSION} | ${CUT} -d. -f3`
fi
if test "${CXX_VERSION_LENGTH}" -ge 4 ; then
	CXX_VERSION_4=`echo ${CXX_VERSION} | ${CUT} -d. -f4`
fi
AC_DEFINE_UNQUOTED(PROJECT[]_COMPILER_VERSION_MAJOR, ${CXX_VERSION_1})
AC_DEFINE_UNQUOTED(PROJECT[]_COMPILER_VERSION_MINOR, ${CXX_VERSION_2})
AC_DEFINE_UNQUOTED(PROJECT[]_COMPILER_VERSION_MINOR_MINOR, ${CXX_VERSION_3})
])

dnl
dnl		Check whether CXX is a GNU compiler and retrieve its
dnl			version number.
dnl
AC_DEFUN([CF_IDENTIFY_GXX],[
	AC_MSG_CHECKING(for GNU compiler)
	cat > /tmp/$$.conftest.c << EOF
	#ifdef __GNUC__
	GXX:true
	#else
	GXX:false
	#endif
EOF

	IS_GXX=`${CXX} -E /tmp/$$.conftest.c 2>/dev/null | ${EGREP} GXX|${CUT} -d: -f2|${TR} -d " "`
	if test "${IS_GXX}" = "true" ; then
		AC_MSG_RESULT(yes)
		HAS_GPLUSPLUS=true
		CXX_NAME="g++"
		CXX_IDENTIFIED=true

		dnl 
		dnl 	Define a symbol for G++.
		dnl
		AC_DEFINE(PROJECT[]_COMPILER_GXX, )
		AC_DEFINE(PROJECT[]_COMPILER, GXX)
	else
		AC_MSG_RESULT(no)
		HAS_GPLUSPLUS=false
	fi
	${RM} /tmp/$$.conftest.c
])

AC_DEFUN([CF_GXX_OPTIONS], [
AC_MSG_CHECKING(compiler version)	
VERSION_FILE=/tmp/$$.gnu_version.C
echo "__GNUC__.__GNUC_MINOR__.__GNUC_PATCHLEVEL__" > ${VERSION_FILE}
CXX_VERSION=`${CXX} -E ${VERSION_FILE} | ${GREP} -v "^#" | ${TR} -d " "`
${RM} ${VERSION_FILE}
if test `echo ${CXX_VERSION}|${CUT} -c1-4` = "egcs" ; then
IS_EGXX=true
CXX_NAME="egcs"
CXX_VERSION=`${CXX} -v 2>&1 | grep release | ${CUT} -d\( -f2 | cut -d\) -f1 | ${SED} "s/egcs-//" | ${CUT} -d" " -f1`
VERSION_OUTPUT="egcs ${CXX_VERSION}"
CXX_COMPILER_NAME="egcs"
else
IS_EGXX=false
VERSION_OUTPUT="g++ ${CXX_VERSION}"
CXX_COMPILER_NAME="g++"
fi

AC_MSG_RESULT(${VERSION_OUTPUT})
CF_DIGEST_CXX_VERSION

if test "${CXX_VERSION_1}" -lt 2 \
-o "${CXX_VERSION_1}" = 2 -a "${CXX_VERSION_2}" -lt 95 ; then
AC_MSG_RESULT()
AC_MSG_RESULT([The version of gcc you are using is not supported by PROJECT[].])
AC_MSG_RESULT([Please update to a newer version of g++ (at least 2.95.x)])
AC_MSG_RESULT([which can be obtained from])
AC_MSG_RESULT([  ftp://gcc.gnu.org/pub/gcc/releases/index.html])
AC_MSG_RESULT([or specify a different compiler using the option --with-compiler=])
CF_ERROR
fi

dnl
dnl		For g++ 3 and above we have to link against libiberty.a in order
dnl			to get the name demangling done.
dnl
if test "${CXX_VERSION_1}" -ge 3 ; then
AC_MSG_CHECKING(whether libiberty is required)
AC_MSG_RESULT(yes)
AC_MSG_CHECKING(whether libiberty is available)
SAVE_LIBS="${LIBS}"
LIBS="${LIBS} -liberty"
HAS_LIBIBERTY=false
AC_TRY_LINK([],[], HAS_LIBIBERTY=true)
if test "${HAS_LIBIBERTY}" != true ; then
LIBS="${SAVE_LIBS}"
AC_MSG_RESULT(no)
else
AC_MSG_RESULT(yes)
fi
fi

dnl
dnl		For g++ 4.3 and above, we want to enable c++0x
dnl
if test "${HAS_GPLUSPLUS}" = true; then
	AC_MSG_CHECKING(whether c++0x can be enabled)
	if test "${CXX_VERSION_1}" -ge 4 ; then
		if test "${CXX_VERSION_2}" -ge 3; then
			AC_MSG_RESULT(yes)
			CXXFLAGS="${CXXFLAGS} -std=c++0x"
			USE_TR1=true
		else
			AC_MSG_RESULT(no)
		fi
	else
		AC_MSG_RESULT(no)
	fi
fi

dnl
dnl		Here go the g++-specific options
dnl
CXXFLAGS="${CXXFLAGS} -pipe"
CXX_MAKEDEPEND="${CXX}"
MAKEDEP_CXX_OPTS="-MM"
CXXFLAGS_D="${CXXFLAGS_D} -Wall -W -pedantic -Wno-long-long"
CXXFLAGS_DI="${CXXFLAGS_DI} -g"

dnl
dnl	Some compiler versions have problems with -O3 unter Darwin (3.3.0),
dnl so we go back to -O1.
dnl
if test "${OS}" = "Darwin" ; then
	if test "${CXX_VERSION_1}" -lt 4 ; then
		CXXFLAGS_O="${CXXFLAGS_O} -O1 -Wall -W -pedantic -Wno-long-long -Wno-long-double"		
	else
		CXXFLAGS_O="${CXXFLAGS_O} -O3 -Wall -W -pedantic -Wno-long-long -Wno-long-double"		
	fi
	CXXFLAGS_D="${CXXFLAGS_D} -O3"
else
	CXXFLAGS_O="${CXXFLAGS_O} -O3 -Wall -W -pedantic -Wno-long-long"
fi
MAKEDEP_CXX_SUFFIX=" >.Dependencies"

dnl  We do not need the -fPIC flag for CYGWIN and Darwin,
dnl  because its code is always position independent.
dnl  A warning is emitted otherwise.
if test "${OS}" != "CYGWIN" -a "${OS}" != "Darwin" ; then
	CXXFLAGS="${CXXFLAGS} -fPIC"
fi

DYNAR="${CXX}"
if test "${OS}" = "Solaris" ; then
	DYNAROPTS="${DYNAROPTS} -G -fPIC -o"
else 
	if test "${OS}" = "Darwin" ; then
		DYNAROPTS="${DYNAROPTS} -headerpad_max_install_names -compatibility_version ${BALL_COMPATIBILITY_VERSION} -current_version ${BALL_CURRENT_VERSION} -dynamiclib -o"
		ADD_DYNAROPTS_LIBBALL="-seg1addr 0xb0000000 -install_name \$(DESTINATION)/\$(LIBDIR)/libBALL.dylib"
		ADD_DYNAROPTS_LIBVIEW="-seg1addr 0x80000000 -install_name \$(DESTINATION)/\$(LIBDIR)/libVIEW.dylib"
		DYNAROPTS_PYMODULE="-headerpad_max_install_names -bundle -framework Python -o"
		RANLIB="ranlib -s "
	else	
		DYNAROPTS="${DYNAROPTS} -shared -fPIC -o"
	fi
fi

if test "${IS_EGXX}" = true; then
	PROJECT[]_TYPENAME=typename
else
	if test "${CXX_VERSION_1}" -gt 2 -o "${CXX_VERSION_1}" -eq 2 -a "${CXX_VERSION_2}" -ge 8 ; then
		PROJECT[]_TYPENAME=typename
	fi
fi
])


dnl		Check for KAI C++ (KCC)
dnl   At least under linux the damned frontend won't tell
dnl   its version number, so we try to extract the word kai
dnl   from its drivers options when called in verbose mode.
dnl   Nasty - but seems to work. Anybody with a better solution
dnl   should feel free to inform me!
dnl
AC_DEFUN([CF_IDENTIFY_KAI], [
AC_MSG_CHECKING(for KAI C++ compiler)
KAI=`${CXX} -v --version 2>&1 | sed "s/.*KAI.*/__KAI__/g" |sed "s/.*kai.*/__KAI__/g" | ${EGREP} "^__KAI__$" | sed -n 1p`
if test "${KAI}" = "__KAI__" ; then
IS_KCC=true
AC_MSG_RESULT(yes)
CXX_NAME="KAI"
CXX_IDENTIFIED=true

Dnl 
dnl 	Define a symbol for KAI C++.
dnl
AC_DEFINE(PROJECT[]_COMPILER_KAI, )
AC_DEFINE(PROJECT[]_COMPILER, KAI)
else
IS_KCC=false
AC_MSG_RESULT(no)
fi

])

dnl
dnl		KAI-C++-specific options
dnl
AC_DEFUN([CF_KAI_OPTIONS], [
	AC_MSG_CHECKING(compiler version)
	echo "int main(){}" > conftest.C
	CXX_VERSION=`${CXX} -v --version conftest.C 2>&1| ${GREP} "KAI C++ " | ${CUT} -d" " -f3`
	CXX_NAME="KCC"
	VERSION_OUTPUT="KAI C++ ${CXX_VERSION}"
	CXX_COMPILER_NAME="KCC"
		
	AC_MSG_RESULT(${VERSION_OUTPUT})
	CF_DIGEST_CXX_VERSION

	dnl   KAI C++ stores a list of instantiated templates
	dnl   in directories called ti_files
	dnl   make clean should remove these
	TEMPLATE_DIR="ti_files"
	AR="${CXX}"
	DYNAR="${CXX}"
	AROPTS="${AROPTS} -o"
	DYNAROPTS="${DYNAROPTS} -o"
	CXX_MAKEDEPEND="${CXX}"
	MAKEDEP_CXX_OPTS="-M"
	MAKEDEP_CXX_SUFFIX=" >.Dependencies"

	dnl
	dnl   Someone at KAI seems to have the need
	dnl   to torture developers by introducing
	dnl   a new flag for position independent code
	dnl   on EVERY platform...
	dnl
	CXXFLAGS="${CXXFLAGS} --one_per"
	if test "${OS}" = Linux ; then
		CXXFLAGS="${CXXFLAGS} -fPIC"
	fi
	if test "${OS}" = Solaris ; then
		CXXFLAGS="${CXXFLAGS} -KPIC"
	fi
	if test "${OS}" = IRIX ; then
		CXXFLAGS="${CXXFLAGS} -KPIC"
	fi

	dnl   optimze as on highest level: this compiler
	dnl   does a good job optimizing!
	CXXFLAGS_O="${CXXFLAGS_O} +K3"

	dnl   avoid high level optimization to
	dnl   get debuggable code...
	CXXFLAGS_D="${CXXFLAGS_D} +K0"
	CXXFLAGS_DI="${CXXFLAGS_DI}"

	dnl
	dnl if we are running under Solaris/SPARC,
	dnl KAI can produce 32 or 64 bit code
	dnl
	if test "${OS}" = "Solaris" -a "${ARCHITECTURE}" = sparc ; then
		if test "${BINFMT_64_BIT}" = true ; then
			CXX_NAME="${CXX_NAME}_V9"
			LDFLAGS="${LDFLAGS} -xarch=v9"
			CXXFLAGS="${CXXFLAGS} -xarch=v9"
			AROPTS="${AROPTS} -xarch=v9"
			DYNAROPTS="-xarch=v9 ${DYNAROPTS}"
		else
			CXX_NAME="${CXX_NAME}_V8"
		fi
	fi
])

dnl
dnl   Check for Intel C++ (icc)
dnl
AC_DEFUN([CF_IDENTIFY_INTEL], [
AC_MSG_CHECKING(for Intel C++ compiler)
ICC=`${CXX} -V 2>&1 | ${SED} -n 1p |${SED} "s/Intel(R) C.* Compiler.*/__INTELCC__/g"| ${SED} "s/Intel(R) C++.* Compiler.*/__INTELCC__/g" | ${EGREP} "^__INTELCC__$" | sed -n 1p`
if test "${ICC}" = "__INTELCC__" ; then
IS_INTELCC=true
AC_MSG_RESULT(yes)
CXX_NAME="Intel"
CXX_IDENTIFIED=true


dnl 
dnl 	Define a symbol for Intel C++.
dnl
AC_DEFINE(PROJECT[]_COMPILER_INTEL, )
AC_DEFINE(PROJECT[]_COMPILER, INTEL)
else
IS_INTELCC=false
AC_MSG_RESULT(no)
fi
])

dnl
dnl		Set the Intel icc-specific options.
dnl
AC_DEFUN([CF_INTEL_OPTIONS],[
AC_MSG_CHECKING(compiler version)
echo "int main(){}" > conftest.C
CXX_VERSION=`${CXX} -V conftest.C 2>&1| ${GREP} "Intel(R) C++" | ${SED} -n 1p | ${SED} "s/.*Version //" | ${CUT} -d" " -f1`
if test "${CXX_VERSION}" = "" ; then
  CXX_VERSION=`${CXX} -v 2>&1| ${SED} "s/.*ersion //" | ${CUT} -d " " -f1`
fi
CXX_NAME="icc"
VERSION_OUTPUT="Intel C++ Compiler ${CXX_VERSION}"
CXX_COMPILER_NAME="icc"

AC_MSG_RESULT(${VERSION_OUTPUT})
CF_DIGEST_CXX_VERSION


DYNAROPTS="${DYNAROPTS} -shared -o"
TEMPLATE_DIR=""
AR="${CXX}"
DYNAR="${CXX}"
AROPTS="${AROPTS} -o"
CXX_MAKEDEPEND="${CXX}"
MAKEDEP_CXX_OPTS="-M"
MAKEDEP_CXX_SUFFIX=" >.Dependencies"
CXXFLAGS="${CXXFLAGS} -fPIC"

dnl
if test "${CXX_VERSION_1}" != "" -a "${CXX_VERSION_2}" != "" ; then
  if test ${CXX_VERSION_1} = 8 -a ${CXX_VERSION_2} -ge 1 -o ${CXX_VERSION_1} -ge 9 ; then
    if test `basename ${CXX}` = "icc" ; then
      AC_MSG_RESULT([WARNING: Starting with version 8.1, icpc should be used instead of icc.])
      AC_MSG_RESULT([Otherwise, linking errors will occur! Please call configure again])
      AC_MSG_RESULT([with icpc as the compiler.])
      AC_MSG_RESULT()
      CF_ERROR(Aborted.)
    fi
  fi 
  dnl
  dnl  icpc 9.0 has a problem: linking without -O0 invokes IPO, which
  dnl  doesn't seem to work ("Cannot find "("..." as error message).
  dnl  So we just turn it off.
  dnl
  if test ${CXX_VERSION_1} -ge 9 ; then
    LDFLAGS="$LDFLAGS -O0"
    DYNAROPTS="-O0 $DYNAROPTS"
  fi
fi



dnl   optimze as on highest level: this compiler
CXXFLAGS_O="-cxxlib -xW -fPIC -O3 -inline-level=1 -restrict -w1 -msse -msse2"

dnl   avoid high level optimization to
dnl   get debuggable code...
CXXFLAGS_D="-cxxlib -g -O0 -inline-level=0 -w1 -msse -msse2"
CXXFLAGS_DI="${CXXFLAGS_DI}"
])

dnl
dnl   check for the Digital C++ compiler
dnl
dnl
AC_DEFUN([CF_IDENTIFY_COMPAQ],[
AC_MSG_CHECKING(for Digital/Compaq C++ compiler)
DIGITAL_CXX=`${CXX} -V 2>/dev/null | ${GREP} "C++" | ${CUT} -d" " -f-2`
if test "${DIGITAL_CXX}" = "DIGITAL C++" -o "${DIGITAL_CXX}" = "Compaq C++"; then
IS_DIGITALCXX=true
AC_MSG_RESULT(yes)
CXX_NAME="Compaq"
CXX_IDENTIFIED=true

dnl 
dnl 	Define a symbol for Compaq C++.
dnl
AC_DEFINE(PROJECT[]_COMPILER_COMPAQ, )
AC_DEFINE(PROJECT[]_COMPILER, COMPAQ)
else
IS_DIGITALCXX=false
AC_MSG_RESULT(no)
fi

])

AC_DEFUN([CF_COMPAQ_OPTIONS], [
AC_MSG_CHECKING(compiler version)
echo "int main(){}" > conftest.C
CXX_VERSION=`${CXX} -V  2>/dev/null| ${GREP} "C++" | ${SED} "s/^.*C++ //" | ${CUT} -d" " -f1 | ${TR} -d V | ${TR} "-" "."`
CXX_NAME=`${CXX} -V | ${GREP} "C++" | ${CUT} -d" " -f1`
VERSION_OUTPUT="${CXX_NAME} C++ ${CXX_VERSION}"
CXX_COMPILER_NAME="Digital"

AC_MSG_RESULT(${VERSION_OUTPUT})
CF_DIGEST_CXX_VERSION

if test "${CXX_VERSION_1}" -lt 6 -o "${CXX_VERSION_1}" -eq 6 -a "${CXX_VERSION_2}" -lt 2 ; then
AC_MSG_RESULT()
AC_MSG_RESULT(Your version of Digital/Compaq C++ does not provide all)
AC_MSG_RESULT(ANSI C++ features required by PROJECT[].)
AC_MSG_RESULT(Please upgrade to release 6.2 or above.)
CF_ERROR
fi

TEMPLATE_DIR="cxx_rep"
AR="ar"
DYNAR="${CXX}"
AROPTS="${AROPTS} -o"
DYNAROPTS="${DYNAROPTS} -shared -nocxxstd -ptr \$(PROJECT[]_PATH)/source/cxx_rep -o"
CXX_MAKEDEPEND="${CXX}"
MAKEDEP_CXX_OPTS="-M -noimplicit_include"
MAKEDEP_CXX_SUFFIX=" >.Dependencies"

dnl 
dnl  CXX 6.2 does not provide the -nopure_cname flag
if test "${CXX_VERSION_2}" -lt 3 ; then
CXXFLAGS="${CXXFLAGS} -ieee"
else
if test "${CXX_VERSION_2}" -ge 5 ; then
dnl
dnl  Starting with cxx 6.5, we had some trouble with
dnl  floating point accuracy -- that should take care of it.
CXXFLAGS="${CXXFLAGS} -ieee -nopure_cname"
else
CXXFLAGS="${CXXFLAGS} -ieee -nopure_cname"
fi
fi

LIB_CXXFLAGS="${LIB_CXXFLAGS} -ptr \$(PROJECT[]_PATH)/source/cxx_rep"
CXXFLAGS_O="${CXXFLAGS_O} -O3"

CXXFLAGS_D="${CXXFLAGS_D}"
CXXFLAGS_DI="${CXXFLAGS_DI} -g"

dnl   Problem with linux headers:
dnl   cannot use -std strict_ansi since the socket headers
dnl   cause an error #280
if test "${OS}" != "Linux" ; then
	CXXFLAGS="${CXXFLAGS} -std strict_ansi"
	MAKEDEP_CXX_OPTS="${MAKEDEP_CXX_OPTS} -std strict_ansi"
fi
])

AC_DEFUN([CF_IDENTIFY_SGI], [
AC_MSG_CHECKING(for SGI/MipsPro C++ compiler)
SGI_CXX=`${CXX} -version -n32 2>&1 | ${GREP} "MIPSpro" | ${CUT} -d" " -f1`
if test "${SGI_CXX}" = "MIPSpro"; then
IS_MIPSPRO=true
AC_MSG_RESULT(yes)
CXX_NAME="MIPSpro"
CXX_IDENTIFIED=true

dnl 
dnl 	Define a symbol for SGI C++.
dnl
AC_DEFINE(PROJECT[]_COMPILER_MIPSPRO, )
AC_DEFINE(PROJECT[]_COMPILER, MIPSPRO)
else
IS_MIPSPRO=false
AC_MSG_RESULT(no)
fi
])

AC_DEFUN([CF_MIPSPRO_OPTIONS], [
AC_MSG_CHECKING(compiler version)
CXX_VERSION_STRING=`${CXX} -n32 -version 2>&1 | ${EGREP} ersion`
if test "${CXX_VERSION_STRING}" = "" ; then
CF_BASENAME(${CXX})
CXX_VERSION="${TMP__NAME}"
CXX_VERSION_OUTPUT="${CXX_VERSION} (unknown version)"
else
CXX_VERSION=`echo ${CXX_VERSION_STRING} | ${SED} "s/^.*ersion //g"`
CXX_COMPILER_NAME=`echo ${CXX_VERSION_STRING} | ${CUT} -d\  -f1`
CXX_VERSION_OUTPUT="${CXX_VERSION} (${CXX_COMPILER_NAME})"
fi
CF_DIGEST_CXX_VERSION	
AC_MSG_RESULT(${CXX_VERSION_OUTPUT})

dnl  set the name for the template repository
dnl
TEMPLATE_DIR="ii_files"

dnl  set the default binary format (if none selected)
dnl
if test "${BINFMT_64_BIT}" = true ; then
	IRIX_BINFMT=64
	CXX_NAME="${CXX_NAME}_64"
else
	IRIX_BINFMT=N32
	CXX_NAME="${CXX_NAME}_N32"
fi

PROJECT[]_TYPENAME=typename

dnl
dnl     a version above 7.2 is required
dnl
if test "${CXX_VERSION_1}" -lt 7\
		-o "${CXX_VERSION_1}" -eq 7 -a "${CXX_VERSION_2}" = 10\
		-o "${CXX_VERSION_1}" -eq 7 -a "${CXX_VERSION_2}" = 20\
		-o "${CXX_VERSION_1}" -eq 7 -a "${CXX_VERSION_2}" -lt 2; then
AC_MSG_RESULT()
AC_MSG_RESULT(MipsPro CC version 7.30 or above is required. Please update your compiler.)
CF_ERROR
fi

AR=${CXX}
AROPTS="${AROPTS} -ar -o"
DYNAR=${CXX}
DYNAROPTS="${DYNAROPTS} -LANG:std -shared -quickstart_info -no_unresolved -o"

dnl  issue a warning about an old compiler with a broken ostream implementation: reopening a fstream
dnl  and writing to it will omit the first 16k written to the stream. Nasty, but confirmed with SGI
dnl  and fixed in 7.3.1.1m
if test "${CXX_VERSION_1}" = 7 -a "${CXX_VERSION_2}" = 3 \
		-a "${CXX_VERSION_3}" = 1m -a "${CXX_VERSION_4}" = ""\
		-o "${CXX_VERSION_1}" = 7 -a "{CXX_VERSION_2}" = 30 ; then
COMMENTS="${COMMENTS}\nPlease take care - this version of SGI CC (7.3.1m) contains serious bugs\n"
COMMENTS="${COMMENTS}in its implementaion of fstream/iostream. This may lead to strange behaviour\n"
COMMENTS="${COMMENTS}and causes PDBFile_test to fail. Please update your compiler.\n\n"
AC_MSG_RESULT(${COMMENTS})
ADDITIONAL_COMMENTS="${ADDITIONAL_COMMENTS}${COMMENTS}"
COMMENTS=""
fi

CXX_MAKEDEPEND="${CXX}"
MAKEDEP_CXX_OPTS="-M 2>/dev/null"
MAKEDEP_CXX_SUFFIX=" >.Dependencies"

if test "${IRIX_BINFMT}" = 64 ; then
	DEF_BOOL=false
	CXXFLAGS="$CXXFLAGS -64 -LANG:std"
	DYNAROPTS="-64 ${DYNAROPTS}"
	CXXFLAGS_O="${CXXFLAGS_O} -O3 -OPT:Olimit=60000 -multigot -G 5 -DEBUG:woff=3333"
	CXXFLAGS_D="${CXXFLAGS_D} -fullwarn -multigot -G 5 -DEBUG:woff=1375,3201,1424,3333,1110,1209"
	CXXFLAGS_DI="${CXXFLAGS_DI} -g"
	LDFLAGS="$LDFLAGS -64 -LANG:std"
	AC_DEFINE(IRIX64,)
fi
if test "${IRIX_BINFMT}" = N32 ; then
	DEF_BOOL=false
	CXXFLAGS="$CXXFLAGS -n32 -LANG:std"
	DYNAROPTS="-n32 ${DYNAROPTS}"
	CXXFLAGS_O="${CXXFLAGS_O} -O3 -OPT:Olimit=60000 -multigot -G 5 -DEBUG:woff=3333"
	CXXFLAGS_D="${CXXFLAGS_D} -fullwarn -multigot -G 5 -DEBUG:woff=1375,3201,1424,3333,1110,1209"
	CXXFLAGS_DI="${CXXFLAGS_DI} -g"
	LDFLAGS="$LDFLAGS -n32 -LANG:std"
	AC_DEFINE(IRIX32,)
fi

dnl
dnl  -O3 requires -IPA for linking
dnl
if test "${DEBUG}" = yes ; then
DYNAROPTS="-IPA ${DYNAROPTS}"
fi

AC_DEFINE(MIPS,)
AC_DEFINE(IRIX,)
])

AC_DEFUN([CF_IDENTIFY_SUN], [
AC_MSG_CHECKING(for SUN WorkShop/Forte C++ compiler)
SUN_CXX=`${CXX} -V 2>&1 | ${EGREP} -e "(Sun)|(WorkShop)|(Forte)"`
if test "${SUN_CXX}" != ""; then
IS_SUNCC=true
AC_MSG_RESULT(yes)
CXX_NAME="SunCC"
CXX_IDENTIFIED=true

dnl 
dnl 	Define a symbol for SUNPro C++.
dnl
AC_DEFINE(PROJECT[]_COMPILER_SUNPRO)
AC_DEFINE(PROJECT[]_COMPILER, SUNPRO)
else
IS_SUNCC=false
AC_MSG_RESULT(no)
fi
])

AC_DEFUN([CF_SUNCC_OPTIONS], [

AC_MSG_CHECKING(compiler version for Sun C++)
CXX_VERSION=`echo ${SUN_CXX} | ${SED} "s/^.*C++ //" | ${CUT} -d" " -f1`
changequote(<<,>>)
CXX_VERSION_TEST=`echo ${CXX_VERSION} | ${SED} s/\^\\[0-9\\.\\]*[a-zA-Z\\.]*//g`
changequote([,])
if test "${CXX_VERSION_TEST}" != "" ; then
CF_BASENAME(${CXX})
CXX_VERSION="${TMP__NAME}"
CXX_VERSION_OUTPUT="${CXX_VERSION} (unknown version)"
else
CXX_VERSION_OUTPUT="${CXX_VERSION}"
CF_DIGEST_CXX_VERSION	
fi
AC_MSG_RESULT(${CXX_VERSION_OUTPUT})

dnl
dnl   Make sure we use at least Workshop 6U2 (C++ 5.3)
dnl   (SUNPro < 6 is a mess - hasn't even heard of ANSI C++!)
dnl
if test "${CXX_VERSION_1}" -lt 5 ; then
AC_MSG_RESULT()
AC_MSG_RESULT(PROJECT[] requires an ANSI C++ compliant compiler)
AC_MSG_RESULT(SUNPro compilers are (mostly) ANSI compliant for version 5.3 and above)
AC_MSG_RESULT(Please upgrade your compiler!)
CF_ERROR
fi

AC_DEFINE(SOLARIS,)

TEMPLATE_DIR="SunWS_cache"

dnl  set the default binary format (if none selected)
dnl
if test "${BINFMT_64_BIT}" = true ; then
	SUN_BINFMT=V9
	CXX_NAME="${CXX_NAME}_V9"
	LDFLAGS="${LDFLAGS} -xarch=v9"
	CXXFLAGS="${CXXFLAGS} -xarch=v9"
	AROPTS="${AROPTS} -xarch=v9"
	DYNAROPTS="-xarch=v9 ${DYNAROPTS}"
else
	SUN_BINFMT=V8
	CXX_NAME="${CXX_NAME}_V8"
fi

DEF_BOOL=true
AR="${CXX}"
DYNAR="${CXX}"
AROPTS="${AROPTS} -xar -KPIC -o"
DYNAROPTS="${DYNAROPTS} -pto -G -KPIC -o"
NONLIB_CXXFLAGS="-pto"
LIB_CXXFLAGS=""
CXX_MAKEDEPEND="${CXX}"
MAKEDEP_CXX_OPTS="-xM1"
MAKEDEP_CXX_SUFFIX=" >.Dependencies"

AC_DEFINE(PROJECT[]_NO_INLINE_FUNCTIONS,)

CXXFLAGS="${CXXFLAGS} -KPIC"
CXXFLAGS_O="${CXXFLAGS_O} -xO5"
CXXFLAGS_D="${CXXFLAGS_D}"
CXXFLAGS_DI="${CXXFLAGS_DI} -g"
])

dnl
dnl   Assemble the complete compiler name by adding
dnl   the release numbers (if known) of the compiler
dnl
AC_DEFUN([CF_BUILD_FULL_CXX_NAME], [
AC_MSG_CHECKING(standardized compiler name)
if test "${CXX_VERSION_1}" != "" ; then
CXX_NAME="${CXX_NAME}_${CXX_VERSION_1}"
if test "${CXX_VERSION_2}" != "" ; then
CXX_NAME="${CXX_NAME}.${CXX_VERSION_2}"
if test "${CXX_VERSION_3}" != "" ; then
CXX_NAME="${CXX_NAME}.${CXX_VERSION_3}"
if test "${CXX_VERSION_4}" != "" ; then
	CXX_NAME="${CXX_NAME}.${CXX_VERSION_4}"
fi
fi
fi
fi

AC_MSG_RESULT(${CXX_NAME})
])

dnl
dnl
dnl   checking for DEBUG-Flag
dnl
AC_DEFUN([CF_CHECK_DEBUG_FLAG], [
AC_MSG_CHECKING(for DEBUG flag)
dnl   define a debug flag and prevent the compilation of
dnl   inline functions by defining PROJECT[]_NO_INLINE_FUNCTIONS
dnl   (see COMMON/debug.h)
if test "$DEBUG" = "true" ; then
	dnl  if debug information is also required, add the corresponding flag
	dnl
	if test "${DEBUG_INFO}" = true -a "$CXXFLAGS_DI" != "" ; then
		CXXFLAGS_D="${CXXFLAGS_D} ${CXXFLAGS_DI}"
	fi
	AC_DEFINE(PROJECT[]_DEBUG,)
	AC_DEFINE(PROJECT[]_NO_INLINE_FUNCTIONS,)
	AC_MSG_RESULT(enabled)
	CPP_MODE_FLAGS="${CXXFLAGS_D}"
	CPP_MODE_FLAGS_NO_OPTIMIZATION="${CXXFLAGS_D}"
else
	AC_MSG_RESULT(disabled)
	dnl Even with optimization enabled, we still 
	dnl might want to add debug information
	if test "${DEBUG_INFO}" = true -a "$CXXFLAGS_DI" != "" ; then
		CXXFLAGS_O="${CXXFLAGS_O} ${CXXFLAGS_DI}"
	fi
	CPP_MODE_FLAGS="${CXXFLAGS_O}"
	CPP_MODE_FLAGS_NO_OPTIMIZATION=""
fi
])

dnl
dnl   check for endianness of the architecture
dnl
dnl
AC_DEFUN([CF_C_BIGENDIAN], [
AC_MSG_CHECKING(for byte order)
AC_TRY_RUN(
[
#include <iostream>
#include <fstream>
int main(int, char**)
{
] ${PROJECT[]_SIZE_TYPE} endian_one = 1; [
std::ofstream os("config.endian.log", std::ios::out);


if (*(char*)&endian_one == '\001')
{
// big endian
os << "LITTLE" << std::endl;
}
else
{
// little endian
os << "BIG" << std::endl;
}
os.close();

return 0;
}
],
PROJECT[]_ENDIAN_TEST=true,
DUMMY=0,
DUMMY=0
)
if test "${PROJECT[]_ENDIAN_TEST+set}" != set ; then
AC_MSG_RESULT(<cannot determine>)
CF_ERROR
else
dnl
dnl read the result of the endian test from the file
dnl and delete the file
dnl
ENDIAN_TYPE=`${CAT} config.endian.log`
${RM} config.endian.log 2>/dev/null
if test "${ENDIAN_TYPE}" = "LITTLE" ; then
PROJECT[]_LITTLE_ENDIAN=true
AC_DEFINE(PROJECT[]_LITTLE_ENDIAN, true)
AC_MSG_RESULT(little endian)
else
if test "${ENDIAN_TYPE}" = "BIG" ; then
PROJECT[]_BIG_ENDIAN=true
AC_DEFINE(PROJECT[]_BIG_ENDIAN, true)
AC_MSG_RESULT(big endian)
else
AC_MSG_RESULT(<cannot determine>)
CF_ERROR
fi
fi
fi
])

dnl
dnl   check for limits header (class numeric limits, to be precise)
dnl
dnl
AC_DEFUN([CF_CHECK_NUM_LIMITS], [
AC_MSG_CHECKING(for numeric_limits class)
AC_TRY_COMPILE(
[
#include <limits>
],
[
float f = std::numeric_limits<float>::min();
],
PROJECT[]_HAS_NUMERIC_LIMITS=true
)
if test "${HAS_NUMERIC_LIMITS}" = true ; then
AC_MSG_RESULT(available)
AC_DEFINE(PROJECT[]_HAS_NUMERIC_LIMITS)
else
AC_MSG_RESULT(not available)
PROJECT[]_HAS_NUMERIC_LIMITS=false

dnl
dnl  we didn't find a numeric limits class, so we implement
dnl  it by ourselves. Try to figure out whether we only need
dnl  limts.h or also float.h (sometimes FLT_MIN seems to be missing)
dnl
AC_MSG_CHECKING(whether float.h has to be included)
AC_TRY_COMPILE(
[
#include <limits.h>
],
[
float a = FLT_MAX;
float b = FLT_MIN;
float c = DBL_MAX;
float d = DBL_MIN;
],
PROJECT[]_HAS_FLOAT_H=false
)
if test "${PROJECT[]_HAS_FLOAT_H}" != false ; then
AC_TRY_COMPILE(
[
	#include <float.h>
],
[
	float a = FLT_MAX;
	float b = FLT_MIN;
	float c = DBL_MAX;
	float d = DBL_MIN;
],
PROJECT[]_HAS_FLOAT_H=true
)
fi
if test "${PROJECT[]_HAS_FLOAT_H+set}" != set ; then
AC_MSG_RESULT()
AC_MSG_RESULT(limits.h seems to be corrupt or float.h is missing!)
AC_MSG_RESULT()
else
if test "${PROJECT[]_HAS_FLOAT_H}" = true ; then
AC_MSG_RESULT(yes)
AC_DEFINE(PROJECT[]_HAS_FLOAT_H)
else
AC_MSG_RESULT(no)
fi
fi
fi
${RM} /tmp/$$.conftest.c a.out 2>/dev/null
])


dnl
dnl   check for template arguments needed for friends of template
dnl   classses. Some compilers require "<>" after the method name,
dnl   others don't - so let's find it out!
dnl
AC_DEFUN([CF_CHECK_TPL_NULL_ARGS], [
AC_MSG_CHECKING(for null template arguments)
PROJECT[]_NULL_TEMPLATE_ARGS="NULL"
AC_TRY_COMPILE(
[
template <typename T>
class A
{
public:
friend bool operator == <> (const A&, const A&);
};
],
[
],
PROJECT[]_NULL_TEMPLATE_ARGS="<>")
if test "${PROJECT[]_NULL_TEMPLATE_ARGS}" = "NULL" ; then
AC_TRY_COMPILE(
[
template <typename T>
class A
{
	public:
	friend bool operator == (const A&, const A&);
};
],
[
],
PROJECT[]_NULL_TEMPLATE_ARGS="")
fi
AC_MSG_RESULT(\"$PROJECT[]_NULL_TEMPLATE_ARGS\")
if test "${PROJECT[]_NULL_TEMPLATE_ARGS}" = "NULL" ; then
AC_MSG_RESULT(could not find a suitable argument for null templates)
CF_ERROR
fi
])

dnl
dnl		Check whether the compiler allows parameterization oftemplate functions
dnl		with inline functions (SGI CC has a problem with that)
dnl
AC_DEFUN([CF_CHECK_INLINE_TPL_ARGS], [
AC_MSG_CHECKING(for inline template function arguments)
PROJECT[]_HAS_INLINE_TPL_ARGS=no
AC_TRY_COMPILE(
[
template <int i>
inline double foo(double x){ return i * x; }

typedef double (*Function)(double);

template <Function F>
inline double bar(double x) { return F(x); }
],
[
double d = bar< foo<3> >(2.0);
],
PROJECT[]_HAS_INLINE_TPL_ARGS=true
)
if test "${PROJECT[]_HAS_INLINE_TPL_ARGS}" = true ; then
AC_MSG_RESULT(yes)
AC_DEFINE(PROJECT[]_HAS_INLINE_TPL_ARGS)
else
AC_MSG_RESULT(no)
fi
])

dnl
dnl   check for ANSI compliant <iostream>
dnl   We need this for the base classes (ios vs. basic_ios<char>) in socket.h/C
dnl
AC_DEFUN([CF_CHECK_ANSI_IOSTREAM], [
AC_MSG_CHECKING(for ANSI compliant iostream)
PROJECT[]_HAS_ANSI_IOSTREAM=no
AC_TRY_COMPILE(
[
#include <iostream>
class A:public std::iostream
{
A():std::basic_ios<char>(0),std::iostream(0)
{}
};
],
[
],
PROJECT[]_HAS_ANSI_IOSTREAM=yes
)
AC_MSG_RESULT($PROJECT[]_HAS_ANSI_IOSTREAM)
])

dnl
dnl		check if we can use <sstream> or if we need to support old
dnl		style strstream
dnl
AC_DEFUN([CF_CHECK_HAS_SSTREAM], [
AC_MSG_CHECKING(for sstream headers)
PROJECT[]_HAS_SSTREAM=no
AC_TRY_COMPILE(
[
#include <sstream>
class A : public std::stringbuf
{
A() : std::stringbuf()
{}
};
],
[
],
PROJECT[]_HAS_SSTREAM=yes
)
AC_MSG_RESULT($PROJECT[]_HAS_SSTREAM)
])

dnl
dnl   check for ANSI or ARM style access modification
dnl   either (ARM style) Base::foo or (ANSI style) using Base::foo
dnl
AC_DEFUN([CF_CHECK_ARM_ACCESS_MODIFICATION], [
AC_MSG_CHECKING(for ANSI or ARM style access modification)
PROJECT[]_CFG_USING_METHOD_DIRECTIVE=none
AC_TRY_COMPILE(
[
class A
{
protected: void foo(){};
};

class B : public A
{
public: using A::foo;
};
],
[
B b;
b.foo();
],
PROJECT[]_CFG_USING_METHOD_DIRECTIVE=ANSI
)
if test ${PROJECT[]_CFG_USING_METHOD_DIRECTIVE} = none ; then
AC_TRY_COMPILE(
[
class A
{
	protected: void foo(){};
};

class B : public A
{
	public: A::foo;
};
],
[
B b;
b.foo();
],
PROJECT[]_CFG_USING_METHOD_DIRECTIVE=ARM
)
fi
AC_MSG_RESULT(${PROJECT[]_CFG_USING_METHOD_DIRECTIVE})
if test ${PROJECT[]_CFG_USING_METHOD_DIRECTIVE} = ANSI ; then
AC_DEFINE(PROJECT[]_CFG_USING_METHOD_DIRECTIVE)
fi
if test ${PROJECT[]_CFG_USING_METHOD_DIRECTIVE} = none ; then
AC_MSG_RESULT()
AC_MSG_RESULT([Compiler does not understand ARM or ANSI style method access modification.])
AC_MSG_RESULT([Please specify a different compiler (e.g. g++ 2.95.2) using the option])
AC_MSG_RESULT([--with-compiler=<compiler>.])
CF_ERROR
fi
])

dnl
dnl	Check for the sizes of the tzpes and define a set of portable
dnl		types of different word lengths.
dnl	This might profit very nicely from stdint.h at some point.
dnl
AC_DEFUN([CF_GET_TYPE_SIZES], [
dnl
dnl   check for the size of int and pointers (may cause trouble on 64 bit architectures)
dnl   we define the type PointerInt (in COMMON/global.h) according to the macro
dnl   PROJECT[]_POINTERSIZE_UINT (which is set here)
dnl   We also define a 64 bit unsigned numeric type. All pointers that are read or written
dnl   in persistence-related methods use this type to ensure compatibility between 32 and
dnl   64bit PROJECT[] versions.
dnl   missing: usage of the result of AC_TYPE_SIZE_T
dnl
AC_CHECK_SIZEOF(char, 4)
AC_CHECK_SIZEOF(int, 4)
AC_CHECK_SIZEOF(long, 4)
AC_CHECK_SIZEOF(long long, 8)
AC_CHECK_SIZEOF(size_t, 4)
AC_CHECK_SIZEOF(void*, 4)
SIZEOF_INT=$ac_cv_sizeof_int
SIZEOF_CHAR=$ac_cv_sizeof_char
SIZEOF_LONG=$ac_cv_sizeof_long
SIZEOF_SIZE_T=$ac_cv_sizeof_size_t
SIZEOF_VOID_P=$ac_cv_sizeof_voidp
SIZEOF_UINT=$ac_cv_sizeof_int
SIZEOF_ULONG=$ac_cv_sizeof_long
SIZEOF_ULONGLONG=$ac_cv_sizeof_long_long

AC_DEFINE_UNQUOTED(PROJECT[]_CHAR_SIZE, ${SIZEOF_CHAR})
AC_DEFINE_UNQUOTED(PROJECT[]_INT_SIZE, ${SIZEOF_INT})
AC_DEFINE_UNQUOTED(PROJECT[]_LONG_SIZE, ${SIZEOF_LONG})
AC_DEFINE_UNQUOTED(PROJECT[]_SIZE_T_SIZE, ${SIZEOF_SIZE_T})
AC_DEFINE_UNQUOTED(PROJECT[]_POINTER_SIZE, ${SIZEOF_VOID_P})
AC_DEFINE_UNQUOTED(PROJECT[]_UINT_SIZE, ${SIZEOF_UINT})
AC_DEFINE_UNQUOTED(PROJECT[]_ULONG_SIZE, ${SIZEOF_ULONG})
AC_DEFINE_UNQUOTED(PROJECT[]_ULONGLONG_SIZE, ${SIZEOF_ULONGLONG})
dnl
dnl  define an unsigned type that can hold 64 bit pointers
dnl
if test "${SIZEOF_UINT}" = 8; then
PROJECT[]_64BIT_UINT="unsigned int"
else
if test "${SIZEOF_ULONG}" = 8; then
PROJECT[]_64BIT_UINT="unsigned long"
else
if test "${SIZEOF_ULONGLONG}" = 8 ; then
PROJECT[]_64BIT_UINT="unsigned long long"
else
AC_MSG_RESULT()
AC_MSG_RESULT(cannot find appropriate numeric type for 64bit unsigned int)
CF_ERROR
fi
fi
fi
AC_DEFINE_UNQUOTED(PROJECT[]_64BIT_UINT_TYPE, ${PROJECT[]_64BIT_UINT})

dnl
dnl define a 32 bit type for Size and Index
dnl
if test "${SIZEOF_UINT}" = "${SIZEOF_VOID_P}" ; then
PROJECT[]_POINTER_TYPE="unsigned int"
else
if test "${SIZEOF_ULONG}" = "${SIZEOF_VOID_P}" ; then
PROJECT[]_POINTER_TYPE="unsigned long"
else
AC_MSG_RESULT()
AC_MSG_RESULT(cannot find appropriate integer type of same size as void*)
CF_ERROR
fi
fi
AC_DEFINE_UNQUOTED(PROJECT[]_POINTERSIZEUINT_TYPE, ${PROJECT[]_POINTER_TYPE})

dnl
dnl define a (true) pointer size int for several conversion issues
dnl (e.g. PersistenceManager) - just a define, no real typedef
dnl since for internal use only!
dnl
if test "${SIZEOF_INT}" = 4 ; then
PROJECT[]_INDEX_TYPE="int"
PROJECT[]_SIZE_TYPE="unsigned int"
else
if test "${SIZEOF_LONG}" = 4 ; then
PROJECT[]_INDEX_TYPE="long"
PROJECT[]_SIZE_TYPE="unsigned long"
else
AC_MSG_RESULT()
AC_MSG_RESULT(cannot find appropriate numeric type for 32bit int)
CF_ERROR
fi
fi
AC_DEFINE_UNQUOTED(PROJECT[]_SIZE_TYPE, ${PROJECT[]_SIZE_TYPE})
AC_DEFINE_UNQUOTED(PROJECT[]_INDEX_TYPE, ${PROJECT[]_INDEX_TYPE})

dnl  define 64 bit signed/unsigned type
if test "${SIZEOF_ULONG}" = "8" ; then
PROJECT[]_ULONG64_TYPE="unsigned long"
PROJECT[]_LONG64_TYPE="long"
AC_DEFINE(PROJECT[]_64BIT_ARCHITECTURE)
else
if test "${SIZEOF_ULONGLONG}" = "8" ; then
PROJECT[]_ULONG64_TYPE="unsigned long long"
PROJECT[]_LONG64_TYPE="long long"			
else
AC_MSG_RESULT()
AC_MSG_RESULT(cannot find unsigned 64bit type.)
CF_ERROR
fi
fi
AC_DEFINE_UNQUOTED(PROJECT[]_ULONG64_TYPE, ${PROJECT[]_ULONG64_TYPE})
AC_DEFINE_UNQUOTED(PROJECT[]_LONG64_TYPE, ${PROJECT[]_LONG64_TYPE})

dnl
dnl Check for size of Complex type
dnl
PROJECT[]_COMPLEX_PRECISION=float
AC_MSG_CHECKING(for Complex type precision)
if test "${enable_double_cplx}" = yes ; then
PROJECT[]_COMPLEX_PRECISION=double
fi
if test "${enable_longdbl_cplx}" = yes ; then
PROJECT[]_COMPLEX_PRECISION="long double"
fi
AC_DEFINE_UNQUOTED(PROJECT[]_COMPLEX_PRECISION, ${PROJECT[]_COMPLEX_PRECISION})
AC_MSG_RESULT(${PROJECT[]_COMPLEX_PRECISION})
])


dnl
dnl   check whether the <regex.h> header exists
dnl
AC_DEFUN([CF_CHECK_REGEX_H], [
AC_CHECK_HEADER(regex.h, HAS_REGEX_H=true, HAS_REGEX_H=false)
if test "${HAS_REGEX_H}" = "false" ; then
AC_CHECK_HEADER(regexp.h, HAS_REGEX_H=true, HAS_REGEX_H=false)
AC_DEFINE(PROJECT[]_HAS_REGEXP_H,)
else
AC_DEFINE(PROJECT[]_HAS_REGEX_H,)
fi
if test "${HAS_REGEX_H}" = "false" ; then
AC_MSG_RESULT()
AC_MSG_RESULT([Regular expression headers regex.h not found!])
AC_MSG_RESULT([If you do not have this header on your system,])
AC_MSG_RESULT([please install the GNU regexp package from])
AC_MSG_RESULT()
AC_MSG_RESULT([  ftp://ftp.gnu.org/gnu/regex/regex-0.12.tar.gz])
CF_ERROR
fi
])

dnl
dnl   Check whether ieeefp.h does really exist.
dnl
AC_DEFUN([CF_CHECK_IEEEFP_H], [
AC_CHECK_HEADERS(ieeefp.h,
[PROJECT[]_HAS_IEEEFP_H=true],
[PROJECT[]_HAS_IEEEFP_H=false])
if test ${PROJECT[]_HAS_IEEEFP_H} = true ; then
AC_DEFINE(PROJECT[]_HAS_IEEEFP_H,)
fi
])

dnl
dnl   check for ISO C99 stdint.h
dnl
AC_DEFUN([CF_CHECK_STDINT_H], [
AC_CHECK_HEADERS(stdint.h,
[PROJECT[]_HAS_STDINT_H=true],
[PROJECT[]_HAS_STDINT_H=false])
if test ${PROJECT[]_HAS_STDINT_H} = true ; then
AC_DEFINE(PROJECT[]_HAS_STDINT_H,)
fi
])

dnl
dnl   check whether values.h does really exist
dnl
AC_DEFUN([CF_CHECK_VALUES_H], [
AC_CHECK_HEADERS(values.h,
[PROJECT[]_HAS_VALUES_H=true],
[PROJECT[]_HAS_VALUES_H=false])
if test ${PROJECT[]_HAS_VALUES_H} = true ; then
AC_DEFINE(PROJECT[]_HAS_VALUES_H,)
fi
])

dnl
dnl   Check whether unistd.h does really exist.
dnl
AC_DEFUN([CF_CHECK_UNISTD_H], [
AC_CHECK_HEADERS(unistd.h,
[PROJECT[]_HAS_UNISTD_H=true],
[PROJECT[]_HAS_UNISTD_H=false])
if test ${PROJECT[]_HAS_UNISTD_H} = true ; then
AC_DEFINE(PROJECT[]_HAS_UNISTD_H,)
fi
])

dnl
dnl   Check whether limits.h does really exist.
dnl
AC_DEFUN([CF_CHECK_LIMITS_H], [
AC_CHECK_HEADERS(limits.h,
[PROJECT[]_HAS_LIMITS_H=true],
[PROJECT[]_HAS_LIMITS_H=false])
if test ${PROJECT[]_HAS_LIMITS_H} = true ; then
AC_DEFINE(PROJECT[]_HAS_LIMITS_H,)
fi
])

dnl
dnl   Check whether process.h does really exist.
dnl
AC_DEFUN([CF_CHECK_PROCESS_H], [
AC_CHECK_HEADERS(process.h,
[PROJECT[]_HAS_PROCESS_H=true],
[PROJECT[]_HAS_PROCESS_H=false])
if test ${PROJECT[]_HAS_PROCESS_H} = true ; then
AC_DEFINE(PROJECT[]_HAS_PROCESS_H,)
fi
])

dnl
dnl   Check whether sys/time.h does really exist.
dnl
AC_DEFUN([CF_CHECK_SYS_TIME_H], [
AC_CHECK_HEADERS(sys/time.h,
[PROJECT[]_HAS_SYS_TIME_H=true],
[PROJECT[]_HAS_SYS_TIME_H=false])
if test ${PROJECT[]_HAS_SYS_TIME_H} = true ; then
AC_DEFINE(PROJECT[]_HAS_SYS_TIME_H,)
fi
])

dnl
dnl   Check whether sys/stat.h does really exist.
dnl
AC_DEFUN([CF_CHECK_SYS_STAT_H], [
AC_CHECK_HEADERS(sys/stat.h,
[PROJECT[]_HAS_SYS_STAT_H=true],
[PROJECT[]_HAS_SYS_STAT_H=false])
if test ${PROJECT[]_HAS_SYS_STAT_H} = true ; then
AC_DEFINE(PROJECT[]_HAS_SYS_STAT_H,)
fi
])

dnl
dnl   Check whether sys/times.h does really exist.
dnl
AC_DEFUN([CF_CHECK_SYS_TIMES_H], [
AC_CHECK_HEADERS(sys/times.h,
[PROJECT[]_HAS_SYS_TIMES_H=true],
[PROJECT[]_HAS_SYS_TIMES_H=false])
if test ${PROJECT[]_HAS_SYS_TIMES_H} = true ; then
AC_DEFINE(PROJECT[]_HAS_SYS_TIMES_H,)
fi
])

dnl
dnl   Check whether sys/types.h does really exist.
dnl
AC_DEFUN([CF_CHECK_SYS_TYPES_H], [
AC_CHECK_HEADERS(sys/types.h,
[PROJECT[]_HAS_SYS_TYPES_H=true],
[PROJECT[]_HAS_SYS_TYPES_H=false])
if test ${PROJECT[]_HAS_SYS_TYPES_H} = true ; then
AC_DEFINE(PROJECT[]_HAS_SYS_TYPES_H,)
fi
])

dnl
dnl   Check whether sys/ioctl.h does really exist.
dnl
AC_DEFUN([CF_CHECK_SYS_IOCTL_H], [
AC_CHECK_HEADERS(sys/ioctl.h,
[PROJECT[]_HAS_SYS_IOCTL_H=true],
[PROJECT[]_HAS_SYS_IOCTL_H=false])
if test ${PROJECT[]_HAS_SYS_IOCTL_H} = true ; then
AC_DEFINE(PROJECT[]_HAS_SYS_IOCTL_H,)
fi
])

dnl
dnl   Check whether time.h does really exist.
dnl
AC_DEFUN([CF_CHECK_TIME_H], [
AC_CHECK_HEADERS(time.h,
[PROJECT[]_HAS_TIME_H=true],
[PROJECT[]_HAS_TIME_H=false])
if test ${PROJECT[]_HAS_TIME_H} = true ; then
AC_DEFINE(PROJECT[]_HAS_TIME_H,)
fi
])

dnl
dnl   Check whether sys/param.h does really exist.
dnl
AC_DEFUN([CF_CHECK_SYS_PARAM_H], [
AC_CHECK_HEADERS(sys/param.h,
[PROJECT[]_HAS_SYS_PARAM_H=true],
[PROJECT[]_HAS_SYS_PARAM_H=false])
if test ${PROJECT[]_HAS_SYS_PARAM_H} = true ; then
AC_DEFINE(PROJECT[]_HAS_SYS_PARAM_H,)
fi
])

dnl
dnl   Check whether dirent.h does really exist.
dnl
AC_DEFUN([CF_CHECK_DIRENT_H], [
AC_CHECK_HEADERS(dirent.h,
[PROJECT[]_HAS_DIRENT_H=true],
[PROJECT[]_HAS_DIRENT_H=false])
if test ${PROJECT[]_HAS_DIRENT_H} = true ; then
AC_DEFINE(PROJECT[]_HAS_DIRENT_H,)
fi
])

dnl
dnl   Check whether pwd.h does really exist.
dnl
AC_DEFUN([CF_CHECK_PWD_H], [
AC_CHECK_HEADERS(pwd.h,
[PROJECT[]_HAS_PWD_H=true],
[PROJECT[]_HAS_PWD_H=false])
if test ${PROJECT[]_HAS_PWD_H} = true ; then
AC_DEFINE(PROJECT[]_HAS_PWD_H,)
fi
])

dnl
dnl   Check whether direct.h does really exist.
dnl
AC_DEFUN([CF_CHECK_DIRECT_H], [
AC_CHECK_HEADERS(direct.h,
[PROJECT[]_HAS_DIRECT_H=true],
[PROJECT[]_HAS_DIRECT_H=false])
if test ${PROJECT[]_HAS_DIRECT_H} = true ; then
AC_DEFINE(PROJECT[]_HAS_DIRECT_H,)
fi
])

dnl
dnl   Check whether io.h does really exist.
dnl
AC_DEFUN([CF_CHECK_IO_H], [
AC_CHECK_HEADERS(io.h,
[PROJECT[]_HAS_IO_H=true],
[PROJECT[]_HAS_IO_H=false])
if test ${PROJECT[]_HAS_IO_H} = true ; then
AC_DEFINE(PROJECT[]_HAS_IO_H,)
fi
])

dnl
dnl   Check whether sys/socket.h does really exist.
dnl
AC_DEFUN([CF_CHECK_SYS_SOCKET_H], [
AC_CHECK_HEADERS(sys/socket.h,
[PROJECT[]_HAS_SYS_SOCKET_H=true],
[PROJECT[]_HAS_SYS_SOCKET_H=false])
if test ${PROJECT[]_HAS_SYS_SOCKET_H} = true ; then
AC_DEFINE(PROJECT[]_HAS_SYS_SOCKET_H,)
fi
])

dnl
dnl   Check whether netinet/in.h does really exist.
dnl
AC_DEFUN([CF_CHECK_NETINET_IN_H], [
AC_CHECK_HEADERS(netinet/in.h,
[PROJECT[]_HAS_NETINET_IN_H=true],
[PROJECT[]_HAS_NETINET_IN_H=false])
if test ${PROJECT[]_HAS_NETINET_IN_H} = true ; then
AC_DEFINE(PROJECT[]_HAS_NETINET_IN_H,)
fi
])

dnl
dnl   Check whether netdb.h does really exist.
dnl
AC_DEFUN([CF_CHECK_NETDB_H], [
AC_CHECK_HEADERS(netdb.h,
[PROJECT[]_HAS_NETDB_H=true],
[PROJECT[]_HAS_NETDB_H=false])
if test ${PROJECT[]_HAS_NETDB_H} = true ; then
AC_DEFINE(PROJECT[]_HAS_NETDB_H,)
fi
])

dnl
dnl   Check whether arpa/inet.h does really exist.
dnl
AC_DEFUN([CF_CHECK_ARPA_INET_H], [
AC_CHECK_HEADERS(arpa/inet.h,
[PROJECT[]_HAS_ARPA_INET_H=true],
[PROJECT[]_HAS_ARPA_INET_H=false])
if test ${PROJECT[]_HAS_ARPA_INET_H} = true ; then
AC_DEFINE(PROJECT[]_HAS_ARPA_INET_H,)
fi
])

dnl
dnl   Check whether sys/sysinfo.h does exist.
dnl
AC_DEFUN([CF_CHECK_SYS_SYSINFO_H], [
AC_CHECK_HEADERS(sys/sysinfo.h,
[PROJECT[]_HAS_SYS_SYSINFO_H=true],
[PROJECT[]_HAS_SYS_SYSINFO_H=false])
if test ${PROJECT[]_HAS_SYS_SYSINFO_H} = true ; then
AC_DEFINE(PROJECT[]_HAS_SYS_SYSINFO_H,)
fi
])

AC_DEFUN([CF_CHECK_SYSCONF], [
AC_CHECK_FUNCS(sysconf, HAS_SYSCONF=1)
if test "${HAS_SYSCONF}" = 1 ; then
AC_DEFINE(PROJECT[]_HAS_SYSCONF,)
fi
])

AC_DEFUN([CF_CHECK_KILL], [
AC_CHECK_FUNCS(kill, HAS_KILL=1)
if test "${HAS_KILL}" = 1 ; then
AC_DEFINE(PROJECT[]_HAS_KILL,)
fi
])

AC_DEFUN([CF_CHECK_HYPOT], [
AC_CHECK_FUNCS(kill, HAS_HYPOT=1)
if test "${HAS_HYPOT}" = 1 ; then
AC_DEFINE(PROJECT[]_HAS_HYPOT,)
fi
])

dnl
dnl   check whether vsnprintf is defined
dnl
AC_DEFUN([CF_CHECK_VSNPRINTF], [
	AC_CHECK_FUNCS(vsnprintf, HAVE_VSNPRINTF=1)
	if test "${HAVE_VSNPRINTF}" = 1 ; then
		dnl
		dnl   check whether vsnprintf works as expected
		dnl   on Solaris 2.x it is broken in the 64bit version
		dnl
		AC_TRY_RUN(
[
#include <stdlib.h>
int main()
{
	char* buffer[50];
	vsnprintf(buffer, "%1s", "TEST");

	if (!strcmp(buffer, "T"))
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
],
			VSNPRINTF_OK=1,
			DUMMY=0,
			DUMMY=0
		)

		if test "${VSNPRINTF_OK}" = 1 ; then
			AC_DEFINE(PROJECT[]_HAVE_VSNPRINTF)
		fi
	fi
])


dnl
dnl   check whether we need sysinfo or gethostname
dnl
AC_DEFUN([CF_CHECK_GETHOSTNAME], [
	AC_CHECK_FUNCS(gethostname, HAVE_GETHOSTNAME=1)
	if test "${HAVE_GETHOSTNAME}" = 1 ; then
		AC_DEFINE(PROJECT[]_HAVE_GETHOSTNAME)
	else
		AC_CHECK_FUNCS(sysinfo, HAVE_SYSINFO=1)
		if test "${HAVE_SYSINFO}" = 1  ; then
			AC_DEFINE(PROJECT[]_HAVE_SYSINFO)
		else
			AC_MSG_RESULT()
			AC_MSG_RESULT([Could not find gethostname or sysinfo methods!])
			AC_MSG_RESULT([Please refer to config.log to identify the problem.])
			CF_ERROR
		fi
	fi

	dnl
	dnl check for gethostname in the header
	dnl
	if test "${HAVE_GETHOSTNAME}" = 1 ; then
		AC_MSG_CHECKING(for gethostname in unistd.h)
		AC_TRY_COMPILE(
		[
		#include <unistd.h>
		],
		[
		char name[1024];
		gethostname(name, 1023);
		],
		HAVE_GETHOSTNAME_HEADER=1
		)
		if test "${HAVE_GETHOSTNAME_HEADER+set}" != set ; then
			AC_MSG_RESULT(no)
			AC_DEFINE(PROJECT[]_DEFINE_GETHOSTNAME)
		else
			AC_MSG_RESULT(yes)
		fi
	fi
])

AC_DEFUN([CF_CHECK_NETLIBS], [
dnl
dnl   first check if everythings already defined in libc
dnl
AC_CHECK_FUNCS(inet_addr, HAVE_INET_ADDR=1)
AC_CHECK_FUNCS(gethostbyname, HAVE_GETHOSTBYNAME=1)
if test "${HAVE_INET_ADDR+set}" = set ; then
AC_CHECK_FUNC(inet_aton, HAVE_INET_ATON=1)
fi

dnl   if gethostbyname was not defined in libc, try libxnet (Solaris only?)
if test "${HAVE_GETHOSTBYNAME+set}" != set -a "${USE_LIBXNET}" != false; then
AC_CHECK_LIB(xnet, gethostbyname)
unset ac_cv_func_gethostbyname
AC_CHECK_FUNCS(gethostbyname,HAVE_GETHOSTBYNAME=1)
fi
if test "${HAVE_INET_ADDR+set}" != set ; then
unset ac_cv_func_inet_addr
AC_CHECK_FUNCS(inet_addr,HAVE_INET_ADDR=1)
if test "${HAVE_INET_ADDR+set}" != set -a "${USE_LIBXNET}" != false; then
AC_CHECK_LIB(xnet, inet_addr)
unset ac_cv_func_inet_addr
AC_CHECK_FUNCS(inet_addr,HAVE_INET_ADDR=1)
fi
fi

if test "${HAVE_GETHOSTBYNAME+set}" != set ; then
AC_CHECK_LIB(nsl, gethostbyname)
unset ac_cv_func_gethostbyname
AC_CHECK_FUNCS(gethostbyname,HAVE_GETHOSTBYNAME=1)
fi

if test "${HAVE_INET_ADDR+set}" != set ; then
AC_CHECK_LIB(socket, inet_addr)
unset ac_cv_func_inet_addr
AC_CHECK_FUNCS(inet_addr,HAVE_INET_ADDR=1)
fi


dnl check again whether inet_aton exists (perhaps it was hidden in one
dnl of the other libraries..
if test "${HAVE_INET_ATON+set}" != set ; then
unset ac_cv_func_inet_aton
AC_CHECK_FUNC(inet_aton,HAVE_INET_ATON=1)
fi

if test "${HAVE_INET_ATON+set}" = set ; then
AC_DEFINE(HAVE_INET_ATON,)
fi

])

dnl
dnl   Check whether size arguments
dnl   in function calls like getsockname getpeername or accept
dnl   require a specialized typename or int
dnl   We simply compile a short example with all known types
dnl   and take one that didn't cause a warning (or an error)
dnl
AC_DEFUN([CF_CHECK_SOCKET_ARGS_AND_TYPES], [
AC_MSG_CHECKING(for socketlen type)
AC_TRY_COMPILE(
[
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
],
[ sockaddr_in   addr;
socklen_t     len = 0;
getsockname(0, (struct sockaddr*)&addr, &len);
],
PROJECT[]_SOCKLEN_TYPE=socklen_t)
if test "${PROJECT[]_SOCKLEN_TYPE}" = "" ; then
AC_TRY_COMPILE(
[
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
],
[ sockaddr_in   addr;
size_t    len = 0;
getsockname(0, (struct sockaddr*)&addr, &len);
],
PROJECT[]_SOCKLEN_TYPE=size_t)
fi
if test "${PROJECT[]_SOCKLEN_TYPE}" = "" ; then
AC_TRY_COMPILE(
[
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
],
[ sockaddr_in   addr;
unsigned int  len = 0;
getsockname(0, (struct sockaddr*)&addr, &len);
],
PROJECT[]_SOCKLEN_TYPE="unsigned int")
fi
if test "${PROJECT[]_SOCKLEN_TYPE}" = "" ; then
AC_TRY_COMPILE(
[
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
],
[ sockaddr_in   addr;
int           len = 0;
getsockname(0, (struct sockaddr*)&addr, &len);
],
PROJECT[]_SOCKLEN_TYPE="int")
fi
if test "${PROJECT[]_SOCKLEN_TYPE}" = "" ; then
AC_MSG_RESULT(FAILED)
AC_MSG_RESULT(-------------WARNING!---------------)
AC_MSG_RESULT(could not find a matching type for socket length argument)
AC_MSG_RESULT(in call to getsockname)
AC_MSG_RESULT(please check the setting for PROJECT[]_SOCKLEN_TYPE in config.mak)
AC_MSG_RESULT(and set it to the type needed for the third argument of getsockname)
AC_MSG_RESULT()
else
AC_MSG_RESULT($PROJECT[]_SOCKLEN_TYPE)
fi

AC_DEFINE_UNQUOTED(PROJECT[]_SOCKLEN_TYPE, ${PROJECT[]_SOCKLEN_TYPE})
])

AC_DEFUN([CF_CLEAR_DEP_FILES], [
	dnl
	dnl   make sure the dependencies and object lists are (re)built
	dnl
	${RM}  .Dependencies 2>/dev/null
	${RM}  lib*.objects 2>/dev/null
])

AC_DEFUN([CF_VALGRIND], [
	dnl	
	dnl	Check for the valgrind application (a memory leak tester).
	dnl Valgrind can be used to identify leaks from the test programs
 	dnl	(target valgrind in PROJECT[]/source/TEST).
	dnl
	AC_PATH_PROG(VALGRIND, valgrind, valgrind)
	AC_SUBST(VALGRIND, $VALGRIND)
	if test "${VALGRIND}" != "valgrind" ; then
		AC_MSG_CHECKING(valgrind version)

		VALGRIND_VERSION=`${VALGRIND} --version | tr -d "a-zA-Z-_" 2>&1`
		VALGRIND_VERS_NUM=`echo ${VALGRIND_VERSION}| ${CUT} -d\  -f1`
		VALGRIND_VERS_MAJOR=`echo ${VALGRIND_VERS_NUM} | ${CUT} -d. -f1`
		VALGRIND_VERS_MINOR=`echo ${VALGRIND_VERS_NUM} | ${CUT} -d. -f2`
		VALGRIND_VERS_MINOR_MINOR=`echo ${VALGRIND_VERS_NUM} | ${CUT} -d. -f3`
		AC_MSG_RESULT(${VALGRIND_VERSION} (${VALGRIND_VERS_MAJOR}.${VALGRIND_VERS_MINOR}))	
		

		if test "${VALGRIND_VERS_MAJOR}" = "2" -a "${VALGRIND_VERS_MINOR}" -gt "0" ; then 
			AC_SUBST(VALGRIND_OPTS, "--tool=memcheck  --num-callers=20 --show-below-main=yes -v --leak-check=yes --leak-resolution=high --show-reachable=yes")
		else
			AC_SUBST(VALGRIND_OPTS, "-v --leak-check=yes --leak-resolution=high")
		fi
	else
		AC_SUBST(VALGRIND_OPTS, "-v --leak-check=yes --leak-resolution=high")
	fi
])

dnl
dnl fix libtool breakage due to -rpath option
dnl
AC_DEFUN([AC_FIX_LIBTOOL], [
	# by Marcelo Magallon <mmagallo@efis.ucr.ac.cr>
	# Turn around -rpath problem with libtool 1.0c
	# This define should be improbable enough to not conflict with anything
	if test "$OS" = Linux ; then
			AC_MSG_RESULT([Fixing libtool for -rpath problems.])
			sed < libtool > libtool-2 \
			's/^hardcode_libdir_flag_spec.*$'/'hardcode_libdir_flag_spec=" -D__LIBTOOL_IGNORE_RPATH__ "/'
			mv libtool-2 libtool
			chmod 755 libtool
	fi
])

AC_DEFUN([CF_MOVE_CONFIG_FILES], [
	${MV} Makefile.tmp Makefile
	${MV} common.mak.tmp common.mak
	${MV} config.mak.tmp config.mak
	${MV} rules.mak.tmp rules.mak
	${MV} targets.mak.tmp targets.mak
	${MV} BALLPythonConfig.h.tmp $PROJECT[]_PATH/include/PROJECT[]/PYTHON/BALLPythonConfig.h
	${MV} BALLTestConfig.h.tmp $PROJECT[]_PATH/source/TEST/BALLTestConfig.h

	dnl
	dnl move that damned file only if it differs from the previous
	dnl version. Otherwise we have to rebuild _everything_ after each configure
	dnl
	if test -f $PROJECT[]_PATH/include/PROJECT[]/CONFIG/config.h ; then
		if test "`${DIFF} config.h $PROJECT[]_PATH/include/PROJECT[]/CONFIG/config.h`" != "" ; then
			${MV} -f config.h $PROJECT[]_PATH/include/PROJECT[]/CONFIG/config.h
		fi
	else
		dnl
		dnl  create the directory PROJECT[]/include/CONFIG
		dnl  and move config.h to that directory
		dnl
		mkdir ${PROJECT[]_PATH}/include/PROJECT[]/CONFIG 2>/dev/null
		${MV} -f config.h $PROJECT[]_PATH/include/PROJECT[]/CONFIG/config.h
	fi
])
