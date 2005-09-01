// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: PTEDialog.h,v 1.2.2.1 2005/09/01 22:17:42 amoll Exp $ 
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
			: public PTEDialogData,
				public ModularWidget
	  {
			Q_OBJECT

			public:
			
			BALL_EMBEDDABLE(PTEDialogData,ModularWidget)

			///
			PTEDialog(QWidget* parent = 0, const char* name = 0, WFlags fl = 0);

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
