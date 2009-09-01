AC_DEFUN([CF_VIEW_QT_BASICS], [
	AC_MSG_CHECKING(for QT headers)
	if test "${QTDIR}" != "" ; then
		CF_FIND_HEADER(QT_INCPATH,Qt/qglobal.h,${QTDIR}/include ${PROJECT[]_PATH}/contrib/qt/include)
	else
		CF_FIND_HEADER(QT_INCPATH,Qt/qglobal.h,${PROJECT[]_PATH}/contrib/qt/include)
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
		AC_MSG_RESULT()
		AC_MSG_RESULT(Note: BALL requires QT 4.x! QT3 is no longer supported.)
		CF_ERROR
	else
		AC_MSG_RESULT((${QT_INCPATH}))	
	fi

	AC_MSG_CHECKING(for libQtCore)
	if test "${QTDIR}" != "" ; then
		if test -a "${QTDIR}/lib/libQtCore.${SHARED_LIB_SUFFIX}" ; then
			QT_LIBPATH="${QTDIR}/lib"
		fi
		if test "${QT_LIBPATH}" = "" ; then
			CF_FIND_LIB(QT_LIBPATH, libQtCore, ${QTDIR}/lib ${QTDIR}/lib ${PROJECT[]_PATH}/contrib/lib)
		fi
	fi

	if test "${QT_LIBPATH}" = "" ; then
		AC_MSG_RESULT((not found!))
		AC_MSG_RESULT()
		AC_MSG_RESULT([The QtCore library could not be found. Please specify the path to libqt])
 		AC_MSG_RESULT([by passing the option --with-qt-libs=DIR to configure.])
		AC_MSG_RESULT([You may also set the environment variable QTDIR to the correct])
		AC_MSG_RESULT([path - configure will recognize this, too.])
		AC_MSG_RESULT([If the QT library was built with thread support enabled (libqt-mt])
		AC_MSG_RESULT([instead of libqt), please specify the option --with-threadsafe-qt.])
		AC_MSG_RESULT([The QT package can be found under the following URL:])
		AC_MSG_RESULT(  http://www.troll.no/qt)
		AC_MSG_RESULT()
		AC_MSG_RESULT(Note: BALL requires QT 4.x! QT3 is no longer supported.)
		CF_ERROR
	else
		AC_MSG_RESULT((${QT_LIBPATH}))	
	fi

	AC_MSG_CHECKING(for libQtGui)
	if test "${QTDIR}" != "" ; then
		if test -a "${QTDIR}/lib/libQtGui.${SHARED_LIB_SUFFIX}" ; then
			QT_LIBPATH="${QTDIR}/lib"
			AC_MSG_RESULT((${QT_LIBPATH}))	
		fi
		if test "${QT_LIBPATH}" = "" ; then
			CF_FIND_LIB(QT_LIBPATH, libQtGui, ${QTDIR}/lib ${QTDIR}/lib ${PROJECT[]_PATH}/contrib/lib)
			AC_MSG_RESULT((${QT_LIBPATH}))	
		fi
		if test "${QT_LIBPATH}" = "" ; then
			AC_MSG_RESULT((not found!))
			AC_MSG_RESULT()
			AC_MSG_RESULT([The QtGui library could not be found. Please specify the path to libqt])
			AC_MSG_RESULT([by passing the option --with-qt-libs=DIR to configure.])
			AC_MSG_RESULT([You may also set the environment variable QTDIR to the correct])
			AC_MSG_RESULT([path - configure will recognize this, too.])
			AC_MSG_RESULT([If the QT library was built with thread support enabled (libqt-mt])
			AC_MSG_RESULT([instead of libqt), please specify the option --with-threadsafe-qt.])
			AC_MSG_RESULT([The QT package can be found under the following URL:])
			AC_MSG_RESULT(  http://www.troll.no/qt)
			AC_MSG_RESULT()
			AC_MSG_RESULT(Note: BALL requires QT 4.x! QT3 is no longer supported.)
			CF_ERROR
		else
			AC_MSG_RESULT()
		fi
	fi

	AC_MSG_CHECKING(for libQtNetwork)
	if test "${QTDIR}" != "" ; then
		if test -a "${QTDIR}/lib/libQtNetwork.${SHARED_LIB_SUFFIX}" ; then
			QT_LIBPATH="${QTDIR}/lib"
			AC_MSG_RESULT((${QT_LIBPATH}))	
		fi
		if test "${QT_LIBPATH}" = "" ; then
			CF_FIND_LIB(QT_LIBPATH, libQtNetwork, ${QTDIR}/lib ${QTDIR}/lib ${PROJECT[]_PATH}/contrib/lib)
			AC_MSG_RESULT((${QT_LIBPATH}))	
		fi
		if test "${QT_LIBPATH}" = "" ; then
			AC_MSG_RESULT((not found!))
			AC_MSG_RESULT()
			AC_MSG_RESULT([The QtNetwork library could not be found. Please specify the path to libqt])
			AC_MSG_RESULT([by passing the option --with-qt-libs=DIR to configure.])
			AC_MSG_RESULT([You may also set the environment variable QTDIR to the correct])
			AC_MSG_RESULT([path - configure will recognize this, too.])
			AC_MSG_RESULT([The QT package can be found under the following URL:])
			AC_MSG_RESULT(  http://www.troll.no/qt)
			AC_MSG_RESULT()
			AC_MSG_RESULT(Note: BALL requires QT 4.x! QT3 is no longer supported.)
			CF_ERROR
		else
			AC_MSG_RESULT()
		fi
	fi
	AC_MSG_CHECKING(for libQtSql)
	if test "${QTDIR}" != "" ; then
		if test -a "${QTDIR}/lib/libQtSql.${SHARED_LIB_SUFFIX}" ; then
			QT_LIBPATH="${QTDIR}/lib"
			AC_MSG_RESULT((${QT_LIBPATH}))	
		fi
		if test "${QT_LIBPATH}" = "" ; then
			CF_FIND_LIB(QT_LIBPATH, libQtSql, ${QTDIR}/lib ${QTDIR}/lib ${PROJECT[]_PATH}/contrib/lib)
			AC_MSG_RESULT((${QT_LIBPATH}))	
		fi
		if test "${QT_LIBPATH}" = "" ; then
			AC_MSG_RESULT((not found!))
			AC_MSG_RESULT()
			AC_MSG_RESULT([The QtSql library could not be found. Please specify the path to libqt])
			AC_MSG_RESULT([by passing the option --with-qt-libs=DIR to configure.])
			AC_MSG_RESULT([You may also set the environment variable QTDIR to the correct])
			AC_MSG_RESULT([path - configure will recognize this, too.])
			AC_MSG_RESULT([If the QT library was built with thread support enabled (libqt-mt])
			AC_MSG_RESULT([instead of libqt), please specify the option --with-threadsafe-qt.])
			AC_MSG_RESULT([The QT package can be found under the following URL:])
			AC_MSG_RESULT(  http://www.troll.no/qt)
			AC_MSG_RESULT()
			AC_MSG_RESULT(Note: BALL requires QT 4.x! QT3 is no longer supported.)
			CF_ERROR
		else
			AC_MSG_RESULT()
		fi
	fi

	AC_MSG_CHECKING(for libQtXml)
	if test "${QTDIR}" != "" ; then
		if test -a "${QTDIR}/lib/libQtXml.${SHARED_LIB_SUFFIX}" ; then
			QT_LIBPATH="${QTDIR}/lib"
			AC_MSG_RESULT((${QT_LIBPATH}))	
		fi
		if test "${QT_LIBPATH}" = "" ; then
			CF_FIND_LIB(QT_LIBPATH, libQtXml, ${QTDIR}/lib ${QTDIR}/lib ${PROJECT[]_PATH}/contrib/lib)
			AC_MSG_RESULT((${QT_LIBPATH}))	
		fi
		if test "${QT_LIBPATH}" = "" ; then
			AC_MSG_RESULT((not found!))
			AC_MSG_RESULT()
			AC_MSG_RESULT([The QtXml library could not be found. Please specify the path to libqt])
			AC_MSG_RESULT([by passing the option --with-qt-libs=DIR to configure.])
			AC_MSG_RESULT([You may also set the environment variable QTDIR to the correct])
			AC_MSG_RESULT([path - configure will recognize this, too.])
			AC_MSG_RESULT([The QT package can be found under the following URL:])
			AC_MSG_RESULT(  http://www.troll.no/qt)
			AC_MSG_RESULT()
			AC_MSG_RESULT(Note: BALL requires QT 4.x! QT3 is no longer supported.)
			CF_ERROR
		else
			AC_MSG_RESULT()
		fi
	fi

	AC_MSG_CHECKING(for libQtOpenGL)
	if test "${QTDIR}" != "" ; then
		if test -a "${QTDIR}/lib/libQtOpenGL.${SHARED_LIB_SUFFIX}" ; then
			QT_LIBPATH="${QTDIR}/lib"
			AC_MSG_RESULT((${QT_LIBPATH}))	
		fi
		if test "${QT_LIBPATH}" = "" ; then
			CF_FIND_LIB(QT_LIBPATH, libQtopenGL, ${QTDIR}/lib ${QTDIR}/lib ${PROJECT[]_PATH}/contrib/lib)
			AC_MSG_RESULT((${QT_LIBPATH}))	
		fi
		if test "${QT_LIBPATH}" = "" ; then
			AC_MSG_RESULT((not found!))
			AC_MSG_RESULT()
			AC_MSG_RESULT([The QtOpenGL library could not be found. Please specify the path to libqt])
			AC_MSG_RESULT([by passing the option --with-qt-libs=DIR to configure.])
			AC_MSG_RESULT([You may also set the environment variable QTDIR to the correct])
			AC_MSG_RESULT([path - configure will recognize this, too.])
			AC_MSG_RESULT([If the QT library was built with thread support enabled (libqt-mt])
			AC_MSG_RESULT([instead of libqt), please specify the option --with-threadsafe-qt.])
			AC_MSG_RESULT([The QT package can be found under the following URL:])
			AC_MSG_RESULT(  http://www.troll.no/qt)
			AC_MSG_RESULT()
			AC_MSG_RESULT(Note: BALL requires QT 4.x! QT3 is no longer supported.)
			CF_ERROR
		else
			AC_MSG_RESULT()
		fi
	fi

	AC_DEFINE(PROJECT[]_QT_HAS_THREADS,)
		
	dnl
	dnl	Add the QT include path to the VIEW includes
	dnl
	if test "${QT_INCPATH}" != /usr/include && test "${QT_INCPATH}" != "" ; then
		VIEW_INCLUDES="${VIEW_INCLUDES} -I${QT_INCPATH}"
	fi	
])

dnl
dnl Make sure we can link against OpenGL or Mesa
dnl
AC_DEFUN([CF_VIEW_OPENGL_LINK_TEST], [
	if test "${VIEW_PLATFORM}" = OpenGL ; then
		if test "${OPENGL_LIBPATH}" != "/usr/lib" -a "${OPENGL_LIBPATH}" != "" ; then
			OPENGL_LIBOPTS="-L${OPENGL_LIBPATH} -lGLU -lGL ${GLEW_LIBOPTS}"
		else
			OPENGL_LIBPATH=""
			OPENGL_LIBOPTS="-lGLU -lGL ${GLEW_LIBOPTS} ${GLEW_INCLUDES}"
		fi

		dnl make sure we have OpenGL libs and no Mesa libs!
		dnl
		SAVE_LIBS=${LIBS}
		SAVE_LDFLAGS=${LDFLAGS}
		LIBS="${LIBS} ${X11_LIBOPTS}"
		if test "${OPENGL_LIBPATH}" != "" ; then
			LDFLAGS="${LDFLAGS} -L${OPENGL_LIBPATH}"
		fi
		AC_CHECK_LIB(GL, XMesaGarbageCollect, VIEW_PLATFORM=Mesa)
		LIBS=${SAVE_LIBS}
		LDFLAGS=${SAVE_LDFLAGS}
		if test "${VIEW_PLATFORM}" != Mesa ; then
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
				CF_ERROR
			else
				AC_MSG_RESULT(yes)
			fi
		fi
	fi

	if test "${VIEW_PLATFORM}" = Mesa ; then
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
			CF_ERROR
		else
			AC_MSG_RESULT(yes)
			OPENGL_LIBOPTS="${OPENGL_LIBPATHOPT} ${OPENGL_LIBS} ${GLEW_LIBOPTS} ${GLEW_INCLUDES}"
		fi
	fi
])


AC_DEFUN([CF_VIEW_QT_LINK_TEST], [
		X=`pwd`
		AC_MSG_CHECKING(linking against QT libraries)

		GENERIC_QT_LIBPATH=""
		DARWIN_QT_LIBPATH=""
		if test "${QT_LIBPATH}" != "/usr/lib" ; then
			GENERIC_QT_LIBPATH="-L${QT_LIBPATH}"
			DARWIN_QT_LIBPATH="-F${QT_LIBPATH}"
		fi

		if test "${OS}" = "Darwin" ; then
			QTQGL_LIBOPTS="${DARWIN_QT_LIBPATH} -framework QtOpenGL -framework QtGui -framework QtCore -framework QtTest -framework QtSql -framework QtXml -framework QtNetwork"
			QT_BALL_LIBOPTS="${DARWIN_QT_LIBPATH} -framework QtCore -framework QtSql -framework QtXml -framework QtNetwork"
			QT_VIEW_LIBOPTS="${QT_BALL_LIBOPTS} -framework QtOpenGL -framework QtGui -framework QtTest -framework QtXml -framework QtNetwork"
		else
			QTQGL_LIBOPTS="${GENERIC_QT_LIBPATH} -lQtOpenGL -lQtGui -lQtCore -lQtTest -lQtSql -lQtXml -lQtNetwork"
			QT_BALL_LIBOPTS="${GENERIC_QT_LIBPATH} -lQtCore -lQtSql -lQtXml -lQtNetwork"
			QT_VIEW_LIBOPTS="${QT_BALL_LIBOPTS} -lQtOpenGL -lQtGui -lQtTest -lQtNetwork"
		fi

		AC_DEFINE_UNQUOTED(QT_BALL_LIBOPTS, ${QT_BALL_LIBOPTS})
		AC_DEFINE_UNQUOTED(QT_VIEW_LIBOPTS, ${QT_VIEW_LIBOPTS})

		SAVE_LIBS=${LIBS}
		LIBS="${QTQGL_LIBOPTS} ${OPENGL_LIBOPTS} ${X11_LIBOPTS} ${LIBS} ${VIEW_INCLUDES}"
		AC_TRY_LINK([#include <QtOpenGL/QGLWidget>], [QGLWidget wid;], QT_LINKING_OK=1)
		LIBS=${SAVE_LIBS}

		if test "${QT_LINKING_OK+set}" != set ; then
			SAVE_LIBS=${LIBS}
			LIBS="${QT_VIEW_LIBOPTS} ${OPENGL_LIBOPTS} ${X11_LIBOPTS} ${LIBS} ${VIEW_INCLUDES}"
			AC_TRY_LINK([#include <QtOpenGL/QGLWidget>], [QGLWidget wid;], QT_LINKING_OK=1)
			LIBS=${SAVE_LIBS}
		fi

		if test "${QT_LINKING_OK+set}" != set ; then
			SAVE_LIBS=${LIBS}
			X11_LIBOPTS="-lXrender -lfreetype ${X11_LIBOPTS}"
			LIBS="${QT_VIEW_LIBOPTS} ${OPENGL_LIBOPTS} ${X11_LIBOPTS} ${LIBS} ${VIEW_INCLUDES}"
			AC_TRY_LINK([#include <QtOpenGL/QGLWidget>], [QGLWidget wid;], QT_LINKING_OK=1)
			LIBS=${SAVE_LIBS}
		fi

	if test "${QT_LINKING_OK+set}" != set ; then
		AC_MSG_RESULT(no)
		AC_MSG_RESULT()
		AC_MSG_RESULT([Cannot link against QT libraries!])
		AC_MSG_RESULT([If QT is installed, please specify the path to the library])
		AC_MSG_RESULT([using the option --with-qt-libs=DIR or the environment variable QTDIR.])
		CF_ERROR
	else
		AC_MSG_RESULT(yes)
		
		dnl  
		dnl  identify the version of the library
		dnl
		AC_MSG_CHECKING(QT library version)
		SAVE_LIBS=${LIBS}
		LIBS="${QT_BALL_LIBOPTS} ${OPENGL_LIBOPTS} ${X11_LIBOPTS} ${LIBS}"
		if test "${OS}" = "Darwin" ; then
			DYLD_LIBRARY_PATH="${QT_LIBPATH}:${X11_LIBPATH}:${OPENGL_LIBPATH}:${GLEW_LIBPATH}:${DYLD_LIBRARY_PATH}"
			export DYLD_LIBRARY_PATH
			echo "DYLD_LIBRARY_PATH = ${DYLD_LIBRARY_PATH}" 1>&5
		else
			LD_LIBRARY_PATH="${QT_LIBPATH}:${X11_LIBPATH}:${OPENGL_LIBPATH}:${GLEW_LIBPATH}:${LD_LIBRARY_PATH}"
			export LD_LIBRARY_PATH
			echo "LD_LIBRARY_PATH = ${LD_LIBRARY_PATH}" 1>&5
		fi
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
			CF_ERROR
		else
			QT_VERSION_STR=`cat qt.version | ${SED} "s/-.*//"`
			AC_MSG_RESULT(${QT_VERSION_STR})

			dnl
			dnl  Check for the right version number of QT
			dnl
			if test `echo ${QT_VERSION_STR} | ${CUT} -c1-2 | ${SED} "s/-.*//"` != "0x" ; then
				QT_VERSION=`echo ${QT_VERSION_STR} | ${TR} -d "." ` 
				if test "${QT_VERSION}" -lt "${QT_MIN_VERSION}" -o "${QT_VERSION}" -gt "${QT_MAX_VERSION}" ; then
					AC_MSG_RESULT()
					AC_MSG_RESULT([QT version ]${QT_RECOMMENDED_VERSION}[ is recommended for PROJECT[]. Please update])
					AC_MSG_RESULT([to a suitable version or specify the path to a more])
					AC_MSG_RESULT([suitable version of libqt* by passing the option --with-qt-libs=DIR])
					AC_MSG_RESULT([to configure.])
					AC_MSG_RESULT([You may also set the environment variable QTDIR to the correct])
					AC_MSG_RESULT([path - configure will recognize this, too.])
					AC_MSG_RESULT()
					AC_MSG_RESULT([The complete QT package can be found under the following URL:])
					AC_MSG_RESULT([  http://www.troll.no/qt])
					CF_ERROR
				fi
			fi
		fi
	fi
])

AC_DEFUN([CF_VIEW_QT_EXECUTABLES], [
	dnl
	dnl	try to find the MOC (QT meta object compiler)
	dnl It is usually installed in ${QTDIR}/bin/moc
	dnl
	if test "${USE_VIEW}" = true ; then
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
			CF_ERROR
		fi
    dnl
    dnl  Make sure the MOC we found is actually executable
    dnl
		AC_MSG_CHECKING(whether we can run moc)
    if test ! -x "${MOC}" ; then
			AC_MSG_RESULT()
			AC_MSG_RESULT([The QT Meta Object Compiler (moc) found in ])
      AC_MSG_RESULT("  ${MOC}")
      AC_MSG_RESULT([seems not to be an executable!])
			AC_MSG_RESULT([Please include the correct path to moc into your])
			AC_MSG_RESULT([PATH environment variable or specify the path to moc])
			AC_MSG_RESULT([using the option --with-moc=PATH to rerun configure.])
			CF_ERROR
		else
			AC_MSG_RESULT(yes)
			AC_MSG_CHECKING(moc version)
			MOC_VERSION=`${MOC} -v 2>&1 | ${TR} -d "()" | ${SED} "s/.* Qt //" | ${SED} "s/-.*//"`
			AC_MSG_RESULT(${MOC_VERSION})
			
			if test "${MOC_VERSION}" != "${QT_VERSION_STR}" ; then
				AC_MSG_RESULT()
				AC_MSG_RESULT([QT version (${QT_VERSION_STR}) is incompatible with moc version (${MOC_VERSION})!])
				AC_MSG_RESULT([Please check your QTDIR environment variable, include the correct])
				AC_MSG_RESULT([path to moc in your PATH environment variable, or specify the correct])
				AC_MSG_RESULT([path to moc using the option --with-moc=PATH to rerun configure.])
				CF_ERROR
			fi
		fi
	fi

	dnl
	dnl	try to find the UIC (QT user interface compiler)
	dnl It is usually installed in ${QTDIR}/bin/uic
	dnl
	if test "${USE_VIEW}" = true ; then
		if test "${UIC}" = uic ; then
			if test "${QTDIR}" != "" ; then
				UIC=${QTDIR}/bin/uic
			fi
		fi

		dnl
		dnl  try to find that damned uic
		dnl
		AC_PATH_PROG(UIC,uic,uic)
		if test "${UIC}" = uic ; then
			AC_MSG_RESULT()
			AC_MSG_RESULT([Could not find the QT User Interface Compiler (uic)!])
			AC_MSG_RESULT([You might run into trouble if you want to compile MolVIEW.])
			AC_MSG_RESULT([Please include the correct path to uic into your])
			AC_MSG_RESULT([PATH environment variable or specify the path to uic])
			AC_MSG_RESULT([using the option --with-uic=PATH to rerun configure.])
			CF_ERROR
		fi
    
    dnl
    dnl  Make sure the UIC we found is actually executable
    dnl
		AC_MSG_CHECKING(whether uic is executable)
    if test ! -x "${UIC}" ; then
			AC_MSG_RESULT()
			AC_MSG_RESULT([The QT User Interface Compiler (uic) found in ])
      AC_MSG_RESULT("   ${UIC}")
      AC_MSG_RESULT([seems not to be an executable!])
			AC_MSG_RESULT([Please include the correct path to uic into your])
			AC_MSG_RESULT([PATH environment variable or specify the path to uic])
			AC_MSG_RESULT([using the option --with-uic=PATH to rerun configure.])
			CF_ERROR
		else
			AC_MSG_RESULT(yes)
			AC_MSG_CHECKING(uic version)
			changequote(<<,>>)
			UIC_VERSION=`${UIC} -v 2>&1 | ${SED} "s/[A-Za-z ]*//g" | ${SED} "s/\.$//" | ${SED} "s/-.*//"`
			changequote([,])

			AC_MSG_RESULT(${UIC_VERSION})
			
			if test "${UIC_VERSION}" != "${QT_VERSION_STR}" ; then
				AC_MSG_RESULT()
				AC_MSG_RESULT([QT version (${QT_VERSION_STR}) is incompatible with uic version (${UIC_VERSION})!])
				AC_MSG_RESULT([Please check your QTDIR environment variable, include the correct])
				AC_MSG_RESULT([path to uic in your PATH environment variable, or specify the correct])
				AC_MSG_RESULT([path to uic using the option --with-uic=PATH to rerun configure.])
				CF_ERROR
			fi
		fi
	fi

	dnl
	dnl	try to find the RCC (QT meta object compiler)
	dnl It is usually installed in ${QTDIR}/bin/rcc
	dnl
	if test "${USE_VIEW}" = true ; then
		if test "${RCC}" = rcc ; then
			if test "${QTDIR}" != "" ; then
				RCC=${QTDIR}/bin/rcc
			fi
		fi

		dnl
		dnl  try to find that damned moc
		dnl
		AC_PATH_PROG(RCC,rcc,rcc)
		if test "${RCC}" = rcc ; then
			AC_MSG_RESULT()
			AC_MSG_RESULT([Could not find the QT Meta Object Compiler (rcc)!])
			AC_MSG_RESULT([You might run into trouble if you want to compile MolVIEW.])
			AC_MSG_RESULT([Please include the correct path to rcc into your])
			AC_MSG_RESULT([PATH environment variable or specify the path to moc])
			AC_MSG_RESULT([using the option --with-rcc=PATH to rerun configure.])
			CF_ERROR
		fi
    dnl
    dnl  Make sure the RCC we found is actually executable
    dnl
		AC_MSG_CHECKING(whether we can run rcc)
    if test ! -x "${RCC}" ; then
			AC_MSG_RESULT()
			AC_MSG_RESULT([The QT Resource Compiler (rcc) found in ])
      AC_MSG_RESULT("  ${RCC}")
      AC_MSG_RESULT([seems not to be an executable!])
			AC_MSG_RESULT([Please include the correct path to rcc into your])
			AC_MSG_RESULT([PATH environment variable or specify the path to rcc])
			AC_MSG_RESULT([using the option --with-rcc=PATH to rerun configure.])
			CF_ERROR
		else
			AC_MSG_RESULT(yes)
			AC_MSG_CHECKING(rcc version)
			RCC_VERSION=`${RCC} -v 2>&1 | ${TR} -d "()" | ${SED} "s/.* Qt //" | ${SED} "s/-.*//"`
			AC_MSG_RESULT(${RCC_VERSION})

		dnl ???????????????????
			if test "${QT_VERSION_STR}" != "${QT_VERSION_STR}" ; then
				AC_MSG_RESULT()
				AC_MSG_RESULT([QT version (${QT_VERSION_STR}) is incompatible with moc version (${RCC_VERSION})!])
				AC_MSG_RESULT([Please check your QTDIR environment variable, include the correct])
				AC_MSG_RESULT([path to moc in your PATH environment variable, or specify the correct])
				AC_MSG_RESULT([path to moc using the option --with-rcc=PATH to rerun configure.])
				CF_ERROR
			fi
		fi
	fi
])

dnl  Try to identify whether SQL support has been compiled into QT
AC_DEFUN([CF_VIEW_QT_SQL], [
	dnl ????
])


