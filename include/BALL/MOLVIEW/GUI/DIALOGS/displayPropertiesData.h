// $Id:

#ifndef BALL_MOLVIEW_GUI_DIALOGS_DISPLAYPROPERTIESDATA_H
#define BALL_MOLVIEW_GUI_DIALOGS_DISPLAYPROPERTIESDATA_H

#include <qdialog.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qpushbutton.h>

namespace BALL
{

	namespace MOLVIEW
	{

		class DisplayPropertiesData 
			: public QDialog
		{
			Q_OBJECT
				
			public:
			
			DisplayPropertiesData(QWidget *parent = NULL, const char *name = NULL);
			
			virtual ~DisplayPropertiesData();
			
			protected slots:
				
			virtual void selectPrecision(const QString&) =0;
			virtual void selectModel(const QString&) =0;
			virtual void applyButtonClicked() =0;
			virtual void selectColoringMethod(const QString&) =0;
			virtual void editColor() =0;
			
			protected:
			
			QComboBox *coloring_type_combobox_;
			QComboBox *model_type_combobox_;
			QComboBox *mode_resolution_combobox_;
			QLabel *color_sample;
			QPushButton *apply_button_;
		};

	} // namespace MOLVIEW

} // namespace BALL
		
#endif // BALL_MOLVIEW_GUI_DIALOGS_DISPLAYPROPERTIESDATA_H
