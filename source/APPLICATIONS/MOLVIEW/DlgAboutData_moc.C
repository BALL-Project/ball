/****************************************************************************
** DlgAboutData meta object code from reading C++ file 'DlgAboutData.h'
**
** Created: Mon Jan 10 15:59:53 2000
**      by: The Qt Meta Object Compiler ($Revision: 1.1 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_DlgAboutData
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 3
#elif Q_MOC_OUTPUT_REVISION != 3
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "DlgAboutData.h"
#include <qmetaobject.h>
#include <qapplication.h>

#if defined(Q_SPARCWORKS_FUNCP_BUG)
#define Q_AMPERSAND
#else
#define Q_AMPERSAND &
#endif


const char *DlgAboutData::className() const
{
    return "DlgAboutData";
}

QMetaObject *DlgAboutData::metaObj = 0;


#if QT_VERSION >= 199
static QMetaObjectInit init_DlgAboutData(&DlgAboutData::staticMetaObject);

#endif

void DlgAboutData::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(QDialog::className(), "QDialog") != 0 )
	badSuperclassWarning("DlgAboutData","QDialog");

#if QT_VERSION >= 199
    staticMetaObject();
}

QString DlgAboutData::tr(const char* s)
{
    return ((QNonBaseApplication*)qApp)->translate("DlgAboutData",s);
}

void DlgAboutData::staticMetaObject()
{
    if ( metaObj )
	return;
    QDialog::staticMetaObject();
#else

    QDialog::initMetaObject();
#endif

    metaObj = QMetaObject::new_metaobject(
	"DlgAboutData", "QDialog",
	0, 0,
	0, 0 );
}
