/* TRANSLATOR BALL::VIEW::MolecularControl

		Necessary for lupdate.
*/

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/WIDGETS/molecularControl.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/representation.h>
#include <BALL/VIEW/PRIMITIVES/tube.h>
#include <BALL/VIEW/PRIMITIVES/sphere.h>
#include <BALL/VIEW/PRIMITIVES/label.h>
#include <BALL/VIEW/DIALOGS/compositeProperties.h>
#include <BALL/VIEW/DIALOGS/bondProperties.h>
#include <BALL/VIEW/DIALOGS/atomOverview.h>
#include <BALL/MATHS/analyticalGeometry.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/selector.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/STRUCTURE/residueRotamerSet.h>
#include <BALL/STRUCTURE/rotamerLibrary.h>
#include <BALL/STRUCTURE/peptides.h>
#include <BALL/STRUCTURE/disulfidBondProcessor.h>

#include <QtWidgets/QComboBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QToolTip>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QCompleter>

#include <QtCore/QPoint>
#include <QtCore/QThread>
#include <QtCore/QSignalMapper>

#include <set>

namespace BALL
{
	namespace VIEW
	{

		// This class loads an instance of the RotamerLibrary into a supplied memory location.
		class RotamerLibraryLoader : public QThread
		{
			public:
				RotamerLibraryLoader(QObject* parent, RotamerLibrary** rotamer_library, ReadWriteLock* rotamer_lock)
					: QThread(parent),
					  rotamer_library_(rotamer_library),
					  rotamer_lock_(rotamer_lock)
				{
				}

				void run() {
					rotamer_lock_->lockForWrite();
					*rotamer_library_ = new RotamerLibrary;
					rotamer_lock_->unlock();
				}

			private:
				RotamerLibrary** rotamer_library_;
				ReadWriteLock*   rotamer_lock_;
		};

		MolecularControl::MyTreeWidgetItem::MyTreeWidgetItem(QTreeWidget* parent,
																												 QStringList& sl, 
																												 Composite* c)
			: QTreeWidgetItem(parent, sl),
				composite(c)
		{
			init_();
		}

		MolecularControl::MyTreeWidgetItem::MyTreeWidgetItem(QTreeWidgetItem* parent, 
																												 QStringList& sl, 
																												 Composite* c)
			: QTreeWidgetItem(parent, sl),
				composite(c)
		{
			init_();
		}

		void MolecularControl::MyTreeWidgetItem::init_()
		{
			setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
			
			// force QT to show checkboxes!
			setCheckState(2, Qt::Checked);
			if (!composite->isSelected()) setCheckState(2, Qt::Unchecked);
		}



		MolecularControl::MolecularControl(const MolecularControl& mc)
			: GenericControl(mc)
		{
			rotamer_library_mutex_.lockForWrite();
			rotamer_library_ = new RotamerLibrary(*mc.rotamer_library_);
			rotamer_library_mutex_.unlock();
		}


		MolecularControl::MolecularControl(QWidget* parent, const char* name)
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
					ignore_messages_(false),
					rotamer_library_(0)
		{
		#ifdef BALL_VIEW_DEBUG
			Log.error() << "new MolecularControl " << this << std::endl;
		#endif

			QGridLayout* glay = getGuestLayout();
			QGridLayout* lay = new QGridLayout();
			glay->addLayout(lay, 2, 0);

			listview->setObjectName("MolecularControlList");
			listview->headerItem()->setText(0, tr("Name") + " ["+ tr("highlight") + "]");
			listview->headerItem()->setText(1, tr("Type"));
			listview->headerItem()->setText(2, tr("checked"));
			listview->resizeColumnToContents(2);
			listview->resizeColumnToContents(0);
			lay->addWidget(listview,0, 0, 1, -1);

			selector_edit_ = new QComboBox(this);
			BALL_ASSIGN_NAME(selector_edit_)
			selector_edit_->setLineEdit(new QLineEdit());
			selector_edit_->lineEdit()->setObjectName("lineedit");

			// set up suitable completion for the selector widget
			QCompleter* completer = selector_edit_->completer();
			completer->setCaseSensitivity(Qt::CaseSensitive);
			completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);

			selector_edit_->setDuplicatesEnabled(false);
			selector_edit_->setEditable(true);
			selector_edit_->resize(100, 25);
			selector_edit_->setMinimumSize(50,25);
			lay->addWidget(selector_edit_,2, 0, 1, -1);

			QPushButton* clear_button = new QPushButton(this);
			BALL_ASSIGN_NAME(clear_button)
			clear_button->resize(60, 25);
			clear_button->setMinimumSize(40, 25);
			clear_button->setText(tr("Clear"));
			connect(clear_button, SIGNAL(clicked()), this, SLOT(clearSelector()));
			clear_button->setToolTip(tr("Clear the selection."));
			lay->addWidget(clear_button,3, 0);

			QPushButton* select_button = new QPushButton(this);
			BALL_ASSIGN_NAME(select_button)
			select_button->resize(60, 25);
			select_button->setText(tr("Select"));
			select_button->setDefault(true);
			select_button->setMinimumSize(40, 25);
			connect(select_button, SIGNAL(clicked()), this, SLOT(applySelector()));
			connect(selector_edit_->lineEdit(), SIGNAL(returnPressed()), this, SLOT(applySelector()));
			select_button->setToolTip(tr("Apply the current expression."));
			lay->addWidget(select_button,3, 1);

			resize(200,400);

			registerWidget(this);

			buildContextMenu_();

			// Load the rotamer library. This is done in a different thread as parsing the
			// files is very slow.
			rl_thread_ = new RotamerLibraryLoader(this, &rotamer_library_, &rotamer_library_mutex_);
			rl_thread_->start();
		}

		MolecularControl::~MolecularControl()
		{
		#ifdef BALL_VIEW_DEBUG
			Log.error() << "Destroying MolecularControl " << this << std::endl;
		#endif

			clearClipboard();

			rotamer_library_mutex_.lockForWrite();
			delete rotamer_library_;
			rotamer_library_mutex_.unlock();
		}

		void MolecularControl::checkMenu(MainControl& main_control)
		{
			String hint;
			bool busy = main_control.isBusy();

			// prevent changes to composites while simulations are running
			
			// check for paste-slot: enable only if one selected item &&
			// 																			copy_list_ not empty && 
			// 																			no simulation running &&
			// 																			it makes sense
			bool allow_paste = getSelection().size() <= 1 && copy_list_.size() > 0 && !busy;

			if (allow_paste)
			{
				hint = (String)tr("Paste a copied or cuted object into current selected object.");
				allow_paste = allowPaste_();
				hint = (String)tr("Invalid Combination, cant paste into this entity.");
			}
			else
			{
				if (getSelection().size() != 1)         hint = (String)tr("One item must be selected to paste into.");
				else if (copy_list_.size() == 0)        hint = (String)tr("No copied/cuted object.");
				else if (main_control.isBusy())         hint = (String)tr("Simulation running, cant paste meanwhile");
				else                                    hint = (String)tr("Update of Representation running, cant paste meanwhile");
			}
			
			if (paste_id_)
			{
				paste_id_->setEnabled(allow_paste);	
				getMainControl()->setMenuHint(paste_id_, hint);
			}

			// ------------------------------------------------------------------
			// clearClipboard
			// enable only if copy_list_ not empty
			bool copy_list_filled = (copy_list_.size() > 0);
			if (clipboard_id_)
			{
				clipboard_id_->setEnabled(copy_list_filled && !main_control.isBusy());
				if (clipboard_id_->isEnabled())
				{
					hint = (String)tr("No item copied/cuted or simulation running");
				} 
				else
				{
					hint = (String)tr("Clear the items in the clipboard");
				}
				getMainControl()->setMenuHint(clipboard_id_, hint);
			}

			// ------------------------------------------------------------------
			// cut / delete  +  select / deselect
			bool list_filled = selected_.size() && !busy;
			
			if (list_filled) hint = "";
			else hint = (String)tr("No item selected or simulation running");
			
			if (cut_id_)
			{
				cut_id_->setEnabled(list_filled);
				getMainControl()->setMenuHint(cut_id_, hint);
			}

			if (copy_id_)
			{
				copy_id_->setEnabled(list_filled);	
				getMainControl()->setMenuHint(copy_id_, hint);
			}

			if (select_id_)
			{
				select_id_->setEnabled(list_filled);
				getMainControl()->setMenuHint(select_id_, hint);
			}

			if (deselect_id_)
			{
				deselect_id_->setEnabled(list_filled);	
				getMainControl()->setMenuHint(deselect_id_, hint);
			}

			if (angle_action_)
				angle_action_->setEnabled(list_filled && selected_.size() == 3);	
			if (distance_action_)
				distance_action_->setEnabled(list_filled && selected_.size() == 2);	

			// enable global delete entry for all GenericControls, if this Control has the selection
			if (selected_.size() > 0)
			{
				getMainControl()->setDeleteEntryEnabled(!busy);
			}
		}


		bool MolecularControl::reactToMessages_(Message* message)
		{
			// react only to NewMolecularMessage, but not to NewCompositeMessage
            if (RTTI::isKindOf<CompositeMessage>(message))
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
						return true;
					}
					
					case CompositeMessage::CHANGED_COMPOSITE:
						return false;

					case CompositeMessage::CHANGED_COMPOSITE_HIERARCHY:
					{
						Composite* composite_ptr;
						vector<Composite*> roots;
						
						bool was_enabled = !ignore_messages_;
						selected_.clear();
						enableUpdates_(false);
						HashSet<Composite*> open_items;
						HashSet<Composite*> highlighted;
						QTreeWidgetItemIterator qit(listview);
						while (*qit != 0)
						{
							composite_ptr = (*(MyTreeWidgetItem*)*qit).composite;
							
							if (listview->isItemExpanded(*qit))
							{
								open_items.insert(composite_ptr);
							}

							if (listview->isItemSelected(*qit))
							{
								highlighted.insert(composite_ptr);
							}
							
				
							if ((*qit)->parent() == NULL)
							{
								roots.push_back(composite_ptr);
							}

							qit++;
						}

						for (Position i=0; i!=roots.size(); ++i)
						{
							removeComposite(*roots[i]);
							addComposite(*roots[i]);
						}

						list<QTreeWidgetItem*> item_list;
						qit = QTreeWidgetItemIterator(listview);
						while (*qit != 0)
						{
							Composite* com = ((MyTreeWidgetItem*) *qit)->composite;
							if (open_items.has(com))
							{
								listview->expandItem(*qit);
							}

							if (highlighted.has(com))
							{
								item_list.push_back(*qit);
							}

							qit++;
						}

						listview->selectItems(item_list);

						if (was_enabled) enableUpdates_(true);
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
            else if (RTTI::isKindOf<NewSelectionMessage>(message))
			{
				NewSelectionMessage* nsm = (NewSelectionMessage*) message;
				setSelection_(true, nsm->openItems());
				return true;
			}

            if (RTTI::isKindOf<ControlSelectionMessage>(message))
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
			context_menu_.setObjectName("ContextMenu");
			model_menu_.setObjectName("ModelMenu");

			model_menu_.addAction(tr("Custom"), this, SLOT(createRepresentation())); 

			const ModelInformation& mi = getMainControl()->getModelInformation();
			QAction* action = 0;
			Position p = 0;
			for (Position pos = MODEL_LINES; pos < MODEL_LABEL; pos++)
			{
				if (!mi.modelMuteableByDisplayProperties((ModelType)pos)) break;
				action = model_menu_.addMenu(&color_menu_[p]);
				color_menu_[p].setObjectName((String("ColorMenu") + String(p)).c_str());
				action->setText(mi.getModelName((ModelType)pos).c_str());
				action->setObjectName(String(pos).c_str());
				p++;
			}

			// create a new coloring popupmenu for every model entry
			// otherwise we get into trouble with qt
			for (Position i = 0; i< p; i++)
			{
				for (Position pos = COLORING_ELEMENT; pos < COLORING_UNKNOWN; pos++)
				{
					if (!mi.coloringMuteableByDisplayProperties((ColoringMethod)pos)) break;
					action = color_menu_[i].addAction(mi.getColoringName((ColoringMethod)pos).c_str(), this, 
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
			cr->setText(tr("Create Representation"));
			context_menu_.addSeparator();
			cr = context_menu_.addMenu(&edit_menu_);
			cr->setText(tr("Edit"));
			context_menu_.addSeparator();

			select_action_ = context_menu_.addAction(tr("Select"), this, SLOT(select()));
			deselect_action_ = context_menu_.addAction(tr("Deselect"), this, SLOT(deselect()));

			context_menu_.addSeparator();
			center_camera_action_ = context_menu_.addAction(tr("Focus"), this, SLOT(centerCamera()));
			context_menu_.addSeparator();

			// -----------------------------------> AtomContainer
			count_items_action_ = context_menu_.addAction(tr("Count items"), this, SLOT(countItems()), 0);
			atom_overview_ = context_menu_.addAction(tr("Atom Overview"), this, SLOT(showAtomOverview()), 0);
			atom_overview_selection_ = context_menu_.addAction(tr("Atom Overview for Selection"), this,
																												SLOT(showAtomOverviewForSelection()), 0);
			// <----------------------------------- AtomContainer

			context_menu_.addSeparator();

			rotamer_mapper_ = new QSignalMapper(this);
			connect(rotamer_mapper_, SIGNAL(mapped(int)), this, SLOT(changeRotamer_(int)));
			rotamer_menu_ = new QMenu(tr("Apply Rotamer"), this);
			current_residue_ = 0;
			context_menu_.addMenu(rotamer_menu_);

			disulfidbond_action_ = context_menu_.addAction(tr("Create Disulfidbond"), this, SLOT(toggleDisulfidBond()));

			composite_properties_action_ = context_menu_.addAction(tr("Properties"), this, SLOT(compositeProperties()));

			// -----------------------------------> Atoms
			bond_propertes_action_ = context_menu_.addAction(tr("Show Bonds"), this, SLOT(bondProperties()));
			// <----------------------------------- Atoms

			context_menu_.addSeparator();
			context_menu_.addAction(tr("Collapse all"), this, SLOT(collapseAll()));
			context_menu_.addAction(tr("Expand all"), this, SLOT(expandAll()));
			context_menu_.addAction(tr("Highlight Selection"), this, SLOT(highlightSelection()));
			context_menu_.addSeparator();
			show_ss_id_ = context_menu_.addAction(tr("Show Secondary Structures"), this,
																						SLOT(switchShowSecondaryStructure()));
			show_ss_id_->setCheckable(true);

			// ===============================================================
			// edit context menu:
			// ===============================================================

			edit_menu_.addAction(tr("Cut"), this, SLOT(cut()));
			edit_menu_.addAction(tr("Copy"), this, SLOT(copy()));
			paste_action_ = edit_menu_.addAction(tr("Paste"), this, SLOT(paste()));
			edit_menu_.addAction(tr("Delete"), this, SLOT(deleteCurrentItems()));
			edit_menu_.addSeparator();
			edit_menu_.addAction(tr("Move"), this, SLOT(moveItems()));
		}

		void MolecularControl::updateContextMenu(Composite& composite)
		{
			bool composites_muteable = !getMainControl()->isBusy();

			bool one_item = (getSelection().size() == 1);

			select_action_->setEnabled(!composite.isSelected() && composites_muteable);
			deselect_action_->setEnabled(composite.isSelected() && composites_muteable);

			bool allow_paste = allowPaste_();
			paste_action_->setEnabled(allow_paste && composites_muteable);

			// -----------------------------------> AtomContainer
            bool ac = RTTI::isKindOf<AtomContainer>(&composite);
			count_items_action_->setEnabled(ac);
			atom_overview_->setEnabled(ac);
			atom_overview_selection_->setEnabled(ac);
			// <----------------------------------- AtomContainer

			// -----------------------------------> Atoms
            bond_propertes_action_->setEnabled(RTTI::isKindOf<Atom>(&composite) && one_item && composites_muteable);
			// <----------------------------------- Atoms

			composite_properties_action_->setEnabled(one_item);

			// -----------------------------------> Residues
			bool two_cysteins = false;
			Size num_cysteins = 0;
			bool two_sulfurs  = false;
			Size num_sulfurs  = 0;
			Atom *s1 = NULL;
			Atom *s2 = NULL;

			list<Composite*> composite_list = getMainControl()->getMolecularControlSelection();
			if (composite_list.size() == 2)
			{
				list<Composite*>::const_iterator it = composite_list.begin();
				for ( ; it!=composite_list.end(); ++it)
				{
                    if (RTTI::isKindOf<Residue>(*it))
					{
						Residue* res = reinterpret_cast<Residue*>(*it);
						if (Peptides::OneLetterCode(res->getName()) == 'C')
						{
							// find S 
							AtomIterator a_it = res->beginAtom();
							for (; +a_it; ++a_it)
							{
								if (a_it->getElement() == PTE[Element::S])
								{
									if (s1)
										s2 = &*a_it;
									else
										s1 = &*a_it;
								}
							}
							num_cysteins++;
						}
					}
                    else if (RTTI::isKindOf<Atom>(*it))
					{
						Atom* atom = reinterpret_cast<Atom*>(*it);
						if (atom->getElement() == PTE[Element::S])
						{
							num_sulfurs++;
							if (s1)
								s2 = atom;
							else
								s1 = atom;
						}
					}
				}
			}
			two_cysteins = (num_cysteins == 2);
			two_sulfurs  = (num_sulfurs == 2);

			bool has_bond = (s1->getBond(*s2) != NULL);

			disulfidbond_action_->setEnabled(   (two_cysteins || two_sulfurs)
			                                 && ( has_bond == s1->getResidue()->hasProperty(Residue::PROPERTY__HAS_SSBOND))
																			 && ( has_bond == s2->getResidue()->hasProperty(Residue::PROPERTY__HAS_SSBOND)));
			if (has_bond)
			{
				disulfidbond_action_->setText(tr("Disconnect Disulfidbond"));
			}
			else
			{
				disulfidbond_action_->setText(tr("Create Disulfidbond"));
			}

			// ------------------------------------ Residues

			// TODO: Deal with terminal residues. We will probably need to adapt
			//       RotamerLibrary for that.
			if((composite_list.size() != 1) || !composite.isResidue() || (current_residue_ = static_cast<Residue*>(&composite))->isTerminal())
			{
				rotamer_menu_->setEnabled(false);
				return;
			}

			buildRotamerMenu_();
		}

		void MolecularControl::buildRotamerMenu_()
		{
			rotamer_menu_->clear();

			// Make sure the rotamer library has been loaded
			rotamer_library_mutex_.lockForRead();
			ResidueRotamerSet* res_set = rotamer_library_->getRotamerSet(*current_residue_);
			rotamer_library_mutex_.unlock();

			// Check if the rotamer set is valid
			if(!res_set || res_set->getNumberOfRotamers() == 0)
			{
				rotamer_menu_->setEnabled(false);
				return;
			}

			// Helper variables for finding the closest rotamer
			unsigned int i_min = 0;
			float i_min_val = std::numeric_limits<float>::infinity();
			const Rotamer r = res_set->getRotamer(*current_residue_);

			unsigned int i = 0;
			for(ResidueRotamerSet::iterator it = res_set->begin(); it != res_set->end(); ++it, ++i)
			{
				//Create the action
				QAction* action = rotamer_menu_->addAction(QString(res_set->getName().c_str()) + " " + QString::number(i) + " (p: " + QString::number(it->P) + ")");
				action->setCheckable(true);

				// Connect the action triggered signal with the correct slot
				rotamer_mapper_->setMapping(action, i);
				connect(action, SIGNAL(triggered()), rotamer_mapper_, SLOT(map()));

				// Determine the closest residue
				float dist = fabs(r.chi1 - it->chi1) + fabs(r.chi2 - it->chi2)
				           + fabs(r.chi3 - it->chi3) + fabs(r.chi4 - it->chi4);

				if(dist < i_min_val)
				{
					i_min = i;
					i_min_val = dist;
				}
			}

			// If we found a rotamer that is very close to the current conformation
			// then check it. 15.0f is just an arbitrary/empirically chosen threshold.
			if(i_min_val / res_set->getNumberOfTorsions() < 15.0f)
			{
				rotamer_menu_->actions().at(i_min)->setChecked(true);
			}

			rotamer_menu_->setEnabled(true);
		}

		void MolecularControl::toggleDisulfidBond()
		{
			if (context_composite_ == 0) return;
			bool success = false;

			Composite* composite1, *composite2;
			list<Composite*> composite_list = getMainControl()->getMolecularControlSelection();

			if (composite_list.size()==2)
			{
				list<Composite*>::iterator cit = composite_list.begin();
				composite1 = *cit;
				++cit;
				composite2 = *cit;

				DisulfidBondProcessor dbp;
				success = dbp.connect(composite1, composite2, true);
			}
			if (success)
			{
				Scene* scene = Scene::getInstance(0);
				if (scene != 0)
				{
					// update representation
					scene->notify(new CompositeMessage(*composite1, CompositeMessage::CHANGED_COMPOSITE));
					scene->notify(new CompositeMessage(*composite2, CompositeMessage::CHANGED_COMPOSITE));

					scene->getMainControl()->update(*composite1, true);
					scene->getMainControl()->update(*composite2, true);
					setStatusbarText((String)tr("  > disulfid bond created.") + String());
				}
			}
			else
			{
				setStatusbarText((String)tr("Disulfid bond could not be created!"));
			}
		}

		void MolecularControl::compositeProperties()
		{
			if (context_composite_ == 0) return;

			CompositeProperties as(context_composite_, this);
			if (!as.exec()) return;

			getMainControl()->update(*context_composite_, true);
		}

		void MolecularControl::bondProperties()
		{
			if (context_composite_ == 0) return;

			Atom* const atom = dynamic_cast<Atom*>(context_composite_);
			if (atom == 0) return;
			if (atom->countBonds() == 0)
			{
				setStatusbarText((String)tr("Atom has no bonds!"));
				return;
			}
			BondProperties bs(atom, this);
			bs.exec();
		}

		void MolecularControl::buildBonds()
		{
			if (context_composite_ == 0) return;
			notify_(new MolecularTaskMessage(MolecularTaskMessage::BUILD_BONDS));
		}

		void MolecularControl::centerCamera()
		{
			if (context_composite_ == 0) return;
			notify_(new CompositeMessage(*context_composite_, CompositeMessage::CENTER_CAMERA));
		}

		void MolecularControl::checkResidue()
		{
			notify_(new MolecularTaskMessage(MolecularTaskMessage::CHECK_RESIDUE));
		}

		void MolecularControl::showFilename()
		{
			if (context_composite_ == 0) return;
			if (getMainControl()->isBusy()) return;

			const AtomContainer* const ac = dynamic_cast<AtomContainer*>(&context_composite_->getRoot());
			
			if (ac != 0)
			{
				setStatusbarText((String)tr("Composite is from file") + String("  ") + (String)ac->getProperty("FROM_FILE").getString());
			}
			else
			{
				setStatusbarText((String)tr("Composite is not from file"));
			}
		}


		void MolecularControl::updateSelection()
		{
			if (ignore_messages_) return;

 			GenericControl::updateSelection();

			selected_.clear();
			context_composite_ = 0;

			std::set<QTreeWidgetItem*> highlighted;
			QList<QTreeWidgetItem *> items = getSelectedItems();
			QList<QTreeWidgetItem *>::iterator it = items.begin();
			for (; it != items.end(); it++)
			{
				QTreeWidgetItem* item = *it;
				QTreeWidgetItem* parent = item->parent();
				// we have to prevent inserting childs of already selected parents, 
				// otherwise we get serious trouble
				if (parent != 0 && highlighted.find(parent) != highlighted.end())
				{
					highlighted.insert(item);
					continue;
				}

				selected_.push_back(((MyTreeWidgetItem*) item)->composite);
				highlighted.insert(item);
			}

			if (selected_.size() > 0) context_composite_ = *selected_.begin();

            if (selected_.size() == 1 && RTTI::isKindOf<System>(*selected_.begin()))
			{
				bool is_in_move_mode = Scene::getInstance(0) && Scene::getInstance(0)->inMoveMode();

				if (!is_in_move_mode) showFilename();
			}

			// sent new selection through tree
			ControlSelectionMessage* message = new ControlSelectionMessage;
			message->setSelection(selected_);
			notify_(message);
		}

		void MolecularControl::showGuestContextMenu(const QPoint& pos)
		{
			if (getMainControl()->isBusy())
			{
				setStatusbarText((String)tr("No changes allowed, while simulation is running or creating new representations!"), true);
				return;
			}

			QList<QTreeWidgetItem *> items = listview->selectedItems();
			if (items.size() == 0) return;

			MyTreeWidgetItem* item = (MyTreeWidgetItem*)*items.begin();
			Composite* composite = item->composite;

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


		void MolecularControl::recurseGeneration_(QTreeWidgetItem* item, Composite& composite)
		{
			Composite::ChildCompositeIterator it = composite.beginChildComposite();
			for (; it != composite.endChildComposite(); ++it)
			{
				generateListViewItem_(item, *it);
			}
		}

		void MolecularControl::initializeWidget(MainControl& main_control)
		{
			String hint;

			select_id_ = insertMenuEntry(MainControl::EDIT, tr("&Select"), this, SLOT(select()),
			                             "Shortcut|Edit|Select", QKeySequence(),
																	 tr("Select a molecular object to see its position in the scene or to mark it for a simulation"),
																	 UIOperationMode::MODE_ADVANCED);

			deselect_id_ = insertMenuEntry(MainControl::EDIT, tr("&Deselect"), this, SLOT(deselect()),
			                               "Shortcut|Edit|Deselect", QKeySequence(), tr("Deselect a molecular object."),
																		 UIOperationMode::MODE_ADVANCED);

			main_control.insertPopupMenuSeparator(MainControl::EDIT, UIOperationMode::MODE_ADVANCED);

			String description = "Shortcut|Edit|Cut";
			cut_id_ = insertMenuEntry(MainControl::EDIT, tr("Cu&t"), this, 
															  SLOT(cut()), description, QKeySequence::Cut,
															  tr(""), UIOperationMode::MODE_ADVANCED);

			description = "Shortcut|Edit|Copy";
			copy_id_ = insertMenuEntry(MainControl::EDIT, tr("&Copy"), this, 
																 SLOT(copy()), description, QKeySequence::Copy,
																 tr(""), UIOperationMode::MODE_ADVANCED);

			description = "Shortcut|Edit|Paste";
			paste_id_ = insertMenuEntry(MainControl::EDIT, tr("&Paste"), this, 
																  SLOT(paste()), description, QKeySequence::Paste,
																  tr(""), UIOperationMode::MODE_ADVANCED);

			main_control.insertDeleteEntry();
			main_control.insertPopupMenuSeparator(MainControl::EDIT, UIOperationMode::MODE_ADVANCED);

			description = "Shortcut|Edit|Clear_Clipboard";
			clipboard_id_ = insertMenuEntry(MainControl::EDIT, tr("Clear Clipboard"), this, 
																			SLOT(clearClipboard()), description, QKeySequence(),
																			tr("Clear the items in the clipboard"), UIOperationMode::MODE_ADVANCED);

			GenericControl::initializeWidget(main_control);

			description = "Shortcut|Display|Create|Distance_Label";
			distance_action_ = insertMenuEntry(MainControl::DISPLAY_CREATE, tr("Distance Label"), this, 
																				 SLOT(showDistance()), description, QKeySequence(),
																				 tr("Render a label for the distance between two highlighted atoms"),
																				 UIOperationMode::MODE_ADVANCED);

			description = "Shortcut|Display|Create|Angle_Label";
			angle_action_ = insertMenuEntry(MainControl::DISPLAY_CREATE, tr("Angle Label"), this, 
																			SLOT(showAngle()), description, QKeySequence(),
																			tr("Render a label for the angle between three highlighted atoms"),
																			UIOperationMode::MODE_ADVANCED);
		}


		void MolecularControl::addComposite(Composite& composite, String given_name)
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
			updateSelection();
		}


		Size MolecularControl::removeComposite(Composite& composite)
		{
			std::map<Composite*, MyTreeWidgetItem*>::iterator to_find = 
				composite_to_item_.find(&composite);

			if (to_find == composite_to_item_.end())
			{
//   				setStatusbarText(String("Tried to remove an invalid Composite in ") 
//   																 + String(__FILE__) + " " + String(__LINE__), true);
				return 0;
			}
			
			QTreeWidgetItem* item = to_find->second;

			context_composite_ = 0;
			context_item_ = 0;
			nr_items_removed_ = 1;

			bool enabled = !ignore_messages_;
			enableUpdates_(false);

			if (composite.isRoot() && listview->topLevelItemCount() <= 1)
			{
				composite_to_item_.clear();

				listview->takeTopLevelItem(0);
				delete item;
				enableUpdates_(enabled);
				return 1;
			}

			removeRecursive_(to_find->second);
			enableUpdates_(enabled);
			return nr_items_removed_;
		}

		void MolecularControl::removeRecursive_(QTreeWidgetItem* item)
		{
			composite_to_item_.erase(((MyTreeWidgetItem*)item)->composite);

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
		{
		#ifdef BALL_VIEW_DEBUG
			Log.error() << "MolecularControl " << this << " onNotify " << message << std::endl;
		#endif

			if (ignore_messages_) return;
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


		// set the highlighting according to a given list
		void MolecularControl::highlight(const list<Composite*>& selection)
		{	
			enableUpdates_(false);
			listview->clearSelection();
			selected_.clear();

			list<QTreeWidgetItem*> items;
			list<Composite*>::const_iterator sit = selection.begin();
			std::map<Composite*, MyTreeWidgetItem*>::iterator fit;
			for (; sit != selection.end(); ++sit)
			{
				fit = composite_to_item_.find(*sit);
				if (fit == composite_to_item_.end()) continue;
				items.push_back(fit->second);
				selected_.push_back(*sit);

				QTreeWidgetItem* parent = (*fit->second).parent();
				while (parent != 0 && !listview->isItemExpanded(parent))
				{
					listview->expandItem(parent);
					parent = parent->parent();
				}
			}
			listview->selectItems(items);

			ControlSelectionMessage* message = new ControlSelectionMessage;
			message->setSelection(selected_);
			notify_(message);

			enableUpdates_(true);
		}

		// set the checkboxes according to the selection in the MainControl
		void MolecularControl::setSelection_(bool /*open*/, bool /*force*/)
		{	
			//if (!force) open = false;

			QTreeWidgetItemIterator qit(listview);
			while (*qit != 0)
			{
				Composite* c = ((MyTreeWidgetItem*)*qit)->composite;
				if (c->isSelected())
				{
					(*qit)->setCheckState(2, Qt::Checked);
				}
				else
				{
					(*qit)->setCheckState(2, Qt::Unchecked);
				}
				qit++;
			}

			getMainControl()->printSelectionInfos();
		}


		void MolecularControl::cut()
		{
			if (selected_.size() == 0) return;

			// delete old composites in copy list
			if (!was_delete_) clearClipboard();

			enableUpdates_(false);

			// remove the selected composites from the tree and from the scene
			// if !was_delete_, copy them into the copy_list_
			// for all roots of removed items the representations are rebuild
			HashSet<Composite*> roots, deleted_roots;

			list<Composite*> sel = selected_;
			selected_.clear();
			list<Composite*>::iterator it = sel.begin();
			for (; it != sel.end(); it++)
			{
				bool update = false;
				Composite& c = **it;
				if (!c.isRoot())
				{
					roots.insert(&c.getRoot());
				}
				else
				{
 					removeComposite(c);
					update = true;
					deleted_roots.insert(&c);
				}

				getMainControl()->remove(c, was_delete_, update);

				if (!was_delete_) copy_list_.push_back(*it);
			}


			listview->clearSelection();
 			enableUpdates_(true);
			HashSet<Composite*>::Iterator roots_it = roots.begin();
			for (; +roots_it; roots_it++)
			{
				if (!deleted_roots.has(*roots_it))
				{
					getMainControl()->update(**roots_it, true);
				}
			}
		}


		void MolecularControl::copy()
		{
			if (getMainControl()->isBusy()) return;

			const list<Composite*> selection = getSelection();
			if (selection.size() == 0) return;

			setStatusbarText((String)tr("copied ") + String(selection.size()) + (String)tr(" objects") + " ...");

			// delete old cutted composites
			clearClipboard();

			// copy the selected composites into the copy_list_
			list<Composite*>::iterator it = selected_.begin();
			for (; it != selected_.end(); it++)
			{
				copy_list_.push_back((Composite*)(*it)->create());
			}
		}


		void MolecularControl::paste()
		{
			if (getMainControl()->isBusy() || selected_.size() > 1) return;

			if (copy_list_.size() == 0) return;

			setStatusbarText((String)tr("Pasted ") + String(copy_list_.size()) + (String)tr(" objects") + "...");

			HashSet<Composite*> changed_roots;
			// copying composites
			list<Composite*>::const_iterator list_it = copy_list_.begin();
			for (; list_it != copy_list_.end(); ++list_it)
			{
                if (RTTI::isKindOf<System>(*list_it))
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
					setStatusbarText((String)tr("Could not paste, no or more than 1 item selected!"));
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
			
			// TODO: for convenience, move back to rotate mode if we were in edit mode before
		}


		void MolecularControl::clearClipboard()
		{
			// delete old composites
			if (copy_list_.size() > 0)
			{
				list<Composite*>::const_iterator list_it = copy_list_.begin();
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
		{
			#ifdef BALL_VIEW_DEBUG
			if (composite_to_item_.find(&composite) != composite_to_item_.end())
			{
				Log.error() << (String)tr("Composite ") << & composite << (String)tr(" already added!") << std::endl;
			}
			#endif

			if (!show_ss_)
			{
                if (RTTI::isKindOf<SecondaryStructure>(&composite))
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
			MyTreeWidgetItem* new_item = 0;

			// is this a root item?
			if (parent == 0)
			{
				new_item = new MyTreeWidgetItem(listview, sl, &composite);
			} 
			else 
			{
				// no, insert into the current item
				new_item = new MyTreeWidgetItem(parent, sl, &composite);
			}

			composite_to_item_[&composite] = new_item;
			recurseGeneration_(new_item, composite);

			return new_item;
		}


		MolecularInformation& MolecularControl::getInformationVisitor_()
		{
			return information_;
		}

		const list<Composite*>& MolecularControl::getSelection() const
		{
			return selected_;
		}

		Composite* MolecularControl::getContextComposite()
		{
			return context_composite_;
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
			QTreeWidgetItemIterator qit(listview);
			while(*qit != 0)
			{
				listview->collapseItem(*qit);
				qit++;
			}
		}

		void MolecularControl::expandAll()
		{
			QTreeWidgetItemIterator qit(listview);
			while(*qit != 0)
			{
				listview->expandItem(*qit);
				qit++;
			}
		}

		void MolecularControl::createRepresentation_()
		{
			notify_(new CreateRepresentationMessage(selected_, selected_model_, selected_coloring_method_));
		}

		void MolecularControl::countItems()
		{
			if (context_composite_ == 0 ||
					dynamic_cast<AtomContainer*>(context_composite_) == 0) 
			{
				return;
			}

			AtomContainer& ac = *(AtomContainer*) context_composite_;

			String s;
            if (RTTI::isKindOf<System>(&ac))
			{
				s+=String(((System*)&ac)->countResidues());
				s+= (String)tr(" Residues") + ", ";
			}
            else if (RTTI::isKindOf<Protein>(&ac))
			{
				s+=String(((Protein*)&ac)->countResidues());
				s+= (String)tr(" Residues") + ", ";
			}
            else if (RTTI::isKindOf<Chain>(&ac))
			{
				s+=String(((Chain*)&ac)->countResidues());
				s+= (String)tr(" Residues") + ", ";
			}
            if (RTTI::isKindOf<SecondaryStructure>(&ac))
			{
				s+=String(((SecondaryStructure*)&ac)->countResidues());
				s+= (String)tr(" Residues") + ", ";
			}

			s += String(ac.countAtoms()) + (String)tr(" Atoms") + ", ";
			s += String(ac.countBonds()) + (String)tr(" Bonds");
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
			catch(Exception::ParseError& e)
			{
				setStatusbarText(tr("Invalid expression ") + e.getMessage(), true);
				return 0;
			}
			catch(Exception::GeneralException& e)
			{
				setStatusbarText(QString(e.getMessage()), true);
				return 0;
			}

			HashSet<Composite*> roots;
			Size nr_of_atoms = 0;

			CompositeManager::CompositeIterator it = getMainControl()->getCompositeManager().begin();
			for(; +it; it++)
			{
				try
				{
					(*it)->apply(s);
				}
				catch(Exception::GeneralException& e)
				{
					setStatusbarText(tr("Invalid expression ") + e.getMessage(), true);
					return 0;
				}

				list<Atom*>::iterator ait = s.getSelectedAtoms().begin();
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

			setStatusbarText((String)tr("Selected ") + String(s.getNumberOfSelectedAtoms()) + (String)tr(" atoms."));
			listview->setFocus();

			return nr_of_atoms;
		}


		void MolecularControl::clearSelector()
		{
			selector_edit_->addItem("");
			Index x = selector_edit_->findText("");
			selector_edit_->setCurrentIndex(x);
			getMainControl()->clearSelection();
		}


		void MolecularControl::deleteCurrentItems()
		{
			if(getMainControl()->isBusy()) {
				setStatusbarText(tr("No changes allowed, while simulation is running or creating new representations!"), true);
				return;
			}
			was_delete_ = true;
			cut();
			was_delete_ = false;
		}


		bool MolecularControl::pasteAllowedFor_(Composite& child)
		{
            if (RTTI::isKindOf<System>(&child))
			{
				return !getSelection().size();
			}

			if (!getSelection().size()) return false;

            const Composite* parent = *getSelection().begin();

			if (RTTI::isKindOf<Atom>(parent)) return false;
			
            if (RTTI::isKindOf<Residue>(parent)) return (RTTI::isKindOf<Atom>(&child));

            if (RTTI::isKindOf<SecondaryStructure>(parent)) return (RTTI::isKindOf<Residue>(&child));

            if (RTTI::isKindOf<Chain>(parent)) return (RTTI::isKindOf<SecondaryStructure>(&child) ||
                                                                                                 RTTI::isKindOf<Residue>(&child));
			
            if (RTTI::isKindOf<Protein>(parent)) return (RTTI::isKindOf<Chain>(&child));

            if (RTTI::isKindOf<Nucleotide>(parent)) return (RTTI::isKindOf<Atom>(&child));

            if (RTTI::isKindOf<NucleicAcid>(parent)) return (RTTI::isKindOf<Nucleotide>(&child));

            if (RTTI::isKindOf<Molecule>(parent)) return (!RTTI::isKindOf<Molecule>(&child));
            if (RTTI::isKindOf<System>(parent)) return (RTTI::isKindOf<Molecule>(&child));

			return true;
		}


		void MolecularControl::select()
		{
			// copy list, because selection could change
			list<Composite*> selection = selected_;
			newSelection_(selection, true);
		}


		void MolecularControl::deselect()
		{
			// copy list, because selection could change
			list<Composite*> selection = selected_;
			newSelection_(selection, false);
		}

		void MolecularControl::newSelection_(list<Composite*>& sel, bool selected)
		{
			HashSet<Composite*> roots;
			MainControl* mc = getMainControl();
			
			list<Composite*>::iterator it = sel.begin();
			for(; it != sel.end(); it++)
			{
				roots.insert(&(*it)->getRoot());
				if (selected)
				{
					mc->selectCompositeRecursive(*it, true);
				}
				else
				{
					mc->deselectCompositeRecursive(*it, true);
				}
			}
			
			setSelection_(false);

			HashSet<Composite*>::Iterator rit = roots.begin();
			for (; +rit; ++rit)
			{
				getMainControl()->updateRepresentationsOf((**rit), false);
			}
		}

		Size MolecularControl::applySelector(const String& expression)
		{
			selector_edit_->addItem(expression.c_str());
			Index x = selector_edit_->findText(expression.c_str());
			selector_edit_->setCurrentIndex(x);
			return applySelector();
		}

		void MolecularControl::highlightSelection()
		{
			enableUpdates_(false);
			collapseAll();
			listview->clearSelection();

			bool to_expand = getMainControl()->getSelection().size() < 50;
			bool expand = true;
			Size nr_expanded = 0;
			list<QTreeWidgetItem *> items;
			QTreeWidgetItemIterator qit(listview);
			while (*qit != 0)
			{
				QTreeWidgetItem* item = *qit;
				if (item->checkState(2) == Qt::Checked)
				{
					items.push_back(item);

					if (expand)
					{
						QTreeWidgetItem* parent = item->parent();
						while (parent != 0 && !listview->isItemExpanded(parent))
						{
							listview->expandItem(parent);
							parent = parent->parent();
						}
						nr_expanded++;
						if (!to_expand && nr_expanded > 10)
						{
							expand = false;
						}
					}
				}
				qit++;
			}
			
			listview->selectItems(items);

			enableUpdates_(true);
			if (items.size()) listview->scrollToItem(*items.begin());
		}

		void MolecularControl::enableUpdates_(bool state)
		{
			if (ignore_messages_ == !state) return;

			ignore_messages_ = !state;
			listview->setUpdatesEnabled(state);
			disconnect(listview, SIGNAL(itemSelectionChanged()), this, SLOT(updateSelection()));
			if (!state) return;

			listview->update();
			updateSelection();
			connect(listview, SIGNAL(itemSelectionChanged()), this, SLOT(updateSelection()));
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
		}

		void MolecularControl::writePreferences(INIFile& inifile)
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

		void MolecularControl::onItemClicked(QTreeWidgetItem* item, int col)
		{
			if (col != 2) return;
			bool checked = (item->checkState(2) == Qt::Checked);

			if (getMainControl()->isBusy())
			{
				if (checked) item->setCheckState(2, Qt::Unchecked);
				else 				 item->setCheckState(2, Qt::Checked);

				VIEW::getMainControl()->setStatusbarText((String)tr("Cannot select items now!"), true);
				return;
			}

			list<Composite*> l;
			l.push_back(((MyTreeWidgetItem*)item)->composite);
			newSelection_(l, checked);
		}

		void MolecularControl::showAtomOverview()
		{
			AtomOverview ao;
			ao.setParent(dynamic_cast<AtomContainer*>(((MyTreeWidgetItem*)context_item_)->composite));
			ao.exec();
		}

		void MolecularControl::showAtomOverviewForSelection()
		{
			AtomOverview ao;
			ao.showOnlySelection(true);
			ao.setParent(dynamic_cast<AtomContainer*>(((MyTreeWidgetItem*)context_item_)->composite));
			ao.exec();
		}

		void MolecularControl::showDistance()
		{
			if (getSelection().size() != 2) return;

			list<Composite*>::const_iterator it = getSelection().begin();

			Atom* a1 = dynamic_cast<Atom*>(*it);
			it++;
			Atom* a2 = dynamic_cast<Atom*>(*it);
			showDistance(a1, a2);
		}

		void MolecularControl::showDistance(Atom* a1, Atom* a2)
		{
			if (!a1 || !a2 || a1 == a2) return;

			Vector3 pos1 = a1->getPosition();
			Vector3 pos2 = a2->getPosition();
			ColorRGBA color(0,0,1.0);
			Scene* scene = Scene::getInstance(0);
			ColorRGBA lcolor(1,0,1.0);
			if (scene != 0)
			{
				color = scene->getStage()->getInfoColor();
				lcolor = scene->getStage()->getBackgroundColor().getInverseColor();
			}

			Representation* rep = new Representation;

			Sphere* sphere = new Sphere;
			sphere->setPosition(pos1);
			sphere->setRadius(0.05);
			sphere->setColor(color);
			rep->insert(*sphere);

			sphere = new Sphere;
			sphere->setPosition(pos2);
			sphere->setRadius(0.05);
			sphere->setColor(color);
			rep->insert(*sphere);

			Tube* tube = new Tube;
			tube->setRadius(0.05);
			tube->setVertex1(pos1);
			tube->setVertex2(pos2);
			tube->setColor(color);
			rep->insert(*tube);

			Label* label = new Label;
			label->setVertex(pos1 + (pos2 - pos1) / 2.);
			label->setText(String((pos1 - pos2).getLength()));
			label->setColor(lcolor);
			rep->insert(*label);

			rep->setProperty(Representation::PROPERTY__ALWAYS_FRONT);
			getMainControl()->insert(*rep);
			getMainControl()->update(*rep);
		}

		void MolecularControl::showAngle()
		{
			list<Composite*>::const_iterator it = getSelection().begin();

			vector<Atom*> atoms;

			for (; it != getSelection().end(); it++)
			{
				Atom* a = dynamic_cast<Atom*>(*it);
				if (a != 0) atoms.push_back(a);
			}

			if (atoms.size() < 3 || atoms.size() > 4) return;

			if (atoms.size() == 3) atoms.push_back((Atom*)0);

			showAngle(atoms[0], atoms[1], atoms[2], atoms[3]);
		}

		// TODO: why a4??
		void MolecularControl::showAngle(Atom* a1, Atom* a2, Atom* a3, Atom* /*a4*/)
		{
			if (!a1 || !a2 || !a3) return;
			
			Vector3 pos1 = a1->getPosition();
			Vector3 pos2 = a2->getPosition();
			Vector3 pos3 = a3->getPosition();

			Vector3 v1,v2,v3;
			try
			{
				v1 = pos1 - pos2;
				v2 = pos3 - pos2;
				v1.normalize();
				v2.normalize();
				v3 = v1 + v2;
				v3 /= 4.;
			}
			catch(...)
			{
				return;
			}

			ColorRGBA color(0,0,1.0);
			ColorRGBA lcolor(1,0,1.0);
			Scene* scene = Scene::getInstance(0);
			if (scene != 0)
			{
				color = scene->getStage()->getInfoColor();
				lcolor = scene->getStage()->getBackgroundColor().getInverseColor();
			}

			Representation* rep = new Representation;

			Sphere* sphere = new Sphere;
			sphere->setPosition(pos1);
			sphere->setRadius(0.05);
			sphere->setColor(color);
			rep->insert(*sphere);

			sphere = new Sphere;
			sphere->setPosition(pos2);
			sphere->setRadius(0.05);
			sphere->setColor(color);
			rep->insert(*sphere);

			sphere = new Sphere;
			sphere->setPosition(pos3);
			sphere->setRadius(0.05);
			sphere->setColor(color);
			rep->insert(*sphere);

			Tube* tube = new Tube;
			tube->setRadius(0.05);
			tube->setVertex1(pos1);
			tube->setVertex2(pos2);
			tube->setColor(color);
			rep->insert(*tube);

			tube = new Tube;
			tube->setRadius(0.05);
			tube->setVertex1(pos2);
			tube->setVertex2(pos3);
			tube->setColor(color);
			rep->insert(*tube);

			Angle angle;
			GetAngle(v1, v2, angle);
			Label* label = new Label;
			Vector3 v = pos2 + v3;
			label->setVertex(v);
			label->setText(String(angle.toDegree()));
			label->setColor(lcolor);
			rep->insert(*label);

			rep->setProperty(Representation::PROPERTY__ALWAYS_FRONT);
			getMainControl()->insert(*rep);
			getMainControl()->update(*rep);
		}

		QMenu& MolecularControl::getContextMenu()
		{
			return context_menu_;
		}

		bool MolecularControl::allowPaste_()
		{
			bool allow_paste = true;
			list<Composite*>::iterator it = copy_list_.begin();
			for (; it != copy_list_.end(); it++)
			{
				if (!pasteAllowedFor_(**it)) 
				{
					allow_paste = false;
					break;
				}
			}

			return allow_paste;
		}

		void MolecularControl::changeRotamer_(int i)
		{
			if(!current_residue_) {
				return;
			}

			rotamer_library_mutex_.lockForRead();
			ResidueRotamerSet* res_set = rotamer_library_->getRotamerSet(*current_residue_);
			rotamer_library_mutex_.unlock();

			if(!res_set || (unsigned int)i >= res_set->getNumberOfRotamers()) {
				return;
			}

			res_set->setRotamer(*current_residue_, res_set->getRotamer(i));

			getMainControl()->updateRepresentationsOf(*current_residue_, true);
		}

	} // namespace VIEW
} // namespace BALL
