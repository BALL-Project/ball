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

// has to come after BALL includes to prevent problems with Visual Stuio Net
#include <qgl.h>



namespace BALL
{
	namespace VIEW
	{
		
class BALL_EXPORT EditableScene
	:public Scene
	{
		Q_OBJECT //macro for QT-Messages

		public:
			
			BALL_EMBEDDABLE(EditableScene, ModularWidget)	

      // nested class
			class BALL_EXPORT EditOperation
			{
				public:
					
					/** Constructor
					 */
					EditOperation()
						throw();

					EditOperation(Atom* atom, Bond* bond, String describtion= "Added Object", int operation=0)
						throw();
					
					EditOperation(const EditOperation& eOperation)
						throw();

					
					/** Destructor.
					*/
					virtual ~EditOperation()
						throw();
					
					enum OperationType
					{
						DEFAULT,
						ADDED__ATOM,
						ADDED__BOND,
						CHANGED__TYPE
						//CHANGED__SYSTEM
						//MOVED__ATOM
					};
					
				protected:
					OperationType operationType_;
					Atom* atom_;
					Bond* bond_;
					String description_;
					
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

			/** Explicit default initialization.
					Reset the default values of this scene to:
					  -  width is set to <tt> 600</tt>
					  -  height is set to <tt> 600</tt>
					  - camera position set to <tt> Vector(1,0,0)</tt>
					  - camera look at position set to <tt> Vector(0,0,0)</tt>
					\par
			*/
			virtual void clear()
				throw();
			

			enum EditMode
			{
				EDIT__MODE = PICKING__MODE + 1,
				BOND__MODE = PICKING__MODE + 2	
			};


			
			void initializeWidget(MainControl& main_control)
				throw();
	
			void finalizeWidget(MainControl& main_control)
				throw();


			void checkMenu(MainControl& main_control)
				throw();


	public slots:
			//#############################################################
			//								Mouse Events
			//############################################################
			virtual void mousePressEvent(QMouseEvent* e);

			
		  virtual void mouseMoveEvent(QMouseEvent *e);


			virtual void mouseReleaseEvent(QMouseEvent *e);

      void setEditElementType(int element_number);

	protected slots:
			virtual void editMode_();
			virtual void bondMode_();
	
			
	protected:
			Index edit_id_;	
			System system_; // Do we need them?? 
			Molecule *current_molecule_;	//Do we need them??
			Atom* first_atom_for_bond_;
			
			float x_ewindow_bond_pos_first_;
			float y_ewindow_bond_pos_first_;
			float x_ewindow_bond_pos_second_new_;
			float y_ewindow_bond_pos_second_new_;
			float x_ewindow_bond_pos_second_old_;
			float y_ewindow_bond_pos_second_old_;
			
			Vector3 near_left_bot_;  //TODO:: name in XYZ left_bot_mnear
			Vector3 near_right_bot_;
			Vector3 near_left_top_;
			
			double limit_;			
			bool   mouse_has_moved_;
			int editAtomType_;

			//undo stack
			//TODO   list_of_operations
			vector< EditOperation > undo_;
			
			/**
			 * Insert a given Atom in the Scene. Its position is specified by the 2-dim 
			 * Mouseclick coordinates of the Screen, which will be translated into the 
			 * 3-dim space of Viewing Volume.
			 */
			void insert_(int x_, int y_, PDBAtom &atom_);

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
			Atom* getClickedAtom_(int x, int y);

			/**
			 * Maps the current viewplane to screen coordinates.
			 * Returns false if the projection matrix is not correctly
			 * initialized.
			 */
			bool mapViewplaneToScreen_();

};

	}//end of namespace
} // end of namespace
#endif
