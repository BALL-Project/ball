// $Id: molecularControl.C,v 1.5 2001/01/26 01:37:12 amoll Exp $

#include <BALL/MOLVIEW/GUI/WIDGETS/molecularControl.h>
#include <qpopupmenu.h>
#include <qmenubar.h>

using namespace std;

namespace BALL
{

	namespace MOLVIEW
	{

MolecularControl::MolecularControl
  (QWidget* parent, const char* name)
		:	Control(parent, name),
			molecular_information_(),
			molecular_filter_()
{
}

MolecularControl::~MolecularControl()
	throw()
{
}

void MolecularControl::checkMenu(MainControl& main_control)
{
	bool copy_list_filled = (getCopyList_().size() > 0);

	// check for paste-slot: enable only if copy_list_ not empty
	(main_control.menuBar())->setItemEnabled(paste_id_, copy_list_filled);	

	// check for clearClipboard-slot: enable only if copy_list_ not empty
	(main_control.menuBar())->setItemEnabled(clipboard_id_, copy_list_filled);

	// check for cut/copy-slot: enable only if all selected composites
	// are systems
	bool list_filled = (getSelection().size() > 0);

	if (list_filled)
	{
		List<Composite*>::ConstIterator list_it = getSelection().begin();	
		for (; list_it != getSelection().end(); ++list_it)
		{
			if (!RTTI::isKindOf<System>(**list_it))
			{
				list_filled = false;
			}
		}
	}

	// cut, copy, and paste will are only available for 
	// top level selections
	(main_control.menuBar())->setItemEnabled(cut_id_, list_filled);
	(main_control.menuBar())->setItemEnabled(copy_id_, list_filled);	

}

void MolecularControl::sentSelection()
{
	// will be inserted later
	Control::sentSelection();

	// refill and filter selection
	filterSelection_(molecular_filter_);

	// sent new selection through tree
	MolecularSelectionMessage* message = new MolecularSelectionMessage;
	message->setSelection(getSelection());
	message->setDeletable(true);

	notify_(message);
}

void MolecularControl::buildContextMenu(Composite* composite, QListViewItem* item)
{
	Control::buildContextMenu(composite, item);

	// to be added: Context menus for molecular objects
}

Information& MolecularControl::getInformationVisitor_()
{
	return molecular_information_;
}

void MolecularControl::recurseGeneration_(QListViewItem* item, Composite* composite)
{
	// if the composite is anything but an atom,
	// we iterate over all children and recurse
	//	if (!RTTI::isKindOf<Atom>(*composite))
	{
		Composite::ChildCompositeReverseIterator it = composite->rbeginChildComposite();
		for (; it != composite->rendChildComposite(); ++it)
		{
			generateListViewItem_(item, &*it);
		}
	}
}

bool MolecularControl::recurseUpdate_(QListViewItem* item, Composite* composite)
{
	bool tree_updated = false;

	// if the composite is anything but an atom,
	// we iterate over all children and recurse
	//	if (!RTTI::isKindOf<Atom>(*composite))
	{
		Composite::ChildCompositeReverseIterator it = composite->rbeginChildComposite();
		for (; it != composite->rendChildComposite(); ++it)
		{
			tree_updated = (tree_updated || updateListViewItem_(item, &*it));
		}
	}

	return tree_updated;
}

bool MolecularControl::reactToMessages_(Message* message)
{
	bool update = false;

	if (RTTI::isKindOf<NewMolecularMessage>(*message))
	{
		NewMolecularMessage *composite_message 
			= RTTI::castTo<NewMolecularMessage>(*message);

		update = addComposite((Composite *)composite_message->getComposite());
	}
	else if (RTTI::isKindOf<RemovedCompositeMessage>(*message))
  {
    RemovedCompositeMessage *composite_message
      = RTTI::castTo<RemovedCompositeMessage>(*message);

    update = removeComposite((Composite *)composite_message->getComposite());
	}   
	else if  (RTTI::isKindOf<ChangedCompositeMessage>(*message))
  {
    ChangedCompositeMessage *composite_message
      = RTTI::castTo<ChangedCompositeMessage>(*message);

		update = updateComposite(&(composite_message->getComposite()->getRoot()));
	}

	return update;
}







/*
void MolecularControl::ContextMenu
  (QListViewItem* item,  const QPoint& point, int column)
{
	enum
	{
		OBJECT__REMOVE               = 0,
		OBJECT__CUT                  = 1,
		OBJECT__COPY                 = 2,
		OBJECT__PASTE                = 3,
		CAMERA__CENTER               = 10,
		BONDS__BUILD                 = 20,
		BONDS__REMOVE                = 21,
		SELECT                       = 30,
		DESELECT                     = 31,
		RESIDUE__CHECK               = 40,
		DISPLAY__CHANGE              = 50
	};

	if (item == 0)
	{
		return;
	}

	bool no_context_available = false;
	// select the current listviewitem
	setSelected(item, TRUE);

	// get composite address
	Composite* composite = getCompositeAddress_(item);
	
	// storing ptr
	selectedcomposite_ = composite;
	selected__mpQListViewItem_ = item;

	// get names and types
	selected_name__mQString_ = _getName(item);
	selected_root_name__mQString_ = _getRootName(item);
	selected_type__mQString_ = _getTypeName(item);
	selected_root_type__mQString_ = _getRootTypeName(item);

	QPopupMenu __QPopupMenu;

	// build the context menu
	switch (getType(composite))
	{
	  case TYPE__RESIDUE:
	  case TYPE__SYSTEM:
	  case TYPE__PROTEIN:
	  case TYPE__MOLECULE:
	  case TYPE__CHAIN:
	  case TYPE__SECONDARY_STRUCTURE:
	  case TYPE__FRAGMENT:
			{
				__QPopupMenu.insertItem("check Residue", RESIDUE__CHECK);
				__QPopupMenu.insertSeparator();
				__QPopupMenu.insertItem("cut", OBJECT__CUT);
				__QPopupMenu.insertItem("copy", OBJECT__COPY);
				__QPopupMenu.insertItem("paste", OBJECT__PASTE);
				
				if (copiedcomposite_ != 0)
				{
					__QPopupMenu.setItemEnabled(OBJECT__PASTE, TRUE);
				}
				else
				{
					__QPopupMenu.setItemEnabled(OBJECT__PASTE, FALSE);
				}
				
				__QPopupMenu.insertSeparator();
				__QPopupMenu.insertItem("build Bonds", BONDS__BUILD);
				__QPopupMenu.insertItem("remove Bonds", BONDS__REMOVE);
				__QPopupMenu.insertSeparator();
				
				QString __QString("remove ");
				__QString += getTypeName(composite);

				__QPopupMenu.insertItem(__QString, OBJECT__REMOVE);
				__QPopupMenu.insertSeparator();
				__QPopupMenu.insertItem("select", SELECT);
				__QPopupMenu.insertItem("deselect", DESELECT);
	
				if (composite->isSelected())
				{
  				__QPopupMenu.setItemEnabled(SELECT, FALSE);
  				__QPopupMenu.setItemEnabled(DESELECT, TRUE);
				}
				else
				{
					__QPopupMenu.setItemEnabled(SELECT, TRUE);
					__QPopupMenu.setItemEnabled(DESELECT, FALSE);
				}
	
				__QPopupMenu.insertSeparator();
				__QPopupMenu.insertItem("center Camera", CAMERA__CENTER);
			}
			break;

	  default:
			no_context_available__bool = true;
			break;
	}

	if (no_context_available__bool)
	{
		return;
	}

	// execute the action
	switch (__QPopupMenu.exec(point))
	{
	  case RESIDUE__CHECK:
			checkResidue();
			break;

	  case OBJECT__CUT:
			cut();
			break;

	  case OBJECT__COPY:
			copy();
			break;

	  case OBJECT__PASTE:
			paste();
			break;

	  case OBJECT__REMOVE:
			removeObject();
			break;

	  case BONDS__BUILD:
			buildBonds();
			break;

	  case BONDS__REMOVE:
			removeBonds();
			break;

	  case SELECT:
			select();
			break;

	  case DESELECT:
			deselect();
			break;

	  case CAMERA__CENTER:
			centerCamera();
			break;

	  default:
			break;
	}
}
*/


#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/GUI/WIDGETS/molecularControl.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
