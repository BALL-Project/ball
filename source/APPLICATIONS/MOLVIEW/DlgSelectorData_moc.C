/****************************************************************************
** DlgSelectorData meta object code from reading C++ file 'DlgSelectorData.h'
**
** Created: Tue Jan 11 19:47:32 2000
**      by: The Qt Meta Object Compiler ($Revision: 1.2 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_DlgSelectorData
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 3
#elif Q_MOC_OUTPUT_REVISION != 3
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "DlgSelectorData.h"
#include <qmetaobject.h>
#include <qapplication.h>

#if defined(Q_SPARCWORKS_FUNCP_BUG)
#define Q_AMPERSAND
#else
#define Q_AMPERSAND &
#endif


const char *DlgSelectorData::className() const
{
    return "DlgSelectorData";
}

QMetaObject *DlgSelectorData::metaObj = 0;


#if QT_VERSION >= 199
static QMetaObjectInit init_DlgSelectorData(&DlgSelectorData::staticMetaObject);

#endif

void DlgSelectorData::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(QDialog::className(), "QDialog") != 0 )
	badSuperclassWarning("DlgSelectorData","QDialog");

#if QT_VERSION >= 199
    staticMetaObject();
}

QString DlgSelectorData::tr(const char* s)
{
    return ((QNonBaseApplication*)qApp)->translate("DlgSelectorData",s);
}

void DlgSelectorData::staticMetaObject()
{
    if ( metaObj )
	return;
    QDialog::staticMetaObject();
#else

    QDialog::initMetaObject();
#endif

    metaObj = QMetaObject::new_metaobject(
	"DlgSelectorData", "QDialog",
	0, 0,
	0, 0 );
}
