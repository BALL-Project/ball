/****************************************************************************
** DlgDisplayProperties meta object code from reading C++ file 'DlgDisplayProperties.h'
**
** Created: Mon Jan 10 18:39:57 2000
**      by: The Qt Meta Object Compiler ($Revision: 1.2 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_DlgDisplayProperties
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 3
#elif Q_MOC_OUTPUT_REVISION != 3
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "DlgDisplayProperties.h"
#include <qmetaobject.h>
#include <qapplication.h>

#if defined(Q_SPARCWORKS_FUNCP_BUG)
#define Q_AMPERSAND
#else
#define Q_AMPERSAND &
#endif


const char *DlgDisplayProperties::className() const
{
    return "DlgDisplayProperties";
}

QMetaObject *DlgDisplayProperties::metaObj = 0;


#if QT_VERSION >= 199
static QMetaObjectInit init_DlgDisplayProperties(&DlgDisplayProperties::staticMetaObject);

#endif

void DlgDisplayProperties::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(DlgDisplayPropertiesData::className(), "DlgDisplayPropertiesData") != 0 )
	badSuperclassWarning("DlgDisplayProperties","DlgDisplayPropertiesData");

#if QT_VERSION >= 199
    staticMetaObject();
}

QString DlgDisplayProperties::tr(const char* s)
{
    return ((QNonBaseApplication*)qApp)->translate("DlgDisplayProperties",s);
}

void DlgDisplayProperties::staticMetaObject()
{
    if ( metaObj )
	return;
    DlgDisplayPropertiesData::staticMetaObject();
#else

    DlgDisplayPropertiesData::initMetaObject();
#endif

    typedef void(DlgDisplayProperties::*m1_t0)();
    m1_t0 v1_0 = Q_AMPERSAND DlgDisplayProperties::done;
    QMetaData *slot_tbl = QMetaObject::new_metadata(1);
    slot_tbl[0].name = "done()";
    slot_tbl[0].ptr = *((QMember*)&v1_0);
    metaObj = QMetaObject::new_metaobject(
	"DlgDisplayProperties", "DlgDisplayPropertiesData",
	slot_tbl, 1,
	0, 0 );
}
