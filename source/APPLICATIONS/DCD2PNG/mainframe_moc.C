/****************************************************************************
** BALL::Mainframe meta object code from reading C++ file 'mainframe.h'
**
** Created: Thu Jul 15 15:28:19 2004
**      by: The Qt MOC ($Id: mainframe_moc.C,v 1.1 2004/07/15 15:20:02 amoll Exp $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "mainframe.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *BALL::Mainframe::className() const
{
    return "BALL::Mainframe";
}

QMetaObject *BALL::Mainframe::metaObj = 0;
static QMetaObjectCleanUp cleanUp_BALL__Mainframe( "BALL::Mainframe", &BALL::Mainframe::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString BALL::Mainframe::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "BALL::Mainframe", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString BALL::Mainframe::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "BALL::Mainframe", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* BALL::Mainframe::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = BALL::VIEW::MainControl::staticMetaObject();
    static const QUMethod slot_0 = {"exportPOVRay", 0, 0 };
    static const QUParameter param_slot_1[] = {
	{ "file", &static_QUType_ptr, "String", QUParameter::In }
    };
    static const QUMethod slot_1 = {"openFile", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ "filename", &static_QUType_ptr, "String", QUParameter::In }
    };
    static const QUMethod slot_2 = {"loadBALLViewProjectFile", 1, param_slot_2 };
    static const QMetaData slot_tbl[] = {
	{ "exportPOVRay()", &slot_0, QMetaData::Public },
	{ "openFile(const String&)", &slot_1, QMetaData::Public },
	{ "loadBALLViewProjectFile(const String&)", &slot_2, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"BALL::Mainframe", parentObject,
	slot_tbl, 3,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_BALL__Mainframe.setMetaObject( metaObj );
    return metaObj;
}

void* BALL::Mainframe::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "BALL::Mainframe" ) )
	return this;
    return MainControl::qt_cast( clname );
}

bool BALL::Mainframe::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: exportPOVRay(); break;
    case 1: openFile((const String&)*((const String*)static_QUType_ptr.get(_o+1))); break;
    case 2: loadBALLViewProjectFile((const String&)*((const String*)static_QUType_ptr.get(_o+1))); break;
    default:
	return MainControl::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool BALL::Mainframe::qt_emit( int _id, QUObject* _o )
{
    return MainControl::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool BALL::Mainframe::qt_property( int id, int f, QVariant* v)
{
    return MainControl::qt_property( id, f, v);
}

bool BALL::Mainframe::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
