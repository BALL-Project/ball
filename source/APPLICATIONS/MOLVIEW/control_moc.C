/****************************************************************************
** Control meta object code from reading C++ file 'control.h'
**
** Created: Fri Jan 14 12:14:43 2000
**      by: The Qt Meta Object Compiler ($Revision: 1.6 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_Control
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 3
#elif Q_MOC_OUTPUT_REVISION != 3
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "control.h"
#include <qmetaobject.h>
#include <qapplication.h>

#if defined(Q_SPARCWORKS_FUNCP_BUG)
#define Q_AMPERSAND
#else
#define Q_AMPERSAND &
#endif


const char *Control::className() const
{
    return "Control";
}

QMetaObject *Control::metaObj = 0;


#if QT_VERSION >= 199
static QMetaObjectInit init_Control(&Control::staticMetaObject);

#endif

void Control::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(QListView::className(), "QListView") != 0 )
	badSuperclassWarning("Control","QListView");

#if QT_VERSION >= 199
    staticMetaObject();
}

QString Control::tr(const char* s)
{
    return ((QNonBaseApplication*)qApp)->translate("Control",s);
}

void Control::staticMetaObject()
{
    if ( metaObj )
	return;
    QListView::staticMetaObject();
#else

    QListView::initMetaObject();
#endif

    typedef void(Control::*m1_t0)(QListViewItem*,const QPoint&,int);
    typedef void(Control::*m1_t1)();
    m1_t0 v1_0 = Q_AMPERSAND Control::ContextMenu;
    m1_t1 v1_1 = Q_AMPERSAND Control::invalidateSelection;
    QMetaData *slot_tbl = QMetaObject::new_metadata(2);
    slot_tbl[0].name = "ContextMenu(QListViewItem*,const QPoint&,int)";
    slot_tbl[1].name = "invalidateSelection()";
    slot_tbl[0].ptr = *((QMember*)&v1_0);
    slot_tbl[1].ptr = *((QMember*)&v1_1);
    typedef void(Control::*m2_t0)();
    typedef void(Control::*m2_t1)();
    typedef void(Control::*m2_t2)();
    typedef void(Control::*m2_t3)();
    typedef void(Control::*m2_t4)();
    typedef void(Control::*m2_t5)();
    typedef void(Control::*m2_t6)();
    m2_t0 v2_0 = Q_AMPERSAND Control::cut;
    m2_t1 v2_1 = Q_AMPERSAND Control::copy;
    m2_t2 v2_2 = Q_AMPERSAND Control::paste;
    m2_t3 v2_3 = Q_AMPERSAND Control::select;
    m2_t4 v2_4 = Q_AMPERSAND Control::deselect;
    m2_t5 v2_5 = Q_AMPERSAND Control::centerCamera;
    m2_t6 v2_6 = Q_AMPERSAND Control::removeObject;
    QMetaData *signal_tbl = QMetaObject::new_metadata(7);
    signal_tbl[0].name = "cut()";
    signal_tbl[1].name = "copy()";
    signal_tbl[2].name = "paste()";
    signal_tbl[3].name = "select()";
    signal_tbl[4].name = "deselect()";
    signal_tbl[5].name = "centerCamera()";
    signal_tbl[6].name = "removeObject()";
    signal_tbl[0].ptr = *((QMember*)&v2_0);
    signal_tbl[1].ptr = *((QMember*)&v2_1);
    signal_tbl[2].ptr = *((QMember*)&v2_2);
    signal_tbl[3].ptr = *((QMember*)&v2_3);
    signal_tbl[4].ptr = *((QMember*)&v2_4);
    signal_tbl[5].ptr = *((QMember*)&v2_5);
    signal_tbl[6].ptr = *((QMember*)&v2_6);
    metaObj = QMetaObject::new_metaobject(
	"Control", "QListView",
	slot_tbl, 2,
	signal_tbl, 7 );
}

// SIGNAL cut
void Control::cut()
{
    activate_signal( "cut()" );
}

// SIGNAL copy
void Control::copy()
{
    activate_signal( "copy()" );
}

// SIGNAL paste
void Control::paste()
{
    activate_signal( "paste()" );
}

// SIGNAL select
void Control::select()
{
    activate_signal( "select()" );
}

// SIGNAL deselect
void Control::deselect()
{
    activate_signal( "deselect()" );
}

// SIGNAL centerCamera
void Control::centerCamera()
{
    activate_signal( "centerCamera()" );
}

// SIGNAL removeObject
void Control::removeObject()
{
    activate_signal( "removeObject()" );
}
