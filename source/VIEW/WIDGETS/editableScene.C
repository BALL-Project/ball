/*    THIS IS EXPERIMENTAL CODE  
 *
 *    WE GIVE NO WARRANTY
 *    
 *    USE AT YOUR OWN RISK!!!!!!
 */

#include <BALL/VIEW/WIDGETS/editableScene.h>

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/mainControl.h>

#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qcursor.h>

#include <BALL/MATHS/vector3.h>
#include <BALL/MATHS/matrix44.h>

using std::endl;
using std::ostream;
using std::istream;


namespace BALL
{
	namespace VIEW
	{

EditableScene::EditableScene()
	throw()
	:	Scene(),
		edit_id_(-1)
{
	current_molecule_ = new Molecule();
}

EditableScene::EditableScene(QWidget* parent_widget, const char* name, WFlags w_flags)
	throw()
	: Scene(parent_widget, name, w_flags),
		edit_id_(-1)
{
	current_molecule_ = new Molecule();
}

EditableScene::EditableScene(const EditableScene& eScene, QWidget* parent_widget, const char* name , WFlags w_flags)
	throw()
	: Scene(eScene, parent_widget, name, w_flags),
		edit_id_(-1)
{
	current_molecule_ = new Molecule();
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
  if (current_mode_ == INSERT__MODE)
	{
		main_control.removeMenuEntry(MainControl::DISPLAY, "&Edit Mode", this, SLOT(editMode_()), CTRL+Key_E);
	}
	Scene::finalizeWidget(main_control);	
}


void EditableScene::checkMenu(MainControl& main_control)
	throw()
{
	menuBar()->setItemChecked(edit_id_,(current_mode_ == INSERT__MODE));
	Scene::checkMenu(main_control);
}



void EditableScene::mousePressEvent(QMouseEvent* e)
{
	// This is a TEST... instead of picking mode we will use edit mode later on...
	if (current_mode_ == INSERT__MODE)
	{
		int x_window_pos_old_ = e->x();
		int y_window_pos_old_ = e->y();

		PDBAtom* a = new PDBAtom(PTE[Element::C], "C");
		//PDBAtom* b = new PDBAtom(PTE[Element::C], "C");
	
	// matrix for the Projection matrix 	
		GLdouble mat_[16];
	// matrox for the Modelview matrix
		GLdouble mod_view_vec_[16];
			
	// 	Scale variables for Frustum
		double xs_ = (getGLRenderer_()).getXScale();
		double ys_ = (getGLRenderer_()).getYScale();

	// variables for definition of projection matrix
		float near_=0, left_=0, right_=0, bottom_ =0, top_=0; 

		// vectors of the nearplane
		Vector3 near_left_bot_(0. , 0., 0.);  //TODO:: name in XYZ left_bot_mnear
	  Vector3 near_right_bot_(0. , 0., 0.);
	  Vector3 near_left_top_(0. , 0., 0.);
		
		// vectors for arithmetics
		Vector3 p_(0., 0., 0.);      // vector look_at ray ----> insertion ray cutting the nearplane
		Vector3 la_m_d_(0., 0., 0.); // look_at vector ray cutting the near plane
		Vector3 s_(0., 0., 0.);      // vector look_at_ray ----> insertion ray cutting viewing plane
		Vector3 k_(0., 0., 0.);      // vector of insertionpoint in the viewing volume
		
	// take the Projection matrix	
		glMatrixMode(GL_PROJECTION);
		
		// !!!!!!!!!!! for testing
		glLoadIdentity();
		glFrustum(-2.*xs_, 2.*xs_, -2.*ys_, 2.*ys_, 1.5, 300.); // left, right, bottom, top, near, far 
		//!!!!!!!!!!!! end testing
		
		glGetDoublev(GL_PROJECTION_MATRIX, mat_);
		glMatrixMode(GL_MODELVIEW);
		glGetDoublev(GL_MODELVIEW_MATRIX, mod_view_vec_); 

		
	// determine the projection variables
		if(mat_[0]==0. || mat_[5]==0. || mat_[10]==1.)
		{	
			Log.error() << "Projection variables equal zero! " << endl;
			return;
		}	
		near_   = mat_[14]/(mat_[10]-1);
		left_   = mat_[14]*(mat_[8]-1) / (mat_[0]*(mat_[10]-1));
		right_  = mat_[14]*(mat_[8]+1) / (mat_[0]*(mat_[10]-1));
		bottom_ = mat_[14]*(mat_[9]-1) / (mat_[5]*(mat_[10]-1));
		top_    = mat_[14]*(mat_[9]+1) / (mat_[5]*(mat_[10]-1));
		
		std::cout << "PROJECTION VARIABLES " << std::endl;
		std::cout << "near:" << near_ << " left:" << left_ << " right:" << right_<< " top:" << top_ << " bottom:" << bottom_ << std::endl;
		std::cout << "FRUSTUM VALUEA " << std::endl;
		std::cout <<  "near:" << "1.5" << " left:" << -2.*xs_<< " right:" << 2.*xs_<< " top:" << 2.*ys_ << " bottom:" <<-2.*ys_ << std::endl;

	// determine the nearplane vectors
		near_left_bot_ =Vector3(left_,  bottom_, near_*-1.);
 		near_right_bot_=Vector3(right_, bottom_, near_*-1.);
		near_left_top_=Vector3(left_,  top_,    near_*-1.); 	

	// determine the vector look_at ray--->insertion_ray cutting the near plane 
		p_=Vector3(  near_left_bot_
							 + ( (e->x() / getGLRenderer_().getWidth() ) * (near_right_bot_ - near_left_bot_) )
							 + ( (e->y() / getGLRenderer_().getHeight()) * (near_left_top_  - near_left_bot_) )
 							);
	// determine the look_at ray cutting the near plane
		la_m_d_=Vector3(  near_left_bot_
										+( (near_right_bot_ - near_left_bot_)*0.5 )
										+( (near_left_top_  - near_left_bot_)*0.5 )
				           );	
	// determine the vector look_at_ray ----> insertion ray cutting viewing plane
		s_= Vector3(   ( ( stage_->getCamera().getLookAtPosition().getLength() ) / (la_m_d_.getLength()) ) 
			  			  	* p_ );
 
	// vector of insertionpoint in the viewing volume
		k_=Vector3( stage_->getCamera().getLookAtPosition() + s_ );		
		
	//we have to move the insertion point of the viewing volume with the inverted Modelview matrix 
		Matrix4x4 mod_view_mat_(mod_view_vec_[0], mod_view_vec_[4], mod_view_vec_[8], mod_view_vec_[12],
			 											mod_view_vec_[1], mod_view_vec_[5], mod_view_vec_[9], mod_view_vec_[13],
														mod_view_vec_[2], mod_view_vec_[6], mod_view_vec_[10], mod_view_vec_[14],
														mod_view_vec_[3], mod_view_vec_[7], mod_view_vec_[11],	mod_view_vec_[15]);
		
	
		Matrix4x4 inverse_mod_view_mat_;
		mod_view_mat_.invert(inverse_mod_view_mat_);

		a->setPosition(  inverse_mod_view_mat_ * k_ );
		
Log.error()<< "Hier!" << endl;		
		//b->setPosition(Vector3(e->x()-10.,e->y()-10., 0));
		//Bond* c = new Bond("first try", *a, *b, Bond::ORDER__DOUBLE);		
		current_molecule_->insert(*a);
		//current_molecule_->insert(*b);
		
Log.info() << "blubb" << std::endl;
		
		CompositeMessage *message_ = new CompositeMessage(*current_molecule_, CompositeMessage::CHANGED_COMPOSITE_AND_UPDATE_MOLECULAR_CONTROL);
		message_->setUpdateRepresentations(true);
		notify_(message_);
	}
	
	Scene::mousePressEvent(e);
}



void EditableScene::mouseMoveEvent(QMouseEvent *e)
{
	Scene::mouseMoveEvent(e);
}


void EditableScene::mouseReleaseEvent(QMouseEvent *e)
{
	Scene::mouseReleaseEvent(e);
}


void EditableScene::editMode_()
{
	current_mode_ = INSERT__MODE;		
	setCursor(QCursor(Qt::SizeAllCursor));
	CompositeMessage *m = new CompositeMessage(*current_molecule_, CompositeMessage::NEW_MOLECULE);	
	notify_(m);                                                                                    	
}

	}//end of namespace 
} //end of namespace
