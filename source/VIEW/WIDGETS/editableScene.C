// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/WIDGETS/editableScene.h>

#include <BALL/SYSTEM/path.h>
#include <BALL/SYSTEM/systemCalls.h>

#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/bond.h>

#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/compositeManager.h>
#include <BALL/VIEW/KERNEL/shortcutRegistry.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/iconLoader.h>

#include <BALL/VIEW/DIALOGS/compositeProperties.h>
#include <BALL/VIEW/DIALOGS/editSettings.h>
#include <BALL/VIEW/DIALOGS/preferences.h>
#include <BALL/VIEW/DIALOGS/PTEDialog.h>
#include <BALL/VIEW/DIALOGS/assignBondOrderConfigurationDialog.h>

#include <BALL/VIEW/PRIMITIVES/box.h>
#include <BALL/VIEW/PRIMITIVES/line.h>
#include <BALL/VIEW/WIDGETS/molecularStructure.h>

#include <BALL/STRUCTURE/geometricTransformations.h>
#include <BALL/STRUCTURE/addHydrogenProcessor.h>
#include <BALL/STRUCTURE/assignBondOrderProcessor.h>

#include <BALL/QSAR/ringPerceptionProcessor.h>

#include <BALL/MATHS/randomNumberGenerator.h>
#include <BALL/MATHS/analyticalGeometry.h>
#include <BALL/MATHS/vector3.h>
#include <BALL/MATHS/matrix44.h>
#include <BALL/MATHS/angle.h>

#include <QtGui/qmenubar.h>
#include <QtGui/QDesktopWidget>
#include <QtGui/QPainter>
#include <QtGui/QBitmap>

#include <sstream>

using std::endl;

namespace BALL
{
	namespace VIEW
	{

//
//-------------------- EditOperation -----------------------
//

		EditableScene::EditOperation::EditOperation()
			: operationType(),
				atom(),
				bond(),
				description()
		{ 
		}

		EditableScene::EditOperation::EditOperation(Atom* new_atom, Bond* new_bond, String new_description, int new_operation)
			: operationType((EditableScene::EditOperation::OperationType)new_operation),
				atom(new_atom),
				bond(new_bond),
				description(new_description)
		{
		}

		EditableScene::EditOperation::EditOperation(const EditOperation& eOperation)
			: operationType(eOperation.operationType),
				atom(eOperation.atom),
				bond(eOperation.bond),
				description(eOperation.description)
		{	
		}

		EditableScene::EditOperation::~EditOperation()	
		{
#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object EditOperation " << this << " of class EditOperation>" << std::endl;
#endif 
		}

//
//-------------------- EditableScene -----------------------

		bool EditableScene::only_highlighted_ = true;

		EditableScene::EditableScene()
			:	Scene(),
				toolbar_edit_controls_(new QToolBar(tr("Edit Controls"), this))
		{
			init_();
		}

		EditableScene::EditableScene(QWidget* parent_widget, const char* name, Qt::WFlags w_flags)
			: Scene(parent_widget, name, w_flags),
				fragment_db_(),
				fragment_db_initialized_(false),
				toolbar_edit_controls_(new QToolBar(tr("Edit Controls"), this))

		{	
			registerWidget(this); 
			init_();
		}

		// undo_ is NOT copied, since we would run into trouble with the pointers to atoms and bonds it saves
		EditableScene::EditableScene(const EditableScene& eScene, QWidget* parent_widget, const char* name , Qt::WFlags w_flags)
			: Scene(eScene, parent_widget, name, w_flags),
				toolbar_edit_controls_(new QToolBar(tr("Edit Controls"), this))
		{
			init_();
		}

		void EditableScene::init_()
		{
			edit_id_ = 0;
			current_atom_ = 0;
			current_bond_ = 0;
			edit_settings_ = 0;
			bond_order_ = Bond::ORDER__SINGLE;
			atomic_number_ = 6;
			atom_number_ = 0;
			temp_move_ = false;
		}

		void EditableScene::setCursor(String c)
		{
			Path path;
			String filename = path.find("graphics/cursor.png");
			if (filename == "") return;

			QPainter p;
			QImage pm(filename.c_str());
			p.begin(&pm);
			p.setFont(QFont("Arial", 5, QFont::Bold));
			
			QColor color;
			stage_->getBackgroundColor().getInverseColor().get(color);
			color.setAlpha(255);
			p.setRenderHint(QPainter::TextAntialiasing, true);
			p.setRenderHint(QPainter::Antialiasing, true);
			p.setPen(color);
			int text_pos_x = 12;
			int text_pos_y = 22;
			if (c.size()==2)
			{
				text_pos_x = 0;
				text_pos_y = 23;
			}
			p.drawText(text_pos_x, text_pos_y, c.c_str());
			p.end();

			pm.createAlphaMask();
			QCursor cursor(QPixmap::fromImage(pm), 0, 0);
			Scene::setCursor(cursor);
		}

		EditableScene::~EditableScene()
		{
#ifdef BALL_VIEW_DEBUG
			Log.info() << "Destructing object EditableScene " << this << " of class EditableScene>" << std::endl;
#endif 
		}

		void EditableScene::initializeWidget(MainControl& main_control)
		{
			Scene::initializeWidget(main_control);
			String help_url("scene.html#editing");
			
			String description = "Shortcut|Display|Edit_Mode";
			edit_id_ = insertMenuEntry(MainControl::DISPLAY, (String)tr("Edit Mode"), this, 
			                           SLOT(editMode_()), description, QKeySequence("Ctrl+E"));
			setMenuHint((String)tr("Create and modify molecular structures"));	
			edit_id_->setToolTip(tr("Switch to edit mode, e.g. draw your own molecule"));

			edit_id_->setCheckable(true);
			setIcon("actions/mode-edit", false);
			setMenuHelp(help_url);
			mode_group_->addAction(edit_id_);

			main_control.insertPopupMenuSeparator(MainControl::DISPLAY);

			IconLoader& loader = IconLoader::instance();

			description = "Shortcut|QuicklyAssignBondOrders";
			bondorders_action_ = new QAction(loader.getIcon("actions/molecule-assign-bond-orders"), tr("Quickly optimize bond orders"), this);
			bondorders_action_->setObjectName(bondorders_action_->text());
			bondorders_action_->setToolTip(tr("Compute the bond orders of the highlighted structures"));
			//TODO
			//registerForHelpSystem(bondorders_action_, "scene.html#bondorders");
			connect(bondorders_action_, SIGNAL(triggered()), this, SLOT(computeBondOrders()));
			getMainControl()->getShortcutRegistry().registerShortcut(description, bondorders_action_);

			description = "Shortcut|QuicklyOptimizeStructure";
			optimize_action_ = new QAction(loader.getIcon("actions/molecule-minimize"), tr("Quickly optimize structure"), this);
			optimize_action_->setObjectName(optimize_action_->text());
			optimize_action_->setToolTip(tr("Quickly optimize the highlighted structure"));
			registerForHelpSystem(optimize_action_, "scene.html#optimize");
			connect(optimize_action_, SIGNAL(triggered()), this, SLOT(optimizeStructure()));
			getMainControl()->getShortcutRegistry().registerShortcut(description, optimize_action_);

			description = "Shortcut|SaturateWithHydrogens";
			add_hydrogens_action_ = new QAction(loader.getIcon("actions/molecule-add-hydrogens"), tr("Saturate with Hydrogens"), this);
			add_hydrogens_action_->setToolTip(tr("Saturate the highlighted structure with hydrogens (with regards to formal charges)"));
			add_hydrogens_action_->setObjectName(add_hydrogens_action_->text());
			registerForHelpSystem(add_hydrogens_action_, "scene.html#saturate");
			connect(add_hydrogens_action_, SIGNAL(triggered()), this, SLOT(saturateWithHydrogens()));
			getMainControl()->getShortcutRegistry().registerShortcut(description, add_hydrogens_action_);

			getMainControl()->initPopupMenu(MainControl::BUILD)->addAction(add_hydrogens_action_);

			description = "Shortcut|EditMode|SetElement";
			element_action_ = new QAction(loader.getIcon("actions/molecule-set-element"), tr("Set element"), this);
			element_action_->setToolTip(tr("Edit mode: Choose element for next atom, to modify atom under cursor: Double left click"));
			element_action_->setObjectName(element_action_->text());
			registerForHelpSystem(element_action_, "scene.html#choose_element");
			connect(element_action_, SIGNAL(triggered()), this, SLOT(changeElement_()));
			getMainControl()->getShortcutRegistry().registerShortcut(description, element_action_);
			QMenu* qmenu = getMainControl()->initPopupMenu(MainControl::BUILD);
			qmenu->addAction(element_action_);

			description = "Shortcut|EditMode|CreateBond";
			bond_action_ = new QAction(loader.getIcon("actions/create-bond"), tr("Create Bond"), this);
			bond_action_->setToolTip(tr("Edit mode: If two atoms are selected, create a single bond between them"));
			bond_action_->setObjectName(bond_action_->text());
			registerForHelpSystem(bond_action_, "scene.html#create_bond");
			//TODO registerForHelpSystem not done yet
			connect(bond_action_, SIGNAL(triggered()), this, SLOT(createBond_()));
			getMainControl()->getShortcutRegistry().registerShortcut(description, bond_action_);
// 			qmenu = getMainControl()->initPopupMenu(MainControl::BUILD);
			qmenu->addAction(bond_action_);


			//TODO create an icon
			new_molecule_action_ = insertMenuEntry(MainControl::BUILD, (String)tr("Create new molecule"), 
			                                       this, SLOT(createNewMolecule()), "Shortcut|Build|Create_new_molecule");
			setMenuHint((String)tr("Create a new molecule for editing"));

			toolbar_edit_controls_->setObjectName(tr("Edit Control toolbar"));
			toolbar_edit_controls_->setIconSize(QSize(23,23));
			toolbar_edit_controls_->layout()->setMargin(2);
			toolbar_edit_controls_->layout()->setSpacing(2);
		}


		void EditableScene::checkMenu(MainControl& main_control)
		{
			bool busy = main_control.compositesAreLocked();
			edit_id_->setChecked(current_mode_ == (Scene::ModeType)EDIT__MODE);
			edit_id_->setEnabled(!busy);
			Scene::checkMenu(main_control);
			bool edit_mode = (current_mode_ == (Scene::ModeType)EDIT__MODE);
			bool selected_system = !busy && main_control.getSelectedSystem();
			
			optimize_action_->setEnabled(selected_system);
			add_hydrogens_action_->setEnabled(selected_system);
			
			list<Composite*> highl = getMainControl()->getMolecularControlSelection();
			list<Composite*>::iterator lit = highl.begin();
			bool selected_system_or_molecule =   (highl.size() == 1)
			                                  && (RTTI::isKindOf<System>(**lit) || RTTI::isKindOf<Molecule>(**lit) ) ;

			bondorders_action_->setEnabled(selected_system_or_molecule && !busy);
			optimize_action_->setEnabled(selected_system_or_molecule && !busy);
			add_hydrogens_action_->setEnabled(selected_system_or_molecule && !busy);
			
			element_action_->setEnabled(!busy && edit_mode);

			new_molecule_action_->setEnabled(!busy);
		}

		void EditableScene::mousePressEvent(QMouseEvent* e)
		{
			if (current_mode_ != (Scene::ModeType) EDIT__MODE)
			{
				Scene::mousePressEvent(e);
				return;
			}

			if (isAnimationRunning() || getMainControl()->isBusy()) return;

			deselect_();

			x_window_pos_old_ = x_window_pos_new_;
			y_window_pos_old_ = y_window_pos_new_;
			x_window_pos_new_ = e->x();
			y_window_pos_new_ = e->y();
			mouse_button_is_pressed_ = true;
			last_buttons_ = e->buttons();

			if (e->button() != Qt::RightButton)
			{
				if (only_highlighted_ &&
						getMainControl()->getMolecularControlSelection().size() == 0)
				{
					setStatusbarText((String)tr("Warning: no AtomContainer highlighted"), true);
					return;
				}
			}

			getClickedItems_(e->x(), e->y());

			/////////////////////////////////////////
			// right button -> context menu
			if (e->button() == Qt::RightButton)
			{
				if (current_atom_)
				{
					current_atom_->select();
					notify_(new CompositeMessage(*current_atom_, CompositeMessage::SELECTED_COMPOSITE));	
				}

				if (current_bond_)
				{
					current_bond_->select();
					Atom* a1 = (Atom*)current_bond_->getFirstAtom();
					Atom* a2 = (Atom*)current_bond_->getSecondAtom();
					a1->select();
					a2->select();
					notify_(new CompositeMessage(*a1, CompositeMessage::SELECTED_COMPOSITE));	
					notify_(new CompositeMessage(*a2, CompositeMessage::SELECTED_COMPOSITE));	
				}

				// we open a context menu at this point
				showContextMenu(QPoint(e->x(), e->y()));
				return;
			}

			////////////////////////////////////////////////
			// left button -> add atom or move existing atom
			if (e->button() == Qt::LeftButton && e->modifiers() != Qt::ControlModifier)
			{	
				if (current_bond_ != 0) return;

				if (current_atom_ != 0)
				{
					getMainControl()->selectCompositeRecursive(current_atom_, true);
					x_window_pos_new_ = x_window_pos_old_ = e->globalX();
					y_window_pos_new_ = y_window_pos_old_ = e->globalY();
					temp_move_ = true;
					return;
				}

				/////////////////////////////////////////
				// insert a new atom:
				String name = PTE[atomic_number_].getSymbol();
				name += String(atom_number_);
				atom_number_ ++;
				PDBAtom* a = new PDBAtom(PTE[atomic_number_], name);
				insert_(e->x(), e->y(), *a);		
				current_atom_ = a;
				
				//store the Operation in undo_
				Vector3 atom_position = a->getPosition();
				
				EditOperation eo(a, NULL, (String)tr("Added atom of type ") + PTE[atomic_number_].getName() + (String)tr(" at position (") 
												+ String(atom_position.x) + ", "
												+ String(atom_position.y) + ", "
												+ String(atom_position.z) + ")", EditOperation::ADDED__ATOM);
				undo_.push_back(eo);

				// tell about the new undo operation
				emit newEditOperation(eo);
				return;
			}

			//////////////////////////////////////////////
			// middle button -> add bond
			if (e->button() == Qt::MidButton ||
					(e->button() == Qt::LeftButton && e->modifiers() == Qt::ControlModifier))
			{	
				if (current_atom_)
				{
					current_atom_->select(); 
					notify_(new CompositeMessage(*current_atom_, CompositeMessage::SELECTED_COMPOSITE));	
					atom_pos_ = current_atom_->getPosition();
				}

				return;
			}
		}

		void EditableScene::wheelEvent(QWheelEvent* e)
		{
			Index delta = e->delta();
			if (delta == 0) return;

			if (current_mode_ != (Scene::ModeType) EDIT__MODE)
			{
				Scene::wheelEvent(e);
				return;
			}

			e->accept();

			if (isAnimationRunning() || getMainControl()->isBusy()) return;

			if (delta > 1) delta = 1;
			if (delta < -1) delta = -1;

			if (e->modifiers() == Qt::ShiftModifier)
			{
				getClickedItems_(e->x(), e->y());
				changeBondOrder_(delta);
			}
			else
			{
				Scene::wheelEvent(e);
			}
		}

		void EditableScene::changeBondOrder_(Index delta)
		{
			if (current_bond_ == 0) return;

			Index order = current_bond_->getOrder();
			order += delta;
			order = BALL_MAX((Index)Bond::ORDER__SINGLE, order);
			if (order > Bond::ORDER__AROMATIC) order = Bond::ORDER__SINGLE;
			if (current_bond_->getOrder() == order) return;

			current_bond_->setOrder(order);
			getMainControl()->update(*(Atom*)current_bond_->getFirstAtom(), true);
			String txt(tr("Set bond order to "));
			txt += getBondOrderString_(order);
			setStatusbarText(txt);
		}

		// TODO: make the renderer dependent on the target
		void EditableScene::mouseMoveEvent(QMouseEvent *e)
		{
			if (current_mode_ != (Scene::ModeType) EDIT__MODE)
			{
				Scene::mouseMoveEvent(e);
				return;
			}

			if (isAnimationRunning() || getMainControl()->isBusy()) return;

			if (temp_move_)
			{
				x_window_pos_new_ = e->globalX();
				y_window_pos_new_ = e->globalY();

				processMoveModeMouseEvents_(e);

				x_window_pos_old_ = x_window_pos_new_;
				y_window_pos_old_ = y_window_pos_new_;
				return;
			}

			// create a new bond
			//
			// is there an atom nearby the actual mouse position? 
			Atom* last_atom = current_atom_;
			getClickedItems_(e->x(), e->y());

			Vector2 draw_to;

			// have we found such an atom? if so, is it different from the one we started with?
			// (self bonds make no sense)
			if (last_atom && 
					current_atom_ &&
					last_atom != current_atom_ &&
					// workaround against crashes:
					&last_atom->getRoot() == &current_atom_->getRoot())
			{
				// if we are really close to an atom, the endpoints of the line we draw will be set to
				// its center, so that the user has a drop in effect for the bonds
				atom_pos_ = current_atom_->getPosition();
				draw_to   = renderers_[0].map3DToViewport(atom_pos_);
			}
			else
			{
				draw_to.x = e->x();
				draw_to.y = e->y();
			}

			current_atom_ = last_atom;

			updateGL();

			if (current_atom_ == 0)
				return;

			Vector2 draw_from = renderers_[0].map3DToViewport(current_atom_->getPosition());

			// paint the line representing the offered bond
			QPainter p(main_display_);

			QColor color;
			stage_->getBackgroundColor().getInverseColor().get(color);

			QPen pen(color);
			pen.setWidth(3);
			pen.setStyle(Qt::DashLine);

			p.setPen(pen);
			p.setRenderHint(QPainter::Antialiasing, true);
			p.drawLine(QPointF(draw_from.x, draw_from.y), QPointF(draw_to.x, draw_to.y));
			p.end();

			GLRenderWindow* gt = dynamic_cast<GLRenderWindow*>(renderers_[0].target);
			if (gt) gt->swapBuffers();

			x_window_pos_old_ = x_window_pos_new_;
			y_window_pos_old_ = y_window_pos_new_;
			x_window_pos_new_ = e->x();
			y_window_pos_new_ = e->y();
		}

		// TODO: make renderer dependent on current target
		void EditableScene::mouseReleaseEvent(QMouseEvent* e)
		{
			if (temp_move_)
			{
				deselect_();
				temp_move_ = false;
				return;
			}

			if ((int)current_mode_ < (int) EDIT__MODE)
			{
				Scene::mouseReleaseEvent(e);
				return;
			}

			if (last_buttons_ == Qt::RightButton) 
			{
				deselect_();
				return;
			}

			if (isAnimationRunning() || getMainControl()->isBusy()) return;

			mouse_button_is_pressed_ = false;

			// if we didnt find first atom: abort
			if (!current_atom_) 
			{
				deselect_();
				return;
			}
			
			current_atom_->deselect();
			notify_(new CompositeMessage(*current_atom_, CompositeMessage::DESELECTED_COMPOSITE));	

			Atom* atom = current_atom_;
			getClickedItems_(e->x(), e->y());

			// decide what to do... did we find an atom at all?
			if (current_atom_)
			{
				// is it the atom we started with?
				if (atom == current_atom_ || // workaround against crashes:
				                             &atom->getRoot() != &current_atom_->getRoot())
				{
					// in this case, we assume that the user does not want to set a bond
					deselect_();
					return;
				}

				// we found _another_ atom: set the bond
				Bond* c = new Bond("Bond", *current_atom_, *atom, Bond::ORDER__SINGLE);		
				
				EditOperation eo(0, c, "Added bond of type single" , EditOperation::ADDED__BOND);
				undo_.push_back(eo);
			
				// tell about the new undo operation
				emit newEditOperation(eo);

				merge_(current_atom_, atom);

				//update representation
				getMainControl()->update(*atom, true);
				setStatusbarText((String)tr("Added a bond"));
			}
			else // no atom found -> create one
			{
				// project the new atom on the plane of the old atom
				current_atom_ = atom;
				Vector3 new_pos = renderers_[0].mapViewportTo3D(e->x(), e->y());

				// test if the two atoms would have the same position
				if (fabs((current_atom_->getPosition() - new_pos).getLength()) < 0.02)
				{
					setStatusbarText((String)tr("Aborting, since both atoms would have the same location!"), true);
					return;
				}

				// build a new atom...
				String name(PTE[atomic_number_].getSymbol());
				name += String(atom_number_);
				atom_number_++;
				PDBAtom* a = new PDBAtom(PTE[atomic_number_], name);
				a->setPosition(new_pos);
				current_atom_->getParent()->appendChild(*a);
				
				//store the Operation in undo_
				Vector3 atom_position = a->getPosition();

				EditOperation eo(a, NULL, (String)tr("Added atom of type ") + PTE[atomic_number_].getName() + (String)tr(" at position (") 
												+ String(atom_position.x) + ", "
												+ String(atom_position.y) + ", "
												+ String(atom_position.z) + ")", EditOperation::ADDED__ATOM);
				undo_.push_back(eo);

				// tell about the new undo operation
				emit newEditOperation(eo);

				//set the bond
				Bond* c = new Bond("Bond", *current_atom_, *a, Bond::ORDER__SINGLE);		

				// tell about the new undo operation
				String bond_string = getBondOrderString_(bond_order_);
				EditOperation eo2(0, c, (String)tr("Added bond of type ") + bond_string, EditOperation::ADDED__BOND);
				undo_.push_back(eo2);
				emit newEditOperation(eo2);

				getMainControl()->update(*a->getParent(), true);
				setStatusbarText((String)tr("Added a bond and an atom"));
			}

			deselect_();
		}	

		String EditableScene::getBondOrderString_(Index order)
		{
			String bond_string;
			switch (order)
			{
				case Bond::ORDER__SINGLE:
					bond_string = (String)tr("single");
					break;
				case Bond::ORDER__DOUBLE:
					bond_string = (String)tr("double");
					break;
				case Bond::ORDER__TRIPLE:						
					bond_string = (String)tr("triple");	
					break;
				case Bond::ORDER__QUADRUPLE:
					bond_string = (String)tr("quadruple");	
					break;
				case Bond::ORDER__AROMATIC:
					bond_string = (String)tr("aromatic");	
					break;
				default:					
					bond_string = (String)tr("unknown");	
					break;
			}

			return bond_string;
		}

/// ******************** Helper Functions *************************
		list<AtomContainer*> EditableScene::getContainers_()
		{
			list<AtomContainer*> containers;
			if (only_highlighted_)
			{
				list<Composite*> highl = getMainControl()->getMolecularControlSelection();
				list<Composite*>::iterator lit = highl.begin();
				for (; lit != highl.end(); ++lit)
				{
					AtomContainer* ac = dynamic_cast<AtomContainer*>(*lit);
					if (ac != 0) containers.push_back(ac);
				}
			}

			if (containers.size() > 0) return containers;

			HashSet<Composite*> composites = getMainControl()->getCompositeManager().getComposites();
			HashSet<Composite*>::Iterator sit = composites.begin();
			for (; +sit; ++sit)
			{
				AtomContainer* ac = dynamic_cast<AtomContainer*>(*sit);
				if (ac != 0) containers.push_back(ac);
			}

			return containers;
		}

		// TODO: make renderer / target configurable
		void EditableScene::getClickedItems_(int x, int y)
		{
#ifdef ENABLE_RAYTRACING
					Log.info() << "EditableScene::getClickedItems_(): sorry, raytracing does not yet support picking!" << std::endl;
					return;
#endif

			current_bond_ = 0;
			current_atom_ = 0;

			QPoint p(x,y);
			list<GeometricObject*> objects;
			renderers_[0].pickObjects((Position)p.x(), (Position)p.y(),
			                          (Position)p.x(), (Position)p.y(), objects);

			if (objects.size() > 0)
			{
				Composite* c = (Composite*)(**objects.begin()).getComposite();
				if (c == 0) return;

				current_bond_ = dynamic_cast<Bond*>(c);
				current_atom_ = dynamic_cast<Atom*>(c);
			}
		}

		void EditableScene::setElementCursor()
		{
			String s = PTE[atomic_number_].getSymbol();
			setCursor(s.c_str());
		}

		// Slot to change to EDIT__MODE
		// TODO: popup a message box if the currently highlighted atom container does not have a
		//			 suitable representation
		void EditableScene::editMode_()
		{
			if (!fragment_db_initialized_)
			{
				fragment_db_.setFilename("fragments/Editing-Fragments.db");
				fragment_db_.init();
				fragment_db_initialized_ = true;
			}

			list<AtomContainer*> acs = getContainers_();

			list<Composite*> sel;
			list<AtomContainer*>::iterator lit = acs.begin();
			for (; lit != acs.end(); lit++)
			{
				sel.push_back(*lit);
			}
			ControlSelectionMessage* msg = new ControlSelectionMessage();
			msg->setSelection(sel);
			notify_(msg);

			last_mode_ = current_mode_;
			current_mode_ = (Scene::ModeType)EDIT__MODE;		
			edit_id_->setChecked(true);
			setElementCursor();
			checkMenu(*getMainControl());

			HashSet<Composite*> selection = getMainControl()->getSelection();
			HashSet<Composite*>::Iterator it = selection.begin();
			for (; +it; ++it)
			{
				if (!(**it).containsSelection()) continue;
				getMainControl()->deselectCompositeRecursive(*it, true);
				getMainControl()->update(**it, false);
			}
			notify_(new NewSelectionMessage);
		}

		// insert an atom at screen positions (x,y) on the view plane
		// TODO: make the renderer dependent on the current target!
		void EditableScene::insert_(int x, int y, PDBAtom &atom)
		{
			// find the 3D coordinates of screen position (x,y) on the view plane
			// move the atom to that position
			atom.setPosition(renderers_[0].mapViewportTo3D(x,y));

			// now we need to find the AtomContainer into which we will insert the atom.
			// get all highlighted composites
			list<Composite*> composite_list = getMainControl()->getMolecularControlSelection();
			
			Size nr_high = composite_list.size();
			if (nr_high > 1 || (only_highlighted_ && nr_high == 0))
			{
				setStatusbarText((String)tr("Please highlight exactly one AtomContainer for insertion of the created atoms!"), true);
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
						setStatusbarText((String)tr("Please highlight exactly one AtomContainer for insertion of the created atoms!"), true);
						return;
					}
				}

				// prevent adding of atoms to a System: 
				// some forcefields will go havoc otherwise
				if (RTTI::isKindOf<System>(*ai))
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
				getMainControl()->update(*ai, true);
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


// Set the element for the next insert operations
void EditableScene::setEditElementType(int element_number)
{
	atomic_number_ = element_number;
 	setElementCursor();
}

// Get the element for the next insert operations
int EditableScene::getEditElementType()
{
	return atomic_number_;
}

void EditableScene::showContextMenu(QPoint pos)
{
	menu_point_ = pos;
	QMenu menu;
	IconLoader& loader = IconLoader::instance();

	QAction* rotate_mode = menu.addAction(tr("Rotate Mode"), this, SLOT(rotateMode_()));
	rotate_mode->setCheckable(true);
	rotate_mode->setChecked(current_mode_ == (Scene::ModeType) MOVE__MODE);

	QAction* picking_mode = menu.addAction(tr("Picking Mode"), this, SLOT(pickingMode_()));
	picking_mode->setCheckable(true);
	picking_mode->setChecked(current_mode_ == (Scene::ModeType) PICKING__MODE);
	
	QAction* move_mode = menu.addAction(tr("Move Mode"), this, SLOT(moveMode_()));
	move_mode->setCheckable(true);
	move_mode->setChecked(current_mode_ == (Scene::ModeType) MOVE__MODE);
	
	QAction* edit_mode = menu.addAction(tr("Edit Mode"), this, SLOT(editMode_()));
	edit_mode->setCheckable(true);
	edit_mode->setChecked(current_mode_ == (Scene::ModeType) EDIT__MODE);

	menu.addSeparator();

	if (current_mode_ == (Scene::ModeType) EDIT__MODE)
	{
		menu.addAction(tr("Atom Properties"), this, SLOT(atomProperties_()))->setEnabled(current_atom_ != 0);
		menu.addAction(tr("Move Atom"), this, SLOT(moveAtom_()))->setEnabled(current_atom_ != 0);
		menu.addAction(tr("Delete Atom"), this, SLOT(deleteAtom_()))->setEnabled(current_atom_ != 0);
		menu.addAction(loader.getIcon("actions/molecule-set-element"), tr("Change Atom Element"), this, SLOT(changeAtomElement_()))->setEnabled(current_atom_ != 0);

		QMenu* charge = new QMenu();
		QAction* change_charge = menu.addMenu(charge);
		change_charge->setText(tr("Set formal charge"));
		Index charge_value = 0;
		if (current_atom_ != 0) charge_value = current_atom_->getFormalCharge();
		for (Index p = +6; p > -7; p--)
		{
			String s(p);
			if (p > 0) s = String("+") + s;

			QAction* action = charge->addAction(s.c_str(), this, SLOT(setFormalCharge_()));
			action->setCheckable(true);
			action->setChecked(p == charge_value);
		}
		change_charge->setEnabled(current_atom_ != 0);

		menu.addSeparator();

		menu.addAction(tr("Delete Bond"), this, SLOT(deleteBond_()))->setEnabled(current_bond_ != 0);

		QMenu* order = new QMenu();
		QAction* change_order = menu.addMenu(order);
		connect(order, SIGNAL(hovered(QAction*)), this, SLOT(activatedOrderItem_(QAction*)));
		change_order->setText(tr("Change bond order"));
		vector<QAction*> oas;
		oas.push_back(order->addAction(tr("Single"),    this, SLOT(changeBondOrder_())));
		oas.push_back(order->addAction(tr("Double"),    this, SLOT(changeBondOrder_())));
		oas.push_back(order->addAction(tr("Triple"),    this, SLOT(changeBondOrder_())));
		oas.push_back(order->addAction(tr("Quadruple"), this, SLOT(changeBondOrder_())));
		oas.push_back(order->addAction(tr("Aromatic"),  this, SLOT(changeBondOrder_())));
		oas.push_back(order->addAction(tr("Unknown"),   this, SLOT(changeBondOrder_())));

		Index bo = 0;
		if (current_bond_) bo = ((Index)current_bond_->getOrder());
		for (Index p = 0; p < (Index) oas.size(); p++)
		{
			oas[p]->setCheckable(true);
		}

		switch (bo)
		{
			case Bond::ORDER__SINGLE:
				oas[0]->setChecked(true);
				break;
			case Bond::ORDER__DOUBLE:
				oas[1]->setChecked(true);
				break;
			case Bond::ORDER__TRIPLE:
				oas[2]->setChecked(true);
				break;
			case Bond::ORDER__QUADRUPLE:
				oas[3]->setChecked(true);
				break;
			case Bond::ORDER__AROMATIC:
				oas[4]->setChecked(true);
				break;
			default:
				oas[5]->setChecked(true);
				break;
		}

		change_order->setEnabled(current_bond_ != 0);

		menu.addSeparator();

		QMenu* add_menu = new QMenu();
		QAction* add_action = menu.addMenu(add_menu);
		add_action->setText(tr("Add"));
		if (getContainers_().size() == 0)
		{
			add_action->setEnabled(false);
		}

		QMenu* rings = new QMenu();
		QAction* ring_action = add_menu->addMenu(rings);
		ring_action->setText(tr("Aromatic rings"));
		rings->addAction(tr("Pyrrole"), this, SLOT(addStructure_()));
		rings->addAction(tr("Benzene"), this, SLOT(addStructure_()));
		rings->addAction(tr("Indole"), this, SLOT(addStructure_()));

		QMenu* aas = new QMenu();
		QAction* aas_action = add_menu->addMenu(aas);
		aas_action->setText(tr("Amino acids"));

		QMenu* nas = new QMenu();
		QAction* nas_action = add_menu->addMenu(nas);
		nas_action->setText(tr("Nucleic acids"));

		HashSet<String> names;
		const std::vector<Residue*>& residues = fragment_db_.getFragments();
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
		
		nas->addAction(tr("Alanine"), this, SLOT(addStructure_()));
		nas->addAction(tr("Cytosine"), this, SLOT(addStructure_()));
		nas->addAction(tr("Guanine"), this, SLOT(addStructure_()));
		nas->addAction(tr("Thymine"), this, SLOT(addStructure_()));
		nas->addAction(tr("Uracil"), this, SLOT(addStructure_()));

		menu.addSeparator();

		menu.addAction(optimize_action_);
		menu.addAction(add_hydrogens_action_);
		menu.addAction(bondorders_action_);
	}

	menu.exec(mapToGlobal(pos));

	// if we switched to move mode, let the user move the atom:
	if (current_mode_ == MOVE__MODE) return;

	// otherwise deselect all selected items
	deselect_();
}

void EditableScene::setFormalCharge_()
{
	deselect_();

	QObject* os = sender();
	if (os == 0) return;
	QAction* action = dynamic_cast<QAction*>(os);
	if (action == 0) return;
	String string = ascii(action->text());
	try
	{
		Index fc = string.toInt();
		if (current_atom_ != 0) current_atom_->setFormalCharge(fc);
		getMainControl()->update(*current_atom_, true);
	}
	catch(...)
	{
		BALLVIEW_DEBUG;
	}
}

void EditableScene::deselect_(bool update)
{
	bool was_mt = getMainControl()->useMultithreading();
	getMainControl()->setMultithreading(false);
	if (current_bond_ != 0 && 
			(current_bond_->isSelected() ||
			 current_bond_->getFirstAtom()->isSelected() ||
			 current_bond_->getSecondAtom()->isSelected()))
	{
		current_bond_->deselect();
		Atom* a1 = (Atom*)current_bond_->getFirstAtom();
		Atom* a2 = (Atom*)current_bond_->getSecondAtom();
		a1->deselect();
		a2->deselect();
		if (update)
		{
			notify_(new CompositeMessage(*a1, CompositeMessage::DESELECTED_COMPOSITE));	
			notify_(new CompositeMessage(*a2, CompositeMessage::DESELECTED_COMPOSITE));	
		}
	}

	if (current_atom_!= 0 && current_atom_->isSelected())
	{
		current_atom_->deselect();
		if (update)
		{
			notify_(new CompositeMessage(*current_atom_, CompositeMessage::DESELECTED_COMPOSITE));	
		}
	}
	getMainControl()->setMultithreading(was_mt);
}

void EditableScene::deleteAtom_()
{
	if (current_atom_ == 0) return;
	getMainControl()->remove(*current_atom_);
	current_atom_ = 0;
}

void EditableScene::changeElement_()
{
	if (current_atom_ != 0)
	{
		atomic_number_ = current_atom_->getElement().getAtomicNumber();
	}

	PTEDialog pte;
	pte.exec();
}

void EditableScene::changeAtomElement_()
{
	if (current_atom_ != 0)
	{
		atomic_number_ = current_atom_->getElement().getAtomicNumber();
	}

	PTEDialog pte;
	pte.exec();

	if ((current_atom_ != 0))
	{
		current_atom_->setElement(PTE[atomic_number_]);
		String new_name = PTE[atomic_number_].getSymbol();
		//get the old atom number
		String old_name = current_atom_->getName();
		old_name.toUpper();
		new_name += old_name.trimLeft("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
		current_atom_->setName(new_name);
		deselect_();
		getMainControl()->update(*current_atom_);
	} 
}

void EditableScene::createBond_()
{
	// this functionaltiy shall be independent from the edit mode	
	// check if two atoms are selected
	HashSet<Composite*> selection = getMainControl()->getSelection();

	// by switching into the edit mode recursive selection 
	// has already cleaned up
	Atom*  first_atom = 0;
	Atom* second_atom = 0;

	// case 1: one system with exactly two atoms 
	if (selection.size() == 1)
	{
		if (RTTI::isKindOf<AtomContainer>(**selection.begin()))
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
						Log.error() << (String)tr("Internal error! Too many atoms selected.") << endl; 
					}
				}
			}
			else 
			{
				setStatusbarText((String)tr("Please select exactly two atoms."), true);
			}
		}
		else 
		{
			setStatusbarText((String)tr("Please select exactly two atoms."), true);
		}
	}
	// case 2: two selected atoms with unselected in 
	//             either distinct atom containers 
	//             or with unselected in the same container
	else if (selection.size() == 2)
	{
		HashSet<Composite*>::Iterator it = selection.begin();
		for (; +it; ++it)
		{
			if (RTTI::isKindOf<Atom>(**it))
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
			else if (RTTI::isKindOf<AtomContainer>(**it))
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
					Log.error() << (String)tr("EditableScene: Internal error! ") << __LINE__ << endl; 
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

		// 		TODO:for undo -operation
		// 		EditOperation eo(0, bond, "Added bond of type single" , EditOperation::ADDED__BOND);
		// 		undo_.push_back(eo);
		// 
		// 		// tell about the new undo operation
		// 		emit newEditOperation(eo);

		// if the bond is between two molecules, merge them
		merge_(first_atom, second_atom);

		// update representation
		getMainControl()->update(*first_atom, true);
		getMainControl()->update(*second_atom, true);
		setStatusbarText((String)tr("Added a bond"));

		// deselect and delete recursively from the selection set
		HashSet<Composite*>::Iterator it = selection.begin();
		for (; +it; ++it)
		{
			if (!(**it).containsSelection()) continue;
			getMainControl()->deselectCompositeRecursive(*it, true);
			getMainControl()->update(**it, false);
		}
		first_atom->deselect();
		second_atom->deselect();

		// update representation
		getMainControl()->update(*first_atom, true);
		getMainControl()->update(*second_atom, true);
	}
	else
	{		
		setStatusbarText((String)tr("Please select exactly two atoms."), true);
	}
}

void EditableScene::deleteBond_()
{
	if (current_bond_ == 0) return;

	Atom* a1 = (Atom*)current_bond_->getFirstAtom();
	Atom* a2 = (Atom*)current_bond_->getSecondAtom();
	a1->destroyBond(*a2);
	a1->deselect();
	a2->deselect();
	notify_(new CompositeMessage(*a1, CompositeMessage::CHANGED_COMPOSITE_HIERARCHY));	
	notify_(new CompositeMessage(*a2, CompositeMessage::CHANGED_COMPOSITE_HIERARCHY));	
	notify_(new CompositeMessage(*a1, CompositeMessage::DESELECTED_COMPOSITE));	
	notify_(new CompositeMessage(*a2, CompositeMessage::DESELECTED_COMPOSITE));	
	current_bond_ = 0;
}

void EditableScene::changeBondOrder_()
{
	if (current_bond_ == 0) return;
	deselect_();
	if (current_bond_->getOrder() == bond_order_) return;

	current_bond_->setOrder((Bond::BondOrder)bond_order_);
	getMainControl()->update(*(Atom*)current_bond_->getFirstAtom(), true);
}

void EditableScene::onNotify(Message *message)
{
	if ((current_atom_ != 0 || current_bond_ != 0) &&
			RTTI::isKindOf<CompositeMessage>(*message))
	{
		CompositeMessage* composite_message = RTTI::castTo<CompositeMessage>(*message);
		if (composite_message->getType() == CompositeMessage::REMOVED_COMPOSITE)
		{
			current_atom_ = 0;
			current_bond_ = 0;
		}

		if (composite_message->getType() == CompositeMessage::CHANGED_COMPOSITE_HIERARCHY)
		{
			current_atom_ = 0;
			current_bond_ = 0;
		}
	}

	if (RTTI::isKindOf<ControlSelectionMessage>(*message))
	{
		checkMenu(*getMainControl());
		return;
	}

	Scene::onNotify(message);
}

void EditableScene::activatedOrderItem_(QAction* action)
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

void EditableScene::moveAtom_()
{
	if (current_atom_ == 0) return;

	current_atom_->setSelected(true);
	notify_(new CompositeMessage(*current_atom_, CompositeMessage::SELECTED_COMPOSITE));	
	setMode(MOVE__MODE);
}

void EditableScene::atomProperties_()
{
	if (current_atom_ == 0) return;

	CompositeProperties as(current_atom_, this);
	bool apply = as.exec();
	deselect_();

	if (apply) getMainControl()->update(*current_atom_, true);
}

void EditableScene::createMolecule_()
{
	System *system = new System();
	Molecule* current_molecule = new Molecule();
	system->insert(*current_molecule);
	getMainControl()->insert(*system);

	ControlSelectionMessage* nsm =  new ControlSelectionMessage();
	list<Composite*> selection;
	selection.push_back(current_molecule);
	nsm->setSelection(selection);
	notify_(nsm);
}

void EditableScene::setMode(ModeType mode)
{
	Scene::setMode(mode);

	if (mode == (Scene::ModeType) EDIT__MODE) editMode_();
	update();
}

void EditableScene::applyPreferences()
{
	Scene::applyPreferences();
	if (edit_settings_ == 0) return;
	only_highlighted_ = edit_settings_->only_highlighted->isChecked();

	if ((int)current_mode_ == (int)EDIT__MODE)
	{
		setElementCursor();
	}
}

void EditableScene::initializePreferencesTab(Preferences& preferences)
{
	Scene::initializePreferencesTab(preferences);
	edit_settings_ = new EditSettings(this);
	preferences.insertEntry(edit_settings_);
}

void EditableScene::finalizePreferencesTab(Preferences& preferences)
{
	Scene::finalizePreferencesTab(preferences);
	if (edit_settings_) 
	{
		preferences.removeEntry(edit_settings_);
		edit_settings_ = 0;
	}
}

void EditableScene::keyPressEvent(QKeyEvent* e)
{
	if (!reactToKeyEvent_(e))
	{
		Scene::keyPressEvent(e);

		// TODO QShortcut* shortcut 
		if (e->key() == Qt::Key_E)
		{
			setMode((Scene::ModeType)EDIT__MODE);
			return;
		}

	}
}

bool EditableScene::reactToKeyEvent_(QKeyEvent* e)
{
	int key = e->key();
	
	// TODO QShortcut* shortcut 
	if (key == Qt::Key_E)
	{
		setMode((ModeType)EDIT__MODE);
		return true;
	}

	if (current_mode_ != (ModeType)EDIT__MODE) return false;

	if (!getMainControl()->isBusy())
	{
		QPoint point = mapFromGlobal(QCursor::pos());
		
		// TODO QShortcut* shortcut 
		if (key == Qt::Key_D)
		{
			getClickedItems_(point.x(), point.y());
			deleteAtom_();
			return true;
		}
		
		// TODO QShortcut* shortcut 
		if (key == Qt::Key_Backspace)
		{
			getClickedItems_(point.x(), point.y());
			deleteBond_();
			return true;
		}
	}

	// TODO QShortcut* shortcut 
	if (key < Qt::Key_A || key > Qt::Key_Z)
	{
		return false;
	}

	// TODO QShortcut* shortcut 
	if      (key == Qt::Key_H) atomic_number_ = 1;
	else if (key == Qt::Key_C) atomic_number_ = 6;
	else if (key == Qt::Key_N) atomic_number_ = 7;
	else if (key == Qt::Key_O) atomic_number_ = 8;
	else if (key == Qt::Key_P) atomic_number_ = 15;
	else if (key == Qt::Key_S) atomic_number_ = 16;
	else
	{
		return false;
	}

	setElementCursor();

	String text(tr("Setting element to "));
	text += PTE[atomic_number_].getName();
	setStatusbarText(text);

	return true;
}

// TODO: make the renderer dependent on the current target!
void EditableScene::addStructure(String name)
{
	deselect_();

	if (!fragment_db_initialized_)
	{
		fragment_db_.setFilename("fragments/Editing-Fragments.db");
		fragment_db_.init();
		fragment_db_initialized_ = true;
	}

	list<AtomContainer*> containers = getContainers_();
	if (containers.size() == 0) return;

	Residue* residue = fragment_db_.getResidueCopy(name);
	if (residue == 0)
	{
		residue = fragment_db_.getResidueCopy(name + "-Skeleton");
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

	Matrix4x4 m;
	Vector3 x = renderers_[0].mapViewportTo3D(menu_point_.x(), menu_point_.y());
	TransformationProcessor tf;

	Vector3 vv = getStage()->getCamera().getViewVector();
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
	if (RTTI::isKindOf<System>(*s))
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
	getMainControl()->deselectCompositeRecursive(s, true);
	getMainControl()->selectCompositeRecursive(residue, true);
	getMainControl()->update(*s);
	notify_(new NewSelectionMessage);
	setMode(MOVE__MODE);
}

void EditableScene::addStructure_()
{
	QObject* os = sender();
	if (os == 0) return;
	QAction* action = dynamic_cast<QAction*>(os);
	if (action == 0) return;
	addStructure(ascii(action->text()));
}

void EditableScene::createNewMolecule()
{
	if (getMainControl()->isBusy()) return;

	System* s = new System();
	Molecule* m = new Molecule();
	s->insert(*m);
	getMainControl()->insert(*s);
	getMainControl()->update(*s);
	ControlSelectionMessage* msg = new ControlSelectionMessage();
	list<Composite*> sel;
	sel.push_back(m);
	msg->setSelection(sel);
	notify_(msg);

	editMode_();
}

void EditableScene::saturateWithHydrogens()
{
	if (getMainControl()->isBusy()) return;

	deselect_(false);
	list<AtomContainer*> containers = getContainers_();
	if (containers.size() < 1) return;
	AtomContainer* ac = *containers.begin();
	RingPerceptionProcessor rpp;
	vector<vector<Atom*> > rings;
	rpp.calculateSSSR(rings, *ac);
	rings = rpp.getAllSmallRings();

	AddHydrogenProcessor ahp;
	ahp.setRings(rings);
	ac->apply(ahp);
	String nr = ahp.getNumberOfAddedHydrogens();
	setStatusbarText((String)tr("Added ") + nr + (String)tr(" hydrogens."), true);
	getMainControl()->update(*ac, true);
}


void EditableScene::computeBondOrders()
{
	if (getMainControl()->isBusy()) return;

	System* system = getMainControl()->getSelectedSystem();  
	if (system == 0) { return;};              

	deselect_(false);

	// do we have a Molecular Structure?
	MolecularStructure* ms = MolecularStructure::getInstance(0);
	if (ms == 0) return;

	// get the highlighted atomcontainer
	list<AtomContainer*> containers;
	if (only_highlighted_)
	{
		list<Composite*> highl = getMainControl()->getMolecularControlSelection();
		list<Composite*>::iterator lit = highl.begin();
		for (; lit != highl.end(); ++lit)
		{
			AtomContainer* ac = dynamic_cast<AtomContainer*>(*lit);
			if (ac != 0) containers.push_back(ac);
		}
	}

	if (containers.size() != 1) 
	{
		setStatusbarText((String)tr("Please highlight exactly one AtomContainer!"), true);
		return;
	}

	AssignBondOrderConfigurationDialog& bond_order_dialog = ms->getBondOrderDialog();

	AssignBondOrderProcessor abop;

	// read the options from the dialog
	// bond_order_dialog.setOptionsForProcessor(abop);
	abop.options[AssignBondOrderProcessor::Option::OVERWRITE_SINGLE_BOND_ORDERS] = bond_order_dialog.overwrite_singleBO_box->isChecked();
	abop.options[AssignBondOrderProcessor::Option::OVERWRITE_DOUBLE_BOND_ORDERS] = bond_order_dialog.overwrite_doubleBO_box->isChecked();
	abop.options[AssignBondOrderProcessor::Option::OVERWRITE_TRIPLE_BOND_ORDERS] = bond_order_dialog.overwrite_tripleBO_box->isChecked();
	abop.options[AssignBondOrderProcessor::Option::OVERWRITE_SELECTED_BONDS]     = bond_order_dialog.overwrite_selected_bonds_box->isChecked();
	abop.options[AssignBondOrderProcessor::Option::KEKULIZE_RINGS]               = bond_order_dialog.kekulizeBonds_button->isChecked();
	abop.options[AssignBondOrderProcessor::Option::ADD_HYDROGENS]                = bond_order_dialog.add_hydrogens_checkBox->isChecked();
	abop.options[AssignBondOrderProcessor::Option::ALGORITHM] = bond_order_dialog.ILP_button->isChecked() ? AssignBondOrderProcessor::Algorithm::ILP 
	                                                                                                     : AssignBondOrderProcessor::Algorithm::A_STAR;
	abop.options[AssignBondOrderProcessor::Option::BOND_LENGTH_WEIGHTING]        = (bond_order_dialog.penalty_balance_slider->value()/100.);
	
	// get the parameter folder
	abop.options[AssignBondOrderProcessor::Option::INIFile] = ascii(bond_order_dialog.parameter_file_edit->text());

	// check for valid inputs
	if (bond_order_dialog.max_n_opt_solutions->text().toInt() < 1)
		bond_order_dialog.max_n_opt_solutions->setText(String(1).c_str());

	if (bond_order_dialog.max_n_all_solutions->text().toInt() < 1)
		bond_order_dialog.max_n_all_solutions->setText(String(1).c_str());

	// get the limitations for number of bond order assignment
	if (bond_order_dialog.single_solution_button->isChecked())
	{
		abop.options[AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS]            = 1;
		abop.options[AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS] = false;
	}
	else if (bond_order_dialog.all_optimal_solutions_button->isChecked())
	{
		abop.options[AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS]            = 0;
		abop.options[AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS] = false;
	}
	else if (bond_order_dialog.n_opt_solutions_button->isChecked())
	{
		abop.options[AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS]            = bond_order_dialog.max_n_opt_solutions->text().toInt();
		abop.options[AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS] = false;
	}
	else if (bond_order_dialog.n_all_solutions_button->isChecked())
	{
		abop.options[AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS]            = bond_order_dialog.max_n_all_solutions->text().toInt();
		abop.options[AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS] = true;
	}

	// automatically applying a solution might confuse the user --> set to false
	abop.options.setBool(AssignBondOrderProcessor::Option::APPLY_FIRST_SOLUTION, false);

	// apply
	containers.front()->apply(abop);

	// give a message
	if (abop.getNumberOfComputedSolutions() == 0)
	{
		setStatusbarText((String)tr("Could not find a valid bond order assignment."), true);
	}
	else
	{	
		String nr = abop.getNumberOfComputedSolutions();
		setStatusbarText((String)tr("Found ") + nr + (String)tr(" bond order assignments."), true);
	
		Log.info()<< (String)tr("  > Result AssignBondOrderProcessor: ") << endl;

		for (Size i = 0; i < abop.getNumberOfComputedSolutions(); i++)
		{
			ostringstream stream_description;
			stream_description.setf(std::ios_base::fixed);
			stream_description.precision(2);

			stream_description  << (String)tr("      Solution ") << i 
			                    << (String)tr(" has penalty ") << abop.getTotalPenalty(i);
			//                  << ", charge " << abop.getTotalCharge(i)
			//                  << ", " <<  abop.getNumberOfAddedHydrogens(i) << " added hydrogens.";
 
			String description = stream_description.str();

			Log.info() << description << endl; 
		}

		ms->showBondOrderAssignmentResults(abop);
	}

	getMainControl()->update(*containers.front(), true);
}


void EditableScene::optimizeStructure()
{
	if (getMainControl()->isBusy()) return;

	deselect_();
	list<AtomContainer*> containers = getContainers_();
	if (containers.size() < 1) return;

	MolecularStructure* ms = MolecularStructure::getInstance(0);
	if (ms == 0) return;
	
	AtomContainer* ac = *containers.begin();
	System* system = (System*)&ac->getRoot();

	setStatusbarText((String)tr("Optimizing Structure..."), true);

	// highlight System for minimization
	ControlSelectionMessage* nsm =  new ControlSelectionMessage();
	list<Composite*> selection;
	selection.push_back(system);
	nsm->setSelection(selection);
	notify_(nsm);

	float range = 0.05;
	RandomNumberGenerator rng;
	rng.setup();
	AtomIterator ait = ac->beginAtom();
	for (; +ait; ++ait)
	{
		ait->getPosition() += Vector3(rng.randomDouble(-range, range),
          	                              rng.randomDouble(-range, range),
		                              rng.randomDouble(-range, range));
	}
	ms->chooseMMFF94();

	MinimizationDialog& md = ms->getMinimizationDialog();
	md.storeValues();
	md.setMaxIterations(30);
	md.setRefresh(25);
	md.setMaxGradient(5);
	ms->runMinimization(false);

	while (getMainControl()->isBusy())
	{
		getMainControl()->processEvents(999);
		sleepFor(40);
	}

	MolecularDynamicsDialog& mdd = ms->getMDSimulationDialog();
	mdd.storeValues();
	mdd.setTemperature(500);
	mdd.setNumberOfSteps(50);
	mdd.setMicroCanonical(false);
	mdd.setDCDFile("");
	ms->MDSimulation(false);

	while (getMainControl()->isBusy())
	{
		getMainControl()->processEvents(999);
		sleepFor(40);
	}

	md.setMaxIterations(200);
	md.setMaxGradient(1);
	ms->runMinimization(false);

	md.restoreValues();
	mdd.restoreValues();
}

void EditableScene::addToolBarEntries(QToolBar* tb)
{
	Scene::addToolBarEntries(tb);
	
	toolbar_actions_edit_controls_.push_back(edit_id_);
	toolbar_actions_edit_controls_.push_back(element_action_);
	toolbar_actions_edit_controls_.push_back(bond_action_);
	toolbar_actions_edit_controls_.push_back(add_hydrogens_action_);
	toolbar_actions_edit_controls_.push_back(optimize_action_);
	toolbar_actions_edit_controls_.push_back(bondorders_action_);	
	//TODO make icon
	//toolbar_actions_edit_controls_.push_back(new_molecule_action_);
	
	toolbar_edit_controls_->addActions(toolbar_actions_edit_controls_);
	getMainControl()->addToolBar(Qt::TopToolBarArea, toolbar_edit_controls_);
	ModularWidget::addToolBarEntries(tb);
	getMainControl()->initPopupMenu(MainControl::WINDOWS)->addAction(toolbar_edit_controls_->toggleViewAction());

	// workaround for Qt, when it only would show the first item:
	//	toolbar_view_controls_->hide();
	//	toolbar_view_controls_->show();
	toolbar_edit_controls_->hide();
	toolbar_edit_controls_->show();
}

void EditableScene::mouseDoubleClickEvent(QMouseEvent* e)
{
	if (current_mode_ != (ModeType) EDIT__MODE) 
	{
		Scene::mouseDoubleClickEvent(e);
		return;
	}

	getClickedItems_(e->x(), e->y());
	if (current_atom_ != 0)
	{
		current_atom_->setElement(PTE[atomic_number_]);	
		String new_name = PTE[atomic_number_].getSymbol();
		//get the old atom number
		String old_name = current_atom_->getName();
		old_name.toUpper();
		new_name += old_name.trimLeft("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
		current_atom_->setName(new_name);
		deselect_();
		getMainControl()->update(*current_atom_);
		return;
	}

	if (current_bond_ != 0)
	{
		Atom* a1 = (Atom*)current_bond_->getFirstAtom();
		Atom* a2 = (Atom*)current_bond_->getSecondAtom();
		
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
			changeBondOrder_(1);
		}
	
		getMainControl()->update(*a1->getParent(), true);
	}
}

void EditableScene::merge_(Composite* a1, Composite* a2)
{
	if ( !a1 || !a2 || !a1->getParent() || !a2->getParent() )
	{
	 	return;
	}

	System* s2 = (System*)&a2->getRoot();

	Composite* p1 = a1->getParent();
	Composite* p2 = a2->getParent();

	Size silb1 = p1->getDegree();
	Size silb2 = p2->getDegree();
	
	if (silb1 == 1)
	{
		p2->appendChild(*a1);
	}
	else if (silb2 == 1)
	{
		p1->appendChild(*a2);
	}

	Molecule dummy_mol;
	Molecule* m1 = a1->getAncestor(dummy_mol);
	Molecule* m2 = a2->getAncestor(dummy_mol);

	if (!m1 || !m2)
	{	
		Log.error() << (String)tr("Internal error! ") << __FILE__ << " " << __LINE__ << endl; 
		return;
	}

	Composite* anchestor = a1->getLowestCommonAncestor(*a2);
	if (!anchestor)
	{
 		m1->spliceBefore(*m2);
		getMainControl()->remove(*s2);
		return;
	}

	if (m1 == p1)
	{
		p2->appendChild(*a1);
	}
	else if (m2 == p2)
	{
		p1->appendChild(*a2);
	}
	else
	{
		Log.error() << (String)tr("Internal error! ") << __FILE__ << " " << __LINE__ << endl; 
	}
	
}

	}//end of namespace 
} //end of namespace
