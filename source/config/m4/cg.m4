dnl
dnl	Cg - support for NVIDIA's shading language
dnl
AC_DEFUN([CF_CG], [
	if test "${CG_SUPPORT}" = true ; then
		CG_SUPPORT=true

		AC_MSG_CHECKING(Checking for NVIDIA Cg support)
		dnl
		dnl  variable substitutions required for Cg support
		dnl
		AC_SUBST(PROJECT[]_HAS_CG,)
		AC_SUBST(CG_INCLUDES,)
		AC_SUBST(CG_LIBS,)

		AC_MSG_CHECKING(for NVIDIA Cg headers)
		CF_FIND_HEADER(CG_INCL_PATH, Cg/cgGL.h, ${CG_INCL})

		if test "${CG_INCL_PATH}" = "" ; then
			AC_MSG_RESULT((not found!))
			AC_MSG_RESULT()
			AC_MSG_RESULT(No CG header files found! Please specify the path to the CG header Cg/cgGL.h)
			AC_MSG_RESULT(by passing the option --with-cg-incl=DIR to configure.)
			CF_ERROR
		else
			AC_MSG_RESULT((${CG_INCL_PATH}))
		fi

	if test "${CG_INCL_PATH}" != /usr/include ; then
		BALL_INCLUDES="${BALL_INCLUDES} -I${CG_INCL_PATH}"
	fi

	AC_MSG_CHECKING(for NVIDIA Cg/CgGL libraries)
	CF_FIND_LIB(CG_LIB,libCg, ${CG_LIB_PATH})
	if test "${CG_LIB}" = "" ; then
			AC_MSG_RESULT((not found!))
			AC_MSG_RESULT(Please install libCG in a standard location or specify the path)
			AC_MSG_RESULT(with --with-cg-lib=DIR on the command line.)
			CF_ERROR
	else
			AC_MSG_RESULT((${CG_LIB_PATH}))
	fi
	CF_FIND_LIB(CG_GL_LIB,libCgGL, ${CG_LIB_PATH})
	if test "${CG_GL_LIB}" = "" ; then
			AC_MSG_RESULT((not found!))
			AC_MSG_RESULT(Please install libCG in a standard location or specify the path)
			AC_MSG_RESULT(with --with-cg-lib=DIR on the command line.)
			CF_ERROR
	else
			AC_MSG_RESULT((${CG_LIB_PATH}))
	fi

	AC_MSG_CHECKING(linking against libCg/libCgGL)
	SAVE_LIBS=${LIBS}
	SAVE_LDFLAGS=${LDFLAGS}
	LIBS="-L${CG_LIB_PATH} -lCg -lCgGL ${LIBS} -L${OPENGL_LIBOPTS} -lpthread"
	LDFLAGS="$LDFLAGS -I${CG_INCL_PATH}"
	CG_LINKING_OK=0
	AC_TRY_LINK([
		#include <Cg/cg.h>
		#include <Cg/cgGL.h>
		],
		[
		cgGetError();
		], CG_LINKING_OK=1)
		LIBS=${SAVE_LIBS}
		LDFLAGS=${SAVE_LDFLAGS}
		if test "${CG_LINKING_OK}" != "1" ; then
			AC_MSG_RESULT(no)
			AC_MSG_RESULT()
			AC_MSG_RESULT([Cannot link against libCg. Please check config.log and])
			AC_MSG_RESULT([specify appropriate options to configure (e.g. --with-cg-lib/incl).])
			CF_ERROR
		else
			AC_MSG_RESULT(yes)
		fi
	fi

	CG_LIBS="-L${CG_LIB_PATH} -lCg -lCgGL"

	AC_DEFINE(PROJECT[]_HAS_CG, true)
	PROJECT[]_HAS_CG=true
])


