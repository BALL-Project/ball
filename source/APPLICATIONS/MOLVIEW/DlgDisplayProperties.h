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

    protected slots:

    virtual void selectPrecision(const QString& string);
    virtual void selectModel(const QString& string);
    virtual void selectColoringMethod(const QString& string);
    virtual void applyButtonClicked();
    virtual void editColor();

    signals:

		void apply();

    private:

		MoleculeObjectProcessor *object_processor_;
};
#endif // DlgDisplayProperties_included
