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
			while test _`egrep / $_TMP_FIND_NAME`_ != __ ; do
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
