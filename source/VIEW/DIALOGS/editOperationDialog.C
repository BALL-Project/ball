// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: editOperationDialog.C,v 1.9.2.1 2006/01/13 15:35:49 amoll Exp $

#include <BALL/VIEW/DIALOGS/editOperationDialog.h>
#include <BALL/VIEW/WIDGETS/editableScene.h>

#include <qtooltip.h>
#include <q3buttongroup.h>
#include <q3button.h>
#include <q3listbox.h>

namespace BALL
{
	namespace VIEW
	{
		EditOperationDialog::EditOperationDialog(QWidget* parent, const char* name, Qt::WFlags fl)
			: QWidget(parent, name, fl),
				Ui_EditOperationDialogData(),
				ModularWidget(name)
		{	
			setupUi(this);

			EditableScene* scene = EditableScene::getInstance(0);

			if (scene == 0)
			{
				Log.error() << "Expected an EditableScene, but found none!" << std::endl;
			}
			else
			{ 
				connect(scene, SIGNAL(newEditOperation(EditableScene::EditOperation&)),
								this,    SLOT(addEditOperation(EditableScene::EditOperation&)));
			}
			undo_operation_list->setSelectionMode(Q3ListBox::Extended);	
		}


		EditOperationDialog::~EditOperationDialog()
			throw()
		{
		}

			// ------------------------- SLOTS ------------------------------------------------
			// --------------------------------------------------------------------------------


		void EditOperationDialog::undo(int number_of_undo_steps)
		{
			// TODO: Implementation :-)
		}

		void EditOperationDialog::operationSelected(int operation)
		{
			int number_of_items = undo_operation_list->count(); 
			for(Position i = number_of_items-1; i > (Position) operation; i--)
			{	
				Log.info() << "Hallo: " << i<<std::endl;
				undo_operation_list->setSelected(i, true);
			}
		}

		void EditOperationDialog::addEditOperation(EditableScene::EditOperation& eo)
		{
			undo_operation_list->insertItem(eo.description.c_str());
		}


	}
}
