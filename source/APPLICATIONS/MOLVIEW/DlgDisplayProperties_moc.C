/****************************************************************************
** DlgDisplayProperties meta object code from reading C++ file 'DlgDisplayProperties.h'
**
** Created: Tue Jan 11 19:47:11 2000
**      by: The Qt Meta Object Compiler ($Revision: 1.3 $)
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

    typedef void(DlgDisplayProperties::*m1_t0)(const QString&);
    typedef void(DlgDisplayProperties::*m1_t1)(const QString&);
    typedef void(DlgDisplayProperties::*m1_t2)(const QString&);
    typedef void(DlgDisplayProperties::*m1_t3)();
    typedef void(DlgDisplayProperties::*m1_t4)();
    m1_t0 v1_0 = Q_AMPERSAND DlgDisplayProperties::selectPrecision;
    m1_t1 v1_1 = Q_AMPERSAND DlgDisplayProperties::selectModel;
    m1_t2 v1_2 = Q_AMPERSAND DlgDisplayProperties::selectColoringMethod;
    m1_t3 v1_3 = Q_AMPERSAND DlgDisplayProperties::applyButtonClicked;
    m1_t4 v1_4 = Q_AMPERSAND DlgDisplayProperties::editColor;
    QMetaData *slot_tbl = QMetaObject::new_metadata(5);
    slot_tbl[0].name = "selectPrecision(const QString&)";
    slot_tbl[1].name = "selectModel(const QString&)";
    slot_tbl[2].name = "selectColoringMethod(const QString&)";
    slot_tbl[3].name = "applyButtonClicked()";
    slot_tbl[4].name = "editColor()";
    slot_tbl[0].ptr = *((QMember*)&v1_0);
    slot_tbl[1].ptr = *((QMember*)&v1_1);
    slot_tbl[2].ptr = *((QMember*)&v1_2);
    slot_tbl[3].ptr = *((QMember*)&v1_3);
    slot_tbl[4].ptr = *((QMember*)&v1_4);
    typedef void(DlgDisplayProperties::*m2_t0)();
    m2_t0 v2_0 = Q_AMPERSAND DlgDisplayProperties::apply;
    QMetaData *signal_tbl = QMetaObject::new_metadata(1);
    signal_tbl[0].name = "apply()";
    signal_tbl[0].ptr = *((QMember*)&v2_0);
    metaObj = QMetaObject::new_metaobject(
	"DlgDisplayProperties", "DlgDisplayPropertiesData",
	slot_tbl, 5,
	signal_tbl, 1 );
}

// SIGNAL apply
void DlgDisplayProperties::apply()
{
    activate_signal( "apply()" );
}
