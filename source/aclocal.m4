	dnl
	dnl		display the license and abort if not accepted
	dnl		we create the file config.lic if the license was
	dnl		accepted and do not show the license the second time
	dnl

	AC_DEFUN(CF_CHECK_LICENSE,[
	AC_PATH_PROG(PAGER, more, no)
	if test "${PAGER}" = "no" ; then
		PAGER=cat
	fi
	if test ! -f config.lic ; then
		${PAGER} LICENSE
		echo " "
		echo "Do you accept this license terms (y/n)?"
		answer=""
		while test "$answer" != "y" -a "$answer" != "n" ; do
			read answer 
		done
		if test "$answer" = "n" ; then	
			exit
		else
			echo "accepted" > config.lic
		fi
	fi
	])

	dnl		define a macro to remove the directory name
	dnl		from a fully specified path
	dnl
	AC_DEFUN(CF_BASENAME,[
	TMP__DIR="$1/"
	while test "${TMP__DIR}" != "" ; do
		TMP__NAME=`echo ${TMP__DIR}|${CUT} -d/ -f1`
		TMP__DIR=`echo ${TMP__DIR}|${CT} -d/ -f2-`
		if test "${TMP__DIR}" = "${TMP__NAME}" ; then
			TMP__DIR=""
		fi
	done
	])

	dnl    define a macro to inform the user about failed tests for programs
	dnl    it checks for the unix command given as second parameter and
	dnl    sets the shell variable given as second parameter to its absolute path
	dnl 
	AC_DEFUN(CF_MSG_PATH_PROG,[
	AC_PATH_PROG($1,$2,no)
	if test $$1 = no ; then
		AC_MSG_RESULT()
		AC_MSG_RESULT([This script requires the unix command $2, but cannot find it.])
		AC_MSG_RESULT([Please add the correct path to $2 to your \$PATH variable])
		AC_MSG_RESULT([and restart configure.])
		AC_MSG_RESULT()
		AC_MSG_ERROR(aborted)
		exit
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
	dnl        additional dirnames are included in searches these should be absolute names!
	dnl 

	AC_DEFUN(CF_FIND_HEADER,[
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
	dnl        additional dirnames are included in searches these should be absolute names!
	dnl 

	AC_DEFUN(CF_FIND_LIB,[
	_LIBS=

	dnl   immediate "return" on preset directory (read from file?)
	if test "${$1}" != "" ; then
		_LIBS=${$1}
	fi

	if test "${_LIBS}" = "" ; then
		for i in $3 /usr/lib /opt/lib ; do
			for j in $i/$2.* ; do
				if test -f "$j" -a "${_LIBS}" = ""; then
					_LIBS="$i"
				fi
			done
		done
	fi

	if test "${_LIBS}" = "" ; then
		for i in /usr/*/lib /opt/*/lib ; do
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
					_TMP=`${FIND} $i -name "$2*" -print 2>/dev/null`
					for j in ${_TMP} ; do
						if test "${_LIBS}" = "" ; then
							_LIBS=`echo $j|${SED} "s/\/$2.*/\//"`
						fi
					done
				fi
			done
		fi
		
		if test "${_LIBS}" = "" ; then
			_TMP=`${FIND} /opt -name "$2*" -print 2>/dev/null`
			for j in ${_TMP} ; do
				if test "${_LIBS}" = "" ; then
					_LIBS=`echo $j|${SED} "s/\/$2.*/\//"`
				fi
			done
		fi

		if test "${_LIBS}" = "" ; then
			_TMP=`${FIND} /usr -name "$2*" -print 2>/dev/null`
			for j in ${_TMP} ; do
				if test "${_LIBS}" = "" ; then
					_LIBS=`echo $j|${SED} "s/\/$2.*/\//"`
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
	AC_DEFUN(CF_CHECK_ECHO,[
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
	AC_DEFUN(CF_CHECK_FIND,[
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
	AC_DEFUN(CF_DETECT_OS,[
	AC_SUBST(OSMAJOR)
	AC_SUBST(OS)
	AC_SUBST(OSREV)
	AC_SUBST(BINFMT)
	AC_SUBST(BINFMT_PATH)
	AC_SUBST(ARCHITECTURE)

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

		if test "${ARCHITECTURE}" = "unknown" ; then
			AC_MSG_RESULT(OS: ${OS} / hardware: ${PROCESSOR})
			AC_MSG_RESULT(Sorry - this architecture is currently not supported...)
			AC_MSG_ERROR(aborted)
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

	if test "`echo $OS | ${CUT} -d_ -f1`" = "CYGWIN" ; then
		OS="CYGWIN"
		BALL_NO_XDR=true
		USE_BALLVIEW=false
	fi

	if test "$OS" != Linux -a "$OS" != Solaris -a "$OS" != IRIX -a  "$OS" != OSF1 -a "$OS" != FreeBSD -a "$OS" != "CYGWIN" ; then
		AC_MSG_RESULT(Sorry - your OS is currently not supported...)
		AC_MSG_ERROR(aborted)
	fi

	dnl
	dnl 	create OS defines in config.h:
	dnl
	if test "${OS}" = Linux ; then
		AC_DEFINE(BALL_OS_LINUX,LINUX)
	fi
	if test "${OS}" = Solaris ; then
		AC_DEFINE(BALL_OS_SOLARIS,SOLARIS)
	fi
	if test "${OS}" = IRIX ; then
		AC_DEFINE(BALL_OS_IRIX,IRIX)
	fi
	if test "${OS}" = OSF1 ; then
		AC_DEFINE(BALL_OS_OSF1,OSF1)
	fi
	if test "${OS}" = FreeBSD ; then
		AC_DEFINE(BALL_OS_FREEBSD,FREEBSD)
	fi

	dnl
	dnl		create ARCHITECTURE defines
	dnl
	if test "$ARCHITECTURE" = sparc ; then
		AC_DEFINE(BALL_ARCH_SPARC,SPARC)
	fi
	if test "$ARCHITECTURE" = i386 ; then
		AC_DEFINE(BALL_ARCH_I386,I386)
	fi
	if test "$ARCHITECTURE" = mips ; then
		AC_DEFINE(BALL_ARCH_MIPS,MIPS)
	fi
	if test "$ARCHITECTURE" = alpha ; then
		AC_DEFINE(BALL_ARCH_ALPHA,ALPHA)
	fi

	AC_MSG_RESULT($OS $OSREV (BINFMT=$BINFMT))

	dnl
	dnl	some definitions the depend solely on the OS
	dnl
	SHARED_LIB_SUFFIX=so
	if test "${OS}" = HP-UX ; then
		SHARED_LIB_SUFFIX=sl
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
	AC_DEFUN(CF_SEARCH_CXX,[
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
				AC_MSG_ERROR(aborted)
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
			AC_MSG_RESULT()
			AC_MSG_ERROR(aborted)
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
AC_DEFUN(CF_DIGEST_CXX_VERSION,[
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
	])

dnl
dnl		Check whether CXX is a GNU compiler and retrieve its
dnl			version number
dnl
AC_DEFUN(CF_IDENTIFY_GXX,[
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
		AC_MSG_CHECKING(compiler version)
		CXX_VERSION=`${CXX} --version | ${SED} -n 1p | ${SED} ["]s/^[[^0-9]]*//["] | ${CUT} -f1 -d" "`
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
			AC_MSG_RESULT([The version of gcc you are using is not supported by BALL.])
			AC_MSG_RESULT([Please update to a newer version of g++ (at least 2.95.x)])
			AC_MSG_RESULT([which can be obtained from])
			AC_MSG_RESULT([  ftp://gcc.gnu.org/pub/gcc/releases/index.html])
			AC_MSG_RESULT([or specify a different compiler using the option --with-compiler=])
			AC_MSG_RESULT()
			AC_MSG_ERROR(aborted)
		fi

		dnl
		dnl		For g++ 3 and above we have to link against libiberty.a in order
		dnl			to get the name demangling done.
		dnl
		if test "${CXX_VERSION_1}" -ge 3 ; then
			LIBS="${LIBS} -liberty"
			AC_MSG_CHECKING(whether libiberty is required)
			AC_MSG_RESULT(yes)
		fi
	else
		AC_MSG_RESULT(no)
		HAS_GPLUSPLUS=false
	fi
	${RM} /tmp/$$.conftest.c

])

dnl		Check for KAI C++ (KCC)
dnl   At least under linux the damned frontend won't tell
dnl   its version number, so we try to extract the word kai
dnl   from its drivers options when called in verbose mode.
dnl   Nasty - but seems to work. Anybody with a better solution
dnl   should feel free to inform me!
dnl
AC_DEFUN(CF_IDENTIFY_KAI, [
	AC_MSG_CHECKING(for KAI C++ compiler)
	KAI=`${CXX} -v --version 2>&1 | sed "s/.*KAI.*/__KAI__/g" |sed "s/.*kai.*/__KAI__/g" | ${EGREP} "^__KAI__$" | sed -n 1p`
	if test "${KAI}" = "__KAI__" ; then
		IS_KCC=true
		AC_MSG_RESULT(yes)
		CXX_NAME="KAI"
		CXX_IDENTIFIED=true

		AC_MSG_CHECKING(compiler version)
		echo "int main(){}" > conftest.C
		CXX_VERSION=`${CXX} -v --version conftest.C 2>&1| ${GREP} "KAI C++ " | ${CUT} -d" " -f3`
		CXX_NAME="KCC"
		VERSION_OUTPUT="KAI C++ ${CXX_VERSION}"
		CXX_COMPILER_NAME="KCC"

		AC_MSG_RESULT(${VERSION_OUTPUT})
		CF_DIGEST_CXX_VERSION
	else
		IS_KCC=false
		AC_MSG_RESULT(no)
	fi

])

dnl
dnl   Check for Intel C++ (icc)
dnl
AC_DEFUN(CF_IDENTIFY_INTEL, [
	AC_MSG_CHECKING(for Intel C++ compiler)
	ICC=`${CXX} -V 2>&1 | ${SED} -n 1p | ${SED} "s/Intel(R) C++ Compiler.*/__INTELCC__/g" | ${EGREP} "^__INTELCC__$" | sed -n 1p`
	if test "${ICC}" = "__INTELCC__" ; then
		IS_INTELCC=true
		AC_MSG_RESULT(yes)
		CXX_NAME="Intel"
		CXX_IDENTIFIED=true

		AC_MSG_CHECKING(compiler version)
		echo "int main(){}" > conftest.C
		CXX_VERSION=`${CXX} -V conftest.C 2>&1| ${GREP} "Intel(R) C++" | ${SED} -n 1p | ${SED} "s/.*Version //" | ${CUT} -d" " -f1`
		CXX_NAME="icc"
		VERSION_OUTPUT="Intel C++ Compiler ${CXX_VERSION}"
		CXX_COMPILER_NAME="icc"

		AC_MSG_RESULT(${VERSION_OUTPUT})
		CF_DIGEST_CXX_VERSION
	else
		IS_INTELCC=false
		AC_MSG_RESULT(no)
	fi

])

dnl
dnl   check for the Digital C++ compiler
dnl
dnl
AC_DEFUN(CF_IDENTIFY_COMPAQ,[
	AC_MSG_CHECKING(for Digital/Compaq C++ compiler)
	DIGITAL_CXX=`${CXX} -V 2>/dev/null | ${GREP} "C++" | ${CUT} -d" " -f-2`
	if test "${DIGITAL_CXX}" = "DIGITAL C++" -o "${DIGITAL_CXX}" = "Compaq C++"; then
		IS_DIGITALCXX=true
		AC_MSG_RESULT(yes)
		CXX_NAME="Compaq"
		CXX_IDENTIFIED=true

		AC_MSG_CHECKING(compiler version)
		echo "int main(){}" > conftest.C
		CXX_VERSION=`${CXX} -V  2>/dev/null| ${GREP} "C++" | ${CUT} -d" " -f3 | tr -d V`
		CXX_NAME=`${CXX} -V | ${GREP} "C++" | ${CUT} -d" " -f1`
		VERSION_OUTPUT="${CXX_NAME} C++ ${CXX_VERSION}"
		CXX_COMPILER_NAME="Digital"

		AC_MSG_RESULT(${VERSION_OUTPUT})
		CF_DIGEST_CXX_VERSION

		if test "${CXX_VERSION_1}" -lt 6 -o "${CXX_VERSION_1}" -eq 6 -a "${CXX_VERSION_2}" -lt 2 ; then
			AC_MSG_RESULT()
			AC_MSG_RESULT(Your version of Digital/Compaq C++ does not provide all)
			AC_MSG_RESULT(ANSI C++ features required by BALL.)
			AC_MSG_RESULT(Please upgrade to release 6.2 or above.)
			AC_MSG_RESULT()
			AC_MSG_ERROR(aborted.)
		fi
	else
		IS_DIGITALCXX=false
		AC_MSG_RESULT(no)
	fi

])

AC_DEFUN(CF_IDENTIFY_SGI, [
	AC_MSG_CHECKING(for SGI/MipsPro C++ compiler)
	SGI_CXX=`${CXX} -version -n32 2>&1 | ${GREP} "MIPSpro" | ${CUT} -d" " -f1`
	if test "${SGI_CXX}" = "MIPSpro"; then
		IS_MIPSPRO=true
		AC_MSG_RESULT(yes)
		CXX_NAME="MIPSpro"
		CXX_IDENTIFIED=true

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
	else
		IS_MIPSPRO=false
		AC_MSG_RESULT(no)
	fi

])

AC_DEFUN(CF_IDENTIFY_SUN, [
	AC_MSG_CHECKING(for SUN WorkShop/Forte C++ compiler)
	SUN_CXX=`${CXX} -V 2>&1 | ${EGREP} -e "(Sun)|(WorkShop)|(Forte)"`
	if test "${SUN_CXX}" != ""; then
		IS_SUNCC=true
		AC_MSG_RESULT(yes)
		CXX_NAME="SunCC"
		CXX_IDENTIFIED=true

		AC_MSG_CHECKING(compiler version)
		CXX_VERSION=`echo ${SUN_CXX} | ${SED} "s/^.*C++ //" | ${CUT} -d" " -f1`
		changequote(<<,>>)
		CXX_VERSION_TEST=`echo ${CXX_VERSION} | ${SED} s/\^\\[0-9\\.\\]*[a-zA-Z\\.]*//g`
		changequote([,])
		if test "${CXX_VERSION_TEST}" != "" ; then
			CF_BASENAME(${CXX})
			CXX_VERSION="${TMP__NAME}"
			CXX_VERSION_OUTPUT="${CXX_VERSION} (unknown version)"
		fi
		CF_DIGEST_CXX_VERSION	
		AC_MSG_RESULT(${CXX_VERSION_OUTPUT})		
	else
		IS_SUNCC=false
		AC_MSG_RESULT(no)
	fi

])

dnl
dnl   Assemble the complete compiler name by adding
dnl   the release numbers (if known) of the compiler
dnl
AC_DEFUN(CF_BUILD_FULL_CXX_NAME, [
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
dnl   check for endianness of the architecture
dnl
dnl
AC_DEFUN(CF_C_BIGENDIAN, [
	AC_MSG_CHECKING(for byte order)
	AC_TRY_RUN(
    [
      #include <iostream>
      #include <fstream>
      int main(int, char**)
      {
    ] ${BALL_SIZE_TYPE} endian_one = 1; [
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
    BALL_ENDIAN_TEST=true,
    DUMMY=0,
    DUMMY=0
	)
	if test "${BALL_ENDIAN_TEST+set}" != set ; then
		AC_MSG_RESULT(<cannot determine>)
		AC_MSG_RESULT()
		AC_MSG_ERROR(Cannot determine endianness of machine.)
	else
		dnl
		dnl read the result of the endian test from the file
		dnl and delete the file
		dnl
		ENDIAN_TYPE=`${CAT} config.endian.log`
		${RM} config.endian.log 2>/dev/null
		if test "${ENDIAN_TYPE}" = "LITTLE" ; then
			BALL_LITTLE_ENDIAN=true
			AC_DEFINE(BALL_LITTLE_ENDIAN, true)
			AC_MSG_RESULT(little endian)
		else
			if test "${ENDIAN_TYPE}" = "BIG" ; then
				BALL_BIG_ENDIAN=true
				AC_DEFINE(BALL_BIG_ENDIAN, true)
				AC_MSG_RESULT(big endian)
			else
				AC_MSG_RESULT(<cannot determine>)
				AC_MSG_RESULT()
				AC_MSG_ERROR(Cannot determine endianness of machine.)
			fi
		fi
	fi
])
