/****************************************************************************
** Mainframe meta object code from reading C++ file 'mainframe.h'
**
** Created: Fri Jan 14 20:57:21 2000
**      by: The Qt Meta Object Compiler ($Revision: 1.7 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_Mainframe
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 3
#elif Q_MOC_OUTPUT_REVISION != 3
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "mainframe.h"
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
    typedef void(Mainframe::*m1_t3)();
    typedef void(Mainframe::*m1_t4)();
    typedef void(Mainframe::*m1_t5)();
    typedef void(Mainframe::*m1_t6)();
    typedef void(Mainframe::*m1_t7)();
    typedef void(Mainframe::*m1_t8)();
    typedef void(Mainframe::*m1_t9)();
    typedef void(Mainframe::*m1_t10)();
    typedef void(Mainframe::*m1_t11)();
    typedef void(Mainframe::*m1_t12)();
    typedef void(Mainframe::*m1_t13)();
    typedef void(Mainframe::*m1_t14)();
    typedef void(Mainframe::*m1_t15)();
    typedef void(Mainframe::*m1_t16)();
    typedef void(Mainframe::*m1_t17)();
    typedef void(Mainframe::*m1_t18)();
    typedef void(Mainframe::*m1_t19)();
    typedef void(Mainframe::*m1_t20)();
    typedef void(Mainframe::*m1_t21)();
    typedef void(Mainframe::*m1_t22)();
    m1_t0 v1_0 = Q_AMPERSAND Mainframe::checkMenuEntries;
    m1_t1 v1_1 = Q_AMPERSAND Mainframe::importPDB;
    m1_t2 v1_2 = Q_AMPERSAND Mainframe::importHIN;
    m1_t3 v1_3 = Q_AMPERSAND Mainframe::exportPovray;
    m1_t4 v1_4 = Q_AMPERSAND Mainframe::cut;
    m1_t5 v1_5 = Q_AMPERSAND Mainframe::copy;
    m1_t6 v1_6 = Q_AMPERSAND Mainframe::paste;
    m1_t7 v1_7 = Q_AMPERSAND Mainframe::erase;
    m1_t8 v1_8 = Q_AMPERSAND Mainframe::clearClipboard;
    m1_t9 v1_9 = Q_AMPERSAND Mainframe::select;
    m1_t10 v1_10 = Q_AMPERSAND Mainframe::deselect;
    m1_t11 v1_11 = Q_AMPERSAND Mainframe::checkResidue;
    m1_t12 v1_12 = Q_AMPERSAND Mainframe::buildBonds;
    m1_t13 v1_13 = Q_AMPERSAND Mainframe::addHydrogens;
    m1_t14 v1_14 = Q_AMPERSAND Mainframe::openDisplayPropertiesDialog;
    m1_t15 v1_15 = Q_AMPERSAND Mainframe::openPreferencesDialog;
    m1_t16 v1_16 = Q_AMPERSAND Mainframe::centerCamera;
    m1_t17 v1_17 = Q_AMPERSAND Mainframe::about;
    m1_t18 v1_18 = Q_AMPERSAND Mainframe::applyDisplayPropertiesDialog;
    m1_t19 v1_19 = Q_AMPERSAND Mainframe::applyPreferencesDialog;
    m1_t20 v1_20 = Q_AMPERSAND Mainframe::startServer;
    m1_t21 v1_21 = Q_AMPERSAND Mainframe::stopServer;
    m1_t22 v1_22 = Q_AMPERSAND Mainframe::toggleServer;
    QMetaData *slot_tbl = QMetaObject::new_metadata(23);
    slot_tbl[0].name = "checkMenuEntries()";
    slot_tbl[1].name = "importPDB()";
    slot_tbl[2].name = "importHIN()";
    slot_tbl[3].name = "exportPovray()";
    slot_tbl[4].name = "cut()";
    slot_tbl[5].name = "copy()";
    slot_tbl[6].name = "paste()";
    slot_tbl[7].name = "erase()";
    slot_tbl[8].name = "clearClipboard()";
    slot_tbl[9].name = "select()";
    slot_tbl[10].name = "deselect()";
    slot_tbl[11].name = "checkResidue()";
    slot_tbl[12].name = "buildBonds()";
    slot_tbl[13].name = "addHydrogens()";
    slot_tbl[14].name = "openDisplayPropertiesDialog()";
    slot_tbl[15].name = "openPreferencesDialog()";
    slot_tbl[16].name = "centerCamera()";
    slot_tbl[17].name = "about()";
    slot_tbl[18].name = "applyDisplayPropertiesDialog()";
    slot_tbl[19].name = "applyPreferencesDialog()";
    slot_tbl[20].name = "startServer()";
    slot_tbl[21].name = "stopServer()";
    slot_tbl[22].name = "toggleServer()";
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
    slot_tbl[14].ptr = *((QMember*)&v1_14);
    slot_tbl[15].ptr = *((QMember*)&v1_15);
    slot_tbl[16].ptr = *((QMember*)&v1_16);
    slot_tbl[17].ptr = *((QMember*)&v1_17);
    slot_tbl[18].ptr = *((QMember*)&v1_18);
    slot_tbl[19].ptr = *((QMember*)&v1_19);
    slot_tbl[20].ptr = *((QMember*)&v1_20);
    slot_tbl[21].ptr = *((QMember*)&v1_21);
    slot_tbl[22].ptr = *((QMember*)&v1_22);
    metaObj = QMetaObject::new_metaobject(
	"Mainframe", "QWidget",
	slot_tbl, 23,
	0, 0 );
}
