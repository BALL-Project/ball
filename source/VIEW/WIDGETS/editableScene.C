/*    THIS IS EXPERIMENTAL CODE  
 *
 *    WE GIVE NO WARRANTY
 *    
 *    USE AT YOUR OWN RISK!!!!!!
 */

#include <BALL/VIEW/WIDGETS/editableScene.h>

#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/compositeManager.h>
#include <BALL/VIEW/DIALOGS/compositeProperties.h>

#include <QtGui/qpainter.h>
#include <QtGui/qmenubar.h>
#include <QtGui/qcursor.h>
#include <QtGui/QDesktopWidget>

#include <BALL/MATHS/vector3.h>
#include <BALL/MATHS/matrix44.h>
#include <BALL/MATHS/angle.h>

using std::endl;
using std::ostream;
using std::istream;


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
//
EditableScene::EditableScene()
	throw()
	:	Scene(),
		atom_id_(0),
		bond_id_(0),
		current_atom_(0),
		atom_limit_(1.5),
		bond_limit_(2.0),
		atom_type_(6),
		undo_()
{
}

EditableScene::EditableScene(QWidget* parent_widget, const char* name, Qt::WFlags w_flags)
	throw()
	: Scene(parent_widget, name, w_flags),
		atom_id_(0),
		bond_id_(0),
		current_atom_(0),
		atom_limit_(1.5),
		bond_limit_(2.0),
		atom_type_(6),
		bond_order_(Bond::ORDER__SINGLE),
		undo_()
{	
	registerWidget(this); 
}

// undo_ is NOT copied, since we would run into trouble with the pointers to atoms and bonds it saves
EditableScene::EditableScene(const EditableScene& eScene, QWidget* parent_widget, const char* name , Qt::WFlags w_flags)
	throw()
	: Scene(eScene, parent_widget, name, w_flags),
		atom_id_(0),
		bond_id_(0),
		current_atom_(eScene.current_atom_),
		atom_limit_(eScene.atom_limit_),
		bond_limit_(eScene.bond_limit_),
		atom_type_(eScene.atom_type_),
		undo_()
{
}

EditableScene::~EditableScene()
	throw()
{
#ifdef BALL_VIEW_DEBUG
	Log.info() << "Destructing object EditableScene " << this << " of class EditableScene>" << std::endl;
#endif 
}

void EditableScene::clear()
	throw()
{
	Scene::clear();
}

void EditableScene::initializeWidget(MainControl& main_control)
	throw()
{
	Scene::initializeWidget(main_control);
	atom_id_ =	main_control.insertMenuEntry(MainControl::DISPLAY, "Set Atoms Mode", this, SLOT(atomMode_()), Qt::CTRL+Qt::Key_E);
	atom_id_->setCheckable(true);
	setMenuHint("Create and modify atoms");
	bond_id_ =	main_control.insertMenuEntry(MainControl::DISPLAY, "Set Bonds Mode", this, SLOT(bondMode_()));
	bond_id_->setCheckable(true);
	setMenuHint("Create and modify bonds");
}


void EditableScene::finalizeWidget(MainControl& main_control)
	throw()
{
	Scene::finalizeWidget(main_control);	
}


void EditableScene::checkMenu(MainControl& main_control)
	throw()
{
	atom_id_->setChecked(current_mode_ == (Scene::ModeType)ATOM__MODE);
	bond_id_->setChecked(current_mode_ == (Scene::ModeType)BOND__MODE);
	Scene::checkMenu(main_control);
}

void EditableScene::mousePressEvent(QMouseEvent* e)
{
	if (current_mode_ < (Scene::ModeType) ATOM__MODE)
	{
		Scene::mousePressEvent(e);
		return;
	}

	if (isAnimationRunning() || getMainControl()->isBusy()) return;

	x_window_pos_old_ = x_window_pos_new_;
	y_window_pos_old_ = y_window_pos_new_;
	x_window_pos_new_ = e->x();
	y_window_pos_new_ = e->y();
	mouse_button_is_pressed_ = true;

	if (current_mode_ == (Scene::ModeType)ATOM__MODE)
	{
		if (e->button() == Qt::LeftButton)
		{	
			// insert a new atom:
			PDBAtom* a = new PDBAtom(PTE[atom_type_], PTE[atom_type_].getName());
			insert_(e->x(), e->y(), *a);		
			current_atom_ = a;
			
			//store the Operation in undo_
			Vector3 atom_position = a->getPosition();
			draw_line_ = false;
			
			EditOperation eo(a, NULL, "Added atom of type " + PTE[atom_type_].getName() + " at position (" 
											+ String(atom_position.x) + ", "
											+ String(atom_position.y) + ", "
											+ String(atom_position.z) + ")", EditOperation::ADDED__ATOM);
			undo_.push_back(eo);

			// tell about the new undo operation
			emit newEditOperation(eo);
			return;
		}

		// find an atom in a radius of limit_ around the current mouse position
		current_atom_ = getClickedAtom_(e->x(), e->y());
		if (current_atom_)
		{
			current_atom_->select(); 
			notify_(new CompositeMessage(*current_atom_, CompositeMessage::SELECTED_COMPOSITE));	
		}

		if (e->button() == Qt::RightButton)
		{	
			// we open a context menu at this point
			showContextMenu(QPoint(e->x(), e->y()));
		}
	}
	else if (current_mode_ == (Scene::ModeType)BOND__MODE)
	{
		if (e->button() == Qt::LeftButton)
		{	
			current_atom_ = getClickedAtom_(e->x(), e->y());
			if (current_atom_)
			{
				current_atom_->select(); 
				notify_(new CompositeMessage(*current_atom_, CompositeMessage::SELECTED_COMPOSITE));	
			}
		}
		if (e->button() == Qt::RightButton)
		{
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
			}

			current_bond_ = bond;
			showContextMenu(QPoint(e->x(), e->y()));
		}
	}
}

void EditableScene::wheelEvent(QWheelEvent* e)
{
	if (current_mode_ < (Scene::ModeType) ATOM__MODE)
	{
		Scene::wheelEvent(e);
		return;
	}

	e->accept();

	if (isAnimationRunning() || getMainControl()->isBusy()) return;

	Index delta = e->delta();
	if (delta == 0) return;
	if (delta > 1) delta = 1;
	if (delta < -1) delta = -1;

	if (current_mode_ == (Scene::ModeType)BOND__MODE)
	{
		QPoint point = mapFromGlobal(QPoint(e->globalX(), e->globalY()));
		Bond* bond = getClickedBond_(point.x(), point.y());
		if (bond == 0) return;

		Index order = bond->getOrder();
		order += delta;
		order = BALL_MAX((Index)Bond::ORDER__SINGLE, order);
		order = BALL_MIN((Index)Bond::ORDER__AROMATIC, order);
		bond->setOrder(order);
		getMainControl()->update(*(Atom*)bond->getFirstAtom(), true);
		String txt = "Set bond order to ";
		txt += getBondOrderString_(order);
		setStatusbarText(txt, true);
	}
	else if (current_mode_ == (Scene::ModeType) ATOM__MODE)
	{
	}
}

void EditableScene::mouseMoveEvent(QMouseEvent *e)
{
	if (current_mode_ < (Scene::ModeType) ATOM__MODE)
	{
		Scene::mouseMoveEvent(e);
		return;
	}

	if (isAnimationRunning() || getMainControl()->isBusy()) return;

	// ============ bond mode ================
	if (current_mode_ == (Scene::ModeType)BOND__MODE)
	{
		if (e->buttons() == Qt::LeftButton)
		{
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
		}
	}
	else if (current_mode_ == (Scene::ModeType)ATOM__MODE) 
	{
		if (current_atom_ == 0 ||
				e->buttons() != Qt::MidButton)
		{
			return;
		}

		float ydiff = (float)e->y() - (float)y_window_pos_new_;
		ydiff /= qApp->desktop()->height();
		ydiff *= mouse_sensitivity_;
		Vector3 diff = stage_->getCamera().getViewVector();
		diff.normalize();
		diff *= ydiff;

		current_atom_->getPosition() += diff;
		getMainControl()->update(*current_atom_);
	}

	x_window_pos_old_ = x_window_pos_new_;
	y_window_pos_old_ = y_window_pos_new_;
	x_window_pos_new_ = e->x();
	y_window_pos_new_ = e->y();
}

void EditableScene::paintGL()
{
	Scene::paintGL();

	if (current_mode_ != (Scene::ModeType) BOND__MODE) draw_line_ = false;

	if (!draw_line_ || current_atom_ == 0) return;

	glDisable(GL_LIGHTING);
	gl_renderer_.setColorRGBA_(stage_->getBackgroundColor().getInverseColor());

	glBegin(GL_LINES);
		gl_renderer_.vertexVector3_(current_atom_->getPosition());
		gl_renderer_.vertexVector3_(atom_pos_);
	glEnd();
}

void EditableScene::mouseReleaseEvent(QMouseEvent* e)
{
	if ((int)current_mode_ < (int) ATOM__MODE)
	{
		Scene::mouseReleaseEvent(e);
		return;
	}

	if (isAnimationRunning() || getMainControl()->isBusy()) return;

	mouse_button_is_pressed_ = false;

	if (current_atom_)
 	{	
		current_atom_->deselect();
		notify_(new CompositeMessage(*current_atom_, CompositeMessage::DESELECTED_COMPOSITE));	
	}

	// are we in BOND__MODE? 
	if (current_mode_ == (Scene::ModeType)BOND__MODE)
	{
		// if we didnt find first atom: abort
		if (!current_atom_) return;

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
				// TODO: - make the bond_order variable. Even better: change order with click on the bond
				//       - if there is already a bond, change it to a double bond
				Bond* c = new Bond("Bond", *current_atom_, *atom, Bond::ORDER__SINGLE);		
				
				EditOperation eo(0, c, "Added bond of type " , EditOperation::ADDED__BOND);
				undo_.push_back(eo);
			
				// tell about the new undo operation
				emit newEditOperation(eo);

				//update representation
				getMainControl()->update(*atom, true);
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
			PDBAtom* a = new PDBAtom(PTE[atom_type_], PTE[atom_type_].getName());
			a->setPosition(new_pos);
			current_atom_->getParent()->appendChild(*a);
			
			//store the Operation in undo_
			Vector3 atom_position = a->getPosition();
		
			EditOperation eo(a, NULL, "Added atom of type " + PTE[atom_type_].getName() + " at position (" 
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
		}
	}
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

// Find closest atom to screen position (x,y). If there is none closer than atom_limit_, return NULL
Atom* EditableScene::getClickedAtom_(int x, int y)
{
	//get the AtomContainer
	CompositeManager& cm = getMainControl()->getCompositeManager();
	CompositeManager::iterator it = cm.begin();

	float min_dist = FLT_MAX;
	Atom* min_atom = 0;
	float dist;

	for (; it != cm.end(); it++)
	{
		//check if composite is a system
		System* s = dynamic_cast<System*>(*it);
		if (s == 0) continue;

		Vector3 cam_to_atom;
		Vector3 cam_to_clickedPoint = get3DPosition_(x, y) - getStage()->getCamera().getViewPoint();

		for (AtomIterator ai = s->beginAtom(); +ai; ++ai)
		{
			cam_to_atom = (ai->getPosition() - getStage()->getCamera().getViewPoint());

			// compute the angle between the rays Cam->Atom and Cam->clicked point
			Angle	alpha((float)acos(  (cam_to_atom * cam_to_clickedPoint)
						/(cam_to_atom.getLength() * cam_to_clickedPoint.getLength())
						));

			// the distance between the two rays is the sine of the angle between them times the length of Cam->Atom	
			dist  = sin(alpha) * cam_to_atom.getLength();

			// now save atom and distance
			if (dist < min_dist)
			{
				min_dist = dist;
				min_atom = &(*ai);
			}
		}
	}

	// is the minimal distance beyond the threshold?
	if (min_dist < atom_limit_)
	{
		return min_atom;
	}

	return 0;
}

// Find closest bond to screen position (x,y). If there is none closer than bond_limit_, return NULL
// Note: this code is very similar to getClickedAtom. Maybe those two should be united.
Bond* EditableScene::getClickedBond_(int x, int y)
{
	// get the AtomContainer
	CompositeManager& cm = getMainControl()->getCompositeManager();

	Bond* closest = 0;
	float min_dist = FLT_MAX;

	for (CompositeManager::iterator it = cm.begin(); +it; it++)
	{
		// check if composite is a system
		System* s = dynamic_cast<System*>(*it);
		if (s == 0) continue;

		// The Composite is a system
		// save the atom and its distance to the click ray
		Vector3 cam_to_bond;
		Vector3 cam_to_clickedPoint = get3DPosition_(x, y) - getStage()->getCamera().getViewPoint();

		Vector3 vp = getStage()->getCamera().getViewPoint();

		// To iterate over all bonds, we have to iterate over all atoms and then over all their bonds
		// Unfortunately, this counts each bond twice...
		for (AtomIterator ai = s->beginAtom(); +ai; ++ai)
		{
			for (AtomBondIterator bi = ai->beginBond(); +bi; ++bi)
			{
				if (bi->getPartner(*ai) < &*ai) continue;

				// first point the position vector to the first atom of the bond
				cam_to_bond = (bi->getFirstAtom()->getPosition() - vp);
				// then add 1/2 * the vector pointing from first to second
				cam_to_bond += (bi->getSecondAtom()->getPosition() - bi->getFirstAtom()->getPosition()) * 0.5;

				// compute the angle between the rays Cam->Bond and Cam->clicked point
				Angle	alpha((float)acos(  (cam_to_bond * cam_to_clickedPoint)
							/(cam_to_bond.getLength() * cam_to_clickedPoint.getLength())
							));

				// the distance between the two rays is the sine of the angle between them times the length of Cam->Bond
				float dist = sin(alpha) * cam_to_bond.getLength();

				// now save bond and distance
				if (dist < min_dist)
				{
					min_dist = dist;
					closest = &*bi;
				}
			}
		}
	}

	// is the minimal distance beyond the threshold?
	if (min_dist < bond_limit_)
	{
		return closest;
	}

	return 0;
}

// Slot to change to BOND__MODE
void EditableScene::bondMode_()
{
	last_mode_ = current_mode_;
	current_mode_ = (Scene::ModeType)BOND__MODE;		
	setCursor(QCursor(Qt::IBeamCursor));
	//ToDo:: Cursor should look different
}


// Slot to change to ATOM__MODE
void EditableScene::atomMode_()
{
	last_mode_ = current_mode_;
	current_mode_ = (Scene::ModeType)ATOM__MODE;		
	setCursor(QCursor(Qt::UpArrowCursor));
	//ToDo:: Cursor should look different
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
bool EditableScene::mapViewplaneToScreen_()
{
	// matrix for the Projection matrix 	
	GLdouble projection_matrix[16];
	// matrix for the Modelview matrix
	GLdouble modelview_matrix[16];

	// variables for definition of projection matrix
	float near_=0, left_=0, right_=0, bottom_ =0, top_=0; 

	// take the Projection matrix	
	glMatrixMode(GL_PROJECTION);
	glGetDoublev(GL_PROJECTION_MATRIX, projection_matrix);
	glMatrixMode(GL_MODELVIEW);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview_matrix); 

	// determine the projection variables
	if(projection_matrix[0]==0. || projection_matrix[5]==0. || projection_matrix[10]==1.)
	{	
		Log.error() << "Projection variables equal zero! " << endl;
		return false;
	}	
	near_   = projection_matrix[14]/(projection_matrix[10]-1);
	left_   = projection_matrix[14]*(projection_matrix[8]-1) / (projection_matrix[0]*(projection_matrix[10]-1));
	right_  = projection_matrix[14]*(projection_matrix[8]+1) / (projection_matrix[0]*(projection_matrix[10]-1));
	bottom_ = projection_matrix[14]*(projection_matrix[9]-1) / (projection_matrix[5]*(projection_matrix[10]-1));
	top_    = projection_matrix[14]*(projection_matrix[9]+1) / (projection_matrix[5]*(projection_matrix[10]-1));

	// we have to move all points of the viewing volume with the inverted Modelview matrix 
	Matrix4x4 mod_view_mat_(modelview_matrix[0], modelview_matrix[4], modelview_matrix[8], modelview_matrix[12],
			modelview_matrix[1], modelview_matrix[5], modelview_matrix[9], modelview_matrix[13],
			modelview_matrix[2], modelview_matrix[6], modelview_matrix[10], modelview_matrix[14],
			modelview_matrix[3], modelview_matrix[7], modelview_matrix[11],	modelview_matrix[15]);


	Matrix4x4 inverse_mod_view_mat_;
	mod_view_mat_.invert(inverse_mod_view_mat_);

	// determine the nearplane vectors
	near_left_bot_ = Vector3(left_,  bottom_, near_*-1.); //a
	near_right_bot_= Vector3(right_, bottom_, near_*-1.); //b
	near_left_top_ = Vector3(left_,  top_,    near_*-1.); //c	

	near_left_bot_  = inverse_mod_view_mat_*near_left_bot_;
	near_right_bot_ = inverse_mod_view_mat_*near_right_bot_;
	near_left_top_  = inverse_mod_view_mat_*near_left_top_;

	return true;
}

TVector2<Position> EditableScene::getScreenPosition_(Vector3 vec)
{
	// find the monitor coordinates of a given vector
	TVector2<Position> pos ; 

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
			Vector3 la_m_d(near_left_bot_
					+ (near_right_bot_ - near_left_bot_) * 0.5 
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

			pos.x=x_mouse;
			pos.y=y_mouse;
			return pos;
		}
		else
		{
			pos.x = std::numeric_limits<unsigned int>::max();
			pos.y = std::numeric_limits<unsigned int>::max();

			return pos;
		}
	}	
	catch (...)
	{
		pos.x = std::numeric_limits<unsigned int>::max();
		pos.y = std::numeric_limits<unsigned int>::max();

		return pos;
	}
}


// Convert 2D screen coordinate to 3D coordinate on the view plane
Vector3 EditableScene::get3DPosition_(int x, int y)
{
	// 	Scale variables for Frustum
	double xs_ = width();
	double ys_ = height(); 

	mapViewplaneToScreen_();

	// vectors for arithmetics
	// TODO: give sensible names!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	Vector3 p_(0., 0., 0.);      // vector look_at ray ----> insertion ray cutting the nearplane
	Vector3 la_m_d_(0., 0., 0.); // look_at vector ray cutting the near plane
	Vector3 la_m_v_(0., 0., 0.); // look_at vector ray cutting the near plane
	Vector3 s_(0., 0., 0.);      // vector look_at_ray ----> insertion ray cutting viewing plane
	Vector3 k_(0., 0., 0.);      // vector of insertionpoint in the viewing volume

	// determine the vector/look_at ray : camera --> lookAt cuts the near plane
	la_m_d_=Vector3(  near_left_bot_
			+( (near_right_bot_ - near_left_bot_)*0.5 )
			+( (near_left_top_  - near_left_bot_)*0.5 )
			);	

	// determine the vector look_at point--->insertion_ray cutting the near plane 
	p_=Vector3((   near_left_top_  //c
				+ ( x / (float)xs_ * (near_right_bot_ - near_left_bot_) )  //b-a
				- ( y / (float)ys_ * (near_left_top_  - near_left_bot_) )  //c-a
				)
			- la_m_d_ );

	// determine the vector look_at_ray ----> insertion ray cutting viewing plane
	s_= Vector3(   ( ( getStage()->getCamera().getLookAtPosition() - getStage()->getCamera().getViewPoint() ).getLength()
				/ (la_m_d_ -  getStage()->getCamera().getViewPoint()).getLength()) 
			* p_ );

	// vector of insertionpoint in the viewing volume
	k_=Vector3( getStage()->getCamera().getLookAtPosition() + s_ );		

	return k_;
}	

// Set the element for the next insert operations
void EditableScene::setEditElementType(int element_number)
{
	atom_type_= element_number;
}

// Get the element for the next insert operations
int EditableScene::getEditElementType()
{
	return atom_type_;
}

void EditableScene::showContextMenu(QPoint pos)
{
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
	
	QAction* atom_mode = menu.addAction("Atom Mode", this, SLOT(atomMode_()));
	atom_mode->setCheckable(true);
	atom_mode->setChecked(current_mode_ == (Scene::ModeType) ATOM__MODE);
	
	QAction* bond_mode = menu.addAction("Bond Mode", this, SLOT(bondMode_()));
	bond_mode->setCheckable(true);
	bond_mode->setChecked(current_mode_ == (Scene::ModeType) BOND__MODE);

	menu.addSeparator();
	menu.addAction("Create a new molecule", this, SLOT(createMolecule_()));
	menu.addSeparator();

	if (current_mode_ == (Scene::ModeType) ATOM__MODE)
	{
		QAction* properties = menu.addAction("Properties", this, SLOT(atomProperties_()));
		properties->setEnabled(current_atom_ != 0);
		QAction* move = menu.addAction("Move", this, SLOT(moveAtom_()));
		move->setEnabled(current_atom_ != 0);
		QAction* change = menu.addAction("Change element", this, SLOT(changeElement_()));
		change->setEnabled(current_atom_ != 0);
		QAction* delete_atom = menu.addAction("Delete", this, SLOT(deleteAtom_()));
		delete_atom->setEnabled(current_atom_ != 0);
	}
	else if (current_mode_ == (Scene::ModeType) BOND__MODE)
	{
		QAction* delete_bond = menu.addAction("Delete", this, SLOT(deleteBond_()));
		delete_bond->setEnabled(current_bond_ != 0);

		QMenu* order = new QMenu();
		QAction* change = menu.addMenu(order);
		connect(order, SIGNAL(hovered(QAction*)), this, SLOT(activatedOrderItem_(QAction*)));
		change->setText("Change order");
		order->addAction("Single",    this, SLOT(changeBondOrder_()));
		order->addAction("Double",    this, SLOT(changeBondOrder_()));
		order->addAction("Triple",    this, SLOT(changeBondOrder_()));
		order->addAction("Quadruple", this, SLOT(changeBondOrder_()));
		order->addAction("Aromatic",  this, SLOT(changeBondOrder_()));
		change->setEnabled(current_bond_ != 0);
	}

	menu.exec(mapToGlobal(pos));

	// if we switched to move mode, let the user move the atom:
	if (current_mode_ == MOVE__MODE) return;

	// otherwise deselect all selected items
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
	if (!as.exec()) return;

	getMainControl()->update(*current_atom_, true);
}

void EditableScene::createMolecule_()
{
	System *system = new System();
	Molecule* current_molecule = new Molecule();
	system->insert(*current_molecule);
	getMainControl()->insert(*system);
}

void EditableScene::setMode(ModeType mode)
	throw()
{
	Scene::setMode(mode);

	if 			(mode == (Scene::ModeType) ATOM__MODE) 	atomMode_();
	else if (mode == (Scene::ModeType) BOND__MODE) pickingMode_();
}

	}//end of namespace 
} //end of namespace
