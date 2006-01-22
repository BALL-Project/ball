// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: molecularControl.C,v 1.99.2.5 2006/01/22 13:19:51 amoll Exp $
//

#include <BALL/VIEW/WIDGETS/molecularControl.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/DIALOGS/compositeProperties.h>
#include <BALL/VIEW/DIALOGS/bondProperties.h>
#include <BALL/VIEW/DIALOGS/atomOverview.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/selector.h>
#include <qpushbutton.h> 
#include <qlineedit.h> 
#include <qmessagebox.h> 
#include <qtooltip.h> 
#include <QGridLayout> 

namespace BALL
{
	namespace VIEW
	{

		MolecularControl::MolecularControl(const MolecularControl& mc)
			throw()
			: GenericControl(mc)
		{
		}

					

		MolecularControl::MolecularControl(QWidget* parent, const char* name)
			throw()
				:	GenericControl(parent, name),
					selected_(),
					information_(),
					selector_edit_(0),
					context_menu_(this),
					model_menu_(this),
					edit_menu_(this),
					context_composite_(0),
					was_delete_(false),
					nr_items_removed_(0),
					show_ss_(false),
					ignore_checked_changes_(false)
		{
		#ifdef BALL_VIEW_DEBUG
			Log.error() << "new MolecularControl " << this << std::endl;
		#endif

 			QGridLayout* lay = new QGridLayout();
			((QGridLayout*)layout())->addLayout(lay, 2, 0);

			listview->setObjectName("MolecularControlList");
			listview->headerItem()->setText(0, "Name [highlight]");
			listview->headerItem()->setText(1, "[selected] Type");
			listview->headerItem()->setSizeHint(0, QSize(20, 120));
			listview->headerItem()->setSizeHint(1, QSize(20, 60));
 			lay->addWidget(listview,0, 0, 1, -1);

			selector_edit_ = new QComboBox(this);
			selector_edit_->setAutoCompletion(true);
			selector_edit_->setDuplicatesEnabled(false);
			selector_edit_->setEditable(true);
			selector_edit_->resize(100, 25);
			selector_edit_->setMinimumSize(50,25);
			lay->addWidget(selector_edit_,1, 0, 1, -1);

			QPushButton* clear_button = new QPushButton(this);
			clear_button->resize(60, 25);
			clear_button->setMinimumSize(40, 25);
			clear_button->setText("Clear");
			connect(clear_button, SIGNAL(clicked()), this, SLOT(clearSelector()));
 			clear_button->setToolTip("Clear the selection.");
			lay->addWidget(clear_button,2, 0);

			QPushButton* help_button = new QPushButton(this);
			help_button->resize(60, 25);
			help_button->setMinimumSize(40, 25);
			help_button->setText("Help");
			connect(help_button, SIGNAL(clicked()), this, SLOT(showSelectorHelp()));
			help_button->setToolTip("Show a help dialog.");
			lay->addWidget(help_button,2, 1);

			QPushButton* select_button = new QPushButton(this);
			select_button->resize(60, 25);
			select_button->setText("Select");
			select_button->setDefault(true);
			select_button->setMinimumSize(40, 25);
			connect(select_button, SIGNAL(clicked()), this, SLOT(applySelector()));
			select_button->setToolTip("Apply the current expression.");
			lay->addWidget(select_button,2, 2);

			buildContextMenu_();
			resize(200,400);

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
			paste_id_->setEnabled(allow_paste);	
			getMainControl()->setMenuHint(paste_id_, hint);

			// ------------------------------------------------------------------
			// clearClipboard
			// enable only if copy_list_ not empty
			bool copy_list_filled = (copy_list_.size() > 0);
			clipboard_id_->setEnabled(copy_list_filled && !main_control.compositesAreLocked());
			if (clipboard_id_->isEnabled())
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
			
			cut_id_->setEnabled(list_filled);
			getMainControl()->setMenuHint(cut_id_, hint);
			copy_id_->setEnabled(list_filled);	
			getMainControl()->setMenuHint(copy_id_, hint);

			select_id_->setEnabled(list_filled);
			getMainControl()->setMenuHint(select_id_, hint);
			deselect_id_->setEnabled(list_filled);	
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
					{
						addComposite(*(Composite *)composite_message->getComposite(), 
																			 composite_message->getCompositeName());
						return false;
					}
					
					case CompositeMessage::REMOVED_COMPOSITE:
					{
						removeComposite(*(Composite *)composite_message->getComposite());
						return false;
					}
					
					case CompositeMessage::CHANGED_COMPOSITE:
						return false;

					case CompositeMessage::CHANGED_COMPOSITE_HIERARCHY:
					{
						List<Composite*> open_items;
						HashMap<QTreeWidgetItem*, Composite*>::Iterator it = item_to_composite_.begin();
						for (; +it; ++it)
						{
							if (listview->isItemExpanded((*it).first))
							{
								open_items.push_back((*it).second);
							}
						}

						removeComposite(composite_message->getComposite()->getRoot());
						addComposite(composite_message->getComposite()->getRoot());

						List<Composite*>::Iterator lit = open_items.begin();
						HashMap<Composite*, QTreeWidgetItem*>::Iterator to_find;
						for (; lit != open_items.end(); lit++)
						{
							to_find = composite_to_item_.find(*lit);
							if (to_find == composite_to_item_.end()) continue;

							listview->expandItem(to_find->second);
						}

						return true;
					}
					case CompositeMessage::SELECTED_COMPOSITE:
					case CompositeMessage::DESELECTED_COMPOSITE:
					{
 						setSelection_(false);
						return true;
					}
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
				highlight(nsm->getSelection());
				return true;
			}

			return false;
		}

		void MolecularControl::activatedItem_(QAction* action)
		{
			String a = ascii(action->objectName());
			try
			{
				Index pos = a.toUnsignedInt();
				if (pos < 1000)
				{
					selected_model_ = (ModelType)(pos);
				}
				else if (pos >= 1000 && pos < 1100)
				{
					selected_coloring_method_ = (ColoringMethod)(pos - 1000);
				}
			}
			catch(...)
			{
			}
		}

		void MolecularControl::buildContextMenu_()
		{

			// ===============================================================
			// create representation context menu:
			// ===============================================================

			model_menu_.addAction("Custom", this, SLOT(createRepresentation())); 

			QAction* action =	0;
			Position p = 0;
			for (Position pos = MODEL_LINES; pos < MODEL_LABEL; pos++)
			{
				action = model_menu_.addMenu(&color_menu_[p]);
				action->setText(getModelName((ModelType)pos).c_str());
				action->setObjectName(String(pos).c_str());
				p++;
			}

			// create a new coloring popupmenu for every model entry
			// otherwise we get into trouble with qt
			for (Position i = 0; i< p; i++)
			{
				for (Position pos = COLORING_ELEMENT; pos < COLORING_UNKNOWN; pos++)
				{
					action = color_menu_[i].addAction(getColoringName((ColoringMethod)pos).c_str(), this, 
																								 SLOT(createRepresentation_()));
					action->setObjectName(String(pos + 1000).c_str());
					connect(&color_menu_[i], SIGNAL(hovered(QAction*)), this, SLOT(activatedItem_(QAction*)));
				}
			}

			connect(&model_menu_, SIGNAL(hovered(QAction*)), this, SLOT(activatedItem_(QAction*)));

			// ===============================================================
			// main context menu:
			// ===============================================================

			QAction* cr = context_menu_.addMenu(&model_menu_);
			cr->setText("Create Representation");
			context_menu_.addSeparator();
			cr = context_menu_.addMenu(&edit_menu_);
			cr->setText("Edit");
			context_menu_.addSeparator();

			select_action_ = context_menu_.addAction("Select", this, SLOT(select()));
			deselect_action_ = context_menu_.addAction("Deselect", this, SLOT(deselect()));

			context_menu_.addSeparator();
			center_camera_action_ = context_menu_.addAction("Focus", this, SLOT(centerCamera()));
			context_menu_.addSeparator();

			// -----------------------------------> AtomContainer
			count_items_action_ = context_menu_.addAction("Count items", this, SLOT(countItems()), 0);
			atom_overview_ = context_menu_.addAction("Atom Overview", this, SLOT(showAtomOverview()), 0);
			// <----------------------------------- AtomContainer
			
			context_menu_.addSeparator();
			composite_properties_action_ = context_menu_.addAction("Properties", this, SLOT(compositeProperties()));

			// -----------------------------------> Atoms
			bond_propertes_action_ = context_menu_.addAction("Show Bonds", this, SLOT(bondProperties()));
			// <----------------------------------- Atoms

			context_menu_.addSeparator();
			context_menu_.addAction("Collapse all", this, SLOT(collapseAll()));
			context_menu_.addAction("Expand all", this, SLOT(expandAll()));
			context_menu_.addAction("Highlight Selection", this, SLOT(highlightSelection()));
			context_menu_.addSeparator();
			show_ss_id_ = context_menu_.addAction("Show Secondary Structures", this, 
																						SLOT(switchShowSecondaryStructure()));

			// ===============================================================
			// edit context menu:
			// ===============================================================

			edit_menu_.addAction("Cut", this, SLOT(cut()));
			edit_menu_.addAction("Copy", this, SLOT(copy()));
			edit_menu_.addAction("Paste", this, SLOT(paste()));
			edit_menu_.addAction("Delete", this, SLOT(deleteCurrentItems()));
			edit_menu_.addSeparator();
			edit_menu_.addAction("Move", this, SLOT(moveItems()));
		}

		void MolecularControl::updateContextMenu(Composite& composite)
			throw()
		{
			bool composites_muteable = !getMainControl()->compositesAreLocked();

			bool one_item = (getSelection().size() == 1);

			select_action_->setEnabled(!composite.isSelected() && composites_muteable);
			deselect_action_->setEnabled(composite.isSelected() && composites_muteable);

			// -----------------------------------> AtomContainer
			bool ac = RTTI::isKindOf<AtomContainer>(composite);
			count_items_action_->setEnabled(ac);
			atom_overview_->setEnabled(ac);
			// <----------------------------------- AtomContainer
			
			// -----------------------------------> Atoms
			bond_propertes_action_->setEnabled(RTTI::isKindOf<Atom>(composite) && one_item && composites_muteable);
			// <----------------------------------- Atoms

			composite_properties_action_->setEnabled(one_item);
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
			notify_(new MolecularTaskMessage(MolecularTaskMessage::BUILD_BONDS));
		}
			
		void MolecularControl::centerCamera()
		{
			notify_(new CompositeMessage(*context_composite_, CompositeMessage::CENTER_CAMERA));
		}

		void MolecularControl::checkResidue()
		{
			notify_(new MolecularTaskMessage(MolecularTaskMessage::CHECK_RESIDUE));
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

			QList<QTreeWidgetItem *> items = getSelectedItems();
			QList<QTreeWidgetItem *>::iterator it = items.begin();
			for (; it != items.end(); it++)
			{
				QTreeWidgetItem* item = *it;
				QTreeWidgetItem* parent = item->parent();
				// we have to prevent inserting childs of already selected parents, 
				// otherwise we get serious trouble
				if (parent != 0 && listview->isItemSelected(parent))
				{
					continue;
				}

				selected_.push_back(item_to_composite_[item]);
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

		void MolecularControl::showGuestContextMenu(const QPoint& pos)
		{
			if (getMainControl()->compositesAreLocked() ||
					getMainControl()->getPrimitiveManager().updateRunning()) 
			{
				setStatusbarText("No changes allowed, while simulation is running or creating new representations!", true);
				return;
			}

			QList<QTreeWidgetItem *> items = listview->selectedItems();
			if (items.size() == 0) return;

			QTreeWidgetItem* item = *items.begin();
			Composite* composite = item_to_composite_[item];

			// create the context menu
			if (composite != 0)
			{
				context_composite_ = composite;
				context_item_ = item;
				updateContextMenu(*composite);
			}

			// show the context menu if it is not empty
			context_menu_.popup(mapToGlobal(pos));
		}


		void MolecularControl::selectedComposite_(Composite* composite, bool state)
		{
			if (composite->isSelected() == state) return;
			
			CompositeMessage::CompositeMessageType id = CompositeMessage::DESELECTED_COMPOSITE;

			if (state) id = CompositeMessage::SELECTED_COMPOSITE;

			notify_(new CompositeMessage(*composite, id));
		}


		void MolecularControl::recurseGeneration_(QTreeWidgetItem* item, Composite& composite)
			throw()
		{
			Composite::ChildCompositeReverseIterator it = composite.rbeginChildComposite();
			for (; it != composite.rendChildComposite(); ++it)
			{
				generateListViewItem_(item, *it);
			}
		}

		void MolecularControl::initializeWidget(MainControl& main_control)
			throw()
		{
			String hint;

			select_id_ = insertMenuEntry(MainControl::EDIT, "&Select", this, SLOT(select()));   
			setMenuHint("Select a molecular object to see its position in the scene or to mark it for a simulation");
			setMenuHelp("molecularControl.html#selection_highlight");
			deselect_id_ = insertMenuEntry(MainControl::EDIT, "&Deselect", this, SLOT(deselect()));
			setMenuHint("Deselect a molecular object.");
			setMenuHelp("molecularControl.html#selection_highlight");

			main_control.insertPopupMenuSeparator(MainControl::EDIT);

			cut_id_ = main_control.insertMenuEntry(MainControl::EDIT, "Cu&t", this, SLOT(cut()), 
																						 Qt::CTRL+Qt::Key_X);
			copy_id_ = main_control.insertMenuEntry(MainControl::EDIT, "&Copy", this, SLOT(copy()), 
																							Qt::CTRL+Qt::Key_C);
			paste_id_ = main_control.insertMenuEntry(MainControl::EDIT, "&Paste", this, SLOT(paste()), 
																								Qt::CTRL+Qt::Key_V);

			main_control.insertDeleteEntry();
			main_control.insertPopupMenuSeparator(MainControl::EDIT);

			clipboard_id_ = main_control.insertMenuEntry(MainControl::EDIT, "Clear Clipboard", this, 
																									 SLOT(clearClipboard()));
			setMenuHint("Clear the items in the clipboard");

			GenericControl::initializeWidget(main_control);

			registerForHelpSystem(this, "molecularControl.html");
			registerForHelpSystem(selector_edit_, "molecularControl.html#regular_expressions"); 
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
			ignore_checked_changes_ = true;
			generateListViewItem_(0, composite, &name);
			ignore_checked_changes_ = false;

			// update the view
			updateSelection();
		}


		Size MolecularControl::removeComposite(Composite& composite)
			throw()
		{
			HashMap<Composite*, QTreeWidgetItem*>::Iterator to_find = 
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

		void MolecularControl::removeRecursive_(QTreeWidgetItem* item)
			throw()
		{
			composite_to_item_.erase(item_to_composite_[item]);
			item_to_composite_.erase(item);

			QTreeWidgetItem* child = item->child(0);
			while (child != 0)
			{
				removeRecursive_(child);
				child = item->child(0);
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

			GenericControl::onNotify(message);

			listview->setUpdatesEnabled(false);

			// react accordingly to the given message
			if (reactToMessages_(message))
			{
				// if message makes an update of the control necessary
				// sends new selection through tree
				updateSelection();
			}
			listview->setUpdatesEnabled(true);
		}


		void MolecularControl::invalidateSelection()
		{
			//// weg? ??????????????????????
			QList<QTreeWidgetItem *> items = listview->selectedItems();
			QList<QTreeWidgetItem *>::iterator it = items.begin();

			for (; it != items.end(); it++);
			{
				listview->setItemSelected(*it, false);
			}

			listview->setUpdatesEnabled(true);
		}

		// set the highlighting according to the selection in the MainControl
		void MolecularControl::highlight(const List<Composite*>& selection)
			throw()
		{	
			listview->setUpdatesEnabled(false);
			listview->clearSelection();

			List<Composite*>::ConstIterator cit = selection.begin();
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
				listview->setItemSelected(composite_to_item_[*cit], true);
			}

			listview->setUpdatesEnabled(true);
			updateSelection();
		}

		// set the checkboxes according to the selection in the MainControl
		void MolecularControl::setSelection_(bool open, bool force)
			throw()
		{	
			if (!force) open = false; // ????????

			ignore_checked_changes_ = true;
			HashMap<Composite*, QTreeWidgetItem*>::Iterator cit = composite_to_item_.begin();
			for (; +cit; ++cit)
			{
				QTreeWidgetItem* const item = (*cit).second;
 				item->setCheckState(1, Qt::Unchecked);
			}

			const HashSet<Composite*>& selection = getMainControl()->getSelection();
			HashSet<Composite*>::ConstIterator sit = selection.begin();
			for (; +sit; ++sit)
			{
				composite_to_item_[*sit]->setCheckState(1, Qt::Checked);
				Composite::ChildCompositeIterator cit = (**sit).beginChildComposite();
				for (; cit != (**sit).endChildComposite(); ++cit)
				{
					composite_to_item_[&*cit]->setCheckState(1, Qt::Checked);
				}
			}

			setStatusbarText(String(getMainControl()->getSelection().size()) + " objects selected.");
			ignore_checked_changes_ = false;
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

			notify_(new ControlSelectionMessage);

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

			notify_(new SceneMessage(SceneMessage::ENTER_MOVE_MODE));
		}


		QTreeWidgetItem* MolecularControl::generateListViewItem_(QTreeWidgetItem* parent, 
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

			QStringList sl;
			sl << name << type;

			// create a new list item
			QTreeWidgetItem* new_item = 0;

			// is this a root item?
			if (parent == 0)
			{
				new_item = new QTreeWidgetItem(listview, sl);
			} 
			else 
			{
				// no, insert into the current item
				new_item = new QTreeWidgetItem(parent, sl);
			}

			new_item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
			
			// force QT to show checkboxes!
			new_item->setCheckState(1, Qt::Checked);
			if (!composite.isSelected()) new_item->setCheckState(1, Qt::Unchecked);

			composite_to_item_[&composite] = new_item;
			item_to_composite_[new_item] = &composite;
			recurseGeneration_(new_item, composite);

			return new_item;
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

			notify_(new ShowDisplayPropertiesMessage);
		}

		void MolecularControl::collapseAll()
		{
			for (Position p = 0; true; p++)
			{
				QTreeWidgetItem* item = listview->topLevelItem(p);
				if (item == 0) return;
				listview->collapseItem(item);
			}
		}

		void MolecularControl::expandAll()
		{
			HashMap<QTreeWidgetItem*, Composite*>::Iterator it = item_to_composite_.begin();
			for (; +it; ++it)
			{
				listview->expandItem((*it).first);
			}
		}

		void MolecularControl::createRepresentation_()
		{
			notify_(new CreateRepresentationMessage(selected_, selected_model_, selected_coloring_method_));
		}

		void MolecularControl::countItems()
		{
			if (context_item_ == 0) return;

			AtomContainer& ac = *(AtomContainer*) context_composite_;

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
				s.setExpression(Expression(ascii(selector_edit_->currentText())));
			}
			catch(Exception::ParseError e)
			{
				setStatusbarText(String("Invalid expression ") + e.getMessage(), true);
				return 0;
			}

			HashSet<Composite*> roots;
			Size nr_of_atoms = 0;

			CompositeManager::CompositeIterator it = getMainControl()->getCompositeManager().begin();
			for(; it != getMainControl()->getCompositeManager().end(); it++)
			{
				try
				{
					(*it)->apply(s);
				}
				catch(Exception::GeneralException e)
				{
					setStatusbarText(String("Invalid expression ") + e.getMessage(), true);
				}

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
			selector_edit_->addItem("");
			Index x = selector_edit_->findText("");
			selector_edit_->setCurrentIndex(x);
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
			selector_edit_->addItem(expression.c_str());
			Index x = selector_edit_->findText(expression.c_str());
			selector_edit_->setCurrentIndex(x);
			return applySelector();
		}

		void MolecularControl::highlightSelection()
			throw()
		{
			listview->setUpdatesEnabled(false);

			HashMap<QTreeWidgetItem*, Composite*>::Iterator it = item_to_composite_.begin();
			for (; +it; ++it)
			{
				QTreeWidgetItem* item = (*it).first;

				if (item->checkState(1) == Qt::Checked)
				{
					listview->expandItem(item);
					listview->setItemSelected(item, true);
					QTreeWidgetItem* parent = item->parent();
					while (parent != 0 && !listview->isItemExpanded(parent))
					{
						listview->expandItem(parent);
						parent = parent->parent();
					}
				}
				else
				{
					listview->collapseItem(item);
					listview->setItemSelected(item, false);
				}
					
			}
			listview->setUpdatesEnabled(true);
		}

		void MolecularControl::switchShowSecondaryStructure()
		{
			if (show_ss_) show_ss_ = false;
			else 					show_ss_ = true;

			show_ss_id_->setChecked(show_ss_);

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
					selector_edit_->addItem(fields[p].c_str());
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
				set.insert(ascii(selector_edit_->itemText(p)));
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

		void MolecularControl::onItemClicked(QTreeWidgetItem* item)
		{
			if (ignore_checked_changes_) return;
			bool checked = (item->checkState(1) == Qt::Checked);

			if (getMainControl()->compositesAreLocked())
			{
				if (checked) item->setCheckState(1, Qt::Unchecked);
				else 				 item->setCheckState(1, Qt::Checked);

				VIEW::getMainControl()->setStatusbarText("Cannot select items now!", true);
				return;
			}

			Composite* c = item_to_composite_[item];
			if (checked == c->isSelected()) return;

 			selectedComposite_(c, checked);
		}

		void MolecularControl::showAtomOverview()
		{
			AtomOverview ao;
			ao.setParent(dynamic_cast<AtomContainer*>(item_to_composite_[context_item_]));
			ao.exec();
		}

	} // namespace VIEW
} // namespace BALL
