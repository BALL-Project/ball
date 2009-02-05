// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: scene.C,v 1.174.16.3 2007-04-11 12:06:25 amoll Exp $
//

#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/stage.h>
#include <BALL/VIEW/KERNEL/threads.h>
#include <BALL/VIEW/KERNEL/clippingPlane.h>

#include <BALL/VIEW/DIALOGS/setCamera.h>
#include <BALL/VIEW/DIALOGS/preferences.h>
#include <BALL/VIEW/DIALOGS/lightSettings.h>
#include <BALL/VIEW/DIALOGS/stageSettings.h>
#include <BALL/VIEW/DIALOGS/materialSettings.h>
#include <BALL/VIEW/DIALOGS/exportGeometryDialog.h>

#include <BALL/VIEW/DATATYPE/standardDatasets.h>

#include <BALL/VIEW/RENDERING/POVRenderer.h>
#include <BALL/VIEW/RENDERING/VRMLRenderer.h>
#include <BALL/VIEW/RENDERING/STLRenderer.h>

#include <BALL/VIEW/PRIMITIVES/simpleBox.h>
#include <BALL/VIEW/PRIMITIVES/box.h>
#include <BALL/VIEW/PRIMITIVES/label.h>
#include <BALL/VIEW/PRIMITIVES/sphere.h>
#include <BALL/VIEW/PRIMITIVES/tube.h>
#include <BALL/VIEW/PRIMITIVES/disc.h>
#include <BALL/VIEW/PRIMITIVES/line.h>

#include <BALL/SYSTEM/timer.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/SYSTEM/directory.h>
#include <BALL/MATHS/quaternion.h>

#include <BALL/STRUCTURE/geometricTransformations.h>
#include <BALL/STRUCTURE/geometricProperties.h>

#include <QtGui/qmenubar.h>
#include <QtGui/qprinter.h>
#include <QtGui/qprintdialog.h>
#include <QtGui/qpainter.h>
#include <QtGui/qimage.h>
#include <QtGui/qcursor.h>
#include <QtGui/qapplication.h>
#include <QtGui/QDesktopWidget>
#include <QtGui/QFileDialog>
#include <QtGui/QInputDialog>
#include <QtOpenGL/QGLPixelBuffer>
#include <QtGui/qmessagebox.h>


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
		Position Scene::vrml_nr_ = 100000;
		bool Scene::offscreen_rendering_ = true;
		QSize Scene::PNG_size_ = QSize(1500,1000);

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
#ifndef BALL_OS_DARWIN
																QGL::StereoBuffers 	| 
#endif
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
				gl_renderer_(new GLRenderer()),
				light_settings_(new LightSettings(this)),
				stage_settings_(new StageSettings(this)),
				material_settings_(new MaterialSettings(this)),
				animation_thread_(0),
				toolbar_(new QToolBar("3D View Controls", this)),
				mode_group_(new QActionGroup(this))
		{
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
				gl_renderer_(new GLRenderer),
				light_settings_(0),
				stage_settings_(0),
				animation_thread_(0),
				stop_animation_(false),
				content_changed_(true),
				want_to_use_vertex_buffer_(false),
				mouse_button_is_pressed_(false),
				preview_(false),
				use_preview_(true),
				show_fps_(false),
				toolbar_(new QToolBar("3D View Controls", this)),
				mode_group_(new QActionGroup(this)),
				volume_width_(FLT_MAX)
		{
#ifdef BALL_VIEW_DEBUG
			Log.error() << "new Scene (2) " << this << std::endl;
#endif
			setObjectName(name);
			// the widget with the MainControl
			registerWidget(this);
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
				gl_renderer_(new GLRenderer()),
				light_settings_(new LightSettings(this)),
				stage_settings_(new StageSettings(this)),
				material_settings_(new MaterialSettings(this)),
				animation_thread_(0),
				stop_animation_(false),
				toolbar_(new QToolBar("3D View Controls", this)),
				mode_group_(new QActionGroup(this))
		{
#ifdef BALL_VIEW_DEBUG
			Log.error() << "new Scene (3) " << this << std::endl;
#endif

			setObjectName(name);

			resize((Size) scene.gl_renderer_->getWidth(), 
						 (Size) scene.gl_renderer_->getHeight());

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
				delete gl_renderer_;

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
//   			qApp->processEvents(); ??????? AM
			makeCurrent();

			if (RTTI::isKindOf<RepresentationMessage>(*message)) 
			{
				RepresentationMessage* rm = RTTI::castTo<RepresentationMessage>(*message);
				Representation* rep = rm->getRepresentation();
				switch (rm->getType())
				{
					case RepresentationMessage::UPDATE:
					{
						RepresentationManager& pm = getMainControl()->getRepresentationManager();
						if (pm.startRendering(rep))
						{
							gl_renderer_->bufferRepresentation(*rep);
							pm.finishedRendering(rep);
						}
						break;
					}

					case RepresentationMessage::REMOVE:
						gl_renderer_->removeRepresentation(*rep);
						break;

					default:
						break;
				}

				content_changed_ = true;

				update(false);
				return;
			}

			if (RTTI::isKindOf<DatasetMessage>(*message))
			{
				DatasetMessage* rm = RTTI::castTo<DatasetMessage>(*message);
				if (!rm->isValid()) return;
				RegularData3DDataset* set = dynamic_cast<RegularData3DDataset*>(rm->getDataset());
				if (set == 0) return;

				switch (rm->getType())
				{
					case DatasetMessage::UPDATE:
					case DatasetMessage::REMOVE:
						gl_renderer_->removeTextureFor_(*set->getData());
						break;

					default:
						return;
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

			gl_renderer_->init(*this);
			gl_renderer_->initSolid();
			if (stage_->getLightSources().size() == 0) setDefaultLighting(false);
			gl_renderer_->updateCamera();
			gl_renderer_->enableVertexBuffers(want_to_use_vertex_buffer_);
 			stage_settings_->getGLSettings();
		}

		void Scene::paintGL()
		{
			time_ = PreciseTime::now();
			// cannot call update here, because it calls updateGL
   		renderView_(DISPLAY_LISTS_RENDERING);
			glFlush();

			if (info_string_ != "")
			{
				ColorRGBA c = stage_->getBackgroundColor().getInverseColor();
				QFont font;
				font.setPixelSize(16);
				font.setBold(true);
				glDisable(GL_LIGHTING);
				gl_renderer_->setColorRGBA_(c);
				renderText(info_point_.x(), info_point_.y(), info_string_.c_str(), font);
				glEnable(GL_LIGHTING);
			}

			renderGrid_();

			if (show_fps_)
			{
				float ti = 1000000.0 / (PreciseTime::now().getMicroSeconds() - time_.getMicroSeconds());

				if (ti < 0)
				{
					time_ = PreciseTime::now();
					return;
				}

				if (fps_.size() > 0)
				{
					if (BALL_ABS(*fps_.begin() - ti) > ti / 0.5)
					{
						fps_.clear();
					}
				}

				fps_.push_back(ti);
				if (fps_.size() > 10) fps_.pop_front();

				float fps = 0;
				list<float>::iterator lit = fps_.begin();
				for (; lit != fps_.end(); lit++)
				{
					fps += *lit;
				}

				fps /= fps_.size();

				String temp = createFloatString(fps, 1);

				if (fps < 10.0) temp = String(" ") + temp;
				if (fps < 100.0) temp = String(" ") + temp;

				if (!temp.has('.')) temp = temp + ".0";

				temp = String("FPS ") + temp;

				ColorRGBA color = getStage()->getBackgroundColor().getInverseColor();

				QFont font;
				font.setStyleHint(QFont::Courier, QFont::OpenGLCompatible);
				font.setPixelSize(16);
				font.setBold(true);
				gl_renderer_->setColorRGBA_(color);
				glDisable(GL_LIGHTING);
				QFontMetrics fm(font);
				QRect r = fm.boundingRect(temp.c_str());

				renderText(width() - 20 - r.width(), 20, temp.c_str(), font);
				glEnable(GL_LIGHTING);
			}
		}

		void Scene::resizeGL(int width, int height)
		{
			gl_renderer_->setSize(width, height);
			gl_renderer_->updateCamera();
			content_changed_ = true;
		}


		void Scene::renderView_(RenderMode mode)
			throw()
		{
			makeCurrent();

			if (use_preview_ && preview_) gl_renderer_->setAntialiasing(false);

			glDepthMask(GL_TRUE);
			if (gl_renderer_->getStereoMode() == GLRenderer::NO_STEREO)
			{
				glDrawBuffer(GL_BACK);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				renderRepresentations_(mode);
				content_changed_ = false;

				if (use_preview_ && preview_) gl_renderer_->setAntialiasing(true);

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

      float left  = -2.0 *gl_renderer_->getXScale() - 0.5 * stage_->getEyeDistance() * ndfl;
      float right =  2.0 *gl_renderer_->getXScale() - 0.5 * stage_->getEyeDistance() * ndfl;

			//================== draw first buffer =============
	    glMatrixMode(GL_PROJECTION);
			if (gl_renderer_->getStereoMode() == GLRenderer::DUAL_VIEW_STEREO)
			{
				gl_renderer_->setSize(width()/2, height());
				glLoadIdentity();
				glFrustum(left,right,
								-2.0 * gl_renderer_->getXScale() / 2, 
								 2.0 * gl_renderer_->getYScale(),
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
									-2.0 * gl_renderer_->getXScale(), 
									 2.0 * gl_renderer_->getYScale(),
									nearf,farf);
			}

			// draw models
      glMatrixMode(GL_MODELVIEW);
			if (gl_renderer_->getStereoMode() == GLRenderer::ACTIVE_STEREO)
			{
				glDrawBuffer(GL_BACK_RIGHT);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			}

			glPushMatrix();
			stereo_camera_.setViewPoint(old_view_point + diff);
			stereo_camera_.setLookAtPosition(old_look_at + diff);
			gl_renderer_->updateCamera(&stereo_camera_);
 			gl_renderer_->setLights(true);
			renderRepresentations_(mode);
			glPopMatrix();

			//================== draw second buffer =============
	    glMatrixMode(GL_PROJECTION);
      left  = -2.0 *gl_renderer_->getXScale() + 0.5 * stage_->getEyeDistance() * ndfl;
      right =  2.0 *gl_renderer_->getXScale() + 0.5 * stage_->getEyeDistance() * ndfl;

			if (gl_renderer_->getStereoMode() == GLRenderer::DUAL_VIEW_STEREO)
			{
				gl_renderer_->setSize(width()/2, height());
				glLoadIdentity();
				glFrustum(left,right,
								-2.0 * gl_renderer_->getXScale() / 2, 
								 2.0 * gl_renderer_->getYScale(),
								nearf,farf);
				glViewport(width() / 2, 0, width() / 2, height());
		  }
			else
			{
				glLoadIdentity();
				glFrustum(left,right,
									-2.0 * gl_renderer_->getXScale(), 
									 2.0 * gl_renderer_->getYScale(),
									nearf,farf);
			}


      glMatrixMode(GL_MODELVIEW);

			if (gl_renderer_->getStereoMode() == GLRenderer::ACTIVE_STEREO)
			{
				glDrawBuffer(GL_BACK_LEFT);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			}

			glPushMatrix();
			stereo_camera_.setViewPoint(old_view_point - diff);
			stereo_camera_.setLookAtPosition(old_look_at - diff);
			gl_renderer_->updateCamera(&stereo_camera_);
 			gl_renderer_->setLights(true);
			renderRepresentations_(DISPLAY_LISTS_RENDERING);
			glPopMatrix();
			content_changed_ = false;

			if (use_preview_ && preview_) gl_renderer_->setAntialiasing(true);
		}

		void Scene::renderRepresentations_(RenderMode mode)
			throw()
		{
			RepresentationManager& pm = getMainControl()->getRepresentationManager();

			if (volume_width_ != FLT_MAX)
			{
				const Camera& camera = stage_->getCamera();
				Vector3 n(-camera.getViewVector());
				n.normalize();
				Vector3 nr(camera.getRightVector());
				nr.normalize();
				Vector3 nu(camera.getLookUpVector());
				nu.normalize();
				ClippingPlane plane;

				for (Position p = 1; p < 6; p++)
				{
					if (p == 1)
					{
						plane.setPoint(camera.getLookAtPosition() - n * volume_width_);
						plane.setNormal(n);
					}
					else if (p == 2)
					{
						plane.setPoint(camera.getLookAtPosition() - nr * volume_width_);
						plane.setNormal(nr);
					}
					else if (p == 3)
					{
						plane.setPoint(camera.getLookAtPosition() + nr * volume_width_);
						plane.setNormal(-nr);
					}
					else if (p == 4)
					{
						plane.setPoint(camera.getLookAtPosition() - nu * volume_width_);
						plane.setNormal(nu);
					}
					else if (p == 5)
					{
						plane.setPoint(camera.getLookAtPosition() + nu * volume_width_);
						plane.setNormal(-nu);
					}

					Vector3 v = plane.getNormal();
					GLdouble planef[] ={v.x, v.y, v.z, plane.getDistance()};
					glClipPlane(GL_CLIP_PLANE0 + p, planef);
					glEnable(GL_CLIP_PLANE0 + p);
				}
			}
		
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
					gl_renderer_->renderSphere_(s);

					Tube t;
					t.setVertex1(pos);
					t.setVertex2(pos + diff);
					t.setColor(ColorRGBA(255,255,255));
					gl_renderer_->renderTube_(t);
				}
			}
			// -------------------------------------------------------------------
			
			
			// we draw all the representations in different runs, 
			// 1. normal reps
			// 2. transparent reps
			// 3. allways front
			for (Position run = 0; run < 3; run++)
			{
				if (run == 1)
				{
					for (Position p = 0; p < 20; p++)
					{
						if (volume_width_ != FLT_MAX) glDisable(GL_CLIP_PLANE0 + p);
					}

					// render inactive clipping planes
					for (plane_it = inactive_planes.begin(); plane_it != inactive_planes.end(); plane_it++)
					{
						gl_renderer_->renderClippingPlane_(**plane_it);
					}
				}

				RepresentationList::ConstIterator it = pm.getRepresentations().begin();
				for(; it != pm.getRepresentations().end(); it++)
				{
					Representation& rep = **it;
					if (rep.isHidden()) continue;

					if (run == 0)
					{
						// render all "normal" (non always front and non transparent models)
						if (!rep.hasProperty("DONT_CLIP") && (
								rep.getTransparency() != 0 ||
								rep.hasProperty(Representation::PROPERTY__ALWAYS_FRONT)))
						{
							continue;
						}
					}
					else if (run == 1)
					{
						// render all transparent models
						if (rep.hasProperty("DONT_CLIP") ||
								rep.getTransparency() == 0 ||
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
					
					// disable all clipping planes
					for (Position p = 0; p < rep_active_planes.size(); p++)
					{
						glDisable(rep_active_planes[p] + GL_CLIP_PLANE0);
					}

					// fill the stencil buffer
					glEnable(cap_nr + GL_CLIP_PLANE0);
					glEnable(GL_STENCIL_TEST);
					glClearStencil(0);
					glClear(GL_STENCIL_BUFFER_BIT);
					glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
					glStencilFunc(GL_ALWAYS, 0x0, 0xff);
					glStencilOp(GL_KEEP, GL_INVERT, GL_INVERT);
					render_(rep, mode);
					
					// disable all clipping planes
					for (Position p = 0; p < rep_active_planes.size(); p++)
					{
						glEnable(rep_active_planes[p] + GL_CLIP_PLANE0);
					}

					// render the Representation once again, this time with colors
					gl_renderer_->setColorRGBA_(ColorRGBA(0,1.0,0));
					glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
					glDisable(GL_STENCIL_TEST);
					render_(rep, mode);

					// render the capping plane
					glEnable(GL_STENCIL_TEST);
					glStencilFunc(GL_NOTEQUAL, 0x0, 0xff);

					ColorRGBA color = ClippingPlane::getCappingColor();
					bool transparent = (int)color.getAlpha() != 255;
					if (transparent) gl_renderer_->initTransparent();
					else 						 gl_renderer_->initSolid();

					glDisable(cap_nr + GL_CLIP_PLANE0);
					Disc d(Circle3(p, n, 400));
					d.setColor(color);
					gl_renderer_->render_(&d);
					
					glDisable(GL_STENCIL_TEST);
				}
			}

			if (text_ != "")
			{
				ColorRGBA c = stage_->getBackgroundColor().getInverseColor();
				QFont font;
				font.setPixelSize(font_size_);
				font.setBold(true);
				glDisable(GL_LIGHTING);
				gl_renderer_->setColorRGBA_(c);
				QFontMetrics fm(font);
				QRect r = fm.boundingRect(text_.c_str());
				renderText(width() -  (20 + r.width()), 
									 height() - (r.height() - 5),
									 text_.c_str(), font);
				glEnable(GL_LIGHTING);
			}
		}


		void Scene::render_(const Representation& repr, RenderMode mode)
			throw()
		{
			RepresentationManager& pm = getMainControl()->getRepresentationManager();
			Representation* rep = (Representation*)& repr;

			// preview mode:
			// if we have a model with only sticks and cylinders, decrease the rendering detail
			// and draw it directly instead of using display lists.
			// BUT: if the representation is currently rebuild, we can only draw buffered!
			bool used_preview = false;
			DrawingPrecision pbak = repr.getDrawingPrecision();
			if (use_preview_ && preview_)
			{
				if (repr.getModelType() >= MODEL_STICK &&
						repr.getModelType() <= MODEL_VDW &&
						pm.startRendering(rep))
				{
					rep->setDrawingPrecision(DRAWING_PRECISION_LOW);
					mode = DIRECT_RENDERING;
					used_preview = true;
				}
			}

			if (mode == DISPLAY_LISTS_RENDERING &&
					!repr.hasProperty("RENDER_DIRECT") && 
					repr.getDrawingMode() != DRAWING_MODE_TOON)
			{
				gl_renderer_->drawBuffered(repr);
				return;
			}

			if (!used_preview) 
			{
				// if we use previewing mode, the RepresentationManager was already notified above
				if (!pm.startRendering(rep))
				{
					// if Representation is to be rebuilded, but it is currently recalculated,
					// we can only draw it from the DisplayList
					gl_renderer_->drawBuffered(repr);
					return;
				}
			}

			if (mode == REBUILD_DISPLAY_LISTS)
			{
				gl_renderer_->bufferRepresentation(repr);
			}
			else //	DIRECT_RENDERING:
			{
				gl_renderer_->render(repr);
			}

			if (used_preview)
			{
				if (repr.getDrawingPrecision() != pbak)
				{
					// if previewing mode was used: reset the drawing precision
					(*(Representation*)&repr).setDrawingPrecision(pbak);
				}
			}

			pm.finishedRendering(rep);
		}

		/////////////////////////////////////////////////////////

		Vector3 Scene::getTranslationVector_(const Vector3& v)
		{
			const Camera& camera = stage_->getCamera();

			Vector3 vv = camera.getViewVector();
			float length = vv.getLength();
			if (!Maths::isZero(length)) vv /= length;

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
			gl_renderer_->pickObjects1((Position) p0.x(),
																(Position) p0.y(),
																(Position) p1.x(),
																(Position) p1.y());

			// draw the representations
			renderView_(DIRECT_RENDERING);

 			gl_renderer_->pickObjects2(objects);

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
			gl_renderer_->updateCamera();
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
			if (gl_renderer_->getStereoMode() == GLRenderer::NO_STEREO)
			{
				gl_renderer_->updateCamera();
 				gl_renderer_->setLights();
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
			gl_renderer_->setLights(true);
			light_settings_->updateFromStage();

			if (update_GL) renderView_(REBUILD_DISPLAY_LISTS);
		}

		void Scene::createCoordinateSystem()
			throw()
		{
			createCoordinateSystem_(false);
		}

		void Scene::createCoordinateSystemAtOrigin()
			throw()
		{
			createCoordinateSystem_(true);
		}

		void Scene::createCoordinateSystem_(bool at_origin)
			throw()
		{
			RepresentationManager& pm = getMainControl()->getRepresentationManager();

			Representation* rp = pm.createRepresentation();
			rp->setTransparency(90);

			const Camera& s = getStage()->getCamera();

			float size = 100;

			Vector3 x,y,z,p;

			if (at_origin)
			{
				p = Vector3(0,0,0);
				x = Vector3(1,0,0);
				y = Vector3(0,1,0);
				z = Vector3(0,0,1);
			}
			else
			{
				Vector3 v = s.getViewVector();
				v.normalize();
				
				p = s.getViewPoint() + (v * 25) - (s.getLookUpVector() * 5.);

				x = -v + s.getRightVector();
				x.normalize();
				y = -v - s.getRightVector();
				y.normalize();

				z = -(x % y);
 			}

			float delta = 0.001;
			Box* xp = new Box(p, x * size, z * size, delta);
			Box* yp = new Box(p, y * size, z * size, delta);
			Box* zp = new Box(p, x * size, y * size, delta);

			ColorRGBA bcolor(0,255,190,180);
			xp->setColor(bcolor);
			yp->setColor(bcolor);
			zp->setColor(bcolor);

			rp->insert(*xp);
			rp->insert(*yp);
 			rp->insert(*zp);

			ColorRGBA color;
			ColorRGBA color1(0,255,255,160);
			ColorRGBA color2(0,0,0,230);

			Vector3 p1[3], p2[3], d[3];
			p1[0].set(x); p2[0].set(z); d[0].set(y * delta);
			p1[1].set(y); p2[1].set(z); d[1].set(x * delta);
			p1[2].set(x); p2[2].set(y); d[2].set(z * delta);

			Vector3 px;
			for (Position i = 0; i <= size; i+=1)
			{
				if (i % 10 == 0) 
				{
					color = color2;
				}
				else
				{
					color = color1;
				}

				for (Position j = 0; j < 3; j++)
				{
					px = p + d[j];

					Line* line = new Line();
					line->setVertex1(px + p1[j] * i);
					line->setVertex2(px + p1[j] * i + p2[j] * size);
					line->setColor(color);
					rp->insert(*line);

					line = new Line();
					line->setVertex1(px + p2[j] * i);
					line->setVertex2(px + p2[j] * i + p1[j] * size);
					line->setColor(color);
					rp->insert(*line);

					px = p - d[j];

					line = new Line();
					line->setVertex1(px + p1[j] * i);
					line->setVertex2(px + p1[j] * i + p2[j] * size);
					line->setColor(color);
					rp->insert(*line);

					line = new Line();
					line->setVertex1(px + p2[j] * i);
					line->setVertex2(px + p2[j] * i + p1[j] * size);
					line->setColor(color);
					rp->insert(*line);
				}
			}

			rp->setProperty(Representation::PROPERTY__IS_COORDINATE_SYSTEM);

			// we have to add the representation in the GLRenderer manualy,
			// because the message wont arrive in Scene::onNotify
			gl_renderer_->bufferRepresentation(*rp);

			// notify GeometricControl
			notify_(new RepresentationMessage(*rp, RepresentationMessage::ADD));
		}


		bool Scene::exportScene(Renderer &er) const
			throw()
		{
			if (er.init(*stage_, (float) width(), (float) height()))
			{
				RepresentationList::ConstIterator it;
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
			// workaround: otherwise the variable might not get set
			window_menu_entry_->setChecked(!isHidden());

			ModularWidget::writePreferences(inifile);

			inifile.appendSection("EXPORT");
			inifile.insertValue("EXPORT", "POVNR", String(pov_nr_));
			inifile.insertValue("EXPORT", "VRMLNR", String(vrml_nr_));


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

			if (inifile.hasEntry("WINDOWS", getIdentifier() + "::on"))
			{
				setWidgetVisible(inifile.getValue("WINDOWS", getIdentifier() + "::on").toBool());
			}

			if (inifile.hasEntry("EXPORT", "POVNR"))
			{
				pov_nr_ = inifile.getValue("EXPORT", "POVNR").toUnsignedInt();
			}

			if (inifile.hasEntry("EXPORT", "VRMLNR"))
			{
				vrml_nr_ = inifile.getValue("EXPORT", "VRMLNR").toUnsignedInt();
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
			gl_renderer_->setLights(true);
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

			gl_renderer_->updateBackgroundColor(); 

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

				glFogf(GL_FOG_START, 10.0);
				float end = ((float) stage_->getFogIntensity());
				end = 400 - end;
				end += 20;
 				glFogf(GL_FOG_END, end);
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

			draw_grid_ = false;
			ignore_pick_ = false;

			Path path;
			String filename;

			main_control.initPopupMenu(MainControl::DISPLAY);

			mode_group_->setExclusive(true);

			create_coordinate_system_ = getMainControl()->initPopupMenu(MainControl::DISPLAY)->
				addMenu("Show Coordinate System");
			setMenuHint("Show a coordinate system");
			create_coordinate_system_->addAction("at origin", this, SLOT(createCoordinateSystemAtOrigin()));
			create_coordinate_system_->addAction("here", this, SLOT(createCoordinateSystem()));
			
			// ======================== ANIMATION ===============================================
			String help_url = "tips.html#animations";

			record_animation_action_ = insertMenuEntry(MainControl::DISPLAY_ANIMATION, "Record", this, SLOT(dummySlot()));
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

			animation_export_PNG_action_ = insertMenuEntry(MainControl::DISPLAY_ANIMATION, "Export PNG", this, SLOT(dummySlot()));
			setMenuHelp(help_url);
			animation_export_PNG_action_->setCheckable(true);

			animation_export_POV_action_ = insertMenuEntry(MainControl::DISPLAY_ANIMATION, "Export POV", this, SLOT(dummySlot()));
			setMenuHelp(help_url);
			animation_export_POV_action_->setCheckable(true);


			animation_repeat_action_ = insertMenuEntry(MainControl::DISPLAY_ANIMATION, "Repeat", this, SLOT(dummySlot()));
			setMenuHelp(help_url);
			animation_repeat_action_->setCheckable(true);

			main_control.insertPopupMenuSeparator(MainControl::DISPLAY);

			no_stereo_action_ = insertMenuEntry(MainControl::DISPLAY_STEREO, "No Stereo", this, SLOT(exitStereo()));
			no_stereo_action_->setCheckable(true);
			no_stereo_action_->setChecked(true);
			setMenuHelp("tips.html#3D");

			active_stereo_action_ = insertMenuEntry (
 					MainControl::DISPLAY_STEREO, "Shuttter Glasses", this, SLOT(enterActiveStereo()));
			setMenuHelp("tips.html#3D");
			active_stereo_action_->setCheckable(true);

			dual_stereo_action_ = insertMenuEntry (
 					MainControl::DISPLAY_STEREO, "Side by Side", this, SLOT(enterDualStereo()));
			setMenuHelp("tips.html#3D");
			dual_stereo_action_->setCheckable(true);

			getMainControl()->insertPopupMenuSeparator(MainControl::DISPLAY_VIEWPOINT);
			insertMenuEntry(MainControl::DISPLAY_VIEWPOINT, "&Store Viewpoint", this, SLOT(storeViewPoint()));
			setMenuHint("Store the current viewpoint");

			insertMenuEntry(MainControl::DISPLAY_VIEWPOINT, "&Restore Viewpoint", this, SLOT(restoreViewPoint()));
			setMenuHint("Restore the viewpoint");

			getMainControl()->insertPopupMenuSeparator(MainControl::DISPLAY_VIEWPOINT);

			insertMenuEntry(MainControl::DISPLAY_VIEWPOINT, "Show Vie&wpoint", this, SLOT(showViewPoint_()), Qt::CTRL+Qt::Key_W);
			setMenuHint("Print the coordinates of the current viewpoint");

			insertMenuEntry(MainControl::DISPLAY_VIEWPOINT, "Set Viewpoi&nt", this, SLOT(setViewPoint_()), Qt::CTRL+Qt::Key_N);
			setMenuHint("Move the viewpoint to the given coordinates");

			insertMenuEntry(MainControl::DISPLAY_VIEWPOINT, "Rese&t Camera", this, SLOT(resetCamera_()));
			setMenuHint("Reset the camera to the orgin (0,0,0)");

			getMainControl()->insertPopupMenuSeparator(MainControl::DISPLAY_VIEWPOINT);
			insertMenuEntry(MainControl::DISPLAY_VIEWPOINT, "Limit ViewVolume", this, SLOT(setupViewVolume()));
			insertMenuEntry(MainControl::DISPLAY_VIEWPOINT, "Reset ViewVolume", this, SLOT(disableViewVolumeRestriction()));

			QAction* screenshot_action = insertMenuEntry(MainControl::FILE_EXPORT, "PNG...", 
																	this, SLOT(showExportPNGDialog()), Qt::ALT + Qt::Key_P);
			setMenuHint("Export a PNG image file from the Scene");
			setIcon("screenshot.png", false);

			insertMenuEntry(MainControl::FILE_EXPORT, "POVRa&y scene", this, SLOT(exportPOVRay()), Qt::CTRL+Qt::Key_Y);
			setIcon("povray.png", false);
			setMenuHint("tips.html#povray");

 			insertMenuEntry(MainControl::FILE_EXPORT, "3D Prototyping Export", this, SLOT(showExportVRMLDialog()));
 			setMenuHint("Export a VRML or stl file from the scene");

			// ====================================== MODES =====================================
			main_control.insertPopupMenuSeparator(MainControl::DISPLAY);
			rotate_action_ =	insertMenuEntry(
					MainControl::DISPLAY, "&Rotate Mode", this, SLOT(rotateMode_()), Qt::CTRL+Qt::Key_R);
			setMenuHint("Switch to rotate/zoom mode");
			setMenuHelp("scene.html#rotate_mode");
			rotate_action_->setCheckable(true);
			setIcon("rotate.png", false);
			toolbar_actions_.push_back(rotate_action_);
			mode_group_->addAction(rotate_action_);

			picking_action_ = insertMenuEntry(MainControl::DISPLAY, "&Picking Mode", this, SLOT(pickingMode_()), Qt::CTRL+Qt::Key_P);
			setMenuHint("Switch to picking mode, e.g. to identify singe atoms or groups");
			setMenuHelp("scene.html#identify_atoms");
			setIcon("picking.png", false);
			picking_action_->setCheckable(true);
			toolbar_actions_.push_back(picking_action_);
			mode_group_->addAction(picking_action_);

			move_action_ = insertMenuEntry(MainControl::DISPLAY, "Move Mode", this, SLOT(moveMode_()));
			setMenuHint("Move selected items");
			setMenuHelp("molecularControl.html#move_molecule");
			setIcon("move.png", false);
			move_action_->setCheckable(true);
			toolbar_actions_.push_back(move_action_);
			mode_group_->addAction(move_action_);

			fullscreen_action_ = new QAction("Fullscreen", this);
			fullscreen_action_->setObjectName(fullscreen_action_->text());
			connect(fullscreen_action_, SIGNAL(triggered()), getMainControl(), SLOT(toggleFullScreen()));
			filename = path.find("graphics/fullscreen.png");
			fullscreen_action_->setIcon(QIcon(filename.c_str()));
			toolbar_actions_.push_back(fullscreen_action_);

			switch_grid_ = new QAction("Show ruler", this);
			switch_grid_->setObjectName(switch_grid_->text());
			connect(switch_grid_, SIGNAL(triggered()), this, SLOT(switchShowGrid()));
			switch_grid_->setCheckable(true);
			switch_grid_->setChecked(draw_grid_);
			filename = path.find("graphics/ruler.png");
			switch_grid_->setIcon(QIcon(filename.c_str()));
			toolbar_actions_.push_back(switch_grid_);

			toolbar_actions_.push_back(screenshot_action);

			insertMenuEntry(MainControl::FILE, "Print", this, SLOT(printScene()));

			window_menu_entry_ = insertMenuEntry(MainControl::WINDOWS, "Scene", this, SLOT(switchShowWidget()));
			window_menu_entry_->setCheckable(true);
			setMenuHelp("scene.html");

			setCursor(QCursor(Qt::ArrowCursor));

			setFocusPolicy(Qt::StrongFocus);
			registerForHelpSystem(this, "scene.html");

			toolbar_->setObjectName("3D toolbar");
			toolbar_->setIconSize(QSize(23,23));
			toolbar_->layout()->setMargin(2);
			toolbar_->layout()->setSpacing(2);
		}

		void Scene::checkMenu(MainControl& main_control)
			throw()
		{
			bool busy = main_control.compositesAreLocked();
			rotate_action_->setChecked(current_mode_ == ROTATE__MODE);
			picking_action_->setChecked(current_mode_ == PICKING__MODE);
			picking_action_->setEnabled(!busy);
			move_action_->setChecked(current_mode_ == MOVE__MODE);
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
			info_string_ = "";

			if (isAnimationRunning()) return;

			makeCurrent();

			mouse_button_is_pressed_ = true;
			ignore_pick_ = false;

			x_window_pos_old_ = e->globalX();
			y_window_pos_old_ = e->globalY();

			if (current_mode_ == ROTATE__MODE) 
			{
				preview_ = true;
				return;
			}

			if (current_mode_ == PICKING__MODE)
			{	
				if (e->modifiers() == Qt::NoModifier &&(
						e->buttons() == Qt::LeftButton ||
						e->buttons() == Qt::RightButton))
				{
					pick_select_ = (e->buttons() == Qt::LeftButton);
					selectionPressed_();
				}
				else
				{
					QPoint p = mapFromGlobal(QCursor::pos());
					pickParent_(p);
				}
			}
			else if (current_mode_ == MOVE__MODE)
			{
				preview_ = true;
			}
		}

		Index Scene::getMoveModeAction_(const QMouseEvent& e)
		{
			if (e.modifiers() == Qt::NoModifier)
			{
				if (e.buttons() == Qt::LeftButton) 	return TRANSLATE_ACTION;
				if (e.buttons() == Qt::MidButton) 	return ZOOM_ACTION;
				if (e.buttons() == Qt::RightButton) return ROTATE_ACTION;
			}
			else if (e.buttons() == Qt::LeftButton)
			{
				if (e.modifiers() == Qt::ShiftModifier) 											return ZOOM_ACTION;
				if (e.modifiers() == Qt::ControlModifier) 										return ROTATE_ACTION;
				if (e.modifiers() == Qt::ShiftModifier | Qt::ControlModifier) return ROTATE_CLOCKWISE_ACTION;
			}
			else if (e.buttons() == Qt::LeftButton | Qt::RightButton)
			{
				return ROTATE_CLOCKWISE_ACTION;
			}

			return ROTATE_ACTION;
		}

		void Scene::processRotateModeMouseEvents_(QMouseEvent* e)
		{
			if (x_window_pos_old_ == e->globalX() &&
					y_window_pos_old_ == e->globalY())
			{
				return;
			}

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
			Camera& camera = stage_->getCamera();

			x_window_pos_new_ = e->globalX();
			y_window_pos_new_ = e->globalY();

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
			preview_ = false;

			// ============ picking mode ================
			if (current_mode_ == PICKING__MODE)
			{
				if (ignore_pick_)
				{
					ignore_pick_ = false;

					return;
				}
				x_window_pos_new_ = e->globalX();
				y_window_pos_new_ = e->globalY();
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

		void Scene::showInfos()
		{
			info_string_ = "";

			if (getMainControl()->isBusy()) return;

			info_point_ = QCursor::pos();
			info_point_ = mapFromGlobal(info_point_);

			if (!rect().contains(info_point_) ||
					!lockComposites()) 
			{
				return;
			}

			Position pos_x = info_point_.x();
			Position pos_y = info_point_.y();

			// if the mouse was at on other position 500 ms before, store position and abort

			List<GeometricObject*> objects;

			// ok, do the picking, until we find something
			for (Position p = 0; p < 8; p++)
			{
				gl_renderer_->pickObjects1(pos_x - p, pos_y - p, pos_x + p, pos_y + p);
				renderView_(DIRECT_RENDERING);
				gl_renderer_->pickObjects2(objects);
				if (objects.size() != 0) break;
			}

			if (objects.size() == 0)
			{
				unlockComposites();
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

 				if (this_string == "UNKNOWN") continue;

				if (RTTI::isKindOf<Atom>(*composite))
				{
					this_string += "[T:";
					this_string += ((Atom*)composite)->getTypeName();
					this_string += ",FC:";
					this_string += String(((Atom*)composite)->getFormalCharge());
					this_string += "]";
				}

				if (string != "") string += ", ";
				string += this_string;
			}

			if (string == "") 
			{
				unlockComposites();
				return;
			}

			info_string_ = string;

			String string2 = String("Object at cursor is ") + string;

			if (getMainControl()->getStatusbarText() != string2) 
			{
				setStatusbarText(string2, false);

				QPoint diff(20, 20);
				if (pos_x > (Position) width() / 2) diff.setX(-20);
				if (pos_y > (Position) height() / 2) diff.setY(-20);
				info_point_ += diff;
				updateGL();
			}
			unlockComposites();
		}

		void Scene::wheelEvent(QWheelEvent *qmouse_event)
		{
			qmouse_event->accept();

			y_window_pos_new_ = (Position)(y_window_pos_old_ + (qmouse_event->delta() / 120 * mouse_wheel_sensitivity_));
			zoomSystem_();
			y_window_pos_old_ = y_window_pos_new_;
		}

		void Scene::keyPressEvent(QKeyEvent* e)
		{
			if (gl_renderer_->getStereoMode() == GLRenderer::NO_STEREO &&
			    e->key() == Qt::Key_Escape) 
			{
				switchToLastMode();
				return;
			}

			if (e->key() == Qt::Key_R)
			{
				setMode(ROTATE__MODE);
				return;
			}

			if (e->key() == Qt::Key_W)
			{
				setMode(MOVE__MODE);
				return;
			}

			if (e->key() == Qt::Key_Q)
			{
				setMode(PICKING__MODE);
				return;
			}


			if (gl_renderer_->getStereoMode() == GLRenderer::NO_STEREO) 
			{
				e->ignore();
				return;
			}

			if ((e->key() == Qt::Key_Y && e->modifiers() == Qt::AltModifier) ||
					 e->key() == Qt::Key_Escape)
			{
				exitStereo();
				return;
			}

			// setting of eye and focal distance
			if (e->key() != Qt::Key_Left  &&
					e->key() != Qt::Key_Right &&
					e->key() != Qt::Key_Up    &&
					e->key() != Qt::Key_Down)
			{
				e->ignore();
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
			
			gl_renderer_->exitPickingMode();
			last_mode_ = current_mode_;
			current_mode_ = ROTATE__MODE;		
			setCursor(QCursor(Qt::ArrowCursor));
			rotate_action_->setChecked(true);
			checkMenu(*getMainControl());
		}

		void Scene::pickingMode_()
		{
			if (current_mode_ == PICKING__MODE) return;
			
			gl_renderer_->enterPickingMode();
			last_mode_ = current_mode_;
			current_mode_ = PICKING__MODE;
			setCursor(QCursor(Qt::CrossCursor));
			picking_action_->setChecked(true);
			checkMenu(*getMainControl());
		}

		void Scene::moveMode_()
		{
			if (current_mode_ == MOVE__MODE) return;
			
			gl_renderer_->exitPickingMode();
			last_mode_ = current_mode_;
			current_mode_ = MOVE__MODE;
			setCursor(QCursor(Qt::SizeAllCursor));
			move_action_->setChecked(true);
			checkMenu(*getMainControl());
		}

		void Scene::selectionPressed_()
		{
			x_window_pick_pos_first_ = x_window_pos_old_;
			y_window_pick_pos_first_ = y_window_pos_old_;
		}


		void Scene::selectionPressedMoved_()
		{
			Position x0, x1, y0, y1;

			x0 = BALL_MIN(x_window_pos_new_, x_window_pick_pos_first_);
			x1 = BALL_MAX(x_window_pos_new_, x_window_pick_pos_first_);
			y0 = BALL_MIN(y_window_pos_new_, y_window_pick_pos_first_);
			y1 = BALL_MAX(y_window_pos_new_, y_window_pick_pos_first_);

			QPoint p0 = mapFromGlobal(QPoint(x0, y0));
			QPoint p1 = mapFromGlobal(QPoint(x1, y1));

			rb_->setGeometry(QRect(p0, p1));
			rb_->show();
		}

		//Opens a dialog in which parts of the scene can be exported as VRML or stl files
		void Scene::showExportVRMLDialog()
		{
			bool change = false;
			Position count = 0;
			ExportGeometryDialog ts;
			ts.setFilename(QString::number(vrml_nr_));
			ts.exec();

			bool *checked = ts.reps;
			bool ve = ts.export_vrml();
			bool se = ts.export_stl();

			//prepare the filename
			QString filename = ts.getFilename();
			QString vrml_end = ".vrml";
			QString stl_end = ".stl"; 
			filename.remove( vrml_end );
			filename.remove( stl_end );

			MainControl* mc = getMainControl();
			RepresentationManager& rm = mc->getRepresentationManager();
			RepresentationList::ConstIterator rit;
			
			if ( ve || se)
			{	
				if ( ts.split() )
				//every representation gets its own file
				{
				//at the beginning all reps are hidden (see "saving base stats")
				count = 0;
				//number of filepartition
				Position partCounter = 0;

				//now all representations are hidden. in the next iteration we show one that was not hidden before and print
				rit = rm.getRepresentations().begin();
				for (; rit != rm.getRepresentations().end(); rit++)
				{
					if(checked[count])
					{
						//show one, print all (meaning just the one)	
						(*rit)->setHidden(false);

						QString vtemp = filename;
						
						if (ve)
						{
							vtemp.append("_");
							vtemp.append(QString::number(partCounter));
							vtemp.append(".vrml");
							VRMLRenderer vrml(ascii(vtemp));
							if ( exportScene(vrml) )
							{
								change = true;
							}
						}
						if (se)
						{
							vtemp = filename;
							vtemp.append("_");
							vtemp.append(QString::number(partCounter));
							vtemp.append(".stl");
							STLRenderer stl(ascii(vtemp));
							if ( exportScene(stl) )
							{
								change = true;
							}						}	

						//hide again (for all other reps)
						(*rit)->setHidden(true);
						partCounter++;	
					}
					count++;	
				}
			}
				else
				//print in single file case
				{
					count = 0;

					//make all checked representations visible
					rit = rm.getRepresentations().begin();
					for (; rit != rm.getRepresentations().end(); rit++)
					{
						if(checked[count])
						{
							(*rit)->setHidden(false);
						}
						count++;
					}								

					QString vtemp = filename;				

					//export as requested
					if (ve)
					{
						vtemp.append(".vrml");
						VRMLRenderer vrml(ascii(vtemp));
						if ( exportScene(vrml) )
						{
							change = true;
						}
					}
					if (se)
					{
						vtemp = filename;
						vtemp.append(".stl");
						STLRenderer stl(ascii(vtemp));
						if ( exportScene(stl) )
						{
							change = true;
						}
					}	
				}
			}

			//it is very important to restore the representations just when a basestats array was created
			//as he is only created if an export was made whenever the export is started the "change" boolean is set true
			if ( change )
			{	
				//now we have to restore the basestats:
				bool *base = ts.basestats;
				count = 0;
//				rit = mc->getRepresentationManager().representations_.begin();
//				for (; !(rit == mc->getRepresentationManager().representations_.end()); rit++)
				rit = rm.getRepresentations().begin();
				for (; rit != rm.getRepresentations().end(); rit++)
				{
					if(base[count])
					{
						(*rit)->setHidden(false);
					}
					else
					{
						(*rit)->setHidden(true);
					}	
					count ++;
				}		
				setWorkingDirFromFilename_(ascii(filename));
				vrml_nr_ ++;
			}
			getMainControl()->redrawAllRepresentations();			
		}

		void Scene::printScene()
		{
			QPrinter printer;
			QPrintDialog dialog(&printer, this);
			if (!dialog.exec()) return;
			
			setStatusbarText("printing..");

			QPainter p;
			if(!p.begin(&printer)) return; 

			QImage pic = grabFrameBuffer();
			p.drawImage(0,0, pic);	
			p.end();

			setStatusbarText("finished printing");
		}


		String Scene::exportPNG()
		{
			String filename = String("BALLView_screenshot" + String(screenshot_nr_) +".png");
			screenshot_nr_ ++;

			exportPNG(filename);
			Log.info() << "Exporting PNG to " 
								 << Directory().getPath() << FileSystem::PATH_SEPARATOR
								 << filename << std::endl;

			return filename;
		}

		void Scene::showExportPNGDialog()
		{
			String start = String(screenshot_nr_) + ".png";
			screenshot_nr_ ++;
			QString qresult = QFileDialog::getSaveFileName(
												0,
												"Export POVRay File",
												(getWorkingDir() + String(FileSystem::PATH_SEPARATOR) + start).c_str(),
												"*.png");

			if (qresult == QString::null) return;

			String result = ascii(qresult);
			if (!offscreen_rendering_)
			{
				update(false);
				getMainControl()->processEvents(9999);
			}
			exportPNG(result);
		}

		bool Scene::exportPNG(const String& filename)
		{
			makeCurrent();
			QImage image;

			if (offscreen_rendering_)
			{
				glFlush();
				QGLFormat f = format();
				f.setSampleBuffers(true);
				Log.info() << "Starting offscreen rendering with " << PNG_size_.width() << " * " << PNG_size_.height() << std::endl;
				QGLPixelBuffer pbuffer(PNG_size_, f,this );
				bool pb = pbuffer.makeCurrent();
				if (!pb)
				{
					setStatusbarText("Offscreen rendering not supported, using normal screenshots", true);
					image = grabFrameBuffer();
				}
				else
				{
					gl_renderer_->init(*this);
					gl_renderer_->initSolid();
					gl_renderer_->setSize(PNG_size_.width(), PNG_size_.height());
					gl_renderer_->updateCamera();
					gl_renderer_->setLights(true);
					gl_renderer_->enableVertexBuffers(want_to_use_vertex_buffer_);
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					renderRepresentations_(DIRECT_RENDERING);
					glFlush();
					image = pbuffer.toImage();
					makeCurrent();
					gl_renderer_->setSize(width(), height());
					gl_renderer_->updateCamera();
					gl_renderer_->setLights(true);
				}
			}
			else
			{
				image = grabFrameBuffer();
			}
			bool ok = image.save(filename.c_str(), "PNG");

			setWorkingDirFromFilename_(filename);

			if (ok) setStatusbarText("Saved PNG to " + filename);
			else 		setStatusbarText("Could not save PNG", true);

			return ok;
		}

		void Scene::exportNextPOVRay()
		{
			String filename = String("BALLView_pov_" + String(pov_nr_) +".pov");

			POVRenderer pr(filename);
			bool result = exportScene(pr);
			pov_nr_ ++;

			if (result) setStatusbarText("Saved POVRay to " + filename);
			else 				setStatusbarText("Could not save POVRay to " + filename);
		}

		void Scene::exportPOVRay()
		{
			QString qresult = QFileDialog::getSaveFileName(
												0,
												"Export POVRay File",
												getWorkingDir().c_str(),
												"*.pov");

			if (qresult == QString::null) return;

			String result = ascii(qresult);

			if (!result.hasSuffix(".pov"))
			{
				result += ".pov";
			}

			bool ok = false;

			try
			{
				POVRenderer pr(result);
				if (exportScene(pr)) ok = true;
			}
			catch(...)
			{
			}

			if (!ok)
			{
				setStatusbarText("Could not export POV to " + result, true);
			}
			else
			{
				setStatusbarText("Exported POV to " + result);
				setWorkingDirFromFilename_(result);
			}
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
			no_stereo_action_->setChecked(true);
			active_stereo_action_->setChecked(false);
			dual_stereo_action_->setChecked(false);

			if (gl_renderer_->getStereoMode() == GLRenderer::NO_STEREO) return;

			gl_renderer_->setStereoMode(GLRenderer::NO_STEREO);
			gl_renderer_->setSize(width(), height());

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gl_renderer_->initPerspective();
			glMatrixMode(GL_MODELVIEW);

			setFullScreen(false);
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
			gl_renderer_->setStereoMode(GLRenderer::ACTIVE_STEREO);
			setFullScreen(true);

			no_stereo_action_->setChecked(false);
			active_stereo_action_->setChecked(true);
			dual_stereo_action_->setChecked(false);
		}

		void Scene::enterDualStereo()
			throw()
		{
			gl_renderer_->setStereoMode(GLRenderer::DUAL_VIEW_STEREO);
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
			animation_thread_->setScene(this);
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
						animation_thread_->mySleep(30);

						camera.setViewPoint(camera.getViewPoint() - diff_viewpoint);
						camera.setLookUpVector(camera.getLookUpVector() - diff_up);
						camera.setLookAtPosition(camera.getLookAtPosition() - diff_look_at);

						SceneMessage* msg = new SceneMessage(SceneMessage::UPDATE_CAMERA);
						Stage stage(*getStage());
						stage.setCamera(camera);
						msg->setStage(stage);
						qApp->postEvent(getMainControl(), new MessageEvent(msg));

						if (export_PNG)
						{
							SceneMessage* msg = new SceneMessage(SceneMessage::EXPORT_PNG);
							qApp->postEvent(getMainControl(), new MessageEvent(msg));
						}

						if (export_POV)
						{
							SceneMessage* msg = new SceneMessage(SceneMessage::EXPORT_POVRAY);
							qApp->postEvent(getMainControl(), new MessageEvent(msg));
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
			setMode(last_mode_);
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

		void Scene::setWidgetVisible(bool state)
		{
			// only for Python needed
			QGLWidget::setVisible(state);
		}

		void Scene::updateGL()
		{
 			QGLWidget::updateGL();

		}

		void Scene::setOffScreenRendering(bool enabled, Size factor)
		{
			offscreen_rendering_ = enabled;
			if (enabled) 
			{
				Size w = width() * factor;
				Size h = height() * factor;
				Size max = BALL_MAX(w, h);
				Size min = BALL_MIN(w, h);
				if (max < 4096)
				{
					PNG_size_ = QSize(w, h);
				}
				else
				{
					Size f = (Size) (4095. * (float) min / (float) max);
					if (w < h)
					{
						PNG_size_ = QSize(f, 4095);
					}
					else
					{
						PNG_size_ = QSize(4095, f);
					}
				}
			}
		}

		void Scene::showText(const String& text, Size font_size) 
		{ 
			text_ = text; 
			font_size_= font_size;
			update();
		}

		void Scene::addToolBarEntries(QToolBar* tb)
		{
			toolbar_->addActions(toolbar_actions_);
			toolbar_->insertSeparator(fullscreen_action_);
			getMainControl()->addToolBar(Qt::TopToolBarArea, toolbar_);
			ModularWidget::addToolBarEntries(tb);
			getMainControl()->initPopupMenu(MainControl::WINDOWS)->addAction(toolbar_->toggleViewAction());
		}

		void Scene::switchShowGrid()
		{
			draw_grid_ = !draw_grid_;
			switch_grid_->setChecked(draw_grid_);
			update();
		}


		void Scene::renderGrid_()
		{
			if (!draw_grid_) return;

			const Camera& s = getStage()->getCamera();
			Vector3 v = s.getViewVector();
			v.normalize();
			const Vector3 x = s.getRightVector();
			const Vector3 y = s.getLookUpVector();
			float delta = 0.001;
			float size = 50;

			gl_renderer_->initTransparent();

			Vector3 p = get3DPosition_((Index)(width() / 2.0), (Index)(height() / 2.0)) - x * size / 2.0 - y * size / 2.0;
			Box xp(p, x * size, y * size, delta);
			xp.setColor(ColorRGBA(0,255,190,90));
			gl_renderer_->render_(&xp);

			ColorRGBA color1(255,255,255,255);
			ColorRGBA color2(0,0,0,230);
			Line line;
			p -= v * delta;

			for (Position i = 0; i <= size; i+=1)
			{
				if (i % 10 == 0) line.setColor(color2);
				else  					 line.setColor(color1);

				line.setVertex1(p + x * i);
				line.setVertex2(p + x * i + y * size);
				gl_renderer_->render_(&line);

				line.setVertex1(p + y * i);
				line.setVertex2(p + y * i + x * size);
				gl_renderer_->render_(&line);
			}

			gl_renderer_->initSolid();
		}


		// Convert 2D screen coordinate to 3D coordinate on the view plane
		Vector3 Scene::get3DPosition_(int x, int y)
		{
			// 	Scale variables for Frustum
			double xs_ = width();
			double ys_ = height(); 

			mapViewplaneToScreen_();

			// vectors for arithmetics
			// TODO: give sensible names!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			Vector3 p_(0., 0., 0.);      // vector look_at ray ----> insertion ray cutting the nearplane
			Vector3 la_m_d_(0., 0., 0.); // look_at vector ray cutting the near plane
			Vector3 la_m_v_(0., 0., 0.); // look_at vector ray cutting the near plane
			Vector3 s_(0., 0., 0.);      // vector look_at_ray ----> insertion ray cutting viewing plane
			Vector3 k_(0., 0., 0.);      // vector of insertionpoint in the viewing volume

			// determine the vector/look_at ray : camera --> lookAt cuts the near plane
			la_m_d_=Vector3(  near_left_bot_
					+( (near_right_bot_ - near_left_bot_)*0.5 )
					+( (near_left_top_  - near_left_bot_)*0.5 )
					);	

			// determine the vector look_at point--->insertion_ray cutting the near plane 
			p_=Vector3((   near_left_top_  //c
						+ ( x / (float)xs_ * (near_right_bot_ - near_left_bot_) )  //b-a
						- ( y / (float)ys_ * (near_left_top_  - near_left_bot_) )  //c-a
						)
					- la_m_d_ );

			// determine the vector look_at_ray ----> insertion ray cutting viewing plane
			s_= Vector3(   ( ( getStage()->getCamera().getLookAtPosition() - getStage()->getCamera().getViewPoint() ).getLength()
						/ (la_m_d_ -  getStage()->getCamera().getViewPoint()).getLength()) 
					* p_ );

			// vector of insertionpoint in the viewing volume
			k_=Vector3( getStage()->getCamera().getLookAtPosition() + s_ );		

			return k_;
		}	

		bool Scene::mapViewplaneToScreen_()
		{
			// matrix for the Projection matrix 	
			GLdouble projection_matrix[16];
			// matrix for the Modelview matrix
			GLdouble modelview_matrix[16];

			// variables for definition of projection matrix
			float near_=0, left_=0, right_=0, bottom_ =0, top_=0; 

			// take the Projection matrix	
			glMatrixMode(GL_PROJECTION);
			glGetDoublev(GL_PROJECTION_MATRIX, projection_matrix);
			glMatrixMode(GL_MODELVIEW);
			glGetDoublev(GL_MODELVIEW_MATRIX, modelview_matrix); 

			// determine the projection variables
			if(projection_matrix[0]==0. || projection_matrix[5]==0. || projection_matrix[10]==1.)
			{	
				Log.error() << "Projection variables equal zero! " << endl;
				return false;
			}	
			near_   = projection_matrix[14]/(projection_matrix[10]-1);
			left_   = projection_matrix[14]*(projection_matrix[8]-1) / (projection_matrix[0]*(projection_matrix[10]-1));
			right_  = projection_matrix[14]*(projection_matrix[8]+1) / (projection_matrix[0]*(projection_matrix[10]-1));
			bottom_ = projection_matrix[14]*(projection_matrix[9]-1) / (projection_matrix[5]*(projection_matrix[10]-1));
			top_    = projection_matrix[14]*(projection_matrix[9]+1) / (projection_matrix[5]*(projection_matrix[10]-1));

			// we have to move all points of the viewing volume with the inverted Modelview matrix 
			Matrix4x4 mod_view_mat_(modelview_matrix[0], modelview_matrix[4], modelview_matrix[8], modelview_matrix[12],
					modelview_matrix[1], modelview_matrix[5], modelview_matrix[9], modelview_matrix[13],
					modelview_matrix[2], modelview_matrix[6], modelview_matrix[10], modelview_matrix[14],
					modelview_matrix[3], modelview_matrix[7], modelview_matrix[11],	modelview_matrix[15]);


			Matrix4x4 inverse_mod_view_mat_;
			mod_view_mat_.invert(inverse_mod_view_mat_);

			// determine the nearplane vectors
			near_left_bot_ = Vector3(left_,  bottom_, near_*-1.); //a
			near_right_bot_= Vector3(right_, bottom_, near_*-1.); //b
			near_left_top_ = Vector3(left_,  top_,    near_*-1.); //c	

			near_left_bot_  = inverse_mod_view_mat_*near_left_bot_;
			near_right_bot_ = inverse_mod_view_mat_*near_right_bot_;
			near_left_top_  = inverse_mod_view_mat_*near_left_top_;

			return true;
		}


		void Scene::mouseDoubleClickEvent(QMouseEvent*)
		{
			if (getMainControl()->isBusy()) return;

			if (current_mode_ == PICKING__MODE)
			{
				QPoint p = mapFromGlobal(QCursor::pos());
				pickParent_(p);
			}
			else
			{
				if (current_mode_ == ROTATE__MODE)
				{
					showInfos();
				}
			}
		}

		void Scene::pickParent_(QPoint p)
		{
			ignore_pick_ = true;
			List<GeometricObject*> objects;
			gl_renderer_->pickObjects1((Position) p.x(), (Position) p.y(), 
																(Position) p.x(), (Position) p.y());
			renderView_(DIRECT_RENDERING);
			gl_renderer_->pickObjects2(objects);

			if (objects.size() == 0) return;
		
			Composite* composite = 	(Composite*)(**objects.begin()).getComposite();
			if (composite == 0) return;

			Composite* to_select = 0;
			Atom* atom = dynamic_cast<Atom*>(composite);
			if (atom != 0)
			{
				to_select = atom->getParent();
			}
			else
			{
				Bond* bond = dynamic_cast<Bond*>(composite);
				if (bond!= 0) 
				{
					to_select = (Composite*)bond->getFirstAtom()->getParent();
				}
				else
				{
					to_select = composite;
				}
			}

			if (to_select != 0)
			{
				if (to_select->isSelected())
				{
					getMainControl()->deselectCompositeRecursive(to_select, true);
				}
				else
				{
					getMainControl()->selectCompositeRecursive(to_select, true);
				}
				getMainControl()->update(*to_select, true);
			}
		}

		void Scene::disableViewVolumeRestriction()
		{
			volume_width_ = FLT_MAX;
		}

		void Scene::setupViewVolume()
		{
			bool ok;
			float value = QInputDialog::getDouble(this, "Setup view volume",
																						"Enter volume length:", 
																						20, 5, 200, 1, &ok);
			if (!ok) return;

			volume_width_ = value;
			update(false);
		}

		void Scene::restoreViewPoint()
		{
			getStage()->setCamera(stored_camera_);
			updateCamera_();
		}

		void Scene::storeViewPoint()
		{
			stored_camera_ = getStage()->getCamera();
		}

		void Scene::setGLRenderer(GLRenderer& renderer)
		{
			delete gl_renderer_;
			gl_renderer_ = &renderer;
		}

		void AnimationThread::mySleep(Size msec) 
		{
			msleep(msec);
			while (scene_ != 0 && scene_->isUpdateRunning())
			{
				msleep(10);
			}
		}

	} // namespace VIEW
} // namespace BALL
