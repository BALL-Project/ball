/**********************************************************************

	--- Qt Architect generated file ---

	File: DlgDisplayProperties.h
	Last generated: Mon Jan 10 18:55:27 2000

 *********************************************************************/

#ifndef DlgDisplayProperties_included
#define DlgDisplayProperties_included

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_FORMAT_INIFILE_H
#	include <BALL/FORMAT/INIFile.h>
#endif

#ifndef BALL_VIEW_KERNEL_CONNECTIONOBJECT_H
#	include <BALL/VIEW/KERNEL/connectionObject.h>
#endif

#ifndef BALL_MOLVIEW_GUI_FUNCTOR_MOLECULEOBJECTPROCESSOR_H
# include <BALL/MOLVIEW/GUI/FUNCTOR/moleculeObjectProcessor.h>
#endif

#ifndef BALL_MOLVIEW_KERNEL_MOLECULARMESSAGE_H
# include <BALL/MOLVIEW/KERNEL/molecularMessage.h>
#endif

#include "DlgDisplayPropertiesData.h"

using namespace BALL;
using namespace BALL::VIEW;
using namespace BALL::MOLVIEW;

class DlgDisplayProperties : public DlgDisplayPropertiesData,
														 public ConnectionObject
{
    Q_OBJECT

public:

    DlgDisplayProperties(
        QWidget *parent = NULL,
        const char *name = NULL );

    virtual ~DlgDisplayProperties();

		void setObjectProcessor(const MoleculeObjectProcessor& object_processor);

		void setPreferences(INIFile& inifile) const;

		void getPreferences(const INIFile& inifile);

		virtual void onNotify(Message *message);


    protected slots:

    virtual void selectPrecision(const QString& string);
    virtual void selectModel(const QString& string);
    virtual void selectColoringMethod(const QString& string);
    virtual void applyButtonClicked();
    virtual void editColor();


    private:

		void setComboBoxIndex_(QComboBox* combo_box, QString& item_string);

		QString   model_string_;
		QString   precision_string_;
		QString   coloring_method_string_;
		ColorRGBA custom_color_;

		MoleculeObjectProcessor *object_processor_;

		AtomDistanceColorCalculator distance_color_calculator_;
		bool distance_coloring_;

		List<Composite*> selection_;
};
#endif // DlgDisplayProperties_included
