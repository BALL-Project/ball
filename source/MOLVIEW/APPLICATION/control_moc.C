/****************************************************************************
** Control meta object code from reading C++ file 'control.h'
**
** Created: Sun Oct 31 20:11:18 1999
**      by: The Qt Meta Object Compiler ($Revision: 1.1 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_Control
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 3
#elif Q_MOC_OUTPUT_REVISION != 3
#error "Moc format conflict - please regenerate all moc files"
#endif

#include <BALL/MOLVIEW/APPLICATION/control.h>
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
    m1_t0 v1_0 = Q_AMPERSAND Control::ContextMenu;
    QMetaData *slot_tbl = QMetaObject::new_metadata(1);
    slot_tbl[0].name = "ContextMenu(QListViewItem*,const QPoint&,int)";
    slot_tbl[0].ptr = *((QMember*)&v1_0);
    metaObj = QMetaObject::new_metaobject(
	"Control", "QListView",
	slot_tbl, 1,
	0, 0 );
}
