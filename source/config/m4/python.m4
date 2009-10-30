dnl
dnl		Python extension support
dnl
dnl   TODO: make sure that all configure options are really parsed
dnl
AC_DEFUN([CF_PYTHON], [
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
		dnl Python support won't work without VIEW!
		dnl (at least for the moment...)
		dnl
		if test "${USE_VIEW}" = false ; then
			AC_MSG_RESULT()
			AC_MSG_RESULT(PROJECT[] Python support requires the visualization component)
			AC_MSG_RESULT(VIEW. Please reconfigure without --without-VIEW.)
			CF_ERROR
		fi
		AC_DEFINE(PROJECT[]_PYTHON_SUPPORT)

		dnl
		dnl first, run the macro provided by the autoconf archive
		dnl
		AC_PYTHON_DEVEL(>= '2.3')

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
			CF_ERROR
		fi

		AC_MSG_CHECKING(for Python interpreter)
		if test -x "${PYTHON_EXECUTABLE}" ; then
			AC_MSG_RESULT(${PYTHON_EXECUTABLE})
		else
			AC_MSG_RESULT()
			AC_MSG_RESULT([Could not find Python interpreter ]${PYTHON_EXCUTABLE})
			AC_MSG_RESULT([Please use --with-python=EXE to specify its location.])
			CF_ERROR
		fi
			
		PYTHON_INCLUDES="${PYTHON_CPPFLAGS}"	
		PYTHON_INC_PATH=`echo ${PYTHON_INCLUDES} | sed s+-I++g`

		dnl
		dnl	Python library path
		dnl
    dnl use framework Python instead for Darwin
    if test "${OS}" = "Darwin" ; then
 			PYTHON_LIBS="-framework Python"
		else     
			PYTHON_LIBS="${PYTHON_LDFLAGS} ${PYTHON_EXTRA_LDFLAGS} ${PYTHON_EXTRA_LIBS}"
			AC_MSG_RESULT(Linker options for Python library: ${PYTHON_LIBS})
		fi

		dnl
		dnl	 SIP executable
		dnl
		CF_MSG_PATH_PROG(SIP,sip,no)
		if test "${SIP}" = no ; then
			AC_MSG_RESULT()
			AC_MSG_RESULT(Could not find SIP executable!)
			AC_MSG_RESULT(Please specify the location of SIP using the)
			AC_MSG_RESULT( --with-sip=PATH)
			AC_MSG_RESULT(option or make sure it is in your current PATH.)
			CF_ERROR
		fi

		dnl
		dnl	 SIP executable
		dnl
		AC_MSG_CHECKING(whether ${SIP} is executable)
		if test ! -x "${SIP}" ; then
			AC_MSG_RESULT()
			AC_MSG_RESULT(Could not execute ${SIP}!)
			AC_MSG_RESULT(Please specify the location of SIP using the)
			AC_MSG_RESULT( --with-sip=PATH)
			AC_MSG_RESULT(option or make sure it is in your current PATH.)
			CF_ERROR
		fi
		AC_MSG_RESULT(yes)

		dnl
		dnl		SIP version
		dnl
		AC_MSG_CHECKING(sip version)
		SIP_VERSION=`$SIP -V`
		AC_MSG_RESULT(${SIP_VERSION})
		BALL_SIP_VERSION_STR=${SIP_VERSION}
		AC_SUBST(BALL_SIP_VERSION_STR)
		if test "${SIP}" = "" ; then
			AC_MSG_RESULT()
			AC_MSG_RESULT(Could not determine version number of ${SIP}!)
			AC_MSG_RESULT(Please specify the location of SIP using the)
			AC_MSG_RESULT( --with-sip=PATH)
			AC_MSG_RESULT(option or make sure it is in your current PATH.)
			CF_ERROR
		fi
		SIP_VERS_NUM=`echo ${SIP_VERSION}| ${CUT} -d\  -f1`
		SIP_VERS_MAJOR=`echo ${SIP_VERS_NUM} | ${CUT} -d. -f1`
		SIP_VERS_MINOR=`echo ${SIP_VERS_NUM} | ${CUT} -d. -f2`
		SIP_VERS_MINOR_MINOR=`echo ${SIP_VERS_NUM} | ${CUT} -d. -f3`
		if test "${SIP_VERS_MINOR_MINOR}" = "" ; then
			SIP_VERS_MINOR_MINOR="0"
		fi
		if test "${SIP_VERS_MAJOR}" -lt 4 \
				-o "${SIP_VERS_MAJOR}" = 4 -a "${SIP_VERS_MINOR}" -lt 4 \
				-o "${SIP_VERS_MAJOR}" = 4 -a "${SIP_VERS_MINOR}" = 4 -a "${SIP_VERS_MINOR_MINOR}" -lt 1; then
			AC_MSG_RESULT()
			AC_MSG_RESULT(SIP release 4.4.5 or above required.)
			AC_MSG_RESULT(Your version: ${SIP_VERSION}")
			AC_MSG_RESULT(Please upgrade or specify the location of the correct SIP using the)
			AC_MSG_RESULT( --with-sip=PATH)
			AC_MSG_RESULT()
			CF_ERROR
		fi
	
		dnl
		dnl	SIP header file (sip.h)
		dnl
		AC_MSG_CHECKING(sip header file)
		if test "${SIP_INCLUDE_PATH}" = "" ; then
			SIP_INCLUDE_PATH="${PYTHON_INC_PATH}"
		fi
		CF_FIND_HEADER(SIP_INC_PATH, sip.h, ${SIP_INCLUDE_PATH})
		if test "${SIP_INC_PATH}" = "" ; then
			AC_MSG_RESULT(not found!)
			AC_MSG_RESULT()
			AC_MSG_RESULT(Please specify the path to the directory that contains)
			AC_MSG_RESULT(sip.h using the option --with-sip-incl=DIR.)
			AC_MSG_RESULT([If you do not have that file, you should obtain SIP])
			AC_MSG_RESULT(from)
			AC_MSG_RESULT(  www.thekompany.com/projects/pykde)
			CF_ERROR
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
])

