// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: molecularControl.C,v 1.91.2.11 2005/02/02 15:23:15 amoll Exp $

#include <BALL/VIEW/WIDGETS/molecularControl.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/DIALOGS/compositeProperties.h>
#include <BALL/VIEW/DIALOGS/bondProperties.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/selector.h>
#include <qmenubar.h>
#include <qlineedit.h> 
#include <qpushbutton.h> 
#include <qmessagebox.h> 
#include <qtooltip.h> 

using std::endl;

namespace BALL
{
	namespace VIEW
	{

MolecularControl::SelectableListViewItem::SelectableListViewItem(QListViewItem* parent, const QString& text, const QString& type, Composite* composite, VIEW::MolecularControl& control)
	throw()
	: QCheckListItem(parent, text, QCheckListItem::CheckBox),
		composite_(composite),
		control_reference_(control),
		ignore_change_(true)
{
	setText(1, type);
	setOn(composite->isSelected());
	ignore_change_ = false;
}


MolecularControl::SelectableListViewItem::SelectableListViewItem(QListView* parent, const QString& text, const QString& type, Composite* composite, VIEW::MolecularControl& control)
	throw()
	: QCheckListItem(parent, text, QCheckListItem::CheckBox),
		composite_(composite),
		control_reference_(control),
		ignore_change_(true)
{
	setText(1, type);
	setOn(composite->isSelected());
	ignore_change_ = false;
}

void MolecularControl::SelectableListViewItem::setSelected(bool state)
{
	QCheckListItem::setSelected(state);
	
	if (!state)
	{
		QListViewItem* child = firstChild();
		while (child != 0)
		{
			child->setSelected(false);
			child = child->nextSibling();
		}
	}
}

void MolecularControl::SelectableListViewItem::stateChange(bool state)
	throw()
{
	if (ignore_change_)
	{
		ignore_change_ = false;
		return;
	}

	if (control_reference_.getMainControl()->compositesAreLocked() ||
			control_reference_.getMainControl()->updateOfRepresentationRunning())
	{
		if (composite_->isSelected() == state) return;

		ignore_change_ = true;
		setOn(!state);
		return;
	}
	control_reference_.selectedComposite_(composite_, isOn());
}


MolecularControl::MolecularControl(const MolecularControl& mc)
	throw()
	: GenericControl(mc)
{
}

			

MolecularControl::MolecularControl(QWidget* parent, const char* name)
	throw()
		:	GenericControl(parent, name),
			cut_id_(-1),
			copy_id_(-1),
			paste_id_(-1),
			clipboard_id_(-1),
			selected_(),
			information_(),
			selector_edit_(new QLineEdit(this)),
			context_menu_(this),
			model_menu_(this),
			context_composite_(0),
			was_delete_(false),
			nr_items_removed_(0)
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "new MolecularControl " << this << std::endl;
#endif
	listview->addColumn("[selected] Name");
	listview->addColumn("Type");
	listview->setColumnWidth(0, 120);
	listview->setColumnWidth(1, 60);

	QHBoxLayout* layout2 = new QHBoxLayout();
	getLayout()->addLayout(layout2);

	selector_edit_->setPaletteBackgroundColor(QColor(255, 255, 0));
	selector_edit_->resize(90, 30);
	selector_edit_->setSizePolicy( QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed, 0, 0, false));
	layout2->addWidget(selector_edit_);

	QPushButton* clear_button = new QPushButton(this);
	clear_button->resize(30, 30);
	clear_button->setMaximumSize(30, 30);
	clear_button->setText("<");
	clear_button->setSizePolicy( QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed, 0, 0, false));
	connect(clear_button, SIGNAL(clicked()), this, SLOT(clearSelector()));
	QToolTip::add(clear_button, tr("Clear the selection."));
	layout2->addWidget(clear_button);

	QPushButton* help_button = new QPushButton(this);
	help_button->resize(30, 30);
	help_button->setMaximumSize(30, 30);
	help_button->setText("?");
	help_button->setSizePolicy( QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed, 0, 0, false));
	connect(help_button, SIGNAL(clicked()), this, SLOT(showSelectorHelp()));
	QToolTip::add(help_button, tr("Show a help dialog."));
	layout2->addWidget(help_button);

	connect(selector_edit_, SIGNAL(returnPressed()), this, SLOT(applySelector()));

	// if the selection of any item changed,
	// mark the complete selection as invalid
	// it is then re-determined by getSelection()
	connect(listview, SIGNAL(selectionChanged()), this, SLOT(updateSelection()));

	model_menu_.insertItem("Custom", this, SLOT(createRepresentation())); 

	Position p = 0;
	for (Position pos = MODEL_LINES; pos < MODEL_LABEL; pos++)
	{
		model_menu_.insertItem(getModelName((ModelType)pos).c_str(), &color_menu_[p], 1000+pos); 
		p++;
	}

	// create a new coloring popupmenu for every model entry
	// otherwise we get into trouble with qt
	for (Position i = 0; i< p; i++)
	{
		for (Position pos = COLORING_ELEMENT; pos < COLORING_UNKNOWN; pos++)
		{
			color_menu_[i].insertItem(getColoringName((ColoringMethod)pos).c_str(), this, 
																						 SLOT(createRepresentation_()), 0,2000 + pos);  
			connect(&color_menu_[i], SIGNAL(highlighted(int)), this, SLOT(activatedItem_(int)));
		}
	}

	connect(&model_menu_, SIGNAL(highlighted(int)), this, SLOT(activatedItem_(int)));
	registerWidget(this);
}

MolecularControl::~MolecularControl()
	throw()
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "Destroying MolecularControl " << this << std::endl;
#endif

	clearClipboard();
}

void MolecularControl::checkMenu(MainControl& main_control)
	throw()
{
	String hint;
	bool busy = main_control.compositesAreLocked() ||
							main_control.updateOfRepresentationRunning();

	// prevent changes to composites while simulations are running
	
	// check for paste-slot: enable only if one selected item &&
	// 																			copy_list_ not empty && 
	// 																			no simulation running &&
	// 																			it makes sense
	bool allow_paste = getSelection().size() <= 1 &&
										 copy_list_.size() && 
										 !busy;
	if (allow_paste)
	{
		hint = "Paste a copied or cuted object into current selected object.";
		List<Composite*>::Iterator it = copy_list_.begin();
		for (; it != copy_list_.end(); it++)
		{
			if (!pasteAllowedFor_(**it)) 
			{
				allow_paste = false;
				hint = "Invalid Combination, cant paste into this entity.";
				break;
			}
		}
	}
	else
	{
		if (getSelection().size() != 1)
			hint = "One item must be selected to paste into.";
		else if (copy_list_.size() == 0)
			hint = "No copied/cuted object.";
		else if (main_control.compositesAreLocked())
			hint = "Simulation running, cant paste meanwhile";
		else
			hint = "Update of Representation running, cant paste meanwhile";
	}
	menuBar()->setItemEnabled(paste_id_, allow_paste);	
 	getMainControl()->setMenuHint(paste_id_, hint);

	// ------------------------------------------------------------------
	// clearClipboard
	// enable only if copy_list_ not empty
	bool copy_list_filled = (copy_list_.size() > 0);
	menuBar()->setItemEnabled(clipboard_id_, copy_list_filled && 
																					 !main_control.compositesAreLocked());
	if (!menuBar()->isItemEnabled(clipboard_id_))
	{
 		hint = "No item copied/cuted or simulation running";
	} 
	else
	{
		hint = "Clear the items in the clipboard";
	}
 	getMainControl()->setMenuHint(clipboard_id_, hint);

	// ------------------------------------------------------------------
	// cut / delete  +  select / deselect
	bool list_filled = selected_.size() && !busy;
	
	if (list_filled) hint = "";
	else hint = "No item selected or simulation running";
	
	menuBar()->setItemEnabled(cut_id_, list_filled);
 	getMainControl()->setMenuHint(cut_id_, hint);
	menuBar()->setItemEnabled(copy_id_, list_filled);	
 	getMainControl()->setMenuHint(copy_id_, hint);

	menuBar()->setItemEnabled(select_id_, list_filled);
 	getMainControl()->setMenuHint(select_id_, hint);
	menuBar()->setItemEnabled(deselect_id_, list_filled);	
 	getMainControl()->setMenuHint(deselect_id_, hint);


	if (selected_.size() > 0)
	{
		// enable global delete entry for all GenericControls, if this Control has the selection
		getMainControl()->setDeleteEntryEnabled(!busy);
	}
}


bool MolecularControl::reactToMessages_(Message* message)
	throw()
{
	// react only to NewMolecularMessage, but not to NewCompositeMessage
	if (RTTI::isKindOf<CompositeMessage>(*message))
	{
		CompositeMessage* composite_message = RTTI::castTo<CompositeMessage>(*message);
		switch (composite_message->getType())
		{
			case CompositeMessage::NEW_MOLECULE:
				addComposite(*(Composite *)composite_message->getComposite());
				return false;
			
			case CompositeMessage::REMOVED_COMPOSITE:
				removeComposite(*(Composite *)composite_message->getComposite());
				return false;
			
			case CompositeMessage::CHANGED_COMPOSITE:
				return false;

			case CompositeMessage::CHANGED_COMPOSITE_HIERARCHY:
			{
				List<Composite*> open_items;
				QListViewItemIterator it(listview);
				for (; it.current(); ++it)
				{
					if (it.current()->isOpen())
					{
						open_items.push_back(((SelectableListViewItem*)*it)->getComposite());
					}
				}

				removeComposite(composite_message->getComposite()->getRoot());
 				addComposite(composite_message->getComposite()->getRoot());

				List<Composite*>::Iterator lit = open_items.begin();
				HashMap<Composite*, SelectableListViewItem*>::Iterator to_find;
				for (; lit != open_items.end(); lit++)
				{
					to_find = composite_to_item_.find(*lit);
					if (to_find == composite_to_item_.end()) continue;

					to_find->second->setOpen(true);
				}

				return true;
			}
			case CompositeMessage::SELECTED_COMPOSITE:
			case CompositeMessage::DESELECTED_COMPOSITE:
				updateListViewItem_(0, *composite_message->getComposite());
				return true;

			default:
				return false;
		}
	}
	else if (RTTI::isKindOf<NewSelectionMessage> (*message))
	{
		NewSelectionMessage* nsm = (NewSelectionMessage*) message;
		setSelection_(true, nsm->openItems());
		return true;
	}

	return false;
}

void MolecularControl::activatedItem_(int pos)
{
	if (pos >= 1000 && pos < 2000)
	{
		selected_model_ = (ModelType)(pos - 1000);
	}
	if (pos >= 2000 && pos < 3000)
	{
		selected_coloring_method_ = (ColoringMethod)(pos - 2000);
	}

}

void MolecularControl::buildContextMenu(Composite& composite)
	throw()
{
	bool composites_muteable = !getMainControl()->compositesAreLocked();
	bool one_item = (getSelection().size() == 1);

	context_menu_.insertItem("Create Representation", &model_menu_, 0, CREATE_REPRESENTATION);
	context_menu_.insertSeparator();

	context_menu_.insertItem("Move", this, SLOT(moveItems()), 0, OBJECT__MOVE);

	context_menu_.insertItem("Select", this, SLOT(select()), 0, SELECT);
	context_menu_.insertItem("Deselect", this, SLOT(deselect()), 0, DESELECT);
	context_menu_.setItemEnabled(SELECT,   !composite.isSelected() && composites_muteable);
	context_menu_.setItemEnabled(DESELECT,  composite.isSelected() && composites_muteable);

	context_menu_.insertSeparator();
	context_menu_.insertItem("Focus camera", this, SLOT(centerCamera()), 0, CAMERA__CENTER);
	context_menu_.insertSeparator();

	// -----------------------------------> AtomContainer
	context_menu_.insertItem("Count items", this, SLOT(countItems()), 0, COUNT__ITEMS);
	context_menu_.setItemEnabled(COUNT__ITEMS, RTTI::isKindOf<AtomContainer>(composite));
	// <----------------------------------- AtomContainer
	
	context_menu_.insertSeparator();
	context_menu_.insertItem("Properties", this, SLOT(compositeProperties()), 0, COMPOSITE__PROPERTIES);
	context_menu_.setItemEnabled(COMPOSITE__PROPERTIES, composites_muteable && one_item);

	// -----------------------------------> Atoms
	context_menu_.insertItem("Show Bonds", this, SLOT(bondProperties()), 0, BOND__PROPERTIES);
	context_menu_.setItemEnabled(BOND__PROPERTIES, 
															 RTTI::isKindOf<Atom>(composite) && one_item && composites_muteable);
	// <----------------------------------- Atoms

	context_menu_.insertSeparator();
	context_menu_.insertItem("Collapse all", this, SLOT(collapseAll()), 0, COLLAPSE_ALL);
	context_menu_.insertItem("Expand all", this, SLOT(expandAll()), 0, EXPAND_ALL);
	context_menu_.insertItem("Highlight Selection", this, SLOT(highlightSelection()));
}


void MolecularControl::compositeProperties()
{
	CompositeProperties as(context_composite_, this);
	as.exec();

	CompositeMessage* message = new CompositeMessage(
			*context_composite_, CompositeMessage::CHANGED_COMPOSITE_HIERARCHY);
	getMainControl()->sendMessage(*message);
}

void MolecularControl::bondProperties()
{
	Atom* const atom = dynamic_cast<Atom*>(context_composite_);
	if (atom->countBonds() == 0) 
	{
		setStatusbarText("Atom has no bonds!");
		return;
	}
	BondProperties bs(atom, this);
	bs.exec();
}
	
void MolecularControl::buildBonds()
{
	MolecularTaskMessage* message = new MolecularTaskMessage(MolecularTaskMessage::BUILD_BONDS);
	notify_(message);
}
	
void MolecularControl::centerCamera()
{
	CompositeMessage* message = new CompositeMessage(*context_composite_, CompositeMessage::CENTER_CAMERA);
	notify_(message);
}

void MolecularControl::checkResidue()
{
	MolecularTaskMessage* message = new MolecularTaskMessage(MolecularTaskMessage::CHECK_RESIDUE);
	notify_(message);
}

void MolecularControl::showFilename()
{
	if (context_composite_ == 0) return;

	const AtomContainer* const ac = dynamic_cast<AtomContainer*>(&context_composite_->getRoot());
	
	if (ac != 0)
	{
		setStatusbarText("Composite is from file  " +
											ac->getProperty("FROM_FILE").getString());
	}
	else
	{
		setStatusbarText("Composite is not from file");
	}
}


void MolecularControl::updateSelection()
{
	GenericControl::updateSelection();

	selected_.clear();

	// we have to prevent inserting childs of already selected parents, 
	// otherwise we get serious trouble
	QListViewItemIterator it(listview);
	for (; it.current(); ++it)
	{
		SelectableListViewItem* item = (SelectableListViewItem*) it.current();
		if (item->isSelected())
		{
			// always add systems
			Composite* const composite = item->getComposite();
			if (composite->getParent() == 0)
			{
				selected_.push_back(composite);
				continue;
			}

			List<Composite*>::Iterator lit = selected_.begin();
			for (; lit != selected_.end(); lit++)
			{
				if (composite->isDescendantOf(**lit)) break;
			}

			// no added parent found
			if (lit == selected_.end())
			{
				selected_.push_back(composite);
			}
		}
	}


	if (selected_.size() == 1 && RTTI::isKindOf<System>(**selected_.begin()))
	{
		context_composite_ = *selected_.begin();

		bool is_in_move_mode = 
			(Scene::getInstance(0) != 0) &&
			(Scene::getInstance(0)->getMode() == Scene::MOVE__MODE);
	
		if (!is_in_move_mode) showFilename();
	}

	// sent new selection through tree
	ControlSelectionMessage* message = new ControlSelectionMessage;
	message->setSelection(selected_);
	notify_(message);
}

void MolecularControl::onContextMenu_(QListViewItem* item,  const QPoint& point, int /* col*/)
{
	// clear the context menu
	context_menu_.clear();
	SelectableListViewItem* sitem = dynamic_cast<SelectableListViewItem*>(item);

	if (item == 0 || sitem == 0)
	{
		return;
	}

	Composite* composite = sitem->getComposite();

	// create the context menu
	if (composite != 0)
	{
		context_composite_ = composite;
		context_item_ = sitem;
		buildContextMenu(*composite);
	}

	// show the context menu if it is not empty
	if (context_menu_.count())
	{
		context_menu_.popup(point);
	}
}


void MolecularControl::selectedComposite_(Composite* composite, bool state)
{
	if (composite->isSelected() == state) return;
	
	CompositeMessage::CompositeMessageType id = CompositeMessage::DESELECTED_COMPOSITE;
	if (state) id = CompositeMessage::SELECTED_COMPOSITE;
	CompositeMessage* message = new CompositeMessage(*composite, id);
	notify_(message);
}


void MolecularControl::recurseGeneration_(SelectableListViewItem* item, Composite& composite)
	throw()
{
	Composite::ChildCompositeReverseIterator it = composite.rbeginChildComposite();
	for (; it != composite.rendChildComposite(); ++it)
	{
		generateListViewItem_(item, *it);
	}
}


void MolecularControl::recurseUpdate_(SelectableListViewItem* item, Composite& composite)
	throw()
{
	Composite::ChildCompositeReverseIterator it = composite.rbeginChildComposite();
	for (; it != composite.rendChildComposite(); ++it)
	{
		updateListViewItem_(item, *it);
	}
}

void MolecularControl::initializeWidget(MainControl& main_control)
	throw()
{
	String hint;

	hint = "Select a molecular object to see its position in the scene or to mark it for a simulation";
	select_id_ = insertMenuEntry(MainControl::EDIT, "&Select", this, SLOT(select()), ALT+Key_S, -1, hint);   
	hint = "Deselect a molecular object.";
	deselect_id_ = insertMenuEntry(MainControl::EDIT, "&Deselect", this, SLOT(deselect()), ALT+Key_D, -1, hint);
	main_control.insertPopupMenuSeparator(MainControl::EDIT);
	cut_id_ = main_control.insertMenuEntry(MainControl::EDIT, "Cu&t", this, SLOT(cut()), CTRL+Key_X);
	copy_id_ = main_control.insertMenuEntry(MainControl::EDIT, "&Copy", this, SLOT(copy()), CTRL+Key_C);
	paste_id_ = main_control.insertMenuEntry(MainControl::EDIT, "&Paste", this, SLOT(paste()), CTRL+Key_V);
	main_control.insertDeleteEntry();
	main_control.insertPopupMenuSeparator(MainControl::EDIT);
	hint = "Clear the items in the clipboard";
	clipboard_id_ = main_control.insertMenuEntry(MainControl::EDIT, "Clear Clipboard", this, 
																							 SLOT(clearClipboard()), 0, -1, hint);
	GenericControl::initializeWidget(main_control);
}


void MolecularControl::finalizeWidget(MainControl& main_control)
	throw()
{
	main_control.removeMenuEntry(MainControl::EDIT, "Cut", this, SLOT(cut()), CTRL+Key_X);
	main_control.removeMenuEntry(MainControl::EDIT, "&Copy", this, SLOT(copy()), CTRL+Key_C);
	main_control.removeMenuEntry(MainControl::EDIT, "Paste", this, SLOT(paste()), CTRL+Key_V);
	main_control.removeMenuEntry(MainControl::EDIT, "&Delete", this, SLOT(deleteCurrentItems()), 0);
	main_control.removeMenuEntry(MainControl::EDIT, "Clear Clipboard", this, SLOT(clearClipboard()));
	main_control.removeMenuEntry(MainControl::EDIT, "&Select", this, SLOT(select()), ALT+Key_S);   
	main_control.removeMenuEntry(MainControl::EDIT, "&Deselect", this, SLOT(deselect()), ALT+Key_D);   
	GenericControl::finalizeWidget(main_control);
}


void MolecularControl::addComposite(Composite& composite, QString* own_name)
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
	invalidateSelection();
	composite_to_item_[&composite]->setSelected(true);
	updateSelection();
}


Size MolecularControl::removeComposite(Composite& composite)
	throw()
{
	HashMap<Composite*, SelectableListViewItem*>::Iterator to_find = 
		composite_to_item_.find(&composite);

	if (to_find == composite_to_item_.end())
	{
		setStatusbarText(String("Tried to remove an invalid Composite in ") 
														 + __FILE__ + " " + __LINE__, true);
		return 0;
	}

	nr_items_removed_ = 1;
	removeRecursive_(to_find->second);
	return nr_items_removed_;
}

void MolecularControl::removeRecursive_(SelectableListViewItem* item)
	throw()
{
	composite_to_item_.erase(item->getComposite());

	SelectableListViewItem* child = dynamic_cast<SelectableListViewItem*>(item->firstChild());
	while (child != 0)
	{
		removeRecursive_(child);
		child = dynamic_cast<SelectableListViewItem*>(item->firstChild());
		nr_items_removed_++;
	}

	delete item;
}


void MolecularControl::onNotify(Message *message)
	throw()
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "MolecularControl " << this << " onNotify " << message << std::endl;
#endif

	listview->setUpdatesEnabled(false);

 	GenericControl::onNotify(message);

	// react accordingly to the given message
	if (reactToMessages_(message))
	{
		// if message makes an update of the control necessary
		// sends new selection through tree
		updateSelection();
	}
	listview->setUpdatesEnabled(true);
	listview->triggerUpdate();
}


void MolecularControl::invalidateSelection()
{
	listview->setUpdatesEnabled(false);
	QListViewItemIterator it(listview);
	for (; it.current(); ++it)
	{
		it.current()->setSelected(false);
	}

	listview->setUpdatesEnabled(true);
	listview->triggerUpdate();
}


// set the checkboxes according to the selection in the MainControl
void MolecularControl::setSelection_(bool open, bool force)
	throw()
{	
	const HashSet<Composite*>& selection = getMainControl()->getSelection();
	if (!selection.size())
	{
		listview->clearSelection();

		QListViewItemIterator it(listview);
		for (; it.current(); ++it)
		{
			SelectableListViewItem* item = dynamic_cast<SelectableListViewItem*>(it.current());
			item->setOn(false);
			item->setSelected(false);
		}
		return;
	}

	if (!force) open = false;

	Size nr = 0;
	QListViewItemIterator it(listview);
	for (; it.current(); ++it)
	{
		SelectableListViewItem* item = dynamic_cast<SelectableListViewItem*>(it.current());
		if (!item->getComposite()->isSelected()) 
		{
			item->setOn(false);
			item->setSelected(false);
			continue;
		}
		
		nr++;
		item->setOn(true);
		item->setSelected(true);
		if (open)
		{
			QListViewItem* parent = item->parent();
			while (parent != 0 && !parent->isOpen())
			{
				parent->setOpen(true);
				parent = parent->parent();
			}
		}
	}

	setStatusbarText(String(nr) + " objects selected.");
}


void MolecularControl::cut()
{
	// delete old composites in copy list
	if (!was_delete_)
	{
		clearClipboard();
	}

	// remove the selected composites from the tree and from the scene
	// if !was_delete_, copy them into the copy_list_
	// for all roots of removed items the representations are rebuild
	Size nr_of_items = 0;
	HashSet<Composite*> roots;

	List<Composite*>::Iterator it = selected_.begin();	
	for (; it != selected_.end(); it++)
	{
		getMainControl()->deselectCompositeRecursive(*it, false);

		if (!(**it).isRoot())
		{
			nr_of_items += removeComposite(**it);
			roots.insert(&(**it).getRoot());
			(**it).getParent()->removeChild(**it);
 			if (was_delete_) delete *it;
		}

		getMainControl()->remove(**it, was_delete_);

		if (!was_delete_) 
		{
 			copy_list_.push_back(*it);
		}
	}

	setStatusbarText("Deleted " + String(nr_of_items) + " items.");

	selected_.clear();
	ControlSelectionMessage* message = new ControlSelectionMessage;
	notify_(message);

	HashSet<Composite*>::Iterator roots_it = roots.begin();
	for (roots_it = roots.begin(); roots_it != roots.end(); roots_it++)
	{
		CompositeMessage* ccmessage = new CompositeMessage(**roots_it, CompositeMessage::CHANGED_COMPOSITE_HIERARCHY);
		notify_(ccmessage);
	}
}


void MolecularControl::copy()
{
	const List<Composite*> selection = getSelection();
	if (selection.size() == 0) return;

	setStatusbarText("copied " + String(selection.size()) + " objects ...");

	// delete old cutted composites
	clearClipboard();

	// copy the selected composites into the copy_list_
	List<Composite*>::Iterator it = selected_.begin();	
	for (; it != selected_.end(); it++)
	{
		copy_list_.push_back((Composite*)(*it)->create());
	}
}


void MolecularControl::paste()
{
	if (copy_list_.size() == 0) return;

	setStatusbarText("Pasted " + String(copy_list_.size()) + " objects...");

	HashSet<Composite*> changed_roots;
	// copying composites
	List<Composite*>::ConstIterator list_it = copy_list_.begin();	
	for (; list_it != copy_list_.end(); ++list_it)
	{
		if (RTTI::isKindOf<System>(**list_it))
		{
			// create a new copy of the composite
			Composite *new_composite = (Composite*)(*list_it)->create();

			// insert Composite in mainControl
			new_composite->host(getInformationVisitor_());
			getMainControl()->insert(*new_composite, getInformationVisitor_().getName());
 			continue;
		}

		if (selected_.size() != 1)
		{
			setStatusbarText("Could not paste, no or more than 1 item selected!");
			continue;
		}

		Composite *new_child = (Composite*)(*list_it)->create();

		Composite* parent = *selected_.begin();
		parent->appendChild(*new_child);
		changed_roots.insert(&parent->getRoot());
	}

	HashSet<Composite*>::Iterator it = changed_roots.begin();
	for (; it != changed_roots.end(); it++)
	{
		updateListViewItem_(0, **it);
		CompositeMessage *new_message = new CompositeMessage(**it, CompositeMessage::CHANGED_COMPOSITE_HIERARCHY);
		notify_(*new_message);
	}
}


void MolecularControl::clearClipboard()
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
}


void MolecularControl::moveItems()
{
	select();
	SceneMessage* msg = new SceneMessage(SceneMessage::ENTER_MOVE_MODE);
	notify_(msg);
}


MolecularControl::SelectableListViewItem* 
	MolecularControl::generateListViewItem_(SelectableListViewItem* parent, 
																					Composite& composite, QString* default_name)
	throw()
{
  #ifdef BALL_VIEW_DEBUG
	if (composite_to_item_.has(&composite))
	{
		Log.error() << "Composite " << & composite << " already added!" << std::endl;
	}
  #endif

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
		new_item = new SelectableListViewItem(listview, name, type, &composite, *this);
	} 
	else 
	{
		// no, insert into the current item
		new_item = new SelectableListViewItem(parent, name, type, &composite, *this);
	}

	composite_to_item_[&composite] = new_item;
	recurseGeneration_(new_item, composite);

	return new_item;
}


void MolecularControl::updateListViewItem_(SelectableListViewItem* parent, Composite& composite)
	throw()
{
	HashMap<Composite*, SelectableListViewItem*>::Iterator to_find = 
		composite_to_item_.find(&composite);

	// an item does not exist => create a new SelectableListViewItem 
	if (to_find == composite_to_item_.end())
	{
		generateListViewItem_(parent, composite);
		return;
	}

	recurseUpdate_(to_find->second, composite);
}


MolecularInformation& MolecularControl::getInformationVisitor_()
	throw()
{
	return information_;
}

List<Composite*>& MolecularControl::getSelection()
	throw()
{
	return selected_;
}

const List<Composite*>& MolecularControl::getSelection() const
	throw()
{
	return selected_;
}

void MolecularControl::createRepresentation() 
{
	// make sure selection is send, to enter create Represenation Mode in DP
	ControlSelectionMessage* message = new ControlSelectionMessage;
	message->setSelection(selected_);
	notify_(message);

	ShowDisplayPropertiesMessage* msg = new ShowDisplayPropertiesMessage;
	notify_(msg);
}

void MolecularControl::collapseAll()
{
	QListViewItemIterator it(listview);
	for (; it.current(); ++it)
	{
		(*it)->setOpen(false);
	}
}

void MolecularControl::expandAll()
{
	QListViewItemIterator it(listview);
	for (; it.current(); ++it)
	{
		(*it)->setOpen(true);
	}
}

void MolecularControl::createRepresentation_()
{
	CreateRepresentationMessage* crm = new CreateRepresentationMessage(selected_, 
			selected_model_, selected_coloring_method_);
	notify_(crm);
}

void MolecularControl::countItems()
{
	if (context_item_ == 0 ||
			context_item_->getComposite() == 0 || 
			!RTTI::isKindOf<AtomContainer>(*context_item_->getComposite()))
	{
		return;
	}
	AtomContainer& ac = *(AtomContainer*) context_item_->getComposite(); 

	String s;
	if (RTTI::isKindOf<System>(ac))
	{
		s+=String(((System*)&ac)->countResidues());
		s+= " Residues, ";
	}
	else if (RTTI::isKindOf<Protein>(ac))
	{
		s+=String(((Protein*)&ac)->countResidues());
		s+= " Residues, ";
	}
	else if (RTTI::isKindOf<Chain>(ac))
	{
		s+=String(((Chain*)&ac)->countResidues());
		s+= " Residues, ";
	}
	if (RTTI::isKindOf<SecondaryStructure>(ac))
	{
		s+=String(((SecondaryStructure*)&ac)->countResidues());
		s+= " Residues, ";
	}

	s+=String(ac.countAtoms()) + " Atoms, ";
	s+=String(ac.countBonds()) + " Bonds";
	setStatusbarText(s);
}

Size MolecularControl::applySelector()
{
	if (parentWidget() == 0) return 0;
	if (selector_edit_->text() == "")
	{
		getMainControl()->clearSelection();
		return 0;
	}

	Selector s;
	try
	{
		s.setExpression(Expression(selector_edit_->text().ascii()));
	}
	catch(Exception::ParseError e)
	{
		setStatusbarText(String("Invalid expression ") + e.getMessage());
		return 0;
	}

	HashSet<Composite*> roots;
	Size nr_of_atoms = 0;

	CompositeManager::CompositeIterator it = getMainControl()->getCompositeManager().begin();
	for(; it != getMainControl()->getCompositeManager().end(); it++)
	{
		(*it)->apply(s);
		List<Atom*>::Iterator ait = s.getSelectedAtoms().begin();
		for (; ait != s.getSelectedAtoms().end(); ait++)
		{
			getMainControl()->selectCompositeRecursive(*ait, true);
			roots.insert(&(*ait)->getRoot());
		}
		nr_of_atoms += s.getSelectedAtoms().size();
	}

	HashSet<Composite*>::Iterator sit = roots.begin();
	for (; sit != roots.end(); sit++)
	{
	 	getMainControl()->updateRepresentationsOf(**sit, false);
	}

	NewSelectionMessage* nm = new NewSelectionMessage;
	nm->setOpenItems(true);
	getMainControl()->sendMessage(*nm);

	setStatusbarText(String("Selected " + String(s.getNumberOfSelectedAtoms()) + " Atoms."));
	listview->setFocus();

	return nr_of_atoms;
}


void MolecularControl::showSelectorHelp()
{
	QMessageBox::information( this, "BALLView",
			String(
			String("In this text field, you can enter regular expressions to select molecular entities.\n")+
			"To apply your selection, just press Return key after you are finished. If you want to\n"+
			"clear your selection, just click on the button next to the help button.\n\n"+
			"Possible predicates are: \n"+
			"true() \t this is always true\n" +
			"false() \t this is always false\n" +
			"selected() \t this is true for already selected atoms\n" +
			"name(string) \t the name of the atom \n" +
			"type(string) \t the type name of the atom\n" +
			"element(char) \t the element (abbreviated by its symbol)\n" +
			"residue(string) \t the name of the residue containing the atom\n" +
			"residueID(int) \t the PDB ID of the residue (usally a number)\n" +
			"protein() \t the name of the protein the atom is contained in\n" +
			"secondaryStruct() \t the name of the secondary structure the atom is contained in\n" +
			"solvent() \t true if the atom is a solvent atom added by BALL\n" +
			"backbone() \t true for backbone atoms\n" +
			"chain() \n" +
			"nucleotide() \n" +
			"inRing() \n" +
			"doubleBonds() \n" +
			"tripleBonds() \n" +
			"aromaticBonds() \n" +
			"numberOfBonds(int) \n" +
			"connectedTo(char) \n" +
			"sp3Hybridized() \n" +
			"sp2Hybridized() \n" +
			"spHybridized() \n" +
			"charge() \n" +
			"isAxial() \n" +
			"is4C1() \n\n" +
			"They can be connected with\n" +
			"AND and OR, grouped with brackets, and each predicate can be negated with '!'\n"
			).c_str(),
			"&OK");
}

void MolecularControl::clearSelector()
{
	selector_edit_->setText("");
	getMainControl()->clearSelection();
}


void MolecularControl::deleteCurrentItems()
	throw()
{
	was_delete_ = true;
	cut();
	was_delete_ = false;
}


bool MolecularControl::pasteAllowedFor_(Composite& child)
	throw()
{
	if (getSelection().size() == 0 &&
			RTTI::isKindOf<System>(child))
	{
		return true;
	}

	Composite& parent = **getSelection().begin();

	if (RTTI::isKindOf<System>(child)) return false;

	if (RTTI::isKindOf<Atom>(parent)) return false;

	if (RTTI::isKindOf<Residue>(parent)) return (RTTI::isKindOf<Atom>(child));

	if (RTTI::isKindOf<SecondaryStructure>(parent)) return (RTTI::isKindOf<Residue>(child));

	if (RTTI::isKindOf<Chain>(parent)) return (RTTI::isKindOf<SecondaryStructure>(child) ||
																						 RTTI::isKindOf<Residue>(child));
	
	if (RTTI::isKindOf<Protein>(parent)) return (RTTI::isKindOf<Chain>(child));

	if (RTTI::isKindOf<Nucleotide>(parent)) return (RTTI::isKindOf<Atom>(child));

	if (RTTI::isKindOf<NucleicAcid>(parent)) return (RTTI::isKindOf<Nucleotide>(child));

	if (RTTI::isKindOf<System>(parent)) return (RTTI::isKindOf<Molecule>(child));

	return true;
}


void MolecularControl::select()
{
	// copy list, because selection could change
	List<Composite*> selection = selected_;

	List<Composite*>::Iterator it = selection.begin();
	for(; it != selection.end(); it++)
	{
		selectedComposite_(*it, true);
	}
}


void MolecularControl::deselect()
{
	// copy list, because selection could change
	List<Composite*> selection = selected_;

	List<Composite*>::Iterator it = selection.begin();
	for(; it != selection.end(); it++)
	{
		selectedComposite_(*it, false);
	}	
}

Size MolecularControl::applySelector(const String& expression)
	throw()
{
	selector_edit_->setText(expression.c_str());
	return applySelector();
}

void MolecularControl::highlightSelection()
	throw()
{
	listview->setUpdatesEnabled(false);
	QListViewItemIterator it(listview);
	for (; it.current(); ++it)
	{
		SelectableListViewItem* item = (SelectableListViewItem*) it.current();
		if (item->isOn())
		{
			item->setOpen(true);
			listview->setSelected(item, true);
			QListViewItem* parent = item->parent();
			while (parent != 0 && !parent->isOpen())
			{
				parent->setOpen(true);
				parent = parent->parent();
			}
		}
		else
		{
			item->setOpen(false);
			listview->setSelected(item, false);
		}
			
	}
	listview->setUpdatesEnabled(true);
	listview->triggerUpdate();
}

} } // namespaces
