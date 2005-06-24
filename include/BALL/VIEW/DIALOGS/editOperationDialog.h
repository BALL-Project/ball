// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: editOperationDialog.h,v 1.3 2005/06/24 19:06:22 anne Exp $ 
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

#include <vector>

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
				void invalidateComposite(Composite* composite);  //TODO: * oder & 
				
			protected:
				//undo stack
				std::vector< EditableScene::EditOperation > list_of_operations_;
				bool removeEditOperationFromList_(Atom* atom);
				bool removeEditOperationFromList_(Bond* bond);
				
		};
	}
}

#endif
