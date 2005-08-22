// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: molecularControl.C,v 1.96.2.32 2005/08/22 13:15:23 amoll Exp $
//

#include <BALL/VIEW/WIDGETS/molecularControl.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/DIALOGS/compositeProperties.h>
#include <BALL/VIEW/DIALOGS/bondProperties.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/selector.h>
#include <qmenubar.h>
#include <qpushbutton.h> 
#include <qlineedit.h> 
#include <qmessagebox.h> 
#include <qtooltip.h> 

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
					selector_edit_(new QComboBox(this)),
					context_menu_(this),
					model_menu_(this),
					edit_menu_(this),
					context_composite_(0),
					was_delete_(false),
					nr_items_removed_(0),
					show_ss_(false)
		{
		#ifdef BALL_VIEW_DEBUG
			Log.error() << "new MolecularControl " << this << std::endl;
		#endif
			listview->addColumn("[selected] Name");
			listview->addColumn("Type");
			listview->setColumnWidth(0, 120);
			listview->setColumnWidth(1, 60);

			QVBoxLayout* layout2 = new QVBoxLayout();
			getLayout()->addLayout(layout2);

			selector_edit_->resize(90, 45);
			selector_edit_->setSizePolicy( QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed, 0, 0, false));
			selector_edit_->setAutoCompletion(true);
			selector_edit_->setDuplicatesEnabled(false);
			selector_edit_->setEditable(true);
			layout2->addWidget(selector_edit_);

			QPushButton* clear_button = new QPushButton(this);
			clear_button->resize(60, 25);
			clear_button->setMinimumSize(40, 25);
			clear_button->setText("Clear");
			clear_button->setSizePolicy( QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred, 0, 0, false));
			connect(clear_button, SIGNAL(clicked()), this, SLOT(clearSelector()));
			QToolTip::add(clear_button, tr("Clear the selection."));
			QHBoxLayout* layout3 = new QHBoxLayout;
			layout3->addWidget(clear_button);
			layout2->addLayout(layout3);

			QPushButton* help_button = new QPushButton(this);
			help_button->resize(60, 25);
			help_button->setMinimumSize(40, 25);
			help_button->setText("Help");
			help_button->setSizePolicy( QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred, 0, 0, false));
			connect(help_button, SIGNAL(clicked()), this, SLOT(showSelectorHelp()));
			QToolTip::add(help_button, tr("Show a help dialog."));
			layout3->addWidget(help_button);

			QPushButton* select_button = new QPushButton(this);
			select_button->resize(60, 25);
			select_button->setText("Select");
			select_button->setDefault(true);
			select_button->setMinimumSize(40, 25);
			select_button->setSizePolicy( QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred, 0, 0, false));
			connect(select_button, SIGNAL(clicked()), this, SLOT(applySelector()));
			QToolTip::add(select_button, tr("Apply the current expression."));
			layout3->addWidget(select_button);

			// if the selection of any item changed,
			// mark the complete selection as invalid
			// it is then re-determined by getSelection()
			connect(listview, SIGNAL(selectionChanged()), this, SLOT(updateSelection()));

			buildContextMenu_();

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
												 copy_list_.size() > 0 && 
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
						addComposite(*(Composite *)composite_message->getComposite(), 
																			 composite_message->getCompositeName());
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

			if (RTTI::isKindOf<ControlSelectionMessage>(*message))
			{
				ControlSelectionMessage* nsm = (ControlSelectionMessage*) message;
				setHighlighting_(nsm->getSelection());
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

		void MolecularControl::buildContextMenu_()
		{

			// ===============================================================
			// create representation context menu:
			// ===============================================================

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

			// ===============================================================
			// main context menu:
			// ===============================================================

			context_menu_.insertItem("Create Representation", &model_menu_, CREATE_REPRESENTATION);
			context_menu_.insertSeparator();
			context_menu_.insertItem("Edit", &edit_menu_, EDIT_MENU);
			context_menu_.insertSeparator();

			context_menu_.insertItem("Select", this, SLOT(select()), 0, SELECT);
			context_menu_.insertItem("Deselect", this, SLOT(deselect()), 0, DESELECT);

			context_menu_.insertSeparator();
			context_menu_.insertItem("Focus", this, SLOT(centerCamera()), 0, CAMERA__CENTER);
			context_menu_.insertSeparator();

			// -----------------------------------> AtomContainer
			context_menu_.insertItem("Count items", this, SLOT(countItems()), 0, COUNT__ITEMS);
			// <----------------------------------- AtomContainer
			
			context_menu_.insertSeparator();
			context_menu_.insertItem("Properties", this, SLOT(compositeProperties()), 0, COMPOSITE__PROPERTIES);

			// -----------------------------------> Atoms
			context_menu_.insertItem("Show Bonds", this, SLOT(bondProperties()), 0, BOND__PROPERTIES);
			// <----------------------------------- Atoms

			context_menu_.insertSeparator();
			context_menu_.insertItem("Collapse all", this, SLOT(collapseAll()), 0, COLLAPSE_ALL);
			context_menu_.insertItem("Expand all", this, SLOT(expandAll()), 0, EXPAND_ALL);
			context_menu_.insertItem("Highlight Selection", this, SLOT(highlightSelection()));
			context_menu_.insertSeparator();
			show_ss_id_ = context_menu_.insertItem("Show Secondary Structures", this, 
																						SLOT(switchShowSecondaryStructure()));

			// ===============================================================
			// edit context menu:
			// ===============================================================

			edit_menu_.insertItem("Cut", this, SLOT(cut()), 0, CUT);
			edit_menu_.insertItem("Copy", this, SLOT(copy()), 0, COPY);
			edit_menu_.insertItem("Paste", this, SLOT(paste()), 0, PASTE);
			edit_menu_.insertItem("Delete", this, SLOT(deleteCurrentItems()), 0, DELETE_ENTRY);
			edit_menu_.insertSeparator();
			edit_menu_.insertItem("Move", this, SLOT(moveItems()), 0, OBJECT__MOVE);
		}

		void MolecularControl::updateContextMenu(Composite& composite)
			throw()
		{
			bool composites_muteable = !getMainControl()->compositesAreLocked();
			bool one_item = (getSelection().size() == 1);

			context_menu_.setItemEnabled(SELECT,   !composite.isSelected() && composites_muteable);
			context_menu_.setItemEnabled(DESELECT,  composite.isSelected() && composites_muteable);

			// -----------------------------------> AtomContainer
			context_menu_.setItemEnabled(COUNT__ITEMS, RTTI::isKindOf<AtomContainer>(composite));
			// <----------------------------------- AtomContainer
			
			// -----------------------------------> Atoms
			context_menu_.setItemEnabled(BOND__PROPERTIES, 
																	 RTTI::isKindOf<Atom>(composite) && one_item && composites_muteable);
			// <----------------------------------- Atoms

			context_menu_.setItemEnabled(CREATE_REPRESENTATION, composites_muteable);
			context_menu_.setItemEnabled(EDIT_MENU, composites_muteable);
			context_menu_.setItemEnabled(COMPOSITE__PROPERTIES, composites_muteable && one_item);
			context_menu_.setItemEnabled(CAMERA__CENTER, composites_muteable);
			context_menu_.setItemEnabled(COUNT__ITEMS, composites_muteable);
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
				updateContextMenu(*composite);
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

			select_id_ = insertMenuEntry(MainControl::EDIT, "&Select", this, SLOT(select()));   
			setMenuHint("Select a molecular object to see its position in the scene or to mark it for a simulation");
			deselect_id_ = insertMenuEntry(MainControl::EDIT, "&Deselect", this, SLOT(deselect()));
			setMenuHint("Deselect a molecular object.");

			main_control.insertPopupMenuSeparator(MainControl::EDIT);

			cut_id_ = main_control.insertMenuEntry(MainControl::EDIT, "Cu&t", this, SLOT(cut()), CTRL+Key_X);
			copy_id_ = main_control.insertMenuEntry(MainControl::EDIT, "&Copy", this, SLOT(copy()), CTRL+Key_C);
			paste_id_ = main_control.insertMenuEntry(MainControl::EDIT, "&Paste", this, SLOT(paste()), CTRL+Key_V);

			main_control.insertDeleteEntry();
			main_control.insertPopupMenuSeparator(MainControl::EDIT);

			clipboard_id_ = main_control.insertMenuEntry(MainControl::EDIT, "Clear Clipboard", this, 
																									 SLOT(clearClipboard()));
			setMenuHint("Clear the items in the clipboard");

			GenericControl::initializeWidget(main_control);

			registerWidgetForHelpSystem(this, "molecularControl.html");
			registerWidgetForHelpSystem(selector_edit_, "molecularControl.html#regular_expressions"); 
		}


		void MolecularControl::addComposite(Composite& composite, String given_name)
			throw()
		{
			// get information about the composite
			composite.host(getInformationVisitor_());

			// if the own name is empty use name from information visitor
			if (given_name == "")
			{
				given_name = getInformationVisitor_().getName();
			}

			QString name = given_name.c_str();

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
																 + String(__FILE__) + " " + String(__LINE__), true);
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
				listview->setUpdatesEnabled(true);
				listview->triggerUpdate();
				return;
			}
			listview->setUpdatesEnabled(true);
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

		// set the highlighting according to the selection in the MainControl
		void MolecularControl::setHighlighting_(List<Composite*> selection)
			throw()
		{	
			listview->clearSelection();

			List<Composite*>::Iterator cit = selection.begin();
			for (; cit != selection.end(); cit++)
			{
				if (*cit == 0 || 
						!composite_to_item_.has(*cit) ||
						composite_to_item_[*cit] == 0) 
				{
		#ifdef BALL_VIEW_DEBUG
					BALLVIEW_DEBUG
		#endif
					continue;
				}
				composite_to_item_[*cit]->setSelected(true);
			}
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
			if (getMainControl()->compositesAreLocked()) return;

			// delete old composites in copy list
			if (!was_delete_) clearClipboard();

			listview->setUpdatesEnabled(false);

			// remove the selected composites from the tree and from the scene
			// if !was_delete_, copy them into the copy_list_
			// for all roots of removed items the representations are rebuild
			HashSet<Composite*> roots;

			List<Composite*>::Iterator it = selected_.begin();	
			for (; it != selected_.end(); it++)
			{
				if (!(**it).isRoot()) roots.insert(&(**it).getRoot());

				getMainControl()->remove(**it, was_delete_, false);

				if (!was_delete_) copy_list_.push_back(*it);
			}

			selected_.clear();
			listview->setUpdatesEnabled(true);
			listview->triggerUpdate();
			ControlSelectionMessage* message = new ControlSelectionMessage;
			notify_(message);

			HashSet<Composite*>::Iterator roots_it = roots.begin();
			for (; +roots_it; roots_it++)
			{
				getMainControl()->update(**roots_it, true);
			}
		}


		void MolecularControl::copy()
		{
			if (getMainControl()->compositesAreLocked()) return;

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
			if (getMainControl()->compositesAreLocked()) return;

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

			// update of representations, etc.
			HashSet<Composite*>::Iterator it = changed_roots.begin();
			for (it = changed_roots.begin(); it != changed_roots.end(); it++)
			{
				getMainControl()->update(**it, true);
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

			if (!show_ss_)
			{
				if (RTTI::isKindOf<SecondaryStructure>(composite))
				{
					recurseGeneration_(parent, composite);
					return 0;
				}
			}

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

			s += String(ac.countAtoms()) + " Atoms, ";
			s += String(ac.countBonds()) + " Bonds";
			setStatusbarText(s, true);
		}

		Size MolecularControl::applySelector()
		{
			if (parentWidget() == 0) return 0;
			if (selector_edit_->currentText() == "")
			{
				getMainControl()->clearSelection();
				return 0;
			}

			Selector s;
			try
			{
				s.setExpression(Expression(selector_edit_->currentText().ascii()));
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
			showHelp("molecularControl.html#regular_expressions");
		}

		void MolecularControl::clearSelector()
		{
			selector_edit_->setCurrentText("");
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
			if (RTTI::isKindOf<System>(child)) 
			{
				return !getSelection().size();
			}

			if (!getSelection().size()) return false;

			const Composite& parent = **getSelection().begin();

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
			selector_edit_->setCurrentText(expression.c_str());
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

		void MolecularControl::switchShowSecondaryStructure()
		{
			if (show_ss_) show_ss_ = false;
			else 					show_ss_ = true;

			context_menu_.setItemChecked(show_ss_id_, show_ss_);

			CompositeManager::CompositeConstIterator cit = getMainControl()->getCompositeManager().begin();
			for (; +cit; ++cit)
			{
				removeComposite(**cit);
			}

			cit = getMainControl()->getCompositeManager().begin();
			for (; +cit; ++cit)
			{
				addComposite(**cit);
			}
		}

		void MolecularControl::fetchPreferences(INIFile& inifile)
			throw()
		{
			DockWidget::fetchPreferences(inifile);

			if (inifile.hasEntry("MOLECULARCONTROL", "ShowSS"))
			{
				// following function will negate the value:
				show_ss_ = !inifile.getValue("MOLECULARCONTROL", "ShowSS").toBool();
				switchShowSecondaryStructure();
			}

			if (inifile.hasEntry("MOLECULARCONTROL", "RegularExpressions"))
			{
				vector<String> fields;
				Size size = inifile.getValue("MOLECULARCONTROL", "RegularExpressions").split(fields, "|");
				for (Position p = 0; p < size; p++)
				{
					selector_edit_->insertItem(fields[p].c_str());
				}
			}

			connect(selector_edit_->lineEdit(), SIGNAL(returnPressed()), this, SLOT(applySelector()));
		}

		void MolecularControl::writePreferences(INIFile& inifile)
			throw()
		{
			inifile.appendSection("MOLECULARCONTROL");
			inifile.insertValue("MOLECULARCONTROL", "ShowSS", show_ss_);

			HashSet<String> set;

			for (Position p = 0; p < (Position)selector_edit_->count(); p++)
			{
				set.insert(selector_edit_->text(p).ascii());
			}

			String regexps;
			
			HashSet<String>::ConstIterator it = set.begin();
			for (; +it; ++it)
			{
				regexps += *it;
				regexps += "|";
			}

			if (regexps.size() != 0)
			{
				inifile.insertValue("MOLECULARCONTROL", "RegularExpressions", regexps);
			}

			DockWidget::writePreferences(inifile);
		}


	} // namespace VIEW
} // namespace BALL
