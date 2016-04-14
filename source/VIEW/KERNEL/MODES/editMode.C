/* TRANSLATOR BALL::VIEW::editMode

		Necessary for lupdate.
*/

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/KERNEL/MODES/editMode.h>

#include <QtWidgets/QMenu>
#include <QtGui/QKeyEvent>
#include <QtGui/QWheelEvent>

#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/PDBAtom.h>
#include <BALL/KERNEL/system.h>
#include <BALL/QSAR/ringPerceptionProcessor.h>
#include <BALL/STRUCTURE/geometricTransformations.h>

#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/iconLoader.h>
#include <BALL/VIEW/DIALOGS/PTEDialog.h>
#include <BALL/VIEW/DIALOGS/compositeProperties.h>

namespace BALL
{
	namespace VIEW
	{
		EditMode::EditMode(Scene* scene)
			: InteractionMode(scene),
				temp_move_(false),
				atom_number_(0),
			  fragment_db_(0),
				delete_atom_(0),
				atom_properties_(0),
				charge_menu_(0),
				order_menu_(0),
				add_menu_(0)
		{
			createActions_();
		}

		EditMode::~EditMode()
		{
			delete charge_menu_;
			delete order_menu_;
			delete fragment_db_;
		}

		void EditMode::createActions_()
		{
			IconLoader& loader = IconLoader::instance();

			String description = "Shortcut|Display|Edit_Mode";
			main_action_ = scene_->insertMenuEntry(MainControl::DISPLAY, tr("Edit Mode"), 0,
			                                       0, description, QKeySequence("Ctrl+E"),
																						 tr("Create and modify molecular structures"),
																						 UIOperationMode::MODE_ADVANCED);

			if (main_action_)
			{
				main_action_->setToolTip(tr("Switch to edit mode, e.g. draw your own molecule"));

				main_action_->setCheckable(true);
				main_action_->setIcon(loader.getIcon("actions/mode-edit"));
				connect(main_action_, SIGNAL(triggered()), SLOT(modeChangeSlot_()));
			}

			QMenu* qmenu;

			if (UIOperationMode::instance().getMode() <= UIOperationMode::MODE_ADVANCED)
			{
				description = "Shortcut|EditMode|SetElement";
				element_action_ = new QAction(loader.getIcon("actions/molecule-set-element"), tr("Set element"), this);
				element_action_->setToolTip(tr("Edit mode: Choose element for next atom, to modify atom under cursor: Double left click"));
				element_action_->setObjectName(element_action_->text());
				connect(element_action_, SIGNAL(triggered()), this, SLOT(changeAtomElementTriggered_()));
				getMainControl()->getShortcutRegistry().registerShortcut(description, element_action_);
				qmenu = scene_->getMainControl()->initPopupMenu(MainControl::BUILD);
				qmenu->addAction(element_action_);

				description = "Shortcut|EditMode|CreateBond";
				bond_action_ = new QAction(loader.getIcon("actions/create-bond"), tr("Create Bond"), this);
				bond_action_->setToolTip(tr("Edit mode: If two atoms are selected, create a single bond between them"));
				bond_action_->setObjectName(bond_action_->text());
				connect(bond_action_, SIGNAL(triggered()), this, SLOT(createBond_()));
				scene_->getMainControl()->getShortcutRegistry().registerShortcut(description, bond_action_);
				// 			qmenu = getMainControl()->initPopupMenu(MainControl::BUILD);
				qmenu->addAction(bond_action_);
			}
		}

		void EditMode::addToolBarEntries(QToolBar* tb)
		{
			if (UIOperationMode::instance().getMode() <= UIOperationMode::MODE_ADVANCED)
			{
				tb->addAction(main_action_);
				tb->addAction(element_action_);
				tb->addAction(bond_action_);
			}
		}

		void EditMode::keyPressEvent(QKeyEvent* evt)
		{
			if (!scene_->getMainControl()->isBusy())
			{
				QPoint point = scene_->mapFromGlobal(QCursor::pos());

				// TODO QShortcut* shortcut
				if (evt->key() == Qt::Key_D)
				{
					scene_->getClickedItems(point);
					deleteCurrentAtomTriggered_();

					return;
				}

				// TODO QShortcut* shortcut
				if (evt->key() == Qt::Key_Backspace)
				{
					scene_->getClickedItems(point);
					deleteCurrentBondTriggered_();
					return;
				}
			}

			// TODO QShortcut* shortcut
			if (evt->key() < Qt::Key_A || evt->key() > Qt::Key_Z)
			{
				return;
			}

			// TODO QShortcut* shortcut
			if      (evt->key() == Qt::Key_H) scene_->setEditElementType(1);
			else if (evt->key() == Qt::Key_C) scene_->setEditElementType(6);
			else if (evt->key() == Qt::Key_N) scene_->setEditElementType(7);
			else if (evt->key() == Qt::Key_O) scene_->setEditElementType(8);
			else if (evt->key() == Qt::Key_P) scene_->setEditElementType(15);
			else if (evt->key() == Qt::Key_S) scene_->setEditElementType(16);
			else
			{
				return;
			}

			String text(qApp->tr("Edit Mode", "Setting element to "));
			text += PTE[scene_->getEditElementType()].getName();
			scene_->setStatusbarText(text);
		}

		void EditMode::keyReleaseEvent(QKeyEvent* /*evt*/)
		{
		}

		void EditMode::mouseDoubleClickEvent(QMouseEvent* evt)
		{
			scene_->getClickedItems(evt->pos());

			if (scene_->getCurrentAtom() != 0)
			{
				int atomic_number = scene_->getEditElementType();

				Atom* current_atom = scene_->getCurrentAtom();
				current_atom->setElement(PTE[atomic_number]);
				String new_name = PTE[atomic_number].getSymbol();

				//get the old atom number
				String old_name = current_atom->getName();
				old_name.toUpper();
				new_name += old_name.trimLeft("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
				current_atom->setName(new_name);

				scene_->deselect();
				scene_->getMainControl()->update(*current_atom);

				return;
			}

			if (scene_->getCurrentBond() != 0)
			{
				Bond* current_bond = scene_->getCurrentBond();
				Atom* a1 = (Atom*)current_bond->getFirstAtom();
				Atom* a2 = (Atom*)current_bond->getSecondAtom();

				if (!a1->getParent() || (a1->getParent() != a2->getParent()))
					return;

				RingPerceptionProcessor rpp;
				vector<vector<Atom*> > rings;
				Composite* comp = a1->getParent();
				AtomContainer* ac = static_cast<AtomContainer*>(comp);
				if (!ac) return;
				rpp.calculateSSSR(rings, *ac);
				rings = rpp.getAllSmallRings();
				vector<Position> rings_to_modify;
				for (Position r = 0; r < rings.size(); r++)
				{
					Size found = 0;
					for (Position a = 0; a < rings[r].size(); a++)
					{
						if (   (rings[r][a] == a1)
								|| (rings[r][a] == a2))
						{
							found++;
						}
					}

					if (found == 2) rings_to_modify.push_back(r);
				}

				for (Position r = 0; r < rings_to_modify.size(); r++)
				{
					HashSet<Atom*> ratoms;
					vector<Atom*>& ring = rings[rings_to_modify[r]];
					for (Position a = 0; a < ring.size(); a++)
					{
						ratoms.insert(ring[a]);
					}

					for (Position a = 0; a < ring.size(); a++)
					{
						AtomBondIterator abit = ring[a]->beginBond();
						for (;+abit; ++abit)
						{
							if (ratoms.has(abit->getPartner(*ring[a])))
							{
								abit->setOrder(Bond::ORDER__AROMATIC);
							}
						}
					}
				}

				if (rings_to_modify.size() == 0)
				{
					scene_->changeBondOrder(1);
				}

				scene_->getMainControl()->update(*a1->getParent(), true);
			}

		}

		void EditMode::mouseMoveEventImpl_(QMouseEvent* evt)
		{
			if (scene_->isAnimationRunning() || scene_->getMainControl()->isBusy()) return;

			if (temp_move_)
			{
				// TODO: this *has* to work again!
//				scene_->processMoveModeMouseEvents_(evt);

				return;
			}

			// create a new bond
			//
			// is there an atom nearby the actual mouse position?
			Atom* last_atom = scene_->getCurrentAtom();
			scene_->getClickedItems(evt->pos());

			Vector2 draw_to;

			// have we found such an atom? if so, is it different from the one we started with?
			// (self bonds make no sense)
			if (last_atom && scene_->getCurrentAtom() &&
					last_atom != scene_->getCurrentAtom() &&
					// workaround against crashes:
					&last_atom->getRoot() == &scene_->getCurrentAtom()->getRoot())
			{
				// if we are really close to an atom, the endpoints of the line we draw will be set to
				// its center, so that the user has a drop in effect for the bonds
				draw_to = scene_->map3DToViewport(scene_->getCurrentAtom()->getPosition());
			}
			else
			{
				draw_to.x = evt->x();
				draw_to.y = evt->y();
			}

			scene_->setCurrentAtom(last_atom);

			scene_->updateGL();

			if (scene_->getCurrentAtom() == 0)
				return;

			Vector2 draw_from = scene_->map3DToViewport(scene_->getCurrentAtom()->getPosition());
			scene_->drawLine(draw_from, draw_to);

		}

		void EditMode::mousePressEventImpl_(QMouseEvent* evt)
		{
			if (scene_->isAnimationRunning() || scene_->getMainControl()->isBusy()) return;

			scene_->deselect();

			last_buttons_ = evt->buttons();

			if (evt->button() != Qt::RightButton)
			{
				//TODO: readd this
				if (//only_highlighted_ &&
						getMainControl()->getMolecularControlSelection().size() == 0)
				{
					scene_->setStatusbarText(qApp->tr("Edit Mode", "Warning: no AtomContainer highlighted"), true);
					return;
				}
			}

			scene_->getClickedItems(evt->pos());

			/////////////////////////////////////////
			// right button -> context menu
			if (evt->button() == Qt::RightButton)
			{
				if (scene_->getCurrentAtom())
				{
					scene_->getCurrentAtom()->select();
					scene_->notify(new CompositeMessage(*scene_->getCurrentAtom(), CompositeMessage::SELECTED_COMPOSITE));
				}

				if (scene_->getCurrentBond())
				{
					scene_->getCurrentBond()->select();
					Atom* a1 = (Atom*)scene_->getCurrentBond()->getFirstAtom();
					Atom* a2 = (Atom*)scene_->getCurrentBond()->getSecondAtom();
					a1->select();
					a2->select();
					scene_->notify(new CompositeMessage(*a1, CompositeMessage::SELECTED_COMPOSITE));
					scene_->notify(new CompositeMessage(*a2, CompositeMessage::SELECTED_COMPOSITE));
				}

				// we open a context menu at this point
				scene_->showContextMenu(evt->pos());
				return;
			}

			////////////////////////////////////////////////
			// left button -> add atom or move existing atom
			if (evt->button() == Qt::LeftButton && evt->modifiers() != Qt::ControlModifier)
			{
				if (scene_->getCurrentBond() != 0) return;

				if (scene_->getCurrentAtom() != 0)
				{
					getMainControl()->selectCompositeRecursive(scene_->getCurrentAtom(), true);
					mouse_pos_old_ = mouse_pos_new_ = evt->globalPos();
					temp_move_ = true;
					return;
				}

				/////////////////////////////////////////
				// insert a new atom:
				int atomic_number = scene_->getEditElementType();

				String name = PTE[atomic_number].getSymbol();
				name += String(atom_number_);
				++atom_number_;
				PDBAtom* a = new PDBAtom(PTE[atomic_number], name);
				insert_(evt->x(), evt->y(), *a);
				scene_->setCurrentAtom(a);

				Vector3 atom_position = a->getPosition();

				return;
			}

			//////////////////////////////////////////////
			// middle button -> add bond
			if ( evt->button() == Qt::MidButton ||
			    (evt->button() == Qt::LeftButton && evt->modifiers() == Qt::ControlModifier))
			{
				if (scene_->getCurrentAtom())
				{
					scene_->getCurrentAtom()->select();
					scene_->notify(new CompositeMessage(*scene_->getCurrentAtom(), CompositeMessage::SELECTED_COMPOSITE));
				}
			}
		}

		void EditMode::mouseReleaseEventImpl_(QMouseEvent* evt)
		{
			if (temp_move_)
			{
				scene_->deselect();
				temp_move_ = false;
				return;
			}

			if (last_buttons_ == Qt::RightButton)
			{
				scene_->deselect();
				return;
			}

			if (scene_->isAnimationRunning() || scene_->getMainControl()->isBusy()) return;

			// if we didnt find first atom: abort
			if (!scene_->getCurrentAtom())
			{
				scene_->deselect();
				return;
			}

			scene_->getCurrentAtom()->deselect();
			scene_->notify(new CompositeMessage(*scene_->getCurrentAtom(), CompositeMessage::DESELECTED_COMPOSITE));

			Atom* atom = scene_->getCurrentAtom();
			scene_->getClickedItems(evt->pos());

			// decide what to do... did we find an atom at all?
			if (scene_->getCurrentAtom())
			{
				// is it the atom we started with?
				if (atom == scene_->getCurrentAtom() || // workaround against crashes:
						&atom->getRoot() != &scene_->getCurrentAtom()->getRoot())
				{
					// in this case, we assume that the user does not want to set a bond
					scene_->deselect();
					return;
				}

				// we found _another_ atom: set the bond
				new Bond("Bond", *scene_->getCurrentAtom(), *atom, Bond::ORDER__SINGLE);
				scene_->merge(scene_->getCurrentAtom(), atom);

				// update representation
				getMainControl()->update(*atom, true);
				scene_->setStatusbarText(qApp->tr("Edit Mode", "Added a bond"));
			}
			else // no atom found -> create one
			{
				// project the new atom on the plane of the old atom
				scene_->setCurrentAtom(atom);
				Vector3 new_pos = scene_->mapViewportTo3D(evt->x(), evt->y());

				// test if the two atoms would have the same position
				if (fabs((scene_->getCurrentAtom()->getPosition() - new_pos).getLength()) < 0.02)
				{
					scene_->setStatusbarText(qApp->tr("Edit Mode", "Aborting, since both atoms would have the same location!"), true);
					return;
				}

				// build a new atom...
				int atomic_number = scene_->getEditElementType();
				String name(PTE[atomic_number].getSymbol());
				name += String(atom_number_);
				atom_number_++;
				PDBAtom* a = new PDBAtom(PTE[atomic_number], name);
				a->setPosition(new_pos);
				scene_->getCurrentAtom()->getParent()->appendChild(*a);

				Vector3 atom_position = a->getPosition();
				// set the bond
				new Bond("Bond", *scene_->getCurrentAtom(), *a, Bond::ORDER__SINGLE);

				scene_->getMainControl()->update(*a->getParent(), true);
				scene_->setStatusbarText(qApp->tr("Edit Mode", "Added a bond and an atom"));
			}

			scene_->deselect();
		}

		void EditMode::wheelEvent(QWheelEvent* evt)
		{
			Index delta = evt->delta();
			if (delta == 0) return;

			if (scene_->isAnimationRunning() || scene_->getMainControl()->isBusy()) return;

			if (delta > 1) delta = 1;
			if (delta < -1) delta = -1;

			if (evt->modifiers() == Qt::ShiftModifier)
			{
				scene_->getClickedItems(evt->pos());
				scene_->changeBondOrder(delta);
			}
			else
			{
				InteractionMode::wheelEvent(evt);
			}

			evt->accept();
		}

		void EditMode::activate()
		{
			InteractionMode::activate();

			if (!fragment_db_)
			{
				fragment_db_ = new FragmentDB("fragments/Editing-Fragments.db");
			}

			list<AtomContainer*> acs = scene_->getContainers();

			list<Composite*> sel;
			list<AtomContainer*>::iterator lit = acs.begin();
			for (; lit != acs.end(); lit++)
			{
				sel.push_back(*lit);
			}
			ControlSelectionMessage* msg = new ControlSelectionMessage();
			msg->setSelection(sel);
			scene_->notify(msg);

			//edit_id_->setChecked(true);
			scene_->setElementCursor(scene_->getEditElementType());

			HashSet<Composite*> selection = scene_->getMainControl()->getSelection();
			HashSet<Composite*>::Iterator it = selection.begin();
			for (; +it; ++it)
			{
				if (!(**it).containsSelection()) continue;
				scene_->getMainControl()->deselectCompositeRecursive(*it, true);
				scene_->getMainControl()->update(**it, false);
			}
			scene_->notify(new NewSelectionMessage);

			main_action_->setChecked(true);
		}

		// insert an atom at screen positions (x,y) on the view plane
		// TODO: make the renderer dependent on the current target!
		void EditMode::insert_(int x, int y, PDBAtom &atom)
		{
			// find the 3D coordinates of screen position (x,y) on the view plane
			// move the atom to that position
			atom.setPosition(scene_->mapViewportTo3D(x,y));

			// now we need to find the AtomContainer into which we will insert the atom.
			// get all highlighted composites
			list<Composite*> composite_list = scene_->getMainControl()->getMolecularControlSelection();

			Size nr_high = composite_list.size();
			//TODO: read highlighting
			if (nr_high > 1 /*|| (only_highlighted_ && nr_high == 0)*/)
			{
				scene_->setStatusbarText(qApp->tr("Edit Mode", "Please highlight exactly one AtomContainer for insertion of the created atoms!"), true);
				return;
			}

			// exactly one highlighted composite
			if (nr_high == 1)
			{
				// is it an AtomContainer?
				AtomContainer* ai = dynamic_cast<AtomContainer*>(*composite_list.begin());
				if (ai == 0)
				{
					// is the parent an AtomContainer?
					Composite* parent = (**composite_list.begin()).getParent();
					if (parent != 0)
					{
						ai = dynamic_cast<AtomContainer*>(parent);
					}

					if (ai == 0)
					{
						scene_->setStatusbarText(qApp->tr("Edit Mode", "Please highlight exactly one AtomContainer for insertion of the created atoms!"), true);
						return;
					}
				}

				// prevent adding of atoms to a System:
				// some forcefields will go havoc otherwise
                if (RTTI::isKindOf<System>(ai))
				{
					System* system = (System*) ai;
					Molecule* mol = system->getMolecule(0);
					if (mol == 0)
					{
						mol = new Molecule();
						system->insert(*mol);
					}
					ai = mol;
				}

				// we do not need to create our own system
				ai->insert(atom);
				scene_->getMainControl()->update(*ai, true);
				return;
			}

			/////////////////////////////////////////////////////////
			// no atom container highlighted:

			HashSet<Composite*> composites = getMainControl()->getCompositeManager().getComposites();

			// no System exists? -> create one
			if (composites.size() == 0)
			{
				System *system = new System();
				Molecule* current_molecule = new Molecule();
				system->insert(*current_molecule);
				current_molecule->insert(atom);
				getMainControl()->insert(*system);
				getMainControl()->update(*system);
				return;
			}

			// add to first Molecule in first System
			System* system = dynamic_cast<System*>(*composites.begin());
			Molecule* mol = system->getMolecule(0);
			if (mol == 0)
			{
				mol = new Molecule();
				system->insert(*mol);
			}

			mol->appendChild(atom);
			getMainControl()->update(*mol, true);
		}

		Qt::Key EditMode::getKey() const
		{
			return Qt::Key_E;
		}

		void EditMode::populateContextMenu(QMenu* menu)
		{
			if(!isCurrent())
			{
				return;
			}

			if (!fragment_db_)
			{
				fragment_db_ = new FragmentDB("fragments/Editing-Fragments.db");
			}

			createContextMenuEntries_();

			menu->addAction(atom_properties_);
			menu->addAction(delete_atom_);
			menu->addAction(element_action_);

			menu->addAction(tr("Delete Bond"), this, SLOT(deleteCurrentBondTriggered_()))->setEnabled(scene_->getCurrentBond() != 0);

			menu->addSeparator();

			QAction* change_charge = menu->addMenu(charge_menu_);
			change_charge->setText(tr("Set formal charge"));
			change_charge->setEnabled(scene_->getCurrentAtom() != 0);

			QAction* change_order = menu->addMenu(order_menu_);
			change_order->setText(tr("Change bond order"));
			change_order->setEnabled(scene_->getCurrentBond() != 0);

			Index bo = 5;
			if (scene_->getCurrentBond()) {
				bo = ((Index)scene_->getCurrentBond()->getOrder());
			}
			checkBondOrderActions_(bo);

			menu->addSeparator();

			QAction* add_action = menu->addMenu(add_menu_);
			add_action->setText(tr("Add"));
			if (scene_->getContainers().empty())
			{
				add_action->setEnabled(false);
			}
			else
			{
				add_action->setEnabled(true);
			}
		}

		void EditMode::activatedOrderItem_(QAction* action)
		{
			if (action == 0) return;
			String text = ascii(action->text());

			if (text == (String)tr("Single")) bond_order_ = Bond::ORDER__SINGLE;
			else if (text == (String)tr("Double")) bond_order_ = Bond::ORDER__DOUBLE;
			else if (text == (String)tr("Triple")) bond_order_ = Bond::ORDER__TRIPLE;
			else if (text == (String)tr("Quadruple")) bond_order_ = Bond::ORDER__QUADRUPLE;
			else if (text == (String)tr("Aromatic")) bond_order_ = Bond::ORDER__AROMATIC;
			else if (text == (String)tr("Unknown")) bond_order_ = Bond::ORDER__UNKNOWN;

		}


		void EditMode::addStructure_()
		{
			QAction* action = dynamic_cast<QAction*>(sender());
			if (action == 0) return;

			String name = ascii(action->text());

			scene_->deselect();

			if (!fragment_db_)
			{
				fragment_db_ = new FragmentDB("fragments/Editing-Fragments.db");
			}

			list<AtomContainer*> containers = scene_->getContainers();
			if (containers.size() == 0) return;

			Residue* residue = fragment_db_->getResidueCopy(name);
			if (residue == 0)
			{
				residue = fragment_db_->getResidueCopy(name + "-Skeleton");
				if (residue == 0) return;
			}

			Vector3 p;
			Size nr = 0;
			AtomIterator ait = residue->beginAtom();
			for (;+ait; ++ait)
			{
				p += ait->getPosition();
				nr++;
			}

			if (nr == 0)
			{
				BALLVIEW_DEBUG
					delete residue;
				return;
			}

			p /= (float) nr;

			QPoint pos = scene_->mapFromGlobal(mouse_pos_new_);

			Matrix4x4 m;
			Vector3 x = scene_->mapViewportTo3D(pos.x(), pos.y());
			TransformationProcessor tf;

			Vector3 vv = scene_->getStage()->getCamera().getViewVector();
			float l = vv.getLength();
			if (!Maths::isZero(l)) vv /= l;
			Vector3 axis = Vector3(1,0,0) % vv;
			if (axis.getSquareLength() != 0)
			{
				Angle a = vv.getAngle(Vector3(1,0,0));
				m.setRotation(a, axis);
				tf.setTransformation(m);
				residue->apply(tf);
			}

			m.setTranslation(x - p);
			tf.setTransformation(m);
			residue->apply(tf);

			AtomContainer* s = *containers.begin();
            if (RTTI::isKindOf<System>(s))
			{
				System* system = (System*) s;
				Molecule* mol = system->getMolecule(0);
				if (mol == 0)
				{
					mol = new Molecule();
					system->insert(*mol);
				}
				s = mol;
			}

			s->insert(*residue);
			scene_->getMainControl()->deselectCompositeRecursive(s, true);
			scene_->getMainControl()->selectCompositeRecursive(residue, true);
			scene_->getMainControl()->update(*s);
			scene_->notify(new NewSelectionMessage);
//			setMode(MOVE__MODE);
		}

		void EditMode::changeBondOrder_()
		{
			if (scene_->getCurrentBond() == 0) return;
			scene_->deselect();
			if (scene_->getCurrentBond()->getOrder() == bond_order_) return;

			scene_->getCurrentBond()->setOrder((Bond::BondOrder)bond_order_);
			scene_->getMainControl()->update(*(Atom*)scene_->getCurrentBond()->getFirstAtom(), true);
		}


		void EditMode::setFormalCharge_()
		{
			scene_->deselect();

			QAction* action = dynamic_cast<QAction*>(sender());
			if (action == 0) return;
			String string = ascii(action->text());
			try
			{
				Index fc = string.toInt();
				if (scene_->getCurrentAtom() != 0) {
					scene_->getCurrentAtom()->setFormalCharge(fc);
					getMainControl()->update(*scene_->getCurrentAtom(), true);
				}
			}
			catch(...)
			{
				BALLVIEW_DEBUG;
			}
		}

		void EditMode::deleteCurrentAtomTriggered_()
		{
			if (scene_->getCurrentAtom() == 0) return;
			scene_->getMainControl()->remove(*scene_->getCurrentAtom());
			scene_->setCurrentAtom(0);
		}

		void EditMode::deleteCurrentBondTriggered_()
		{
			if (scene_->getCurrentBond() == 0) return;

			Atom* a1 = (Atom*)scene_->getCurrentBond()->getFirstAtom();
			Atom* a2 = (Atom*)scene_->getCurrentBond()->getSecondAtom();
			a1->destroyBond(*a2);
			a1->deselect();
			a2->deselect();
			scene_->notify(new CompositeMessage(*a1, CompositeMessage::CHANGED_COMPOSITE_HIERARCHY));
			scene_->notify(new CompositeMessage(*a2, CompositeMessage::CHANGED_COMPOSITE_HIERARCHY));
			scene_->notify(new CompositeMessage(*a1, CompositeMessage::DESELECTED_COMPOSITE));
			scene_->notify(new CompositeMessage(*a2, CompositeMessage::DESELECTED_COMPOSITE));
			scene_->setCurrentBond(0);
		}

		void EditMode::atomPropertiesTriggered_()
		{
			if (scene_->getCurrentAtom() == 0) return;

			CompositeProperties as(scene_->getCurrentAtom(), scene_);
			bool apply = as.exec();
			scene_->deselect();

			if (apply) getMainControl()->update(*scene_->getCurrentAtom(), true);
		}

		void EditMode::changeAtomElementTriggered_()
		{
			int atomic_number = scene_->getEditElementType();

			if (scene_->getCurrentAtom() != 0)
			{
				atomic_number = scene_->getCurrentAtom()->getElement().getAtomicNumber();
			}

			PTEDialog pte;
			pte.exec();

			if (scene_->getCurrentAtom() != 0 && scene_->getCurrentAtom()->isSelected())
			{
				atomic_number = scene_->getEditElementType();
				scene_->getCurrentAtom()->setElement(PTE[atomic_number]);
				String new_name = PTE[atomic_number].getSymbol();
				//get the old atom number
				String old_name = scene_->getCurrentAtom()->getName();
				old_name.toUpper();
				new_name += old_name.trimLeft("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
				scene_->getCurrentAtom()->setName(new_name);
				scene_->deselect();
				scene_->getMainControl()->update(*scene_->getCurrentAtom());
			}
		}

		void EditMode::createBond_()
		{
			// this functionality shall be independent from the edit mode
			// check if two atoms are selected
			HashSet<Composite*> selection = scene_->getMainControl()->getSelection();

			// by switching into the edit mode recursive selection
			// has already cleaned up
			Atom*  first_atom = 0;
			Atom* second_atom = 0;

			// case 1: one system with exactly two atoms
			if (selection.size() == 1)
			{
                if (RTTI::isKindOf<AtomContainer>(*selection.begin()))
				{
					AtomContainer* ac = reinterpret_cast<AtomContainer*>(*selection.begin());
					if (ac->countAtoms() == 2)
					{
						AtomIterator atom_it = ac->beginAtom();
						for(; +atom_it; ++atom_it)
						{
							if (!first_atom)
							{
								first_atom = &*atom_it;
							}
							else if (!second_atom)
							{
								second_atom = &*atom_it;
							}
							else
							{
								Log.error() << (String)tr("Internal error! Too many atoms selected.") << std::endl;
							}
						}
					}
					else
					{
						scene_->setStatusbarText(tr("Please select exactly two atoms."), true);
					}
				}
				else
				{
					scene_->setStatusbarText(tr("Please select exactly two atoms."), true);
				}
			}
			// case 2: two selected atoms with unselected in
			//         either distinct atom containers
			//         or with unselected in the same container
			else if (selection.size() == 2)
			{
				HashSet<Composite*>::Iterator it = selection.begin();
				for (; +it; ++it)
				{
                    if (RTTI::isKindOf<Atom>(*it))
					{
						if (!first_atom)
						{
							first_atom = reinterpret_cast<Atom*>(*it);
						}
						else if (!second_atom)
						{
							second_atom = reinterpret_cast<Atom*>(*it);
						}
					}
					// case 3: a single atom in selected atomcontainer
                    else if (RTTI::isKindOf<AtomContainer>(*it))
					{
						AtomContainer* ac = reinterpret_cast<AtomContainer*>(*it);
						if (ac->countAtoms() == 1)
						{
							if (!first_atom)
							{
								first_atom = &*ac->beginAtom();
							}
							else if (!second_atom)
							{
								second_atom = &*ac->beginAtom();
							}
						}
						else
						{
							Log.error() << (String)tr("Scene: Internal error! ") << __LINE__ << std::endl;
						}
					}
				}
			}

			// we found two atoms
			if (first_atom && second_atom)
			{
				// create a bond
				Bond* bond = first_atom->createBond(*second_atom);
				bond->setOrder(Bond::ORDER__SINGLE); //TODO single bond or current edit mode default bond order?

				// if the bond is between two molecules, merge them
				scene_->merge(first_atom, second_atom);

				// update representation
				scene_->getMainControl()->update(*first_atom, true);
				scene_->getMainControl()->update(*second_atom, true);
				scene_->setStatusbarText(tr("Added a bond"));

				// deselect and delete recursively from the selection set
				HashSet<Composite*>::Iterator it = selection.begin();
				for (; +it; ++it)
				{
					if (!(**it).containsSelection()) continue;
					scene_->getMainControl()->deselectCompositeRecursive(*it, true);
					scene_->getMainControl()->update(**it, false);
				}
				first_atom->deselect();
				second_atom->deselect();

				// update representation
				scene_->getMainControl()->update(*first_atom, true);
				scene_->getMainControl()->update(*second_atom, true);
			}
			else
			{
				scene_->setStatusbarText(tr("Please select exactly two atoms."), true);
			}
		}

		void EditMode::createContextMenuEntries_()
		{
			if(!atom_properties_)
			{
				atom_properties_ = new QAction(tr("Atom Properties"), this);
				connect(atom_properties_, SIGNAL(triggered()), this, SLOT(atomPropertiesTriggered_()));
			}

			if(!delete_atom_)
			{
				delete_atom_ = new QAction(tr("Delete Atom"), this);
				connect(delete_atom_, SIGNAL(triggered()), this, SLOT(deleteCurrentAtomTriggered_()));
			}

			if(!charge_menu_)
			{
				charge_menu_ = new QMenu();
				Index charge_value = 0;
				if (scene_->getCurrentAtom() != 0) charge_value = scene_->getCurrentAtom()->getFormalCharge();
				for (Index p = +6; p > -7; p--)
				{
					QString s = QString::number(p);
					if (p > 0) s = QString("+") + s;

					QAction* action = charge_menu_->addAction(s, this, SLOT(setFormalCharge_()));
					action->setCheckable(true);
					action->setChecked(p == charge_value);
				}
			}

			if(!order_menu_)
			{
				order_menu_ = new QMenu();
				connect(order_menu_, SIGNAL(hovered(QAction*)), this, SLOT(activatedOrderItem_(QAction*)));
				order_actions_.push_back(order_menu_->addAction(tr("Single"),    this, SLOT(changeBondOrder_())));
				order_actions_.push_back(order_menu_->addAction(tr("Double"),    this, SLOT(changeBondOrder_())));
				order_actions_.push_back(order_menu_->addAction(tr("Triple"),    this, SLOT(changeBondOrder_())));
				order_actions_.push_back(order_menu_->addAction(tr("Quadruple"), this, SLOT(changeBondOrder_())));
				order_actions_.push_back(order_menu_->addAction(tr("Aromatic"),  this, SLOT(changeBondOrder_())));
				order_actions_.push_back(order_menu_->addAction(tr("Unknown"),   this, SLOT(changeBondOrder_())));
			}

			if(!add_menu_)
			{
				add_menu_ = new QMenu();

				QMenu* rings = new QMenu(add_menu_);
				QAction* ring_action = add_menu_->addMenu(rings);
				ring_action->setText(tr("Aromatic rings"));
				rings->addAction(tr("Pyrrole"), this, SLOT(addStructure_()));
				rings->addAction(tr("Benzene"), this, SLOT(addStructure_()));
				rings->addAction(tr("Indole"), this, SLOT(addStructure_()));

				QMenu* aas = new QMenu(add_menu_);
				QAction* aas_action = add_menu_->addMenu(aas);
				aas_action->setText(tr("Amino acids"));

				QMenu* nas = new QMenu(add_menu_);
				QAction* nas_action = add_menu_->addMenu(nas);
				nas_action->setText(tr("Nucleic acids"));

				HashSet<String> names;
				const std::vector<Residue*>& residues = fragment_db_->getFragments();
				vector<Residue*>::const_iterator rit = residues.begin();
				vector<Residue*> nucleotides;
				for (;rit != residues.end();++rit)
				{
					String name = (**rit).getName();
					if ((**rit).isAminoAcid())
					{
						if (names.has(name)) continue;
						names.insert(name);
						aas->addAction(name.c_str(), this, SLOT(addStructure_()));
						continue;
					}
				}

				nas->addAction(tr("Alanine"),  this, SLOT(addStructure_()));
				nas->addAction(tr("Cytosine"), this, SLOT(addStructure_()));
				nas->addAction(tr("Guanine"),  this, SLOT(addStructure_()));
				nas->addAction(tr("Thymine"),  this, SLOT(addStructure_()));
				nas->addAction(tr("Uracil"),   this, SLOT(addStructure_()));
			}

		}

		void EditMode::checkBondOrderActions_(unsigned int bo)
		{
			for (size_t p = 0; p < order_actions_.size(); p++)
			{
				order_actions_[p]->setCheckable(true);
				order_actions_[p]->setChecked(p + 1 == bo);
			}
		}


	}
}
