/****************************************************************************
** DlgDisplayPropertiesData meta object code from reading C++ file 'DlgDisplayPropertiesData.h'
**
** Created: Thu Jan 13 18:50:30 2000
**      by: The Qt Meta Object Compiler ($Revision: 1.4 $)
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

    typedef void(DlgDisplayPropertiesData::*m1_t0)(const QString&);
    typedef void(DlgDisplayPropertiesData::*m1_t1)(const QString&);
    typedef void(DlgDisplayPropertiesData::*m1_t2)();
    typedef void(DlgDisplayPropertiesData::*m1_t3)(const QString&);
    typedef void(DlgDisplayPropertiesData::*m1_t4)();
    m1_t0 v1_0 = Q_AMPERSAND DlgDisplayPropertiesData::selectPrecision;
    m1_t1 v1_1 = Q_AMPERSAND DlgDisplayPropertiesData::selectModel;
    m1_t2 v1_2 = Q_AMPERSAND DlgDisplayPropertiesData::applyButtonClicked;
    m1_t3 v1_3 = Q_AMPERSAND DlgDisplayPropertiesData::selectColoringMethod;
    m1_t4 v1_4 = Q_AMPERSAND DlgDisplayPropertiesData::editColor;
    QMetaData *slot_tbl = QMetaObject::new_metadata(5);
    slot_tbl[0].name = "selectPrecision(const QString&)";
    slot_tbl[1].name = "selectModel(const QString&)";
    slot_tbl[2].name = "applyButtonClicked()";
    slot_tbl[3].name = "selectColoringMethod(const QString&)";
    slot_tbl[4].name = "editColor()";
    slot_tbl[0].ptr = *((QMember*)&v1_0);
    slot_tbl[1].ptr = *((QMember*)&v1_1);
    slot_tbl[2].ptr = *((QMember*)&v1_2);
    slot_tbl[3].ptr = *((QMember*)&v1_3);
    slot_tbl[4].ptr = *((QMember*)&v1_4);
    metaObj = QMetaObject::new_metaobject(
	"DlgDisplayPropertiesData", "QDialog",
	slot_tbl, 5,
	0, 0 );
}
