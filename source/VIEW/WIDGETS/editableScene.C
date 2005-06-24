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
				Log.info() << "Destructing object EditOperation " << this << " of class EditableScene>" << std::endl;
#endif 
		}

		// undo an EditOperation
		
		void EditableScene::EditOperation::undo(EditableScene* es)
		{	
			// delete an atom
			if(operationType == ADDED__ATOM)
			{
				CompositeManager& cm = es->getMainControl()->getCompositeManager();

				//does the atom still exist?
				Log.info()<< cm.has(atom) <<std::endl;
			  if(cm.has(atom) )	
				{
					Composite* parent = atom->getParent();
					parent->removeChild(*atom);
					delete(atom);
				
					//update representation
					CompositeMessage *m = 0;
					m = new CompositeMessage(parent->getRoot(), CompositeMessage::CHANGED_COMPOSITE_HIERARCHY);	
					es->notify_(m); 

					Log.info()<< "Atom deleted"  << std::endl;
				}
			} 
			// delete a bond
			else if(operationType == ADDED__BOND)
			{
				//Composite* parent = bond->getParent();
				bond->destroy();
			
				//TODO: Die message funktioniert aus irgendeinem Grund nicht!!! 
				
				//CompositeMessage *m = 0;
				//m = new CompositeMessage(parent->getRoot(), CompositeMessage::CHANGED_COMPOSITE_HIERARCHY);	
				//es->notify_(m);
				Log.info()<< "Bond deleted"  << std::endl;
			}
			//change type
			else if(operationType == CHANGED__TYPE)
			{
				//TODO: Implementation
				Log.info()<< "Atomtype rechanged"  << std::endl;
			}
			else
			{
				Log.error() << "Operation is not undoable! " << std::endl;
			}
		}

		
		//
		//-------------------- EditableScene -----------------------
		//
		EditableScene::EditableScene()
			throw()
			:	Scene(),
				edit_id_(-1),
				//system_(),
				first_atom_for_bond_(0),
				context_menu_composite_(),
				context_menu_(),
				atom_limit_(1.5),
				bond_limit_(2.0),
				editAtomType_(6)
		{
		}

		EditableScene::EditableScene(QWidget* parent_widget, const char* name, WFlags w_flags)
			throw()
			: Scene(parent_widget, name, w_flags),
				edit_id_(-1),
				//system_(), 
				first_atom_for_bond_(0),
				context_menu_composite_(),
				context_menu_(),
				atom_limit_(1.5),  
				bond_limit_(2.0),
				editAtomType_(6)
		{	
			registerWidget(this); 
			Log.error() << "EditableScene " << dynamic_cast<ModularWidget*>(this) << std::endl;
		}

		// TODO: Was sollte ein Copyconstructor sinnvolles tun?
		// undo_ is NOT copied, since we would run into trouble with the pointers to atoms and bonds it saves
		EditableScene::EditableScene(const EditableScene& eScene, QWidget* parent_widget, const char* name , WFlags w_flags)
			throw()
			: Scene(eScene, parent_widget, name, w_flags),
				edit_id_(-1),
				//system_(eScene.system_),
				first_atom_for_bond_(eScene.first_atom_for_bond_),
				context_menu_composite_(),
				context_menu_(),
				atom_limit_(eScene.atom_limit_),
				bond_limit_(eScene.bond_limit_),
				editAtomType_(eScene.editAtomType_)
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

/*
		void EditableScene::onNotify(Message *message)
			throw()
		{
#ifdef BALL_VIEW_DEBUG
			Log.error() << "EditableScene " << this  << "onNotify " << message << std::endl;
#endif
			
			//TODO: Implementation
			
			if (RTTI::isKindOf<CompositeMessage>(*message)) 
			{
				CompositeMessage* cm = RTTI::castTo<CompositeMessage>(*message);
				switch (cm->getType())
				{
					case CompositeMessage::CHANGED_COMPOSITE_HIERARCHY:
						if() // emited by EditableScene itself? 
						{

						}
						break;
					//other cases
				  	default: 
							break;
					}	
				}
					
			Scene::onNotify(message);	
		}
*/	
		
		void EditableScene::onNotify(Message *message)
			throw()
		{	
#ifdef BALL_VIEW_DEBUG
			Log.error() << "EditableScene " << this  << "onNotify " << message << std::endl;
#endif
			
			// react now only to SceneMessage
			if (RTTI::isKindOf<CompositeMessage>(*message))
			{
				//
				// ToDO: Ueberpruefen, wie es sich bei geändertem higehlighten verhaelt! 
				//
				Composite* deleted_composite; 
				CompositeMessage *composite_message = RTTI::castTo<CompositeMessage>(*message);
				switch (composite_message->getType())
				{
					case CompositeMessage::REMOVED_COMPOSITE:
						// check wheather the removed Composite was an ancestor of the actual atomcontainer
						// editableScene was working on (current_atomContainer_) 
/*
						if( (composite_message->getComposite())->isAncestorOf(*current_atomContainer_))
						{ // an ancestor of our current atomcontainer was deleted 
							//TODO: Dieser Fall tritt irgendwie nie ein, KONTROLLIEREN!! Da immer oberstes System gehighlighted wird! 
							Log.info() << "Vorfahr wurde geloescht!!!"<< std::endl;
							current_atomContainer_=0;
						}
						else if(  current_atomContainer_->isAncestorOf( *(composite_message->getComposite())))
						{	// inheritor of current atomContainer was deleted 
							Log.info() << "Erbe wurde geloescht!!!"<< std::endl;
						  // TODO: Delete deleted composite from undolist 
						}
						else if(current_atomContainer_==composite_message->getComposite())
						{ //actual atomContainer was deleted
							current_atomContainer_=0;
							Log.info() << "Current atomcontainer was deleted"<< std::endl;
							// TODO: Delete all inheritors of deleted composite from undolist
						}
						else	
						{
						  // the deleted composite is not in relationship to the currend atomContainer
							// we should make sure, that none of the deleted composites occure any longer in 
							// our undo list
							Composite* deleted_composite = composite_message->getComposite();
							emit invalidComposite(deleted_composite);
	//	 ----> funktioniert noch nicht !!! EditOperationDialog::invalidateComposite
						
						}*/	
						deleted_composite = composite_message->getComposite();
						emit invalidComposite(deleted_composite);
						Log.info() << " invalid Composite angezeigt! "<< std::endl;
						break;	// no return since we want Scene::onNotify to be performed
						
					case CompositeMessage::UNDEFINED:
						Log.error() << "Unknown type of CompositeMessage in " << __FILE__ << __LINE__ << std::endl;
						break;
						
					default: // in all other cases 	Scene::onNotify(message) is called
						break;
				}
	
			}
			
			// A New Selection -> change of current_atomContainer_ is organized in the atom insertion procedure 
			
			Scene::onNotify(message);
			return;
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

		// initializes the context_menu
		void EditableScene::popupContextMenu_()
		{	
				context_menu_.clear();
				if(RTTI::isKindOf<Atom>(*context_menu_composite_))
				{
					//Atom* atom  = dynamic_cast<Atom*>(&composite);
					// context_menu_composite is atom 
					context_menu_.insertItem("select",     	this, SLOT(selectAtom()), 0, 				SELECT_ATOM);
					context_menu_.insertItem("properties", 	this, SLOT(setAtomProperties()), 0, SET_ATOM_PROPERTIES);
					context_menu_.insertItem("move", 				this, SLOT(moveAtom()), 0, 					MOVE_ATOM);

				}
				else if (RTTI::isKindOf<Bond>(*context_menu_composite_))
				{
					//Bond* bond  = dynamic_castTo<Bond*>(composite);
					// context_menu_composite is bond
					context_menu_.insertItem("select",			this,  SLOT(selectBond()), 0, 					SELECT_BOND); 
					context_menu_.insertItem("properites", 	this,  SLOT(setBondProperties()), 0, 	SET_BOND_PROPERTIES);
					context_menu_.insertItem("set length",  this,  SLOT(setBondLength()), 0, 			SET_BOND_LENGTH);
				}  
				
				//!! zum PopUp sollten die Orginal klick punkte mitgegeben werden
				//ToDO: Pruefe, ob man schon zum Rand hin rausfaellt! :-)) 
				//context_menu_.exec(QCursor::pos());
				context_menu_.exec(QPoint( (int) (x_ewindow_bond_pos_first_ + x()), 
																	 (int) (y_ewindow_bond_pos_first_ + y())));
				//!!!! x_ewindow_bond... sind relativ, da nur fuer die Scene muss noch addiert werden!!  
		}

		

		//    
		//               SLOTS 
		//
				
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
					bond_length_ = 0;
					TVector2<Position>	coords = getScreenPosition_(first_atom_for_bond_->getPosition());
					//ToDO: change the 30 to half height of mousecursor
					x_text_position_ = coords.x + 30;
					y_text_position_ = coords.y + 30;

					
					//store the Operation in EditOperationDialog
					Vector3 atom_position = a->getPosition();
					
					EditOperation eo(a, NULL, "Added atom of type " + PTE[editAtomType_].getName() + " at position (" 
													+ String(atom_position.x) + ", "
													+ String(atom_position.y) + ", "
													+ String(atom_position.z) + ")", EditOperation::ADDED__ATOM);

					// tell about the new undo operation
					emit newEditOperation(eo);
	
					return;
				}
				if(e->button() == Qt::RightButton )
				{
					// find an atom in a radius of limit_ around the current mouse position
					Atom *atom = getClickedAtom_(e->x(), e->y());

					if (atom)
					{
						context_menu_composite_=atom;
						popupContextMenu_();
						//atom->select(); 					
						//CompositeMessage *m = new CompositeMessage(*atom, CompositeMessage::SELECTED_COMPOSITE);	
						//notify_(m); 
					}
					else
					{
						// try to find a bond
						Bond *bond = getClickedBond_(e->x(), e->y());

						if (bond)
						{
							popupContextMenu_();
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
						bond_length_=0;
						TVector2<Position>	coords = getScreenPosition_(first_atom_for_bond_->getPosition());
					 	//change 30 to half of mouse cursor height
						x_text_position_ = coords.x + 30;
					 	y_text_position_ = coords.y + 30;
					
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

					// delete old bondlength remark
					QString bondstring;
					bondstring.setNum(bond_length_, 'f', 2);
					// don't do this for really short bonds...
					if (bond_length_ > 0.3)
					  painter.drawText((int)x_text_position_, (int)y_text_position_, bondstring);

					// this draws the new line
					painter.drawLine(
							(int) (x_ewindow_bond_pos_second_new_) , 
							(int) (y_ewindow_bond_pos_second_new_), 
							(int) (x_ewindow_bond_pos_first_),
							(int) (y_ewindow_bond_pos_first_));

					//draw the new bondlength	remark
					x_text_position_ =	x_ewindow_bond_pos_second_new_  ;
				  y_text_position_ = y_ewindow_bond_pos_second_new_ ;
					
				  bond_length_  = (atom && (atom!=first_atom_for_bond_)) 
													? (   atom->getPosition()
														  - first_atom_for_bond_->getPosition()).getLength()
													: (   clickedPointOnViewPlane_(e->x(), e->y()) 
													    - first_atom_for_bond_->getPosition()).getLength()
													;

					bondstring.setNum(bond_length_, 'f', 2);
					// don't do this for really short bonds...
					if (bond_length_ > 0.3)
					  painter.drawText(x_text_position_, y_text_position_, bondstring);
				
					
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
				
					// delete the bondlength-remark
					QString bondstring;
					bondstring.setNum(bond_length_, 'f', 2);
					// don't do this for really short bonds...
					if (bond_length_ > 0.3)
						painter.drawText(x_text_position_, y_text_position_,bondstring);
				
				}
				
				painter.end();	
				
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
						Bond* c = new Bond("Bond", *first_atom_for_bond_, *atom, Bond::ORDER__SINGLE);		
						
						String bond_string;
						int bond_type = c->getOrder();

						switch (bond_type)
						{
							case Bond::ORDER__SINGLE:
								bond_string = "single bond";
								break;
							case Bond::ORDER__DOUBLE:
								bond_string = "double bond";
								break;
							case Bond::ORDER__TRIPLE:						
								bond_string = "triple bond";	
								break;
							case Bond::ORDER__QUADRUPLE:
								bond_string = "quadruple bond";	
								break;
							case Bond::ORDER__AROMATIC:
								bond_string = "aromatic bond";	
								break;
							default:					
								bond_string = "unknown";	
								break;
						}

						EditOperation eo( NULL, c, "Added bond of type " + bond_string, EditOperation::ADDED__BOND);
					
						// tell about the new undo operation
						emit newEditOperation(eo);

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
						
						Vector3 atom_position = a->getPosition();
					
						EditOperation eo(a, NULL, "Added atom of type " + PTE[editAtomType_].getName() + " at position (" 
														+ String(atom_position.x) + ", "
														+ String(atom_position.y) + ", "
														+ String(atom_position.z) + ")", EditOperation::ADDED__ATOM);

						// tell about the new undo operation
						emit newEditOperation(eo);
	
						//TODO: test if they have the same position, i.e. a and first_atom_for_bond!
						if (a->getPosition() == first_atom_for_bond_->getPosition())
						{
							Log.error() << "Warning: inserted two atoms at the same position!" << endl;
						}
	
						//set the bond
						//update representation
						// TODO: bond_oder
						CompositeMessage *m = 0;
						Bond* c = new Bond("Bond", *first_atom_for_bond_, *a, Bond::ORDER__SINGLE);		
						m = new CompositeMessage(*a, CompositeMessage::CHANGED_COMPOSITE_HIERARCHY);	
					
						String bond_string;
						int bond_type = c->getOrder();

						switch (bond_type)
						{
							case Bond::ORDER__SINGLE:
								bond_string = "single bond";
								break;
							case Bond::ORDER__DOUBLE:
								bond_string = "double bond";
								break;
							case Bond::ORDER__TRIPLE:						
								bond_string = "triple bond";	
								break;
							case Bond::ORDER__QUADRUPLE:
								bond_string = "quadruple bond";	
								break;
							case Bond::ORDER__AROMATIC:
								bond_string = "aromatic bond";	
								break;
							default:					
								bond_string = "unknown";	
								break;
						}

						EditOperation eo2( NULL, c, "Added bond of type " + bond_string, EditOperation::ADDED__BOND);
						
						// tell about the new undo operation
						emit newEditOperation(eo2);

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


		void EditableScene::renderView_(RenderMode mode)
			throw()
		{
			Scene::renderView_(mode);
				drawRuler_();	
				Log.info() << "render" << std::endl;

		}

		void EditableScene::paintEvent(QPaintEvent*e)
		{
			Scene::paintEvent(e);
			static int i=0;
		//	if (current_mode_ == (Scene::ModeType)EDIT__MODE)
			{
				drawRuler_();
				Log.info() << "paint" << std::endl;
			}
		}
		
		//     slots for contextMenue
		void  EditableScene::moveAtom()
		{
			Log.error()<< "move atom"<< std::endl;
		}
		
		void  EditableScene::selectAtom()
		{
			Log.error()<< "select atom"<< std::endl;
			
			Atom* atom  = dynamic_cast<Atom*>(context_menu_composite_);
			atom->select(); 					
			CompositeMessage *m = new CompositeMessage(*atom, CompositeMessage::SELECTED_COMPOSITE);	
			notify_(m); 
		}
		
		void  EditableScene::setAtomProperties()
		{
			Log.error()<<"set atom property" << std::endl;

		}
		
		void  EditableScene::selectBond()
		{
			Log.error()<<"select bond" << std::endl;
		}
		
		void  EditableScene::setBondProperties()
		{
			Log.error()<<"set bond property" << std::endl;

		}
		
		void  EditableScene::setBondLength()
		{
			Log.error()<< "set bond length" << std::endl;

		}
				
		
		/** ******************** Helper Functions ************************* **/

		// Find closest atom to screen position (x,y). If there is none closer than atom_limit_, return NULL
		Atom* EditableScene::getClickedAtom_(int x, int y)
		{
			//get the AtomContainer
			CompositeManager& cm = getMainControl()->getCompositeManager();
			CompositeManager::iterator it = cm.begin();

			float min_dist = numeric_limits<float>::max();
			Atom* min_atom = 0;
			float dist;

			for (; it != cm.end(); it++)
			{
				//check if composite is a system
				// TODO: do we have to check for Protein, Molecule, Chain, ... or is the System check already sufficient?
				System* s = dynamic_cast<System*>(*it);
				if (s == 0) continue;

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
					if (dist < min_dist)
					{
						min_dist = dist;
						min_atom = &(*ai);
					}
				}
			}

			//is the minimal distance beyond the threshold?
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
			//get the AtomContainer
			CompositeManager& cm = getMainControl()->getCompositeManager();
			CompositeManager::iterator it = cm.begin();

			Bond* closest = 0;
			float min_dist = FLT_MAX;


			for (; it != cm.end(); it++)
			{
				//check if composite is a system
				// TODO: do we have to check for Protein, Molecule, Chain, ... or is the System check already sufficient?
				System* s = dynamic_cast<System*>(*it);
				if (s == 0) continue;

				//The Composite is a system
				// save the atom and its distance to the click ray
				Vector3 cam_to_bond;
				Vector3 cam_to_clickedPoint = clickedPointOnViewPlane_(x, y) - getStage()->getCamera().getViewPoint();

				// To iterate over all bonds, we have to iterate over all atoms and then over all their bonds
				// Unfortunately, this counts each bond twice...
				for (AtomIterator ai = s->beginAtom(); +ai; ++ai)
				{
					AtomBondIterator bi;
					for (bi = ai->beginBond(); +bi; ++bi)
					{
						if (bi->getPartner(*ai) < &*ai) continue;

						// first point the position vector to the first atom of the bond
						cam_to_bond = (bi->getFirstAtom()->getPosition() - getStage()->getCamera().getViewPoint());
						// then add 1/2 * the vector pointing from first to second
						cam_to_bond += (bi->getSecondAtom()->getPosition() - bi->getFirstAtom()->getPosition())*0.5;

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

			//is the minimal distance beyond the threshold?
			if (min_dist < bond_limit_)
			{
				return closest;
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
	
			// get all highlighted composites
			List <Composite * > compositeList = getMainControl()->getMolecularControlSelection(); 
			List <Composite *>::iterator it = compositeList.begin();
			
			if(compositeList.size() > 1 )
			{
				Log.error() <<"Please highlight exactly one AtomContainer for insertion of the created atoms!" << endl;
				return;
			}
		
			// only one highlighted composite
			if(compositeList.size() == 1)
			{
				// is it an AtomContainer?
				AtomContainer* ai = dynamic_cast<AtomContainer*>(*it);
				if (ai == 0)
				{
					Log.error() <<"Please highlight exactly one AtomContainer for insertion of the created atoms!" << endl;
					return;
				}

				// Yes? we do not need to create our own system
				current_atomContainer_ = ai;
				ai->insert(atom_);
				getMainControl()->update(*ai, true);
			}
			else 
			{	//  no highlighted composite => we create a new system, including a new molecule
				//  into which the new atom will be inserted! 
				System *system = new System();
				Molecule* current_molecule = new Molecule();
				system->insert(*current_molecule);
				current_molecule->insert(atom_);
				getMainControl()->insert(*system);
				current_atomContainer_ = current_molecule;
			}	

			// do we need to refocus the camera?
			if (refocus)
			{
				CompositeMessage* m = new CompositeMessage(atom_, CompositeMessage::CENTER_CAMERA);
				notify_(m);
				
				//move the mouse to focused position for the draw bond code
				x_ewindow_bond_pos_first_  = width() / 2.;
				y_ewindow_bond_pos_first_  = height() / 2.;
				x_ewindow_bond_pos_second_old_ = width() / 2.;
				y_ewindow_bond_pos_second_old_ = height() / 2.;
			
							
		/*		//we need to draw the ruler axis
				//install the painter	
				QPainter painter(this);
				painter.setPen(white);
						 
				// this erases the old line from the last move event
				painter.drawLine(
							(int) (5),  
							(int) (0),   
							(int) (width()),
							(int) (height()));

				painter.end();
				Log.info()<< "height() " <<  height()<< std::endl;
			*/
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

		//
		// slots for communication with PTEDialog
		//
		
		// Set the element for the next insert operations
		void EditableScene::setEditElementType(int element_number)
		{
			editAtomType_=element_number;
		}
		
	  // Get the element for the next insert operations
		int EditableScene::getEditElementType()
		{
			return editAtomType_;
		}	
		
		void EditableScene::drawRuler_()
		{
			Log.info() << "Draw Ruler " << std::endl;
			
			// the lines for the ruler are always the same => storing not needed
			// for each angstroem we paint a stick on each axis of the ruler
			// once painted ruler axis (painted with the first atom) remain still the
			// editmodus is quit
			// for the ticks we just store the distances (in angstroem) 
			// of the ticks and the length of the rulers, since we can easily 
			// recompute the old ticks, delete them and compute the new ones   
			// An alternative would be the storing of all ticks!  
			
			//install the painter	
			QPainter painter(this);
			painter.setPen(white);
			// this allows to (a) draw or (b) erase, depending if a line was already drawn on 
			// the same position
	 	 // painter.setRasterOp(XorROP);

			
			/*
			 	painter.setRasterOp(XorROP);

					// this erases the old line from the last move event
					painter.drawLine(
							(int) (x_ewindow_bond_pos_second_old_) ,  
							(int) (y_ewindow_bond_pos_second_old_),   
							(int) (x_ewindow_bond_pos_first_),
							(int) (y_ewindow_bond_pos_first_));

						painter.end();	 
			*/
			
			//delete the old ticks
			//vertical
		/*		for(unsigned i = 1; i < (unsigned int) ruler_vertical_length_; i++)
			{
				painter.drawLine( (int) (0),
													(int) (i * ruler_vertical_scaling_),
													(int) (10),
													(int) (i * ruler_vertical_scaling_)
					              );
			}
			//horizontal
			for(unsigned i = 1; i < (unsigned int) ruler_horizontal_length_; i++)
			{
				painter.drawLine( (int) (0),
													(int) (i * ruler_horizontal_scaling_),
													(int) (10),
													(int) (i * ruler_horizontal_scaling_)
					              );
			}
	*/

			//compute the new scaling
			
			//compute the 3dim positions of "screen limiting points"
			Vector3 zero       = clickedPointOnViewPlane_( 5          , (height()-5) );
			Vector3 horizontal = clickedPointOnViewPlane_((width()-5) , (height()-5) );
			Vector3 vertical   = clickedPointOnViewPlane_( 5          , 5);
			
			float horizontal_distance = (zero - horizontal).getLength();
			float vertical_distance = (zero - vertical).getLength();
			
			float vertical_d   = (height()-10) / vertical_distance ;
			float horizontal_d = (width() -10) / horizontal_distance;
			
			//we need to draw the ruler axis					 
			painter.drawLine(
							(int) (5),  
							(int) (0),   
							(int) (5),
							(int) (height()));
		
			painter.drawLine(
							(int) (0),  
							(int) (height()-5),   
							(int) (width()),
							(int) (height()-5));

			
			//draw the vertical scaling ticks
			for(unsigned i = 1; i < (unsigned int) horizontal_distance +1; i++)
			{
				painter.drawLine( (int) (i * horizontal_d),
													(int) (height()-10),
													(int) (i * horizontal_d),
													(int) (height())
					              );
			}
			
			//draw the horizontal scaling
			for(unsigned i = 1; i < (unsigned int) vertical_distance +1; i++)
			{
				painter.drawLine( (int) (2),
													(int) (height()-5-(i * vertical_d)),
													(int) (8),
													(int) (height()-5-(i * vertical_d))
					              );
			}

			//store the old scaling
			ruler_vertical_scaling_   = vertical_d;
			ruler_horizontal_scaling_ = horizontal_d;
			ruler_vertical_length_    = vertical_distance;
			ruler_horizontal_length_  = horizontal_distance;
		
			painter.end();	
		}



		

	}//end of namespace 
} //end of namespace
