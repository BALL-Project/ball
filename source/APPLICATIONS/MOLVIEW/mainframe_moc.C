/****************************************************************************
** Mainframe meta object code from reading C++ file 'mainframe.h'
**
** Created: Sat Oct 30 14:45:51 1999
**      by: The Qt Meta Object Compiler ($Revision: 1.1 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_Mainframe
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 3
#elif Q_MOC_OUTPUT_REVISION != 3
#error "Moc format conflict - please regenerate all moc files"
#endif

#include <BALL/MOLVIEW/APPLICATION/mainframe.h>
#include <qmetaobject.h>
#include <qapplication.h>

#if defined(Q_SPARCWORKS_FUNCP_BUG)
#define Q_AMPERSAND
#else
#define Q_AMPERSAND &
#endif


const char *Mainframe::className() const
{
    return "Mainframe";
}

QMetaObject *Mainframe::metaObj = 0;


#if QT_VERSION >= 199
static QMetaObjectInit init_Mainframe(&Mainframe::staticMetaObject);

#endif

void Mainframe::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(QWidget::className(), "QWidget") != 0 )
	badSuperclassWarning("Mainframe","QWidget");

#if QT_VERSION >= 199
    staticMetaObject();
}

QString Mainframe::tr(const char* s)
{
    return ((QNonBaseApplication*)qApp)->translate("Mainframe",s);
}

void Mainframe::staticMetaObject()
{
    if ( metaObj )
	return;
    QWidget::staticMetaObject();
#else

    QWidget::initMetaObject();
#endif

    typedef void(Mainframe::*m1_t0)();
    typedef void(Mainframe::*m1_t1)();
    typedef void(Mainframe::*m1_t2)();
    m1_t0 v1_0 = Q_AMPERSAND Mainframe::importPDB;
    m1_t1 v1_1 = Q_AMPERSAND Mainframe::importHIN;
    m1_t2 v1_2 = Q_AMPERSAND Mainframe::about;
    QMetaData *slot_tbl = QMetaObject::new_metadata(3);
    slot_tbl[0].name = "importPDB()";
    slot_tbl[1].name = "importHIN()";
    slot_tbl[2].name = "about()";
    slot_tbl[0].ptr = *((QMember*)&v1_0);
    slot_tbl[1].ptr = *((QMember*)&v1_1);
    slot_tbl[2].ptr = *((QMember*)&v1_2);
    metaObj = QMetaObject::new_metaobject(
	"Mainframe", "QWidget",
	slot_tbl, 3,
	0, 0 );
}
