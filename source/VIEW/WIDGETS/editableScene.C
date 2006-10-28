// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: editableScene.C,v 1.20.2.54 2006/10/28 20:06:44 amoll Exp $
//

#include <BALL/VIEW/WIDGETS/editableScene.h>

#include <BALL/SYSTEM/path.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/compositeManager.h>
#include <BALL/VIEW/DIALOGS/compositeProperties.h>
#include <BALL/VIEW/DIALOGS/editSettings.h>
#include <BALL/VIEW/DIALOGS/preferences.h>
#include <BALL/VIEW/DIALOGS/PTEDialog.h>
#include <BALL/VIEW/PRIMITIVES/box.h>
#include <BALL/VIEW/PRIMITIVES/line.h>
#include <BALL/VIEW/WIDGETS/molecularStructure.h>
#include <BALL/STRUCTURE/geometricTransformations.h>
#include <BALL/STRUCTURE/addHydrogenProcessor.h>
#include <BALL/QSAR/ringPerceptionProcessor.h>
#include <BALL/MATHS/randomNumberGenerator.h>

#include <QtGui/qmenubar.h>
#include <QtGui/QDesktopWidget>
#include <QtGui/QPainter>

#include <BALL/MATHS/vector3.h>
#include <BALL/MATHS/matrix44.h>
#include <BALL/MATHS/angle.h>

using std::endl;

namespace BALL
{
	namespace VIEW
	{

//
//-------------------- EditOperation -----------------------
//

EditableScene::EditOperation::EditOperation()
	throw()
	: operationType(),
		atom(),
		bond(),
		description()
{ 
}

EditableScene::EditOperation::EditOperation(Atom* new_atom, Bond* new_bond, String new_description, int new_operation)
	throw()
	: operationType((EditableScene::EditOperation::OperationType)new_operation),
		atom(new_atom),
		bond(new_bond),
		description(new_description)
{
}

EditableScene::EditOperation::EditOperation(const EditOperation& eOperation)
	throw()
	: operationType(eOperation.operationType),
		atom(eOperation.atom),
		bond(eOperation.bond),
		description(eOperation.description)
{	
}

EditableScene::EditOperation::~EditOperation()	
	throw()
{
#ifdef BALL_VIEW_DEBUG
		Log.info() << "Destructing object EditOperation " << this << " of class EditOperation>" << std::endl;
#endif 
}

//
//-------------------- EditableScene -----------------------

float EditableScene::atom_limit_ = 1.;
float EditableScene::bond_limit_ = 1.0;
bool EditableScene::only_highlighted_ = true;

EditableScene::EditableScene()
	throw()
	:	Scene()
{
	init_();
}

EditableScene::EditableScene(QWidget* parent_widget, const char* name, Qt::WFlags w_flags)
	throw()
	: Scene(parent_widget, name, w_flags),
	  fragment_db_(),
		fragment_db_initialized_(false)
{	
	registerWidget(this); 
	init_();
}

// undo_ is NOT copied, since we would run into trouble with the pointers to atoms and bonds it saves
EditableScene::EditableScene(const EditableScene& eScene, QWidget* parent_widget, const char* name , Qt::WFlags w_flags)
	throw()
	: Scene(eScene, parent_widget, name, w_flags)
{
	init_();
}

void EditableScene::init_()
{
	edit_id_ = 0;
	current_atom_ = 0;
	edit_settings_ = 0;
	bond_order_ = Bond::ORDER__SINGLE;
	draw_line_ = 0;
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
	QPixmap pm(filename.c_str());
	p.begin(&pm);
		QFont font;
		font.setPixelSize(9);
		p.setFont(font);
		QColor color;
		stage_->getBackgroundColor().getInverseColor().get(color);
   	p.setPen(color);
  	p.drawText(13, 11, c.c_str());
	p.end();

	QCursor cursor(pm, 0, 0);
	QWidget::setCursor(cursor);
}

EditableScene::~EditableScene()
	throw()
{
#ifdef BALL_VIEW_DEBUG
	Log.info() << "Destructing object EditableScene " << this << " of class EditableScene>" << std::endl;
#endif 
}


void EditableScene::initializeWidget(MainControl& main_control)
	throw()
{
	Scene::initializeWidget(main_control);
	String help_url("scene.html#editing");

	edit_id_ = insertMenuEntry(MainControl::DISPLAY, "Edit Mode", 
										this, SLOT(editMode_()), Qt::CTRL+Qt::Key_E);
	edit_id_->setCheckable(true);
	setIcon(String("edit.png"), false);
	setMenuHint("Create and modify molecular structures");
	setMenuHelp(help_url);
	mode_group_->addAction(edit_id_);

	new_molecule_ = insertMenuEntry(MainControl::BUILD, "Create new molecule", 
										this, SLOT(createNewMolecule()));
 	setMenuHint("Create a new molecule for editing");

	Path path;
	QIcon icon(path.find("graphics/minimize.png").c_str());
	optimize_ = new QAction(icon, "Quickly optimize structure", this);
	connect(optimize_, SIGNAL(triggered()), this, SLOT(optimizeStructure()));

	QIcon icon2(path.find("graphics/hydrogens.png").c_str());
	add_hydrogens_ = new QAction(icon2, "Saturate with hydrogens", this);
	connect(add_hydrogens_, SIGNAL(triggered()), this, SLOT(addHydrogens()));

	QIcon icon3(path.find("graphics/element.png").c_str());
	element_action_ = new QAction(icon3, "Set element", this);
	connect(element_action_, SIGNAL(triggered()), this, SLOT(changeElement_()));
}


void EditableScene::checkMenu(MainControl& main_control)
	throw()
{
	bool busy = main_control.compositesAreLocked();
	edit_id_->setChecked(current_mode_ == (Scene::ModeType)EDIT__MODE);
	edit_id_->setEnabled(!busy);
	Scene::checkMenu(main_control);
	optimize_->setEnabled(!busy);
	add_hydrogens_->setEnabled(!busy);
	element_action_->setEnabled(!busy);

	new_molecule_->setEnabled(!busy);
}

void EditableScene::mousePressEvent(QMouseEvent* e)
{
	if (current_mode_ < (Scene::ModeType) EDIT__MODE)
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
			setStatusbarText("Warning: no AtomContainer highlighted", true);
			return;
		}
	}

	if (e->button() == Qt::LeftButton)
	{	
		current_atom_ = getClickedAtom_(e->x(), e->y());
		if (current_atom_ != 0)
		{
			getMainControl()->selectCompositeRecursive(current_atom_, true);
			x_window_pos_new_ = x_window_pos_old_ = e->globalX();
			y_window_pos_new_ = y_window_pos_old_ = e->globalY();
			temp_move_ = true;
			return;
		}

		// insert a new atom:
		String name = PTE[atomic_number_].getSymbol();
		name += String(atom_number_);
		atom_number_ ++;
		PDBAtom* a = new PDBAtom(PTE[atomic_number_], name);
		insert_(e->x(), e->y(), *a);		
		current_atom_ = a;
		
		//store the Operation in undo_
		Vector3 atom_position = a->getPosition();
		draw_line_ = false;
		
		EditOperation eo(a, NULL, "Added atom of type " + PTE[atomic_number_].getName() + " at position (" 
										+ String(atom_position.x) + ", "
										+ String(atom_position.y) + ", "
										+ String(atom_position.z) + ")", EditOperation::ADDED__ATOM);
		undo_.push_back(eo);

		// tell about the new undo operation
		emit newEditOperation(eo);
		return;
	}

	current_atom_ = getClickedAtom_(e->x(), e->y());

	if (e->button() == Qt::MidButton)
	{	
		if (current_atom_)
		{
			current_atom_->select(); 
			notify_(new CompositeMessage(*current_atom_, CompositeMessage::SELECTED_COMPOSITE));	
			atom_pos_ = current_atom_->getPosition();
			draw_line_ = false;
		}

		return;
	}

	// find an atom in a radius of limit_ around the current mouse position
	if (current_atom_)
	{
		current_atom_->select(); 
		notify_(new CompositeMessage(*current_atom_, CompositeMessage::SELECTED_COMPOSITE));	
		current_bond_ = 0;
	}
	
	// try to find a bond
	Bond* bond = getClickedBond_(e->x(), e->y());

	if (bond)
	{
		bond->select();
		Atom* a1 = (Atom*)bond->getFirstAtom();
		Atom* a2 = (Atom*)bond->getSecondAtom();
		a1->select();
		a2->select();
		notify_(new CompositeMessage(*a1, CompositeMessage::SELECTED_COMPOSITE));	
		notify_(new CompositeMessage(*a2, CompositeMessage::SELECTED_COMPOSITE));	
		current_bond_ = bond;
	}

	if (e->button() == Qt::RightButton)
	{	
		// we open a context menu at this point
		showContextMenu(QPoint(e->x(), e->y()));
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
		current_bond_ = getClickedBond_(e->x(), e->y());
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
	order = BALL_MIN((Index)Bond::ORDER__AROMATIC, order);
	if (current_bond_->getOrder() == order) return;

	current_bond_->setOrder(order);
	getMainControl()->update(*(Atom*)current_bond_->getFirstAtom(), true);
	String txt = "Set bond order to ";
	txt += getBondOrderString_(order);
	setStatusbarText(txt);
}

void EditableScene::mouseMoveEvent(QMouseEvent *e)
{
	if (current_mode_ < (Scene::ModeType) EDIT__MODE)
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
	Atom* atom = getClickedAtom_(e->x(), e->y());

	// have we found such an atom? if so, is it different from the one we started with?
	// (self bonds make no sense)
	if (atom && atom != current_atom_)
	{
		// if we are really close to an atom, the endpoints of the line we draw will be set to
		// its center, so that the user has a drop in effect for the bonds
		atom_pos_ = atom->getPosition();
	}
	else
	{
		atom_pos_ = get3DPosition_(e->x(), e->y());
	}

	// paint the line representing the offered bond
	draw_line_ = true;
	update(false);

	x_window_pos_old_ = x_window_pos_new_;
	y_window_pos_old_ = y_window_pos_new_;
	x_window_pos_new_ = e->x();
	y_window_pos_new_ = e->y();
}

void EditableScene::paintGL()
{
	Scene::paintGL();

	if (current_mode_ != (Scene::ModeType) EDIT__MODE ||
			last_buttons_ != Qt::MidButton) 
	{
		draw_line_ = false;
	}

	if (!draw_line_ || current_atom_ == 0) return;

	glDisable(GL_LIGHTING);
	gl_renderer_.setColorRGBA_(stage_->getBackgroundColor().getInverseColor());

	glBegin(GL_LINES);
		gl_renderer_.vertexVector3_(current_atom_->getPosition());
		gl_renderer_.vertexVector3_(atom_pos_);
	glEnd();

	glEnable(GL_LIGHTING);
}

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

	if (last_buttons_ == Qt::LeftButton) return;

	if (last_buttons_ == Qt::RightButton) 
	{
		deselect_();
		return;
	}

	if (isAnimationRunning() || getMainControl()->isBusy()) return;

	mouse_button_is_pressed_ = false;

	if (current_atom_)
 	{	
		current_atom_->deselect();
		notify_(new CompositeMessage(*current_atom_, CompositeMessage::DESELECTED_COMPOSITE));	
	}

	// if we didnt find first atom: abort
	if (!current_atom_) 
	{
		deselect_();
		return;
	}

	// is there an atom in radius "limit_" Angstroem?
	Atom* atom = getClickedAtom_(e->x(), e->y());

	// decide what to do... did we find an atom at all?
	if (atom)
	{
		// is it the atom we started with?
		if (atom == current_atom_)
		{
			// in this case, we assume that the user does not want to set a bond
			draw_line_ = false;
		}
		else // we found _another_ atom
		{
			//set the bond
			Bond* c = new Bond("Bond", *current_atom_, *atom, Bond::ORDER__SINGLE);		
			
			EditOperation eo(0, c, "Added bond of type single" , EditOperation::ADDED__BOND);
			undo_.push_back(eo);
		
			// tell about the new undo operation
			emit newEditOperation(eo);

			//update representation
			getMainControl()->update(*atom, true);
			setStatusbarText("Added bond");
		}	
	}
	else // no atom found -> create one
	{
		Vector3 new_pos = get3DPosition_(e->x(), e->y());
		// test if the two atoms would have the same position
		if (current_atom_->getPosition() == new_pos)
		{
			setStatusbarText("Aborting, since both atoms would have the same location!", true);
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
	
		EditOperation eo(a, NULL, "Added atom of type " + PTE[atomic_number_].getName() + " at position (" 
										+ String(atom_position.x) + ", "
										+ String(atom_position.y) + ", "
										+ String(atom_position.z) + ")", EditOperation::ADDED__ATOM);
		undo_.push_back(eo);

		// tell about the new undo operation
		emit newEditOperation(eo);

		//set the bond
		Bond* c = new Bond("Bond", *current_atom_, *a, bond_order_);		
	
		// tell about the new undo operation
		String bond_string = getBondOrderString_(bond_order_);
		EditOperation eo2(0, c, "Added bond of type " + bond_string, EditOperation::ADDED__BOND);
		undo_.push_back(eo2);
		emit newEditOperation(eo2);

		getMainControl()->update(*a->getParent(), true);
		setStatusbarText("Added bond and atom");
	}

	deselect_();
}	

String EditableScene::getBondOrderString_(Index order)
{
	String bond_string;
	switch (order)
	{
		case Bond::ORDER__SINGLE:
			bond_string = "single";
			break;
		case Bond::ORDER__DOUBLE:
			bond_string = "double";
			break;
		case Bond::ORDER__TRIPLE:						
			bond_string = "triple";	
			break;
		case Bond::ORDER__QUADRUPLE:
			bond_string = "quadruple";	
			break;
		case Bond::ORDER__AROMATIC:
			bond_string = "aromatic";	
			break;
		default:					
			bond_string = "unknown";	
			break;
	}
	
	return bond_string;
}

/// ******************** Helper Functions *************************
List<AtomContainer*> EditableScene::getContainers_()
{
	List<AtomContainer*> containers;
	if (only_highlighted_)
	{
		List<Composite*> highl = getMainControl()->getMolecularControlSelection();
		List<Composite*>::Iterator lit = highl.begin();
		for (; lit != highl.end(); ++lit)
		{
			AtomContainer* ac = dynamic_cast<AtomContainer*>(*lit);
			if (ac != 0) containers.push_back(ac);
		}
	}
	else
	{
		HashSet<Composite*> composites = getMainControl()->getCompositeManager().getComposites();
		HashSet<Composite*>::Iterator sit = composites.begin();
		for (; +sit; ++sit)
		{
			AtomContainer* ac = dynamic_cast<AtomContainer*>(*sit);
			if (ac != 0) containers.push_back(ac);
		}
	}

	return containers;
}

// Find closest atom to screen position (x,y). If there is none closer than atom_limit_, return NULL
Atom* EditableScene::getClickedAtom_(int x, int y)
{
	QPoint p(x,y);
	List<GeometricObject*> objects;
	gl_renderer_.pickObjects1((Position) p.x(), (Position) p.y(), 
														(Position) p.x(), (Position) p.y());
	renderView_(DIRECT_RENDERING);
 	gl_renderer_.pickObjects2(objects);

	if (objects.size() > 0)
	{
		Atom* atom = dynamic_cast<Atom*>((Composite*)(**objects.begin()).getComposite());
		return atom;
	}

	return 0;
}

// Find closest bond to screen position (x,y). If there is none closer than bond_limit_, return NULL
// Note: this code is very similar to getClickedAtom. Maybe those two should be united.
Bond* EditableScene::getClickedBond_(int x, int y)
{
	QPoint p(x,y);
	List<GeometricObject*> objects;
	gl_renderer_.pickObjects1((Position) p.x(), (Position) p.y(), 
														(Position) p.x(), (Position) p.y());
	renderView_(DIRECT_RENDERING);
 	gl_renderer_.pickObjects2(objects);

	if (objects.size() > 0)
	{
		Composite* c = (Composite*)(**objects.begin()).getComposite();
		if (c == 0) return 0;

		Bond* bond = dynamic_cast<Bond*>(c);
		return bond;
	}

	// code below does not work correctly.
	return 0;
}

void EditableScene::setElementCursor()
{
	String s = PTE[atomic_number_].getSymbol();
	s.truncate(1);
 	setCursor(s.c_str());
}

// Slot to change to EDIT__MODE
void EditableScene::editMode_()
{
	if (!fragment_db_initialized_)
	{
		fragment_db_.setFilename("fragments/Editing-Fragments.db");
		fragment_db_.init();
		fragment_db_initialized_ = true;
	}

	last_mode_ = current_mode_;
	current_mode_ = (Scene::ModeType)EDIT__MODE;		
	edit_id_->setChecked(true);
	setElementCursor();

	HashSet<Composite*> selection = getMainControl()->getSelection();
	HashSet<Composite*>::Iterator it = selection.begin();
	for (; +it; ++it)
	{
		getMainControl()->deselectCompositeRecursive(*it, true);
		getMainControl()->update(**it, false);
	}
	notify_(new NewSelectionMessage);
}

// insert an atom at screen positions (x,y) on the view plane
void EditableScene::insert_(int x, int y, PDBAtom &atom)
{
	// find the 3D coordinates of screen position (x,y) on the view plane
	// move the atom to that position
	atom.setPosition(get3DPosition_(x,y));

	// now we need to find the AtomContainer into which we will insert the atom.
	// get all highlighted composites
	List<Composite*> composite_list = getMainControl()->getMolecularControlSelection(); 
	
	if (composite_list.size() > 1 )
	{
		setStatusbarText("Please highlight exactly one AtomContainer for insertion of the created atoms!", true);
		return;
	}

	// only one highlighted composite
	if (composite_list.size() == 1)
	{
		// is it an AtomContainer?
		AtomContainer* ai = dynamic_cast<AtomContainer*>(*composite_list.begin());
		if (ai == 0)
		{
			setStatusbarText("Please highlight exactly one AtomContainer for insertion of the created atoms!", true);
			return;
		}

		// Yes? we do not need to create our own system
		ai->insert(atom);
		getMainControl()->update(*ai, true);
	}
	else  // more or less than 1 highlighted
	{
		HashSet<Composite*> composites = getMainControl()->getCompositeManager().getComposites();
		if (composites.size() != 0)
		{
			System* system = dynamic_cast<System*>(*composites.begin());
			Molecule* mol = system->getMolecule(0);
			if (mol != 0)
 			{
				mol->appendChild(atom);
				getMainControl()->update(*mol, true);
				return;
			}
		}
		
		System *system = new System();
		Molecule* current_molecule = new Molecule();
		system->insert(*current_molecule);
		current_molecule->insert(atom);
		getMainControl()->insert(*system);
	}	
}	


// this code projects the 3D view plane to 2D screen coordinates


TVector2<Position> EditableScene::getScreenPosition_(Vector3 vec)
{
	// find the monitor coordinates of a given vector
	TVector2<Position> pos ; 
	pos.x = INT_MAX;
	pos.y = INT_MAX;

	double xs_ = width();
	double ys_ = height(); 

	// vectors for arithmetics
	// first we compute the projection of the given atom on the Viewplane
	Vector3 cam = getStage()->getCamera().getViewPoint();
	Vector3 look_at = getStage()->getCamera().getLookAtPosition() -  cam ;
	Vector3 cam_to_atom = vec - cam;

	try 
	{
		if (mapViewplaneToScreen_())
		{
			Vector3 la_m_d(near_left_bot_ + (near_right_bot_ - near_left_bot_) * 0.5 
																		+ (near_left_top_  - near_left_bot_) * 0.5 - cam);

			look_at.normalize();

			float parm = la_m_d.getLength() /(look_at * cam_to_atom); 
			Vector3 origin_to_atom_projection_on_viewplane =   cam + parm * cam_to_atom;	

			//now we compute the position on Screen of the projected atom
			Vector3 screen_top_left_pos_to_proj_atom =  origin_to_atom_projection_on_viewplane  - near_left_top_ ; 
			unsigned int x_mouse = (unsigned int)( screen_top_left_pos_to_proj_atom * ( near_right_bot_ - near_left_bot_ )
					/   (( near_right_bot_ - near_left_bot_ ).getSquareLength())  * xs_);
			unsigned int y_mouse = (unsigned int) (screen_top_left_pos_to_proj_atom * ( near_left_bot_ - near_left_top_ ) 
					/   (( near_left_bot_ - near_left_top_ ).getSquareLength() )  * ys_);

			pos.x = x_mouse;
			pos.y = y_mouse;
		}
	}	
	catch (...)
	{
	}

	return pos;
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

	QAction* rotate_mode = menu.addAction("Rotate Mode", this, SLOT(rotateMode_()));
	rotate_mode->setCheckable(true);
	rotate_mode->setChecked(current_mode_ == (Scene::ModeType) MOVE__MODE);

	QAction* picking_mode = menu.addAction("Picking Mode", this, SLOT(pickingMode_()));
	picking_mode->setCheckable(true);
	picking_mode->setChecked(current_mode_ == (Scene::ModeType) PICKING__MODE);
	
	QAction* move_mode = menu.addAction("Move Mode", this, SLOT(moveMode_()));
	move_mode->setCheckable(true);
	move_mode->setChecked(current_mode_ == (Scene::ModeType) MOVE__MODE);
	
	QAction* edit_mode = menu.addAction("Edit Mode", this, SLOT(editMode_()));
	edit_mode->setCheckable(true);
	edit_mode->setChecked(current_mode_ == (Scene::ModeType) EDIT__MODE);

	menu.addSeparator();

	if (current_mode_ == (Scene::ModeType) EDIT__MODE)
	{
		menu.addAction("Atom Properties", this, SLOT(atomProperties_()))->setEnabled(current_atom_ != 0);
		menu.addAction("Move Atom", this, SLOT(moveAtom_()))->setEnabled(current_atom_ != 0);
		menu.addAction("Delete Atom", this, SLOT(deleteAtom_()))->setEnabled(current_atom_ != 0);
		menu.addAction(element_action_);

		QMenu* charge = new QMenu();
		QAction* change_charge = menu.addMenu(charge);
		change_charge->setText("Set formal charge");
		for (Index p = +6; p > -7; p--)
		{
			String s(p);
			if (p > 0) s = String("+") + s;

			charge->addAction(s.c_str(), this, SLOT(setFormalCharge_()));
		}
		change_charge->setEnabled(current_atom_ != 0);

		menu.addSeparator();

		menu.addAction("Delete Bond", this, SLOT(deleteBond_()))->setEnabled(current_bond_ != 0);

		QMenu* order = new QMenu();
		QAction* change_order = menu.addMenu(order);
		connect(order, SIGNAL(hovered(QAction*)), this, SLOT(activatedOrderItem_(QAction*)));
		change_order->setText("Change bond order");
		order->addAction("Single",    this, SLOT(changeBondOrder_()));
		order->addAction("Double",    this, SLOT(changeBondOrder_()));
		order->addAction("Triple",    this, SLOT(changeBondOrder_()));
		order->addAction("Quadruple", this, SLOT(changeBondOrder_()));
		order->addAction("Aromatic",  this, SLOT(changeBondOrder_()));
		change_order->setEnabled(current_bond_ != 0);

		menu.addSeparator();

		QMenu* add_menu = new QMenu();
		QAction* add_action = menu.addMenu(add_menu);
		add_action->setText("Add");
		if (getContainers_().size() == 0)
		{
			add_action->setEnabled(false);
		}
		add_menu->addAction("Pyrrole", this, SLOT(addStructure_()));
		add_menu->addAction("Benzene", this, SLOT(addStructure_()));
		add_menu->addAction("Indole", this, SLOT(addStructure_()));

		menu.addSeparator();

		menu.addAction(optimize_);
		menu.addAction(add_hydrogens_);
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

void EditableScene::deselect_()
{
	if (current_bond_ != 0 && current_bond_->isSelected())
	{
		current_bond_->deselect();
		Atom* a1 = (Atom*)current_bond_->getFirstAtom();
		Atom* a2 = (Atom*)current_bond_->getSecondAtom();
		a1->deselect();
		a2->deselect();
		notify_(new CompositeMessage(*a1, CompositeMessage::DESELECTED_COMPOSITE));	
		notify_(new CompositeMessage(*a2, CompositeMessage::DESELECTED_COMPOSITE));	
	}

	if (current_atom_!= 0 && current_atom_->isSelected())
	{
		current_atom_->deselect();
		notify_(new CompositeMessage(*current_atom_, CompositeMessage::DESELECTED_COMPOSITE));	
	}
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

	if (current_atom_ != 0)
	{
		current_atom_->setElement(PTE[atomic_number_]);
		deselect_();
		getMainControl()->update(*current_atom_);
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
	current_bond_ = 0;
}

void EditableScene::changeBondOrder_()
{
	if (current_bond_ == 0) return;
	if (current_bond_->getOrder() == bond_order_) return;

	current_bond_->setOrder((Bond::BondOrder)bond_order_);
	getMainControl()->update(*(Atom*)current_bond_->getFirstAtom(), true);
}

void EditableScene::onNotify(Message *message)
	throw()
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
			// TODO ????
			current_atom_ = 0;
			current_bond_ = 0;
		}
	}

	Scene::onNotify(message);
}

void EditableScene::activatedOrderItem_(QAction* action)
{
	if (action == 0) return;
	String text = ascii(action->text());

	if (text == "Single") bond_order_ = Bond::ORDER__SINGLE;
	else if (text == "Double") bond_order_ = Bond::ORDER__DOUBLE;
	else if (text == "Triple") bond_order_ = Bond::ORDER__TRIPLE;
	else if (text == "Quadruple") bond_order_ = Bond::ORDER__QUADRUPLE;
	else if (text == "Aromatic") bond_order_ = Bond::ORDER__AROMATIC;
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
	List<Composite*> selection;
	selection.push_back(current_molecule);
	nsm->setSelection(selection);
	notify_(nsm);
}

void EditableScene::setMode(ModeType mode)
	throw()
{
	Scene::setMode(mode);

	if (mode == (Scene::ModeType) EDIT__MODE)	editMode_();
	update();
}

void EditableScene::applyPreferences()
	throw()
{
	Scene::applyPreferences();
	if (edit_settings_ == 0) return;
	bond_limit_ = edit_settings_->bond_distance->text().toFloat();
	atom_limit_ = edit_settings_->atom_distance->text().toFloat();
	only_highlighted_ = edit_settings_->only_highlighted->isChecked();

	if ((int)current_mode_ == (int)EDIT__MODE)
	{
		setElementCursor();
	}
}

void EditableScene::initializePreferencesTab(Preferences& preferences)
	throw()
{
	Scene::initializePreferencesTab(preferences);
	edit_settings_ = new EditSettings(this);
	preferences.insertEntry(edit_settings_);
}

void EditableScene::finalizePreferencesTab(Preferences& preferences)
	throw()
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
	}
}

bool EditableScene::reactToKeyEvent_(QKeyEvent* e)
{
	int key = e->key();

	if (key == Qt::Key_E)
	{
		setMode((ModeType)EDIT__MODE);
		return true;
	}

	if (current_mode_ != (ModeType)EDIT__MODE) return false;

	if (key == Qt::Key_D && !getMainControl()->isBusy())
	{
		QPoint point = mapFromGlobal(QCursor::pos());
	  current_atom_ = getClickedAtom_(point.x(), point.y());
		deleteAtom_();
		return true;
	}

	if (key == Qt::Key_Backspace && !getMainControl()->isBusy())
	{
		QPoint point = mapFromGlobal(QCursor::pos());
	  current_bond_ = getClickedBond_(point.x(), point.y());
		deleteBond_();
		return true;
	}


	if (key < Qt::Key_A ||
			key > Qt::Key_Z)
	{
		return false;
	}

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

	String text("Setting element to ");
	text += PTE[atomic_number_].getName();
	setStatusbarText(text, true);

	return true;
}

void EditableScene::addStructure(String name)
{
	deselect_();

	if (!fragment_db_initialized_)
	{
		fragment_db_.setFilename("fragments/Editing-Fragments.db");
		fragment_db_.init();
		fragment_db_initialized_ = true;
	}

	List<AtomContainer*> containers = getContainers_();
	if (containers.size() == 0) return;

	Residue* residue = fragment_db_.getResidueCopy(name);
	if (residue == 0)
	{
		residue = fragment_db_.getResidueCopy(name + "-Skeleton");
		if (residue == 0) return;
	}

	Matrix4x4 m;
	Vector3 x = get3DPosition_(menu_point_.x(), menu_point_.y());
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

	m.setTranslation(x);
	tf.setTransformation(m);
	residue->apply(tf);

	AtomContainer* s = *containers.begin();
	s->insert(*residue);
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
	List<Composite*> sel;
	sel.push_back(m);
	msg->setSelection(sel);
	notify_(msg);
	editMode_();
}

void EditableScene::addHydrogens()
{
	if (getMainControl()->isBusy()) return;

	deselect_();
	List<AtomContainer*> containers = getContainers_();
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
	setStatusbarText(String("Added ") + nr + " hydrogens.", true);
	getMainControl()->update(*ac, true);
}

void EditableScene::optimizeStructure()
{
	if (getMainControl()->isBusy()) return;

	List<AtomContainer*> containers = getContainers_();
	if (containers.size() < 1) return;

	MolecularStructure* ms = MolecularStructure::getInstance(0);
	if (ms == 0) return;
	
	AtomContainer* ac = *containers.begin();
	System* system = (System*)&ac->getRoot();

	setStatusbarText("Optimizing Structure...", true);

	// highlight System for minimization
	ControlSelectionMessage* nsm =  new ControlSelectionMessage();
	List<Composite*> selection;
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
	md.setMaxIterations(100);
	md.setRefresh(25);
	md.setMaxGradient(1);
	ms->runMinimization(false);
}

void EditableScene::addToolBarEntries(QToolBar* tb)
{
	toolbar_actions_.insert(toolbar_actions_.lastIndexOf(move_action_) + 1, edit_id_);
	toolbar_actions_.push_back(element_action_);
	toolbar_actions_.push_back(add_hydrogens_);
	toolbar_actions_.push_back(optimize_);
	Scene::addToolBarEntries(tb);
	toolbar_->insertSeparator(del_selection_);
}

void EditableScene::mouseDoubleClickEvent(QMouseEvent* e)
{
	if (current_mode_ != (ModeType) EDIT__MODE) return;

	current_atom_ = getClickedAtom_(e->x(), e->y());
	if (current_atom_ != 0)
	{
		current_atom_->setElement(PTE[atomic_number_]);
		deselect_();
		getMainControl()->update(*current_atom_);
	}
}

	}//end of namespace 
} //end of namespace
