// $Id:

#ifndef BALL_MOLVIEW_GUI_DIALOGS_LABELPROPERTIESDATA_H
#define BALL_MOLVIEW_GUI_DIALOGS_LABELPROPERTIESDATA_H

#include <qdialog.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlineedit.h>

namespace BALL
{

	namespace MOLVIEW
	{

		class LabelPropertiesData : public QDialog
		{
			Q_OBJECT
				
			public:
			
			LabelPropertiesData(QWidget *parent = NULL, const char *name = NULL);
			
			virtual ~LabelPropertiesData();
			
			protected slots:

			virtual void applyButtonClicked() =0;
			virtual void editColor() =0;
			
      protected:
			
			QLineEdit *label_edit_;
			QLabel *color_sample_;
			QPushButton *apply_button_;
		};

		//#		ifndef BALL_NO_INLINE_FUNCTIONS
		//#			include <BALL/MOLVIEW/GUI/DIALOGS/labelPropertiesData.iC>
		//#		endif 

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_GUI_DIALOGS_LABELPROPERTIESDATA_H

