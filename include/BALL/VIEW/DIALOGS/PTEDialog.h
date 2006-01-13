// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: PTEDialog.h,v 1.3.2.1 2006/01/13 15:35:21 amoll Exp $ 
//

#ifndef BALL_VIEW_DIALOGS_PTEDIALOG_H
#define BALL_VIEW_DIALOGS_PTEDIALOG_H

#include <BALL/VIEW/UIC/PTEDialogData.h>

#ifndef BALL_VIEW_WIDGETS_DOCKWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

namespace BALL
{
	namespace VIEW
	{
		/** Dialog for choosing the element type for the edit mode of scene.
		 		\ingroups ViewDialogs
		*/
		class BALL_VIEW_EXPORT PTEDialog
			: public QDialog,
				public Ui_PTEDialogData,
				public ModularWidget
	  {
			Q_OBJECT

			public:
			
			BALL_EMBEDDABLE(PTEDialogData,ModularWidget)

			///
			PTEDialog(QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0);

			///
			~PTEDialog()
				throw();
			
			///
			virtual void onNotify(Message *message)
				throw();

			public slots:
				
				void newElementType(int elementNumber);

			protected:
				
				QString atomProperties_(int number);
				QColor button_standard_color_;

		};
	}
}

#endif
