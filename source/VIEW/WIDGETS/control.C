// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: control.C,v 1.1 2003/08/26 12:38:46 oliver Exp $
//

#include <BALL/VIEW/GUI/WIDGETS/control.h>
#include <BALL/VIEW/GUI/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/COMMON/global.h>
#include <BALL/KERNEL/system.h>
#include <BALL/VIEW/GUI/DIALOGS/transformationDialog.h>
#include <qpopupmenu.h>
#include <qmenubar.h>

using std::endl;

namespace BALL
{
	namespace VIEW
	{

Control::SelectableListViewItem::SelectableListViewItem(QListViewItem* parent, const QString& text, const QString& type, Composite* composite, VIEW::Control& control)
	throw()
	: QCheckListItem(parent, text, QCheckListItem::CheckBox),
		composite_(composite),
		control_reference_(control)
{
	setText(1, type);
}


Control::SelectableListViewItem::SelectableListViewItem(QListView* parent, const QString& text, const QString& type, Composite* composite, VIEW::Control& control)
	throw()
	: QCheckListItem(parent, text, QCheckListItem::CheckBox),
		composite_(composite),
		control_reference_(control)
{
	setText(1, type);
}


void Control::SelectableListViewItem::stateChange(bool)
	throw()
{
	control_reference_.selectedComposite_(composite_, isOn());
}



Control::Control(QWidget* parent, const char* name)
	throw()
		:	QListView(parent, name),
			ModularWidget(name),
			cut_id_(-1),
			copy_id_(-1),
			paste_id_(-1),
			clipboard_id_(-1),
			selected_(),
			information_(),
			context_menu_(),
			context_composite_(0),
			context_item_(0),
			transformation_dialog_(0)
{
	// appearance
	setRootIsDecorated(TRUE);
	setSorting(-1);
	setSelectionMode(QListView::Extended);
	addColumn("[selected] Name");
	addColumn("Type");
	setColumnWidth(0, 120);
	setColumnWidth(1, 60);

	// if the selection of any item changed,
	// mark the complete selection as invalid
	// it is then re-determined by getSelection()
	connect(this, SIGNAL(selectionChanged()), this, SLOT(updateSelection()));

	connect(this, SIGNAL(rightButtonPressed(QListViewItem*, const QPoint&, int)), this,
					SLOT(onContextMenu_(QListViewItem*, const QPoint&, int)));

	// register ModularWidget
	registerWidget(this);
}


Control::~Control()
	throw()
{
  #ifdef BALL_VIEW_DEBUG
	  Log.error() << "Destructing object " << (void *)this << " of class " << RTTI::getName<Control>() << endl;
  #endif 

	if (transformation_dialog_) delete transformation_dialog_;
}


void Control::clear()
	throw()
{
}


void Control::initializeWidget(MainControl& main_control)
	throw()
{
	cut_id_ = main_control.insertMenuEntry(MainControl::EDIT, "Cut", this, SLOT(cut()), CTRL+Key_X);
	copy_id_ = main_control.insertMenuEntry(MainControl::EDIT, "&Copy", this, SLOT(copy()), CTRL+Key_C);
	paste_id_ = main_control.insertMenuEntry(MainControl::EDIT, "Paste", this, SLOT(paste()), CTRL+Key_V);
	clipboard_id_ = main_control.insertMenuEntry(MainControl::EDIT, "Clear Clipboard", this, 
																							 SLOT(clearClipboard()));
	window_menu_entry_id_ = 
		main_control.insertMenuEntry(MainControl::WINDOWS, "Control", this, SLOT(switchShowWidget()));
	getMainControl()->menuBar()->setItemChecked(window_menu_entry_id_, true);
}


void Control::finalizeWidget(MainControl& main_control)
	throw()
{
	main_control.removeMenuEntry(MainControl::EDIT, "Cut", this, SLOT(cut()), CTRL+Key_X);
	main_control.removeMenuEntry(MainControl::EDIT, "&Copy", this, SLOT(copy()), CTRL+Key_C);
	main_control.removeMenuEntry(MainControl::EDIT, "Paste", this, SLOT(paste()), CTRL+Key_V);
	main_control.removeMenuEntry(MainControl::EDIT, "Clear Clipboard", this, SLOT(clearClipboard()));
	main_control.removeMenuEntry(MainControl::WINDOWS, "Control", this, SLOT(switchShowWidget()));
}


void Control::checkMenu(MainControl& main_control)
	throw()
{
	bool copy_list_filled = (getCopyList_().size() > 0);
	// prevent changes to composites while simulations are running
	copy_list_filled = copy_list_filled && main_control.compositesAreMuteable();

	// check for paste-slot: enable only if copy_list_ not empty
	(main_control.menuBar())->setItemEnabled(paste_id_, copy_list_filled);	

	// check for clearClipboard-slot: enable only if copy_list_ not empty
	(main_control.menuBar())->setItemEnabled(clipboard_id_, copy_list_filled);

	// check for cut-slot 
	bool list_filled = (selected_.size() != 0);
	list_filled = list_filled && main_control.compositesAreMuteable();
	(main_control.menuBar())->setItemEnabled(cut_id_, list_filled);
}


void Control::addComposite(Composite& composite, QString* own_name)
	throw()
{
	// get information about the composite
	composite.host(getInformationVisitor_());

	// if the own name is empty use name as name
	QString name = getInformationVisitor_().getName().c_str();

	if ((name[0] == '<') && (own_name != 0))
	{
		name = *own_name;
	}

	// generate ListViewItem and insert it into the ListView
	generateListViewItem_(0, composite, &name);

	// update the view
	updateContents();
}


void Control::removeComposite(Composite& composite)
	throw()
{
	if (!composite_to_item_.has(&composite)) return;

	SelectableListViewItem* item = composite_to_item_[&composite];
	delete item;
	composite_to_item_.erase(&composite);		
	updateContents();
}


Size Control::removeRecursiveComposite(Composite& composite)
	throw()
{
	if (!composite_to_item_.has(&composite)) return 0;
	SelectableListViewItem* item = composite_to_item_[&composite];

	Size nr = 1;

	for (Size i = 0; i < composite.getDegree(); i++)
	{
		Composite& child = *composite.getChild(i);
		nr += removeRecursiveComposite(child);
	}

	delete item;
	composite_to_item_.erase(&composite);		

	return nr;
}


void Control::onNotify(Message *message)
	throw()
{
	// react accordingly to the given message
	if (reactToMessages_(message))
	{
		// if message makes an update of the control necessary
		// sends new selection through tree
		updateSelection();
	}
}


void Control::insertContextMenuEntry(const String& name, const QObject* receiver, const char* slot, int entry_ID, int accel)
	throw()
{
	context_menu_.insertItem(name.c_str(), receiver, slot, accel, entry_ID);
}


void Control::invalidateSelection()
{
	QListViewItemIterator it(this);
	for (; it.current(); ++it)
	{
		it.current()->setSelected(FALSE);
	}

	updateSelection();
}


// set the checkboxes according to the selection in the MainControl
void Control::setSelection_(bool open)
	throw()
{	
	const HashSet<Composite*>& selection = getMainControl()->getSelection();
	
	if (selection.size() == 0)
	{
		QListViewItemIterator it(this);
		for (; it.current(); ++it)
		{
			((QCheckListItem*) it.current())->setOn(false);
		}
		return;
	}

	if (selection.size() > 5) open = false;

	QListViewItemIterator it(this);
	for (; it.current(); ++it)
	{
		SelectableListViewItem* item = (SelectableListViewItem*) it.current();
		if (selection.has(item->getComposite()))
		{
			item->setOn(true);
			if (open)
			{
				item->setSelected(true);
				QListViewItem* parent = item->parent();
				while (parent != 0)
				{
					parent->setOpen(true);
					parent = (QListViewItem*)parent->parent();
				}
			}
		}
		else
		{
			item->setOn(false);
			if (open)
			{
				item->setSelected(false);
				item->setOpen(false);
			}
		}
	}
}


void Control::updateSelection()
{
	setStatusbarText("");
	selected_.clear();

	QListViewItemIterator it(this);
	for (; it.current(); ++it)
	{
		SelectableListViewItem* item = (SelectableListViewItem*) it.current();
		if (item->isSelected())
		{
			selected_.push_back(item->getComposite());
		}
	}

	sendSelection();
}


void Control::sendSelection()
{
	// sent new selection through tree
	ControlSelectionMessage* message = new ControlSelectionMessage;
	message->setSelection(selected_);
	message->setDeletable(true);

	if (transformation_dialog_ && selected_.size()>0)
	{
		transformation_dialog_->setComposite(*selected_.begin());
	}

	notify_(message);
}

MolecularInformation& Control::getInformationVisitor_()
	throw()
{
	return information_;
}


bool Control::reactToMessages_(Message* message)
	throw()
{
	if (RTTI::isKindOf<RemovedCompositeMessage>(*message))
  {
    RemovedCompositeMessage *composite_message = RTTI::castTo<RemovedCompositeMessage>(*message);
    removeComposite(*(Composite *)composite_message->getComposite());
	}   
	else if (RTTI::isKindOf<ChangedCompositeMessage>(*message))
  {
		#ifdef BALL_VIEW_DEBUG
			Log.error() << "updating composite in Control" << std::endl;
		#endif 
    ChangedCompositeMessage *composite_message = RTTI::castTo<ChangedCompositeMessage>(*message);
		recurseUpdate_(0, *composite_message->getComposite());
		#ifdef BALL_VIEW_DEBUG
			Log.error() << "updating composite in Control finished" << std::endl;
		#endif 
		return (composite_message->isUpdateControl());
	}
	else if (RTTI::isKindOf<CompositeSelectedMessage>(*message))
  {
    CompositeSelectedMessage* composite_message = RTTI::castTo<CompositeSelectedMessage>(*message);
		updateListViewItem_(0, *composite_message->getComposite());
		return true;
	}
	else if (RTTI::isKindOf<NewSelectionMessage> (*message))
	{
		setSelection_(true);
	}
	else if (RTTI::isKindOf<NewCompositeMessage>(*message))
	{
		NewCompositeMessage *composite_message = RTTI::castTo<NewCompositeMessage>(*message);
		addComposite(*(Composite *)composite_message->getComposite());
	}

	return false;
}


Control::SelectableListViewItem* Control::generateListViewItem_(SelectableListViewItem* parent, 
																													Composite& composite, QString* default_name)
	throw()
{
	// if getName returns "<xxxx>"  and name contains a valid string
	// use it instead of the default name
	// get information about the composite
	composite.host(getInformationVisitor_());

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

	// create a new list item
	SelectableListViewItem* new_item = 0;

	// is this a root item?
	if (parent == 0)
	{
		new_item = new SelectableListViewItem(this, name, type, &composite, *this);
	} 
	else 
	{
		// no, insert into the current item
		new_item = new SelectableListViewItem(parent, name, type, &composite, *this);
	}
	CHECK_PTR(new_item);

	composite_to_item_[&composite] = new_item;

	recurseGeneration_(new_item, composite);

	return new_item;
}

void Control::updateListViewItem_(SelectableListViewItem* parent, Composite& composite)
	throw()
{
	// an item does not exist => create a new SelectableListViewItem 
	if (!composite_to_item_.has(&composite))
	{
		generateListViewItem_(parent, composite);
		return;
	}

	// find the ListViewItem belonging to the composite
	recurseUpdate_(composite_to_item_[&composite], composite);
}


void Control::cut()
{
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

	// remove the selected composites from the tree and from the scene
	// selected systems are copied to the copy_list_
	// for all selected items, which are not Systems, the representations of their root are rebuild
	HashSet<Composite*> roots;
	HashSet<Composite*> to_delete;
	List<Composite*>::Iterator it = selected_.begin();	
	for (; it != selected_.end(); it++)
	{
		getMainControl()->deselectCompositeRecursive(*it, false);

		if (RTTI::isKindOf<System>(**it)) 
		{
			// insert deep clone of the composite into the cut list
			copy_list_.push_back((System*)(*it)->create());
			to_delete.insert(*it);
		}
		else
		{
			roots.insert(&(*it)->getRoot());
			to_delete.insert(*it);
		}
	}

	List<Composite*> childs;
	HashSet<Composite*>::Iterator delete_it = to_delete.begin();
	for (; delete_it != to_delete.end(); delete_it++)
	{
		HashSet<Composite*>::Iterator delete_it2 = to_delete.begin();
		for (; delete_it2 != to_delete.end(); delete_it2++)
		{
			if ((*delete_it)->isAncestorOf(**delete_it2))
			{
				childs.push_back(*delete_it2);
			}
		}
	}

	List<Composite*>::Iterator child_it = childs.begin();
	for (; child_it != childs.end(); child_it++)
	{
		to_delete.erase(*child_it);
	}
	
	RemovedCompositeMessage* remove_message = new RemovedCompositeMessage;
	remove_message->setDeletable(false);
	Size nr_of_items = 0;

	delete_it = to_delete.begin();
	for (; delete_it!= to_delete.end(); delete_it++)
	{
		// remove composite representation from tree
		nr_of_items += removeRecursiveComposite(**delete_it);
		remove_message->setComposite(*delete_it);
		notify_(remove_message);
	}

	setStatusbarText("Deleted " + String(nr_of_items) + " items.");

	selected_.clear();
	ControlSelectionMessage* message = new ControlSelectionMessage;
	message->setDeletable(true);
	notify_(message);
	*/
	selected_.clear();
	sentSelection();

	ChangedCompositeMessage* ccmessage = new ChangedCompositeMessage;
	ccmessage->setDeletable(false);
	ccmessage->setUpdateControl(false);
	HashSet<Composite*>::Iterator roots_it = roots.begin();
	for (roots_it = roots.begin(); roots_it != roots.end(); roots_it++)
	{
		ccmessage->setComposite(*roots_it);
		notify_(ccmessage);
	}

	delete ccmessage;
}


void Control::copy()
{
	const List<Composite*> selection = getSelection();
	if (selection.size() == 0) return;

	setStatusbarText("copying " + String(selection.size()) + " objects ...");

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
	List<Composite*>::Iterator it = selected_.begin();	
	for (; it != selected_.end(); it++)
	{
		if (RTTI::isKindOf<System>(**it)) 
		{
			copy_list_.push_back((Composite*)(*it)->create());
		}
	}
	
	setStatusbarText("");
}


void Control::paste()
{
	if (copy_list_.size() == 0) return;

	setStatusbarText("pasting " + String(copy_list_.size()) + " objects...");

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

	setStatusbarText("");
}


void Control::clearClipboard()
{
	setStatusbarText("clearing clipboard...");

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

	setStatusbarText("");
}


void Control::onContextMenu_(QListViewItem* item,  const QPoint& point, int /* column */)
{
	// clear the context menu
	context_menu_.clear();

	// get composite address
	if (item == 0) return;

	Composite* composite = ((SelectableListViewItem*)item)->getComposite();

	// create the context menu
	if (composite != 0)
	{
		context_composite_ = composite;
		context_item_ = (SelectableListViewItem*)item;

		buildContextMenu(*composite);
	}

	// show the context menu if it is not empty
	if (context_menu_.count())
	{
		context_menu_.exec(point);
	}
}


void Control::selectedComposite_(Composite* composite, bool state)
{
	const HashSet<Composite*>& selection = getMainControl()->getSelection();
	if (selection.has(composite) == state)
	{
		return;
	}

	if (state)
	{
		getMainControl()->selectCompositeRecursive(composite, true);
	}
	else
	{
		getMainControl()->deselectCompositeRecursive(composite, true);
	}
	
	SelectableListViewItem* item = composite_to_item_[composite];
	item->setOn(state);
  item->setSelected(true);
	if (selection.size() < 10)
	{
		QListViewItem* parent = item->parent();
		while (parent != 0)
		{
			parent->setOpen(true);
			parent = parent->parent();
		}
  }

	CompositeSelectedMessage* message = new CompositeSelectedMessage(composite, state);
	message->setDeletable(false);
	message->setComposite(composite);
	notify_(message);
	
	setSelection_(false);
}


void Control::recurseGeneration_(SelectableListViewItem* item, Composite& composite)
	throw()
{
	Composite::ChildCompositeReverseIterator it = composite.rbeginChildComposite();
	for (; it != composite.rendChildComposite(); ++it)
	{
		generateListViewItem_(item, *it);
	}
}


void Control::recurseUpdate_(SelectableListViewItem* item, Composite& composite)
	throw()
{
	Composite::ChildCompositeReverseIterator it = composite.rbeginChildComposite();
	for (; it != composite.rendChildComposite(); ++it)
	{
		updateListViewItem_(item, *it);
	}
}


void Control::buildContextMenu(Composite& composite)
	throw()
{
	insertContextMenuEntry("Cut", this, SLOT(cut()), OBJECT__CUT);
	insertContextMenuEntry("Copy", this, SLOT(copy()), OBJECT__COPY);
	insertContextMenuEntry("Paste", this, SLOT(paste()), OBJECT__PASTE);

	insertContextMenuEntry("Move", this, SLOT(move()), OBJECT__MOVE);

	bool composites_muteable = getMainControl()->compositesAreMuteable();
	context_menu_.setItemEnabled(OBJECT__CUT, composites_muteable);
	context_menu_.setItemEnabled(OBJECT__PASTE, getCopyList_().size() > 0 && composites_muteable);
	context_menu_.setItemEnabled(OBJECT__MOVE, composites_muteable);
	context_menu_.insertSeparator();

	insertContextMenuEntry("Select", this, SLOT(select()), SELECT);
	insertContextMenuEntry("Deselect", this, SLOT(deselect()), DESELECT);
	context_menu_.setItemEnabled(SELECT,   !composite.isSelected() && composites_muteable);
	context_menu_.setItemEnabled(DESELECT,  composite.isSelected() && composites_muteable);
}


void Control::select()
{
	// copy list, because selection could change
	List<Composite*> selection = selected_;

	List<Composite*>::Iterator it = selection.begin();
	for(; it != selection.end(); it++)
	{
		selectedComposite_(*it, true);
	}
}

void Control::deselect()
{
	// copy list, because selection could change
	List<Composite*> selection = selected_;

	List<Composite*>::Iterator it = selection.begin();
	for(; it != selection.end(); it++)
	{
		selectedComposite_(*it, false);
	}	
}

void Control::move()
{
	if (selected_.size() == 0) return;

	if (transformation_dialog_) 
	{
		transformation_dialog_->hide();
		delete transformation_dialog_;
	}
	
	transformation_dialog_ = new TransformationDialog(this);
	transformation_dialog_->show();
	transformation_dialog_->setComposite(*selected_.begin());
}

void Control::switchShowWidget()
	throw()
{
	QMenuBar* menu = getMainControl()->menuBar();
	if (menu->isItemChecked(window_menu_entry_id_))
	{
		hide();
		menu->setItemChecked(window_menu_entry_id_, false);
	}
	else
	{
		show();
		menu->setItemChecked(window_menu_entry_id_, true);
	}
}

void Control::writePreferences(INIFile& inifile)
	throw()
{
	inifile.insertValue("WINDOWS", "Control::on", 
		String(getMainControl()->menuBar()->isItemChecked(window_menu_entry_id_)));
}

void Control::fetchPreferences(INIFile & inifile)
	throw()
{
	if (!inifile.hasEntry("WINDOWS", "Control::on")) return;
	if (inifile.getValue("WINDOWS", "Control::on").toUnsignedInt() == 0) 
	{
		switchShowWidget();
	}
}


#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/VIEW/GUI/WIDGETS/control.iC>
#	endif

	} // namespace VIEW
} // namespace BALL
