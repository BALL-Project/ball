// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: scene.C,v 1.152 2004/11/09 15:55:49 amoll Exp $
//

#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/stage.h>

#include <BALL/VIEW/DIALOGS/setCamera.h>
#include <BALL/VIEW/DIALOGS/preferences.h>
#include <BALL/VIEW/DIALOGS/lightSettings.h>
#include <BALL/VIEW/DIALOGS/stageSettings.h>
#include <BALL/VIEW/DIALOGS/materialSettings.h>

#include <BALL/VIEW/PRIMITIVES/simpleBox.h>
#include <BALL/VIEW/PRIMITIVES/label.h>
#include <BALL/VIEW/RENDERING/POVRenderer.h>

#include <BALL/VIEW/PRIMITIVES/sphere.h>
#include <BALL/VIEW/PRIMITIVES/tube.h>
#include <BALL/VIEW/PRIMITIVES/box.h>

#include <BALL/SYSTEM/timer.h>

#include <qpainter.h>
#include <qmenubar.h>
#include <qimage.h>
#include <qmenubar.h>
#include <qcursor.h>
#include <qapp.h>
#include <qdragobject.h>
#include <qdir.h>

// #define BALL_BENCHMARKING

using std::endl;
using std::istream;

namespace BALL
{
	namespace VIEW
	{

		// ###############CONSTRUCTORS,DESTRUCTORS,CLEAR###################

		// values for mouse-sensitivity 
		float Scene::mouse_sensitivity_ = 5;
		float Scene::mouse_wheel_sensitivity_ = 5;
		bool  Scene::show_light_sources_ = false;
		float Scene::animation_smoothness_ = 2;
		#define  ZOOM_FACTOR 			55
		#define  ROTATE_FACTOR    22
		#define  TRANSLATE_FACTOR 10

	  QGLFormat Scene::gl_format_(QGL::DepthBuffer | QGL::StereoBuffers | QGL::DoubleBuffer);

		Scene::Scene()
			throw()
			:	QGLWidget(gl_format_),
				ModularWidget("<Scene>"),
				current_mode_(ROTATE__MODE),
				last_mode_(PICKING__MODE),
				rotate_id_(-1),
				picking_id_(-1),
				system_origin_(0.0),
				quaternion_(),
				need_update_(false),
				update_running_(false),
				x_window_pos_old_(0),
				y_window_pos_old_(0),
				x_window_pos_new_(0),
				y_window_pos_new_(0),
				x_window_pick_pos_first_(0),
				y_window_pick_pos_first_(0),
				x_window_pick_pos_second_(0),
				y_window_pick_pos_second_(0),
				stage_(new Stage),
				light_settings_(0),
				stage_settings_(0),
				screenshot_nr_(10000),
				pov_nr_(10000),
				animation_thread_(0),
				stop_animation_(false)
		{
			gl_renderer_.setSize(600, 600);
			setAcceptDrops(true);
#ifdef BALL_VIEW_DEBUG
			Log.error() << "new Scene (1) " << this << std::endl;
#endif
		}

		Scene::Scene(QWidget* parent_widget, const char* name, WFlags w_flags)
			throw()
			:	QGLWidget(gl_format_, parent_widget, name, 0, w_flags),
				ModularWidget(name),
				current_mode_(ROTATE__MODE),
				last_mode_(PICKING__MODE),
				rotate_id_(-1),
				picking_id_(-1),
				system_origin_(0.0, 0.0, 0.0),
				quaternion_(),
				need_update_(false),
				update_running_(false),
				x_window_pos_old_(0),
				y_window_pos_old_(0),
				x_window_pos_new_(0),
				y_window_pos_new_(0),
				x_window_pick_pos_first_(0),
				y_window_pick_pos_first_(0),
				x_window_pick_pos_second_(0),
				y_window_pick_pos_second_(0),
				stage_(new Stage),
				light_settings_(0),
				stage_settings_(0),
				screenshot_nr_(10000),
				pov_nr_(10000),
				animation_thread_(0),
				stop_animation_(false)
		{
#ifdef BALL_VIEW_DEBUG
			Log.error() << "new Scene (2) " << this << std::endl;
#endif
			// the widget with the MainControl
			registerWidget(this);
			gl_renderer_.setSize(600, 600);
			setAcceptDrops(true);

			if (!QGLWidget::isValid())
			{
				Log.error() << "QGLWidget is not valid in Scene!" << std::endl;
			}
		}

		Scene::Scene(const Scene& scene, QWidget* parent_widget, const char* name, WFlags w_flags)
			throw()
			:	QGLWidget(gl_format_, parent_widget, name, 0, w_flags),
				ModularWidget(scene),
				system_origin_(scene.system_origin_),
				quaternion_(scene.quaternion_),
				x_window_pos_old_(0),
				y_window_pos_old_(0),
				x_window_pos_new_(0),
				y_window_pos_new_(0),
				x_window_pick_pos_first_(0),
				y_window_pick_pos_first_(0),
				x_window_pick_pos_second_(0),
				y_window_pick_pos_second_(0),
				stage_(new Stage(*scene.stage_)),
				light_settings_(new LightSettings(this)),
				stage_settings_(new StageSettings(this)),
				material_settings_(new MaterialSettings(this)),
				screenshot_nr_(10000),
				pov_nr_(10000),
				animation_thread_(0),
				stop_animation_(false)
		{
#ifdef BALL_VIEW_DEBUG
			Log.error() << "new Scene (3) " << this << std::endl;
#endif

			resize((Size) scene.gl_renderer_.getWidth(), (Size) scene.gl_renderer_.getHeight());

			// the widget with the MainControl
			ModularWidget::registerWidget(this);
			setAcceptDrops(true);
		}

		Scene::~Scene()
			throw()
			{
#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object Scene " << this << " of class Scene" << std::endl;
#endif 

				delete stage_;

#ifdef BALL_QT_HAS_THREADS
				if (animation_thread_ != 0) delete animation_thread_;
#endif
			}

		void Scene::clear()
			throw()
		{
			system_origin_.set(0.0, 0.0, 0.0);
			quaternion_.setIdentity();

			stage_->clear();
			animation_points_.clear();
		}

		void Scene::set(const Scene& scene)
			throw()
		{
			stage_ = scene.stage_;
			system_origin_.set(scene.system_origin_);
			quaternion_.set(scene.quaternion_);
		}

		const Scene& Scene::operator = (const Scene& scene)
			throw()
		{
			set(scene);
			return *this;
		}

		bool Scene::isValid() const
			throw()
		{
			return (getParent() != 0);
		}

		void Scene::dump(std::ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);

			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			stage_->dump(s, depth);

			quaternion_.dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);     
		}

		// ####################GL, CAMERA############################################

		void Scene::update(bool rebuild_displaylists)
			throw()
		{
			if (update_running_) return;
			update_running_ = true;
			if (rebuild_displaylists)
			{
				renderView_(REBUILD_DISPLAY_LISTS);
			}

			update_running_ = false;
			updateGL();
		}


		void Scene::onNotify(Message *message)
			throw()
		{
#ifdef BALL_VIEW_DEBUG
			Log.error() << "Scene " << this  << "onNotify " << message << std::endl;
#endif
			if (RTTI::isKindOf<RepresentationMessage>(*message)) 
			{
				RepresentationMessage* rm = RTTI::castTo<RepresentationMessage>(*message);
				Representation* rep = rm->getRepresentation();
				switch (rm->getType())
				{
					case RepresentationMessage::UPDATE:
					case RepresentationMessage::ADD:
						gl_renderer_.rebuildDisplayListFor(*rep);
						break;

					case RepresentationMessage::REMOVE:
						gl_renderer_.removeDisplayListFor(*rep);
						break;

					default:
						break;
				}

				update(false);
				return;
			}

			// react now only to SceneMessage
			if (!RTTI::isKindOf<SceneMessage>(*message)) return;

			SceneMessage *scene_message = RTTI::castTo<SceneMessage>(*message);

			switch (scene_message->getType())
			{
				case SceneMessage::REDRAW:
					update(false);
					return;

				case SceneMessage::REBUILD_DISPLAY_LISTS:
					update(true);
					return;

				case SceneMessage::UPDATE_CAMERA:
					stage_->moveCameraTo(scene_message->getStage().getCamera());
					system_origin_ = scene_message->getStage().getCamera().getLookAtPosition();
					updateCamera_();
					return;

				case SceneMessage::REMOVE_COORDINATE_SYSTEM:
					stage_->showCoordinateSystem(false);
					stage_settings_->updateFromStage();
					return;

				case SceneMessage::EXPORT_PNG:
					exportPNG();
					return;

				case SceneMessage::EXPORT_POVRAY:
					exportPOVRay();
					return;

				case SceneMessage::ENTER_ROTATE_MODE:
					rotateMode_();
					return;

				case SceneMessage::ENTER_PICKING_MODE:
					pickingMode_();
					return;

				case SceneMessage::ENTER_MOVE_MODE:
					moveMode_();
					return;

				case SceneMessage::UNDEFINED:
					Log.error() << "Unknown type of SceneMessage in " << __FILE__ << __LINE__ << std::endl;
			}
		}


		void Scene::initializeGL()
		{
			QGLWidget::initializeGL();
			if (!format().rgba())  Log.error() << "no rgba mode for OpenGL available." << endl;

			gl_renderer_.init(*stage_, (float) width(), (float) height());
			gl_renderer_.initSolid();
			if (stage_->getLightSources().size() == 0) setDefaultLighting(false);
			gl_renderer_.updateCamera();
		}

		void Scene::paintGL()
		{
			// cannot call update here, because it calls updateGL
			renderView_(DISPLAY_LISTS_RENDERING);
		}

		void Scene::resizeGL(int width, int height)
		{
			gl_renderer_.setSize(width, height);
			gl_renderer_.updateCamera();
			updateGL();
		}


		void Scene::renderView_(RenderMode mode)
			throw()
		{
			makeCurrent();

			glDepthMask(GL_TRUE);
			glDrawBuffer(GL_BACK_LEFT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			if (gl_renderer_.getStereoMode() == GLRenderer::NO_STEREO)
			{
				renderRepresentations_(mode);
				return;
			}

			// ok, this is going the stereo way...
			stereo_camera_ = stage_->getCamera();

			Vector3	diff = stage_->getCamera().getRightVector();
			Vector3 old_view_point = stage_->getCamera().getViewPoint();
			Vector3 old_look_at = stage_->getCamera().getLookAtPosition();
			diff.normalize();
			diff = diff * (stage_->getEyeDistance() / 2.0);  

			float nearf = 1.5; 
			float farf = 300;

			float ndfl    = nearf / stage_->getFocalDistance();

      float left  = -2.0 *gl_renderer_.getXScale() - 0.5 * stage_->getEyeDistance() * ndfl;
      float right =  2.0 *gl_renderer_.getXScale() - 0.5 * stage_->getEyeDistance() * ndfl;

			//================== draw first buffer =============
	    glMatrixMode(GL_PROJECTION);
			if (gl_renderer_.getStereoMode() == GLRenderer::DUAL_VIEW_STEREO)
			{
				gl_renderer_.setSize(width()/2, height());
				glLoadIdentity();
				glFrustum(left,right,
								-2.0 * gl_renderer_.getXScale() / 2, 
								 2.0 * gl_renderer_.getYScale(),
								nearf,farf);
				glViewport(0, 0, width() / 2, height());

				if (stage_->swapSideBySideStereo())
				{
					diff *= -1;
				}
		  }
			else
			{
				glLoadIdentity();
				glFrustum(left,right,
									-2.0 * gl_renderer_.getXScale(), 
									 2.0 * gl_renderer_.getYScale(),
									nearf,farf);
			}

			// draw models
      glMatrixMode(GL_MODELVIEW);
			if (gl_renderer_.getStereoMode() == GLRenderer::ACTIVE_STEREO)
			{
				glDrawBuffer(GL_BACK_RIGHT);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			}

			glPushMatrix();
			stereo_camera_.setViewPoint(old_view_point + diff);
			stereo_camera_.setLookAtPosition(old_look_at + diff);
			gl_renderer_.updateCamera(&stereo_camera_);
 			gl_renderer_.setLights(true);
			renderRepresentations_(mode);
			glPopMatrix();

			//================== draw second buffer =============
	    glMatrixMode(GL_PROJECTION);
      left  = -2.0 *gl_renderer_.getXScale() + 0.5 * stage_->getEyeDistance() * ndfl;
      right =  2.0 *gl_renderer_.getXScale() + 0.5 * stage_->getEyeDistance() * ndfl;

			if (gl_renderer_.getStereoMode() == GLRenderer::DUAL_VIEW_STEREO)
			{
				gl_renderer_.setSize(width()/2, height());
				glLoadIdentity();
				glFrustum(left,right,
								-2.0 * gl_renderer_.getXScale() / 2, 
								 2.0 * gl_renderer_.getYScale(),
								nearf,farf);
				glViewport(width() / 2, 0, width() / 2, height());
		  }
			else
			{
				glLoadIdentity();
				glFrustum(left,right,
									-2.0 * gl_renderer_.getXScale(), 
									 2.0 * gl_renderer_.getYScale(),
									nearf,farf);
			}


      glMatrixMode(GL_MODELVIEW);

			if (gl_renderer_.getStereoMode() == GLRenderer::ACTIVE_STEREO)
			{
				glDrawBuffer(GL_BACK_LEFT);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			}

			glPushMatrix();
			stereo_camera_.setViewPoint(old_view_point - diff);
			stereo_camera_.setLookAtPosition(old_look_at - diff);
			gl_renderer_.updateCamera(&stereo_camera_);
 			gl_renderer_.setLights(true);
 			light_settings_->updateFromStage();
			renderRepresentations_(DISPLAY_LISTS_RENDERING);
			glPopMatrix();
		}


		void Scene::renderClippingPlane_(const Representation& rep)
			throw()
		{
			Vector3 n(rep.getProperty("AX").getDouble(),
								rep.getProperty("BY").getDouble(),
								rep.getProperty("CZ").getDouble());

			if (n.getSquareLength() != 0) n.normalize();

			float d = rep.getProperty("D").getDouble();

			if (rep.isHidden())
			{
				Vector3 x(1,0,0);
				Vector3 y(0,1,0);
				Vector3 z(0,0,1);

				Vector3 v[3];
				v[0] = x - n.x * n;
				v[1] = y - n.y * n;
				v[2] = z - n.z * n;

				Vector3 e[2];
				Position i = 0;

				for (Position j = 0; j < 3 && i < 2; j++)
				{
					if (v[j].getSquareLength() != 0) 
					{
						e[i] = v[j];
						e[i].normalize();
						i++;
					}
				}
			
				n *= -d;

				Box b(n - (e[0] * 1500.0  + e[1] * 1500.0), e[0] * 3000.0, e[1] * 3000.0, 0.01);
				b.setColor(0,0,255, 255);
				gl_renderer_.render_(&b);

				return;
			}

			GLdouble plane[] ={n.x, n.y, n.z, d};

			glEnable(current_clipping_plane_);
			glClipPlane(current_clipping_plane_, plane);

			current_clipping_plane_++;
		}

		void Scene::renderRepresentations_(RenderMode mode)
			throw()
		{
#ifdef BALL_BENCHMARKING
	Timer t;
	t.start();
#endif
			PrimitiveManager::RepresentationList::ConstIterator it;
			// ============== render Clipping planes ==============================
			it = getMainControl()->getPrimitiveManager().getRepresentations().begin();

			current_clipping_plane_ = GL_CLIP_PLANE0;

			for(; it != getMainControl()->getPrimitiveManager().end(); it++)
			{
				if ((**it).getModelType() != MODEL_CLIPPING_PLANE) continue;
				renderClippingPlane_(**it);
			}
		
			for (GLint i = current_clipping_plane_; i <= GL_CLIP_PLANE0 + GL_MAX_CLIP_PLANES; i++)
			{
				glDisable(i);
			}

			// -------------------------------------------------------------------
			// show light sources
			if (show_light_sources_)
			{
				gl_renderer_.initSolid();
				List<LightSource>::ConstIterator lit = stage_->getLightSources().begin();
				for (; lit != stage_->getLightSources().end(); lit++)
				{
					Sphere s;
					s.setPosition((*lit).getPosition());
					s.setRadius(5);
					s.setColor(ColorRGBA(255,255,255));
					gl_renderer_.renderSphere_(s);

					Tube t;
					t.setVertex1((*lit).getPosition());
					t.setVertex2((*lit).getDirection());
					t.setColor(ColorRGBA(255,255,255));
					gl_renderer_.renderTube_(t);
				}
			}
			// -------------------------------------------------------------------
			
			// render all "normal" (non always front and non transparent models)
			it = getMainControl()->getPrimitiveManager().getRepresentations().begin();
			for(; it != getMainControl()->getPrimitiveManager().getRepresentations().end(); it++)
			{
				if ((*it)->getTransparency() == 0 &&
						!(*it)->hasProperty(Representation::PROPERTY__ALWAYS_FRONT))
				{
					gl_renderer_.initSolid();
					render_(**it, mode);
				}
			}

			// render all transparent models
			it = getMainControl()->getPrimitiveManager().getRepresentations().begin();
			for(; it != getMainControl()->getPrimitiveManager().getRepresentations().end(); it++)
			{
				if ((*it)->getTransparency() != 0)
				{
					gl_renderer_.initTransparent();
					render_(**it, mode);
				}
			}

			// render all always front models
			it = getMainControl()->getPrimitiveManager().getRepresentations().begin();
			for(; it != getMainControl()->getPrimitiveManager().getRepresentations().end(); it++)
			{
				if ((*it)->hasProperty(Representation::PROPERTY__ALWAYS_FRONT))
				{
					gl_renderer_.initSolid();
					render_(**it, mode);
				}
			}


#ifdef BALL_BENCHMARKING
	Log.info() << "Rendering time: " << t.getCPUTime() << std::endl;
	t.stop();
#endif
		}


		void Scene::render_(const Representation& rep, RenderMode mode)
			throw()
		{
			switch (mode)
			{
				case DISPLAY_LISTS_RENDERING:
					gl_renderer_.drawFromDisplayList(rep);
					break;

				case REBUILD_DISPLAY_LISTS:
					gl_renderer_.rebuildDisplayListFor(rep);
					break;

				case DIRECT_RENDERING:
					gl_renderer_.render(rep);
					break;
			}
		}

		void Scene::rotateSystem2_(Scene* /*scene*/)
		{
			if (current_mode_ != ROTATE__MODE ||
					x_window_pos_old_ == x_window_pos_new_) return;

			float angle = (x_window_pos_new_ - x_window_pos_old_) * (mouse_sensitivity_ / (ROTATE_FACTOR * -30));
				
			Matrix4x4 m;
			Camera& camera = stage_->getCamera();
			m.setRotation(Angle(angle), camera.getViewVector());
			camera.setLookUpVector(m * camera.getLookUpVector());
			updateCamera_();
		}


		void Scene::rotateSystem_(Scene* scene)
		{
			scene->calculateQuaternion_(quaternion_);

			// ??? We have to put that into the stage_'s Camera somehow...
			stage_->translate((float)(-1.)*system_origin_);
			stage_->rotate(quaternion_);
			stage_->translate(system_origin_);
			updateCamera_();
		}


		void Scene::translateSystem_(Scene* scene)
		{
			Camera& camera = stage_->getCamera();
			Camera& camera_scene = scene->stage_->getCamera();

			// Differences between the old and new x and y positions in the window
			float delta_x = scene->x_window_pos_new_ - scene->x_window_pos_old_;
			float delta_y = scene->y_window_pos_new_ - scene->y_window_pos_old_;

			// calculate translation in x-axis direction
			Vector3 right_translate = camera_scene.getRightVector()
				* (delta_x / scene->gl_renderer_.getWidth()) 
				* 1.4 * camera.getDistance()   // take distance from this scene
				* 2.0 * scene->gl_renderer_.getXScale()
				* mouse_sensitivity_ / TRANSLATE_FACTOR;

			// calculate translation in y-axis direction
			Vector3 up_translate 		= camera_scene.getLookUpVector() 
				* (delta_y / scene->gl_renderer_.getHeight()) 
				* 1.4 * camera.getDistance() // take distance from this scene
				* 2.0 * scene->gl_renderer_.getYScale()
				* mouse_sensitivity_ / TRANSLATE_FACTOR;

			Vector3 v(right_translate + up_translate);

			stage_->translate(v);
			updateCamera_();
		}


		void Scene::zoomSystem_(Scene *scene)
		{
			Camera& camera = stage_->getCamera();

			// Difference between the old and new y position in the window 
			float delta_y = scene->y_window_pos_new_ - scene->y_window_pos_old_;

			// stop if no movement in y-axis-direction
			if (delta_y == 0) return;

			Vector3 v((delta_y / 
						scene->gl_renderer_.getHeight() * camera.getDistance()) // take distance from this scene
					* scene->stage_->getCamera().getViewVector()
					* mouse_sensitivity_ / ZOOM_FACTOR);  

			stage_->translate(v);
			updateCamera_();
		}


		void Scene::calculateQuaternion_(Quaternion& q, const Quaternion* rotate)
		{
			if ((x_window_pos_old_ == x_window_pos_new_) &&
					(y_window_pos_old_ == y_window_pos_new_))
			{
				return;
			}

			Quaternion tmp;

			float right1 = (gl_renderer_.getWidth()  - x_window_pos_old_) / 
				gl_renderer_.getWidth() * mouse_sensitivity_ / -ROTATE_FACTOR;
			float up1 = 	 (gl_renderer_.getHeight() - y_window_pos_old_) / 
				gl_renderer_.getHeight() * mouse_sensitivity_ / -ROTATE_FACTOR;
			float right2 = (gl_renderer_.getWidth()  - x_window_pos_new_) / 
				gl_renderer_.getWidth() * mouse_sensitivity_ / -ROTATE_FACTOR;
			float up2 = 	 (gl_renderer_.getHeight() - y_window_pos_new_) / 
				gl_renderer_.getHeight() * mouse_sensitivity_ / -ROTATE_FACTOR;

			Camera& camera = stage_->getCamera();

			Vector3 a = camera.getRightVector()  * right1
				+ camera.getLookUpVector() * up1
				+ camera.getViewVector()   * sphereProject_(0.8, right1, up1);

			Vector3 b = camera.getRightVector()  * right2
				+ camera.getLookUpVector() * up2
				+ camera.getViewVector()   * sphereProject_(0.8, right2, up2);

			Vector3 cross = a % b;

			// calculate the rotate vector from the quaternion, if given
			// and apply it
			if (rotate != 0)
			{
				Matrix4x4 m;
				rotate->getRotationMatrix(m);
				Vector4 tmp_vector(cross.x, cross.y, cross.z);
				tmp_vector = m * tmp_vector;
				cross.set(tmp_vector.x, tmp_vector.y, tmp_vector.z);
			}

			float phi = (a - b).getLength();

			if (BALL_REAL_GREATER(phi, (float)1, Constants::EPSILON))
			{
				phi = (float)1;
			}

			if (BALL_REAL_LESS(phi, (float)-1, Constants::EPSILON))
			{
				phi = (float)-1;
			}

			phi = (float)(2.0 * asin((float)phi));

			tmp.set(cross, -phi);
			q = tmp;
		}


		float Scene::sphereProject_(float radius, float x, float y)
		{
			float dist = (float)sqrt((float)(x * x + y * y));
			float z;

			if (BALL_REAL_LESS(dist, radius * sqrt(2.0) / 2.0, Constants::EPSILON))
			{
				z = (float)sqrt((float)(radius * radius - dist * dist));
			}
			else
			{
				float t = radius / sqrt(2.0);
				z = t * t / dist;
			}

			return z;
		}

		// picking routine ------
		void Scene::selectObjects_(bool select)
		{
			List<GeometricObject*> objects;
			gl_renderer_.pickObjects1(x_window_pick_pos_first_,
					y_window_pick_pos_first_,
					x_window_pick_pos_second_,
					y_window_pick_pos_second_);

			// draw the representations
			renderView_(DIRECT_RENDERING);
			updateGL();

			int width  = BALL_ABS((int)x_window_pick_pos_second_ - (int)x_window_pick_pos_first_);
			int height = BALL_ABS((int)y_window_pick_pos_second_ - (int)y_window_pick_pos_first_);
			if (width == 0)	width = 1;
			if (height == 0) height = 1;
			gl_renderer_.pickObjects2(objects, width, height);
			glFlush();

			GeometricObjectSelectionMessage* message = new GeometricObjectSelectionMessage;
			message->setSelection(objects);
			message->setSelected(select);
			// sent collected objects
			notify_(message);
		}


		void Scene::setViewPoint_()
			throw()
		{
			SetCamera set_camera(this);
			set_camera.exec();
			gl_renderer_.updateCamera();
			updateGL();
		}


		void Scene::showViewPoint_()
			throw()
		{
			const Camera& camera = stage_->getCamera();

			String text("ViewPoint: (" 
					+ String(camera.getViewPoint().x) + "|" 
					+ String(camera.getViewPoint().y) + "|" 
					+ String(camera.getViewPoint().z) 
					+ ")   LookAt: (" 
					+ String(camera.getLookAtPosition().x) + "|" 
					+ String(camera.getLookAtPosition().y) + "|" 
					+ String(camera.getLookAtPosition().z) 
					+ ")   LookUp: (" 
					+ String(camera.getLookUpVector().x) + "|" 
					+ String(camera.getLookUpVector().y) + "|" 
					+ String(camera.getLookUpVector().z) + ")");

			setStatusbarText(text);
			Log.info() << text << std::endl;
		}


		void Scene::resetCamera_()
			throw()
		{
			// new instance for default values
			Camera camera;
			stage_->moveCameraTo(camera);
			updateCamera_();
		}

		
		void Scene::updateCamera_()
			throw()
		{
			if (gl_renderer_.getStereoMode() == GLRenderer::NO_STEREO)
			{
				gl_renderer_.updateCamera();
 				gl_renderer_.setLights();
 				light_settings_->updateFromStage();
			}

			updateGL();
		}


		// ##################MISC############################

		void Scene::setDefaultLighting(bool update_GL)
			throw()
		{
			LightSource light;
			light.setType(LightSource::POSITIONAL);
			light.setPosition(stage_->getCamera().getViewPoint() - 
												stage_->getCamera().getViewVector() * 4);
			light.setDirection(stage_->getCamera().getLookAtPosition());

			List<LightSource>& lights = stage_->getLightSources();
			lights.clear();
			lights.push_back(light);
			gl_renderer_.setLights(true);
			light_settings_->updateFromStage();

			if (update_GL) renderView_(REBUILD_DISPLAY_LISTS);
		}


		void Scene::createCoordinateSystem_()
			throw()
		{
			PrimitiveManager& pm = getMainControl()->getPrimitiveManager();

			Representation* rp = pm.createRepresentation();
			ColorRGBA red(255,0,0,120);
			ColorRGBA yellow(255,255,0,120);
			ColorRGBA color = red;

			for (Position p = 0; p <= 990; p+=10)
			{
				if (color == red) color = yellow;
				else 							color = red;

				SimpleBox* box = new SimpleBox;
				box->a.set(p,0,0);
				box->b.set((p+10),2,2);
				box->setColor(color);
				rp->insert(*box);

				box = new SimpleBox;
				box->a.set(0,p,0);
				box->b.set(2,p+10,2);
				box->setColor(color);
				rp->insert(*box);

				box = new SimpleBox;
				box->a.set(0,0,p);
				box->b.set(2,2,p+10);
				box->setColor(color);
				rp->insert(*box);
			}

			Label* labelx = new Label;
			labelx->setText("x-axis");
			labelx->setColor(ColorRGBA(0,0,1.0,0));
			labelx->setVertex(10,0,-2);
			rp->insert(*labelx);

			Label* labely = new Label;
			labely->setText("y-axis");
			labely->setColor(ColorRGBA(0,0,1.0,0));
			labely->setVertex(-2,10,0);
			rp->insert(*labely);


			Label* labelz = new Label;
			labelz->setText("z-axis");
			labelz->setColor(ColorRGBA(0,0,1.0,0));
			labelz->setVertex(0,-2,10);
			rp->insert(*labelz);

			rp->setProperty(Representation::PROPERTY__IS_COORDINATE_SYSTEM);

			// we have to add the representation in the GLRenderer manualy,
			// because the message wont arrive in Scene::onNotify
			gl_renderer_.rebuildDisplayListFor(*rp);

			// notify GeometricControl
			RepresentationMessage* message = new RepresentationMessage(*rp, RepresentationMessage::ADD);
			notify_(message);
		}


		bool Scene::exportScene(Renderer &er) const
			throw()
		{
			if (er.init(*stage_, (float) width(), (float) height()))
			{
				PrimitiveManager::RepresentationList::ConstIterator it;
				MainControl *main_control = MainControl::getMainControl(this);

				it = main_control->getPrimitiveManager().getRepresentations().begin();
				for (; it != main_control->getPrimitiveManager().getRepresentations().end(); it++)
				{
					if (!er.render(**it))
					{
						getMainControl()->setStatusbarText("Error rendering representation...");
						return false;
					}
				}

				if (er.finish())
				{
					// cant call Scene::setStatusbarText(..), no idea why!!!
					getMainControl()->setStatusbarText("Successfully exported Scene...");
					return true;
				}
			}

			getMainControl()->setStatusbarText("Error while exporting Scene...");
			return false;
		}

		//##########################PREFERENCES#################################

		void Scene::writePreferences(INIFile& inifile)
			throw()
		{
			ModularWidget::writePreferences(inifile);
			// write mouse sensitivity
			inifile.insertValue("WINDOWS", "Main::mouseSensitivity", String(mouse_sensitivity_));
			inifile.insertValue("WINDOWS", "Main::mouseWheelSensitivity", String(mouse_wheel_sensitivity_));
			inifile.appendSection("STAGE");

			String data = "(" + String((Index)stage_->getBackgroundColor().getRed()) +
				"," + String((Index)stage_->getBackgroundColor().getGreen()) +
				"," + String((Index)stage_->getBackgroundColor().getBlue()) +
				"," + String((Index)stage_->getBackgroundColor().getAlpha()) + ")";

			inifile.insertValue("STAGE", "EyeDistance", String(stage_->getEyeDistance()));
			inifile.insertValue("STAGE", "FocalDistance", String(stage_->getFocalDistance()));
			inifile.insertValue("STAGE", "FogIntensity", String(stage_->getFogIntensity()));
			inifile.insertValue("STAGE", "BackgroundColor", String(data));
			inifile.insertValue("STAGE", "ShowCoordinateSystem", String(stage_->coordinateSystemEnabled()));
			inifile.insertValue("STAGE", "Fulcrum", vector3ToString(system_origin_));
			inifile.insertValue("STAGE", "AnimationSmoothness", String(animation_smoothness_));
			writeLights_(inifile);
		}


		void Scene::fetchPreferences(INIFile& inifile)
			throw()
		{
			ModularWidget::fetchPreferences(inifile);
			if (inifile.hasEntry("WINDOWS", "Main::mouseSensitivity"))
			{
				mouse_sensitivity_= inifile.getValue("WINDOWS", "Main::mouseSensitivity").toFloat();
			}
			if (inifile.hasEntry("WINDOWS", "Main::mouseWheelSensitivity"))
			{
				mouse_wheel_sensitivity_= inifile.getValue("WINDOWS", "Main::mouseWheelSensitivity").toFloat();
			}

			if (inifile.hasEntry("STAGE", "EyeDistance"))
			{
				stage_->setEyeDistance(inifile.getValue("STAGE", "EyeDistance").toFloat());
			}

			if (inifile.hasEntry("STAGE", "FocalDistance"))
			{
				stage_->setFocalDistance(inifile.getValue("STAGE", "FocalDistance").toFloat());
			}

			if (inifile.hasEntry("STAGE", "FogIntensity"))
			{
				stage_->setFogIntensity(inifile.getValue("STAGE", "FogIntensity").toFloat());
			}

			if (inifile.hasEntry("STAGE", "Fulcrum"))
			{
				stringToVector3(inifile.getValue("STAGE", "Fulcrum"), system_origin_);
			}

			if (inifile.hasEntry("STAGE", "AnimationSmoothness"))
			{
				setAnimationSmoothness(inifile.getValue("STAGE", "AnimationSmoothness").toFloat());
			}


			if (inifile.hasEntry("STAGE", "BackgroundColor"))
			{
				String data = inifile.getValue("STAGE", "BackgroundColor");
				vector<String> strings;
				data.split(strings, "(,)");
				ColorRGBA color(strings[0].toUnsignedInt(),
						strings[1].toUnsignedInt(),
						strings[2].toUnsignedInt(),
						strings[3].toUnsignedInt());
				stage_->setBackgroundColor(color);
			}

			if (inifile.hasEntry("WINDOWS", getIdentifier() + "::on") &&
					!inifile.getValue("WINDOWS", getIdentifier() + "::on").toUnsignedInt())
			{
				switchShowWidget();
			}

			readLights_(inifile);
			light_settings_->updateFromStage();
			stage_settings_->updateFromStage();

			applyPreferences();
		}


		void Scene::initializePreferencesTab(Preferences &preferences)
			throw()
		{
			light_settings_ = new LightSettings(this);
			preferences.insertEntry(light_settings_);
			stage_settings_= new StageSettings(this);
			preferences.insertEntry(stage_settings_);
			material_settings_= new MaterialSettings(this);
			preferences.insertEntry(material_settings_);
		}


		void Scene::finalizePreferencesTab(Preferences &preferences)
			throw()
		{
			if (light_settings_) 
			{
				preferences.removeEntry(light_settings_);
				light_settings_ = 0;
			}
			if (stage_settings_) 
			{
				preferences.removeEntry(stage_settings_);
				stage_settings_= 0;
			}
			if (material_settings_) 
			{
				preferences.removeEntry(material_settings_);
				material_settings_= 0;
			}
		}

		void Scene::applyPreferences()
			throw()
		{
			if (light_settings_ == 0) return;

			light_settings_->apply();
			gl_renderer_.setLights(true);

			bool showed_coordinate = stage_->coordinateSystemEnabled();
			stage_settings_->apply();
			PrimitiveManager& pm = getMainControl()->getPrimitiveManager();

			if (showed_coordinate && !stage_->coordinateSystemEnabled())
			{
				PrimitiveManager::RepresentationsIterator it = pm.begin();
				Representation* coordinate_rep = 0;
				for (; it != pm.end(); ++it)
				{
					if ((*it)->hasProperty(Representation::PROPERTY__IS_COORDINATE_SYSTEM))
					{
						coordinate_rep = *it;
					}
					else
					{
						(*it)->update(true);
					}
				}
				if (coordinate_rep != 0)
				{
					pm.remove(*coordinate_rep);
					RepresentationMessage* message = new RepresentationMessage(*coordinate_rep, RepresentationMessage::REMOVE);
					notify_(message);
				}
			}
			else if (!showed_coordinate && stage_->coordinateSystemEnabled()) 
			{
				createCoordinateSystem_();
			}

			material_settings_->apply();

			gl_renderer_.updateBackgroundColor(); 

			if (stage_->getFogIntensity() == 0)
			{
				glDisable(GL_FOG);
			}
			else
			{
				glEnable(GL_FOG);
				GLfloat color[4] = {0.0, 0.0, 0.0, 0.4};
				glFogfv(GL_FOG_COLOR, color);
				glFogf(GL_FOG_START, 2.0);
				glFogf(GL_FOG_END, 401 - stage_->getFogIntensity());
				glFogi(GL_FOG_MODE, GL_LINEAR);
			}

			renderView_(REBUILD_DISPLAY_LISTS);
			updateGL();
		}


		void Scene::cancelPreferences()
			throw()
		{
			if (light_settings_ != 0)
			{
				light_settings_->updateFromStage();
			}

			if (stage_settings_ != 0)
			{
				stage_settings_->updateFromStage();
			}
		}


		void Scene::writeLights_(INIFile& inifile) const
			throw()
		{
			String data;
			inifile.appendSection("LIGHTING");

			Position nr = 0;
			List<LightSource>::ConstIterator it = stage_->getLightSources().begin();
			for (; it != stage_->getLightSources().end(); it++)
			{
				const LightSource& light = *it;

				data = "(" + String(light.getPosition().x) +
					"," + String(light.getPosition().y) +
					"," + String(light.getPosition().z) + ")";
				inifile.insertValue("LIGHTING", "Light_" + String(nr) + "_Position", data);

				data = "(" + String(light.getDirection().x) +
					"," + String(light.getDirection().y) +
					"," + String(light.getDirection().z) + ")";
				inifile.insertValue("LIGHTING", "Light_" + String(nr) + "_Direction", data);

				data = String(light.getAngle().toRadian());
				inifile.insertValue("LIGHTING", "Light_" + String(nr) + "_Angle", data);

				data = String(light.getIntensity());
				inifile.insertValue("LIGHTING", "Light_" + String(nr) + "_Intensity", data);

				data = "(" + String((Index)light.getColor().getRed()) +
					"," + String((Index)light.getColor().getGreen()) +
					"," + String((Index)light.getColor().getBlue()) +
					"," + String((Index)light.getColor().getAlpha()) + ")";
				inifile.insertValue("LIGHTING", "Light_" + String(nr) + "_Color", data);

				data = String(light.getType());
				inifile.insertValue("LIGHTING", "Light_" + String(nr) + "_Type", data);

				data = String(light.isRelativeToCamera());
				inifile.insertValue("LIGHTING", "Light_" + String(nr) + "_Relative", data);

				nr++;
			}
		}


		void Scene::readLights_(const INIFile& inifile)
			throw()
			{
				stage_->getLightSources().clear();
				String data;
				vector<String> strings;
				try
				{
					Position nr = 0;
					while(inifile.hasEntry("LIGHTING", "Light_" + String(nr) + "_Position"))
					{
						LightSource light;

						data = inifile.getValue("LIGHTING", "Light_" + String(nr) + "_Position");
						data.split(strings, "(,)");
						light.setPosition(Vector3(strings[0].toFloat(),
									strings[1].toFloat(),
									strings[2].toFloat()));

						data = inifile.getValue("LIGHTING", "Light_" + String(nr) + "_Direction");
						data.split(strings, "(,)");
						light.setDirection(Vector3(strings[0].toFloat(),
									strings[1].toFloat(),
									strings[2].toFloat()));

						data = inifile.getValue("LIGHTING", "Light_" + String(nr) + "_Angle");
						light.setAngle(Angle(data.toFloat()));

						data = inifile.getValue("LIGHTING", "Light_" + String(nr) + "_Intensity");
						light.setIntensity(data.toFloat());

						data = inifile.getValue("LIGHTING", "Light_" + String(nr) + "_Color");
						data.split(strings, "(,)");
						ColorRGBA color(strings[0].toUnsignedInt(),
								strings[1].toUnsignedInt(),
								strings[2].toUnsignedInt(),
								strings[3].toUnsignedInt());
						light.setColor(color);

						data = inifile.getValue("LIGHTING", "Light_" + String(nr) + "_Type");
						light.setType((LightSource::Types)data.toUnsignedInt());

						data = inifile.getValue("LIGHTING", "Light_" + String(nr) + "_Relative");
						light.setRelativeToCamera(data.toUnsignedInt());
						stage_->addLightSource(light);

						nr++;
					}
				}
				catch(Exception::GeneralException e)
				{
					Log.error() << "Could not read lighting settings from Inifile" << std::endl;
					Log.error() << e;
				}

				if (stage_->getLightSources().size() == 0)
				{
					setDefaultLighting(true);
				}
			}

		// ###########################MENUES##################################

		void Scene::initializeWidget(MainControl& main_control)
			throw()
		{
			(main_control.initPopupMenu(MainControl::DISPLAY))->setCheckable(true);

			String hint;
			main_control.insertPopupMenuSeparator(MainControl::DISPLAY);
			hint = "Switch to rotate/zoom mode";
			rotate_id_ =	main_control.insertMenuEntry(
					MainControl::DISPLAY, "&Rotate Mode", this, SLOT(rotateMode_()), CTRL+Key_R, -1, hint);

			hint = "Switch to picking mode, e.g. to identify singe atoms or groups";
			picking_id_ = main_control.insertMenuEntry(
					MainControl::DISPLAY, "&Picking Mode", this, SLOT(pickingMode_()), CTRL+Key_P, -1, hint);

			hint = "Move selected items";
			move_id_ = main_control.insertMenuEntry(
					MainControl::DISPLAY, "Move Mode", this, SLOT(moveMode_()), 0, -1, hint);


			main_control.insertPopupMenuSeparator(MainControl::DISPLAY);

			no_stereo_id_ = main_control.insertMenuEntry (
 					MainControl::DISPLAY_STEREO, "No Stereo", this, SLOT(exitStereo()));
 			menuBar()->setItemChecked(no_stereo_id_, true) ;
			active_stereo_id_ = main_control.insertMenuEntry (
 					MainControl::DISPLAY_STEREO, "Shuttter Glasses", this, SLOT(enterActiveStereo()));
			dual_stereo_id_ = main_control.insertMenuEntry (
 					MainControl::DISPLAY_STEREO, "Side by Side", this, SLOT(enterDualStereo()));

			hint = "Print the coordinates of the current viewpoint";
			main_control.insertMenuEntry(
					MainControl::DISPLAY_VIEWPOINT, "Show Vie&wpoint", this, SLOT(showViewPoint_()), CTRL+Key_W, -1, hint);

			hint = "Move the viewpoint to the given coordinates";
			main_control.insertMenuEntry(
					MainControl::DISPLAY_VIEWPOINT, "Set Viewpoi&nt", this, SLOT(setViewPoint_()), CTRL+Key_N, -1, hint);

			hint = "Reset the camera to the orgin (0,0,0)";
			main_control.insertMenuEntry(
					MainControl::DISPLAY_VIEWPOINT, "Rese&t Camera", this, SLOT(resetCamera_()), CTRL+Key_T, -1, hint);

			hint = "Export a PNG image file from the Scene";
			main_control.insertMenuEntry(
					MainControl::FILE_EXPORT, "PNG", this, SLOT(exportPNG()), ALT+Key_P, -1, hint);

			window_menu_entry_id_ = 
				main_control.insertMenuEntry(MainControl::WINDOWS, "Scene", this, SLOT(switchShowWidget()));
			menuBar()->setItemChecked(window_menu_entry_id_, true);

			hint = "Add an OpenGL Clipping Plane to the Scene";
			main_control.insertMenuEntry(MainControl::DISPLAY, "New Clipping Plane", this, 
					SLOT(createNewClippingPlane()), 0, -1, hint);   

			// ======================== ANIMATION ===============================================
			hint = "Record an animation for later processing";
			record_animation_id_ = main_control.insertMenuEntry(MainControl::DISPLAY_ANIMATION, "Record", this, 
					SLOT(recordAnimationClicked()), 0, -1, hint);   
 			menuBar()->setItemChecked(record_animation_id_, false) ;
			
			clear_animation_id_ = main_control.insertMenuEntry(MainControl::DISPLAY_ANIMATION, "Clear", this, SLOT(clearRecordedAnimation()));
			main_control.insertPopupMenuSeparator(MainControl::DISPLAY_ANIMATION);
			start_animation_id_ = main_control.insertMenuEntry(MainControl::DISPLAY_ANIMATION, "Start", this, SLOT(startAnimation()));

			cancel_animation_id_ = main_control.insertMenuEntry(MainControl::DISPLAY_ANIMATION, "Stop", this, SLOT(stopAnimation()));
			menuBar()->setItemEnabled(cancel_animation_id_, false);

			main_control.insertPopupMenuSeparator(MainControl::DISPLAY_ANIMATION);
			animation_export_PNG_id_ = main_control.insertMenuEntry(MainControl::DISPLAY_ANIMATION, "Export PNG", this, SLOT(animationExportPNGClicked()));
			animation_export_POV_id_ = main_control.insertMenuEntry(MainControl::DISPLAY_ANIMATION, "Export POV", this, SLOT(animationExportPOVClicked()));
			animation_repeat_id_ = main_control.insertMenuEntry(MainControl::DISPLAY_ANIMATION, "Repeat", this, SLOT(animationRepeatClicked()));


			setCursor(QCursor(Qt::SizeAllCursor));
		}

		void Scene::finalizeWidget(MainControl& main_control)
			throw()
		{
			main_control.removeMenuEntry(MainControl::DISPLAY, "&Rotate Mode", this, SLOT(rotateMode_()), CTRL+Key_R);
			main_control.removeMenuEntry(MainControl::DISPLAY, "&Picking Mode", this, SLOT(pickingMode_()), CTRL+Key_P);		
			main_control.removeMenuEntry(MainControl::DISPLAY_VIEWPOINT, "Show Vie&wpoint", this, 
					SLOT(showViewPoint_()), CTRL+Key_W);		
			main_control.removeMenuEntry(MainControl::DISPLAY_VIEWPOINT, "Set Viewpoi&nt", this, 
					SLOT(setViewPoint_()), CTRL+Key_N);		
			main_control.removeMenuEntry(MainControl::DISPLAY_VIEWPOINT, "Rese&t Camera", this, 
					SLOT(resetCamera_()), CTRL+Key_T);		
			main_control.removeMenuEntry(MainControl::DISPLAY, "& Stereo Mode", this, SLOT( switchStereo()), ALT+Key_Y);		
			main_control.removeMenuEntry(MainControl::FILE_EXPORT, "PNG", this, SLOT(exportPNG()), ALT+Key_P);		
			main_control.removeMenuEntry(MainControl::WINDOWS, "Scene", this, SLOT(switchShowWidget()));

			main_control.removeMenuEntry(MainControl::DISPLAY, "New Clipping Plane", this, 
					SLOT(createNewClippingPlane()), 0);
		}

		void Scene::checkMenu(MainControl& /*main_control*/)
			throw()
		{
			menuBar()->setItemChecked(rotate_id_, 	(current_mode_ == ROTATE__MODE));
			menuBar()->setItemChecked(picking_id_,  (current_mode_ == PICKING__MODE));		

			bool animation_running = false;
			#ifdef BALL_QT_HAS_THREADS
				animation_running = (animation_thread_ != 0 && animation_thread_->running());
			#endif
			
			menuBar()->setItemEnabled(start_animation_id_, 
																animation_points_.size() > 0 && 
																!getMainControl()->compositesAreLocked() &&
																!animation_running);
			
			menuBar()->setItemEnabled(clear_animation_id_, animation_points_.size() > 0 && !animation_running);
		}

		//##########################EVENTS#################################

		void Scene::mouseMoveEvent(QMouseEvent* e)
		{
			makeCurrent();

			need_update_ = true;

			x_window_pos_new_ = e->x();
			y_window_pos_new_ = e->y();

			// ============ picking mode ================
			if (current_mode_ == PICKING__MODE)
			{
				if (e->state() == Qt::LeftButton  ||
						e->state() == Qt::RightButton ||
						e->state() == (Qt::LeftButton | Qt::ShiftButton))
				{
					selectionPressedMoved_();
				}
			}
			else if (current_mode_ == MOVE__MODE)
			{
				processMoveModeMouseEvents_(e);
			}
			else
			{
				processRotateModeMouseEvents_(e);
			}

			x_window_pos_old_ = x_window_pos_new_;
			y_window_pos_old_ = y_window_pos_new_;
		}


		void Scene::mousePressEvent(QMouseEvent* e)
		{
			makeCurrent();

			x_window_pos_old_ = e->x();
			y_window_pos_old_ = e->y();

			if(current_mode_ == ROTATE__MODE)
			{
				return;
			}

			if(current_mode_ == PICKING__MODE)
			{	
				if (e->button() == Qt::LeftButton ||
						e->button() == Qt::RightButton)
				{
					selectionPressed_();
				}
			}
		}


		void Scene::processRotateModeMouseEvents_(QMouseEvent* e)
		{
			if(current_mode_ != ROTATE__MODE) return;

			switch (e->state())
			{
				case (Qt::ShiftButton | Qt::LeftButton): 
				case  Qt::MidButton:
					zoomSystem_(this);
					break;

				case (Qt::ControlButton | Qt::LeftButton):
				case  Qt::RightButton:
					translateSystem_(this);
					break;

				case (Qt::LeftButton | Qt::RightButton):
				case (Qt::LeftButton | Qt::ShiftButton | Qt::ControlButton):
					rotateSystem2_(this);
					break;

				case Qt::LeftButton:
					rotateSystem_(this);
					break;

				default:
					break;
			}

			if (menuBar()->isItemChecked(record_animation_id_))
			{
				animation_points_.push_back(stage_->getCamera());
			}
		}

		void Scene::processMoveModeMouseEvents_(QMouseEvent* e)
		{
			if(current_mode_ != MOVE__MODE) return;

			Camera& camera = stage_->getCamera();

			// Difference between the old and new position in the window 
			float delta_x = x_window_pos_new_ - x_window_pos_old_;
			float delta_y = y_window_pos_new_ - y_window_pos_old_;

			// stop if no movement
			if (delta_x == 0 && delta_y == 0) return;

			TransformationMessage* msg = new TransformationMessage;
			Matrix4x4 m;

			switch (e->state())
			{
				// zoom
				case (Qt::ShiftButton | Qt::LeftButton): 
				case  Qt::MidButton:
				{
					Vector3 v((delta_y / 
								gl_renderer_.getHeight() * camera.getDistance()) 
							* -stage_->getCamera().getViewVector()
							* mouse_sensitivity_ / ZOOM_FACTOR);  
					m.setTranslation(v);
					break;
				}

				// =============== translate ========================
				case (Qt::ControlButton | Qt::LeftButton):
				case  Qt::RightButton:
				{
					// calculate translation in x-axis direction
					Vector3 right_translate = camera.getRightVector()
						* (delta_x / gl_renderer_.getWidth()) 
						* 1.4 * camera.getDistance()   
						* 2.0 * gl_renderer_.getXScale()
						* mouse_sensitivity_ / TRANSLATE_FACTOR;

					// calculate translation in y-axis direction
					Vector3 up_translate 		= camera.getLookUpVector() 
						* (delta_y / gl_renderer_.getHeight()) 
						* 1.4 * camera.getDistance() 
						* 2.0 * gl_renderer_.getYScale()
						* mouse_sensitivity_ / TRANSLATE_FACTOR;

					m.setTranslation(-right_translate - up_translate);
					break;
				}

				// rotate
				case Qt::LeftButton:
				{
					if (delta_x * delta_x > delta_y * delta_y)
					{
						Angle angle(delta_x * (mouse_sensitivity_ / (ROTATE_FACTOR * 3)), false);
						m.rotate(angle, camera.getLookUpVector());
					}
					else
					{
						Angle angle(delta_y * (mouse_sensitivity_ / (ROTATE_FACTOR * -3)), false);
						m.rotate(angle, camera.getRightVector());
					}
					break;
				}

				// ===================== rotate2 ===========================
				case (Qt::LeftButton | Qt::RightButton):
				case (Qt::LeftButton | Qt::ShiftButton | Qt::ControlButton):
				default:
					delete msg;
					return;
			}

			msg->setMatrix(m);
			notify_(msg);
		}


		void Scene::mouseReleaseEvent(QMouseEvent* e)
		{
			makeCurrent();

			// ============ picking mode ================
			if(current_mode_ == PICKING__MODE)
			{
				switch (e->state())
				{
					case (Qt::LeftButton | Qt::ShiftButton):
						deselectionReleased_();
						break;

					case Qt::LeftButton:
						selectionReleased_();
						break;

					case Qt::RightButton:
						deselectionReleased_();
						break;

					default:
						break;
				}
			}
			else if (current_mode_ == ROTATE__MODE)
			{
				processRotateModeMouseEvents_(e);
			}
			else if (current_mode_ == MOVE__MODE)
			{
				processMoveModeMouseEvents_(e);
			}

			if (need_update_)
			{
				updateGL();
				need_update_ = false;
			}
		}


#ifndef QT_NO_WHEELEVENT
		void Scene::wheelEvent(QWheelEvent *qmouse_event)
		{
			qmouse_event->accept();

			y_window_pos_new_ = y_window_pos_old_ + (qmouse_event->delta()/120*mouse_wheel_sensitivity_);
			zoomSystem_(this);
			y_window_pos_old_ = y_window_pos_new_;
		}
#endif

		void Scene::keyPressEvent(QKeyEvent* e)
		{
			if (gl_renderer_.getStereoMode() != GLRenderer::NO_STEREO)
			{
				if ((e->key() == Key_Y && e->state() == AltButton) ||
						 e->key() == Key_Escape)
				{
					exitStereo();
				}

				// setting of eye and focal distance
				if (e->key() != Key_Left  &&
						e->key() != Key_Right &&
						e->key() != Key_Up    &&
						e->key() != Key_Down)
				{
					return;
				}

				// setting of eye distance
				if (e->key() == Key_Left ||
						e->key() == Key_Right)
				{
					float new_distance = stage_->getEyeDistance();

					float modifier;
					if (e->key() == Key_Left)
					{
						modifier = -0.1;
					}
					else
					{
						modifier = +0.1;
					}

					if (e->state() == ShiftButton)
					{
						modifier *= 10;
					}

					new_distance += modifier;
					
					// prevent strange values
					if (new_distance < 0)
					{
						new_distance = 0;
					}

					if (new_distance > 4)
					{
						new_distance = 4;
					}

					stage_->setEyeDistance(new_distance);
				}
				// setting of focal distance
				else
				{
					float new_focal_distance = stage_->getFocalDistance();

					float modifier;
					if (e->key() == Key_Down)
					{
						modifier = -1;
					}
					else
					{
						modifier = +1;
					}

					if (e->state() == ShiftButton)
					{
						modifier *= 10;
					}

					new_focal_distance += modifier;
	
					// prevent strange values
					if (new_focal_distance < 7)
					{
						new_focal_distance = 7;
					}
					
					if (new_focal_distance > 100) 
					{
						new_focal_distance = 100;
					}

					stage_->setFocalDistance(new_focal_distance);
				}

				stage_settings_->updateFromStage();

				updateGL();
			}
		}

		void Scene::rotateMode_()
		{
			if (current_mode_ == ROTATE__MODE) return;
			
			last_mode_ = current_mode_;
			current_mode_ = ROTATE__MODE;		
			setCursor(QCursor(Qt::SizeAllCursor));
			menuBar()->setItemChecked(rotate_id_, true);
			menuBar()->setItemChecked(picking_id_, false);
			menuBar()->setItemChecked(move_id_, false);
		}

		void Scene::pickingMode_()
		{
			if (current_mode_ == PICKING__MODE) return;
			
			last_mode_ = current_mode_;
			current_mode_ = PICKING__MODE;
			setCursor(QCursor(Qt::CrossCursor));
			menuBar()->setItemChecked(rotate_id_, false);
			menuBar()->setItemChecked(picking_id_, true);
			menuBar()->setItemChecked(move_id_, false);
		}

		void Scene::moveMode_()
		{
			if (current_mode_ == MOVE__MODE) return;
			
			last_mode_ = current_mode_;
			current_mode_ = MOVE__MODE;
			setCursor(QCursor(Qt::PointingHandCursor));
			menuBar()->setItemChecked(rotate_id_, false);
			menuBar()->setItemChecked(picking_id_, false);
			menuBar()->setItemChecked(move_id_, true);
		}

		void Scene::selectionPressed_()
		{
			x_window_pick_pos_first_ = x_window_pos_old_;
			y_window_pick_pos_first_ = y_window_pos_old_;
			x_window_pick_pos_second_ = x_window_pos_old_;
			y_window_pick_pos_second_ = y_window_pos_old_;
		}

		void Scene::selectionReleased_()
		{
			selectObjects_(true);
			need_update_ = true;
			updateGL();
		}

		void Scene::deselectionReleased_()
		{
			selectObjects_(false);
			need_update_ = true;
			updateGL();
		}


		void Scene::selectionPressedMoved_()
		{
			x_window_pick_pos_second_ = x_window_pos_new_;
			y_window_pick_pos_second_ = y_window_pos_new_;

			QPainter painter(this);
			painter.setPen(white);
			painter.setRasterOp(XorROP);

			painter.drawRect((int) x_window_pick_pos_first_,
					(int) y_window_pick_pos_first_,
					(int) (x_window_pos_old_ - x_window_pick_pos_first_),
					(int) (y_window_pos_old_ - y_window_pick_pos_first_));

			painter.drawRect((int) x_window_pick_pos_first_,
					(int) y_window_pick_pos_first_,
					(int) (x_window_pos_new_ - x_window_pick_pos_first_),
					(int) (y_window_pos_new_ - y_window_pick_pos_first_));

			painter.end();
		}


		String Scene::exportPNG()
		{
			makeCurrent();
			QImage image = grabFrameBuffer();

			String filename = String("BALLView_screenshot" + String(screenshot_nr_) +".png");
			bool result = image.save(filename.c_str(), "PNG");
			screenshot_nr_ ++;

			if (result) setStatusbarText("Saved screenshot to " + filename);
			else 				setStatusbarText("Could not save screenshot to " + filename);

			return filename;
		}

		void Scene::exportPOVRay()
		{
			String filename = String("BALLView_pov_" + String(pov_nr_) +".pov");

			POVRenderer pr(filename);
			bool result = exportScene(pr);
			pov_nr_ ++;

			if (result) setStatusbarText("Saved POVRay to " + filename);
			else 				setStatusbarText("Could not save POVRay to " + filename);
		}

		void Scene::customEvent(QCustomEvent * e)
		{
			if (e->type() == (QEvent::Type)SCENE_EXPORTPNG_EVENT) 
			{  
				exportPNG();
				return;
			}

			if (e->type() == (QEvent::Type)SCENE_EXPORTPOV_EVENT) 
			{  
				exportPOVRay();
				return;
			}

			if (e->type() == (QEvent::Type)SCENE_SETCAMERA_EVENT) 
			{  
				setCamera(((SceneSetCameraEvent*) e)->camera);
				return;
			}
		}

		void Scene::switchShowWidget()
			throw()
		{
			if (window_menu_entry_id_ == -1) return;

			if (!getMainControl()) 
			{
				Log.error() << "Problem in " << __FILE__ << __LINE__ << std::endl;
				return;
			}

			QMenuBar* menu = getMainControl()->menuBar();
			if (menu->isItemChecked(window_menu_entry_id_))
			{
				hide();
				menu->setItemChecked(window_menu_entry_id_, false);
			}
			else
			{
				show();
				menu->setItemChecked(window_menu_entry_id_, true);
			}
		}


		void Scene::exitStereo()
			throw()
		{
			gl_renderer_.setStereoMode(GLRenderer::NO_STEREO);
			gl_renderer_.setSize(width(), height());

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glFrustum(-2.0 * gl_renderer_.getXScale(), 
								 2.0 * gl_renderer_.getXScale(), 
								-2.0 * gl_renderer_.getYScale(), 
								 2.0 * gl_renderer_.getYScale(),
								 1.5, 300);
			glMatrixMode(GL_MODELVIEW);

			hide();
			reparent((QWidget*)getMainControl(), getWFlags() & ~WType_Mask, last_pos_, false);
			((QMainWindow*)getMainControl())->setCentralWidget(this);
			show();

			getMainControl()->menuBar()->setItemChecked(no_stereo_id_, true);
			getMainControl()->menuBar()->setItemChecked(active_stereo_id_, false);
			getMainControl()->menuBar()->setItemChecked(dual_stereo_id_, false);
			update();
		}

		void Scene::enterActiveStereo()
			throw()
		{
			gl_renderer_.setStereoMode(GLRenderer::ACTIVE_STEREO);
			last_pos_ = pos();
			hide();
			showNormal();  // needed on windows
			reparent(NULL, Qt::WType_TopLevel, QPoint(0, 0));
			showFullScreen();
			show();

			getMainControl()->menuBar()->setItemChecked(no_stereo_id_, false);
			getMainControl()->menuBar()->setItemChecked(active_stereo_id_, true);
			getMainControl()->menuBar()->setItemChecked(dual_stereo_id_, false);
			update();
		}

		void Scene::enterDualStereo()
			throw()
		{
			gl_renderer_.setStereoMode(GLRenderer::DUAL_VIEW_STEREO);
			last_pos_ = pos();
			hide();
			showNormal();  // needed on windows
			reparent(NULL, Qt::WType_TopLevel, QPoint(0, 0));
			showFullScreen();
			setGeometry(qApp->desktop()->geometry());
			show();

			getMainControl()->menuBar()->setItemChecked(no_stereo_id_, false);
			getMainControl()->menuBar()->setItemChecked(active_stereo_id_, false);
			getMainControl()->menuBar()->setItemChecked(dual_stereo_id_, true);
			update();
		}

		void Scene::createNewClippingPlane()
		{
			Representation* rep = new Representation();
			rep->setModelType(MODEL_CLIPPING_PLANE);
			Vector3 v = stage_->getCamera().getViewVector();
			rep->setProperty("AX", v.x);
			rep->setProperty("BY", v.y);
			rep->setProperty("CZ", v.z);
			rep->setProperty("D", 10);
			rep->setHidden(true);
			getMainControl()->insert(*rep);
		}

		void Scene::setCamera(const Camera& camera)
			throw()
		{
			stage_->moveCameraTo(camera);
			updateCamera_();
		}

		void Scene::clearRecordedAnimation()
			throw()
		{
			animation_points_.clear();
		}

		void Scene::startAnimation()
			throw()
		{
			menuBar()->setItemEnabled(start_animation_id_, false);
			menuBar()->setItemEnabled(cancel_animation_id_, true);

			menuBar()->setItemEnabled(animation_repeat_id_, false);
			menuBar()->setItemEnabled(animation_export_POV_id_, false);
			menuBar()->setItemEnabled(animation_export_PNG_id_, false);
			menuBar()->setItemEnabled(clear_animation_id_, false);

			#ifdef BALL_QT_HAS_THREADS
				if (animation_thread_ != 0) delete animation_thread_;
				animation_thread_ = new AnimationThread();
				animation_thread_->start();
				return;
			#endif
	
			animate_();
		}

		void Scene::stopAnimation()
			throw()
		{
			stop_animation_ = true;
		}

		void Scene::recordAnimationClicked()
			throw()
		{
			menuBar()->setItemChecked(record_animation_id_, !menuBar()->isItemChecked(record_animation_id_));
		}

		void Scene::animate_()
			throw()
		{
			if (!lockComposites()) return;
			menuBar()->setItemChecked(record_animation_id_, false);
			menuBar()->setItemEnabled(record_animation_id_, false);

			bool export_PNG = menuBar()->isItemChecked(animation_export_PNG_id_);
			bool export_POV = menuBar()->isItemChecked(animation_export_POV_id_);
			bool repeat     = menuBar()->isItemChecked(animation_repeat_id_);

			do
			{
				List<Camera>::Iterator it = getAnimationPoints().begin();
				Camera last_camera = *it;
				it++;

				for (; it != getAnimationPoints().end(); it++)
				{
					if (*it == last_camera) continue;
			 
					Camera camera = last_camera;
					Vector3 diff_viewpoint 	= (camera.getViewPoint() 			- (*it).getViewPoint());
					Vector3 diff_up 				= (camera.getLookUpVector() 	- (*it).getLookUpVector());
					Vector3 diff_look_at 		= (camera.getLookAtPosition() - (*it).getLookAtPosition());

					Vector3 max = diff_viewpoint;
					if (diff_look_at.getLength() > max.getLength()) max = diff_look_at;
					
					Size steps = (Size) (max.getLength() * animation_smoothness_);
					if (steps == 0) steps = 1;
					
					diff_viewpoint 	/= steps;
					diff_up 				/= steps;
					diff_look_at 		/= steps;

					for (Size i = 0; i < steps && !stop_animation_; i++)
					{
						#ifdef BALL_QT_HAS_THREADS
							while (qApp->hasPendingEvents())
							{
								animation_thread_->mySleep(50);
							}
						#endif

						camera.setViewPoint(camera.getViewPoint() - diff_viewpoint);
						camera.setLookUpVector(camera.getLookUpVector() - diff_up);
						camera.setLookAtPosition(camera.getLookAtPosition() - diff_look_at);

						Scene::SceneSetCameraEvent* e = new Scene::SceneSetCameraEvent();
						e->camera = camera;
						qApp->postEvent(this, e);

						if (export_PNG)
						{
							Scene::SceneExportPNGEvent* e = new Scene::SceneExportPNGEvent();
							qApp->postEvent(this, e);
						}

						if (export_POV)
						{
							Scene::SceneExportPOVEvent* e = new Scene::SceneExportPOVEvent();
							qApp->postEvent(this, e);
						}
					}
					
					last_camera = *it;
					if (stop_animation_) break;
				}
			}
			while((!stop_animation_) && repeat);

			stop_animation_ = false;
			menuBar()->setItemEnabled(start_animation_id_, true);
			menuBar()->setItemEnabled(cancel_animation_id_, false);
			menuBar()->setItemEnabled(record_animation_id_, true);

			menuBar()->setItemEnabled(animation_repeat_id_, true);
			menuBar()->setItemEnabled(animation_export_POV_id_, true);
			menuBar()->setItemEnabled(animation_export_PNG_id_, true);
			menuBar()->setItemEnabled(clear_animation_id_, true);
			unlockComposites();
		}

		void Scene::animationRepeatClicked()
			throw()
		{
			menuBar()->setItemChecked(animation_repeat_id_, !menuBar()->isItemChecked(animation_repeat_id_));
		}

		void Scene::animationExportPOVClicked()
			throw()
		{
			menuBar()->setItemChecked(animation_export_POV_id_, !menuBar()->isItemChecked(animation_export_POV_id_));
		}

		void Scene::animationExportPNGClicked()
			throw()
		{
			menuBar()->setItemChecked(animation_export_PNG_id_, !menuBar()->isItemChecked(animation_export_PNG_id_));
		}

		void Scene::switchToLastMode()
			throw()
		{
			switch (last_mode_)
			{
 				case PICKING__MODE: 
					pickingMode_();
					break;
 				case ROTATE__MODE: 
					rotateMode_();
					break;
 				case MOVE__MODE: 
					moveMode_();
					break;
			}
		}

		void Scene::dropEvent(QDropEvent* e)
		{
			if (!QUriDrag::canDecode(e)) 
			{
				e->ignore();
				return;
			}

			QStrList lst;
			QUriDrag::decode(e, lst);
			e->accept();

			for (Position i = 0; i < lst.count(); ++i )
			{
				QString filename = QDir::convertSeparators(QUriDrag::uriToLocalFile(lst.at(i)));
				getMainControl()->openFile(filename.ascii());
			}
		}

		void Scene::dragEnterEvent(QDragEnterEvent* event)
		{
			event->accept(QTextDrag::canDecode(event));
		}

} }// namespaces
