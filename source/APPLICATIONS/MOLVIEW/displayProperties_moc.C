/****************************************************************************
** DisplayProperties meta object code from reading C++ file 'displayProperties.h'
**
** Created: Sun Jan 9 17:28:46 2000
**      by: The Qt Meta Object Compiler ($Revision: 1.1 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_DisplayProperties
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 3
#elif Q_MOC_OUTPUT_REVISION != 3
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "displayProperties.h"
#include <qmetaobject.h>
#include <qapplication.h>

#if defined(Q_SPARCWORKS_FUNCP_BUG)
#define Q_AMPERSAND
#else
#define Q_AMPERSAND &
#endif


const char *DisplayProperties::className() const
{
    return "DisplayProperties";
}

QMetaObject *DisplayProperties::metaObj = 0;


#if QT_VERSION >= 199
static QMetaObjectInit init_DisplayProperties(&DisplayProperties::staticMetaObject);

#endif

void DisplayProperties::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(QWidget::className(), "QWidget") != 0 )
	badSuperclassWarning("DisplayProperties","QWidget");

#if QT_VERSION >= 199
    staticMetaObject();
}

QString DisplayProperties::tr(const char* s)
{
    return ((QNonBaseApplication*)qApp)->translate("DisplayProperties",s);
}

void DisplayProperties::staticMetaObject()
{
    if ( metaObj )
	return;
    QWidget::staticMetaObject();
#else

    QWidget::initMetaObject();
#endif

    typedef void(DisplayProperties::*m1_t0)(int);
    typedef void(DisplayProperties::*m1_t1)(int);
    typedef void(DisplayProperties::*m1_t2)(int);
    m1_t0 v1_0 = Q_AMPERSAND DisplayProperties::modelSelected;
    m1_t1 v1_1 = Q_AMPERSAND DisplayProperties::modelPrecisionSelected;
    m1_t2 v1_2 = Q_AMPERSAND DisplayProperties::modelDrawingModeSelected;
    QMetaData *slot_tbl = QMetaObject::new_metadata(3);
    slot_tbl[0].name = "modelSelected(int)";
    slot_tbl[1].name = "modelPrecisionSelected(int)";
    slot_tbl[2].name = "modelDrawingModeSelected(int)";
    slot_tbl[0].ptr = *((QMember*)&v1_0);
    slot_tbl[1].ptr = *((QMember*)&v1_1);
    slot_tbl[2].ptr = *((QMember*)&v1_2);
    typedef void(DisplayProperties::*m2_t0)();
    m2_t0 v2_0 = Q_AMPERSAND DisplayProperties::apply;
    QMetaData *signal_tbl = QMetaObject::new_metadata(1);
    signal_tbl[0].name = "apply()";
    signal_tbl[0].ptr = *((QMember*)&v2_0);
    metaObj = QMetaObject::new_metaobject(
	"DisplayProperties", "QWidget",
	slot_tbl, 3,
	signal_tbl, 1 );
}

// SIGNAL apply
void DisplayProperties::apply()
{
    activate_signal( "apply()" );
}
