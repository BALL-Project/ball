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
			
			void insert_(int x_, int y_, PDBAtom &atom_);
			TVector2<Position> getScreenPosition_(Vector3 vec);
			Vector3 clickedPointOnViewPlane_(int x, int y);
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
