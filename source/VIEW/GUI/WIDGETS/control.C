// $Id: control.C,v 1.2 2000/10/22 15:17:52 hekl Exp $

#include <BALL/VIEW/GUI/WIDGETS/control.h>
#include <qpopupmenu.h>
#include <qmenubar.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

Control::Control
  (QWidget* parent, const char* name)
		:	QListView(parent, name),
			ModularWidget(name),
			cut_id_(-1),
			copy_id_(-1),
			paste_id_(-1),
			clipboard_id_(-1),
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

void Control::initializeWidget(MainControl& main_control)
{
	cut_id_ 
		= main_control.insertMenuEntry
		   (MainControl::EDIT, "&Cut", this, SLOT(cut()), CTRL+Key_C);

	copy_id_ 
		= main_control.insertMenuEntry
		   (MainControl::EDIT, "C&opy", this, SLOT(copy()), CTRL+Key_O);

	paste_id_ 
		= main_control.insertMenuEntry
		   (MainControl::EDIT, "&Paste", this, SLOT(paste()), CTRL+Key_P);

	clipboard_id_ 
		= main_control.insertMenuEntry
		   (MainControl::EDIT, "Cl&ear Clipboard", this, SLOT(clearClipboard()), CTRL+Key_E);
}

void Control::finalizeWidget(MainControl& main_control)
{
	main_control.removeMenuEntry(MainControl::EDIT, "&Cut", this, SLOT(cut()), CTRL+Key_C);
	main_control.removeMenuEntry(MainControl::EDIT, "C&opy", this, SLOT(copy()), CTRL+Key_O);
	main_control.removeMenuEntry(MainControl::EDIT, "&Paste", this, SLOT(paste()), CTRL+Key_P);
	main_control.removeMenuEntry(MainControl::EDIT, "Cl&ear Clipboard", this, SLOT(clearClipboard()), CTRL+Key_E);
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

void Control::cut()
{
	const List<Composite*> *selection = &getSelection();
	if (selection->size() == 0)
	{
		return;
	}

	QString message;

	// notify the main window
	WindowMessage *window_message = new WindowMessage;
	message.sprintf("cutting %d objects ...", selection->size());
	window_message->setStatusBar(message.ascii());
	window_message->setDeletable(true);
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
	List<Composite*>::ConstIterator list_it = selection->begin();	
	for (; list_it != selection->end(); ++list_it)
	{
		// only if selected composite equals root => remove composite from tree and mainControl
		if (*list_it == &((*list_it)->getRoot()))
		{
			// insert deep clone of the composite into the cut list
			copy_list_.push_back((Composite*)(*list_it)->create());
			
			// remove composite representation from tree
			removeComposite(*list_it);
			
			// remove Composite from from mainControl
			RemovedCompositeMessage *remove_message = new RemovedCompositeMessage;
			remove_message->setComposite(*list_it);
			remove_message->setDeletable(true);
			notify_(remove_message);
		}
	}

	updateSelection();	

	// update scene
	SceneMessage *scene_message = new SceneMessage;
	scene_message->updateOnly();
	scene_message->setDeletable(true);
	notify_(scene_message);

	// notify the main window
	WindowMessage *window_message_2 = new WindowMessage;
	window_message_2->setStatusBar("");
	window_message_2->setDeletable(true);
	notify_(window_message_2);
}

void Control::copy()
{
	const List<Composite*> *selection = &getSelection();
	if (selection->size() == 0)
	{
		return;
	}

	QString message;

	// notify the main window
	WindowMessage *window_message = new WindowMessage;
	message.sprintf("copying %d objects ...", selection->size());
	window_message->setStatusBar(message.ascii());
	window_message->setDeletable(true);
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
	List<Composite*>::ConstIterator list_it = selection->begin();	
	for (; list_it != selection->end(); ++list_it)
	{
		// insert deep clone of the composite into the cut list
		copy_list_.push_back((Composite*)(*list_it)->create());
	}

	// notify the main window
	WindowMessage *window_message_2 = new WindowMessage;
	window_message_2->setStatusBar("");
	window_message_2->setDeletable(true);
	notify_(window_message_2);
}

void Control::paste()
{
	if (copy_list_.size() == 0)
	{
		return;
	}

	QString message;

	// notify the main window
	WindowMessage *window_message = new WindowMessage;
	message.sprintf("pasting %d objects ...", copy_list_.size());
	window_message->setStatusBar(message.ascii());
	window_message->setDeletable(true);
	notify_(window_message);

	// copying composites
	List<Composite*>::ConstIterator list_it = copy_list_.begin();	
	for (; list_it != copy_list_.end(); ++list_it)
	{
		// create a new copy of the composite
		Composite *new_composite = (Composite*)(*list_it)->create();

		// insert Composite in mainControl
		NewCompositeMessage *new_message = new NewCompositeMessage;
		new_message->setDeletable(true);
		new_message->setComposite(new_composite);
		
		new_composite->host(getInformationVisitor_());
		
		new_message->setCompositeName(getInformationVisitor_().getName());
		notify_(new_message);
	}

	// notify the main window
	WindowMessage *window_message_2 = new WindowMessage;
	window_message_2->setDeletable(true);
	window_message_2->setStatusBar("");
	notify_(window_message_2);
}

void Control::clearClipboard()
{
	// notify the main window
	WindowMessage *window_message = new WindowMessage;
	window_message->setStatusBar("clearing clipboard...");
	window_message->setDeletable(true);
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
	WindowMessage *window_message_2 = new WindowMessage;
	window_message_2->setStatusBar("");
	window_message_2->setDeletable(true);
	notify_(window_message_2);
}


#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/WIDGETS/control.iC>
#		endif

	} // namespace VIEW

} // namespace BALL
