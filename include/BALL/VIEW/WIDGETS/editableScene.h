/*    THIS IS EXPERIMENTAL CODE  
 *
 *    WE GIVE NO WARRANTY
 *    
 *    USE AT YOUR OWN RISK!!!!!!
 */

#ifndef BALL_VIEW_WIDGETS_EDITABLESCENE_H
#define BALL_VIEW_WIDGETS_EDITABLESCENE_H

#ifndef BALL_VIEW_WIDGETS_SCENE_H
#include <BALL/VIEW/WIDGETS/scene.h>
#endif

#ifndef BALL_KERNEL_MOLECULE_H
#include <BALL/KERNEL/molecule.h>
#endif

#ifndef BALL_VIEW_KERNEL_COMMON_H
# include <BALL/VIEW/KERNEL/common.h>
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
				INSERT__MODE = PICKING__MODE + 1
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


			
		private:
			Index edit_id_;	
			Molecule *current_molecule_;	
};

	}//end of namespace
} // end of namespace
#endif
