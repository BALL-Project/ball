/****************************************************************************
** DlgAbout meta object code from reading C++ file 'DlgAbout.h'
**
** Created: Tue Jan 11 19:42:56 2000
**      by: The Qt Meta Object Compiler ($Revision: 1.2 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_DlgAbout
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 3
#elif Q_MOC_OUTPUT_REVISION != 3
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "DlgAbout.h"
#include <qmetaobject.h>
#include <qapplication.h>

#if defined(Q_SPARCWORKS_FUNCP_BUG)
#define Q_AMPERSAND
#else
#define Q_AMPERSAND &
#endif


const char *DlgAbout::className() const
{
    return "DlgAbout";
}

QMetaObject *DlgAbout::metaObj = 0;


#if QT_VERSION >= 199
static QMetaObjectInit init_DlgAbout(&DlgAbout::staticMetaObject);

#endif

void DlgAbout::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(DlgAboutData::className(), "DlgAboutData") != 0 )
	badSuperclassWarning("DlgAbout","DlgAboutData");

#if QT_VERSION >= 199
    staticMetaObject();
}

QString DlgAbout::tr(const char* s)
{
    return ((QNonBaseApplication*)qApp)->translate("DlgAbout",s);
}

void DlgAbout::staticMetaObject()
{
    if ( metaObj )
	return;
    DlgAboutData::staticMetaObject();
#else

    DlgAboutData::initMetaObject();
#endif

    metaObj = QMetaObject::new_metaobject(
	"DlgAbout", "DlgAboutData",
	0, 0,
	0, 0 );
}
