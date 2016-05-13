// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_WIDGETS_SCENE_H
#define BALL_VIEW_WIDGETS_SCENE_H

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
# include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#ifndef BALL_VIEW_RENDERING_RENDERERSGLRENDERER_H
# include <BALL/VIEW/RENDERING/RENDERERS/glRenderer.h>
#endif

#ifndef BALL_VIEW_KERNEL_COMMON_H
# include <BALL/VIEW/KERNEL/common.h>
#endif

#ifndef BALL_VIEW_RENDERING_RENDERWINDOW_H
# include <BALL/VIEW/RENDERING/renderWindow.h>
#endif

#ifndef BALL_VIEW_RENDERING_GLRENDERWINDOW_H
# include <BALL/VIEW/RENDERING/glRenderWindow.h>
#endif

#ifndef BALL_VIEW_RENDERING_RENDERSETUP_H
# include <BALL/VIEW/RENDERING/renderSetup.h>
#endif

#ifndef BALL_VIEW_DIALOGS_EDITSETTINGS_H
# include <BALL/VIEW/DIALOGS/editSettings.h>
#endif

#ifndef BALL_VIEW_KERNEL_EDITOPERATION_H
# include <BALL/VIEW/KERNEL/editOperation.h>
#endif

#ifndef BALL_VIEW_KERNEL_MODES_INTERACTIONMODEMANAGER_H
# include <BALL/VIEW/KERNEL/MODES/interactionModeManager.h>
#endif

#include <QtCore/QThread>
#include <QtCore/QTimer>
#include <QtGui/QDragEnterEvent>
#include <QtGui/QDropEvent>
#include <QtWidgets/QToolBar>
#include <QtGui/QFont>
#include <QtGui/QPicture>

#include <boost/shared_ptr.hpp>

#ifdef BALL_HAS_RTFACT

#ifndef BALL_VIEW_RENDERING_RAYTRACINGRENDERER_H
# include <BALL/VIEW/RENDERING/RENDERERS/raytracingRenderer.h>
#endif

#endif // BALL_HAS_RTFACT

class QMouseEvent;
class QRubberBand;
class QMenu;
class QImage;
class QAction;
class QWheelEvent;
class QKeyEvent;

namespace BALL
{
	class Bond;
	class PDBAtom;
	class AtomContainer;

	namespace VIEW
	{
		class ColorMap;
		class InteractionMode;
		class Preferences;
		class LightSettings;
		class StageSettings;
		class MaterialSettings;
		class AnimationThread;
		class ClippingPlane;
		class CompositeMessage;
		class ControlSelectionMessage;
		class RepresentationMessage;
		class DatasetMessage;
		class SceneMessage;

		class TransformationEvent6D;
		class MotionTrackingEvent;
		class ButtonEvent;
		class RenderToBufferFinishedEvent;

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
				Furthermore, users can create modified versions of the GLRenderer and pass them to
				the Scene with the method setGLRenderer.
				\ingroup ViewWidgets
		*/
		class BALL_VIEW_EXPORT Scene
			: public QWidget,
				public ModularWidget
		{
			friend class AnimationThread;
			friend class RenderSetup;

#ifdef BALL_HAS_RTFACT
      typedef boost::shared_ptr<RaytracingRenderer> RaytracingRendererPtr;
      typedef boost::shared_ptr<RenderWindow> RaytracingWindowPtr;
#endif

			Q_OBJECT

			public:
				BALL_EMBEDDABLE(Scene, ModularWidget)

				/** @name Enums
				*/
				//@{

				/**
				 * Scene may hold several windows for different renering modes.
				 * If you need to retrieve a particular window, use this enum together
				 * with Scene::getWindow() to get a reference to it.
				 */
				enum WindowType
				{
					/// Main window that is always being displayed directly in the QT main interface
					CONTROL_WINDOW = 0,

					/// Window into which left eye should be rendered. Indicates same window as CONTROL_WINDOW if no stereo is running.
					LEFT_EYE_WINDOW,

					/// Window into which right eye should be renderer.  Indicates same window as CONTROL_WINDOW if no stereo is running.
					RIGHT_EYE_WINDOW
				};

				/**	@name	Constructors and Destructor
				*/
				//@{

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
				Scene(QWidget* parent_widget = 0, const char* name = NULL, Qt::WindowFlags w_flags = 0);

				/** Destructor.
				*/
				virtual ~Scene();

				/** Explicit default initialization.
					Reset the default values of this scene to:
					-  width is set to <tt> 600</tt>
					-  height is set to <tt> 600</tt>
					- camera position set to <tt> Vector(1,0,0)</tt>
					- camera look at position set to <tt> Vector(0,0,0)</tt>
					\par
					*/
				virtual void clear();

				//@}

				//@}
				/**	@name	Accessors: inspectors and mutators
				*/
				//@{

				/** Handles messages sent by other registered ConnectionObject objects.
					Filters for SceneMessage and sets the Camera appropriate or updates the visualization.
					\param message the pointer to the message that should be processed
					\see   ConnectionObject
					\see   Message
					*/
				virtual void onNotify(Message *message);

				/** This method exports the content of the Scene to an external Renderer.
				*/
				virtual bool exportScene(Renderer &er) const;

				String getBondOrderString_(Index order);

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
				virtual void initializeWidget(MainControl& main_control);

				///
				virtual void fetchPreferences(INIFile& inifile);

				///
				virtual void writePreferences(INIFile& inifile);

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
				virtual void checkMenu(MainControl& main_control);

				//@}
				/**	@name	Debugging and Diagnostics
				*/
				//@{

				/** Internal state and consistency self-validation.
					A Scene is valid if it has a parent.
					*/
				virtual bool isValid() const;

				/** Internal value dump.
					Dump the current internal state of this scene to
					the output ostream <b> s</b> with dumping depth <b> depth</b>.
					\param   s output stream where to output the internal state of this scene
					\param   depth the dumping depth
					*/
				virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;

				void setInfo(const QString& info_string);
				bool isRecording() const;
				void captureCamera();
				Atom* getCurrentAtom() { return current_atom_; }
				void setCurrentAtom(Atom* atom) { current_atom_ = atom; }
				void notify(Message* msg) { notify_(msg); }
				Vector2 map3DToViewport(const Vector3& coords);
				Vector3 mapViewportTo3D(const Vector2& coords);
				Vector3 mapViewportTo3D(int x, int y);
				void setRubberBandGeometry(const QRect& rect);
				void setRubberBandVisible(bool show);
				void pickObjects(const QPoint& pos, list<GeometricObject*>& objects);
				void pickObjects(const QPoint& pos1, const QPoint& pos2, list<GeometricObject*>& objects);
				void merge(Composite* a1, Composite* a2);
				std::list<AtomContainer*> getContainers();
				void enterPickingMode();
				void exitPickingMode();

				void drawLine(const QPointF& a, const QPointF& b, QPainter* painter = 0);
				void drawLine(const Vector2& a, const Vector2& b, QPainter* painter = 0);

				Bond* getCurrentBond() { return current_bond_; }
				void setCurrentBond(Bond* b) { current_bond_ = b; }

				void changeBondOrder();
				void changeBondOrder(Index delta);
				void activatedOrderItem_(QAction* action);
				void createMolecule_();
				void setFormalCharge_();

				void deselect(bool update=true);
				void getClickedItems(const QPoint& p);

				///
				Stage* getStage()
				{ return stage_;}

				///
				const Stage* getStage() const
				{ return stage_;}

				///
				StageSettings* getStageSettings()
				{ return stage_settings_; }

				///
				const StageSettings* getStageSettings() const
				{ return stage_settings_; }

				///
				MaterialSettings* getMaterialSettings()
				{ return material_settings_; }

				///
				const MaterialSettings* getMaterialSettings() const
				{ return material_settings_; }

				///
				void setCamera(const Camera& camera);

				///
				std::list<Camera>& getAnimationPoints()
				{ return animation_points_;}

				///
				static void setAnimationSmoothness(float value)
				{ animation_smoothness_ = value;}

				///
				static float getAnimationSmoothness()
				{ return animation_smoothness_;}

				///
				float getDownsamplingFactor() const
				{ return downsampling_factor_;}

				///
				void setDownsamplingFactor(float ds_factor);

				///
				void setDefaultLighting(bool update_GL = true);

				///
				static void setMouseSensitivity(float sensitivity)
				{ mouse_sensitivity_ = sensitivity; }

				///
				static float getMouseSensitivity()
				{ return mouse_sensitivity_;}

				///
				static void setMouseWheelSensitivity(float sensitivity)
				{ mouse_wheel_sensitivity_ = sensitivity; }

				///
				static float getMouseWheelSensitivity()
				{ return mouse_wheel_sensitivity_;}

				///
				static void setShowLightSources(bool state)
				{ show_light_sources_ = state;}

				///
				static bool showLightSourcesEnabled()
				{ return show_light_sources_;}

				///
				GLRenderer& getGLRenderer()
				{ return *gl_renderer_;}

#ifdef BALL_HAS_RTFACT
				///
				RaytracingRenderer& getRaytracingRenderer()
				{ return *rt_renderer_;}
#endif

				/** Set a new GLRenderer.
					This method is intended for users, that what to
					overload the behaviour of the GLRenderer.
					*/
				void setGLRenderer(GLRenderer& renderer);

				///
				static bool stereoBufferSupportTest();

				///
				bool inMoveMode() const;

				///
				bool exportPNG(const String& filename);

				///
				virtual void setWidgetVisible(bool state);

				///
				void setOffScreenRendering(bool enabled, Size factor);

				///
				virtual void paintEvent(QPaintEvent* e);

				///
				virtual bool eventFilter(QObject* object, QEvent* event);

				///
				virtual void projectionModeChanged();

				///
				static void setScreenShotNumber(Position pos) { screenshot_nr_ = pos;}

				///
				static void setPOVNumber(Position pos) { pov_nr_ = pos;}

				///
				static void setVRMLNumber(Position pos) {vrml_nr_ = pos;}

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

				// TODO: move this and the next function somewhere else...
				/** Move some Composites. \\
					v.x = right  \\
					v.y = up     \\
					v.z = view direction
					*/
				void moveComposites(const std::list<Composite*>& composites, Vector3 v);

				/** Rotate some Composites. \\
					v.x = right  \\
					v.y = up     \\
					v.z = view direction
					*/
				void rotateComposites(const std::list<Composite*>& composites, float degree_right, float degree_up, float degree_clockwise = 0);

				///
				bool isAnimationRunning() const;

				///
				void setTurnPoint(const Vector3& v) { system_origin_ = v;}

				///
				const Vector3& getTurnPoint() const { return system_origin_;}

				///
				void setFullScreen(bool state);

				///
				void setFPSEnabled(bool state) { show_fps_ = state; }

				/** Show text in the lower right corner.
					To clear the text, call this method again with an empty string.
					*/
				void showText(const String& text, Size font_size = 20);

				///
				bool isUpdateRunning() const { return update_running_;}

				void resetTracking() {tracking_initialized_ = false;}

				// TODO: this pretty hacky!
				void updateAllMaterials();

				void updateMaterialForRepresentation(Representation* rep, const Stage::Material& new_material);

				void lightsUpdated(bool redraw = false);

				/// Try to setup an environment map for all renderers that support it
				void setupEnvironmentMap(const QImage& image);

				Position prepareGridTextures(const RegularData3D& grid, const ColorMap& map);

				void updateGL();
				///
				void initializePreferencesTab(Preferences &preferences);

				///
				void finalizePreferencesTab(Preferences &preferences);

				///
				virtual void applyPreferences();

				///
				void showContextMenu(QPoint pos);

				// Allow to use the base class setCursor, too
				using QWidget::setCursor;

				///
				void setCursor(String c);

				///
				void setElementCursor(int number);

				///
				virtual void addToolBarEntries(QToolBar* tb);

				/// Catch key events
				virtual void keyPressEvent(QKeyEvent* e);

			/////////////////////////////////////////
			public slots:
				/// Create an coordinate system at current position
				void createCoordinateSystem();

				/// Create an coordinate system at origin
				void createCoordinateSystemAtOrigin();

				/// Export PNG image and return the filename
				String exportPNG();

				///
				void exportPOVRay();

				/// Export to POVRay whithout showing file dialog
				void exportNextPOVRay();

				///
				void exportXML3D();

				///
				void dumpXML3D(std::ostream& xml3ddump);

				///
				void printScene();

				/// show an dialog to save an PNG file to
				void showExportPNGDialog();

				/// opens the VIEW/DIALOGS/GeometryExportDialog for vrml and stl export
				void showExportVRMLDialog();

				/// Enable or disable model previews e.g. while rotating
				void setPreview(bool state) { use_preview_ = state; }

				/// Returns the state of the preview mode
				bool usePreview() const { return use_preview_; }

				bool useVertexBuffers() const { return want_to_use_vertex_buffer_; }

				/** Show or hide widget (Called by menu entry in "WINDOWS")
					If the ModularWidget is not also a QWidget, this method does nothing
					*/
				virtual void switchShowWidget();

				///
				void switchRenderer(RenderSetup::RendererType new_type);

				///
				void addGlWindow();

#ifdef BALL_HAS_RTFACT
				///
				void addRTfactWindow();
#endif

				///
				void exitStereo();

				///
				void enterStereo();

				///
				void enterActiveStereo();

				///
				void enterDualStereo();

				///
				void enterDualStereoDifferentDisplays();

				///
				void clearRecordedAnimation();

				///
				void startAnimation();

				///
				void stopAnimation();

				///
				void switchToLastMode();

				///
				void switchShowGrid();

				/// Popup informations for object under mouse cursor
				void showInfos();

				///
				void setupViewVolume();

				///
				void storeViewPoint();

				///
				void restoreViewPoint();

				///
				void startContinuousLoop();

				///
				void stopContinuousLoop();

				///
				void toggleContinuousLoop();

				///
				void applyStereoDefaults();

#ifdef BALL_HAS_RTFACT
				RaytracingWindowPtr getWindow(WindowType aWindowType);
#endif

				void createNewMolecule();
				void saturateWithHydrogens();
				void optimizeStructure();
				void computeBondOrders();

				// slots for communication with PTEDialog
				void setEditElementType(int element_number);
				int getEditElementType();

			////////////////////////////////////////
			protected slots:

				/** @name Protected slots
				*/
				//@{

				/**
				*/
				/// Show the viewpoint and the look at point in the statusline of the mainwidget.
				virtual void showViewPoint_();

				/// Set the viewpoint
				virtual void setViewPoint_();

				/// Reset the camera to standard values
				virtual void resetCamera_();

				/** Rebuffer all representations for the current renderer_.
				 *  Note: this function does not yet clear the old state of the renderer.
				 */
				virtual void resetRepresentationsForRenderer_(RenderSetup& rs);

				///
				virtual void dropEvent(QDropEvent* e);

				///
				virtual void dragEnterEvent(QDragEnterEvent* e);

				// dummy slot for menu entries without immediate action (saves many lines code this way)
				void dummySlot(){}

				//@}

			////////////////////////////////////////
			signals:

				// signal for communication with EditOperationDialog
				void newEditOperation(EditOperation &eo);

			////////////////////////////////////////
			protected:

				/** React to RepresentationMessages.
				 */
				void handleRepresentationMessage_(RepresentationMessage* rm);

				/** React to DatasetMessages.
				 */
				void handleDatasetMessage_(DatasetMessage* dm);

				/** React to SceneMessages.
				 */
				void handleSceneMessage_(SceneMessage* sm);

				/** React to CompositeMessages.
				 */
				void handleCompositeMessage_(CompositeMessage* cm);

				/** React to ControlSelectionMessages.
				 */
				void handleControlSelectionMessage_(ControlSelectionMessage* csm);

				/** React to RenderToBufferFinishedEvents
				 */
				virtual void handleRenderToBufferFinishedEvent_(RenderToBufferFinishedEvent* evt);

				/** Render a text at the given position.
				 *  Currently, this always uses a 16pt Arial bold font.
				 *  TODO: this should be configurable in the GUI.
				 */
				void renderText_(QPointF const& point, QString const& text, QPaintDevice* current_dev);

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
				TVector2<float> getScreenPosition_(Vector3 vec);

				void initializeMembers_();

				void renderGrid_();

				QAction *new_molecule_action_, *optimize_action_;
				QAction *add_hydrogens_action_;
				QAction *bondorders_action_;

				Atom* current_atom_;
				Bond* current_bond_;

				// pick atoms/bonds only in highlighted AtomContainer?
				static bool only_highlighted_;
				// element for new atoms
				int atomic_number_;

				//undo stack
				vector<EditOperation> undo_;
				EditSettings* edit_settings_;
				QList<QAction*> toolbar_actions_edit_controls_;

				/** @name Protected QT overridden virtual methods
				*/
				//@{

				/** Initialize the scene and all predefined render setups.
				*/
				virtual void init();

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
						\param  width the new width of this scene
						\param  height the new height of this scene
				*/
				virtual void resizeEvent(QResizeEvent* event);

				/**
				 * This function handles custom events that for example are sent
				 * by the input device drivers
				 * \param evt A pointer to the event that shell be processed
				 */
				virtual void customEvent(QEvent* evt);

				/**
				 * Function that interpretes the transformation events sent to customEvent()
				 */
				virtual void transformationEvent6D(TransformationEvent6D* evt);

				/**
				 * Function that interpretes the motion tracker events sent to customEvent()
				 */
				virtual void motionTrackingEvent(MotionTrackingEvent* evt);

				/**
				 * Function that interpretes the button press events sent to customEvent()
				 */
				virtual void buttonPressEvent(ButtonEvent* evt);

				/**
				 * Function that interpretes the button release events sent to customEvent()
				 */
				virtual void buttonReleaseEvent(ButtonEvent* evt);

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

				virtual void mouseDoubleClickEvent(QMouseEvent* e);

				//_
				void animate_();

				void rotateSystemClockwise_();
				void zoomSystem_();

				void selectionPressed_();
				void selectionPressedMoved_();

				void selectObjects_();
				void pickParent_(QPoint p);

				void writeLights_(INIFile& inifile) const;

				void readLights_(const INIFile& inifile);

				void createCoordinateSystem_(bool at_origin);

				void registerRenderers_();

				/// Estimate current fps and convert into a string
				String createFPSInfo_(Renderer* renderer);

				// Menu entry IDs
				QAction *no_stereo_action_, *enter_stereo_action_, *active_stereo_action_, *dual_stereo_action_, *dual_stereo_different_display_action_;
				QAction *record_animation_action_, *start_animation_action_, *clear_animation_action_, *cancel_animation_action_;
				QAction *animation_export_POV_action_, *animation_export_VRML_action_, 	*animation_export_PNG_action_, *animation_repeat_action_;

#ifdef BALL_HAS_RTFACT
                QAction *toggle_continuous_loop_action_;
#endif
				QAction *switch_grid_;
				QMenu* create_coordinate_system_;

				Vector3 system_origin_;
				Vector3 old_trackorigin_;
				bool tracking_initialized_;
				Quaternion old_trackrotation_;

				bool update_running_;

				QRubberBand* rb_;

				Stage* stage_;
				Camera stereo_camera_;
				Camera stored_camera_;

				std::vector<boost::shared_ptr<RenderSetup> > renderers_;
				GLRenderer* gl_renderer_;

#ifdef BALL_HAS_RTFACT
				RaytracingRenderer* rt_renderer_;
				RaytracingWindowPtr rt_window_;
#endif

				static float mouse_sensitivity_;
				static float mouse_wheel_sensitivity_;
				static bool show_light_sources_;
				static float animation_smoothness_;
				static float downsampling_factor_;

				LightSettings* light_settings_;
				StageSettings* stage_settings_;
				MaterialSettings* material_settings_;

				// nr of last png file export
				static Position screenshot_nr_;
				// nr of last pov file export
				static Position pov_nr_;
				//nr of last vrml or stl export
				static Position vrml_nr_;

				std::list<Camera> animation_points_;
				AnimationThread* animation_thread_;
				bool stop_animation_;
#ifdef BALL_HAS_RTFACT
				bool continuous_loop_;
#endif
				bool want_to_use_vertex_buffer_;
				bool use_preview_;

				PreciseTime time_;
				QPoint info_point_;
				QByteArray last_state_;
				list<float> fps_;
				bool show_fps_;
				static bool offscreen_rendering_;
				Size offscreen_factor_;
				Size   font_size_;
				QToolBar* toolbar_view_controls_;
				QToolBar* toolbar_edit_controls_;
				QList<QAction*> toolbar_actions_view_controls_;

				QString info_string_;

				GLRenderWindow* main_display_;
				/// The index of the renderer responsible for the main display
				Position main_renderer_;

				Index stereo_left_eye_;
				Index stereo_right_eye_;

				QFont default_font_;

				QPicture overlay_;
				bool has_overlay_;
				InteractionModeManager mode_manager_;
		};


		///
		class BALL_VIEW_EXPORT AnimationThread
			: public QThread
		{
			public:

				///
				AnimationThread(){};

				///
				virtual void run() {scene_->animate_();}

				///
				void mySleep(Size msec);

				///
				void setScene(Scene* scene) { scene_ = scene;}

				///
				Scene* getScene() { return scene_;}

			protected:

				Scene* scene_;
		};

	} // namespace VIEW
}  // namespace BALL

#endif // BALL_VIEW_WIDGETS_SCENE_H
