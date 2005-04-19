// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: editOperationDialog.C,v 1.2 2005/04/19 13:28:03 anne Exp $

#include <BALL/VIEW/DIALOGS/editOperationDialog.h>
#include <BALL/VIEW/WIDGETS/editableScene.h>

#include <qtooltip.h>
#include <qbuttongroup.h>
#include <qbutton.h>
#include <qlistbox.h>


namespace BALL
{
	namespace VIEW
	{
		EditOperationDialog::EditOperationDialog(QWidget* parent, const char* name, WFlags fl)
			: EditOperationDialogData(parent, name, fl),
				ModularWidget(name),
				list_of_operations_()
		{	
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
			undo_operation_list->setSelectionMode(QListBox::Extended);	
		}


		EditOperationDialog::~EditOperationDialog()
			throw()
		{
		}

			// ------------------------- SLOTS ------------------------------------------------
			// --------------------------------------------------------------------------------


		void EditOperationDialog::undo()
		{
			signed int number_of_items = undo_operation_list->count(); 
			
			// compute number of smallest highlighted item
			int smallest_highlighted_item = 0; 
			while(    (smallest_highlighted_item < number_of_items) 
					   && (!undo_operation_list->isSelected(smallest_highlighted_item)))
			{
				smallest_highlighted_item++;
			}
				
			// check whether the undo selection is valid
			if( smallest_highlighted_item < number_of_items )
			{
				EditableScene* scene = EditableScene::getInstance(0);

				if (scene == 0)
				{
					Log.error() << "Expected an EditableScene, but found none!" << std::endl;
				}
				else
				{ 
					int number_of_undo_operations=number_of_items - smallest_highlighted_item;
					////////////////////////////scene->undo(number_of_undo_operations);
					
					for(int i = 0 ; i < number_of_undo_operations; i++)
					{
						//remove EditOperation from list_of_operations
						EditableScene::EditOperation undo_operation = list_of_operations_.top();
						list_of_operations_.pop();
						undo_operation.undo(scene);
						
						//remove EditOperation from undo_list in Dialog
						undo_operation_list->removeItem(number_of_items - number_of_undo_operations);
					}
					
					//highlighting of the last operation is done automatically
				}
			}
		}

		void EditOperationDialog::operationSelected(int operation)
		{
			//disconnect the signal for selection to inhibit recursive loops
			disconnect( undo_operation_list, SIGNAL( highlighted(int) ), this, SLOT( operationSelected(int) ));
			signed int number_of_items = undo_operation_list->count(); 
			undo_operation_list->clearSelection();
			// Warning: we _have_ to use signed ints here, since the first item has index
			// 0, and our loop would never terminate
			for(signed int i = number_of_items-1; i >= operation; i--)
			{	
				undo_operation_list->setSelected(i, true);
			}
			//reconnect the signal highlighted with slot operationSelected
			connect( undo_operation_list, SIGNAL( highlighted(int) ), this, SLOT( operationSelected(int) ) );
		}

		
		void EditOperationDialog::addEditOperation(EditableScene::EditOperation& eo)
		{
			//store new EditOperation in list_of_Operations 
			list_of_operations_.push(eo);
			
			undo_operation_list->clearSelection();
			undo_operation_list->insertItem(eo.description.c_str());
			undo_operation_list->setCurrentItem(undo_operation_list->count()-1);
		}


	}
}
