// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: scene.h,v 1.15 2003/03/14 11:50:19 sturm Exp $

#ifndef BALL_VIEW_GUI_WIDGETS_SCENE_H
#define BALL_VIEW_GUI_WIDGETS_SCENE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#include <qgl.h>
#include <iostream>

#ifndef BALL_CONCEPT_NOTIFICATION_H
#	include <BALL/CONCEPT/notification.h>
#endif

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_MATHS_MATRIX44_H
#	include <BALL/MATHS/matrix44.h>
#endif

#ifndef BALL_MATHS_QUATERNION_H
#	include <BALL/MATHS/quaternion.h>
#endif

#ifndef BALL_VIEW_GUI_FUNCTOR_GLOBJECTCOLLECTOR_H
#	include <BALL/VIEW/GUI/FUNCTOR/glObjectCollector.h>
#endif

#ifndef BALL_VIEW_GUI_WIDGETS_EVENTS_H
#	include <BALL/VIEW/GUI/WIDGETS/events.h>
#endif

#ifndef BALL_VIEW_GUI_WIDGETS_MODULARWIDGET_H
#	include <BALL/VIEW/GUI/WIDGETS/modularWidget.h>
#endif

#ifndef BALL_VIEW_GUI_KERNEL_GLOBJECT_H
#	include <BALL/VIEW/GUI/KERNEL/glObject.h>
#endif

#ifndef BALL_VIEW_GUI_KERNEL_GLPRIMITIVEMANAGER_H
#	include <BALL/VIEW/GUI/KERNEL/glPrimitiveManager.h>
#endif

#ifndef BALL_VIEW_COMMON_GLOBAL_H
# include <BALL/VIEW/COMMON/global.h>
#endif 

namespace BALL
{
	namespace VIEW
	{
    /**  \addtogroup  ViewGuiWidgets
     *  @{
     */
		// necessary forward declaration
		class ExternalRenderer;

		/**	The Scene class.
				The class Scene is the main visualization widget that shows that graphical
				representation of the inserted  \link Composite Composite \endlink  objects that are stored
				in the  \link MainControl MainControl \endlink  object.
				To do this the class Scene must be a child of the  \link MainControl MainControl \endlink  object.
				Because the  \link MainControl MainControl \endlink  object is also the main application object
				a scene widget must be created with the pointer to the  \link MainControl MainControl \endlink  object
				as parent widget.
				It is also possible to connect scenes together with the  \link Notification Notification \endlink 
				mechanism of BALL. Connecting two or more scenes together means that that
				mouse action performed in one scene are also transfered to all other connected
				scenes. These other scenes can have different camera angles or other properties.
				The class  \link Events Events \endlink  is a container class of appropriate events that can
				be assigned to a scene.
				
		*/
		class Scene
			: public QGLWidget, public ModularWidget
		{
			Q_OBJECT

		  public:

			/** @name Class friends
			*/
			//@{
			/// The class Events.
			friend class Events;
			//@}

			/** @name Macros
		  */
			//@{
			/** Embeddable Macro.
			*/
			BALL_EMBEDDABLE(Scene)
			//@}

			/**	@name	Type Definitions
			*/
			//@{
			/// Typedefinition of the width of {\em *this} scene.
			typedef unsigned long Width;
			/// Typedefinition of the height of {\em *this} scene.
			typedef unsigned long Height;
			//@}

			/**	@name	Constructors
			*/	
			//@{

			Scene()
				throw();

			/** Default Constructor.
					Construct new scene.
					Initialize the width and height of {\em *this} scene to <tt>600</tt> and sets
					the camera position to:

					  - camera position set to <tt>Vector(1,0,0)</tt>
					  - camera look at position set to <tt>Vector(0,0,0)</tt>
					
					The following events are used initially:

					  -  \link MouseLeftButtonPressed MouseLeftButtonPressed \endlink  &  \link MouseMoved MouseMoved \endlink  are connected to  \link RotateSystem RotateSystem \endlink 
					  -  \link MouseMiddleButtonPressed MouseMiddleButtonPressed \endlink  &  \link MouseMoved MouseMoved \endlink  are connected to  \link ZoomSystem ZoomSystem \endlink 
					  -  \link MouseRightButtonPressed MouseRightButtonPressed \endlink  &  \link MouseMoved MouseMoved \endlink  are connected to  \link TranslateSystem TranslateSystem \endlink 
										
					Register a default  \link GLObjectCollector GLObjectCollector \endlink .
					A  \link GLPrimitiveManager GLPrimitiveManager \endlink  is initialized for {\em *this} scene.
					Calls  \link registerWidget registerWidget \endlink .
					@param      parent_widget the parent widget of {\em *this} scene 
					@param      name the name of {\em *this} scene 
					@param      flags the flags the scene widget should have 
											(See documentation of QT-library for information concerning widget flags) 
					@return     Scene new constructed scene
					@see        QGLwidget
					@see        ModularWidget
			*/
			Scene(QWidget* parent_widget, const char* name = NULL, WFlags w_flags = 0)
				throw();

			/** Copy constructor.
					Construct new scene by copying the scene {\em scene}.
					Initialize the width, height, camera position and the  \link GLObjectCollector GLObjectCollector \endlink 
					of {\em *this} scene to the width, height, camera position and the 
					 \link GLObjectCollector GLObjectCollector \endlink 	of {\em scene}
					The following events are used initially:

					  -  \link MouseLeftButtonPressed MouseLeftButtonPressed \endlink  &  \link MouseMoved MouseMoved \endlink  are connected to  \link RotateSystem RotateSystem \endlink 
					  -  \link MouseMiddleButtonPressed MouseMiddleButtonPressed \endlink  &  \link MouseMoved MouseMoved \endlink  are connected to  \link ZoomSystem ZoomSystem \endlink 
					  -  \link MouseRightButtonPressed MouseRightButtonPressed \endlink  &  \link MouseMoved MouseMoved \endlink  are connected to  \link TranslateSystem TranslateSystem \endlink 
										
					A  \link GLPrimitiveManager GLPrimitiveManager \endlink  is initialized for {\em *this} scene.
					Calls  \link registerWidget registerWidget \endlink .
					@param  scene the scene to be copied
					@param  parent_widget the parent widget of {\em *this} scene 
					@param  name the name of {\em *this} scene (See documentation of QT-library for information concerning widgets)
					@param  flags the flags the scene widget should have 
					@return Scene new constructed scene copied from {\em scene}
					@see    QGLwidget
					@see    ModularWidget
			 */
			Scene (const Scene& scene, QWidget* parent_widget = NULL, const char* name = NULL, WFlags wflags = 0)
				throw();

			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} scene.
					Calls  \link destroy destroy \endlink .
					@see         destroy
			*/
			virtual ~Scene()
				throw();

			/** Explicit default initialization.
					Reset the default values of {\em *this} scene to:

					  - width is set to <tt>600</tt>
					  - height is set to <tt>600</tt>
					  - camera position set to <tt>Vector(1,0,0)</tt>
					  - camera look at position set to <tt>Vector(0,0,0)</tt>
					
					Set the  \link GLObjectCollector GLObjectCollector \endlink  to the default object collector.
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Empty for further purpose.
			*/
			virtual void destroy()
				throw();

			//@}
			/**	@name	Assignment
			*/
			//@{

			/** Assignment.
					Assign the scene {\em scene} to {\em *this}	scene.
					Initialize the width, height, camera position and the  \link GLObjectCollector GLObjectCollector \endlink 
					of {\em *this} scene to the width, height, camera position and the 
					 \link GLObjectCollector GLObjectCollector \endlink 	of {\em scene}.				
					@param  scene the scene to be copied
					@see    get
			*/
			void set(const Scene& scene)
				throw();

			/** Assignment operator.
					Assign the scene {\em scene} to {\em *this}	scene.
					Calls  \link set set \endlink .
					@param   scene the scene to be copied
					@return  Scene& {\em *this} scene copied from {\em scene}
					@see     set
			*/
			const Scene& operator =	(const Scene& scene)
				throw();

			/** Copying.
					Copy {\em *this} scene to the scene	{\em scene}.
					Calls  \link set set \endlink .
					@param  scene the scene to be assigned to
					@see    set
			*/
			void get(Scene& scene) const
				throw();

			//@}
			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Change the camera position.
					Change the camera position of {\em *this} scene.
					@param look_at the look at position of the camera
					@param view_point the view point of the camera
					@param set_origin if set to <tt>true</tt> than the {\em look_at} vector will be used as the system origin. 
								 otherwise the system origin will not be changed
					@see   setViewPointPosition
					@see   Vector3
			*/
			void setCamera(const Vector3& look_at, const Vector3& view_point, bool set_origin = true)
				throw();
			
			/** Change the view point position of the camera.
					Change the view point position of the camera of {\em *this} scene. This method
					does not change the system origin.
					@param v the new view point position
					@see   setLookAtPosition
					@see   getViewPointPosition
					@see   setCamera
					@see   Vector3
			*/
			void setViewPointPosition(const Vector3& v)
				throw();

			/** Non-mutable inspection of the view point position of the camera.
					Acces the constant reference of the view point position of the camera of
					{\em *this} scene.
					@return Vector3& a constant reference to the view point position of the camera of
					                 {\em *this} scene
					@see    setViewPointPosition 
					@see    setCamera
					@see    Vector3
			*/
			const Vector3& getViewPointPosition() const
				throw();

			/** Change the view point position of the camera (coordinate version).
					Change the view point position of the camera of {\em *this} scene. This method
					does not change the system origin.
					@param x the x coordinate of the new view point position
					@param y the y coordinate of the new view point position
					@param z the z coordinate of the new view point position
					@see   getViewPointPosition
					@see   setCamera
					@see   Real
			*/
			void setViewPointPosition(const Real x, const Real y, const Real z)
				throw();		
			
			/** Inspection of the view point position of the camera.
					Access the view point position of the camera of {\em *this} scene by using 
					 \link Vector3 Vector3 \endlink .
					@param  v the vector receiving the view point position of the camera of {\em *this} scene
					@see    setViewPointPosition 
					@see    setCamera
					@see    Vector3
			*/
			void getViewPointPosition(Vector3& v) const
				throw();
			
			/** Inspection of the view point position of the camera (coordinate version).
					Access the view point position of the camera of {\em *this} scene by using 
					 \link Real Real \endlink .
					@param  x reference to the x coordinate receiving the x coordinate of the view point 
									position of the camera of {\em *this} scene
					@param  y reference to the y coordinate receiving the x coordinate of the view point 
									position of the camera of {\em *this} scene
					@param  z reference to the z coordinate receiving the x coordinate of the view point 
									position of the camera of {\em *this} scene
					@see    setViewPointPosition 
					@see    setCamera
					@see    Real
			*/
			void getViewPointPosition(Real &x, Real &y, Real &z) const
				throw();
			
			/** Change the look at position of the camera.
					Change the look at position of the camera of {\em *this} scene. The system origin
					can either be set to the new look at position {\em v} (Default) or not.
					@param v the new look at position
					@param set_origin if set to <tt>true</tt> than {\em v} is also the new system origin of {\em *this} scene, 
								 if set to <tt>false</tt> the system origin is not changed
					@see   setViewPointPosition
					@see   getLookAtPosition
					@see   setCamera
					@see   Vector3
			*/
			void setLookAtPosition(const Vector3& v, bool set_origin = true)
				throw();
			
			/** Non-mutable inspection of the up vector.
					@return Vector3& a constant reference to the up vector of
					        {\em *this} scene
					@see		Vector3
			*/
			const Vector3& getUpVector() const
				throw();
			
			/** Non-mutable inspection of the right vector.
					@return Vector3& a constant reference to the right vector of
					                 {\em *this} scene
					@see	Vector3
			*/
			const Vector3& getRightVector() const
				throw();

			/** Inspection of the scaling factor of {\em *this} scene.
			 */
			void getScalingFactor(Vector3& scaling) const
				throw();
			
			/** Non-mutable inspection of the look at position of the camera.
					Acces the constant reference of the look at position of the camera of
					{\em *this} scene.
					@return Vector3& a constant reference to the look at position of the camera of
					                 {\em *this} scene
					@see    setLookAtPosition 
					@see    setCamera
					@see    Vector3
			*/
			const Vector3& getLookAtPosition() const
				throw();

			/** Change the look at position of the camera (coordinate version).
					Change the look at position of the camera of {\em *this} scene. The system origin
					can either be set to the new look at position (Default) or not.
					@param x the x coordinate of the new look at position
					@param y the y coordinate of the new look at position
					@param z the z coordinate of the new look at position
					@param set_origin if set to <tt>true</tt> than the vector consisting of {\em x}, {\em y}, and {\em z} is also the 
								 new system origin of {\em *this} scene, if set to <tt>false</tt> the system origin is not changed
					@see   getLookAtPosition
					@see   setCamera
					@see   Real
			*/
			void setLookAtPosition(const Real x, const Real y, const Real z, bool set_origin = true)
				throw();
			
			/** Inspection of the look at position of the camera.
					Access the look at position of the camera of {\em *this} scene by using 
					 \link Vector3 Vector3 \endlink .
					@param  v the vector receiving the look at position of the camera of {\em *this} scene
					@see    setLookAtPosition 
					@see    setCamera
					@see    Vector3
			*/
			void getLookAtPosition(Vector3& v) const
				throw();

			/** Inspection of the look at position of the camera (coordinate version).
					Access the look at position of the camera of {\em *this} scene by using 
					 \link Real Real \endlink .
					@param  x reference to the x coordinate receiving the x coordinate of the look at 
									position of the camera of {\em *this} scene
					@param  y reference to the y coordinate receiving the x coordinate of the look at 
									position of the camera of {\em *this} scene
					@param  z reference to the z coordinate receiving the x coordinate of the look at 
									position of the camera of {\em *this} scene
					@see    setLookAtPosition 
					@see    setCamera
					@see    Real
			*/
			void getLookAtPosition(Real& x, Real& y, Real& z) const
				throw();

			/** Register a  \link GLObjectCollector GLObjectCollector \endlink  for {\em *this} scene.
					Initially a default  \link GLObjectCollector GLObjectCollector \endlink  is already registered.
					A  \link GLObjectCollector GLObjectCollector \endlink  is responsible for collecting all  \link GLObject GLObject \endlink  objects
					from the  \link Composite Composite \endlink  object that is inserted into the  \link MainControl MainControl \endlink .
					See this classes for further information.
					@param globject_collector the new  \link GLObjectCollector GLObjectCollector \endlink  to be registered for {\em *this} scene
					@see   unregisterGLObjectCollector
					@see   getGLObjectCollector
					@see   GLObjectCollector
					@see   GLObject
					@see   MainControl
					@see   Composite
			*/
			void registerGLObjectCollector(const GLObjectCollector& globject_collector)
				throw();

			/** Unregister a  \link GLObjectCollector GLObjectCollector \endlink  for {\em *this} scene.
					If this method is called a default  \link GLObjectCollector GLObjectCollector \endlink  will be registered.
					@see   registerGLObjectCollector
					@see   getGLObjectCollector
					@see   GLObjectCollector
			*/
			void unregisterGLObjectCollector()
				throw();

			/** Access a constant pointer to the  \link GLObjectCollector GLObjectCollector \endlink  of {\em *this} scene.
					@return GLObjectCollector* a constant pointer to the registered  \link GLObjectCollector GLObjectCollector \endlink  of {\em *this} scene
					@see   registerGLObjectCollector
					@see   unregisterGLObjectCollector
					@see   GLObjectCollector
			*/
			const GLObjectCollector *getGLObjectCollector() const
				throw();

			/** Update the visualization of {\em *this} scene.
					@param  rebuild_displaylists is set to <tt>true</tt> than all the  \link GLDisplayList GLDisplayList \endlink  objects are updated. 
									If set to <tt>false</tt> only the display is redrawed.
					@see    GLDisplayList
					@see    GLEntityDescriptor
					@see    CompositeDescriptor
					@see    MainControl
			*/
			void update(bool rebuild_displaylists = false)
				throw(MainControlMissing);

			/** Handles messages sent by other registered  \link ConnectionObject ConnectionObject \endlink  objects.
					Filters for  \link SceneMessage SceneMessage \endlink  and sets the camera appropriate or updates the visualization. 
					Calls  \link update update \endlink  with <tt>false</tt> as parameter.
					@param message the pointer to the message that should be processed
					@see   ConnectionObject
					@see   Message
		  */
			virtual void onNotify(Message *message)
				throw();

			/** This method prepares an ExternalRenderer for a later export of this scene.
			*/
			virtual void exportScene(BALL::VIEW::ExternalRenderer &er) const
				throw();

			//@}			
			/** @name Event class
			*/
			//@{
			
			/** This object contains all possible events {\em *this} scene can sent via the 
					 \link Notification Notification \endlink  mechanism of BALL
					@see  Events
			*/
			Events events;

			//@}	
			/**	ModularWidget methods.
			*/
			//@{
			
			/**	Initialize the popup menu {\em Display} and the menus of {\em *this} scene:

					  - the {\em rotate mode} (all mouse actions are attached to rotating, translating and zooming the scene)
					  - the {\em picking mode} (all mouse actions are attached to picking objects from the scene)
					
					This method is called automatically	immediately before the main application 
					is started. 
					This method will be called by  \link show show \endlink  from the  \link MainControl MainControl \endlink  object.
					@param main_control the  \link MainControl MainControl \endlink  object to be initialized with {\em *this} scene
					@see   finalizeWidget
					@see   insertMenuEntry
					@see   checkMenu
					@see   show
			*/
			virtual void initializeWidget(MainControl& main_control)
				throw();
		
			/**	Remove the widget.
					Reverse all actions performed in  \link initializeWidget initializeWidget \endlink  (remove menu entries of {\em *this} scene).
					This method will be called by  \link aboutToExit aboutToExit \endlink  from the  \link MainControl MainControl \endlink  object.
					@param main_control the  \link MainControl MainControl \endlink  object to be finalized with {\em *this} scene
					@see   initializeWidget
					@see   checkMenu
					@see   removeMenuEntry
					@see   aboutToExit
			*/
			virtual void finalizeWidget(MainControl& main_control)
				throw();
		
			/**	Menu checking method.
					This method is called by the method  \link checkMenus checkMenus \endlink  from the
					 \link MainControl MainControl \endlink  object before a popup menu is shown.
					The menus {\em rotate mode} and {\em picking mode} each will be checked
					if {\em *this} scene is in the beloning mode.
					@param main_control the  \link MainControl MainControl \endlink  object whose menus should be checked
					@see   initializeWidget
					@see   finalizeWidget
					@see   checkMenus
					@see   show
			*/
			virtual void checkMenu(MainControl& main_control)
				throw();
			
			//@}
			/**	@name	Predicates
			*/
			//@{

			/** Test if default glObjectCollector is registered.
					@return bool <tt>true</tt> if a default  \link GLObjectCollector GLObjectCollector \endlink  is registered for {\em *this} scene
			*/
			bool hasDefaultGLObjectCollector() const
				throw();

			//@}
			/**	@name	Debugging and Diagnostics
			*/
			//@{

			/** Internal state and consistency self-validation.
					If the internal state of {\em *this} scene is correct 
					(self-validated) and consistent <tt>true</tt> is returned, <tt>false</tt> otherwise. 
					{\em *this} scene is valid if:

					  - the  \link GLPrimitiveManager GLPrimitiveManager \endlink  is valid
						- {\em *this} scene has a parent
					
					Calls  \link GLPrimitiveManager::isValid GLPrimitiveManager::isValid \endlink .
					@return			bool <tt>true</tt> if the internal state of {\em *this} scene is correct (self-validated) and consistent,
					 						<tt>false</tt> otherwise
			*/
			virtual bool isValid() const
				throw();

			/** Internal value dump.
					Dump the current internal state of {\em *this} scene to 
					the output ostream {\em s} with dumping depth {\em depth}.
					@param   s output stream where to output the internal state of {\em *this} scene
					@param   depth the dumping depth
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();

			protected:

			//@}
			/** @name Protected members
			*/
			//@{

			/** Non-mutable inspection of the zoom vector.
					The zoom vector is the translation vector from the view point position to the
					look at position of the camera.
					This vector is internally needed for calculating rotation, translation and zoom
					of {\em *this} scene.
					This vector will be calculated anew if the position of the camera changes.
					@return  Vector3& constant reference to the zoom vector of {\em *this} scene
					@see     setLookAtPosition
					@see     setViewPointPosition
					@see     setCamera
			*/
			const Vector3& getZoomVector_() const
				throw();

			/** Non-mutable inspection of the up vector.
					The up vector is the vector that defines the y-coordinate axis of {\em *this} scene.
					This vector is internally needed for calculating rotation, translation and zoom of {\em *this} scene.
					This vector will be calculated anew if the position of the camera changes.
					@return  Vector3& constant reference to the up vector of {\em *this} scene
					@see     setLookAtPosition
					@see     setViewPointPosition
					@see     setCamera
			*/
			const Vector3& getUpVector_() const
				throw();

			/** Non-mutable inspection of the right vector.
					The right vector is the vector that defines the x-coordinate axis of {\em *this} scene.
					This vector is internally needed for calculating rotation, translation and zoom of {\em *this} scene.
					This vector will be calculated anew if the position of the camera changes.
					@return  Vector3& constant reference to the right vector of {\em *this} scene
					@see     setLookAtPosition
					@see     setViewPointPosition
					@see     setCamera
			*/
			const Vector3& getRightVector_() const
				throw();

			/** Non-mutable inspection of the look up vector.
					The look up vector is a vector that helps define the y- and the x-coordinate axis
					of {\em *this} scene (see  \link getUpVector getUpVector \endlink  and  \link getRightVector getRightVector \endlink ).
					This vector is internally needed for calculating rotation, translation and zoom of {\em *this} scene.
					This vector will be calculated anew if the position of the camera changes.
					@return  Vector3& constant reference to the look up vector of {\em *this} scene
					@see     getUpVector
					@see     getRightVector
					@see     setLookAtPosition
					@see     setViewPointPosition
					@see     setCamera
			*/
			const Vector3& getLookUpVector_() const
				throw();

			//@}
			/** @name Protected QT overridden virtual methods
			*/
			//@{

			/** Initialize the OpenGL context.
					Overriden qt method for initializing the OpenGL context of {\em *this} scene.
					This method will be called automatically before any call to  \link paintGL paintGL \endlink  or  \link resizeGL resizeGL \endlink .
					See QT-library for information concerning the qglwidget events.
					@see  paintGL
					@see  resizeGL
			*/
			virtual void initializeGL();

			/** Render the visualization.
					Overriden qt method for rendering the visualization of {\em *this} scene.
					This method will be called automatically every time an update is necessary.
					See QT-library for information concerning qglwidgets and paintGL methods and events.
					@see  MainControl
					@see  CompositeDescriptor
					@see  GLDisplayList
					@see  GLEntityDescriptor
			*/
			virtual void paintGL();

			/** Resize the widget.
					Overridden qt method for resizing {\em *this} scene. 
					This method will be called automatically every time a rezize event is handled.
					See QT-library for information concerning qglwidgets and resizeGL methods and events.
					@param  width the new width of {\em *this} scene
					@param  height the new height of {\em *this} scene
			*/
			virtual void resizeGL(int width, int height);

			/** Catch key press events.
					Catch the following key press events:

					  - <tt>Key_Shift</tt> the SHIFT key was pressed
					  - <tt>Key_Control</tt> the SHIFT key was pressed
					
					This events will be stored for later processing.
					See QT-library for information concerning key press events.
					@param qkey_event the QT-key event (See QT-library for key events)
					@see   keyReleaseEvent
					@see   mouseMoveEvent
					@see   mousePressEvent
					@see   mouseReleaseEvent
			*/ 
			virtual void keyPressEvent(QKeyEvent* qkey_event);

			/** Catch key release events.
					Catch the following key release events:

					  - <tt>Key_Shift</tt> the SHIFT key was released
					  - <tt>Key_Control</tt> the SHIFT key was released
					
					This events will be stored for later processing.
					See QT-library for information concerning key released events.
					@param qkey_event the QT-key event (See QT-library for key events)
					@see  keyPressEvent
					@see  mouseMoveEvent
					@see  mousePressEvent
					@see  mouseReleaseEvent
			*/ 
			virtual void keyReleaseEvent(QKeyEvent* qkey_event);

			/** Catch mouse move events.
					Catch mouse move events, store the actual mouse position in {\em *this} scene
					widget and sent events accordingly.
					Calls  \link Notify Notify \endlink 
					@param  qmouse_event the QT-mouse event (See QT-library for mouse events)
					@see    Events
			*/
			virtual void mouseMoveEvent(QMouseEvent* qmouse_event);

			/** Catch mouse press events.
					Catch mouse press events, store the actual mouse position in {\em *this} scene
					widget and sent events accordingly.
					Calls  \link Notify Notify \endlink 
					@param  qmouse_event the QT-mouse event (See QT-library for mouse events)
					@see    Events
			*/
			virtual void mousePressEvent(QMouseEvent* qmouse_event);

			/** Catch mouse release events.
					Catch mouse release events, store the actual mouse position in {\em *this} scene
					widget and sent events accordingly.
					Calls  \link Notify Notify \endlink 
					@param  qmouse_event the QT-mouse event (See QT-library for mouse events)
					@see    Events
			*/
			virtual void mouseReleaseEvent(QMouseEvent* qmouse_event);

			protected slots:

			//@}
			/** @name Protected slots
			*/
			//@{

			/** Switch to rotate mode.
					If this method is called the mouse actions of {\em *this} scene will
					perform rotation, translation and zooming the visualization.
					This method will be called from the belonging menu entry.
					@see initializeWidget
					@see checkMenu
			*/
			virtual void rotateMode_();

			/** Switch to picking mode.
					If this method is called the mouse actions of {\em *this} scene will
					perform object picking.
					This method will be called from the belonging menu entry.
					@see initializeWidget
					@see checkMenu
			*/
			virtual void pickingMode_();
			//@}


			private:

			enum Phase
			{
				PHASE__STATIC_SCENE   = 0,
				PHASE__DYNAMIC_SCENE  = 1
			};

			enum RenderMode
			{
				RENDER_MODE__COMPILE        = 0,
				RENDER_MODE__DO_NOT_COMPILE = 1
			};

			enum Event
			{
				EVENT__UNKOWN       = 0,
				EVENT__PRESSED      = 1,
				EVENT__RELEASED     = 2,
				EVENT__MOVED        = 3
			};

			enum KeyPressed
			{
				KEY_PRESSED__NONE          = 0,
				KEY_PRESSED__SHIFT         = 1,
				KEY_PRESSED__CONTROL       = 2,
				KEY_PRESSED__SHIFT_CONTROL = 3
			};

			enum MouseButton
			{
				MOUSE_BUTTON__NONE  = 0,
				MOUSE_BUTTON__ONE   = 1,
				MOUSE_BUTTON__TWO   = 2,
				MOUSE_BUTTON__THREE = 3
			};

			void setRenderMode_(RenderMode mode)
				throw();

			RenderMode getRenderMode_() const
				throw();

			void renderView_(bool woth_names = false);

			// --- registerable functions

			void rotateSystem_(Scene* scene);

			void translateSystem_(Scene* scene);

			void zoomSystem_(Scene* scene);

			void selectionPressed_(Scene* scene);
			void selectionReleased_(Scene* scene);
			void selectionPressedMoved_(Scene* scene);

			void deselectionPressed_(Scene* scene);
			void deselectionReleased_(Scene* scene);
			void deselectionPressedMoved_(Scene* scene);

			Vector3 translateObjectXY_(const Real distance);

			Vector3 translateObjectZ_(const Real distance);

			Vector3 calculateRotatedVector_(const Vector3& v, const Quaternion& q);

			void calculateQuaternion_(Quaternion& quaternion, const Quaternion& rotation_quaternion);

			Real sphereproject_(Real radius, Real x, Real y);

			void convertMatrix_(const Matrix4x4& m, GLfloat glfloat_array[4][4]);

			void selectObjects_(bool select = true);

			
			int rotate_id_;
			int picking_id_;
			
			bool rotate_mode_;

			GLObjectCollector* GL_object_collector_;
			GLPrimitiveManager GL_primitive_manager_;

			Vector3 system_origin_;
			Quaternion quaternion_;

			Scene::Width	width_;
			Scene::Height height_;

			Scene::Phase				phase_;
			Scene::RenderMode		render_mode_;
			Scene::KeyPressed		key_pressed_;
			Scene::MouseButton	actual_mouse_button_;

			bool was_picking_;

			bool need_update_;

			Real x_window_pos_old_;
			Real y_window_pos_old_;

			Real x_window_pos_new_;
			Real y_window_pos_new_;

			Real x_window_pick_pos_first_;
			Real y_window_pick_pos_first_;

			Real x_window_pick_pos_second_;
			Real y_window_pick_pos_second_;

			GLfloat x_scale_;
			GLfloat y_scale_;

			// Camera section -----

			void initViewVectors_();

			void setCamera_(bool set_origin = false);

			bool setCameraPosition_(const Vector3& look_at, const Vector3& view_point, bool set_origin);

			Vector3 zoom_;
			Vector3 position_;
			Vector3 look_at_position_;
			Vector3 last_look_at_position_;
			Vector3 look_up_;
			Vector3 right_;
			Vector3 up_;
		};


#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/WIDGETS/scene.iC>
#		endif
  /** @} */
	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_GUI_WIDGETS_SCENE_H_
