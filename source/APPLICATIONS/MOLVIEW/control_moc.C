/****************************************************************************
** Control meta object code from reading C++ file 'control.h'
**
** Created: Thu Dec 30 18:22:22 1999
**      by: The Qt Meta Object Compiler ($Revision: 1.2 $)
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
    typedef void(Control::*m1_t1)(QListViewItem*);
    typedef void(Control::*m1_t2)();
    typedef void(Control::*m1_t3)();
    typedef void(Control::*m1_t4)();
    typedef void(Control::*m1_t5)();
    typedef void(Control::*m1_t6)();
    typedef void(Control::*m1_t7)();
    typedef void(Control::*m1_t8)();
    typedef void(Control::*m1_t9)();
    typedef void(Control::*m1_t10)();
    typedef void(Control::*m1_t11)();
    typedef void(Control::*m1_t12)();
    typedef void(Control::*m1_t13)();
    m1_t0 v1_0 = Q_AMPERSAND Control::ContextMenu;
    m1_t1 v1_1 = Q_AMPERSAND Control::objectSelected;
    m1_t2 v1_2 = Q_AMPERSAND Control::cut;
    m1_t3 v1_3 = Q_AMPERSAND Control::copy;
    m1_t4 v1_4 = Q_AMPERSAND Control::paste;
    m1_t5 v1_5 = Q_AMPERSAND Control::buildBonds;
    m1_t6 v1_6 = Q_AMPERSAND Control::removeBonds;
    m1_t7 v1_7 = Q_AMPERSAND Control::select;
    m1_t8 v1_8 = Q_AMPERSAND Control::deselect;
    m1_t9 v1_9 = Q_AMPERSAND Control::checkResidue;
    m1_t10 v1_10 = Q_AMPERSAND Control::removeObject;
    m1_t11 v1_11 = Q_AMPERSAND Control::centerCamera;
    m1_t12 v1_12 = Q_AMPERSAND Control::changeDisplay;
    m1_t13 v1_13 = Q_AMPERSAND Control::clearClipboard;
    QMetaData *slot_tbl = QMetaObject::new_metadata(14);
    slot_tbl[0].name = "ContextMenu(QListViewItem*,const QPoint&,int)";
    slot_tbl[1].name = "objectSelected(QListViewItem*)";
    slot_tbl[2].name = "cut()";
    slot_tbl[3].name = "copy()";
    slot_tbl[4].name = "paste()";
    slot_tbl[5].name = "buildBonds()";
    slot_tbl[6].name = "removeBonds()";
    slot_tbl[7].name = "select()";
    slot_tbl[8].name = "deselect()";
    slot_tbl[9].name = "checkResidue()";
    slot_tbl[10].name = "removeObject()";
    slot_tbl[11].name = "centerCamera()";
    slot_tbl[12].name = "changeDisplay()";
    slot_tbl[13].name = "clearClipboard()";
    slot_tbl[0].ptr = *((QMember*)&v1_0);
    slot_tbl[1].ptr = *((QMember*)&v1_1);
    slot_tbl[2].ptr = *((QMember*)&v1_2);
    slot_tbl[3].ptr = *((QMember*)&v1_3);
    slot_tbl[4].ptr = *((QMember*)&v1_4);
    slot_tbl[5].ptr = *((QMember*)&v1_5);
    slot_tbl[6].ptr = *((QMember*)&v1_6);
    slot_tbl[7].ptr = *((QMember*)&v1_7);
    slot_tbl[8].ptr = *((QMember*)&v1_8);
    slot_tbl[9].ptr = *((QMember*)&v1_9);
    slot_tbl[10].ptr = *((QMember*)&v1_10);
    slot_tbl[11].ptr = *((QMember*)&v1_11);
    slot_tbl[12].ptr = *((QMember*)&v1_12);
    slot_tbl[13].ptr = *((QMember*)&v1_13);
    typedef void(Control::*m2_t0)(QString);
    typedef void(Control::*m2_t1)(bool,bool);
    typedef void(Control::*m2_t2)(bool);
    m2_t0 v2_0 = Q_AMPERSAND Control::writeText;
    m2_t1 v2_1 = Q_AMPERSAND Control::itemSelected;
    m2_t2 v2_2 = Q_AMPERSAND Control::itemCutOrCopied;
    QMetaData *signal_tbl = QMetaObject::new_metadata(3);
    signal_tbl[0].name = "writeText(QString)";
    signal_tbl[1].name = "itemSelected(bool,bool)";
    signal_tbl[2].name = "itemCutOrCopied(bool)";
    signal_tbl[0].ptr = *((QMember*)&v2_0);
    signal_tbl[1].ptr = *((QMember*)&v2_1);
    signal_tbl[2].ptr = *((QMember*)&v2_2);
    metaObj = QMetaObject::new_metaobject(
	"Control", "QListView",
	slot_tbl, 14,
	signal_tbl, 3 );
}

// SIGNAL writeText
void Control::writeText( QString t0 )
{
    activate_signal_string( "writeText(QString)", t0 );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL itemSelected
void Control::itemSelected( bool t0, bool t1 )
{
    // No builtin function for signal parameter type bool,bool
    QConnectionList *clist = receivers("itemSelected(bool,bool)");
    if ( !clist || signalsBlocked() )
	return;
    typedef void (QObject::*RT0)();
    typedef RT0 *PRT0;
    typedef void (QObject::*RT1)(bool);
    typedef RT1 *PRT1;
    typedef void (QObject::*RT2)(bool,bool);
    typedef RT2 *PRT2;
    RT0 r0;
    RT1 r1;
    RT2 r2;
    QConnectionListIt it(*clist);
    QConnection   *c;
    QSenderObject *object;
    while ( (c=it.current()) ) {
	++it;
	object = (QSenderObject*)c->object();
	object->setSender( this );
	switch ( c->numArgs() ) {
	    case 0:
		r0 = *((PRT0)(c->member()));
		(object->*r0)();
		break;
	    case 1:
		r1 = *((PRT1)(c->member()));
		(object->*r1)(t0);
		break;
	    case 2:
		r2 = *((PRT2)(c->member()));
		(object->*r2)(t0, t1);
		break;
	}
    }
}

// SIGNAL itemCutOrCopied
void Control::itemCutOrCopied( bool t0 )
{
    activate_signal_bool( "itemCutOrCopied(bool)", t0 );
}
