// $Id: control.C,v 1.1 2000/09/23 13:28:38 hekl Exp $

#include <BALL/VIEW/GUI/WIDGETS/control.h>

using namespace std;

Control::Control
  (QWidget* parent, const char* name)
		:	QListView(parent, name),
			ModularWidget(name),
			selected_(),
			selection_changed_(true),
			information_(),
			geometric_filter_()
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
					SLOT(updateSelection()));

	// register ModularWidget
	registerWidget(this);
}

Control::~Control()
{
}

void Control::initializeWidget(MainControl& /* main_control */)
{
}

void Control::finalizeWidget(MainControl& /* main_control */)
{
}

void Control::checkMenu(MainControl& /* main_control */)
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
	composite->host(getInformationVisitor_());

	// if the own name is empty use name as name
	QString name = getInformationVisitor_().getName().c_str();

	if ((name[0] == '<') && (own_name != 0))
	{
		name = *own_name;
	}

	// generate ListViewItem and insert it into the ListView
	generateListViewItem_(0, composite, &name);

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
	selection_changed_ = updateListViewItem_(0, composite);

	// update the view
	updateContents();

	return selection_changed_;
}

void Control::onNotify(Message *message)
{
	// react accordingly to the given message
	if (reactToMessages_(message))
	{
		// if message makes an update of the control necessary
		// sent new selection through tree
		sentSelection();
	}
}

void Control::invalidateSelection()
{
	QListViewItemIterator it(this);
	for (; it.current(); ++it)
	{
		it.current()->setSelected(FALSE);
	}

	selection_changed_ = true;
	updateSelection();
}

void Control::updateSelection()
{
	filterSelection_(geometric_filter_, true);

	sentSelection();
}

void Control::sentSelection()
{
	// sent new selection through tree
	GeometricObjectSelectionMessage* message = new GeometricObjectSelectionMessage;
	message->setSelection(getSelection());
	message->setDeletable(true);

	notify_(message);
}

Information& Control::getInformationVisitor_()
{
	return information_;
}

void Control::recurseGeneration_(QListViewItem* item, Composite* composite)
{
	// we iterate over all children and recurse
	Composite::ChildCompositeIterator it = composite->beginChildComposite();
	for (; it != composite->endChildComposite(); ++it)
	{
		generateListViewItem_(item, &*it);
	}
}

bool Control::recurseUpdate_(QListViewItem* item, Composite* composite)
{
	bool tree_updated = false;

	// we iterate over all children and recurse
	Composite::ChildCompositeIterator it = composite->beginChildComposite();
	for (; it != composite->endChildComposite(); ++it)
	{
		tree_updated = (tree_updated || updateListViewItem_(item, &*it));
	}

	return tree_updated;
}

void Control::filterSelection_(Filter& filter, bool refill_list)
{
	if (refill_list)
	{
		selection_changed_ = true;

		fillSelectionList_();
	}

	List<Composite*>::Iterator it = selected_.begin();
	List<Composite*>::Iterator remove_it = selected_.end();
	
	for (; it != selected_.end(); ++it)
	{	
		// remove iterator from list
		if (remove_it != selected_.end())
		{
			selected_.erase(remove_it);
		}

		// apply filter criteria
		(*it)->host(filter);

		// if filter does not apply => remove composite from list
		if (!filter)
		{
			remove_it = it;
		}
	}

	// remove iterator from list (the last)
	if (remove_it != selected_.end())
	{
		selected_.erase(remove_it);
	}
}

bool Control::reactToMessages_(Message* message)
{
	bool update = false;

	if (RTTI::isKindOf<NewCompositeMessage>(*message))
	{
		NewCompositeMessage *composite_message 
			= RTTI::castTo<NewCompositeMessage>(*message);

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

void Control::generateListViewItem_
  (QListViewItem* item, Composite* composite, QString* default_name)
{
	if (composite == 0)
	{
		return;
	}

	// if getName returns "<xxxx>"  and name contains a valid string
	// use it instead of the default name
	// get information about the composite
	composite->host(getInformationVisitor_());

	QString name = getInformationVisitor_().getName().c_str();

	if (name[0] == '<')
	{
		if (default_name == 0)
		{
			name = "<";
			name += getInformationVisitor_().getTypeName().c_str();
			name += ">";
		}
		else 
		{
			name = *default_name;
		}
	}

	// create an entry for this composite
	QString type = getInformationVisitor_().getTypeName().c_str();
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
	} 
	else 
	{
		// no, insert into the current item
		new_item = new QListViewItem
							(item, name, type, QString::null, QString::null,
							 QString::null, QString::null, address_string);
	}
	CHECK_PTR(new_item);

	recurseGeneration_(new_item, composite);
}

bool Control::updateListViewItem_
  (QListViewItem* item, Composite* composite, QString* /* default_name */)
{
	// was the tree updated ?
	bool tree_updated = false;

	if (composite == 0)
	{
		return tree_updated;
	}

	// find the ListViewItem belonging to the composite
	// if it is already inserted
	QListViewItem* attached_item = findListViewItem_(composite);

	// an item does not exist => create a new listviewitem 
	if (attached_item == 0)
	{
		generateListViewItem_(item, composite);

		attached_item = item;

		tree_updated = true;
	}

	return (bool)(recurseUpdate_(attached_item, composite) || tree_updated);
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

void Control::fillSelectionList_() 
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
}


/*
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
*/


#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/WIDGETS/control.iC>
#		endif

