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

#include <qpainter.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qcursor.h>

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
	: operationType_(),
		atom_(),
		bond_(),
		description_()
{ 
}
	
EditableScene::EditOperation::EditOperation(Atom* atom, Bond* bond, String description, int operation)
	throw()
	: operationType_((EditableScene::EditOperation::OperationType)operation),
		atom_(atom),
		bond_(bond),
		description_(description)
{
}

EditableScene::EditOperation::EditOperation(const EditOperation& eOperation)
	throw()
	: operationType_(eOperation.operationType_),
		atom_(eOperation.atom_),
		bond_(eOperation.bond_),
		description_(eOperation.description_)
{
}

EditableScene::EditOperation::~EditOperation()	
	throw()
{
	#ifdef BALL_VIEW_DEBUG
		Log.info() << "Destructing object EditOperation " << this << " of class EditableScene>" << std::endl;
	#endif 
}



//
//-------------------- EditableScene -----------------------
//
EditableScene::EditableScene()
	throw()
	:	Scene(),
		edit_id_(-1),
		system_(),
		first_atom_for_bond_(0),
		limit_(1.5),
		editAtomType_(0),
		undo_()
{
}

EditableScene::EditableScene(QWidget* parent_widget, const char* name, WFlags w_flags)
	throw()
	: Scene(parent_widget, name, w_flags),
		edit_id_(-1),
		system_(), 
		first_atom_for_bond_(0),
		limit_(1.5),
		editAtomType_(0),
		undo_()
{
}

// TODO: Was sollte ein Copyconstructor sinnvolles tun?
// undo_ is NOT copied
EditableScene::EditableScene(const EditableScene& eScene, QWidget* parent_widget, const char* name , WFlags w_flags)
	throw()
	: Scene(eScene, parent_widget, name, w_flags),
		edit_id_(-1),
		system_(eScene.system_),
		first_atom_for_bond_(eScene.first_atom_for_bond_),
		limit_(eScene.limit_),editAtomType_(),
		undo_()
{
	//current_molecule_ = new Molecule();
	//current_molecule_ = eScene.current_molecule_;
	//system_.insert(current_molecule_);
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
	(main_control.initPopupMenu(MainControl::DISPLAY))->setCheckable(true);

	String hint;
	main_control.insertPopupMenuSeparator(MainControl::DISPLAY);

	hint = "Switch to edit mode";
	edit_id_ =	main_control.insertMenuEntry(
			MainControl::DISPLAY, "&Edit Mode", this, SLOT(editMode_()), CTRL+Key_E, -1, hint);

	Scene::initializeWidget(main_control);

	return;
}


void EditableScene::finalizeWidget(MainControl& main_control)
	throw()
{
  if (current_mode_ == (Scene::ModeType)EDIT__MODE)
	{
		main_control.removeMenuEntry(MainControl::DISPLAY, "&Edit Mode", this, SLOT(editMode_()), CTRL+Key_E);
	}
	Scene::finalizeWidget(main_control);	
}


void EditableScene::checkMenu(MainControl& main_control)
	throw()
{
	menuBar()->setItemChecked(edit_id_,(current_mode_ == (Scene::ModeType)EDIT__MODE));
	Scene::checkMenu(main_control);
}



void EditableScene::mousePressEvent(QMouseEvent* e)
{
	// start a new mouse_moved cycle
	mouse_has_moved_ = false;

	//store the old positions of the press Event
	x_ewindow_bond_pos_first_ = e->x();
	y_ewindow_bond_pos_first_ = e->y();
	x_ewindow_bond_pos_second_old_ = e->x();
  y_ewindow_bond_pos_second_old_ = e->y();
	
	if (current_mode_ == (Scene::ModeType)EDIT__MODE)
	{
		if(e->button() == Qt::LeftButton )
		{	
			// ToDo: Is the representation ok? Can the user see all aktual atoms?
			
			// if no atom is selected
			PDBAtom* a = new PDBAtom(PTE[editAtomType_], PTE[editAtomType_].getName());
			insert_(e->x(), e->y(), *a);		
			first_atom_for_bond_ = a;
			Log.info() << e->x() << " " << e->y() << endl;
			current_mode_ =(Scene::ModeType)BOND__MODE;
			return;
		}
		if(e->button() == Qt::RightButton )
		{
			//TODO:: Proof, wheather Atom is defined 
			Atom *atom = getClickedAtom_(e->x(), e->y());

			// TEST
			if (atom)
				atom->select();

			Log.info() << atom << " selected" << std::endl;
			return;
			
		}
		if(e->button() == Qt::MidButton)
		{
			//is there an atom in radius 1??  Anstroem? 
			Atom *atom = getClickedAtom_(e->x(), e->y());

			// TEST
			if (atom)
			{
				first_atom_for_bond_ = atom;
				current_mode_ =(Scene::ModeType)BOND__MODE;
				TVector2<Position> pos =  getScreenPosition_(atom->getPosition());
				x_ewindow_bond_pos_first_ = pos.x;
				y_ewindow_bond_pos_first_ = pos.y;
				x_ewindow_bond_pos_second_old_ = pos.x;
				y_ewindow_bond_pos_second_old_ = pos.y;
//				x_ewindow_bond_pos_second_new_ = pos.x; 
//				y_ewindow_bond_pos_second_new_ = pos.y;
			}	
		}
	}
	Scene::mousePressEvent(e);
}



void EditableScene::mouseMoveEvent(QMouseEvent *e)
{
	x_ewindow_bond_pos_second_new_ = e->x();
  y_ewindow_bond_pos_second_new_ = e->y();


  // ============ bond mode ================
  if (current_mode_ == (Scene::ModeType)BOND__MODE)
  {
		Log.info()<< "MouseMoveEvent: x_e_pos_old" << x_ewindow_bond_pos_second_old_ << "\n e_bond_first:" << x_ewindow_bond_pos_first_ << "\n x_e_pos_new : "<< x_ewindow_bond_pos_second_new_ << endl; 
  	
		if (e->state() == Qt::LeftButton  ||
        e->state() == Qt::MidButton )
		{
			mouse_has_moved_ = true;

			//is there an atom nearby the actual mouse position? 
			Atom *atom = getClickedAtom_(e->x(), e->y());

			if (atom && (atom != first_atom_for_bond_))
			{
				Log.info() << "Atom gefunden! " << endl;
				TVector2<Position> pos =  getScreenPosition_(atom->getPosition());
				x_ewindow_bond_pos_second_new_ = pos.x; 
  			y_ewindow_bond_pos_second_new_ = pos.y;
			}
			//paint the line representing the offered bond
			QPainter painter(this);
			painter.setPen(white);
			painter.setRasterOp(XorROP);

			painter.drawLine(
					(int) (x_ewindow_bond_pos_second_old_) ,  
					(int) (y_ewindow_bond_pos_second_old_),   
					(int) (x_ewindow_bond_pos_first_),
					(int) (y_ewindow_bond_pos_first_));


			painter.drawLine(
					(int) (x_ewindow_bond_pos_second_new_) , 
					(int) (y_ewindow_bond_pos_second_new_), 
					(int) (x_ewindow_bond_pos_first_),
					(int) (y_ewindow_bond_pos_first_));

			painter.end();

		}
  }
	
	//tidy up
	x_ewindow_bond_pos_second_old_ =	x_ewindow_bond_pos_second_new_ ;
	y_ewindow_bond_pos_second_old_ =	y_ewindow_bond_pos_second_new_ ;
			
	x_ewindow_bond_pos_second_new_ = e->x();
  y_ewindow_bond_pos_second_new_ = e->y();
	
	Scene::mouseMoveEvent(e);
}


Atom* EditableScene::getClickedAtom_(int x, int y)
{
	// vector to store all atoms and their distances from clicking-ray
	vector< std::pair <Atom*, double> > distvec_;
	
	//get the AtomContainer
	CompositeManager& cm = getMainControl()->getCompositeManager();
	CompositeManager::iterator it = cm.begin();
	System *s=0;
	for (; it != cm.end(); it++)
	{
		//check if composite is a system or a molecule
		if(RTTI::isKindOf<System>(**it))
		{
			s = RTTI::castTo<System>(**it);
			Log.error() << "System" << endl;
		}	
		else
		{
			Log.error() << "Composite has not a useable type!!  " << endl;
			break;
		}

		//The Composite is a system/molecule
		if (s!=0)
		{
			std::pair<Atom*, double> pair_;
			double dist;
			AtomIterator ai;
			for(ai=s->beginAtom();+ai;++ai)
			{
				Vector3 cam_to_atom(0. , 0., 0.);
				Vector3 cam_to_clickedPoint(0. , 0., 0.);

				cam_to_atom = (ai->getPosition() - getStage()->getCamera().getViewPoint());
				Log.info() << "cam to atom: " << cam_to_atom << std::endl;
				cam_to_clickedPoint = clickedPointOnViewPlane_(x, y) - getStage()->getCamera().getViewPoint();

				Angle	alpha((float)acos(  (cam_to_atom * cam_to_clickedPoint)
							/(cam_to_atom.getLength() * cam_to_clickedPoint.getLength())
							)); 
				dist  = sin(alpha) * cam_to_atom.getLength();
				pair_.first = &(*ai);
				pair_.second = dist;
				distvec_.push_back(pair_);
			}
		}
	}	
	std::pair<Atom*, double> minimum;
	minimum.second = limit_ + 100; //first_atom_for_bond_

	//is the minimal distance beyond a boundary 
	std::vector< std::pair <Atom*, double> >  ::iterator pair_it = distvec_.begin();	
	for (; pair_it != distvec_.end(); pair_it++)
	{
	//	Log.info() << "distance: " << pair_it->second << std::endl;
		if(pair_it->second < minimum.second)
			minimum = *pair_it;	
	}	

	if (minimum.second < limit_)
	{
		//Log.info()<<"atom found" << std::endl;
		return minimum.first;
	}

	return 0;
}


void EditableScene::mouseReleaseEvent(QMouseEvent *e)
{
	x_ewindow_bond_pos_second_new_ = e->x();
  y_ewindow_bond_pos_second_new_ = e->y();

	if (current_mode_ == (Scene::ModeType)BOND__MODE)
	{
		// delete last symbol bond line	
		QPainter painter(this);
		painter.setPen(white);
		painter.setRasterOp(XorROP);

		// did we paint a line at all?
		if (mouse_has_moved_)
		{
			painter.drawLine(
						(int) (x_ewindow_bond_pos_second_old_) ,  
						(int) (y_ewindow_bond_pos_second_old_),   
						(int) (x_ewindow_bond_pos_first_),
						(int) (y_ewindow_bond_pos_first_));
		}

		//is there an atom in radius 1 Angstroem
		Atom *atom = getClickedAtom_(e->x(), e->y());

		// decide what to do... did we find an atom at all?
		if (atom)
		{
			// is it the atom we started with?
			if (atom == first_atom_for_bond_)
			{
				// in this case, we assume that the user does not want to set a bond
				// -> do nothing
			}
			else // we found _another_ atom
			{
				//set the bond
				Bond* c = new Bond("first try", *first_atom_for_bond_, *atom, Bond::ORDER__DOUBLE);		
				//update representation
				CompositeMessage *m = 0;
				m = new CompositeMessage(*atom, CompositeMessage::CHANGED_COMPOSITE_AND_UPDATE_MOLECULAR_CONTROL);	
				notify_(m); 
			}	
		}
		else // no atom found
		{
			//did we find a first atom?
			if(first_atom_for_bond_!=0)
			{
				// build a new atom...
				PDBAtom* a = new PDBAtom(PTE[editAtomType_], PTE[editAtomType_].getName());
				insert_(e->x(), e->y(), *a);
				//TODO: test if they have the same position, i.e. a and first_atom_for_bond!
				if (a->getPosition() == first_atom_for_bond_->getPosition())
				{
					Log.error() << "Warning: inserted two atoms at the same position!" << endl;
				}
				//set the bond
				//update representation
				CompositeMessage *m = 0;
				Bond* c = new Bond("first try", *first_atom_for_bond_, *a, Bond::ORDER__DOUBLE);		
				m = new CompositeMessage(*a, CompositeMessage::CHANGED_COMPOSITE_AND_UPDATE_MOLECULAR_CONTROL);	
				notify_(m);
			}
			else // we did not find a first atom!
			{	
				// we do nothing 
			}
		}
		// end the mouse_moved cycle
		mouse_has_moved_ = false;
		first_atom_for_bond_=0;

		//return to edit mode
		current_mode_ =(Scene::ModeType)EDIT__MODE;

		return;
	}
	
	if (current_mode_ == (Scene::ModeType)EDIT__MODE)
	{
		return;
	}	

	Scene::mouseReleaseEvent(e);
}


void EditableScene::bondMode_()
{
	current_mode_ = (Scene::ModeType)BOND__MODE;		
	setCursor(QCursor(Qt::SizeAllCursor));
	//ToDo:: Cursor should look different
}


void EditableScene::editMode_()
{
	current_mode_ = (Scene::ModeType)EDIT__MODE;		
	setCursor(QCursor(Qt::SizeAllCursor));
	//ToDo:: Cursor should look different
}


void EditableScene::insert_(int x, int y, PDBAtom &atom_)
{
	Vector3 point = clickedPointOnViewPlane_(x,y);
	atom_.setPosition( point );

	Log.info() << "Atom inserted" << std::endl;

	//where should the atom be inserted? 
	//Is there an existing AtomContainer? 
	CompositeManager& cm = getMainControl()->getCompositeManager(); 
	System* system = 0;
	List <Composite * > compositeList = getMainControl()->getMolecularControlSelection(); 
	bool newsystem = true;
	List <Composite *>::iterator it = compositeList.begin(); 
	AtomContainer* 	ai = 0;
	bool refocus = (cm.getNumberOfComposites() == 0);

	//message to update the representation
	CompositeMessage *m = 0; 
	
	// CompositeList is the list of _highlighted_ composites
	if(compositeList.size() ==1 )
	{
		if(RTTI::isKindOf<AtomContainer>( **it))
		{
			ai = RTTI::castTo<AtomContainer>(**it);
			newsystem = false;
			ai->insert(atom_);
			m = new CompositeMessage(*ai, CompositeMessage::CHANGED_COMPOSITE_AND_UPDATE_MOLECULAR_CONTROL);	
			notify_(m); 
		}
	}
	else  // more or less than 1 highlighted
	{
		if(compositeList.size() > 1 )
		{
			Log.error() <<"Please insert highlight exactly one AtomContainer" << endl;
		}
		else
		{
			system = new System();
			newsystem = true;
			Molecule* current_molecule = new Molecule();
			system->insert(*current_molecule);
			current_molecule->insert(atom_);
			m = new CompositeMessage(*system, CompositeMessage::NEW_COMPOSITE);	
			notify_(m); 
		}	
	}	

	if (refocus)
	{
		m = new CompositeMessage(atom_, CompositeMessage::CENTER_CAMERA);
		notify_(m);
		//move the mouse to focued position
		x_ewindow_bond_pos_first_  = width() / 2.;
		y_ewindow_bond_pos_first_  = height() / 2.;
		x_ewindow_bond_pos_second_old_ = width() / 2.;
		y_ewindow_bond_pos_second_old_ = height() / 2.;
	}

	Log.info() << "Number of Composites after insertion" << cm.getNumberOfComposites() << std::endl;
}	


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
	near_left_bot_ =Vector3(left_,  bottom_, near_*-1.); //a
	near_right_bot_=Vector3(right_, bottom_, near_*-1.); //b
	near_left_top_ =Vector3(left_,  top_,    near_*-1.);  //c	

	near_left_bot_  = inverse_mod_view_mat_*near_left_bot_;
	near_right_bot_ = inverse_mod_view_mat_*near_right_bot_;
	near_left_top_  = inverse_mod_view_mat_*near_left_top_;

	std::cout << "\tPROJECTION VARIABLES: " << std::endl;
	std::cout << "near:" << near_ << " left:" << left_ << " right:" << right_<< " top:" << top_ << " bottom:" << bottom_ << std::endl;

	return true;
}

TVector2<Position> EditableScene::getScreenPosition_(Vector3 vec)
{
	//find the monitor coordinates of a given vector
	TVector2<Position> pos ; 
	
	double xs_ = width();
	double ys_ = height(); 
	
	// vectors for arithmetics
	// first we compute the projection of the given atom on the Viewplane
	Vector3 cam = getStage()->getCamera().getViewPoint();
	Vector3 look_at = getStage()->getCamera().getLookAtPosition() -  cam ;
	Vector3 cam_to_atom = vec - cam;

	try {

		if (mapViewplaneToScreen_())
		{
			Vector3 la_m_d(near_left_bot_
					+( (near_right_bot_ - near_left_bot_)*0.5 )
					+( (near_left_top_  - near_left_bot_)*0.5 ) - cam);

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


Vector3 EditableScene::clickedPointOnViewPlane_(int x, int y)
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

	std::cout << "v_p: " << getStage()->getCamera().getViewPoint() << std::endl;
	std::cout << "la_d: " << la_m_d_ << std::endl;
	std::cout << "l_a: " << getStage()->getCamera().getLookAtPosition() << std::endl;
	std::cout << "nlt: " << near_left_top_ << "  nrb: " << near_right_bot_ << "  nlb: " << near_left_bot_ << " " << std::endl;
	std::cout << " p_: " << p_ << std::endl;

	// determine the vector look_at_ray ----> insertion ray cutting viewing plane
	s_= Vector3(   ( ( getStage()->getCamera().getLookAtPosition() - getStage()->getCamera().getViewPoint() ).getLength()
				/ (la_m_d_ -  getStage()->getCamera().getViewPoint()).getLength()) 
			* p_ );

	std::cout << "\tStrahlensatzfaktor: " <<  getStage()->getCamera().getLookAtPosition().getLength()/ (la_m_d_.getLength()) << std::endl;
	std::cout << "Länge LookAt:" <<  (getStage()->getCamera().getLookAtPosition() - getStage()->getCamera().getViewPoint()).getLength() 
		<< " Länge LookAt-D:" << (la_m_d_.getLength()) << std::endl;


	std::cout << "Camera:" << getStage()->getCamera().getViewPoint() << std::endl;
	std::cout << "LookAt:" << getStage()->getCamera().getLookAtPosition() << std::endl;

	// vector of insertionpoint in the viewing volume
	k_=Vector3( getStage()->getCamera().getLookAtPosition() + s_ );		

	return k_;
}	

void EditableScene::setEditElementType(int element_number)
{
	 editAtomType_=element_number;
}

	}//end of namespace 
} //end of namespace
