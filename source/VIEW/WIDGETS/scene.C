// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: scene.C,v 1.174.2.30 2006/04/25 16:25:54 amoll Exp $
//

#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/stage.h>
#include <BALL/VIEW/KERNEL/clippingPlane.h>

#include <BALL/VIEW/DIALOGS/setCamera.h>
#include <BALL/VIEW/DIALOGS/preferences.h>
#include <BALL/VIEW/DIALOGS/lightSettings.h>
#include <BALL/VIEW/DIALOGS/stageSettings.h>
#include <BALL/VIEW/DIALOGS/materialSettings.h>

#include <BALL/VIEW/PRIMITIVES/simpleBox.h>
#include <BALL/VIEW/PRIMITIVES/box.h>
#include <BALL/VIEW/PRIMITIVES/label.h>
#include <BALL/VIEW/RENDERING/POVRenderer.h>
#include <BALL/VIEW/RENDERING/VRMLRenderer.h>

#include <BALL/VIEW/PRIMITIVES/sphere.h>
#include <BALL/VIEW/PRIMITIVES/tube.h>
#include <BALL/VIEW/PRIMITIVES/disc.h>
#include <BALL/VIEW/PRIMITIVES/line.h>

#include <BALL/SYSTEM/timer.h>
#include <BALL/MATHS/quaternion.h>

#include <BALL/STRUCTURE/geometricTransformations.h>
#include <BALL/STRUCTURE/geometricProperties.h>

#include <QtGui/qpainter.h>
#include <QtGui/qimage.h>
#include <QtGui/qcursor.h>
#include <QtGui/qapplication.h>
#include <QtGui/QDesktopWidget>
#include <QtGui/QFileDialog>

#include <BALL/VIEW/WIDGETS/datasetControl.h>
#include <BALL/VIEW/DATATYPE/colorMap.h>

//         #define BALL_BENCHMARKING

using std::endl;
using std::istream;

namespace BALL
{
	namespace VIEW
	{

		Position Scene::screenshot_nr_ = 100000;
		Position Scene::pov_nr_ = 100000;

		// ###############CONSTRUCTORS,DESTRUCTORS,CLEAR###################

		// values for mouse-sensitivity 
		float Scene::mouse_sensitivity_ = 5;
		float Scene::mouse_wheel_sensitivity_ = 5;
		bool  Scene::show_light_sources_ = false;
		float Scene::animation_smoothness_ = 2;

		#define  ROTATE_FACTOR    50.
		#define  ROTATE_FACTOR2   50.
		#define  TRANSLATE_FACTOR 4.
    #define  ZOOM_FACTOR      7.

	  QGLFormat Scene::gl_format_(QGL::DepthBuffer 		| 
																QGL::StereoBuffers 	| 
																QGL::DoubleBuffer 	| 
																QGL::DirectRendering |
																QGL::SampleBuffers  |
																QGL::StencilBuffer);

		Scene::Scene()
			throw()
			:	QGLWidget(gl_format_),
				ModularWidget("<Scene>"),
				rb_(new QRubberBand(QRubberBand::Rectangle, this)),
				stage_(new Stage()),
				light_settings_(new LightSettings(this)),
				stage_settings_(new StageSettings(this)),
				material_settings_(new MaterialSettings(this)),
				animation_thread_(0)
		{
			gl_renderer_.setSize(600, 600);
			setAcceptDrops(true);
#ifdef BALL_VIEW_DEBUG
			Log.error() << "new Scene (1) " << this << std::endl;
#endif
		}

		Scene::Scene(QWidget* parent_widget, const char* name, Qt::WFlags w_flags)
			throw()
			:	QGLWidget(gl_format_, parent_widget, (QGLWidget*)0, w_flags),
				ModularWidget(name),
				current_mode_(ROTATE__MODE),
				last_mode_(PICKING__MODE),
				rotate_action_(0),
				picking_action_(0),
				system_origin_(0.0, 0.0, 0.0),
				need_update_(false),
				update_running_(false),
				x_window_pos_old_(0),
				y_window_pos_old_(0),
				x_window_pos_new_(0),
				y_window_pos_new_(0),
				x_window_pick_pos_first_(0),
				y_window_pick_pos_first_(0),
				rb_(new QRubberBand(QRubberBand::Rectangle, this)),
				stage_(new Stage),
				light_settings_(0),
				stage_settings_(0),
				animation_thread_(0),
				stop_animation_(false),
				content_changed_(true),
				want_to_use_vertex_buffer_(false)
		{
#ifdef BALL_VIEW_DEBUG
			Log.error() << "new Scene (2) " << this << std::endl;
#endif
			setObjectName(name);
			// the widget with the MainControl
			registerWidget(this);
			gl_renderer_.setSize(600, 600);
			setAcceptDrops(true);

			if (!QGLWidget::isValid())
			{
				Log.error() << "QGLWidget is not valid in Scene!" << std::endl;
			}
		}

		Scene::Scene(const Scene& scene, QWidget* parent_widget, const char* name, Qt::WFlags w_flags)
			throw()
			:	QGLWidget(gl_format_, parent_widget, (QGLWidget*)0, w_flags),
				ModularWidget(scene),
				system_origin_(scene.system_origin_),
				rb_(new QRubberBand(QRubberBand::Rectangle, this)),
				stage_(new Stage(*scene.stage_)),
				light_settings_(new LightSettings(this)),
				stage_settings_(new StageSettings(this)),
				material_settings_(new MaterialSettings(this)),
				animation_thread_(0),
				stop_animation_(false)
		{
#ifdef BALL_VIEW_DEBUG
			Log.error() << "new Scene (3) " << this << std::endl;
#endif

			setObjectName(name);

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

				if (animation_thread_ != 0) delete animation_thread_;
			}

		void Scene::clear()
			throw()
		{
			system_origin_.set(0.0, 0.0, 0.0);

			stage_->clear();
			animation_points_.clear();
		}

		void Scene::set(const Scene& scene)
			throw()
		{
			stage_ = scene.stage_;
			system_origin_.set(scene.system_origin_);
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
			qApp->processEvents();
			makeCurrent();

			if (RTTI::isKindOf<RepresentationMessage>(*message)) 
			{
				RepresentationMessage* rm = RTTI::castTo<RepresentationMessage>(*message);
				Representation* rep = rm->getRepresentation();
				switch (rm->getType())
				{
					case RepresentationMessage::UPDATE:
						gl_renderer_.bufferRepresentation(*rep);
						break;

					case RepresentationMessage::REMOVE:
						gl_renderer_.removeRepresentation(*rep);
						break;

					default:
						break;
				}

				content_changed_ = true;

				update(false);
				return;
			}

			// react now only to SceneMessage
			if (!RTTI::isKindOf<SceneMessage>(*message)) return;

			SceneMessage *scene_message = RTTI::castTo<SceneMessage>(*message);

			switch (scene_message->getType())
			{
				case SceneMessage::REDRAW:
					content_changed_ = true;
					update(false);
					return;

				case SceneMessage::REBUILD_DISPLAY_LISTS:
					content_changed_ = true;
					update(true);
					return;

				case SceneMessage::UPDATE_CAMERA:
					content_changed_ = true;
					stage_->getCamera() = scene_message->getStage().getCamera();
					system_origin_ = scene_message->getStage().getCamera().getLookAtPosition();
					updateCamera_();
					light_settings_->updateFromStage();
					return;

				case SceneMessage::REMOVE_COORDINATE_SYSTEM:
					content_changed_ = true;
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

			makeCurrent();

			gl_renderer_.init(*this);
			gl_renderer_.initSolid();
			if (stage_->getLightSources().size() == 0) setDefaultLighting(false);
			gl_renderer_.updateCamera();
			gl_renderer_.enableVertexBuffers(want_to_use_vertex_buffer_);
 			stage_settings_->getGLSettings();
		}

		void Scene::paintGL()
		{
//    			if (!content_changed_) return;

			// cannot call update here, because it calls updateGL
   		renderView_(DISPLAY_LISTS_RENDERING);

			if (info_string_ != "")
			{
				ColorRGBA c1 = stage_->getBackgroundColor();
				ColorRGBA c2 = c1.getInverseColor();
				QFont font;
				font.setPixelSize(16);
				font.setBold(true);
				gl_renderer_.setColorRGBA_(c1);
				renderText(info_point_.x() + 1, info_point_.y() + 1, info_string_.c_str(), font);
				renderText(info_point_.x() - 1, info_point_.y() - 1, info_string_.c_str(), font);
				gl_renderer_.setColorRGBA_(c2);
				renderText(info_point_.x(), info_point_.y(), info_string_.c_str(), font);
			}
		}

		void Scene::resizeGL(int width, int height)
		{
			gl_renderer_.setSize(width, height);
			gl_renderer_.updateCamera();
			content_changed_ = true;
		}


		void Scene::renderView_(RenderMode mode)
			throw()
		{
			makeCurrent();

			glDepthMask(GL_TRUE);
			if (gl_renderer_.getStereoMode() == GLRenderer::NO_STEREO)
			{
				glDrawBuffer(GL_BACK);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				renderRepresentations_(mode);
				content_changed_ = false;

				return;
			}

			glDrawBuffer(GL_BACK_LEFT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// ok, this is going the stereo way...
			stereo_camera_ = stage_->getCamera();

			Vector3 old_view_point = stage_->getCamera().getViewPoint();
			Vector3 old_look_at = stage_->getCamera().getLookAtPosition();
			Vector3	diff = stage_->getCamera().getRightVector() * (stage_->getEyeDistance() / 2.0);  

			float nearf = 1.5; 
			float farf = 600;

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
			renderRepresentations_(DISPLAY_LISTS_RENDERING);
			glPopMatrix();
			content_changed_ = false;
		}

		void Scene::renderRepresentations_(RenderMode mode)
			throw()
		{
			RepresentationManager& pm = getMainControl()->getRepresentationManager();
			
			// ============== enable active Clipping planes ==============================
			GLint current_clipping_plane = GL_CLIP_PLANE0;

			vector<ClippingPlane*> active_planes;
			vector<ClippingPlane*> inactive_planes;

			bool move_mode = (mouse_button_is_pressed_ && getMode() == MOVE__MODE);
			
			const vector<ClippingPlane*>& vc = pm.getClippingPlanes();
			vector<ClippingPlane*>::const_iterator plane_it = vc.begin();
			for (;plane_it != vc.end(); plane_it++)
			{
				ClippingPlane& plane = **plane_it;

				if (plane.isHidden()) continue;

				if (!plane.isActive()) 
				{
					inactive_planes.push_back(*plane_it);
					if (!move_mode) continue;
				}

				active_planes.push_back(*plane_it);

				const Vector3& n(plane.getNormal());
				const GLdouble planef[] ={n.x, n.y, n.z, plane.getDistance()};
				glClipPlane(current_clipping_plane, planef);
				current_clipping_plane++;
			}

			// -------------------------------------------------------------------
			// show light sources
			if (show_light_sources_)
			{
				List<LightSource>::ConstIterator lit = stage_->getLightSources().begin();
				for (; lit != stage_->getLightSources().end(); lit++)
				{
					Vector3 pos = (*lit).getPosition();
					Vector3 dir = (*lit).getDirection();

					if ((*lit).isRelativeToCamera())
					{
						pos = stage_->getCamera().getViewPoint() + stage_->calculateAbsoluteCoordinates(pos);
						dir = pos + stage_->calculateAbsoluteCoordinates(dir);
					}

					Vector3 diff = dir - pos;
					if (!Maths::isZero(diff.getSquareLength())) diff.normalize();
					diff *= 100.0;

					Sphere s;
					s.setPosition(pos);
					s.setRadius(5);
					s.setColor(ColorRGBA(255,255,255));
					gl_renderer_.renderSphere_(s);

					Tube t;
					t.setVertex1(pos);
					t.setVertex2(pos + diff);
					t.setColor(ColorRGBA(255,255,255));
					gl_renderer_.renderTube_(t);
				}
			}
			// -------------------------------------------------------------------
			

			DatasetControl& dc = *DatasetControl::getInstance(0);
			List<std::pair<RegularData3D*, String> > grids = dc.get3DGrids();
			ColorMap map;
			map.setRange(0, 50);
			ColorRGBA colors[3];
			colors[0] = ColorRGBA(1.0, 0, 0);
			colors[1] = ColorRGBA(.0, 1.0, 0);
			colors[2] = ColorRGBA(.0, 0, 1.0);
			map.setBaseColors(colors, 3);
			map.setNumberOfColors(255);
			map.createMap();
			if (grids.size() > 0)
			{
				const RegularData3D& grid = *(*grids.begin()).first;
				gl_renderer_.renderVolume(grid, map);
			}

			
			// we draw all the representations in different runs, 
			// 1. normal reps
			// 2. transparent reps
			// 3. allways front
			for (Position run = 0; run < 3; run++)
			{
				if (run == 1)
				{
					// render inactive clipping planes
					for (plane_it = inactive_planes.begin(); plane_it != inactive_planes.end(); plane_it++)
					{
						gl_renderer_.renderClippingPlane_(**plane_it);
					}
				}

				RepresentationManager::RepresentationList::ConstIterator it = pm.getRepresentations().begin();
				for(; it != pm.getRepresentations().end(); it++)
				{
					Representation& rep = **it;
					if (rep.isHidden()) continue;

					if (run == 0)
					{
						// render all "normal" (non always front and non transparent models)
						if (rep.getTransparency() != 0 ||
								rep.hasProperty(Representation::PROPERTY__ALWAYS_FRONT))
						{
							continue;
						}
					}
					else if (run == 1)
					{
						// render all transparent models
						if (rep.getTransparency() == 0 ||
								rep.hasProperty(Representation::PROPERTY__ALWAYS_FRONT))
						{
							continue;
						}
					}
					else
					{
						// render all always front models
						if (!rep.hasProperty(Representation::PROPERTY__ALWAYS_FRONT)) continue;
					}
					
					vector<Position> rep_active_planes; // clipping planes

					Index cap_nr = -1;
					for (Position plane_nr = 0; plane_nr < active_planes.size(); plane_nr++)
					{
						if (!active_planes[plane_nr]->getRepresentations().has(*it)) continue;
						
						const ClippingPlane& plane = *active_planes[plane_nr];
						rep_active_planes.push_back(plane_nr);
						if (!plane.cappingEnabled())
						{
							glEnable(plane_nr + GL_CLIP_PLANE0);
							continue;
						}

						cap_nr = plane_nr;
					}

					// no capping? then we are done: disable again all clipping planes
					if (cap_nr == -1)
					{
						render_(rep, mode);

						for (Position p = 0; p < rep_active_planes.size(); p++)
						{
							glDisable(rep_active_planes[p] + GL_CLIP_PLANE0);
						}
						continue;
					}

					// draw a capping plane
 					const ClippingPlane& plane = *active_planes[cap_nr];
					Vector3 p, x, y, n;
					p = plane.getPoint();
					x = getNormal(plane.getNormal()) * 400;
					y = x % plane.getNormal() * 400;
 					n = plane.getNormal();
					n.normalize();

					// fill the stencil buffer
					glEnable(cap_nr + GL_CLIP_PLANE0);
					glEnable(GL_STENCIL_TEST);
					glClearStencil(0);
					glClear(GL_STENCIL_BUFFER_BIT);
					glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
					glStencilFunc(GL_ALWAYS, 0x0, 0xff);
					glStencilOp(GL_KEEP, GL_INVERT, GL_INVERT);
					render_(rep, mode);

					// render the Representation once again, this time with colors
					gl_renderer_.setColorRGBA_(ColorRGBA(0,1.0,0));
					glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
					glDisable(GL_STENCIL_TEST);
					render_(rep, mode);

					// disable all clipping planes
					for (Position p = 0; p < rep_active_planes.size(); p++)
					{
						glDisable(rep_active_planes[p] + GL_CLIP_PLANE0);
					}

					// render the capping plane
					glEnable(GL_STENCIL_TEST);
					glStencilFunc(GL_NOTEQUAL, 0x0, 0xff);

					ColorRGBA color = ClippingPlane::getCappingColor();
					bool transparent = (int)color.getAlpha() != 255;
					if (transparent) gl_renderer_.initTransparent();
					else 						 gl_renderer_.initSolid();

					Disc d(Circle3(p, n, 400));
					d.setColor(color);
					gl_renderer_.render_(&d);
					
					glDisable(GL_STENCIL_TEST);
				}
			}
		}


		void Scene::render_(const Representation& repr, RenderMode mode)
			throw()
		{
			if (mode == DISPLAY_LISTS_RENDERING)
			{
				gl_renderer_.drawBuffered(repr);
				return;
			}

			RepresentationManager& pm = getMainControl()->getRepresentationManager();
			Representation* rep = (Representation*)& repr;

			pm.startedRendering(rep);

			if (mode == REBUILD_DISPLAY_LISTS)
			{
				gl_renderer_.bufferRepresentation(repr);
			}
			else //	DIRECT_RENDERING:
			{
				gl_renderer_.render(repr);
			}

			pm.finishedRendering(rep);
		}

		/////////////////////////////////////////////////////////

		Vector3 Scene::getTranslationVector_(const Vector3& v)
		{
			const Camera& camera = stage_->getCamera();

			Vector3 vv = camera.getViewVector();
			vv.normalize(); 

			return v.x * camera.getRightVector() +
						 v.y * camera.getLookUpVector() -
						 v.z * vv;
		}
		
		void Scene::rotateClockwise(float degree)
		{
			Camera camera(stage_->getCamera());
			Matrix4x4 m;
			m.setRotation(Angle(-degree, false), camera.getViewVector());

			camera.setLookUpVector(m * camera.getLookUpVector());
			stage_->getCamera() = camera;
			updateCamera_();
		}

		void Scene::move(Vector3 v)
		{
			Camera& camera = stage_->getCamera();

			camera.translate(-getTranslationVector_(v));
			updateCamera_();
		}

		void Scene::rotate(float degree_right, float degree_up)
		{
			const Camera& camera = stage_->getCamera();

			Quaternion q1;
 			q1.set(camera.getLookUpVector(), Angle(degree_right, false).toRadian());

			Quaternion q2;
 			q2.set(camera.getRightVector(), Angle(degree_up, false).toRadian());

 			q1 += q2;
			
			stage_->getCamera().rotate(q1, system_origin_);
			updateCamera_();
		}

		void Scene::moveComposites(const List<Composite*>& composites, Vector3 v)
		{
			HashSet<Composite*> roots;

			Vector3 x = getTranslationVector_(v);

			Matrix4x4 m;
			m.setTranslation(x);
			TransformationProcessor tp(m);

			List<Composite*>::const_iterator cit = composites.begin();
			for (; cit != composites.end(); ++cit)
			{
				roots.insert(&(**cit).getRoot());
				(**cit).apply(tp);
			}

			HashSet<Composite*>::Iterator rit = roots.begin();
			for(; rit != roots.end(); rit++)
			{
				getMainControl()->updateRepresentationsOf(**rit, true, false);
			}
		}
	
		void Scene::rotateComposites(const List<Composite*>& selection, float degree_right, float degree_up, float degree_clockwise)
		{
			const Camera& camera = stage_->getCamera();

			Vector3 vv = camera.getViewVector();
			vv.normalize(); 

			Quaternion q1;
 			q1.set(camera.getLookUpVector(), Angle(degree_right, false).toRadian());

			Quaternion q2;
 			q2.set(camera.getRightVector(), Angle(degree_up, false).toRadian());

			Quaternion Q;
 			Q.set(camera.getViewVector(), Angle(degree_clockwise, false).toRadian());

 			q1 += q2;
 			q1 += Q;
			
			GeometricCenterProcessor center_processor;
			Vector3 center;
			List<Composite*>::const_iterator cit = selection.begin();
			for(; cit != selection.end(); cit++)
			{
				(*cit)->apply(center_processor);
				center += center_processor.getCenter();
			}
				
			center /= (float) selection.size();

			Matrix4x4 mym1, mym2, m;
			mym1.setTranslation(center * -1);
			mym2.setTranslation(center);

			q1.getRotationMatrix(m);

			TransformationProcessor tp1(mym1);
			TransformationProcessor tp2(m);
			TransformationProcessor tp3(mym2);

			HashSet<Composite*> roots;

			for (cit = selection.begin(); cit != selection.end(); cit++) 
			{
				(*cit)->apply(tp1);
				(*cit)->apply(tp2);
				(*cit)->apply(tp3) ;
				roots.insert(&(**cit).getRoot());
			}

			HashSet<Composite*>::Iterator rit = roots.begin();
			for(; rit != roots.end(); rit++)
			{
				getMainControl()->updateRepresentationsOf(**rit, true, false);
			}
		}

		/////////////////////////////////////////////////////////

		float Scene::getXDiff_()
		{
			float delta_x = (float)x_window_pos_new_ - (float)x_window_pos_old_;
			delta_x /= qApp->desktop()->width();
			delta_x *= mouse_sensitivity_;
			return delta_x;
		}

		float Scene::getYDiff_()
		{
			float delta_y = (float)y_window_pos_new_ - (float)y_window_pos_old_;
			delta_y /= qApp->desktop()->width();
			delta_y *= mouse_sensitivity_;
			return delta_y;
		}

		void Scene::rotateSystemClockwise_()
		{
			if (current_mode_ != ROTATE__MODE) return;

			float x = getXDiff_();

			if (x == 0) return;

			x *= ROTATE_FACTOR2;

			rotateClockwise(x);
		}


		void Scene::rotateSystem_()
		{
			float x = getXDiff_() * ROTATE_FACTOR;
			float y = getYDiff_() * ROTATE_FACTOR;

			rotate(-x, -y);
		}


		void Scene::translateSystem_()
		{
			float x = getXDiff_() * TRANSLATE_FACTOR; 
			float y = getYDiff_() * TRANSLATE_FACTOR;

			move(Vector3(x, -y, 0));
		}


		void Scene::zoomSystem_()
		{
			float y = getYDiff_() * ZOOM_FACTOR;

			move(Vector3(0, 0, y));
		}


		// picking routine ------
		void Scene::selectObjects_()
		{
			rb_->hide();

			QPoint p0 = mapFromGlobal(QPoint(x_window_pick_pos_first_, y_window_pick_pos_first_));
			QPoint p1 = mapFromGlobal(QPoint(x_window_pos_new_, y_window_pos_new_));

			List<GeometricObject*> objects;
			gl_renderer_.pickObjects1((Position) p0.x(),
																(Position) p0.y(),
																(Position) p1.x(),
																(Position) p1.y());

			// draw the representations
			renderView_(DIRECT_RENDERING);

 			gl_renderer_.pickObjects2(objects);

			// sent collected objects
			GeometricObjectSelectionMessage* message = new GeometricObjectSelectionMessage;
			message->setSelection(objects);
			message->setSelected(pick_select_);
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
			stage_->getCamera().clear();
			updateCamera_();
 			light_settings_->updateFromStage();
		}

		
		void Scene::updateCamera_()
			throw()
		{
			if (gl_renderer_.getStereoMode() == GLRenderer::NO_STEREO)
			{
				gl_renderer_.updateCamera();
 				gl_renderer_.setLights();
			}

			updateGL();
		}


		// ##################MISC############################

		void Scene::setDefaultLighting(bool update_GL)
			throw()
		{
			LightSource light;
			light.setType(LightSource::POSITIONAL);
			light.setRelativeToCamera(true);
			light.setPosition(Vector3(0, 4, -20));
			light.setDirection(Vector3(0, 0, 1));

			stage_->clearLightSources();
			stage_->addLightSource(light);
			gl_renderer_.setLights(true);
			light_settings_->updateFromStage();

			if (update_GL) renderView_(REBUILD_DISPLAY_LISTS);
		}


		void Scene::createCoordinateSystem()
			throw()
		{
			RepresentationManager& pm = getMainControl()->getRepresentationManager();

			Representation* rp = pm.createRepresentation();
			rp->setTransparency(90);

			const Camera& s = getStage()->getCamera();

			float size = 100;

			Vector3 v = s.getViewVector();
			v.normalize();
			
			Vector3 p = s.getViewPoint() + (v * 25) - (s.getLookUpVector()* 5);

			Vector3 x = -v + s.getRightVector();
			x.normalize();
			Vector3 y = -v - s.getRightVector();
			y.normalize();

			Vector3 z = -(x % y);

			Box* xp = new Box(p, x * size, z * size, 0.05);
			Box* yp = new Box(p, y * size, z * size, 0.05);
			Box* zp = new Box(p, x * size, y * size, 0.05);

			ColorRGBA color(0,255,190,180);
			xp->setColor(color);
			yp->setColor(color);
			zp->setColor(color);

			rp->insert(*xp);
			rp->insert(*yp);
 			rp->insert(*zp);

			ColorRGBA color2(0,255,255,160);

			Vector3 p1[6], p2[6];
			p1[0].set(x); p2[0].set(z);
			p1[1].set(y); p2[1].set(z);
			p1[2].set(x); p2[2].set(y);

			for (Position i = 0; i <= size; i+=1)
			{
				for (Position j = 0; j < 6; j++)
				{
					Line* line1 = new Line();
					line1->setVertex1(p + p1[j] * i);
					line1->setVertex2(p + p1[j] * i + p2[j] * size);
					line1->setColor(color);
					rp->insert(*line1);

					Line* line2 = new Line();
					line2->setVertex1(p + p2[j] * i);
					line2->setVertex2(p + p2[j] * i + p1[j] * size);
					line2->setColor(color);
					rp->insert(*line2);
				}
			}


			for (Position i = 0; i <= size; i+=10)
			{
				for (Position j = 0; j < 6; j++)
				{
					Line* line1 = new Line();
					line1->setVertex1(p + p1[j] * i);
					line1->setVertex2(p + p1[j] * i + p2[j] * size);
					rp->insert(*line1);

					Line* line2 = new Line();
					line2->setVertex1(p + p2[j] * i);
					line2->setVertex2(p + p2[j] * i + p1[j] * size);
					rp->insert(*line2);
				}
			}

			/*
			ColorRGBA color3(0,255,255,200);
			for (Position xi = 10; xi <= size; xi += 10)
			{
				for (Position yi = 10; yi <= size; yi += 10)
				{
					Vector3 point1(p + x * xi + y * yi);
					Label* label1 = new Label();
					label1->setVertex(point1);
					label1->setColor(color3);
					label1->setText(String(xi) + "," + String(yi));
					rp->insert(*label1);

					Vector3 point2(p + x * xi + z * yi);
					Label* label2 = new Label();
					label2->setVertex(point2);
					label2->setColor(color3);
					label2->setText(String(xi) + "," + String(yi));
					rp->insert(*label2);

					Vector3 point3(p + y * xi + z * yi);
					Label* label3 = new Label();
					label3->setVertex(point3);
					label3->setColor(color3);
					label3->setText(String(xi) + "," + String(yi));
					rp->insert(*label3);
				}
			}
			*/

			rp->setProperty(Representation::PROPERTY__IS_COORDINATE_SYSTEM);

			// we have to add the representation in the GLRenderer manualy,
			// because the message wont arrive in Scene::onNotify
			gl_renderer_.bufferRepresentation(*rp);

			// notify GeometricControl
			notify_(new RepresentationMessage(*rp, RepresentationMessage::ADD));
		}


		bool Scene::exportScene(Renderer &er) const
			throw()
		{
			if (er.init(*stage_, (float) width(), (float) height()))
			{
				RepresentationManager::RepresentationList::ConstIterator it;
				MainControl *main_control = MainControl::getMainControl(this);

				it = main_control->getRepresentationManager().getRepresentations().begin();
				for (; it != main_control->getRepresentationManager().getRepresentations().end(); it++)
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
			inifile.insertValue("WINDOWS", getIdentifier() + "::on", String(true));

			inifile.appendSection("EXPORT");
			inifile.insertValue("EXPORT", "POVNR", String(pov_nr_));
			inifile.insertValue("EXPORT", "PNGNR", String(screenshot_nr_));
			writeLights_(inifile);

			if (inifile.hasSection("BALLVIEW_PROJECT"))
			{
				inifile.insertValue("BALLVIEW_PROJECT", "Camera", getStage()->getCamera().toString());
			}
		}


		void Scene::fetchPreferences(INIFile& inifile)
			throw()
		{
			ModularWidget::fetchPreferences(inifile);
			if (inifile.hasEntry("EXPORT", "POVNR"))
			{
				pov_nr_ = inifile.getValue("EXPORT", "POVNR").toUnsignedInt();
			}

			if (inifile.hasEntry("EXPORT", "PNGNR"))
			{
				screenshot_nr_ = inifile.getValue("EXPORT", "PNGNR").toUnsignedInt();
			}

			readLights_(inifile);
			light_settings_->updateFromStage();
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
			updateCamera_();

			bool showed_coordinate = stage_->coordinateSystemEnabled();
			stage_settings_->apply();
			RepresentationManager& pm = getMainControl()->getRepresentationManager();

			if (showed_coordinate && !stage_->coordinateSystemEnabled())
			{
				RepresentationManager::RepresentationsIterator it = pm.begin();
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
					notify_(new RepresentationMessage(*coordinate_rep, RepresentationMessage::REMOVE));
				}
			}
			else if (!showed_coordinate && stage_->coordinateSystemEnabled()) 
			{
				createCoordinateSystem();
			}

			material_settings_->apply();

			gl_renderer_.updateBackgroundColor(); 

			if (stage_->getFogIntensity() == 0)
			{
				glDisable(GL_FOG);
			}
			else
			{
				/// TODO: Fog in POVRAY!
				glEnable(GL_FOG);

				ColorRGBA co = stage_->getBackgroundColor();
				GLfloat color[4] = {(float) co.getRed(), (float) co.getGreen(), (float) co.getBlue(), 1.0};
				glFogfv(GL_FOG_COLOR, color);

				glFogf(GL_FOG_START, 2.0);
 				glFogf(GL_FOG_END, (200 - ((float)stage_->getFogIntensity()) / 2.0) + 12);
				glFogi(GL_FOG_MODE, GL_LINEAR);

				// doesnt work as expected:
				// glFogf(GL_FOG_END, 400);
				// glFogf(GL_FOG_DENSITY, ((float) stage_->getFogIntensity()) / 40.0);
			}

			renderView_(REBUILD_DISPLAY_LISTS);
			updateGL();
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

				Vector3 dir = light.getDirection();
				Vector3 pos = light.getPosition();

				data = vector3ToString(pos);
				inifile.insertValue("LIGHTING", "Light_" + String(nr) + "_Position",  vector3ToString(pos));
				inifile.insertValue("LIGHTING", "Light_" + String(nr) + "_Direction", vector3ToString(dir));

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
				stage_->clearLightSources();
				vector<String> strings;
				Position nr = 0;

				try
				{
					while(inifile.hasEntry("LIGHTING", "Light_" + String(nr) + "_Position"))
					{
						LightSource light;
						Vector3 pos, dir;

						String data = inifile.getValue("LIGHTING", "Light_" + String(nr) + "_Relative");
						light.setRelativeToCamera(data.toUnsignedInt());

						data = inifile.getValue("LIGHTING", "Light_" + String(nr) + "_Position");
						stringToVector3(data, pos);

						data = inifile.getValue("LIGHTING", "Light_" + String(nr) + "_Direction");
						stringToVector3(data, dir);

						light.setPosition(pos);
						light.setDirection(dir);

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

						stage_->addLightSource(light);
						nr++;
					}
				}
				catch(Exception::GeneralException e)
				{
					Log.error() << "Could not read lighting settings from Inifile" << std::endl;
					Log.error() << e;
				}

				if (!stage_->getLightSources().size())
				{
					setDefaultLighting(true);
				}
			}

		// ###########################MENUES##################################

		void Scene::initializeWidget(MainControl& main_control)
			throw()
		{
			setMinimumSize(10, 10);

			main_control.initPopupMenu(MainControl::DISPLAY);

			create_coordinate_system_ = 
				insertMenuEntry(MainControl::DISPLAY, "Show Coordinate System", this, SLOT(createCoordinateSystem()));
			setMenuHint("Show a coordinate system");

			main_control.insertPopupMenuSeparator(MainControl::DISPLAY);
			rotate_action_ =	insertMenuEntry(
					MainControl::DISPLAY, "&Rotate Mode", this, SLOT(rotateMode_()), Qt::CTRL+Qt::Key_R);
			setMenuHint("Switch to rotate/zoom mode");
			rotate_action_->setCheckable(true);

			picking_action_ = insertMenuEntry(MainControl::DISPLAY, "&Picking Mode", this, SLOT(pickingMode_()), Qt::CTRL+Qt::Key_P);
			setMenuHint("Switch to picking mode, e.g. to identify singe atoms or groups");
			setMenuHelp("scene.html#identify_atoms");
			picking_action_->setCheckable(true);

			move_action_ = insertMenuEntry(MainControl::DISPLAY, "Move Mode", this, SLOT(moveMode_()));
			setMenuHint("Move selected items");
			setMenuHelp("molecularControl.html#move_molecule");
			move_action_->setCheckable(true);

			main_control.insertPopupMenuSeparator(MainControl::DISPLAY);

			no_stereo_action_ = insertMenuEntry(MainControl::DISPLAY_STEREO, "No Stereo", this, SLOT(exitStereo()));
			no_stereo_action_->setChecked(true);
			setMenuHelp("tips.html#3D");
			no_stereo_action_->setCheckable(true);

			active_stereo_action_ = insertMenuEntry (
 					MainControl::DISPLAY_STEREO, "Shuttter Glasses", this, SLOT(enterActiveStereo()));
			setMenuHelp("tips.html#3D");
			active_stereo_action_->setCheckable(true);

			dual_stereo_action_ = insertMenuEntry (
 					MainControl::DISPLAY_STEREO, "Side by Side", this, SLOT(enterDualStereo()));
			setMenuHelp("tips.html#3D");
			dual_stereo_action_->setCheckable(true);

			insertMenuEntry(MainControl::DISPLAY_VIEWPOINT, "Show Vie&wpoint", this, SLOT(showViewPoint_()), Qt::CTRL+Qt::Key_W);
			setMenuHint("Print the coordinates of the current viewpoint");

			insertMenuEntry(MainControl::DISPLAY_VIEWPOINT, "Set Viewpoi&nt", this, SLOT(setViewPoint_()), Qt::CTRL+Qt::Key_N);
			setMenuHint("Move the viewpoint to the given coordinates");

			insertMenuEntry(MainControl::DISPLAY_VIEWPOINT, "Rese&t Camera", this, SLOT(resetCamera_()));
			setMenuHint("Reset the camera to the orgin (0,0,0)");

			insertMenuEntry(MainControl::FILE_EXPORT, "PNG...", this, SLOT(showExportPNGDialog()), Qt::ALT + Qt::Key_P);
			setMenuHint("Export a PNG image file from the Scene");

//   			insertMenuEntry(MainControl::FILE_EXPORT, "VRML...", this, SLOT(showExportVRMLDialog()));
//   			setMenuHint("Export a VRML file from the Scene");

			window_menu_entry_ = insertMenuEntry(MainControl::WINDOWS, "Scene", this, SLOT(switchShowWidget()));
			window_menu_entry_->setCheckable(true);
			setMenuHelp("scene.html");

			// ======================== ANIMATION ===============================================
			String help_url = "tips.html#animations";

			record_animation_action_ = insertMenuEntry(MainControl::DISPLAY_ANIMATION, "Record", this, 0);
			setMenuHint("Record an animation for later processing");
			setMenuHelp(help_url);
			record_animation_action_->setCheckable(true);
			
			clear_animation_action_ = insertMenuEntry(MainControl::DISPLAY_ANIMATION, "Clear", this, 
															SLOT(clearRecordedAnimation()));
			setMenuHelp(help_url);

			main_control.insertPopupMenuSeparator(MainControl::DISPLAY_ANIMATION);

			start_animation_action_ = insertMenuEntry(MainControl::DISPLAY_ANIMATION, "Start", this, SLOT(startAnimation()));
			setMenuHelp(help_url);

			cancel_animation_action_ = insertMenuEntry(MainControl::DISPLAY_ANIMATION, "Stop", this, SLOT(stopAnimation()));
			cancel_animation_action_->setEnabled(false);
			setMenuHelp(help_url);

			main_control.insertPopupMenuSeparator(MainControl::DISPLAY_ANIMATION);

			animation_export_PNG_action_ = insertMenuEntry(MainControl::DISPLAY_ANIMATION, "Export PNG", this, 0);
			setMenuHelp(help_url);
			animation_export_PNG_action_->setCheckable(true);

			animation_export_POV_action_ = insertMenuEntry(MainControl::DISPLAY_ANIMATION, "Export POV", this, 0);
			setMenuHelp(help_url);
			animation_export_POV_action_->setCheckable(true);

			animation_repeat_action_ = insertMenuEntry(MainControl::DISPLAY_ANIMATION, "Repeat", this, 0);
			setMenuHelp(help_url);
			animation_repeat_action_->setCheckable(true);

			setCursor(QCursor(Qt::SizeAllCursor));

			connect(&timer_, SIGNAL(timeout()), this, SLOT(timerSignal_()) );			

			registerForHelpSystem(this, "scene.html");
		}

		void Scene::checkMenu(MainControl& main_control)
			throw()
		{
			bool busy = main_control.isBusy();
			rotate_action_->setChecked(current_mode_ == ROTATE__MODE);
			picking_action_->setChecked(current_mode_ == PICKING__MODE);
			picking_action_->setEnabled(!busy);
			move_action_->setEnabled(!busy);

			create_coordinate_system_->setEnabled(!busy);

			bool animation_running = (animation_thread_ != 0 && animation_thread_->isRunning());
			
			start_animation_action_->setEnabled(
																animation_points_.size() > 0 && 
																!busy &&
																!animation_running);
			
			clear_animation_action_->setEnabled(animation_points_.size() > 0 && !animation_running);

			window_menu_entry_->setChecked(isVisible());
		}

		bool Scene::isAnimationRunning() const
			throw()
		{
			if (animation_thread_ != 0 && animation_thread_->isRunning())
			{
				return true;
			}

			return false;
		}

		//##########################EVENTS#################################

		void Scene::mouseMoveEvent(QMouseEvent* e)
		{
			if (isAnimationRunning()) return;

			makeCurrent();

			need_update_ = true;

			x_window_pos_new_ = e->globalX();
			y_window_pos_new_ = e->globalY();

			// ============ picking mode ================
			if (current_mode_ == PICKING__MODE)
			{
				if (e->buttons() == Qt::LeftButton  ||
						e->buttons() == Qt::RightButton)
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
			if (isAnimationRunning()) return;

			makeCurrent();

			mouse_button_is_pressed_ = true;

			x_window_pos_old_ = e->globalX();
			y_window_pos_old_ = e->globalY();

			if (current_mode_ == ROTATE__MODE) return;

			if (current_mode_ == PICKING__MODE)
			{	
				if (e->buttons() == Qt::LeftButton ||
						e->buttons() == Qt::RightButton)
				{
					pick_select_ = (e->buttons() == Qt::LeftButton);
					selectionPressed_();
				}
			}
		}

		Index Scene::getMoveModeAction_(const QMouseEvent& e)
		{
			if (e.modifiers() == Qt::NoModifier)
			{
				if (e.buttons() == Qt::LeftButton) 	return ROTATE_ACTION;
				if (e.buttons() == Qt::MidButton) 	return ZOOM_ACTION;
				if (e.buttons() == Qt::RightButton) return TRANSLATE_ACTION;
			}
			else if (e.buttons() == Qt::LeftButton)
			{
				if (e.modifiers() == Qt::ShiftModifier) 											return ZOOM_ACTION;
				if (e.modifiers() == Qt::ControlModifier) 										return TRANSLATE_ACTION;
				if (e.modifiers() == Qt::ShiftModifier | Qt::ControlModifier) return ROTATE_CLOCKWISE_ACTION;
			}
			else if (e.buttons() == Qt::LeftButton | Qt::RightButton)
			{
				return ROTATE_CLOCKWISE_ACTION;
			}

			return TRANSLATE_ACTION;
		}

		void Scene::processRotateModeMouseEvents_(QMouseEvent* e)
		{
			if (x_window_pos_old_ == e->globalX() &&
					y_window_pos_old_ == e->globalY())
			{
				return;
			}

			if (current_mode_ != ROTATE__MODE) return;
			
			content_changed_ = true;

			switch ((Index)(e->buttons() | e->modifiers()))
			{
				case (Qt::ShiftModifier | Qt::LeftButton): 
				case  Qt::MidButton:
					zoomSystem_();
					break;

				case (Qt::ControlModifier | Qt::LeftButton):
				case  Qt::RightButton:
					translateSystem_();
					break;

				case (Qt::LeftButton | (Index)Qt::RightButton):
				case (Qt::LeftButton | Qt::ShiftModifier | Qt::ControlModifier):
					rotateSystemClockwise_();
					break;

				case Qt::LeftButton:
					rotateSystem_();
					break;

				default:
					break;
			}

			if (record_animation_action_->isChecked())
			{
				animation_points_.push_back(stage_->getCamera());
			}
		}

		void Scene::processMoveModeMouseEvents_(QMouseEvent* e)
		{
			if(current_mode_ != MOVE__MODE) return;

			Camera& camera = stage_->getCamera();

			// Difference between the old and new position in the window 
			float delta_x = getXDiff_() / 4.0;
			float delta_y = getYDiff_() / 4.0;

			// stop if no movement
			if (delta_x == 0 && delta_y == 0) return;

			Index action = getMoveModeAction_(*e);

			// if we have a selection of Composites, we perform the moving here:
			const HashSet<Composite*>& selection = getMainControl()->getSelection();
			if (selection.size() != 0)
			{
				List<Composite*> composites;
				std::copy(selection.begin(), selection.end(), std::front_inserter(composites));

				switch (action)
				{
					case ZOOM_ACTION:
					{
						moveComposites(composites, Vector3(0,0, delta_y * ZOOM_FACTOR));
						return;
					}

					case TRANSLATE_ACTION:
					{
						moveComposites(composites, Vector3(delta_x * TRANSLATE_FACTOR ,-delta_y * TRANSLATE_FACTOR, 0));
						return;
					}

					case ROTATE_ACTION:
					{
						rotateComposites(composites, delta_x * ROTATE_FACTOR * 4., delta_y * ROTATE_FACTOR * 4., 0);
						return;
					}

					case ROTATE_CLOCKWISE_ACTION:
					{
						rotateComposites(composites, 0, 0, delta_x * ROTATE_FACTOR * 4.);
						return;
					}
				}

				return;
			}

			// moving for Clipping Planes is done in DatasetControl:
			// we create the Matrix for this here and send it per Message
			Matrix4x4 m;

			switch (action)
			{
				case ZOOM_ACTION:
				{
					Vector3 v = -stage_->getCamera().getViewVector();
					if (Maths::isZero(v.getSquareLength())) v = Vector3(1,0,0);
					v.normalize();

					v *= delta_y * ZOOM_FACTOR * 1.5;  

					m.setTranslation(v);
					break;
				}

				case TRANSLATE_ACTION:
				{
					// calculate translation in x-axis direction
					Vector3 right_translate = camera.getRightVector() * delta_x * TRANSLATE_FACTOR * 3.0;

					// calculate translation in y-axis direction
					Vector3 up_translate = camera.getLookUpVector() * delta_y * TRANSLATE_FACTOR * 3.0;
					
					m.setTranslation(right_translate - up_translate);
					break;
				}

				case ROTATE_ACTION:
				{
					delta_x *= ROTATE_FACTOR * 4.;
					delta_y *= ROTATE_FACTOR * 4.;
					float angle_total = fabs(delta_x) + fabs(delta_y);

					Vector3 rotation_axis = (camera.getLookUpVector() * delta_x / angle_total) +
																	(camera.getRightVector()  * delta_y / angle_total);

					m.rotate(Angle(angle_total, false), rotation_axis);
					break;
				}

				case ROTATE_CLOCKWISE_ACTION:
				default:
					delta_x *= ROTATE_FACTOR2;
					Vector3 rotation_axis = camera.getViewVector();
					m.rotate(Angle(delta_x, false), rotation_axis);
			}

			TransformationMessage* msg = new TransformationMessage;
			msg->setMatrix(m);
			notify_(msg);
		}


		void Scene::mouseReleaseEvent(QMouseEvent* e)
		{
			if (isAnimationRunning()) return;

			makeCurrent();

			mouse_button_is_pressed_ = false;

			// ============ picking mode ================
			if (current_mode_ == PICKING__MODE)
			{
				selectObjects_();
				// update will be done from MolecularControl!
				need_update_ = false; 			
			}
			else if (current_mode_ == ROTATE__MODE)
			{
				processRotateModeMouseEvents_(e);
 				light_settings_->updateFromStage();
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

		void Scene::initTimer()
		{
 			timer_.start(500); // ?????????????? currently doesnt work with QT 4.1
		}

		void Scene::timerSignal_()
		{
			info_string_ = "";

			if (mouse_button_is_pressed_ || getMainControl()->isBusy())
			{
				return;
			}

			info_point_ = QCursor::pos();
			info_point_ = mapFromGlobal(info_point_);

			if (!rect().contains(info_point_)) return;

			Position pos_x = info_point_.x();
			Position pos_y = info_point_.y();

			// if the mouse was at on other position 500 ms before, store position and abort

			if (pos_x != last_x_pos_ ||
					pos_y != last_y_pos_)
			{
				last_x_pos_ = pos_x;
				last_y_pos_ = pos_y;
				show_info_ = true;
				return;
			}

			if (!show_info_) return;

			// we wont show the info again until the mouse moved
			show_info_ = false;

			List<GeometricObject*> objects;

			// ok, do the picking, until we find something
			for (Position p = 0; p < 8; p++)
			{
				gl_renderer_.pickObjects1(pos_x - p, pos_y - p, pos_x + p, pos_y + p);
				renderView_(DIRECT_RENDERING);
				gl_renderer_.pickObjects2(objects);
				if (objects.size() != 0) break;
			}

			if (objects.size() == 0)
			{
				return;
			}

			// get the description
			String string;
			MolecularInformation info;

			GeometricObject* object = 0;
			List<GeometricObject*>::Iterator git = objects.begin();
			for (; git != objects.end(); git++)
			{
				// do we have a composite?
				Composite* composite = (Composite*) (**git).getComposite();
				if (composite == 0) continue;

				object = *git;

				info.visit(*composite);
				String this_string(info.getName());
				if (composite->getParent() != 0 &&
						RTTI::isKindOf<Residue>(*composite->getParent()))
				{
					info.visit(*composite->getParent());
					this_string = info.getName() + " : " + this_string;
				}

 				if (this_string == "UNKNOWN") continue;;

				if (string != "") string += ", ";
				string += this_string;
			}

			if (string == "") return;

			info_string_ = string;

			String string2 = String("Object at cursor is ") + string;

			if (getMainControl()->getStatusbarText() == string2) return;

			setStatusbarText(string2, false);

			QPoint diff(20, 20);
			if (pos_x > (Position) width() / 2) diff.setX(-20);
			if (pos_y > (Position) height() / 2) diff.setY(-20);
			info_point_ += diff;
			updateGL();
		}


#ifndef QT_NO_WHEELEVENT
		void Scene::wheelEvent(QWheelEvent *qmouse_event)
		{
			qmouse_event->accept();

			y_window_pos_new_ = (Position)(y_window_pos_old_ + (qmouse_event->delta() / 120 * mouse_wheel_sensitivity_));
			zoomSystem_();
			y_window_pos_old_ = y_window_pos_new_;
		}
#endif

		void Scene::keyPressEvent(QKeyEvent* e)
		{
			if (gl_renderer_.getStereoMode() != GLRenderer::NO_STEREO)
			{
				if ((e->key() == Qt::Key_Y && e->modifiers() == Qt::AltModifier) ||
						 e->key() == Qt::Key_Escape)
				{
					exitStereo();
				}

				// setting of eye and focal distance
				if (e->key() != Qt::Key_Left  &&
						e->key() != Qt::Key_Right &&
						e->key() != Qt::Key_Up    &&
						e->key() != Qt::Key_Down)
				{
					return;
				}

				// setting of eye distance
				if (e->key() == Qt::Key_Left ||
						e->key() == Qt::Key_Right)
				{
					float new_distance = stage_->getEyeDistance();

					float modifier;
					if (e->key() == Qt::Key_Left)
					{
						modifier = -0.1;
					}
					else
					{
						modifier = +0.1;
					}

					if (e->modifiers() == Qt::ShiftModifier)
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
					if (e->key() == Qt::Key_Down)
					{
						modifier = -1;
					}
					else
					{
						modifier = +1;
					}

					if (e->modifiers() == Qt::ShiftModifier)
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

		void Scene::setMode(ModeType mode)
			throw()
		{
			if 			(mode == ROTATE__MODE) 	rotateMode_();
			else if (mode == PICKING__MODE) pickingMode_();
			else if (mode == MOVE__MODE) 		moveMode_();
		}

		void Scene::rotateMode_()
		{
			if (current_mode_ == ROTATE__MODE) return;
			
			gl_renderer_.exitPickingMode();
			last_mode_ = current_mode_;
			current_mode_ = ROTATE__MODE;		
			setCursor(QCursor(Qt::SizeAllCursor));
			rotate_action_->setChecked(true);
			picking_action_->setChecked(false);
			move_action_->setChecked(false);
		}

		void Scene::pickingMode_()
		{
			if (current_mode_ == PICKING__MODE) return;
			
			gl_renderer_.enterPickingMode();
			last_mode_ = current_mode_;
			current_mode_ = PICKING__MODE;
			setCursor(QCursor(Qt::CrossCursor));
			rotate_action_->setChecked(false);
			picking_action_->setChecked(true);
			move_action_->setChecked(false);
		}

		void Scene::moveMode_()
		{
			if (current_mode_ == MOVE__MODE) return;
			
			gl_renderer_.exitPickingMode();
			last_mode_ = current_mode_;
			current_mode_ = MOVE__MODE;
			setCursor(QCursor(Qt::PointingHandCursor));
			rotate_action_->setChecked(false);
			picking_action_->setChecked(false);
			move_action_->setChecked(true);
		}

		void Scene::selectionPressed_()
		{
			x_window_pick_pos_first_ = x_window_pos_old_;
			y_window_pick_pos_first_ = y_window_pos_old_;
		}


		void Scene::selectionPressedMoved_()
		{
			Position x[2];
			Position y[2];

			if (x_window_pos_new_ < x_window_pick_pos_first_)
			{
				x[0] = x_window_pos_new_;
				x[1] = x_window_pick_pos_first_;
			}
			else
			{
				x[0] = x_window_pick_pos_first_;
				x[1] = x_window_pos_new_;
			}

			if (y_window_pos_new_ < y_window_pick_pos_first_)
			{
				y[0] = y_window_pos_new_;
				y[1] = y_window_pick_pos_first_;
			}
			else
			{
				y[0] = y_window_pick_pos_first_;
				y[1] = y_window_pos_new_;
			}

			QPoint p0 = mapFromGlobal(QPoint(x[0], y[0]));
			QPoint p1 = mapFromGlobal(QPoint(x[1], y[1]));

			rb_->setGeometry(QRect(p0, p1));
			rb_->show();
		}

		void Scene::showExportVRMLDialog()
		{
			String start = String(screenshot_nr_) + ".vrml";
			screenshot_nr_ ++;
			QFileDialog fd(0, "Export to a VRML file", getMainControl()->getWorkingDir().c_str(), "*.vrml");
			fd.selectFile(start.c_str());
			fd.setFileMode(QFileDialog::AnyFile);
			if (fd.exec() != QDialog::Accepted ||
					fd.selectedFiles().size() == 0)
			{
				return;
			}

			String filename = ascii(*fd.selectedFiles().begin());
			VRMLRenderer vrml(filename);

			if (exportScene(vrml))
			{
				setStatusbarText("Saved VRML to " + filename);
				setWorkingDirFromFilename_(filename);
				return;
			}
			

			setStatusbarText("Could not save VRML", true);
		}



		String Scene::exportPNG()
		{
			String filename = String("BALLView_screenshot" + String(screenshot_nr_) +".png");
			screenshot_nr_ ++;

			exportPNG(filename);

			return filename;
		}

		void Scene::showExportPNGDialog()
		{
			// first create the image, otherwise we get the dialog into the image!!!
			QImage image = grabFrameBuffer();

			String start = String(screenshot_nr_) + ".png";
			screenshot_nr_ ++;
			QFileDialog fd(0, "Export a screenshot to a PNG file", getMainControl()->getWorkingDir().c_str(),
										 "*.png");
			fd.selectFile(start.c_str());
			fd.setFileMode(QFileDialog::AnyFile);
			if (fd.exec() != QDialog::Accepted ||
					fd.selectedFiles().size() == 0)
			{
				return;
			}

			String file_name = ascii(*fd.selectedFiles().begin());
			if (!file_name.has('.')) file_name += ".png";

			bool ok = image.save(file_name.c_str(), "PNG");

			setWorkingDirFromFilename_(file_name);

			if (ok) setStatusbarText("Saved PNG to " + file_name);
			else 		setStatusbarText("Could not save PNG", true);
		}

		bool Scene::exportPNG(const String& filename)
		{
			makeCurrent();

			QImage image = grabFrameBuffer();
			bool ok = image.save(filename.c_str(), "PNG");

			setWorkingDirFromFilename_(filename);

			if (ok) setStatusbarText("Saved PNG to " + filename);
			else 		setStatusbarText("Could not save PNG", true);

			return ok;
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

		bool Scene::event(QEvent* e)
		{
			if (e->type() < QEvent::User) return QGLWidget::event(e);

			if (e->type() == (QEvent::Type)SCENE_EXPORTPNG_EVENT) 
			{  
				exportPNG();
				return true;
			}

			if (e->type() == (QEvent::Type)SCENE_EXPORTPOV_EVENT) 
			{  
				exportPOVRay();
				return true;
			}

			if (e->type() == (QEvent::Type)SCENE_SETCAMERA_EVENT) 
			{  
				setCamera(((SceneSetCameraEvent*) e)->camera);
				return true;
			}

			return false;
		}

		void Scene::switchShowWidget()
			throw()
		{
			if (window_menu_entry_ == 0) return;

			bool vis = isVisible();
			setVisible(!vis);
			window_menu_entry_->setChecked(vis);
		}


		void Scene::exitStereo()
			throw()
		{
			gl_renderer_.setStereoMode(GLRenderer::NO_STEREO);
			gl_renderer_.setSize(width(), height());

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gl_renderer_.initPerspective();
			glMatrixMode(GL_MODELVIEW);

			setFullScreen(false);

			no_stereo_action_->setChecked(true);
			active_stereo_action_->setChecked(false);
			dual_stereo_action_->setChecked(false);
		}

		void Scene::setFullScreen(bool state)
		{
			if (state)
			{
				last_state_ = getMainControl()->saveState();
				showNormal();  // needed on windows
				setParent(0);
				showFullScreen();
				update();
				return;
			}

			hide();
 			setParent((QWidget*)getMainControl());
			show();
			getMainControl()->setCentralWidget(this);
			getMainControl()->restoreState(last_state_);
			update();
		}

		void Scene::enterActiveStereo()
			throw()
		{
			gl_renderer_.setStereoMode(GLRenderer::ACTIVE_STEREO);
			setFullScreen(true);

			no_stereo_action_->setChecked(false);
			active_stereo_action_->setChecked(true);
			dual_stereo_action_->setChecked(false);
		}

		void Scene::enterDualStereo()
			throw()
		{
			gl_renderer_.setStereoMode(GLRenderer::DUAL_VIEW_STEREO);
			setFullScreen(true);

			no_stereo_action_->setChecked(false);
			active_stereo_action_->setChecked(false);
			dual_stereo_action_->setChecked(true);
		}

		void Scene::setCamera(const Camera& camera)
			throw()
		{
			stage_->getCamera() = camera;
			updateCamera_();
 			light_settings_->updateFromStage();
		}

		void Scene::clearRecordedAnimation()
			throw()
		{
			animation_points_.clear();
		}

		void Scene::startAnimation()
			throw()
		{
			if (!lockComposites()) return;
			record_animation_action_->setChecked(false);
			record_animation_action_->setEnabled(false);

			start_animation_action_->setChecked(false);
			cancel_animation_action_->setEnabled(true);

			animation_repeat_action_->setEnabled(false);
			animation_export_POV_action_->setEnabled(false);
			animation_export_PNG_action_->setEnabled(false);
			clear_animation_action_->setEnabled(false);

			if (animation_thread_ != 0) delete animation_thread_;
			animation_thread_ = new AnimationThread();
			animation_thread_->start();
		}

		void Scene::stopAnimation()
			throw()
		{
			stop_animation_ = true;
		}

		void Scene::animate_()
			throw()
		{
			bool export_PNG = animation_export_PNG_action_->isChecked();
			bool export_POV = animation_export_POV_action_->isChecked();
			bool repeat     = animation_repeat_action_->isChecked();

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
						animation_thread_->mySleep(50);

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
			while(!stop_animation_ && repeat);

			stop_animation_ = false;
			start_animation_action_->setEnabled(true);
			cancel_animation_action_->setEnabled(false);
			record_animation_action_->setEnabled(true);

			animation_repeat_action_->setEnabled(true);
			animation_export_POV_action_->setEnabled(true);
			animation_export_PNG_action_->setEnabled(true);
			clear_animation_action_->setEnabled(true);
			unlockComposites();
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
			VIEW::processDropEvent(e);
		}

		void Scene::dragEnterEvent(QDragEnterEvent* event)
		{
			 if (event->mimeData()->hasUrls()) event->acceptProposedAction();
		}


		bool Scene::stereoBufferSupportTest()
		{
			QGLFormat test_format(QGL::DepthBuffer | QGL::StereoBuffers | QGL::DoubleBuffer);
			QGLWidget* gl_test = new QGLWidget(test_format, 0);
			gl_test->makeCurrent();
			bool supports =  gl_test->isValid();
			delete gl_test;
			if (!supports) 
			{
				gl_format_ = (QGL::DepthBuffer | QGL::DoubleBuffer);
				gl_test = new QGLWidget(test_format, 0);
				gl_test->makeCurrent();
				supports =  gl_test->isValid();
				delete gl_test;
				if (!supports)
				{
					gl_format_ = QGLFormat(QGL::DepthBuffer);
				}
			}
			
			return supports;
		}

		void Scene::setPopupInfosEnabled(bool state)
		{
			if (state)
			{
				initTimer();
			}
			else
			{
				timer_.stop();
			}
		}

		void Scene::setVisible(bool state)
		{
			// only for Python needed
			QGLWidget::setVisible(state);
		}

		void Scene::updateGL()
		{
 			QGLWidget::updateGL();

#ifdef BALL_BENCHMARKING

			float ti = 100000.0 / (PreciseTime::now().getMicroSeconds() - time_.getMicroSeconds());

			if (ti < 0)
			{
				time_ = PreciseTime::now();
				return;
			}

			float fps = (ti + last_fps_) / 2.;
			String temp = createFloatString(fps, 1);
			last_fps_ = fps;

			if (fps < 10.0) temp = String(" ") + temp;

			if (!temp.has('.')) temp = temp + ".0";

			temp = String("FPS ") + temp;

			QPainter painter(this);

			ColorRGBA color = getStage()->getBackgroundColor().getInverseColor();

			painter.setBackgroundMode(Qt::OpaqueMode);
			painter.setPen(color.getQColor());
			painter.setBackgroundColor(getStage()->getBackgroundColor().getQColor());

			QPoint point(width() - 70, 20);
			painter.drawText(point, temp.c_str(), 0, -1);

			time_ = PreciseTime::now();
#endif
		}

	} // namespace VIEW
} // namespace BALL
