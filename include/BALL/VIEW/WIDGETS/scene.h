// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: scene.h,v 1.66.2.19 2006/10/20 08:31:50 amoll Exp $
//

#ifndef BALL_VIEW_WIDGETS_SCENE_H
#define BALL_VIEW_WIDGETS_SCENE_H

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
#	include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#ifndef BALL_VIEW_RENDERING_GLRENDERER_H
# include <BALL/VIEW/RENDERING/glRenderer.h>
#endif

#ifndef BALL_VIEW_KERNEL_COMMON_H
# include <BALL/VIEW/KERNEL/common.h>
#endif 

// has to come after BALL includes to prevent problems with Visual Studio Net
#include <QtOpenGL/qgl.h>

#include <QtCore/qthread.h>
#include <QtCore/qtimer.h>
#include <QtGui/qevent.h>
#include <QtGui/QDragEnterEvent>
#include <QtGui/QWheelEvent>
#include <QtGui/QKeyEvent>
#include <QtGui/QDropEvent>

class QMouseEvent;
class QRubberBand;
class QMenu;

namespace BALL
{
	namespace VIEW
	{
		class Preferences;
		class LightSettings;
		class StageSettings;
		class MaterialSettings;
		class AnimationThread;
		class ClippingPlane;

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
		class BALL_VIEW_EXPORT Scene
			: public QGLWidget, 
				public ModularWidget
		{
			friend class AnimationThread;

			Q_OBJECT

		  public:

			BALL_EMBEDDABLE(Scene, ModularWidget)

			/** This class is only intended for usage with multithreading.
			 		It provides a mean for other threads to make the Scene export a PNG.
			*/
			class BALL_VIEW_EXPORT SceneExportPNGEvent : public QEvent
			{
				public:
					SceneExportPNGEvent()
						: QEvent((QEvent::Type) SCENE_EXPORTPNG_EVENT){}
			};

			/** This class is only intended for usage with multithreading.
			 		It provides a mean for other threads to make the Scene export a POVRay file.
			*/
			class BALL_VIEW_EXPORT SceneExportPOVEvent : public QEvent
			{
				public:
					SceneExportPOVEvent()
						: QEvent((QEvent::Type) SCENE_EXPORTPOV_EVENT){}
			};

			/** This class is only intended for usage with multithreading.
			 		It provides a mean for other threads to set the camera position.
			*/
			class BALL_VIEW_EXPORT SceneSetCameraEvent : public QEvent
			{
				public:
					SceneSetCameraEvent()
						: QEvent((QEvent::Type) SCENE_SETCAMERA_EVENT){}

					Camera camera;
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

				/// Move mode
				MOVE__MODE,

				// add new modi here!!!
				
				/// Picking mode
				PICKING__MODE
			};

			/// Different Mouse Mode actions
			enum ModeAction
			{
				///
				TRANSLATE_ACTION,

				///
				ZOOM_ACTION,

				///
				ROTATE_ACTION,

				///
				ROTATE_CLOCKWISE_ACTION
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
			/**	@name	Constructors and Destructor
			*/	
			//@{

			Scene()
				throw();

			/** Default Constructor.
					Initialize the width and height of this scene to <tt> 600</tt> and sets
					the camera position.
					\par
					Calls registerWidget.
					\param      parent_widget the parent widget of this scene 
					\param      name the name of this scene 
					\param      w_flags the flags the scene widget should have 
											(See documentation of QT-library for information concerning widget flags) 
			*/
			Scene(QWidget* parent_widget, const char* name = NULL, Qt::WFlags w_flags = 0)
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
			Scene (const Scene& scene, QWidget* parent_widget = NULL, const char* name = NULL, Qt::WFlags wflags = 0)
				throw();

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
			*/
			virtual void initializeWidget(MainControl& main_control)
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
					if this scene is in the corresponding mode.
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
			void setCamera(const Camera& camera)
				throw();

			///
			List<Camera>& getAnimationPoints()
				throw() { return animation_points_;}

			///
			static void setAnimationSmoothness(float value)
				throw() { animation_smoothness_ = value;}

			///
			static float getAnimationSmoothness()
				throw() { return animation_smoothness_;}

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
			virtual void applyPreferences()
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
			static void setShowLightSources(bool state)
				throw() { show_light_sources_ = state;}
			
			///
			static bool showLightSourcesEnabled() 
				throw() { return show_light_sources_;}

			/// 
			GLRenderer& getGLRenderer()
				throw() { return gl_renderer_;}

			///
			static bool stereoBufferSupportTest();
			
			///
			float getMousePositionX() { return x_window_pos_new_;}

			///
			float getMousePositionY() { return y_window_pos_new_;}

			///
			void setPopupInfosEnabled(bool state);
	
			///
			bool exportPNG(const String& filename);

			///
			virtual void setWidgetVisible(bool state);

			///
			void setOffScreenRendering(bool enabled, Size factor);

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

			/** Catch mouse wheel events and zoom the scene accordingly.
					\param  e the QT-mouse event (See QT-library for mouse events)
			*/
			virtual void wheelEvent(QWheelEvent* qmouse_event);

			/// Catch key events
			void keyPressEvent(QKeyEvent* e);


			public slots:

			/// Create an coordinate system at current position
			void createCoordinateSystem()
				throw();

			/// Create an coordinate system at origin
			void createCoordinateSystemAtOrigin()
				throw();

			/// Export PNG image and return the filename
			String exportPNG();

			/// show an dialog to save an PNG file to
			void showExportPNGDialog();

			///
			void showExportVRMLDialog();
			
			///
			void exportPOVRay();

			/** Show or hide widget (Called by menu entry in "WINDOWS")
					If the ModularWidget is not also a QWidget, this method does nothing
			*/
			virtual void switchShowWidget()
				throw();

			///
			void exitStereo()
				throw();

			///
			void enterActiveStereo()
				throw();

			///
			void enterDualStereo()
				throw();

			///
			void clearRecordedAnimation()
				throw();
			
			///
			void startAnimation()
				throw();

			///
			void stopAnimation()
				throw();

			///
			void switchToLastMode()
				throw();

			///
			ModeType getMode() const
				throw() { return current_mode_;}

			///
			virtual void setMode(ModeType mode)
				throw();

			///
			static void setScreenShotNumber(Position pos) { screenshot_nr_ = pos;}

			///
			static void setPOVNumber(Position pos) { pov_nr_ = pos;}

			///
			void rotate(float degree_right, float degree_up);

			///
			void rotateClockwise(float degree);

			/** Move the view. \\
			 		v.x = right  \\
					v.y = up     \\
					v.z = view direction 
			*/
			void move(Vector3 v);

			/** Move some Composites. \\
			 		v.x = right  \\
					v.y = up     \\
					v.z = view direction 
			*/
			void moveComposites(const List<Composite*>& composites, Vector3 v);

			/** Rotate some Composites. \\
			 		v.x = right  \\
					v.y = up     \\
					v.z = view direction 
			*/
			void rotateComposites(const List<Composite*>& composites, float degree_right, float degree_up, float degree_clockwise = 0);

			void initTimer();

			///
			bool isAnimationRunning() const
				throw();

			///
			void setTurnPoint(const Vector3& v) { system_origin_ = v;}

			///
			const Vector3& getTurnPoint() const { return system_origin_;}

			///
			void setFullScreen(bool state);

			/** Show text in the lower right corner.
			 		To clear the text, call this method again with an empty string.
			*/
			void showText(const String& text, Size font_size = 20);

		
			protected slots:

			//@}
			/** @name Protected slots
			*/
			//@{

			/** Switch to rotate mode.
					If this method is called the mouse actions of this scene will
					perform rotation, translation and zooming the visualization.
					This method will be called from the corresponding menu entry.
					\see initializeWidget
					\see checkMenu
			*/
			virtual void rotateMode_();

			/** Switch to picking mode.
					If this method is called the mouse actions of this scene will
					perform object picking.
					This method will be called from the corresponding menu entry.
					\see initializeWidget
					\see checkMenu
			*/
			virtual void pickingMode_();

			/**
			*/
			virtual void moveMode_();
			
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
			virtual bool event(QEvent* e);

			///
			virtual void dropEvent(QDropEvent* e);

			///
			virtual void dragEnterEvent(QDragEnterEvent* e);

			///
			virtual void timerSignal_();

			//@}

			protected:

			void updateGL();

			void renderView_(RenderMode mode)
				throw();

			//_ called by renderView_
			void renderRepresentations_(RenderMode mode)
				throw();

			//_ called by renderRepresentations_
			void render_(const Representation& rep, RenderMode mode)
				throw();

			//_
			void animate_()
				throw();

			void processRotateModeMouseEvents_(QMouseEvent* e);
			void processMoveModeMouseEvents_(QMouseEvent* e);

			void rotateSystem_();
			void rotateSystemClockwise_();
			void translateSystem_();
			void zoomSystem_();
			Index getMoveModeAction_(const QMouseEvent& e);

			void selectionPressed_();
			void selectionPressedMoved_();

			void selectObjects_();

			void writeLights_(INIFile& inifile) const
				throw();
			
			void readLights_(const INIFile& inifile) 
				throw();

			inline float getXDiff_();
			inline float getYDiff_();
			inline Vector3 getTranslationVector_(const Vector3& v);
			
			void createCoordinateSystem_(bool at_origin)
				throw();
	
			//_ state of the scene: picking or rotate mode?
			ModeType current_mode_;

			//_ last state of the scene: picking or rotate mode?
			ModeType last_mode_;
	
			// Menu entry IDs
			QAction *rotate_action_, *picking_action_, *move_action_;
			QAction *no_stereo_action_, *active_stereo_action_, *dual_stereo_action_;
			QAction *record_animation_action_, *start_animation_action_, *clear_animation_action_, *cancel_animation_action_;
			QAction *animation_export_POV_action_, *animation_export_PNG_action_, *animation_repeat_action_;
			QMenu* create_coordinate_system_;
			
			Vector3 system_origin_;

			bool need_update_;
			bool update_running_;

			Index x_window_pos_old_;
			Index y_window_pos_old_;
			Index x_window_pos_new_;
			Index y_window_pos_new_;

			Index x_window_pick_pos_first_;
			Index y_window_pick_pos_first_;
			Index x_window_pick_pos_second_;
			Index y_window_pick_pos_second_;
			bool pick_select_;
			QRubberBand* rb_;

			Stage* stage_;
			Camera stereo_camera_;

			GLRenderer gl_renderer_;

			static float mouse_sensitivity_;
			static float mouse_wheel_sensitivity_;
			static bool show_light_sources_;
			static float animation_smoothness_;

			LightSettings* light_settings_;
			StageSettings* stage_settings_;
			MaterialSettings* material_settings_;

			// nr of last png file export
			static Position screenshot_nr_;
			// nr of last pov file export
			static Position pov_nr_;

			QPoint last_pos_;

			static QGLFormat gl_format_;
			List<Camera> animation_points_;
			AnimationThread* animation_thread_;
			bool stop_animation_;
			bool content_changed_;
			bool want_to_use_vertex_buffer_;
			bool mouse_button_is_pressed_;
			QTimer timer_;

			// Position of mouse cursor for identifying Composite
			Position last_x_pos_, last_y_pos_;

			bool show_info_;
			PreciseTime time_;
			float last_fps_;
			float zoom_factor_;
			QPoint info_point_;
			String info_string_;
			QByteArray last_state_;
			bool show_fps_;
			static bool offscreen_rendering_;
			static QSize PNG_size_;
			String text_;
			Size   font_size_;
		};


		///
		class BALL_VIEW_EXPORT AnimationThread
			: public QThread
		{
			public:
				
				///
				AnimationThread()
					throw(){};

				///
				virtual void run() {((Scene*) Scene::getInstance(0))->animate_();}

				///
				void mySleep(Size msec) {msleep(msec);}
		};


} } // namespaces

#endif // BALL_VIEW_WIDGETS_SCENE_H
