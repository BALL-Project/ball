// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: editOperationDialog.h,v 1.2 2005/04/19 13:29:12 anne Exp $ 
//

#ifndef BALL_VIEW_DIALOGS_EDITOPERATIONDIALOG_H
#define BALL_VIEW_DIALOGS_EDITOPERATIONDIALOG_H

#include <BALL/VIEW/UIC/editOperationDialogData.h>

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#ifndef BALL_VIEW_WIDGETS_EDITABLESCENE_H
# include <BALL/VIEW/WIDGETS/editableScene.h>
#endif

#include <stack>



namespace BALL
{
	namespace VIEW
	{
		/** Dialog for storing the undo Operations of EditableScene.
		 		\ingroups ViewDialogs
		*/

		class BALL_EXPORT EditOperationDialog
			: public EditOperationDialogData,
				public ModularWidget
	  {
			Q_OBJECT

			public:
			
			BALL_EMBEDDABLE(EditOperationDialogData,ModularWidget)

			///
			EditOperationDialog(QWidget* parent = 0, const char* name = 0, WFlags fl = 0);

			///
			~EditOperationDialog()
				throw();
		 
		
			public slots:
				
				void undo();
				void operationSelected(int operation);
				void addEditOperation(EditableScene::EditOperation& eo);
				
			protected:
				//undo stack
				std::stack< EditableScene::EditOperation > list_of_operations_;
				
		};
	}
}

#endif
