// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: molecularControl.C,v 1.27 2003/12/23 17:59:28 amoll Exp $

#include <BALL/VIEW/WIDGETS/molecularControl.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/DIALOGS/atomProperties.h>
#include <BALL/VIEW/DIALOGS/bondProperties.h>
#include <BALL/VIEW/DIALOGS/transformationDialog.h>
#include <BALL/KERNEL/system.h>
#include <qmenubar.h>

using std::endl;

namespace BALL
{
	namespace VIEW
	{

MolecularControl::SelectableListViewItem::SelectableListViewItem(QListViewItem* parent, const QString& text, const QString& type, Composite* composite, VIEW::MolecularControl& control)
	throw()
	: QCheckListItem(parent, text, QCheckListItem::CheckBox),
		composite_(composite),
		control_reference_(control)
{
	setText(1, type);
}


MolecularControl::SelectableListViewItem::SelectableListViewItem(QListView* parent, const QString& text, const QString& type, Composite* composite, VIEW::MolecularControl& control)
	throw()
	: QCheckListItem(parent, text, QCheckListItem::CheckBox),
		composite_(composite),
		control_reference_(control)
{
	setText(1, type);
}


void MolecularControl::SelectableListViewItem::stateChange(bool)
	throw()
{
	control_reference_.selectedComposite_(composite_, isOn());
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
			context_menu_(this),
			model_menu_(this),
			context_composite_(0),
			transformation_dialog_(0)
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "new MolecularControl " << this << std::endl;
#endif
	listview->addColumn("[selected] Name");
	listview->addColumn("Type");
	listview->setColumnWidth(0, 120);
	listview->setColumnWidth(1, 60);

	// if the selection of any item changed,
	// mark the complete selection as invalid
	// it is then re-determined by getSelection()
	connect(listview, SIGNAL(selectionChanged()), this, SLOT(updateSelection()));

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
}

MolecularControl::~MolecularControl()
	throw()
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "Destroying MolecularControl " << this << std::endl;
#endif
	if (transformation_dialog_) delete transformation_dialog_;
}

void MolecularControl::checkMenu(MainControl& main_control)
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


	List<Composite*>::Iterator it = selected_.begin();	
	for (; it != selected_.end(); it++)
	{
		if (!RTTI::isKindOf<System>(**it)) 
		{
			(main_control.menuBar())->setItemEnabled(copy_id_, true);	
			return;
		}
	}

	// copy is only available for top level selections
	(main_control.menuBar())->setItemEnabled(copy_id_, true);	
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
			
			case CompositeMessage::CHANGED_COMPOSITE_AND_UPDATE_MOLECULAR_CONTROL:
			case CompositeMessage::CHANGED_COMPOSITE:
				removeComposite(*(Composite *)composite_message->getComposite());
				addComposite(*(Composite *)composite_message->getComposite());
				return (composite_message->getType() == CompositeMessage::CHANGED_COMPOSITE_AND_UPDATE_MOLECULAR_CONTROL);
				
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
		updateSelection();
	}

	return false;
}

void MolecularControl::activatedItem_(int pos)
{
	if (pos >= 1000 && pos < 2000)
	{
		selected_model_ = (ModelType)(pos -1000);
	}
	if (pos >= 2000 && pos < 3000)
	{
		selected_coloring_method_ = (ColoringMethod)(pos -2000);
	}

}

void MolecularControl::buildContextMenu(Composite& composite)
	throw()
{
	context_menu_.insertItem("Create Representation...", this, 
			SLOT(createRepresentation()), 0, CREATE_REPRESENTATION_MODE);
	context_menu_.insertItem("Create Representation", &model_menu_, 0, CREATE_REPRESENTATION);
	context_menu_.insertSeparator();

	context_menu_.insertItem("Cut", this, SLOT(cut()), 0, OBJECT__CUT);
	context_menu_.insertItem("Copy", this, SLOT(copy()), 0, OBJECT__COPY);
	context_menu_.insertItem("Paste", this, SLOT(paste()), 0, OBJECT__PASTE);

	context_menu_.insertItem("Move", this, SLOT(move()), 0, OBJECT__MOVE);

	bool composites_muteable = getMainControl()->compositesAreMuteable();
	context_menu_.setItemEnabled(OBJECT__CUT, composites_muteable);
	context_menu_.setItemEnabled(OBJECT__PASTE, getCopyList_().size() > 0 && composites_muteable);
	context_menu_.setItemEnabled(OBJECT__MOVE, composites_muteable);
	context_menu_.insertSeparator();

	context_menu_.insertItem("Select", this, SLOT(select()), 0, SELECT);
	context_menu_.insertItem("Deselect", this, SLOT(deselect()), 0, DESELECT);
	context_menu_.setItemEnabled(SELECT,   !composite.isSelected() && composites_muteable);
	context_menu_.setItemEnabled(DESELECT,  composite.isSelected() && composites_muteable);

	context_menu_.insertSeparator();

	if (RTTI::isKindOf<AtomContainer>(composite))
	{
		context_menu_.insertItem("Check residue", this, SLOT(checkResidue()), 0, RESIDUE__CHECK);

		bool system_selected = true;
		List<Composite*>::Iterator it = selected_.begin();	
		for (; it != selected_.end(); it++)
		{
			if (!RTTI::isKindOf<System>(**it)) system_selected = false;
			break;
		}

		context_menu_.setItemEnabled(OBJECT__COPY, system_selected);

		context_menu_.insertItem("Build Bonds", this, SLOT(buildBonds()), 0, BONDS__BUILD);
		context_menu_.setItemEnabled(BONDS__BUILD, composites_muteable);

		context_menu_.insertItem("Count items", this, SLOT(countItems()), 0, COUNT__ITEMS);
	}

	context_menu_.insertItem("Focus camera", this, SLOT(centerCamera()), 0, CAMERA__CENTER);

	if (RTTI::isKindOf<Atom>(composite))
	{
		context_menu_.insertItem("Properties", this, SLOT(atomProperties()), 0, ATOM__PROPERTIES);
		context_menu_.insertItem("Show Bonds", this, SLOT(bondProperties()), 0, BOND__PROPERTIES);
	}

	context_menu_.insertSeparator();
	context_menu_.insertItem("Show filename", this, SLOT(showFilename()), 0, SHOW__FILENAME);
	context_menu_.insertSeparator();
	context_menu_.insertItem("Collapse all", this, SLOT(collapseAll()), 0, COLLAPSE_ALL);
}


void MolecularControl::atomProperties()
{
	AtomProperties as((Atom*)context_composite_, this);
	as.exec();

	CompositeMessage* message = new CompositeMessage(
			*context_composite_, CompositeMessage::CHANGED_COMPOSITE_AND_UPDATE_MOLECULAR_CONTROL);
	notify_(message);
}

void MolecularControl::bondProperties()
{
	BondProperties bs((Atom*) context_composite_, this);
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
	Composite* c_ptr = context_composite_;
	c_ptr = &context_composite_->getRoot();
	
	if (RTTI::isKindOf<AtomContainer>(*c_ptr))
	{
		setStatusbarText("Composite is from file  " +
											((const AtomContainer*)c_ptr)->getProperty("FROM_FILE").getString());
	}
	else
	{
		setStatusbarText("Composite is not from file");
	}
}


void MolecularControl::updateSelection()
{
	selected_.clear();

	// we have to prevent, to insert child items of already selected parents,
	// otherwise we get trouble in DisplayProperties
	QListViewItemIterator it(listview);
	for (; it.current(); ++it)
	{
		SelectableListViewItem* item = (SelectableListViewItem*) it.current();
		if (item->isSelected())
		{
			// always add systems
			Composite* composite = item->getComposite();
			if (composite->getParent() == 0)
			{
				selected_.push_back(composite);
				continue;
			}

			List<Composite*>::Iterator lit = selected_.begin();
			for (; lit != selected_.end(); lit++)
			{
				if (composite->isDescendantOf(**lit))
				{
					break;
				}
			}
			// no added parent found
			if (lit == selected_.end())
			{
				selected_.push_back(composite);
			}
		}
	}

	
	if (transformation_dialog_ && selected_.size()>0)
	{
		transformation_dialog_->setComposite(*selected_.begin());
	}

	if (selected_.size() == 1 && RTTI::isKindOf<System>(**selected_.begin()))
	{
		context_composite_ = *selected_.begin();
		showFilename();
	}

	// sent new selection through tree
	ControlSelectionMessage* message = new ControlSelectionMessage;
	message->setSelection(selected_);
	notify_(message);
}

void MolecularControl::onContextMenu_(QListViewItem* item,  const QPoint& point, int /* column */)
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
		context_menu_.popup(point);
	}
}


void MolecularControl::selectedComposite_(Composite* composite, bool state)
{
	const HashSet<Composite*>& selection = getMainControl()->getSelection();
	if (selection.has(composite) == state)
	{
		return;
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

	CompositeMessage::CompositeMessageType id = CompositeMessage::DESELECTED_COMPOSITE;
	if (state) id = CompositeMessage::SELECTED_COMPOSITE;
	CompositeMessage* message = new CompositeMessage(*composite, id);
	notify_(message);
	
	setSelection_(false);
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
	main_control.insertPopupMenuSeparator(MainControl::EDIT);
	cut_id_ = main_control.insertMenuEntry(MainControl::EDIT, "Cut", this, SLOT(cut()), CTRL+Key_X);
	copy_id_ = main_control.insertMenuEntry(MainControl::EDIT, "&Copy", this, SLOT(copy()), CTRL+Key_C);
	paste_id_ = main_control.insertMenuEntry(MainControl::EDIT, "Paste", this, SLOT(paste()), CTRL+Key_V);
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
	main_control.removeMenuEntry(MainControl::EDIT, "Clear Clipboard", this, SLOT(clearClipboard()));
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


void MolecularControl::removeComposite(Composite& composite)
	throw()
{
	if (!composite_to_item_.has(&composite)) return;

	SelectableListViewItem* item = composite_to_item_[&composite];
	delete item;
	composite_to_item_.erase(&composite);		
	listview->triggerUpdate();
}


Size MolecularControl::removeRecursiveComposite(Composite& composite)
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


void MolecularControl::onNotify(Message *message)
	throw()
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "MolecularControl " << this << " onNotify " << message << std::endl;
#endif
	// react accordingly to the given message
	if (reactToMessages_(message))
	{
		// if message makes an update of the control necessary
		// sends new selection through tree
		updateSelection();
	}
}


void MolecularControl::invalidateSelection()
{
	QListViewItemIterator it(listview);
	for (; it.current(); ++it)
	{
		it.current()->setSelected(FALSE);
	}

	listview->triggerUpdate();
}


// set the checkboxes according to the selection in the MainControl
void MolecularControl::setSelection_(bool open, bool force)
	throw()
{	
	const HashSet<Composite*>& selection = getMainControl()->getSelection();
	
	if (selection.size() == 0)
	{
		QListViewItemIterator it(listview);
		for (; it.current(); ++it)
		{
			((QCheckListItem*) it.current())->setOn(false);
		}
		return;
	}

	if (selection.size() > 50 && !force) open = false;

	QListViewItemIterator it(listview);
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


void MolecularControl::cut()
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
	
	Size nr_of_items = 0;

	delete_it = to_delete.begin();
	for (; delete_it!= to_delete.end(); delete_it++)
	{
		// remove composite representation from tree
		nr_of_items += removeRecursiveComposite(**delete_it);
		CompositeMessage* remove_message = new CompositeMessage(**delete_it, CompositeMessage::REMOVED_COMPOSITE);
		notify_(remove_message);
	}

	setStatusbarText("Deleted " + String(nr_of_items) + " items.");

	selected_.clear();
	ControlSelectionMessage* message = new ControlSelectionMessage;
	notify_(message);

	if (!roots.size()) return;
	HashSet<Composite*>::Iterator roots_it = roots.begin();
	for (roots_it = roots.begin(); roots_it != roots.end(); roots_it++)
	{
		CompositeMessage* ccmessage = new CompositeMessage(**roots_it, CompositeMessage::CHANGED_COMPOSITE);
		notify_(ccmessage);
	}
}


void MolecularControl::copy()
{
	const List<Composite*> selection = getSelection();
	if (selection.size() == 0) return;

	setStatusbarText("copied " + String(selection.size()) + " objects ...");

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
}


void MolecularControl::paste()
{
	if (copy_list_.size() == 0) return;

	setStatusbarText("pasted " + String(copy_list_.size()) + " objects...");

	// copying composites
	List<Composite*>::ConstIterator list_it = copy_list_.begin();	
	for (; list_it != copy_list_.end(); ++list_it)
	{
		// create a new copy of the composite
		Composite *new_composite = (Composite*)(*list_it)->create();

		// insert Composite in mainControl
		new_composite->host(getInformationVisitor_());
		CompositeMessage *new_message = new CompositeMessage(*new_composite, CompositeMessage::NEW_COMPOSITE);
		new_message->setCompositeName(getInformationVisitor_().getName());
		notify_(new_message);
	}
}


void MolecularControl::clearClipboard()
{
	setStatusbarText("cleared clipboard");

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

void MolecularControl::move()
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


MolecularControl::SelectableListViewItem* 
	MolecularControl::generateListViewItem_(SelectableListViewItem* parent, 
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
		new_item = new SelectableListViewItem(listview, name, type, &composite, *this);
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

void MolecularControl::updateListViewItem_(SelectableListViewItem* parent, Composite& composite)
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

List<Composite*>& MolecularControl::getCopyList_()
	throw()
{
	return copy_list_;
}
			
const List<Composite*>& MolecularControl::getCopyList_() const
	throw()
{
	return copy_list_;
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
	s+=String(ac.countBonds()) + " Bonds, with ";
	s+=String(ac.countInterBonds()) + " InterBonds";
	setStatusbarText(s);
}

} } // namespaces
