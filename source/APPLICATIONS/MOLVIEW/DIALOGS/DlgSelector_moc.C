/****************************************************************************
** DlgSelector meta object code from reading C++ file 'DlgSelector.h'
**
** Created: Tue Jan 11 19:47:29 2000
**      by: The Qt Meta Object Compiler ($Revision: 1.1 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#define Q_MOC_DlgSelector
#if !defined(Q_MOC_OUTPUT_REVISION)
#define Q_MOC_OUTPUT_REVISION 3
#elif Q_MOC_OUTPUT_REVISION != 3
#error "Moc format conflict - please regenerate all moc files"
#endif

#include "DlgSelector.h"
#include <qmetaobject.h>
#include <qapplication.h>

#if defined(Q_SPARCWORKS_FUNCP_BUG)
#define Q_AMPERSAND
#else
#define Q_AMPERSAND &
#endif


const char *DlgSelector::className() const
{
    return "DlgSelector";
}

QMetaObject *DlgSelector::metaObj = 0;


#if QT_VERSION >= 199
static QMetaObjectInit init_DlgSelector(&DlgSelector::staticMetaObject);

#endif

void DlgSelector::initMetaObject()
{
    if ( metaObj )
	return;
    if ( strcmp(DlgSelectorData::className(), "DlgSelectorData") != 0 )
	badSuperclassWarning("DlgSelector","DlgSelectorData");

#if QT_VERSION >= 199
    staticMetaObject();
}

QString DlgSelector::tr(const char* s)
{
    return ((QNonBaseApplication*)qApp)->translate("DlgSelector",s);
}

void DlgSelector::staticMetaObject()
{
    if ( metaObj )
	return;
    DlgSelectorData::staticMetaObject();
#else

    DlgSelectorData::initMetaObject();
#endif

    metaObj = QMetaObject::new_metaobject(
	"DlgSelector", "DlgSelectorData",
	0, 0,
	0, 0 );
}
