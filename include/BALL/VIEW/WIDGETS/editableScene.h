/*    THIS IS EXPERIMENTAL CODE  
 *
 *    WE GIVE NO WARRANTY
 *    
 *    USE AT YOUR OWN RISK!!!!!!
 */

#ifndef BALL_VIEW_WIDGETS_EDITABLESCENE_H
#define BALL_VIEW_WIDGETS_EDITABLESCENE_H


#ifndef BALL_KERNEL_SYSTME_H
#include <BALL/KERNEL/system.h>
#endif

#ifndef BALL_VIEW_WIDGETS_SCENE_H
#include <BALL/VIEW/WIDGETS/scene.h>
#endif

#ifndef BALL_KERNEL_MOLECULE_H
#include <BALL/KERNEL/molecule.h>
#endif

#ifndef BALL_VIEW_KERNEL_COMMON_H
# include <BALL/VIEW/KERNEL/common.h>
#endif 

#ifndef BALL_MATHS_VECTOR2_H
# include <BALL/MATHS/vector2.h>
#endif

#ifndef BALL_VIEW_WIDGETS_MOLECULARCONTROL_H
# include <BALL/VIEW/WIDGETS/molecularControl.h>
#endif

#include <qpopupmenu.h>
#include <qbitmap.h>

#include <vector>

// has to come after BALL includes to prevent problems with Visual Stuio Net
#include <qgl.h>



namespace BALL
{
	namespace VIEW
	{

		class BALL_EXPORT EditableScene
			: public Scene
			{
				Q_OBJECT //macro for QT-Messages

				public:

					BALL_EMBEDDABLE(EditableScene, ModularWidget)	

						// nested class
						// This class represents a single edit operation and stores its type and
						// parameters. This can be used to implement undo functionality.
						class BALL_EXPORT EditOperation
						{
							public:

								/** Constructor
								*/
								EditOperation()
									throw();

								EditOperation(Atom* atom, Bond* bond, std::vector <Atom* > molecule, String description= "Added Object", int operation=0)
									throw();

								EditOperation(const EditOperation& eOperation)
									throw();


								/** Destructor.
								*/
								virtual ~EditOperation()
									throw();


								/** undo-Operator undoes special operations, offered by undo dialog
								 */
								void undo(EditableScene* es);
								
								enum OperationType
								{
									DEFAULT,
									ADDED__ATOM,
									ADDED__BOND,
									MERGED__MOLECULES
									//MOVED__ATOM
										// DELETED__ATOM
										// DELETED__BOND
										// CHANGED__SYSTEM
								};
								
								int	operationType;
								Atom* atom;
								Bond* bond;
								std::vector <Atom* > molecule;
								String description;
							//	Vector3 position;

						};

					//@} 
					/**	@name	Constructors 
					*/	
					//@{

					EditableScene()
						throw();

					/** Default Constructor.
						Initialize the width and height of this scene to <tt> 600</tt> and sets
						the camera position to:
						- camera position set to <tt> Vector(1,0,0)</tt>
						- camera look at position set to <tt> Vector(0,0,0)</tt>
						- camera look up vector to <tt> Vector(0,0,-1)</tt>
						\par
						Calls registerWidget.
						\param      parent_widget the parent widget of this scene 
						\param      name the name of this scene 
						\param      w_flags the flags the scene widget should have 
						(See documentation of QT-library for information concerning widget flags) 
						*/
					EditableScene(QWidget* parent_widget, const char* name = NULL, WFlags w_flags = 0)
						throw();

					/** Copy constructor.
						Initialize the width, height and camera position.
						\par					
						Calls registerWidget.
						\param  scene the scene to be copied
						\param  parent_widget the parent widget of this scene 
						\param  name the name of this scene 
						\param  wflags the flags the scene widget should have 
						(See documentation of QT-library for information concerning widget flags) 
						undo_, stack for operations is NOT copied!				
						*/
					EditableScene (const EditableScene& eScene, QWidget* parent_widget = NULL, const char* name = NULL, WFlags wflags = 0)
						throw();

					//@} 
					/** @name Destructors 
					*/ 
					//@{

					/** Destructor.
					*/
					virtual ~EditableScene()
						throw();

					void onNotify(Message *message)
						throw();

					
					enum EditMode
					{
						// inserting atoms
						EDIT__MODE = PICKING__MODE + 1,
						// drawing new bonds
						BOND__MODE	
					};
					
					//Mouse menue /Context menu entries
					enum Menu_Entries
					{
						SELECT_ATOM = 10,  // MolecularControl::EXPAND_ALL + 1, TODO: This enum is private!!!! 
						DESELECT_ATOM,
						MOVE_ATOM,
						SET_ATOM_PROPERTIES,
						DELETE_ATOM,
						SELECT_BOND,
						DESELECT_BOND,
						SET_BOND_PROPERTIES,
						SET_BOND_LENGTH
					};


					void initializeWidget(MainControl& main_control)
						throw();

					void finalizeWidget(MainControl& main_control)
						throw();

					void checkMenu(MainControl& main_control)
						throw();

					/**
					 * initializes the context_menu_ 
					 * context_menu_composite_ should be set to the current object
					 * in order to gaurantee that the chosen operation takes place 
					 * on the clicked composite
					 */
					void popupContextMenu_();


					public slots:
						//#############################################################
						//								Mouse Events
						//############################################################
						
						
						/**
		 				*  Mouse Events should provide the following functionality: 
		 				*  right click  - context menu to clicked object (atom or bond)
		 				*  left click   - insertion of an atom with type edit_atom_type_ 
					  *  							 in a previouse defined atomContainer (care about the highlight-mechanism)
					  *  middle click - selection of a first bond partner
					  *    if a middle click is released on an atom, this atom is the second bond partner
					  *    if there is no atom, we insert an atom with type edit_atom_type_ into the
					  *    atomcontainer of the first bond partner 
					  *    if two differemt atomcontainers are connected, they have to be merged! 
					  *
					  *  for every operation we have to emit the corresponding undo-operation! 
					  */

						virtual void mousePressEvent(QMouseEvent* e);


						virtual void mouseMoveEvent(QMouseEvent *e);


						virtual void mouseReleaseEvent(QMouseEvent *e);


						//slots for contextMenue
						void moveAtom();
						void selectAtom();
						void deselectAtom();
						void setAtomProperties();
						void deleteAtom();
						void selectBond();
					  void setBondProperties();
						void setBondLength();
																							
						// slots for communication with PTEDialog
						void setEditElementType(int element_number);
						int getEditElementType();

											
					protected slots:
						virtual void editMode_();
						virtual void bondMode_();
						virtual void showScaling_();
						
					signals:
						// signal for communication with EditOperationDialog
						void newEditOperation(EditableScene::EditOperation &eo);
						void invalidComposite(Composite* composite);

		protected:

					Index edit_id_;
					Index scaling_id_;
					
					AtomContainer* current_atomContainer_; // currentAtomContainer in which new atoms should be inserted
					Atom* first_atom_for_bond_;

					Composite* context_menu_composite_; //Composite for which Popupmenu was called  
					QPopupMenu context_menu_;     // Popupmenu for mousclick right
					
					// used for the bond insert algorithm
					float x_ewindow_bond_pos_first_;
					float y_ewindow_bond_pos_first_;
					float x_ewindow_bond_pos_second_new_;
					float y_ewindow_bond_pos_second_new_;
					float x_ewindow_bond_pos_second_old_;
					float y_ewindow_bond_pos_second_old_;

				  float x_text_position_;
					float y_text_position_;
					float bond_length_;
					
					// used for painting the scaling Ruler   //ToDO ist loeschen ueber haupt notwendig?? 
					float ruler_vertical_scaling_;
					float ruler_horizontal_scaling_;	 
					float ruler_vertical_length_;
					float ruler_horizontal_length_;
					
					Vector3 near_left_bot_;  //TODO:: name in XYZ left_bot_mnear
					Vector3 near_right_bot_;
					Vector3 near_left_top_;

					// search range when looking for atoms/bonds (in angstrom)
					//double atom_limit_;			
					//double bond_limit_;		
				
					double select_atom_limit_; 
					double select_bond_limit_;
					double average_atom_radius_limit_;
					
					bool move_an_atom_;   // flag to move an atom in EDIT__MODE
					bool mouse_has_moved_;
					int edit_atom_type_;        //store atomtype for new atoms     
					bool show_scaling_axes_;  //regulates display of thedisplay of the  scaling axes
					

					/**
					 *  for structuring the code we splitted MousePressEvent and 
					 *  MouseMoveEvent into button related functions or Bond-/Edit- Mode
					 */
					
					/**
					 * this function performs the action of MousePressEvent::LeftButton
					 * inserts a new atom 
					 */
					void leftButtonPressEvent_(QMouseEvent* e);
				
					
					/** this function performs the action of MousePressEvent::RightButton
					 * it searchs an object, that is close to the click ray and open a 
					 * special context menu
					 */
					void rightButtonPressEvent_(QMouseEvent* e);
					
					
					/**
					 * this function performs the action of MousePressEvent::MidButton
					 * if there is an atom in select_atom_limit, a new bond from this atom 
					 * is started the new bond is sketched with a thin line 
					 *    => we have to update the corresponding bond_positions 
					 *    (	x_ewindow_bond_pos_first_ = pos.x and x_ewindow_bond_pos_second_old_ = pos.x)
					 */
					void midButtonPressEvent_(QMouseEvent* e);
				

					/**
					 * this function performs the action of mouseMoveEvent in Bond Mode
					 * the bond is sketched and the actual bond length is presented
					 * if the click-ray is within a certain radius to an atom, the
					 * bond snaps into this atom
					 */
					void moveEventBondMode_(QMouseEvent* e);
					
					/**
					 * this function performs the action of mouseMoveEvent in Bond Mode
					 */
					void moveEventMoveMode_();
					

					/**
					 *  this function performs the action of mouseReleaseEvent in Bond Mode
					 *  having found a second bond partner. It checks, if first and second 
					 *  bond partner are the same atom. If not, a bond is inserted. 
					 *  If the two atoms belong to different atomContainers, they are merged
					 *  and the corresponding undo-operation is emited.
					 */
					void releaseEventBondModeWithAtom_(Atom* atom);

					/**
					 * this function performs the action of mouseReleaseEvent in Bond Mode 
					 *  _without_ having found a second bond partner.
					 *
					 *  If there is even no first bond partner -> nothing to do.
					 *  If there is one and the new atom is not in the atomradius of 
					 *  the first bondpartner, we insert a new atom into the atomcontainer
					 *  of the first bondpartner.
					 */
					void releaseEventBondModeWithoutAtom_(QMouseEvent* e);

					
					/**
					 * Insert a given Atom in the Scene. Its position is specified by the 2-dim 
					 * Mouseclick coordinates of the Screen, which will be translated into the 
					 * 3-dim space of Viewing Volume.
					 */
					bool insert_(int x_, int y_, PDBAtom &atom_);

					/**
					 *  Given a 3-dim. Coordinates (in Viewing Volume) getScreenPosition
					 *  computes the 2-dim Coordinates on Screen.
					 */
					TVector2<Position> getScreenPosition_(Vector3 vec);

					/**
					 * Given 2-dim Coordinates of Screen, clickedPointOnViewPlane computes the
					 * appropriate 3-dim Position in Viewing Volume
					 */
					Vector3 clickedPointOnViewPlane_(int x, int y);

					/**
					 * Given 2-dim Coordinates of Screen, getCLickedAtom_ returns the nearest Atom 
					 * within a special radius. If no atom is found, getClickedAtom returns NULL.
					 */
					Atom* getClickedAtom_(int x, int y, double radius);

					/**
					 * Given 2-dim Coordinates of Screen, getCLickedBond returns the nearest Bond
					 * within a special radius. If no bond is found, getClickedBond returns NULL.
					 * Note: This code is very similar to that of getClickedAtom and the two might
					 *       be joined in the future.
					 */
					Bond* getClickedBond_(int x, int y, float radius);

					/***
					 * returns an atomContainer of the given atom
					 * according to the rules of atom_insertion
					 * central point to change these rules
					 */
					AtomContainer* getAtomContainer_(const Atom* atom);
				
					void highlightAtomContainer_(AtomContainer* ac);

					/**
					 * Maps the current viewplane to screen coordinates.
					 * Returns false if the projection matrix is not correctly
					 * initialized.
					 */
					bool mapViewplaneToScreen_();
				
					/**
					 * sets the cursor on the actual atom element type
					 */
					void setCurrentCursor_(int element_type);

					
					/**
					 * draws a ruler showing the scaling of the insertion plain of the EditableScene 
					 */
					void drawRuler_();
				
					void renderView_(RenderMode mode)
						throw();

					void glDraw()
						throw();

					
					/* enables EditOperations like MERGED__MOLECULES do invalidate bonds or atoms*/
					void invalidateComposite(Composite* deleted_composite);
					
					/**
					 * stores the arrow on which the edit cursor is based
					 */
					static const char * const edit_cursor_xpm_[];
					QPixmap edit_cursor_;
					
					};

	}//end of namespace
} // end of namespace
#endif
