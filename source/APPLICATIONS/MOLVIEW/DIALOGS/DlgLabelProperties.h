/**********************************************************************

	--- Qt Architect generated file ---

	File: DlgLabelProperties.h
	Last generated: Sun Jun 4 13:50:59 2000

 *********************************************************************/

#ifndef DlgLabelProperties_included
#define DlgLabelProperties_included

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_FORMAT_INIFILE_H
#	include <BALL/FORMAT/INIFile.h>
#endif

#ifndef BALL_STRUCTURE_GEOMETRICPROPERTIES_H
#	include <BALL/STRUCTURE/geometricProperties.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLOR_H
#	include <BALL/VIEW/DATATYPE/color.h>
#endif

#ifndef BALL_VIEW_KERNEL_CONNECTIONOBJECT_H
#	include <BALL/VIEW/KERNEL/connectionObject.h>
#endif

#ifndef BALL_VIEW_GUI_PRIMITIV_GLLABEL_H
#	include <BALL/VIEW/GUI/PRIMITIV/gllabel.h>
#endif

#ifndef BALL_MOLVIEW_KERNEL_MOLECULARMESSAGE_H
# include <BALL/MOLVIEW/KERNEL/molecularMessage.h>
#endif

#include "DlgLabelPropertiesData.h"

using namespace BALL;
using namespace BALL::VIEW;
using namespace BALL::MOLVIEW;

class DlgLabelProperties : public DlgLabelPropertiesData,
													 public ConnectionObject
{
    Q_OBJECT

 public:

    DlgLabelProperties(
        QWidget *parent = NULL,
        const char *name = NULL );

    virtual ~DlgLabelProperties();

		void setPreferences(INIFile& inifile) const;

		void getPreferences(const INIFile& inifile);

		virtual void onNotify(Message *message);


 protected slots:

    virtual void applyButtonClicked();
    virtual void editColor();

 private:

		ColorRGBA custom_color_;
		List<Composite*> selection_;
};
#endif // DlgLabelProperties_included
