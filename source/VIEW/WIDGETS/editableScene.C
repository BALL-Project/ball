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
				atom_limit_(1.5),
				bond_limit_(2.0),
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
				atom_limit_(1.5),
				bond_limit_(2.0),
				editAtomType_(0),
				undo_()
		{
		}

		// TODO: Was sollte ein Copyconstructor sinnvolles tun?
		// undo_ is NOT copied, since we would run into trouble with the pointers to atoms and bonds it saves
		EditableScene::EditableScene(const EditableScene& eScene, QWidget* parent_widget, const char* name , WFlags w_flags)
			throw()
			: Scene(eScene, parent_widget, name, w_flags),
				edit_id_(-1),
				system_(eScene.system_),
				first_atom_for_bond_(eScene.first_atom_for_bond_),
				atom_limit_(eScene.atom_limit_),
				bond_limit_(eScene.bond_limit_),
				editAtomType_(),
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

			// we don't have to handle the other modes here:
			//   if we are in BOND__MODE, a mouse button is already pressed and we have to
			//      wait for the next release event
			//   if we are in any other mode, we let Scene take care of it
			if (current_mode_ == (Scene::ModeType)EDIT__MODE)
			{
				if(e->button() == Qt::LeftButton)
				{	
					// ToDo: Is the representation ok? Can the user see all actual atoms?

					// if no atom is selected
					PDBAtom* a = new PDBAtom(PTE[editAtomType_], PTE[editAtomType_].getName());
					insert_(e->x(), e->y(), *a);		
					first_atom_for_bond_ = a;
					current_mode_ =(Scene::ModeType)BOND__MODE;
					
					//store the Operation in undo_
					EditOperation eo( *a, NULL, "added atom" , EditOperation::ADDED__ATOM);
					undo_.push_back(eo);
	

					return;
				}
				if(e->button() == Qt::RightButton )
				{
					// find an atom in a radius of limit_ around the current mouse position
					Atom *atom = getClickedAtom_(e->x(), e->y());

					if (atom)
					{
						atom->select(); // later we should open a context menu at this point
						CompositeMessage *m = new CompositeMessage(*atom, CompositeMessage::SELECTED_COMPOSITE);	
						
						notify_(m); 
					}
					else
					{
						// try to find a bond
						Bond *bond = getClickedBond_(e->x(), e->y());

						if (bond)
						{
							bond->select();
							CompositeMessage *m = new CompositeMessage(*bond, CompositeMessage::SELECTED_COMPOSITE);	

							notify_(m); 
						}
					}

					return;
				}
				if(e->button() == Qt::MidButton)
				{
					//is there an atom in radius <= limit_  Anstroem? 
					Atom *atom = getClickedAtom_(e->x(), e->y());

					// if so, start a new bond from this atom
					if (atom)
					{
						first_atom_for_bond_ = atom;
						current_mode_ =(Scene::ModeType)BOND__MODE;
						TVector2<Position> pos =  getScreenPosition_(atom->getPosition());

						// start position for the putative bond
						x_ewindow_bond_pos_first_ = pos.x;
						y_ewindow_bond_pos_first_ = pos.y;

						// we have to initialize this here, so that the first call of MouseMoveEvent does not
						// fail when it tries to erase the old bond
						x_ewindow_bond_pos_second_old_ = pos.x;
						y_ewindow_bond_pos_second_old_ = pos.y;
					}

					return;	
				}
			}

			// nothig to do for us, let's see if Scene want's to do anything
			Scene::mousePressEvent(e);
		}

		void EditableScene::mouseMoveEvent(QMouseEvent *e)
		{
			// save the current mouse position for drawing possible bonds as a line
			// these values are used as endpoints of the line we draw if we are not
			// currently over an atom
			x_ewindow_bond_pos_second_new_ = e->x();
			y_ewindow_bond_pos_second_new_ = e->y();


			// ============ bond mode ================
			if (current_mode_ == (Scene::ModeType)BOND__MODE)
			{

				if (e->state() == Qt::LeftButton  ||
						e->state() == Qt::MidButton )
				{
					mouse_has_moved_ = true;

					//is there an atom nearby the actual mouse position? 
					Atom *atom = getClickedAtom_(e->x(), e->y());

					// have we found such an atom? if so, is it different from the one we started with? (self bonds make no sense :-) )
					if (atom && (atom != first_atom_for_bond_))
					{
						TVector2<Position> pos =  getScreenPosition_(atom->getPosition());

						// if we are really close to an atom, the endpoints of the line we draw will be set to
						// its center, so that the user has a drop in effect for the bonds
						x_ewindow_bond_pos_second_new_ = pos.x; 
						y_ewindow_bond_pos_second_new_ = pos.y;
					}
					
					//paint the line representing the offered bond
					QPainter painter(this);
					painter.setPen(white);

					// this allows to (a) draw or (b) erase, depending if a line was already drawn on 
					// the same position
					painter.setRasterOp(XorROP);

					// this erases the old line from the last move event
					painter.drawLine(
							(int) (x_ewindow_bond_pos_second_old_) ,  
							(int) (y_ewindow_bond_pos_second_old_),   
							(int) (x_ewindow_bond_pos_first_),
							(int) (y_ewindow_bond_pos_first_));

					// this draws the new line
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

			// maybe Scene wants to do anything with this event as well. and if we're not in BOND__MODE, we let it... :-)
			if (current_mode_ != (Scene::ModeType)BOND__MODE)
				Scene::mouseMoveEvent(e);
		}

		void EditableScene::mouseReleaseEvent(QMouseEvent *e)
		{
			// save the current position as the new start position
			x_ewindow_bond_pos_second_new_ = e->x();
			y_ewindow_bond_pos_second_new_ = e->y();

			// are we in BOND__MODE? 
			if (current_mode_ == (Scene::ModeType)BOND__MODE)
			{
				// delete last symbolic bond line	
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

				//is there an atom in radius "limit_" Angstroem?
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
						// TODO: - make the bond_order variable. Even better: change order with click on the bond
						//       - if there is already a bond, change it to a double bond
						Bond* c = new Bond("Bond", *first_atom_for_bond_, *atom, Bond::ORDER__DOUBLE);		
						
						EditOperation eo( *c, NULL, "added bond" , EditOperation::ADDED__BOND);
						undo_.push_back(eo);
	

						//update representation
						CompositeMessage *m = 0;
						m = new CompositeMessage(*atom, CompositeMessage::CHANGED_COMPOSITE_HIERARCHY);	
						
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
						
						//store the Operation in undo_
						EditOperation eo( *a, NULL, "added atom" , EditOperation::ADDED__ATOM);
						undo_.push_back(eo);
	
						//TODO: test if they have the same position, i.e. a and first_atom_for_bond!
						if (a->getPosition() == first_atom_for_bond_->getPosition())
						{
							Log.error() << "Warning: inserted two atoms at the same position!" << endl;
						}
	
						//set the bond
						//update representation
						// TODO: bond_oder
						CompositeMessage *m = 0;
						Bond* c = new Bond("Bond", *first_atom_for_bond_, *a, Bond::ORDER__DOUBLE);		
						m = new CompositeMessage(*a, CompositeMessage::CHANGED_COMPOSITE_HIERARCHY);	
						
						EditOperation eo( *c, NULL, "added bond" , EditOperation::ADDED__BOND);
						undo_.push_back(eo);
	
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

			// in EDIT__MODE, we don't need to do anything
			if (current_mode_ == (Scene::ModeType)EDIT__MODE)
			{
				return;
			}	

			Scene::mouseReleaseEvent(e);
		}	
		

		/** ******************** Helper Functions ************************* **/

		// Find closest atom to screen position (x,y). If there is none closer than atom_limit_, return NULL
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
				//check if composite is a system
				// TODO: do we have to check for Protein, Molecule, Chain, ... or is the System check already sufficient?
				if(RTTI::isKindOf<System>(**it))
				{
					s = RTTI::castTo<System>(**it);
				}	
				else
				{
					break;
				}

				//The Composite is a system
				if (s!=0)
				{
					// save the atom and its distance to the click ray
					std::pair<Atom*, double> pair_;
					double dist;

					Vector3 cam_to_atom;
					Vector3 cam_to_clickedPoint = clickedPointOnViewPlane_(x, y) - getStage()->getCamera().getViewPoint();

					AtomIterator ai;
					for(ai=s->beginAtom();+ai;++ai)
					{
						cam_to_atom = (ai->getPosition() - getStage()->getCamera().getViewPoint());

						// compute the angle between the rays Cam->Atom and Cam->clicked point
						Angle	alpha((float)acos(  (cam_to_atom * cam_to_clickedPoint)
												             /(cam_to_atom.getLength() * cam_to_clickedPoint.getLength())
									                 ));

						// the distance between the two rays is the sine of the angle between them times the length of Cam->Atom	
						dist  = sin(alpha) * cam_to_atom.getLength();
						
						// now save atom and distance
						pair_.first = &(*ai);
						pair_.second = dist;
						
						distvec_.push_back(pair_);
					}
				}
			}

			// find the atom with minimal distance to the click point
			std::pair<Atom*, double> minimum;
			minimum.second = atom_limit_ + 100;

			std::vector< std::pair <Atom*, double> >  ::iterator pair_it = distvec_.begin();	
			for (; pair_it != distvec_.end(); pair_it++)
			{
				if(pair_it->second < minimum.second)
					minimum = *pair_it;	
			}	

			//is the minimal distance beyond the threshold?
			if (minimum.second < atom_limit_)
			{
				return minimum.first;
			}

			return 0;
		}

		// Find closest bond to screen position (x,y). If there is none closer than bond_limit_, return NULL
		// Note: this code is very similar to getClickedAtom. Maybe those two should be united.
		Bond* EditableScene::getClickedBond_(int x, int y)
		{
			// vector to store all bonds and their distances from clicking-ray
			vector< std::pair <Bond*, double> > distvec_;

			//get the AtomContainer
			CompositeManager& cm = getMainControl()->getCompositeManager();
			CompositeManager::iterator it = cm.begin();

			System *s=0;
			for (; it != cm.end(); it++)
			{
				//check if composite is a system
				// TODO: do we have to check for Protein, Molecule, Chain, ... or is the System check already sufficient?
				if(RTTI::isKindOf<System>(**it))
				{
					s = RTTI::castTo<System>(**it);
				}	
				else
				{
					break;
				}

				//The Composite is a system
				if (s!=0)
				{
					// save the atom and its distance to the click ray
					std::pair<Bond*, double> pair_;
					double dist;

					Vector3 cam_to_bond;
					Vector3 cam_to_clickedPoint = clickedPointOnViewPlane_(x, y) - getStage()->getCamera().getViewPoint();

					// To iterate over all bonds, we have to iterate over all atoms and then over all their bonds
					// Unfortunately, this counts each bond twice...
					AtomIterator ai;

					for (ai=s->beginAtom();+ai;++ai)
					{
						AtomBondIterator bi;
						for(bi=ai->beginBond();+bi;++bi)
						{
							// first point the position vector to the first atom of the bond
							cam_to_bond = (bi->getFirstAtom()->getPosition() - getStage()->getCamera().getViewPoint());
							// then add 1/2 * the vector pointing from first to second
							cam_to_bond += (bi->getSecondAtom()->getPosition() - bi->getFirstAtom()->getPosition())*0.5;

							// compute the angle between the rays Cam->Bond and Cam->clicked point
							Angle	alpha((float)acos(  (cam_to_bond * cam_to_clickedPoint)
										/(cam_to_bond.getLength() * cam_to_clickedPoint.getLength())
										));

							// the distance between the two rays is the sine of the angle between them times the length of Cam->Bond
							dist  = sin(alpha) * cam_to_bond.getLength();

							// now save bond and distance
							pair_.first = &(*bi);
							pair_.second = dist;

							distvec_.push_back(pair_);
						}
					}
				}
			}

			// find the atom with minimal distance to the click point
			std::pair<Bond*, double> minimum;
			minimum.second = bond_limit_ + 100;

			std::vector< std::pair <Bond*, double> >  ::iterator pair_it = distvec_.begin();	
			for (; pair_it != distvec_.end(); pair_it++)
			{
				if(pair_it->second < minimum.second)
					minimum = *pair_it;	
			}	

			//is the minimal distance beyond the threshold?
			if (minimum.second < bond_limit_)
			{
				return minimum.first;
			}

			return 0;
		}

		// Slot to change to BOND__MODE
		void EditableScene::bondMode_()
		{
			current_mode_ = (Scene::ModeType)BOND__MODE;		
			setCursor(QCursor(Qt::SizeAllCursor));
			//ToDo:: Cursor should look different
		}


		// Slot to change to EDIT__MODE
		void EditableScene::editMode_()
		{
			current_mode_ = (Scene::ModeType)EDIT__MODE;		
			setCursor(QCursor(Qt::SizeAllCursor));
			//ToDo:: Cursor should look different
		}

		// insert an atom at screen positions (x,y) on the view plane
		void EditableScene::insert_(int x, int y, PDBAtom &atom_)
		{
			// find the 3D coordinates of screen position (x,y) on the view plane
			Vector3 point = clickedPointOnViewPlane_(x,y);
			// move the atom to that position
			atom_.setPosition( point );

			// now we need to find the AtomContainer into which we will insert the atom.
			
			// do we need to refocus after the update? if there was no composite at all before our
			// insertion, we need to focus since otherwise the camera will not be initialized correctly.
			// but we will _not_ refocus if the camera position was already valid. this would confuse
			// the user.
			bool refocus = (getMainControl()->getCompositeManager().getNumberOfComposites() == 0);
			//message to update the representation
			CompositeMessage *m = 0; 
	
			// get all highlighted composites
			List <Composite * > compositeList = getMainControl()->getMolecularControlSelection(); 
			List <Composite *>::iterator it = compositeList.begin();
			
			AtomContainer* ai = 0;
			
			// only one highlighted composite
			if(compositeList.size() == 1)
			{
				// is it an AtomContainer?
				if(RTTI::isKindOf<AtomContainer>( **it))
				{
					// Yes? we do not need to create our own system
					ai = RTTI::castTo<AtomContainer>(**it);
					ai->insert(atom_);
			
					m = new CompositeMessage(*ai, CompositeMessage::CHANGED_COMPOSITE_HIERARCHY);	
					notify_(m); 
				}
			}
			else  // more or less than 1 highlighted
			{
				if(compositeList.size() > 1 )
				{
					Log.error() <<"Please highlight exactly one AtomContainer for insertion of the created atoms!" << endl;
				}
				else
				{
					System *system = new System();
					Molecule* current_molecule = new Molecule();
					system->insert(*current_molecule);
					current_molecule->insert(atom_);
					m = new CompositeMessage(*system, CompositeMessage::NEW_COMPOSITE);	
					notify_(m); 
				}	
			}	

			// do we need to refocus the camera?
			if (refocus)
			{
				m = new CompositeMessage(atom_, CompositeMessage::CENTER_CAMERA);
				notify_(m);
				
				//move the mouse to focused position for the draw bond code
				x_ewindow_bond_pos_first_  = width() / 2.;
				y_ewindow_bond_pos_first_  = height() / 2.;
				x_ewindow_bond_pos_second_old_ = width() / 2.;
				y_ewindow_bond_pos_second_old_ = height() / 2.;
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
			near_left_bot_ =Vector3(left_,  bottom_, near_*-1.); //a
			near_right_bot_=Vector3(right_, bottom_, near_*-1.); //b
			near_left_top_ =Vector3(left_,  top_,    near_*-1.);  //c	

			near_left_bot_  = inverse_mod_view_mat_*near_left_bot_;
			near_right_bot_ = inverse_mod_view_mat_*near_right_bot_;
			near_left_top_  = inverse_mod_view_mat_*near_left_top_;

//			std::cout << "\tPROJECTION VARIABLES: " << std::endl;
//			std::cout << "near:" << near_ << " left:" << left_ << " right:" << right_<< " top:" << top_ << " bottom:" << bottom_ << std::endl;

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


		// Convert 2D screen coordinate to 3D coordinate on the view plane
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
			editAtomType_=element_number;
		}

	}//end of namespace 
} //end of namespace
