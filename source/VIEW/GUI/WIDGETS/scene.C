// $Id: scene.C,v 1.6 2001/05/27 10:31:25 hekl Exp $

#include <BALL/VIEW/GUI/WIDGETS/scene.h>

#include <qpainter.h>
#include <qkeycode.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qgl.h>

#include "events.C"

using namespace std;

namespace BALL
{

	namespace VIEW
	{

	  Scene::MainControlMissing::MainControlMissing
		  (const char* file, int line, const string& data)
			throw()
			: Exception::GeneralException(file, line, string("MainControlMissing"), data)
		{
    }
    
		// --------------------------------------------------------------------------
		// ---- Scene Constructors --------------------------------------------------
		// --------------------------------------------------------------------------
		/*
		Scene::Scene()
			throw()
			:	QGLWidget(),
				ModularWidget("<Scene>"),
				events(),
				rotate_id_(-1),
				picking_id_(-1),
				rotate_mode_(true),
				GL_object_collector_(0),
				system_origin_(0.0),
				quaternion_(),
				width_((Scene::Width)0),
				height_((Scene::Height)0),
				phase_(Scene::PHASE__STATIC_SCENE),
				render_mode_(Scene::RENDER_MODE__COMPILE),
				key_pressed_(Scene::KEY_PRESSED__NONE),
				was_picking_(false),
		    x_scale_((GLfloat)1),
				y_scale_((GLfloat)1),
				zoom_(),
				position_(1,0,0),
				look_at_position_(0,0,0),
				last_look_at_position_(0,0,0),
				look_up_(),
				right_(),
				up_()
		{
		}
		*/
		Scene::Scene
			(QWidget* parent_widget, const char* name, WFlags w_flags)
			throw()
			:	QGLWidget(parent_widget, name, 0, w_flags),
				ModularWidget(name),
				events(this),
				rotate_id_(-1),
				picking_id_(-1),
				rotate_mode_(true),
				GL_object_collector_((GLObjectCollector*)&RTTI::getDefault<GLObjectCollector>()),
				GL_primitive_manager_(),
				system_origin_(0.0, 0.0, 0.0),
				quaternion_(),
				width_((Scene::Width)600),
				height_((Scene::Height)600),
				phase_(Scene::PHASE__STATIC_SCENE),
				render_mode_(Scene::RENDER_MODE__COMPILE),
				key_pressed_(Scene::KEY_PRESSED__NONE),
				was_picking_(false),
				x_scale_((GLfloat)1),
				y_scale_((GLfloat)1),
				zoom_(),
				position_(1,0,0),
				look_at_position_(0,0,0),
				last_look_at_position_(0,0,0),
				look_up_(),
				right_(),
				up_()
		{
			events.RotateSystem.registerRotateSystem(&BALL::VIEW::Scene::rotateSystem_);
			events.TranslateSystem.registerTranslateSystem(&BALL::VIEW::Scene::translateSystem_);
			events.ZoomSystem.registerZoomSystem(&BALL::VIEW::Scene::zoomSystem_);

			events.SelectionPressedMoved.
				registerSelectionPressedMoved(&BALL::VIEW::Scene::selectionPressedMoved_);
			events.SelectionPressed.
				registerSelectionPressed(&BALL::VIEW::Scene::selectionPressed_);
			events.SelectionReleased.
				registerSelectionReleased(&BALL::VIEW::Scene::selectionReleased_);

			events.DeselectionPressedMoved.
				registerDeselectionPressedMoved(&BALL::VIEW::Scene::deselectionPressedMoved_);
			events.DeselectionPressed.
				registerDeselectionPressed(&BALL::VIEW::Scene::deselectionPressed_);
			events.DeselectionReleased.
				registerDeselectionReleased(&BALL::VIEW::Scene::deselectionReleased_);


			NotificationRegister
				(events.MouseLeftButtonPressed & events.MouseMoved, 
				 events.RotateSystem);

			NotificationRegister
				(events.MouseMiddleButtonPressed & events.MouseMoved, 
				 events.ZoomSystem);

			NotificationRegister
				(events.MouseRightButtonPressed & events.MouseMoved, 
				 events.TranslateSystem);

			// the widget with the MainControl
			registerWidget(this);
		}

		Scene::Scene
			(const Scene& scene, QWidget* parent_widget,
			 const char* name, WFlags w_flags)
			throw()
			:	QGLWidget(parent_widget, name, 0, w_flags),
				ModularWidget(scene),
				events(this),
				GL_object_collector_(scene.GL_object_collector_),
				GL_primitive_manager_(),
				system_origin_(scene.system_origin_),
				quaternion_(scene.quaternion_),
				width_(scene.width_),
				height_(scene.height_),
				phase_(Scene::PHASE__STATIC_SCENE),
				render_mode_(Scene::RENDER_MODE__COMPILE),
				key_pressed_(Scene::KEY_PRESSED__NONE),
				was_picking_(false),
				x_scale_(scene.x_scale_),
				y_scale_(scene.y_scale_),
				zoom_(scene.zoom_),
				position_(scene.position_),
				look_at_position_(scene.look_at_position_),
				last_look_at_position_(scene.last_look_at_position_),
				look_up_(scene.look_up_),
				right_(scene.right_),
				up_(scene.up_)
		{
			events.RotateSystem.registerRotateSystem(&BALL::VIEW::Scene::rotateSystem_);
			events.TranslateSystem.registerTranslateSystem(&BALL::VIEW::Scene::translateSystem_);
			events.ZoomSystem.registerZoomSystem(&BALL::VIEW::Scene::zoomSystem_);

			events.SelectionPressedMoved.
				registerSelectionPressedMoved(&BALL::VIEW::Scene::selectionPressedMoved_);
			events.SelectionPressed.
				registerSelectionPressed(&BALL::VIEW::Scene::selectionPressed_);
			events.SelectionReleased.
				registerSelectionReleased(&BALL::VIEW::Scene::selectionReleased_);

			events.DeselectionPressedMoved.
				registerDeselectionPressedMoved(&BALL::VIEW::Scene::deselectionPressedMoved_);
			events.DeselectionPressed.
				registerDeselectionPressed(&BALL::VIEW::Scene::deselectionPressed_);
			events.DeselectionReleased.
				registerDeselectionReleased(&BALL::VIEW::Scene::deselectionReleased_);



			NotificationRegister
				(events.MouseLeftButtonPressed & events.MouseMoved,
				 events.RotateSystem);

			NotificationRegister
				(events.MouseMiddleButtonPressed & events.MouseMoved,
				 events.ZoomSystem);

			NotificationRegister
				(events.MouseRightButtonPressed & events.MouseMoved,
				 events.TranslateSystem);

			resize((int)width_, (int)height_);

			// the widget with the MainControl
			ModularWidget::registerWidget(this);
		}

		Scene::~Scene()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<Scene>() << endl;
			#endif 

			destroy();
		}

		void Scene::clear()
			throw()
		{
			GL_object_collector_ 
				= (GLObjectCollector *)&RTTI::getDefault<GLObjectCollector>();

			system_origin_.set(0.0, 0.0, 0.0);
			quaternion_.setIdentity();
			width_ = (Scene::Width)600;
			height_ = (Scene::Height)600;
			phase_ = Scene::PHASE__STATIC_SCENE;
			render_mode_ = Scene::RENDER_MODE__COMPILE;
			key_pressed_ = Scene::KEY_PRESSED__NONE;
			x_scale_ = (GLfloat)1;
			y_scale_ = (GLfloat)1;

			zoom_ = Vector3(0,0,0);
			position_ = Vector3(1,0,0);
			look_at_position_ = Vector3(0,0,0);
			last_look_at_position_ = Vector3(0,0,0);
			look_up_ = Vector3(0,0,0);
			right_ = Vector3(0,0,0);
			up_ = Vector3(0,0,0);
		}
			
		void Scene::destroy()
			throw()
		{
		}

		void Scene::set(const Scene& scene)
			throw()
		{
			GL_object_collector_ = scene.GL_object_collector_;

			system_origin_.set(scene.system_origin_);

			quaternion_.set(scene.quaternion_);

			width_ = scene.width_;
			height_ = scene.height_;

			resize((int)width_, (int)height_);

			x_scale_ = scene.x_scale_;
			y_scale_ = scene.y_scale_;

			zoom_ = scene.zoom_;
			position_ = scene.position_;
			look_at_position_ = scene.look_at_position_;
			last_look_at_position_ = scene.last_look_at_position_;
			look_up_ = scene.look_up_;
			right_ = scene.right_;
			up_ = scene.up_;
		}

		void Scene::update(bool rebuild_displaylists)
			throw(MainControlMissing)
		{
			// rebuild displaylists
			if (rebuild_displaylists == true)
			{
				MainControl::DescriptorIterator descriptor_iterator;
				
				ConnectionObject *object = getRoot();
				MainControl *main_control = RTTI::castTo<MainControl>(*object);
				
				if (main_control == 0)
				{
					throw ::BALL::VIEW::Scene::MainControlMissing(__FILE__, __LINE__, "");
				}

				// redraw the system
				for (descriptor_iterator 
							 = main_control->getDescriptorList().begin();
						 descriptor_iterator != main_control->getDescriptorList().end(); 
						 ++descriptor_iterator)
					{
						CompositeDescriptor *composite_descriptor
							= *descriptor_iterator;
						
						composite_descriptor->update();
					}
			}

			updateGL();
		}

		void Scene::onNotify(Message *message)
			throw()
		{
			if (RTTI::isKindOf<SceneMessage>(*message))
			{
				SceneMessage *scene_message = RTTI::castTo<SceneMessage>(*message);

				if (scene_message->isUpdateOnly() == false)
				{
					setCamera(scene_message->getCameraLookAt(),
										scene_message->getCameraViewPoint());
				}

				update(false);
			}
		}

		const Scene& Scene::operator = (const Scene& scene)
			throw()
		{
			set(scene);
			return *this;
		}

		void Scene::get(Scene& scene) const
			throw()
		{
			scene.set(*this);
		}

		bool Scene::isValid() const
			throw()
		{
			return (getParent() != 0 &&
							GL_primitive_manager_.isValid() &&
							system_origin_.isValid());
		}

		void Scene::dump(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);

			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "Camera: -----------" << endl;

			BALL_DUMP_DEPTH(s, depth + 1);
			s << "look at point:     " << getLookAtPosition() <<endl;
			
			BALL_DUMP_DEPTH(s, depth + 1);
			s << "viewpoint:         " << getViewPointPosition() <<endl;
			
			GL_primitive_manager_.dump(s, depth + 1);

			quaternion_.dump(s, depth + 1);

			BALL_DUMP_DEPTH(s, depth + 1);
			s << "width: " << width_ <<endl;
			
			BALL_DUMP_DEPTH(s, depth + 1);
			s << "height: " << height_ <<endl;
			
			BALL_DUMP_DEPTH(s, depth + 1);
			s << "Phase: " << (int)phase_ << endl;

			BALL_DUMP_DEPTH(s, depth + 1);
			s << "RenderMode: " << (int)render_mode_ << endl;

			BALL_DUMP_DEPTH(s, depth + 1);
			s << "KeyPressed: " << (int)key_pressed_ << endl;

			BALL_DUMP_DEPTH(s, depth + 1);
			s << "x scale: " << x_scale_ << endl;

			BALL_DUMP_DEPTH(s, depth + 1);
			s << "y scale: " << y_scale_ << endl;

			BALL_DUMP_STREAM_SUFFIX(s);     
		}

		void Scene::read(istream & /*s */)
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void Scene::write(ostream & /*s */) const
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

			
		// --------------------------------------------------------------------------
		// ---- protected Member functions (QT) -------------------------------------
		// --------------------------------------------------------------------------

		void Scene::initializeGL()
		{
			if (!format().rgba())
			{
				cerr << "no rgba mode for OpenGL available." << endl;
			}

			glFrontFace(GL_CCW);
			glCullFace(GL_BACK);
			glEnable(GL_CULL_FACE);

			glEnable(GL_NORMALIZE);

			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);
			glClearDepth(200.0);
			glShadeModel(GL_SMOOTH);
		 
			GLfloat diff[] = {1.0, 1.0, 1.0, 1.0};
			GLfloat shin[] = {50.0};
			GLfloat pos[]  = {0.0,0.0,100.0,0.0};

			glMaterialfv(GL_FRONT, GL_SPECULAR, diff);
			glMaterialfv(GL_FRONT, GL_SHININESS, shin);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
			glLightfv(GL_LIGHT0, GL_POSITION, pos);

			glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

			glEnable(GL_COLOR_MATERIAL);
			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);

			GL_primitive_manager_.init();

			initViewVectors_();
			setCamera_();
		}

		void Scene::paintGL()
		{
			makeCurrent();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			renderView_();
		}

		void Scene::resizeGL(int width, int height)
		{
			makeCurrent();

			if (width > height)
			{
				x_scale_ = (GLfloat)width / ((GLfloat)height * (GLfloat)2);
				y_scale_ = (GLfloat)0.5;
			}
			else
			{
				x_scale_ = (GLfloat)0.5;
				y_scale_ = (GLfloat)height / ((GLfloat)width * (GLfloat)2);
			}

			glViewport(0, 0, width, height);

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();

			glFrustum
				(-2.0 * x_scale_, 2.0 * x_scale_, -2.0 * y_scale_,
					2.0 * y_scale_, 1.5, 300);

			setCamera_();

			glMatrixMode(GL_MODELVIEW);

			width_ = (Scene::Width)width;
			height_ = (Scene::Height)height;
		}

		void Scene::keyPressEvent(QKeyEvent *key_event)
		{
			switch (key_event->key())
			{
				case Key_Shift:
					key_pressed_ = (Scene::KeyPressed)((int)key_pressed_ + 1);
					break;

				case Key_Control:
					key_pressed_ = (Scene::KeyPressed)((int)key_pressed_ + 2);
					break;

				default:
					key_event->ignore();
					break;
			}
		}

		void Scene::keyReleaseEvent(QKeyEvent *key_event)
		{
			switch (key_event->key())
			{
				case Key_Shift:
					key_pressed_ = (Scene::KeyPressed)((int)key_pressed_ - 1);
					break;

				case Key_Control:
					key_pressed_ = (Scene::KeyPressed)((int)key_pressed_ - 2);
					break;

				default:
					key_event->ignore();
					break;
			}
		}

		void Scene::mouseMoveEvent(QMouseEvent *mouse_event)
		{
			makeCurrent();

			need_update_ = true;

			x_window_pos_new_ = (Real)(mouse_event->x());
			y_window_pos_new_ = (Real)(mouse_event->y());

			switch (actual_mouse_button_)
			{
				case Scene::MOUSE_BUTTON__NONE:
					break;

				case Scene::MOUSE_BUTTON__ONE:
					switch (key_pressed_)
					{
						case Scene::KEY_PRESSED__NONE:
							Notify(events.MouseLeftButtonPressedMouseMoved);
							break;

						case Scene::KEY_PRESSED__SHIFT:
							Notify(events.MouseLeftButtonPressedShiftKeyPressedMouseMoved);
							break;

						case Scene::KEY_PRESSED__CONTROL:
							Notify(events.MouseLeftButtonPressedControlKeyPressedMouseMoved);
							break;

						case Scene::KEY_PRESSED__SHIFT_CONTROL:
							Notify(events.MouseLeftButtonPressedShiftKeyPressedControlKeyPressedMouseMoved);
							break;
					}
					break;

				case Scene::MOUSE_BUTTON__TWO:
					switch (key_pressed_)
					{
						case Scene::KEY_PRESSED__NONE:
							Notify(events.MouseMiddleButtonPressedMouseMoved);
							break;

						case Scene::KEY_PRESSED__SHIFT:
							Notify(events.MouseMiddleButtonPressedShiftKeyPressedMouseMoved);
							break;

						case Scene::KEY_PRESSED__CONTROL:
							Notify(events.MouseMiddleButtonPressedControlKeyPressedMouseMoved);
							break;

						case Scene::KEY_PRESSED__SHIFT_CONTROL:
							Notify(events.MouseMiddleButtonPressedShiftKeyPressedControlKeyPressedMouseMoved);
							break;
					}
					break;

				case Scene::MOUSE_BUTTON__THREE:
					switch (key_pressed_)
					{
						case Scene::KEY_PRESSED__NONE:
							Notify(events.MouseRightButtonPressedMouseMoved);
							break;

						case Scene::KEY_PRESSED__SHIFT:
							Notify(events.MouseRightButtonPressedShiftKeyPressedMouseMoved);
							break;

						case Scene::KEY_PRESSED__CONTROL:
							Notify(events.MouseRightButtonPressedControlKeyPressedMouseMoved);
							break;

						case Scene::KEY_PRESSED__SHIFT_CONTROL:
							Notify(events.MouseRightButtonPressedShiftKeyPressedControlKeyPressedMouseMoved);
							break;
					}
					break;
			}

			x_window_pos_old_ = x_window_pos_new_;
			y_window_pos_old_ = y_window_pos_new_;
		}

		void Scene::mousePressEvent(QMouseEvent* mouse_event)
		{
			makeCurrent();

			x_window_pos_old_ = (Real)(mouse_event->x());
			y_window_pos_old_ = (Real)(mouse_event->y());

			phase_ = Scene::PHASE__DYNAMIC_SCENE;

			switch (mouse_event->button())
			{
				case NoButton:
					actual_mouse_button_ = Scene::MOUSE_BUTTON__NONE;
					break;

				case LeftButton:
					actual_mouse_button_ = Scene::MOUSE_BUTTON__ONE;

					switch (key_pressed_)
					{
						case Scene::KEY_PRESSED__NONE:
							Notify(events.MouseLeftButtonPressed);
							break;

						case Scene::KEY_PRESSED__SHIFT:
							Notify(events.MouseLeftButtonPressedShiftKeyPressed);
							break;

						case Scene::KEY_PRESSED__CONTROL:
							Notify(events.MouseLeftButtonPressedControlKeyPressed);
							break;

						case Scene::KEY_PRESSED__SHIFT_CONTROL:
							Notify(events.MouseLeftButtonPressedShiftKeyPressedControlKeyPressed);
							break;
					}
					break;
					
				case MidButton:
					actual_mouse_button_ = Scene::MOUSE_BUTTON__TWO;

					switch (key_pressed_)
					{
						case Scene::KEY_PRESSED__NONE:
							Notify(events.MouseMiddleButtonPressed);
							break;

						case Scene::KEY_PRESSED__SHIFT:
							Notify(events.MouseMiddleButtonPressedShiftKeyPressed);
							break;

						case Scene::KEY_PRESSED__CONTROL:
							Notify(events.MouseMiddleButtonPressedControlKeyPressed);
							break;

						case Scene::KEY_PRESSED__SHIFT_CONTROL:
							Notify(events.MouseMiddleButtonPressedShiftKeyPressedControlKeyPressed);
							break;
					}
					break;
					
				case RightButton:
					actual_mouse_button_ = Scene::MOUSE_BUTTON__THREE;

					switch (key_pressed_)
					{
						case Scene::KEY_PRESSED__NONE:
							Notify(events.MouseRightButtonPressed);
							break;

						case Scene::KEY_PRESSED__SHIFT:
							Notify(events.MouseRightButtonPressedShiftKeyPressed);
							break;

						case Scene::KEY_PRESSED__CONTROL:
							Notify(events.MouseRightButtonPressedControlKeyPressed);
							break;

						case Scene::KEY_PRESSED__SHIFT_CONTROL:
							Notify(events.MouseRightButtonPressedShiftKeyPressedControlKeyPressed);
							break;

						default:
							break;
					}
				default:
					break;

			}
		}

		void Scene::mouseReleaseEvent(QMouseEvent *mouse_event)
		{
			makeCurrent();

			phase_ = Scene::PHASE__STATIC_SCENE;

			switch (mouse_event->button())
			{
				case NoButton:
					actual_mouse_button_ = Scene::MOUSE_BUTTON__NONE;
					break;

				case LeftButton:
					actual_mouse_button_ = Scene::MOUSE_BUTTON__ONE;

					switch (key_pressed_)
					{
						case Scene::KEY_PRESSED__NONE:
							Notify(events.MouseLeftButtonReleased);
							break;

						case Scene::KEY_PRESSED__SHIFT:
							Notify(events.MouseLeftButtonReleasedShiftKeyPressed);
							break;

						case Scene::KEY_PRESSED__CONTROL:
							Notify(events.MouseLeftButtonReleasedControlKeyPressed);
							break;

						case Scene::KEY_PRESSED__SHIFT_CONTROL:
							Notify(events.MouseLeftButtonReleasedShiftKeyPressedControlKeyPressed);
							break;
					}
					break;
					
				case MidButton:
					actual_mouse_button_ = Scene::MOUSE_BUTTON__TWO;

					switch (key_pressed_)
					{
						case Scene::KEY_PRESSED__NONE:
							Notify(events.MouseMiddleButtonReleased);
							break;

						case Scene::KEY_PRESSED__SHIFT:
							Notify(events.MouseMiddleButtonReleasedShiftKeyPressed);
							break;

						case Scene::KEY_PRESSED__CONTROL:
							Notify(events.MouseMiddleButtonReleasedControlKeyPressed);
							break;

						case Scene::KEY_PRESSED__SHIFT_CONTROL:
							Notify(events.MouseMiddleButtonReleasedShiftKeyPressedControlKeyPressed);
							break;
					}
					break;
					
				case RightButton:
					actual_mouse_button_ = Scene::MOUSE_BUTTON__THREE;

					switch (key_pressed_)
					{
						case Scene::KEY_PRESSED__NONE:
							Notify(events.MouseRightButtonReleased);
							break;

						case Scene::KEY_PRESSED__SHIFT:
							Notify(events.MouseRightButtonReleasedShiftKeyPressed);
							break;

						case Scene::KEY_PRESSED__CONTROL:
							Notify(events.MouseRightButtonReleasedControlKeyPressed);
							break;

						case Scene::KEY_PRESSED__SHIFT_CONTROL:
							Notify(events.MouseRightButtonReleasedShiftKeyPressedControlKeyPressed);
							break;

						default:
							break;
					}
				default:
					break;
			}

			if (need_update_ == true)
			{
				updateGL();

				need_update_ = false;
			}
		}

		void Scene::rotateMode_()
		{
			rotate_mode_ = true;
			setRenderMode_(Scene::RENDER_MODE__COMPILE);
	
			// unpicking mode controls
			NotificationUnregister
				(events.MouseLeftButtonPressed);
			
			NotificationUnregister
				(events.MouseLeftButtonPressed & events.MouseMoved);
			
			NotificationUnregister
				(events.MouseLeftButtonReleased);
			
			NotificationUnregister
				(events.MouseRightButtonPressed);
			
			NotificationUnregister
				(events.MouseRightButtonPressed & events.MouseMoved);
			
			NotificationUnregister
				(events.MouseRightButtonReleased);
			
			
			// rotation mode controls
			NotificationRegister
				(events.MouseLeftButtonPressed & events.MouseMoved, 
				 events.RotateSystem);
			
			NotificationRegister
				(events.MouseMiddleButtonPressed & events.MouseMoved, 
				 events.ZoomSystem);
			
			NotificationRegister
				(events.MouseRightButtonPressed & events.MouseMoved, 
				 events.TranslateSystem);			
		}

		void Scene::pickingMode_()
		{
			rotate_mode_ = false;
			setRenderMode_(Scene::RENDER_MODE__DO_NOT_COMPILE);
			
			// unrotation mode controls
			NotificationUnregister
				(events.MouseLeftButtonPressed & events.MouseMoved);
			
			NotificationUnregister
				(events.MouseMiddleButtonPressed & events.MouseMoved);
			
			NotificationUnregister
				(events.MouseRightButtonPressed & events.MouseMoved);
			
			
			// picking mode controls
			NotificationRegister
				(events.MouseLeftButtonPressed,
				 events.SelectionPressed);
			
			NotificationRegister
				(events.MouseLeftButtonPressed & events.MouseMoved, 
				 events.SelectionPressedMoved);
			
			NotificationRegister
				(events.MouseLeftButtonReleased, 
				 events.SelectionReleased);
			
			NotificationRegister
				(events.MouseRightButtonPressed,
				 events.DeselectionPressed);
			
			NotificationRegister
				(events.MouseRightButtonPressed & events.MouseMoved, 
				 events.DeselectionPressedMoved);
			
			NotificationRegister
				(events.MouseRightButtonReleased, 
				 events.DeselectionReleased);
		}

		// --------------------------------------------------------------------------
		// ---- private Member functions --------------------------------------------
		// --------------------------------------------------------------------------
				
		void Scene::renderView_(bool with_names)
		{
			RenderMode saved_render_mode = render_mode_;

			if (with_names)
			{
				render_mode_ = RENDER_MODE__DO_NOT_COMPILE;
			}

			#ifdef BALL_VIEW_DEBUG
				cout << "rendering." << endl;
			#endif

				//			glEnable(GL_COLOR_MATERIAL);

			Matrix4x4 m;
			GLfloat matrix[4][4];

			// calculate system rotation
			quaternion_.getRotationMatrix(m);
			convertMatrix_(m, matrix);

			glPushMatrix();

			/* translate the system origin */
			glTranslatef((GLfloat)system_origin_.x,
									 (GLfloat)system_origin_.y,
									 (GLfloat)system_origin_.z);

			// rotate the system
			glMultMatrixf(&matrix[0][0]);

			MainControl::DescriptorIterator descriptor_iterator;

			ConnectionObject *object = getParent();
			
			if (object == 0)
			{
				throw ::BALL::VIEW::Scene::MainControlMissing(__FILE__, __LINE__, "");
			}
			
			MainControl *main_control = RTTI::castTo<MainControl>(*object);

			if (main_control == 0)
			{
				throw ::BALL::VIEW::Scene::MainControlMissing(__FILE__, __LINE__, "");
			}
			
			// draw the system
			for (descriptor_iterator = main_control->getDescriptorList().begin();
					 descriptor_iterator != main_control->getDescriptorList().end(); 
					 ++descriptor_iterator)
			{
				CompositeDescriptor *composite_descriptor
					= *descriptor_iterator;

				// the primitive manager: used to get the right displaylists
				composite_descriptor->registerPrimitiveManager(GL_primitive_manager_);

				// the opengl collector: collects all graphical information
				composite_descriptor->registerGLObjectCollector(*GL_object_collector_);

				glPushMatrix();

				#ifdef BALL_VIEW_DEBUG
					cout << "drawing." << endl;
				#endif

				/* calculate the the center of the object respective to the system origin */
				Vector3 center = composite_descriptor->getCenter();

				center -= system_origin_;

				glTranslatef((GLfloat)center.x,
										 (GLfloat)center.y,
										 (GLfloat)center.z); 

				// calculate the objects rotation matrix 
				composite_descriptor->getQuaternion().getRotationMatrix(m);

				convertMatrix_(m, matrix);
				
				glMultMatrixf(&matrix[0][0]);

				/* pull displaylist to its center */
				center = composite_descriptor->getCenter();

				glTranslatef((GLfloat)-center.x,
										 (GLfloat)-center.y,
										 (GLfloat)-center.z); 

				switch(phase_)
				{
					case Scene::PHASE__STATIC_SCENE:
						{
							if (render_mode_ == Scene::RENDER_MODE__COMPILE) // use compiled lists
							{
								composite_descriptor->drawEntity();
							}
							else // draw the objects directly
							{
								composite_descriptor->drawDirect(false, with_names);
							}
						}
						break;

					case Scene::PHASE__DYNAMIC_SCENE:
					{
						if (render_mode_ == Scene::RENDER_MODE__COMPILE) // use compiled lists
						{
							composite_descriptor->drawEntity(true);
						}
						else // draw the objects directly
						{
							composite_descriptor->drawDirect(true, with_names);
						}
					}
					break;
				}

				glPopMatrix();
			}

			glPopMatrix();

			//			glDisable(GL_COLOR_MATERIAL);

			// return to saved render mode, if names were turned on
			if (with_names)
			{
				render_mode_ = saved_render_mode;
			}
		}

		//////////////////////////////////////////////////////////////////////////////////
		// registerable functions begin
		//////////////////////////////////////////////////////////////////////////////////

		void Scene::rotateSystem_(Scene *scene)
		{
			scene->calculateQuaternion_(quaternion_, Quaternion());
			
			need_update_ = true;
			was_picking_ = false;

			updateGL();
		}

		void Scene::translateSystem_(Scene *scene)
		{
			Vector3 view = getViewPointPosition();
			Vector3 look_at = getLookAtPosition();

			Real distance = view.getDistance(look_at);

			Vector3 v = scene->translateObjectXY_(distance);

			setViewPointPosition(view + v);
			setLookAtPosition(look_at + v, false);

			need_update_ = true;
			was_picking_ = false;

			updateGL();
		}

		void Scene::zoomSystem_(Scene *scene)
		{
			Vector3 view = getViewPointPosition();
			//			Vector3 look_at = getLastLookAtPosition_();
			Vector3 look_at = getLookAtPosition();

			Real distance = view.getDistance(look_at);

			Vector3 v = scene->translateObjectZ_(distance);

			setViewPointPosition(view + v);

			if (scene != this)
			{
				setLookAtPosition(look_at + v, false);
			}

			need_update_ = true;
			was_picking_ = false;

			updateGL();
		}

	  void Scene::selectionPressed_(Scene* /* scene */)
    {
			x_window_pick_pos_first_ = x_window_pos_old_;
			y_window_pick_pos_first_ = y_window_pos_old_;
			
			x_window_pick_pos_second_ = x_window_pos_old_;
			y_window_pick_pos_second_ = y_window_pos_old_;
    }

  	void Scene::selectionReleased_(Scene* /* scene */)
    {
			selectObjects_(true);
			
			need_update_ = true;
			
			updateGL();
    }

	  void Scene::selectionPressedMoved_(Scene* /* scene */)
    {
			x_window_pick_pos_second_ = x_window_pos_new_;
			y_window_pick_pos_second_ = y_window_pos_new_;

			int x_pick_old_i = (int)x_window_pick_pos_first_;
			int y_pick_old_i = (int)y_window_pick_pos_first_;
			int x_pos_old_i  = (int)x_window_pos_old_;
			int y_pos_old_i  = (int)y_window_pos_old_;
			int x_pos_new_i  = (int)x_window_pos_new_;
			int y_pos_new_i  = (int)y_window_pos_new_;

			QPainter painter(this);
			painter.setPen(white);
			painter.setRasterOp(XorROP);
  
			painter.drawRect(x_pick_old_i,
											 y_pick_old_i,
											 x_pos_old_i - x_pick_old_i,
											 y_pos_old_i - y_pick_old_i);
			
			painter.drawRect(x_pick_old_i,
											 y_pick_old_i,
											 x_pos_new_i - x_pick_old_i,
											 y_pos_new_i - y_pick_old_i);
			
			painter.end();
    }


	  void Scene::deselectionPressed_(Scene* /* scene */)
    {
			x_window_pick_pos_first_ = x_window_pos_old_;
			y_window_pick_pos_first_ = y_window_pos_old_;
			
			x_window_pick_pos_second_ = x_window_pos_old_;
			y_window_pick_pos_second_ = y_window_pos_old_;
    }

  	void Scene::deselectionReleased_(Scene* /* scene */)
    {
			selectObjects_(false);
			
			need_update_ = true;
			
			updateGL();
    }

	  void Scene::deselectionPressedMoved_(Scene* /* scene */)
    {
			x_window_pick_pos_second_ = x_window_pos_new_;
			y_window_pick_pos_second_ = y_window_pos_new_;

			int x_pick_old_i = (int)x_window_pick_pos_first_;
			int y_pick_old_i = (int)y_window_pick_pos_first_;
			int x_pos_old_i  = (int)x_window_pos_old_;
			int y_pos_old_i  = (int)y_window_pos_old_;
			int x_pos_new_i  = (int)x_window_pos_new_;
			int y_pos_new_i  = (int)y_window_pos_new_;

			QPainter painter(this);
			painter.setPen(white);
			painter.setRasterOp(XorROP);
  
			painter.drawRect(x_pick_old_i,
											 y_pick_old_i,
											 x_pos_old_i - x_pick_old_i,
											 y_pos_old_i - y_pick_old_i);
			
			painter.drawRect(x_pick_old_i,
											 y_pick_old_i,
											 x_pos_new_i - x_pick_old_i,
											 y_pos_new_i - y_pick_old_i);

			painter.end();
    }



		//////////////////////////////////////////////////////////////////////////////////
		// registerable functions end
		//////////////////////////////////////////////////////////////////////////////////


		Vector3 Scene::translateObjectXY_(const Real distance)
		{
			Vector3 right = getRightVector_();
			Vector3 up = getUpVector_();

			right *= ((x_window_pos_old_ - x_window_pos_new_) 
								/ (Real)width_) * (1.4 * distance) * 2.0 * x_scale_;

			up *=((y_window_pos_old_ - y_window_pos_new_) 
						/ (Real)height_) * (1.4 * distance) * 2.0 * y_scale_;

			return right + up;
		}

		Vector3 Scene::translateObjectZ_(const Real distance)
		{
			Real delta_y = y_window_pos_new_ - y_window_pos_old_;
			
			return (delta_y / (float)height_ * distance) * getZoomVector_();  
		}

		Vector3 Scene::calculateRotatedVector_(const Vector3& v, const Quaternion& q)
		{
			Matrix4x4 m;
			
			q.getRotationMatrix(m);
			
			Vector4 tmp(v.x, v.y, v.z);
			
			tmp = m * tmp;
			
			return Vector3(tmp.x, tmp.y, tmp.z);
		}

		void Scene::calculateQuaternion_(Quaternion& q, const Quaternion& rotate)
		{
			Quaternion tmp;

			Real right1 = ((float)width_ - x_window_pos_old_ * 2.0) / width_;
			Real up1 = ((float)height_ - y_window_pos_old_ * 2.0) / height_;

			Real right2 = ((float)width_ - x_window_pos_new_ * 2.0) / width_;
			Real up2 = ((float)height_ - y_window_pos_new_ * 2.0) / height_;

			if (BALL_REAL_EQUAL(right1, right2, Constants::EPSILON) == true
					&& BALL_REAL_EQUAL(up1, up2, Constants::EPSILON) == true)
			{
				tmp.setIdentity();
			}
			else
			{
				Vector3 a = (Vector3)(getRightVector_()) * right1
									+ (Vector3)getUpVector_() * up1
									+ (Vector3)getZoomVector_() 
										* sphereproject_(0.8, right1, up1);

				Vector3 b = (Vector3)getRightVector_() * right2
									+ (Vector3)getUpVector_() * up2
									+ (Vector3)getZoomVector_() 
										* sphereproject_(0.8, right2, up2);

				Vector3 cross = a % b;
				
				cross = calculateRotatedVector_(cross, rotate);

				Real phi = (a - b).getLength();
				
				if (BALL_REAL_GREATER(phi, (Real)1, Constants::EPSILON) == true)
				{
					phi = (Real)1;
				}

				if (BALL_REAL_LESS(phi, (Real)-1, Constants::EPSILON) == true)
				{
					phi = (Real)-1;
				}

				phi = (Real)(2.0 * asin((float)phi));
				
				tmp.set(cross, -phi);
			}

			q += tmp;
		}

		Real Scene::sphereproject_(Real radius, Real x, Real y)
		{
			Real z;

			Real dist = (Real)sqrt((float)(x * x + y * y));

			if (BALL_REAL_LESS(dist, radius * sqrt(2.0) / 2.0, Constants::EPSILON) == true)
			{
				z = (Real)sqrt((float)(radius * radius - dist * dist));
			}
			else
			{
				Real t = radius / sqrt(2.0);
				z = t * t / dist;
			}

			return z;
		}

	  // picking routine ------
	  void Scene::selectObjects_(bool select)
    {
			GLint viewport[4];
			int number_of_hits;
			unsigned int object_buffer[10000];

			// init name stack for 10000 objects
			glGetIntegerv(GL_VIEWPORT, viewport);
			glSelectBuffer(10000, object_buffer);
			glRenderMode(GL_SELECT);
			glInitNames();
			glPushName((unsigned int)-1);
			
			glMatrixMode(GL_PROJECTION);
			
			glPushMatrix();
			glLoadIdentity();
			
			// calculate picking rectangle
			int width = BALL_ABS((int)x_window_pick_pos_second_ 
													 - (int)x_window_pick_pos_first_);
			
			int height = BALL_ABS((int)y_window_pick_pos_second_ 
															 - (int)y_window_pick_pos_first_);
			
			int center_x = BALL_MIN((int)x_window_pick_pos_second_,
															(int)x_window_pick_pos_first_) + width / 2;
			
			int center_y = BALL_MIN((int)y_window_pick_pos_second_, 
															(int)y_window_pick_pos_first_) + height / 2;
			
			if (width == 0)	width = 1;
			if (height == 0) height = 1;
			
			// clear all previously used names
			GL_primitive_manager_.clearNames();
			
			// calculate picking matrix
			gluPickMatrix((double)center_x, 
										(double)(viewport[3] - center_y),
										(double)width,
										(double)height,
										viewport);
			
			// prepare camera
			glFrustum(-2.0 * x_scale_,
								2.0 * x_scale_,
								-2.0 * y_scale_,
								2.0 * y_scale_,
								1.5,
								300);
			
			gluLookAt((GLfloat)(getViewPointPosition().x), 
								(GLfloat)(getViewPointPosition().y),
								(GLfloat)(getViewPointPosition().z),
								(GLfloat)(getLookAtPosition().x), 
								(GLfloat)(getLookAtPosition().y),
								(GLfloat)(getLookAtPosition().z),
								(GLfloat)(getLookUpVector_().x),
								(GLfloat)(getLookUpVector_().y),
								(GLfloat)(getLookUpVector_().z));
			
			glMatrixMode(GL_MODELVIEW);
			
			// last action: not picking
			//			if (!was_picking_)
			{
				// render with names
				renderView_(true);
			}
			
			glPopMatrix();
			
			glFlush();
			
			// get number of hits
			number_of_hits = glRenderMode(GL_RENDER);
			
			// return if no objects are picked
			if (number_of_hits == 0)
			{
				setCamera_();
				
				return;
			}
			
			unsigned int minimum_z_coord = UINT_MAX;
			unsigned int names;
			GLPrimitiveManager::Name nearest_name = 0;
			unsigned int *object_buffer_ptr = (unsigned int *)object_buffer;

			GeometricObjectSelectionMessage* message = new GeometricObjectSelectionMessage;
			message->setDeletable(true);

			// collect only the nearest Object
			if (width <= 3
					&& height <= 3) 
			{
				unsigned int z_coord;
				
				// find minimum z-coord
				for (int index = 0; index < number_of_hits; ++index)
				{
					names = *object_buffer_ptr;

					++object_buffer_ptr;
					
					z_coord = *object_buffer_ptr;
					
					++object_buffer_ptr;
					++object_buffer_ptr;
					
					if (z_coord <= minimum_z_coord)
					{
						minimum_z_coord = z_coord;
						
						nearest_name = (GLPrimitiveManager::Name)*object_buffer_ptr;
					}
					
					object_buffer_ptr += names;
				}    
				
				GeometricObject* geometric_object = GL_primitive_manager_.getObject(nearest_name);

				if (select)
				{
					geometric_object->select();
				}
				else
				{
					geometric_object->deselect();
				}
			}
			else // collect all objects that are in the picking area
			{
				for (int index = 0; index < number_of_hits; ++index)
				{
					names = *object_buffer_ptr;
					
					++object_buffer_ptr;
					++object_buffer_ptr;
					++object_buffer_ptr;
					
					nearest_name = (GLPrimitiveManager::Name)*object_buffer_ptr;
					
					object_buffer_ptr += names;

					GeometricObject* geometric_object = GL_primitive_manager_.getObject(nearest_name);
					
					if (select)
					{
						geometric_object->select();
					}
					else
					{
						geometric_object->deselect();
					}
				}    
			}

			// collect selected objects
			GeometricCollector collector;
			collector.collectSelectedGeometricObjects(true);

			MainControl::DescriptorIterator descriptor_iterator;

			ConnectionObject *object = getParent();
			
			if (object == 0)
			{
				throw ::BALL::VIEW::Scene::MainControlMissing(__FILE__, __LINE__, "");
			}
			
			MainControl *main_control = RTTI::castTo<MainControl>(*object);

			// collect selected objects
			for (descriptor_iterator = main_control->getDescriptorList().begin();
					 descriptor_iterator != main_control->getDescriptorList().end(); 
					 ++descriptor_iterator)
			{
				CompositeDescriptor *composite_descriptor
					= *descriptor_iterator;

				composite_descriptor->getComposite()->apply(collector);

				// mark composite for update
				ChangedCompositeMessage change_message;
				change_message.setComposite(composite_descriptor->getComposite());
				notify_(change_message);
			}			

			message->setSelection(collector.getCollection());

			// sent collected objects
			notify_(message);

			// reset camera
			setCamera_();

			// last action: picking
			was_picking_ = true;
    }


		void Scene::convertMatrix_(const Matrix4x4 &matrix, GLfloat GL_float_array[4][4])
		{
			int i, j;

			for (i = 0; i < 4; ++i)
			{
				for (j = 0; j < 4; ++j)
				{
					GL_float_array[i][j] = (GLfloat)matrix(i, j);
				}
			} 
		}


		// Camera section -------

		void Scene::initViewVectors_()
		{
			zoom_ = look_at_position_ - position_;

			if (look_at_position_.x == position_.x
					&& look_at_position_.z == position_.z)
			{
				if (zoom_.y >= 0.0)
				{
					look_up_.set(0.0, 0.0, 1.0);
				}
				else
				{
					look_up_.set(0.0, 0.0, -1.0);
				}
			}
			else 
			{
				look_up_.set(0.0, 1.0, 0.0);
			}

			right_ = zoom_ % look_up_;
			right_.normalize();

			up_ = zoom_ % right_;
			up_.normalize();

			zoom_.normalize();
		}

		void Scene::setCamera_(bool set_origin)
		{
			if (set_origin == true)
			{
				system_origin_ = getLookAtPosition();
			}

			if (QGLWidget::isValid())
			{
				makeCurrent();

				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				
				glFrustum(-2.0 * x_scale_, 2.0 * x_scale_, -2.0 * y_scale_,
 									 2.0 * y_scale_, 1.5, 300);

				gluLookAt((GLfloat)(getViewPointPosition().x), 
									(GLfloat)(getViewPointPosition().y),
									(GLfloat)(getViewPointPosition().z),
									(GLfloat)(getLookAtPosition().x), 
									(GLfloat)(getLookAtPosition().y),
									(GLfloat)(getLookAtPosition().z),
									(GLfloat)(getLookUpVector_().x),
									(GLfloat)(getLookUpVector_().y),
									(GLfloat)(getLookUpVector_().z));

				glMatrixMode(GL_MODELVIEW);
			}
		}

		bool Scene::setCameraPosition_(const Vector3& look_at, const Vector3& view_point, bool set_origin)
		{
			position_ = view_point;
			look_at_position_ = look_at;
			last_look_at_position_ = look_at;

			initViewVectors_();
			setCamera_(set_origin);

			return true;  
		}

		void Scene::initializeWidget(MainControl& main_control)
			throw()
		{
			(main_control.initPopupMenu(MainControl::DISPLAY))->setCheckable(true);

			rotate_id_ 
				= main_control.insertMenuEntry
				   (MainControl::DISPLAY, "&Rotate Mode", this, SLOT(rotateMode_()), CTRL+Key_R);
			
			picking_id_ 
				= main_control.insertMenuEntry
				   (MainControl::DISPLAY, "&Picking Mode", this, SLOT(pickingMode_()), CTRL+Key_P);		
		}

		void Scene::finalizeWidget(MainControl& main_control)
			throw()
		{
			main_control.removeMenuEntry
				(MainControl::DISPLAY, "&Rotate Mode", this, SLOT(rotateMode_()), CTRL+Key_R);
			
			main_control.removeMenuEntry
				(MainControl::DISPLAY, "&Picking Mode", this, SLOT(pickingMode_()), CTRL+Key_P);		
		}

		void Scene::checkMenu(MainControl& main_control)
			throw()
		{
			(main_control.menuBar())->setItemChecked(rotate_id_, (rotate_mode_ == true));
			(main_control.menuBar())->setItemChecked(picking_id_, (rotate_mode_ == false));		
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/WIDGETS/scene.iC>
#		endif

	} // namespace VIEW

} // namespace BALL
