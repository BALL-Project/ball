// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: scene.h,v 1.12 2003/11/24 23:57:51 amoll Exp $
//

#ifndef BALL_VIEW_WIDGETS_SCENE_H
#define BALL_VIEW_WIDGETS_SCENE_H

#include <qgl.h>
#include <iostream>

#ifndef BALL_MATHS_QUATERNION_H
#	include <BALL/MATHS/quaternion.h>
#endif

#ifndef BALL_VIEW_KERNEL_EVENTS_H
#	include <BALL/VIEW/KERNEL/events.h>
#endif

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
#	include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#ifndef BALL_VIEW_RENDERING_GLRENDERER_H
# include <BALL/VIEW/RENDERING/glRenderer.h>
#endif

#ifndef BALL_VIEW_KERNEL_COMMON_H
# include <BALL/VIEW/KERNEL/common.h>
#endif 

namespace BALL
{
	namespace VIEW
	{
		class Preferences;
		class LightSettings;
		class StageSettings;

		/**	The Scene class.
				The class Scene is the main visualization widget that shows that graphical
				representation of the inserted Composite objects that are stored
				in the MainControl object.
				To do this the class Scene must be a child of the MainControl object.
				Because the MainControl object is also the main application object
				a scene widget must be created with the pointer to the MainControl object
				as parent widget.
				It is also possible to connect scenes together with the Notification
				mechanism of BALL. Connecting two or more scenes together means that that
				mouse action performed in one scene are also transfered to all other connected
				scenes. These other scenes can have different camera angles or other properties.
				The class Events is a container class of appropriate events that can
				be assigned to a scene.
				\ingroup ViewWidgets
		*/
		class Scene
			: public QGLWidget, public ModularWidget
		{
			Q_OBJECT

		  public:

			/** This class is only intended for usage with multithreading.
			 		It induces a rebuild of the display lists and an update of the Scene and
					should only be used internaly.
			*/
			class SceneUpdateEvent : public QCustomEvent
			{
				public:
					SceneUpdateEvent()
						: QCustomEvent( SCENE_UPDATE_EVENT){}
			};

			/** @name Class friends
			*/
			//@{
			/// The class Events.
			friend class Events;

			//@} /** @name Macros */ //@{
			
			/** Embeddable Macro.
			*/
			BALL_EMBEDDABLE(Scene)

			//@} /** @name Enums */ //@{
			
			///
			enum RenderMode
			{
				///
				DIRECT_RENDERING = 0,

				///
				DISPLAY_LISTS_RENDERING,

				///
				REBUILD_DISPLAY_LISTS
			};
				
			//@} /**	@name	Constructors */	//@{

			Scene()
				throw();

			/** Default Constructor.
					Construct new scene.
					Initialize the width and height of this scene to <tt> 600</tt> and sets
					the camera position to:
					  - camera position set to <tt> Vector(1,0,0)</tt>
					  - camera look at position set to <tt> Vector(0,0,0)</tt>
						- camera look up vector to <tt> Vector(0,0,-1)</tt>
					\par
					The following events are used initially:
					  - MouseLeftButtonPressed & MouseMoved are connected to RotateSystem
					  - MouseMiddleButtonPressed & MouseMoved are connected to ZoomSystem
					  - MouseRightButtonPressed & MouseMoved are connected to TranslateSystem
					\par					
					Calls registerWidget.
					\param      parent_widget the parent widget of this scene 
					\param      name the name of this scene 
					\param      flags the flags the scene widget should have 
											(See documentation of QT-library for information concerning widget flags) 
					\return     Scene new constructed scene
					\see        QGLwidget
					\see        ModularWidget
			*/
			Scene(QWidget* parent_widget, const char* name = NULL, WFlags w_flags = 0)
				throw();

			/** Copy constructor.
					Initialize the width, height and camera position.
					The following events are used initially:
					  - MouseLeftButtonPressed & MouseMoved are connected to RotateSystem
					  - MouseMiddleButtonPressed & MouseMoved are connected to ZoomSystem
					  - MouseRightButtonPressed & MouseMoved are connected to TranslateSystem
					\par					
					Calls registerWidget.
					\param  scene the scene to be copied
					\param  parent_widget the parent widget of this scene 
					\param  name the name of this scene (See documentation of QT-library for information concerning widgets)
					\param  flags the flags the scene widget should have 
					\see    QGLwidget
					\see    ModularWidget
			 */
			Scene (const Scene& scene, QWidget* parent_widget = NULL, const char* name = NULL, WFlags wflags = 0)
				throw();

			//@} /** @name Destructors */ //@{

			/** Destructor.
			*/
			virtual ~Scene()
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

			//@} /**	@name	Assignment */ //@{

			/** Assignment.
					Initialize the width, height and camera position.
					\param  scene the scene to be copied
			*/
			void set(const Scene& scene)
				throw();

			/** Assignment operator.
					Calls set.
			*/
			const Scene& operator =	(const Scene& scene)
				throw();

			//@}
			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Update the visualization of this scene.
					\param  rebuild_displaylists is set to <tt> true than all the GLDisplayList objects are updated. 
									If set to <tt> false</tt> only the display is redrawed.
					\see    GLDisplayList
			*/
			void update(bool rebuild_displaylists = false)
				throw();

			/** Handles messages sent by other registered ConnectionObject objects.
					Filters for SceneMessage and sets the camera appropriate or updates the visualization. 
					Calls update with <tt>false</tt> as parameter.
					\param message the pointer to the message that should be processed
					\see   ConnectionObject
					\see   Message
		  */
			virtual void onNotify(Message *message)
				throw();

			/** This method prepares an external Renderer for a later export of this scene.
			*/
			virtual void exportScene(Renderer &er) const
				throw();

			//@}			
			/** @name Event class
			*/
			//@{
			
			/** This object contains all possible events this scene can sent via the 
					Notification mechanism of BALL
					\see  Events
			*/
			Events events;

			//@}	
			/**	ModularWidget methods.
			*/
			//@{
			
			/**	Initialize the popup menu <b> Display</b> and the menus of this scene:
					  - the <b> rotate mode</b> (all mouse actions are attached to rotating, translating and zooming the scene)
					  - the <b> picking mode</b> (all mouse actions are attached to picking objects from the scene)
					\par
					This method is called automatically	immediately before the main application 
					is started. 
					This method will be called by show from the MainControl object.
					\param main_control the MainControl object to be initialized with this scene
					\see   finalizeWidget
					\see   insertMenuEntry
					\see   checkMenu
					\see   show
			*/
			virtual void initializeWidget(MainControl& main_control)
				throw();
		
			/**	Remove the widget.
					Reverse all actions performed in initializeWidget (remove menu entries of this scene).
					This method will be called by aboutToExit from the MainControl object.
					\param main_control the MainControl object to be finalized with this scene
					\see   initializeWidget
					\see   checkMenu
					\see   removeMenuEntry
					\see   aboutToExit
			*/
			virtual void finalizeWidget(MainControl& main_control)
				throw();
				
			///
			virtual void fetchPreferences(INIFile& inifile)
				throw();
			
			///
			virtual void writePreferences(INIFile& inifile)
				throw();

			/**	Menu checking method.
					This method is called by the method checkMenus from the
					MainControl object before a popup menu is shown.
					The menus <b> rotate mode</b> and <b> picking mode</b> each will be checked
					if this scene is in the beloning mode.
					\param main_control the MainControl object whose menus should be checked
					\see   initializeWidget
					\see   finalizeWidget
					\see   checkMenus
					\see   show
			*/
			virtual void checkMenu(MainControl& main_control)
				throw();
			
			//@}
			/**	@name	Debugging and Diagnostics
			*/
			//@{

			/** Internal state and consistency self-validation.
					If the internal state of this scene is correct 
					(self-validated) and consistent <tt> true</tt> is returned, <tt> false</tt> otherwise. 
					this scene is valid if:
						- this scene has a parent
					\par
					\return			bool <tt> true</tt> if the internal state of this scene is correct (self-validated) and consistent,
					 						<tt> false</tt> otherwise
			*/
			virtual bool isValid() const
				throw();

			/** Internal value dump.
					Dump the current internal state of this scene to 
					the output ostream <b> s</b> with dumping depth <b> depth</b>.
					\param   s output stream where to output the internal state of this scene
					\param   depth the dumping depth
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();
			
			///
			Stage* getStage()
				throw() { return stage_;}

			///
			const Stage* getStage() const
				throw() { return stage_;}

			///
			void setDefaultLighting(bool update_GL = true)
				throw();

			///
			void initializePreferencesTab(Preferences &preferences)
				throw();
			
			///
			void finalizePreferencesTab(Preferences &preferences)
				throw();

			///
			void applyPreferences(Preferences& /*preferences*/)
				throw();
				
			///
			void cancelPreferences(Preferences& /*preferences*/)
				throw();
				
			/// Set default values for the current page in Preferences
			virtual void defaultPreferences(Preferences& /*preferences*/)
				throw();
			
			///
			static void setMouseSensitivity(float sensitivity)
				throw() { mouse_sensitivity_ = sensitivity; }

			///
			static float getMouseSensitivity()
				throw() { return mouse_sensitivity_;}

			/// 
			GLRenderer& getGLRenderer_()
				throw() { return gl_renderer_;}
			
//			protected:

			//@}
			/** @name Protected QT overridden virtual methods
			*/
			//@{

			/** Initialize the OpenGL context.
					Overriden qt method for initializing the OpenGL context of this scene.
					This method will be called automatically before any call to paintGL or resizeGL.
					It initializes the OpenGL-Renderer via GLRenderer::init and GLRenderer::setStage.
					See QT-library for information concerning the qglwidget events.
					\see  paintGL
					\see  resizeGL
					\see GLRenderer::init
			*/
			virtual void initializeGL();

			/** Render the visualization.
					Overriden qt method for rendering the visualization of this scene.
					This method will be called automatically every time an update is necessary.
					See QT-library for information concerning qglwidgets and paintGL methods and events.
					\see  MainControl
			*/
			virtual void paintGL();

			/** Resize the widget.
					Overridden qt method for resizing this scene. 
					This method will be called automatically every time a rezize event is handled.
					See QT-library for information concerning qglwidgets and resizeGL methods and events.
					\param  width the new width of this scene
					\param  height the new height of this scene
			*/
			virtual void resizeGL(int width, int height);

			/** Catch key press events.
					Catch the following key press events:
					  - <tt> Key_Shift</tt> the SHIFT key was pressed
					  - <tt> Key_Control</tt> the SHIFT key was pressed
					\par
					This events will be stored for later processing.
					See QT-library for information concerning key press events.
					\param qkey_event the QT-key event (See QT-library for key events)
					\see   keyReleaseEvent
					\see   mouseMoveEvent
					\see   mousePressEvent
					\see   mouseReleaseEvent
			*/ 
			virtual void keyPressEvent(QKeyEvent* qkey_event);

			/** Catch key release events.
					Catch the following key release events:
					  - <tt> Key_Shift</tt> the SHIFT key was released
					  - <tt> Key_Control</tt> the SHIFT key was released
					\par
					This events will be stored for later processing.
					See QT-library for information concerning key released events.
					\param qkey_event the QT-key event (See QT-library for key events)
					\see  keyPressEvent
					\see  mouseMoveEvent
					\see  mousePressEvent
					\see  mouseReleaseEvent
			*/ 
			virtual void keyReleaseEvent(QKeyEvent* qkey_event);

			/** Catch mouse move events.
					Catch mouse move events, store the actual mouse position in this scene
					widget and sent events accordingly.
					Calls Notify
					\param  qmouse_event the QT-mouse event (See QT-library for mouse events)
					\see    Events
			*/
			virtual void mouseMoveEvent(QMouseEvent* qmouse_event);

			/** Catch mouse press events.
					Catch mouse press events, store the actual mouse position in this scene
					widget and sent events accordingly.
					Calls Notify
					\param  qmouse_event the QT-mouse event (See QT-library for mouse events)
					\see    Events
			*/
			virtual void mousePressEvent(QMouseEvent* qmouse_event);

			/** Catch mouse release events.
					Catch mouse release events, store the actual mouse position in this scene
					widget and sent events accordingly.
					Calls Notify
					\param  qmouse_event the QT-mouse event (See QT-library for mouse events)
					\see    Events
			*/
			virtual void mouseReleaseEvent(QMouseEvent* qmouse_event);

			public slots:

			///
			void exportPNG();

			/** Show or hide widget (Called by menu entry in "WINDOWS")
					If the ModularWidget is not also a QWidget, this method does nothing
			*/
			virtual void switchShowWidget()
				throw();

			///
			void switchStereo()
				throw();

			protected slots:

			//@}
			/** @name Protected slots
			*/
			//@{

			/** Switch to rotate mode.
					If this method is called the mouse actions of this scene will
					perform rotation, translation and zooming the visualization.
					This method will be called from the belonging menu entry.
					\see initializeWidget
					\see checkMenu
			*/
			virtual void rotateMode_();

			/** Switch to picking mode.
					If this method is called the mouse actions of this scene will
					perform object picking.
					This method will be called from the belonging menu entry.
					\see initializeWidget
					\see checkMenu
			*/
			virtual void pickingMode_();


			/// Open mouse sensitivity dialog
			virtual void mouseSensitivity_()
				throw();

			/// Show the viewpoint and the look at point in the statusline of the mainwidget.
			virtual void showViewPoint_()
				throw();

			/// Set the viewpoint 
			virtual void setViewPoint_()
				throw();

			/// Reset the camera to standard values
			virtual void resetCamera_()
				throw();

			/// Update the GL camera and if necessary the lights
			virtual void updateCamera_()
				throw();
			
			//@}

			virtual void customEvent( QCustomEvent * e );

			protected:

			private:

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

			void renderView_(RenderMode mode)
				throw();

			//_ called by renderView_
			void renderRepresentations_(RenderMode mode)
				throw();

			//_ called by renderRepresentations_
			void render_(const Representation& rep, RenderMode mode)
				throw();

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

			void calculateQuaternion_(Quaternion& quaternion, const Quaternion* rotate = 0);

			float sphereProject_(float radius, float x, float y);

			void selectObjects_(bool select = true);

			void writeLights_(INIFile& inifile) const
				throw();
			
			void readLights_(const INIFile& inifile) 
				throw();

			void createCoordinateSystem_()
				throw();
			
			// Menu entry IDs
			Index rotate_id_, picking_id_, stereo_id_;
			
			bool rotate_mode_;

			Vector3 system_origin_;
			Quaternion quaternion_;

			KeyPressed		key_pressed_;
			MouseButton		actual_mouse_button_;

			bool need_update_;
			bool update_running_;

			float x_window_pos_old_;
			float y_window_pos_old_;

			float x_window_pos_new_;
			float y_window_pos_new_;

			float x_window_pick_pos_first_;
			float y_window_pick_pos_first_;

			float x_window_pick_pos_second_;
			float y_window_pick_pos_second_;

			Stage* stage_;

			GLRenderer gl_renderer_;

			static float mouse_sensitivity_;

			LightSettings* light_settings_;

			StageSettings* stage_settings_;

			Position screenshot_nr_;
		};

#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/VIEW/WIDGETS/scene.iC>
#	endif

} } // namespaces

#endif // BALL_VIEW_WIDGETS_SCENE_H
