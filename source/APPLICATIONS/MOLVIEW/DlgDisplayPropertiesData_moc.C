/****************************************************************************
** DlgDisplayPropertiesData meta object code from reading C++ file 'DlgDisplayPropertiesData.h'
**
** Created: Mon Jan 10 15:26:35 2000
**      by: The Qt Meta Object Compiler ($Revision: 1.1 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_DlgDisplayPropertiesData
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 3
#elif Q_MOC_OUTPUT_REVISION != 3
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "DlgDisplayPropertiesData.h"
#include <qmetaobject.h>
#include <qapplication.h>

#if defined(Q_SPARCWORKS_FUNCP_BUG)
#define Q_AMPERSAND
#else
#define Q_AMPERSAND &
#endif


const char *DlgDisplayPropertiesData::className() const
{
    return "DlgDisplayPropertiesData";
}

QMetaObject *DlgDisplayPropertiesData::metaObj = 0;


#if QT_VERSION >= 199
static QMetaObjectInit init_DlgDisplayPropertiesData(&DlgDisplayPropertiesData::staticMetaObject);

#endif

void DlgDisplayPropertiesData::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(QDialog::className(), "QDialog") != 0 )
	badSuperclassWarning("DlgDisplayPropertiesData","QDialog");

#if QT_VERSION >= 199
    staticMetaObject();
}

QString DlgDisplayPropertiesData::tr(const char* s)
{
    return ((QNonBaseApplication*)qApp)->translate("DlgDisplayPropertiesData",s);
}

void DlgDisplayPropertiesData::staticMetaObject()
{
    if ( metaObj )
	return;
    QDialog::staticMetaObject();
#else

    QDialog::initMetaObject();
#endif

    metaObj = QMetaObject::new_metaobject(
	"DlgDisplayPropertiesData", "QDialog",
	0, 0,
	0, 0 );
}
