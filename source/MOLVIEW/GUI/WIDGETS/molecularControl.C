// $Id: molecularControl.C,v 1.6.4.5 2002/11/28 21:19:10 amoll Exp $

#include <BALL/MOLVIEW/GUI/WIDGETS/molecularControl.h>
#include <BALL/MOLVIEW/KERNEL/molecularMessage.h>
#include <BALL/MOLVIEW/GUI/DIALOGS/atomSettings.h>
#include <BALL/KERNEL/system.h>

#include <qpopupmenu.h>
#include <qmenubar.h>

using std::endl;

namespace BALL
{
	namespace MOLVIEW
	{

MolecularControl::MolecularControl
  (QWidget* parent, const char* name)
	throw()
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
	throw()
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
				break;
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

Information& MolecularControl::getInformationVisitor_()
	throw()
{
	return molecular_information_;
}

void MolecularControl::recurseGeneration_(QListViewItem* item, Composite* composite)
	throw()
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
	throw()
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
	throw()
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
	/*
	else if (RTTI::isKindOf<NewSelectionMessage> (*message))
	{
		setSelection_();
	}
*/
	return update;
}

void MolecularControl::buildContextMenu(Composite* composite, QListViewItem* item)
	throw()
{
	Control::buildContextMenu(composite, item);

	enum MenuEntries
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
		DISPLAY__CHANGE              = 50,
		ATOM__PROPERTIES						 = 60
	};

	// build the context menu
	if (RTTI::isKindOf<Residue>(*composite) || 
			RTTI::isKindOf<System>(*composite) ||
			RTTI::isKindOf<Protein>(*composite) ||
			RTTI::isKindOf<Molecule>(*composite) ||
			RTTI::isKindOf<Chain>(*composite) ||
			RTTI::isKindOf<SecondaryStructure>(*composite) ||
			RTTI::isKindOf<Fragment>(*composite))
	{
		insertContextMenuEntry("check Residue", this, SLOT(checkResidue()), RESIDUE__CHECK);
		context_menu_.insertSeparator();
		insertContextMenuEntry("cut", this, SLOT(cut()), OBJECT__CUT);
		insertContextMenuEntry("copy", this, SLOT(copy()), OBJECT__COPY);
		insertContextMenuEntry("paste", this, SLOT(paste()), OBJECT__PASTE);
		context_menu_.setItemEnabled(OBJECT__PASTE, getCopyList_().size() > 0);
		
		context_menu_.insertSeparator();
		insertContextMenuEntry("build Bonds", this, SLOT(buildBonds()), BONDS__BUILD);
		insertContextMenuEntry("remove Bonds", this, SLOT(removeBonds()), BONDS__REMOVE);

		context_menu_.insertSeparator();
		insertContextMenuEntry("select", this, SLOT(select()), SELECT);
		insertContextMenuEntry("deselect", this, SLOT(deselect()), DESELECT);
		context_menu_.setItemEnabled(SELECT,   !composite->isSelected());
		context_menu_.setItemEnabled(DESELECT,  composite->isSelected());

		context_menu_.insertSeparator();
		insertContextMenuEntry("center Camera", this, SLOT(centerCamera()), CAMERA__CENTER);
	}

	if (RTTI::isKindOf<Atom>(*composite))
	{
		insertContextMenuEntry("atom properties", this, SLOT(atomProperties((Atom*) composite)), ATOM__PROPERTIES);
	}
}

void MolecularControl::atomProperties_(Atom* atom)
	throw()
{
	AtomSettings as(atom, this);
	as.exec();
}
	

	} // namespace MOLVIEW

} // namespace BALL
