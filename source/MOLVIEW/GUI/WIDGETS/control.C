// $Id: control.C,v 1.1 2000/09/23 15:39:17 hekl Exp $

#include <BALL/MOLVIEW/GUI/WIDGETS/control.h>

using namespace std;

Control::Control
  (QWidget* parent, const char* name)
		:	QListView(parent, name),
			ConnectionObject(),
			selected_(),
			selection_changed_(true)
{
	// appearance
	setRootIsDecorated(TRUE);
	setMultiSelection(TRUE);
	setSorting(-1);

	// if the selection of any item changed,
	// mark the complete selection as invalid
	// it is then re-determined by getSelection()
	connect(this,
					SIGNAL(selectionChanged()),
					this,
					SLOT(newSelection()));
}

Control::~Control()
{
}

bool Control::addComposite
  (Composite* composite, QString* own_name)
{
	if (composite == 0)
	{
		return false;
	}

	// get information about the composite
	MolecularInformation molecular_information_;

	composite->host(molecular_information_);
	// if the own name is empty use name as name
	QString name = molecular_information_.getName().c_str();

	if ((name[0] == '<') && (own_name != 0))
	{
		name = *own_name;
	}

	// generate ListViewItem and insert it into the ListView
	genListViewItem_(0, composite, &name);

	// update the view
  selection_changed_ = true;
	updateContents();

	return true;
}

bool Control::removeComposite(Composite* composite)
{
	QListViewItem* item = findListViewItem_(composite);
	if (item != 0)
	{
		delete item;
		selection_changed_ = true;
		updateContents();
	}

	return true;
}

bool Control::updateComposite
  (Composite* composite)
{
	if (composite == 0)
	{
		return false;
	}

	// update ListViewItem and insert it into the ListView
	updateListViewItem_(0, composite);

	// update the view
  selection_changed_ = true;
	updateContents();

	return true;
}

void Control::ContextMenu
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

/*	
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
	*/
}

QListViewItem* Control::getRoot_(QListViewItem* item)
{
	QListViewItem* parent = item;

	while (parent->parent() != 0)
	{
		parent = parent->parent();
	}

	return parent;
}

QString Control::getName_(QListViewItem* item)
{
	if (item == 0)
	{
		return QString("unknown");
	}

	return item->text(COLUMN_ID__NAME);
}

QString Control::getRootName_(QListViewItem* item)
{
	return getName_(getRoot_(item));
}

QString Control::getTypeName_(QListViewItem* item)
{
	if (item == 0)
	{
		return QString("unknown");
	}

	return item->text(COLUMN_ID__TYPE);
}

QString Control::getRootTypeName_(QListViewItem* item)
{
	return getTypeName_(getRoot_(item));
}

Composite* Control::getCompositeAddress_(QListViewItem* item)
{
	if (item == 0)
	{
		return 0;
	}

	// get address from the attached composite
	QString address = item->text(COLUMN_ID__ADDRESS);

	// convert address string to the real composite address
	Composite* composite = 0;
	if (address != 0)
	{
		composite = (Composite*)(atoi(address.ascii()));
	}

	return composite;
}

void Control::genListViewItem_
  (QListViewItem* item, Composite* composite, QString* default_name)
{
	if (composite == 0)
	{
		return;
	}

	// if getName returns "<xxxx>"  and name contains a valid string
	// use it instead of the default name
	// get information about the composite
	MolecularInformation molecular_information_;

	composite->host(molecular_information_);

	QString name = molecular_information_.getName().c_str();

	if (name[0] == '<')
	{
		if (default_name == 0)
		{
			name = "<";
			name += molecular_information_.getTypeName().c_str();
			name += ">";
		} else {
			name = *default_name;
		}
	}


	// create an entry for this composite
	QString type = molecular_information_.getTypeName().c_str();
	QString address_string;
	address_string.sprintf("%ld", (((unsigned long)((void*)composite))));

	// create a new list item
	QListViewItem* new_item = 0;

	// is this the first item?
	if (item == 0)
	{
		// yes, insert into the root node 
		new_item = new QListViewItem
							(this, 
							 name, type, QString::null, QString::null,
							 QString::null, QString::null, address_string);
	} else {
		// no, insert into the current item
		new_item = new QListViewItem
							(item, name, type, QString::null, QString::null,
							 QString::null, QString::null, address_string);
	}
	CHECK_PTR(new_item);

	// if the composite is anything but an atom,
	// we iterate over all children and recurse
	if (!RTTI::isKindOf<Atom>(*composite))
	{
		Composite::ChildCompositeReverseIterator it = composite->rbeginChildComposite();
		for (; it != composite->rendChildComposite(); ++it)
		{
			genListViewItem_(new_item, &*it);
		}
	}
}

void Control::updateListViewItem_
  (QListViewItem* item, Composite* composite, QString* default_name)
{
	if (composite == 0)
	{
		return;
	}

	// find the ListViewItem belonging to the composite
	// if it is already inserted
	QListViewItem* attached_item = findListViewItem_(composite);

	// an item does not exist => create a new listviewitem 
	if (attached_item == 0)
	{
		genListViewItem_(item, composite);

		attached_item = item;
	}

	// if the composite is anything but an atom,
	// we iterate over all children and recurse
	if (!RTTI::isKindOf<Atom>(*composite))
	{
		Composite::ChildCompositeReverseIterator it = composite->rbeginChildComposite();
		for (; it != composite->rendChildComposite(); ++it)
		{
			updateListViewItem_(attached_item, &*it);
		}
	}
}

QListViewItem* Control::findListViewItem_
  (Composite* composite)
{
	QString address;
	address.sprintf("%ld", (((unsigned long)((void *)composite))));

	QListViewItemIterator it(this);

	// iterate through all items
	for (; it.current(); ++it)
	{
		// find the address of the composite
		if (it.current()->text(6) == address)
		{
			return it.current();
		}
	}

	return 0;
}

const List<Composite*>& Control::getSelection()
{
	if (selection_changed_)
	{
		selected_.clear();

		QListViewItemIterator it(this);
		for (; it.current(); ++it)
		{
			if (it.current()->isSelected())
			{
				Composite* composite = getCompositeAddress_(it.current());
				if (composite != 0)
				{
					selected_.push_back(composite);
				}
			}
		}
		
		selection_changed_ = false;
	}
			
	return selected_;
}

void Control::onNotify(Message *message)
{
	if (RTTI::isKindOf<NewMolecularMessage>(*message))
	{
		NewMolecularMessage *composite_message 
			= RTTI::castTo<NewMolecularMessage>(*message);

		addComposite((Composite *)composite_message->getComposite());
	}
	else if (RTTI::isKindOf<RemovedCompositeMessage>(*message))
  {
    RemovedCompositeMessage *composite_message
      = RTTI::castTo<RemovedCompositeMessage>(*message);

    removeComposite((Composite *)composite_message->getComposite());
	}   
	else if  (RTTI::isKindOf<ChangedCompositeMessage>(*message))
  {
    ChangedCompositeMessage *composite_message
      = RTTI::castTo<ChangedCompositeMessage>(*message);

		updateComposite(&(composite_message->getComposite()->getRoot()));
	}

  // sent new selection through tree
	selection_changed_ = true;

  SelectionMessage* sel_message = new SelectionMessage;
  sel_message->setSelection(getSelection());
	sel_message->setDeletable(true);
 
	notify_(sel_message);
}

void Control::newSelection()
{
	selection_changed_ = true;

	// sent new selection through tree
	SelectionMessage message;
	message.setSelection(getSelection());

	notify_(message);
}

void Control::cut()
{
	const List<Composite*>& selection = getSelection();
	if (selection.size() == 0)
	{
		return;
	}

	QString message;

	// notify the main window
	WindowMessage window_message;
	message.sprintf("cutting %d systems...", selection.size());
	window_message.setStatusBar(message.ascii());
	notify_(window_message);

	// delete old composites
	if (copy_list_.size() > 0)
	{
		List<Composite*>::ConstIterator list_it = copy_list_.begin();	
		for (; list_it != copy_list_.end(); ++list_it)
		{
			delete *list_it;
		}

		copy_list_.clear();
	}

	// remove all system composites from the tree and from the scene
	// but do not delete them from memory
	List<Composite*>::ConstIterator list_it = selection.begin();	
	for (; list_it != selection.end(); ++list_it)
	{
		// only if selected composite equals root => remove composite from tree and mainControl
		if (*list_it == &((*list_it)->getRoot()))
		{
			// insert deep clone of the composite into the cut list
			copy_list_.push_back((Composite*)(*list_it)->create());
			
			// remove composite representation from tree
			removeComposite(*list_it);
			
			// remove Composite from from mainControl
			RemovedCompositeMessage remove_message;
			remove_message.setComposite(*list_it);
			notify_(remove_message);
		}
	}

	// update scene
	SceneMessage scene_message;
	scene_message.updateOnly();
	notify_(scene_message);

	// notify the main window
	window_message.setStatusBar("");
	notify_(window_message);
}

void Control::copy()
{
	const List<Composite*>& selection = getSelection();
	if (selection.size() == 0)
	{
		return;
	}

	QString message;

	// notify the main window
	WindowMessage window_message;
	message.sprintf("copying %d systems...", selection.size());
	window_message.setStatusBar(message.ascii());
	notify_(window_message);

	// delete old cutted composites
	if (copy_list_.size() > 0)
	{
		List<Composite*>::ConstIterator list_it = copy_list_.begin();	
		for (; list_it != copy_list_.end(); ++list_it)
		{
			delete *list_it;
		}

		copy_list_.clear();
	}

	// copy the selected composites into the copy_list_
	List<Composite*>::ConstIterator list_it = selection.begin();	
	for (; list_it != selection.end(); ++list_it)
	{
		// insert deep clone of the composite into the cut list
		copy_list_.push_back((Composite*)(*list_it)->create());
	}

	// notify the main window
	window_message.setStatusBar("");
	notify_(window_message);
}

void Control::paste()
{
	if (copy_list_.size() == 0)
	{
		return;
	}

	QString message;

	// notify the main window
	WindowMessage window_message;
	message.sprintf("pasting %d systems...", copy_list_.size());
	window_message.setStatusBar(message.ascii());
	notify_(window_message);

	// copying composites
	List<Composite*>::ConstIterator list_it = copy_list_.begin();	
	for (; list_it != copy_list_.end(); ++list_it)
	{
		addComposite(*list_it, 0);
	}

	// notify the main window
	window_message.setStatusBar("");
	notify_(window_message);
}

void Control::clearClipboard()
{
	// notify the main window
	WindowMessage window_message;
	window_message.setStatusBar("clearing clipboard...");
	notify_(window_message);

	// delete old composites
	if (copy_list_.size() > 0)
	{
		List<Composite*>::ConstIterator list_it = copy_list_.begin();	
		for (; list_it != copy_list_.end(); ++list_it)
		{
			delete *list_it;
		}

		copy_list_.clear();
	}

	// notify the main window
	window_message.setStatusBar("");
	notify_(window_message);
}

void Control::invalidateSelection()
{
	QListViewItemIterator it(this);
	for (; it.current(); ++it)
	{
		it.current()->setSelected(FALSE);
	}
}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/GUI/WIDGETS/control.iC>
#		endif

