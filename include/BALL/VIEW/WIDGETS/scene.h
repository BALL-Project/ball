// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: scene.h,v 1.35 2004/06/03 11:17:18 amoll Exp $
//

#ifndef BALL_VIEW_WIDGETS_SCENE_H
#define BALL_VIEW_WIDGETS_SCENE_H

#ifndef BALL_MATHS_QUATERNION_H
#	include <BALL/MATHS/quaternion.h>
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

// has to come after BALL includes to prevent problems with Visual Stuio Net
#include <qgl.h>

namespace BALL
{
	namespace VIEW
	{
		class Preferences;
		class LightSettings;
		class StageSettings;

		/**	Scene is the main visualization widget that shows the graphical Representation 's.
				To do this, the class Scene must be a child of the MainControl.
				Because the MainControl is also the main application object
				a Scene must be created with the pointer to the MainControl 
				as parent widget. <br>
				Scene is capable of stereo 3D view with shutter glasses and CRT monitors. This only works, if the 
				OpenGL-driver supports this. NVIDIA cards need a Quad buffer, like they are used in the "Quad" cards.
				For other NVIDIA cards, you can try the tool <a href="http://www.guru3d.com/rivatuner">"Rivatuner"</a>, 
				which tries to enable this feature for cards, which natively dont support this. To enter
				and leave the stereo mode, press ALT-Y. If your OpenGL doesnt support the stereo view, you will see an
				error message in the message bar at the bottom of the main window.
				<br>
				<br>
				The Scene has tree different mouse modi: <br>
				- Picking Mode: To select Composite 's, e.g. Molecules
				- Rotate Mode: Transform your view point in the threedimensional room, e.g. rotate or zoom
				- Eye Distance: Change the eye distance for stereo view
					
				To change between the first two modi, there is a menu entry with checkboxes in the main
				menu bar of the application. 
				<br>
				In Picking mode, left mouse button selects composites and right mouse button deselects.
				There are two ways to (de-)select: Either click on single items, or draw a selection rectangle
				by keeping the mouse button pressed and moving the mouse.
				Users with only one mouse button can use the SHIFT button, while pressing the mouse button
				to deselect. 
				<br>
				In Rotate mode, left mouse button rotates, mid mouse button zooms in and out and right button
				moves the view.
				Users with only one mouse button can use the SHIFT / CONTROL button, while pressing the mouse button.
				<br>
				The eye distance change mode is only available in the stereo view mode. Here a user can press the
				ALT-button and move the mouse left or right to adjust the eye distance to their desired value.
				<br>
				Its possible to add new modi, to do so, derive a new class from Scene and overload the mouse*Event methods.
				<br>
				<br>
				It is possible to connect Scenes together with the Notification
				mechanism of BALL. Connecting two or more Scenes together means that that
				mouse action performed in one Scene are also transfered to all other connected
				Scenes. These other Scenes can have different Camera angles or other properties.
				\ingroup ViewWidgets
		*/
		class BALL_EXPORT Scene
			: public QGLWidget, 
				public ModularWidget
		{
			Q_OBJECT

		  public:

			BALL_EMBEDDABLE(Scene, ModularWidget)

			/** This class is only intended for usage with multithreading.
			 		It induces a rebuild of the display lists and an update of the Scene and
					should only be used internaly.
			*/
			class BALL_EXPORT SceneUpdateEvent : public QCustomEvent
			{
				public:
					SceneUpdateEvent()
						: QCustomEvent( SCENE_UPDATE_EVENT){}
			};
			
			/** @name Type definitions 
			*/
			//@{


			/**	Predefined constants for the mode types
			 		Add new enums members in derived classes, for new modi.
					If you add new modi in this class, you have to add them in front of
					the PICKING__MODE entry.
			*/
			enum ModeType
			{
				/// Default value.
				ROTATE__MODE         = 0,

				// add new modi here!!!
				
				/// Picking mode
				PICKING__MODE
			};

			
			//@} 
			/** @name Enums 
			*/ 
			//@{
			
			/** There a three different kind of rendering for a Scene:
			*/
			enum RenderMode
			{
				/// Render without display lists, directly to the Scene
				DIRECT_RENDERING = 0,

				/// Render the contents of the display lists
				DISPLAY_LISTS_RENDERING,

				/// Rebuild the contents of the display lists and redraw them
				REBUILD_DISPLAY_LISTS
			};
				
			//@} 
			/**	@name	Constructors 
			*/	
			//@{

			Scene()
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
			Scene(QWidget* parent_widget, const char* name = NULL, WFlags w_flags = 0)
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
			Scene (const Scene& scene, QWidget* parent_widget = NULL, const char* name = NULL, WFlags wflags = 0)
				throw();

			//@} 
			/** @name Destructors 
			*/ 
			//@{

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

			//@} 
			/**	@name	Assignment 
			*/ 
			//@{

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

			/** Update the visualization.
					\param  rebuild_displaylists is set to true than all GLDisplayList are updated. 
									If set to false, the display is only redrawed.
					\see    GLDisplayList
			*/
			void update(bool rebuild_displaylists = false)
				throw();

			/** Handles messages sent by other registered ConnectionObject objects.
					Filters for SceneMessage and sets the Camera appropriate or updates the visualization. 
					\param message the pointer to the message that should be processed
					\see   ConnectionObject
					\see   Message
		  */
			virtual void onNotify(Message *message)
				throw();

			/** This method exports the content of the Scene to an external Renderer.
			*/
			virtual bool exportScene(Renderer &er) const
				throw();

			//@}	
			/**	ModularWidget methods.
			*/
			//@{
			
			/**	Initialize the popup menu <b> Display</b> and the menus of this scene:
					  - the <b> rotate mode</b> (all mouse actions are attached to rotating, 
							translating and zooming the scene)
					  - the <b> picking mode</b> (all mouse actions are attached to picking objects from the scene)
					\par
					This method is called automatically	immediately before the main application is started 
					by MainControl::show().
					\param main_control the MainControl object to be initialized with this scene
					\see   finalizeWidget
					\see   insertMenuEntry
					\see   checkMenu
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
					This method is called by MainControl::checkMenus before a popup menu is shown.
					The menus <b>rotate mode</b> and <b>picking mode</b> each will be checked
					if this scene is in the belonging mode.
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
					A Scene is valid if it has a parent.
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
			static void setMouseWheelSensitivity(float sensitivity)
				throw() { mouse_wheel_sensitivity_ = sensitivity; }

			///
			static float getMouseWheelSensitivity()
				throw() { return mouse_wheel_sensitivity_;}

			/// 
			GLRenderer& getGLRenderer_()
				throw() { return gl_renderer_;}
			
			protected:

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

			/** Catch mouse move events, store the actual mouse position in this scene
					widget and sent events accordingly.
					\param  e the QT-mouse event (See QT-library for mouse events)
			*/
			virtual void mouseMoveEvent(QMouseEvent* qmouse_event);

			/** Catch mouse press events, store the actual mouse position in this scene
					widget and sent events accordingly.
					\param  e the QT-mouse event (See QT-library for mouse events)
			*/
			virtual void mousePressEvent(QMouseEvent* qmouse_event);

			/** Catch mouse release events, store the actual mouse position in this scene
					widget and sent events accordingly.
					\param  e the QT-mouse event (See QT-library for mouse events)
			*/
			virtual void mouseReleaseEvent(QMouseEvent* qmouse_event);

#ifndef QT_NO_WHEELEVENT
			/** Catch mouse wheel events and zoom the scene accordingly.
					\param  e the QT-mouse event (See QT-library for mouse events)
			*/
			virtual void wheelEvent(QWheelEvent* qmouse_event);
#endif

			/// Catch key events
			void keyPressEvent(QKeyEvent* e);
			
			//_ state of the scene: picking or rotate mode?
			int current_mode_;
			
			public slots:

			/// Export PNG image and return the filename
			String exportPNG();
			
			///
			void exportPOVRay();

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
			
			//_
			virtual void customEvent( QCustomEvent * e );

			//@}

			private:

			void renderView_(RenderMode mode)
				throw();

			//_ called by renderView_
			void renderRepresentations_(RenderMode mode)
				throw();

			//_ called by renderRepresentations_
			void render_(const Representation& rep, RenderMode mode)
				throw();


			void changeEyeDistance_(Scene* scene);
			void changeFocalDistance_(Scene* scene);

			void processRotateModeMouseEvents_(QMouseEvent* e);

			void rotateSystem_(Scene* scene);
			void translateSystem_(Scene* scene);
			void zoomSystem_(Scene* scene);

			void selectionPressed_();
			void selectionReleased_();
			void selectionPressedMoved_();
			void deselectionReleased_();

			void calculateQuaternion_(Quaternion& quaternion, const Quaternion* rotate = 0);

			//_ called by calculateQuaternion_
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
			
			Vector3 system_origin_;
			Quaternion quaternion_;

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
			static float mouse_wheel_sensitivity_;

			LightSettings* light_settings_;

			StageSettings* stage_settings_;

			Position screenshot_nr_,
							 pov_nr_;

			QPoint last_pos_;

			static QGLFormat gl_format_;
		};

} } // namespaces

#endif // BALL_VIEW_WIDGETS_SCENE_H
