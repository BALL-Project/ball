// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: editOperationDialog.C,v 1.6 2005/09/07 13:57:20 anne Exp $

#include <BALL/VIEW/DIALOGS/editOperationDialog.h>
#include <BALL/VIEW/WIDGETS/editableScene.h>

#include <qtooltip.h>
#include <qbuttongroup.h>
#include <qbutton.h>
#include <qlistbox.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/VIEW/KERNEL/compositeManager.h>

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
				connect(scene, SIGNAL(invalidComposite(Composite*)),
								this,    SLOT(invalidateComposite(Composite*)));
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
					
					for(int i = 0 ; i < number_of_undo_operations; i++)
					{
						//remove EditOperation from list_of_operations
						EditableScene::EditOperation undo_operation = list_of_operations_.back();
						list_of_operations_.pop_back();
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
			list_of_operations_.push_back(eo);
			
			undo_operation_list->clearSelection();
			undo_operation_list->insertItem(eo.description.c_str());
			undo_operation_list->setCurrentItem(undo_operation_list->count()-1);
		}
	
		
		void EditOperationDialog::invalidateComposite(Composite* composite)
		{
			if(RTTI::isKindOf<Atom>(*composite))
			{	
				Atom* atom  = dynamic_cast<Atom*>(composite);
				removeEditOperationFromList_(atom);
			}
			else if(RTTI::isKindOf<Bond>(*composite))
			{
				Bond* bond  = dynamic_cast<Bond*>(composite);
				removeEditOperationFromList_(bond);
			}
			else if (RTTI::isKindOf<AtomContainer>(*composite)) 
				//if it is an atomcontainer itself 
			{
				Composite::ChildCompositeIterator cci;
				for(cci = composite->beginChildComposite(); +cci; ++cci)
				{
					// Maybe it would suffice to just iterate over all atoms and bonds here?
					invalidateComposite(cci);
				}
			}
			else //something different from AtomContainer or atom
			{
				Log.error() << " Deleted composite is neither atom nor atomcontainer" << std::endl; 
			}
				
			// now rebuild the edit operation listview
			undo_operation_list->clear();
			for (Position i = 0; i < list_of_operations_.size(); i++)
			{
				undo_operation_list->insertItem(list_of_operations_[i].description.c_str());
			}
		}

		
		bool EditOperationDialog::removeEditOperationFromList_(Atom* atom)
		{	
			vector<Position> to_delete;
			
			for (Position i=0; i < list_of_operations_.size(); i++) 
			{
				// Does the current edit operation contain an atom?
				if (list_of_operations_[i].operationType == EditableScene::EditOperation::ADDED__ATOM) 
				{
					if (list_of_operations_[i].atom == atom) // the current atom has been invalidated
					{
						// delete the current item from the list of edit operations
						to_delete.push_back(i);

					}
				}
				else if (list_of_operations_[i].operationType == EditableScene::EditOperation::MERGED__MOLECULES) 
				{	
					Position atom_to_delete;
					std::vector<Atom*> mol=list_of_operations_[i].molecule;
					//check if the deleted atom is contained in one of the merged molecules
					for( Position j=0; j < mol.size(); j++)
					{
						if(mol[j] == atom)
						{
							// The merge has to be deleted, if the deleted atom is the last atom in this molecule
							if(mol.size() == (Position)1)
							{
								// delete the current item from the list of edit operations
								to_delete.push_back(i);
								break;
							}
							else 
							{	
								// Otherwise we just have to delete this atom from the molecule
								atom_to_delete = j;
								break;
							}
						}
					}
					
					if(atom_to_delete!=0)
					{ // we have to rebuild the molecule list
						std::vector<Atom*> new_molecule;
						for(Position j=0; j< mol.size(); j++)
						{	
							if (atom_to_delete != j)
							{
								new_molecule.push_back(mol[j]);
							}
						}
						// insert the new list
						list_of_operations_[i].molecule = new_molecule;

					}// end if
					
				}// end of MERGED__MOLECULES
				
			} //end of loop 

			if (to_delete.size() > 0)
			{
				sort(to_delete.begin(), to_delete.end());
				vector<EditableScene::EditOperation> new_list_of_operations;
				Position to_delete_index = 0;

				for (Position i = 0; i < list_of_operations_.size(); i++)
				{
					if (to_delete[to_delete_index] != i)
					{
						new_list_of_operations.push_back(list_of_operations_[i]);
					}
					else
					{
						// we have skipped one of the elements
						to_delete_index++;
					}
				}

				// finally replace the old operations with the new ones
				list_of_operations_ = new_list_of_operations;

				// now we have to delete all the invalid bonds
				// delete all the bonds
				for (Atom::BondIterator bi = atom->beginBond(); +bi; ++bi)
				{
					removeEditOperationFromList_(&(*bi));
				}
				return true;
			}

			// the atom was not in the list
			return false;
		}


		bool EditOperationDialog::removeEditOperationFromList_(Bond* bond)
		{	
			
			vector<Position> to_delete;

			for (Position i=0; i < list_of_operations_.size(); i++) 
			{
				// Does the current edit operation contain a bond?
				if ( list_of_operations_[i].operationType == EditableScene::EditOperation::ADDED__BOND ) 
					//later we will remove things like CHANGED__BOND__TYPE
				{
					if (list_of_operations_[i].bond == bond) // the current bond has been invalidated
					{
						// delete the current item from the list of edit operations
						to_delete.push_back(i);
					}
				}		
			}	

			if (to_delete.size() > 0)
			{
				sort(to_delete.begin(), to_delete.end());
				// TODO: This is probably too expensive... there should be a better way :-)
				vector<EditableScene::EditOperation> new_list_of_operations;
				Position to_delete_index = 0;

				for (Position i = 0; i < list_of_operations_.size(); i++)
				{
					if (to_delete[to_delete_index] != i)
					{
						new_list_of_operations.push_back(list_of_operations_[i]);
					}
					else
					{
						// we have skipped one of the elements
						to_delete_index++;
					}
				}
				
				// finally replace the old operations with the new ones
				list_of_operations_ = new_list_of_operations;

				return true;
		
			}
			
			return false; 
		}
 
	} //end of VIEW namespace
	
}//end of BALL namespace
			
