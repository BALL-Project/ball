/**********************************************************************

	--- Qt Architect generated file ---

	File: DlgDisplayProperties.h
	Last generated: Mon Jan 10 18:55:27 2000

 *********************************************************************/

#ifndef DlgDisplayProperties_included
#define DlgDisplayProperties_included

#ifndef BALL_MOLVIEW_OPENGL_FUNCTOR_MOLECULEOBJECTPROCESSOR_H
# include <BALL/MOLVIEW/OPENGL/FUNCTOR/moleculeObjectProcessor.h>
#endif

#ifndef BALL_FORMAT_INIFILE_H
#	include <BALL/FORMAT/INIFile.h>
#endif

#include "DlgDisplayPropertiesData.h"

class DlgDisplayProperties : public DlgDisplayPropertiesData
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

    protected slots:

    virtual void selectPrecision(const QString& string);
    virtual void selectModel(const QString& string);
    virtual void selectColoringMethod(const QString& string);
    virtual void applyButtonClicked();
    virtual void editColor();

    signals:

		void apply();

    private:

		void setComboBoxIndex_(QComboBox* combo_box, QString& item_string);

		QString   model_string_;
		QString   precision_string_;
		QString   coloring_method_string_;
		ColorRGBA custom_color_;

		MoleculeObjectProcessor *object_processor_;
};
#endif // DlgDisplayProperties_included
