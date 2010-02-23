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
#include <BALL/VIEW/KERNEL/shortcutRegistry.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/iconLoader.h>

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
#include <BALL/VIEW/RENDERING/tilingRenderer.h>
#include <BALL/VIEW/RENDERING/glOffscreenTarget.h>

#include <BALL/VIEW/PRIMITIVES/simpleBox.h>
#include <BALL/VIEW/PRIMITIVES/box.h>
#include <BALL/VIEW/PRIMITIVES/label.h>
#include <BALL/VIEW/PRIMITIVES/sphere.h>
#include <BALL/VIEW/PRIMITIVES/tube.h>
#include <BALL/VIEW/PRIMITIVES/disc.h>
#include <BALL/VIEW/PRIMITIVES/line.h>

#include <BALL/VIEW/INPUT/transformationEvent6D.h>
#include <BALL/VIEW/INPUT/headTrackingEvent.h>
#include <BALL/VIEW/INPUT/motionTrackingEvent.h>
#include <BALL/VIEW/INPUT/buttonEvent.h>

#include <BALL/SYSTEM/timer.h>
#include <BALL/SYSTEM/directory.h>
#include <BALL/MATHS/quaternion.h>

#include <BALL/STRUCTURE/geometricTransformations.h>
#include <BALL/STRUCTURE/geometricProperties.h>

#include <QtGui/QMenuBar>
#include <QtGui/QPrinter>
#include <QtGui/QPrintDialog>
#include <QtGui/QPainter>
#include <QtGui/QCursor>
#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#include <QtGui/QFileDialog>
#include <QtGui/QInputDialog>
#include <QtOpenGL/QGLPixelBuffer>
#include <QtGui/QMessageBox>
#include <QtGui/QImage>

#include <BALL/VIEW/WIDGETS/datasetControl.h>
#include <BALL/VIEW/DATATYPE/colorMap.h>

#include <BALL/VIEW/RENDERING/bufferedRenderer.h>

#ifdef BALL_HAS_RTFACT
    
#include <BALL/VIEW/RENDERING/glRenderWindow.h>
//#include <BALL/VIEW/RENDERING/RENDERERS/cudaVolumeRenderer.h>    
#include <BALL/VIEW/RENDERING/RENDERERS/rtfactRenderer.h>    

#endif

//#define BALL_VIEW_DEBUG

//#define BALL_BENCHMARKING


using std::endl;
using std::istream;

namespace BALL
{
	namespace VIEW
	{

#ifdef BALL_HAS_RTFACT    
		//typedef CudaVolumeRenderer t_RaytracingRenderer;
		typedef RTfactRenderer t_RaytracingRenderer;
		typedef GLRenderWindow t_RaytracingWindow;    
#endif

		Position Scene::screenshot_nr_ = 100000;
		Position Scene::pov_nr_ = 100000;
		Position Scene::vrml_nr_ = 100000;
		bool Scene::offscreen_rendering_ = true;

		// ###############CONSTRUCTORS,DESTRUCTORS,CLEAR###################

		// values for mouse-sensitivity 
		float Scene::mouse_sensitivity_ = 5;
		float Scene::mouse_wheel_sensitivity_ = 5;
		bool  Scene::show_light_sources_ = false;
		float Scene::animation_smoothness_ = 2;
		float Scene::downsampling_factor_ = 1;

		#define  ROTATE_FACTOR    50.
		#define  ROTATE_FACTOR2   50.
		#define  TRANSLATE_FACTOR 4.
    #define  ZOOM_FACTOR      7.

		Scene::Scene()
			:	QWidget(),
				ModularWidget("<Scene>"),
				update_running_(false),
				rb_(new QRubberBand(QRubberBand::Rectangle, this)),
				stage_(new Stage()),
				renderers_(),
				gl_renderer_(new GLRenderer()),
#ifdef BALL_HAS_RTFACT
				rt_renderer_(new t_RaytracingRenderer()),
#endif
				light_settings_(new LightSettings(this)),
				material_settings_(new MaterialSettings(this)),
				animation_thread_(0),
				toolbar_view_controls_(new QToolBar("3D View Controls", this)),
				mode_group_(new QActionGroup(this)),
				main_display_(new GLRenderWindow(this)),
				stereo_left_eye_(-1),
				stereo_right_eye_(-1)
		{
			stage_settings_=new StageSettings(this);
#ifndef BALL_HAS_RTFACT
			renderers_.push_back(boost::shared_ptr<RenderSetup>(new RenderSetup(gl_renderer_, main_display_, this, stage_)));
#else
			renderers_.push_back(boost::shared_ptr<RenderSetup>(new RenderSetup(&*rt_renderer_, main_display_, this, stage_)));
#endif

			init();
			renderers_[0]->resize(width(), height());
			renderers_[0]->start();

			setAcceptDrops(true);
#ifdef BALL_VIEW_DEBUG
			Log.error() << "new Scene (1) " << this << std::endl;
#endif
		}

		Scene::Scene(QWidget* parent_widget, const char* name, Qt::WFlags w_flags)
			:	QWidget(parent_widget, w_flags),
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
				renderers_(),
				gl_renderer_(new GLRenderer),
#ifdef BALL_HAS_RTFACT
				rt_renderer_(new t_RaytracingRenderer()),
#endif
				light_settings_(new LightSettings(this)),
				material_settings_(new MaterialSettings(this)),
				animation_thread_(0),
				stop_animation_(false),
#ifdef BALL_HAS_RTFACT	
			  continuous_loop_(false),
#endif
				want_to_use_vertex_buffer_(false),
				mouse_button_is_pressed_(false),
				preview_(false),
				use_preview_(true),
				show_fps_(false),
				toolbar_view_controls_(new QToolBar("3D View Controls", this)),
				mode_group_(new QActionGroup(this)),
				main_display_(new GLRenderWindow(this)),
				stereo_left_eye_(-1),
				stereo_right_eye_(-1)
		{
			stage_settings_=new StageSettings(this);
#ifdef BALL_VIEW_DEBUG
			Log.error() << "new Scene (2) " << this << std::endl;
#endif

#ifndef BALL_HAS_RTFACT
			renderers_.push_back(boost::shared_ptr<RenderSetup>(new RenderSetup(gl_renderer_, main_display_, this, stage_)));
#else
			renderers_.push_back(boost::shared_ptr<RenderSetup>(new RenderSetup(&*rt_renderer_, main_display_, this, stage_)));
#endif

			// the widget with the MainControl
			registerWidget(this);

			setObjectName(name);
			setAcceptDrops(true);

			init();
			renderers_[0]->resize(width(), height());
			renderers_[0]->start();
		}

		Scene::Scene(const Scene& scene, QWidget* parent_widget, const char* name, Qt::WFlags w_flags)
			:	QWidget(parent_widget, w_flags),
				ModularWidget(scene),
				system_origin_(scene.system_origin_),
				update_running_(false),
				rb_(new QRubberBand(QRubberBand::Rectangle, this)),
				stage_(new Stage(*scene.stage_)),
				renderers_(),
				gl_renderer_(new GLRenderer()),
#ifdef BALL_HAS_RTFACT
				rt_renderer_(new t_RaytracingRenderer()),
#endif
				light_settings_(new LightSettings(this)),
				material_settings_(new MaterialSettings(this)),
				animation_thread_(0),
				stop_animation_(false),
#ifdef BALL_HAS_RTFACT	
			  continuous_loop_(false),
#endif
				toolbar_view_controls_(new QToolBar("3D View Controls", this)),
				mode_group_(new QActionGroup(this)),
				main_display_(new GLRenderWindow(this)),
				stereo_left_eye_(-1),
				stereo_right_eye_(-1)
		{
			stage_settings_=new StageSettings(this);
#ifdef BALL_VIEW_DEBUG
			Log.error() << "new Scene (3) " << this << std::endl;
#endif

#ifndef BALL_HAS_RTFACT
			renderers_.push_back(boost::shared_ptr<RenderSetup>(new RenderSetup(gl_renderer_, main_display_, this, stage_)));
#else
			renderers_.push_back(boost::shared_ptr<RenderSetup>(new RenderSetup(&*rt_renderer_, main_display_, this, stage_)));
#endif

			setObjectName(name);

			// the widget with the MainControl
			ModularWidget::registerWidget(this);
			setAcceptDrops(true);

			init();

			resize((Size) scene.renderers_[0]->renderer->getWidth(), 
						 (Size) scene.renderers_[0]->renderer->getHeight());

			renderers_[0]->resize(width(), height());
			renderers_[0]->start();
		}

		Scene::~Scene()
		{
#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object Scene " << this << " of class Scene" << std::endl;
#endif 
			delete stage_;

			for (Position i=0; i<renderers_.size(); ++i)
			{
				//	stop all running renderers
				renderers_[i]->stop();

				renderers_[i]->loop_mutex.lock();
				renderers_[i]->wait_for_render.wakeAll();
				renderers_[i]->loop_mutex.unlock();

				renderers_[i]->wait(1000);
				//	NOTE: This is problematic, since we have some smart pointers
				//	delete renderers_[i].renderer; 
				delete(renderers_[i]->target);
			}

			//rt_renderer_ & rt_render_window are smart pointers

			if (animation_thread_ != 0) delete animation_thread_;
		}

		void Scene::clear()
		{
			system_origin_.set(0.0, 0.0, 0.0);

			stage_->clear();
			animation_points_.clear();


			// TODO: clear the render setups
		}

		void Scene::set(const Scene& scene)
		{
			stage_ = scene.stage_;
			system_origin_.set(scene.system_origin_);

			// TODO: copy the render setups
		}

		const Scene& Scene::operator = (const Scene& scene)
		{
			set(scene);
			return *this;
		}

		bool Scene::isValid() const
		{
			return (getParent() != 0);
		}

		void Scene::dump(std::ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);

			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			stage_->dump(s, depth);

			BALL_DUMP_STREAM_SUFFIX(s);     
		}

		// ####################GL, CAMERA############################################

		void Scene::onNotify(Message *message)
		{
#ifdef BALL_VIEW_DEBUG
			Log.error() << "Scene " << this  << "onNotify " << message << std::endl;
#endif
//   			qApp->processEvents(); ??????? AM

			if (RTTI::isKindOf<RepresentationMessage>(*message)) 
			{
				bool needs_updategl = true;

				RepresentationMessage* rm = RTTI::castTo<RepresentationMessage>(*message);
				Representation* rep = rm->getRepresentation();
				switch (rm->getType())
				{
					case RepresentationMessage::UPDATE:
					{
						RepresentationManager& pm = getMainControl()->getRepresentationManager();
						if (pm.startRendering(rep))
						{
							for (Position i=0; i<renderers_.size(); ++i)
								renderers_[i]->bufferRepresentation(*rep);

							pm.finishedRendering(rep);
						}
						break;
					}

					case RepresentationMessage::REMOVE:
							for (Position i=0; i<renderers_.size(); ++i)
								renderers_[i]->removeRepresentation(*rep);
							break;

					case RepresentationMessage::FINISHED_UPDATE:
						needs_updategl = false;
						break;

					default:
						break;
				}

				if (needs_updategl)
				{
					// TODO: for some reason, we need to reset the
					//       lights here; otherwise, the lighting
					//       will be wrong until the first call to
					//       updateCamera()
					//       this is probably due to some wrong order
					//       of initializations and should be fixed
					//       somewhere else instead of this band-aid
					for (Position i=0; i<renderers_.size(); ++i)
						renderers_[i]->setLights(true);
					updateGL();
				}

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
						// TODO: change to a correct render setup call!
						for (Position i=0; i<renderers_.size(); ++i)
							renderers_[i]->removeGridTextures(*set->getData());
						break;

					default:
						return;
				}

				updateGL();
				return;
			}

			// react now only to SceneMessage
			if (!RTTI::isKindOf<SceneMessage>(*message)) return;

			SceneMessage *scene_message = RTTI::castTo<SceneMessage>(*message);

			switch (scene_message->getType())
			{
				case SceneMessage::REDRAW:
					updateGL();
					return;

				case SceneMessage::REBUILD_DISPLAY_LISTS:
					// TODO: this needs to a complete rebuffering!!!!
					updateGL();
					return;

				case SceneMessage::UPDATE_CAMERA:
					stage_->getCamera() = scene_message->getStage().getCamera();
					system_origin_ = scene_message->getStage().getCamera().getLookAtPosition();
					light_settings_->updateFromStage();
					updateGL();
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

		void Scene::init()
		{
			for (Position i=0; i<renderers_.size(); ++i)
			{
				renderers_[i]->init();
				GLRenderWindow* gt = dynamic_cast<GLRenderWindow*>(renderers_[i]->target);
				if (gt) 
				{
					gt->ignoreEvents(true);
					gt->installEventFilter(this);
				}
			}

			if (stage_->getLightSources().size() == 0) setDefaultLighting(false);
		}

		String Scene::createFPSInfo_()
		{
			String fps_string;

			float ti = 1000000.0 / (PreciseTime::now().getMicroSeconds() - time_.getMicroSeconds());

			if (ti < 0)
			{
				time_ = PreciseTime::now();
				return fps_string;
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

			std::ostringstream stream;
			stream.imbue(std::locale("C"));
			stream.setf(std::ios::fixed, std::ios_base::floatfield);
			stream.setf(std::ios::right, std::ios_base::adjustfield);
			stream.precision(1);
			stream.width(6);

			stream << fps;

			fps_string = stream.str();

			fps_string = String("FPS ") + fps_string;

			return fps_string;
		}

		void Scene::paintGL()
		{
			update_running_ = true;
			
			// This function tries to sync renderers by letting (a) all renderers
			// perform their updates and then (b) swap in the newly created buffers
			// of all render targets

			// this widget does not really paint anything
			//doneCurrent();

			// needed for all fps estimates
			time_ = PreciseTime::now();

			// precompute fps string if necessary
			// NOTE: this currently does not work correctly for multiple renderers, probably
			
			// first, let all renderers do their work
			for (Position i=0; i<renderers_.size(); ++i)
			{
				if (!renderers_[i]->isContinuous())
				{
					renderers_[i]->loop_mutex.lock();
					renderers_[i]->wait_for_render.wakeAll();
					renderers_[i]->loop_mutex.unlock();
				}
			}

			// then, estimate fps if necessary and add to the render target
			ColorRGBA text_color = stage_->getBackgroundColor().getInverseColor();

			String fps_string;
			QPoint fps_point;

			if (show_fps_)
				fps_string = createFPSInfo_();

			// draw all renderable texts and swap the new buffers in
			for (Position i=0; i<renderers_.size(); ++i)
			{
				if (renderers_[i]->isPaused())
					continue;

				renderers_[i]->makeCurrent();

				QPaintDevice* current_dev = dynamic_cast<QPaintDevice*>(renderers_[i]->target);
				if (show_fps_ && current_dev)
				{
					QPainter p(current_dev);

					QPen pen(QColor((int)text_color.getRed(),  (int)text_color.getGreen(), 
													(int)text_color.getBlue(), (int)text_color.getAlpha()));
					p.setPen(pen);

					QFont font("Arial", 16., QFont::Bold);
					font.setPointSizeF(16.);
					font.setBold(true);
					QFontMetrics fm(font);

					QRect r = fm.boundingRect(fps_string.c_str());
					QPointF fps_point((float)current_dev->width() - 20 - r.width(), 20);

					p.setRenderHint(QPainter::Antialiasing, true);
					p.setRenderHint(QPainter::TextAntialiasing, true);
					p.setFont(font);
					p.drawText(QPointF(fps_point), fps_string.c_str());

					p.end();
				}

				if (info_string_ != "")
				{
					QPainter p(current_dev);

					QPen pen(QColor((int)text_color.getRed(),  (int)text_color.getGreen(), 
													(int)text_color.getBlue(), (int)text_color.getAlpha()));

					p.setPen(pen);
					p.setRenderHint(QPainter::Antialiasing, true);
					p.setRenderHint(QPainter::TextAntialiasing, true);
					p.setFont(QFont("Arial", 16., QFont::Bold));

					// account for differently sized windows
					float xscale = current_dev->width()  / width();
					float yscale = current_dev->height() / height();

					p.drawText(QPointF(info_point_.x()*xscale, info_point_.y()*yscale), info_string_.c_str());

					p.end();
				}
			}
			
			update_running_ = false;
		}

		void Scene::paintEvent(QPaintEvent* e)
		{
			paintGL();
		}

		void Scene::resizeEvent(QResizeEvent* /*event*/)
		{						
			for (Position i=0; i<renderers_.size(); ++i)
				renderers_[i]->resize(width(), height());
		}

		bool Scene::eventFilter(QObject* object, QEvent* event)
		{
			for (Position i=0; i<renderers_.size(); ++i)
			{				
				if (static_cast<QObject*>(dynamic_cast<GLRenderWindow*>(renderers_[i]->target)) != object)
					continue;

				bool filter_out = false;

//				printf("received event %d\n", event->type());

				switch (event->type())
				{
					case QEvent::Resize:
						// we already handle resize events for our child widgets
						filter_out = true;
						break;
					case QEvent::Paint:
						paintGL();
						filter_out = true;
						break;
					case QEvent::ToolTip:
						// prevent tool tip events for continuous renderers; these would
						// stall the rendering loop
						if (renderers_[i]->isContinuous())
							filter_out = true;
						break;
					default:
						break;
				}

				if (filter_out)
					return true;
			}

			return QWidget::eventFilter(object, event);
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
			updateGL();
		}

		void Scene::move(Vector3 v)
		{
			Camera& camera = stage_->getCamera();

			camera.translate(-getTranslationVector_(v));
			updateGL();
		}

		void Scene::rotate(float degree_right, float degree_up)
		{
			const Camera& camera = stage_->getCamera();

			Quaternion q1;
 			q1.fromAxisAngle(camera.getLookUpVector(), Angle(degree_right, false).toRadian());

			Quaternion q2;
 			q2.fromAxisAngle(camera.getRightVector(), Angle(degree_up, false).toRadian());

 			q1 += q2;
			
			stage_->getCamera().rotate(q1, system_origin_);
			updateGL();
		}

		void Scene::moveComposites(const list<Composite*>& composites, Vector3 v)
		{
			HashSet<Composite*> roots;

			Vector3 x = getTranslationVector_(v);

			Matrix4x4 m;
			m.setTranslation(x);
			TransformationProcessor tp(m);

			list<Composite*>::const_iterator cit = composites.begin();
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
	
		void Scene::rotateComposites(const list<Composite*>& selection, float degree_right, float degree_up, float degree_clockwise)
		{
			const Camera& camera = stage_->getCamera();

			Vector3 vv = camera.getViewVector();
			vv.normalize(); 

			Quaternion q1;
 			q1.fromAxisAngle(camera.getLookUpVector(), Angle(degree_right, false).toRadian());

			Quaternion q2;
 			q2.fromAxisAngle(camera.getRightVector(), Angle(degree_up, false).toRadian());

			Quaternion Q;
 			Q.fromAxisAngle(camera.getViewVector(), Angle(degree_clockwise, false).toRadian());

 			q1 += q2;
 			q1 += Q;
			
			GeometricCenterProcessor center_processor;
			Vector3 center;
			list<Composite*>::const_iterator cit = selection.begin();
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
		// TODO: make renderer configurable
		void Scene::selectObjects_()
		{
			rb_->hide();

			QPoint p0 = mapFromGlobal(QPoint(x_window_pick_pos_first_, y_window_pick_pos_first_));
			QPoint p1 = mapFromGlobal(QPoint(x_window_pos_new_, y_window_pos_new_));

			list<GeometricObject*> objects;

			// draw the representations
			renderers_[0]->pickObjects((Position)p0.x(), (Position)p0.y(),
															   (Position)p1.x(), (Position)p1.y(), objects);

			// sent collected objects
			GeometricObjectSelectionMessage* message = new GeometricObjectSelectionMessage;
			message->setSelection(objects);
			message->setSelected(pick_select_);
			notify_(message);
		}


		void Scene::setViewPoint_()
		{
			SetCamera set_camera(this);
			set_camera.exec();
			gl_renderer_->updateCamera();
			updateGL();
		}


		void Scene::showViewPoint_()
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
		{
			// new instance for default values
			stage_->getCamera().clear();
 			light_settings_->updateFromStage();

			updateGL();
		}

		void Scene::resetRepresentationsForRenderer_(RenderSetup& rs)
		{
			// TODO: reset all representations already buffered in the renderer
			if (rs.receivesBufferUpdates())
			{
				RepresentationManager& pm = getMainControl()->getRepresentationManager();

				RepresentationList::const_iterator it = pm.getRepresentations().begin();
				for (; it != pm.getRepresentations().end(); ++it)
				{
					rs.bufferRepresentation(**it);
				}
			}
		}

		// ##################MISC############################

		void Scene::setDownsamplingFactor(float ds_factor)
		{
			for (size_t i=0; i<renderers_.size(); ++i)
			{
				if (RTTI::isKindOf<GLRenderWindow>(*(renderers_[i]->target)))
				{
					static_cast<GLRenderWindow*>(renderers_[i]->target)->setDownsamplingFactor(ds_factor);
					renderers_[i]->resize(renderers_[i]->renderer->getWidth(), renderers_[i]->renderer->getHeight());
				}
			}
		}

		void Scene::setDefaultLighting(bool update_GL)
		{
			LightSource light;
			light.setType(LightSource::POSITIONAL);
			light.setRelativeToCamera(true);
			light.setPosition(Vector3(0, 4, -20));
			light.setDirection(Vector3(0, 0, 1));

			stage_->clearLightSources();
			stage_->addLightSource(light);

			for (Position i=0; i<renderers_.size(); ++i)
				renderers_[i]->setLights(true);

			light_settings_->updateFromStage();

			if (update_GL)
				updateGL();
		}

		void Scene::createCoordinateSystem()
		{
			createCoordinateSystem_(false);
		}

		void Scene::createCoordinateSystemAtOrigin()
		{
			createCoordinateSystem_(true);
		}

		void Scene::createCoordinateSystem_(bool at_origin)
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

				x = Vector3(1,0,0);
				y = Vector3(0,1,0);
				z = Vector3(0,0,1);
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

			rp->setModelType(MODEL_COORDINATE_SYSTEM);
			rp->setColoringMethod(COLORING_CUSTOM);

			rp->setProperty(Representation::PROPERTY__IS_COORDINATE_SYSTEM);

			getMainControl()->insert(*rp);
			getMainControl()->update(*rp);

			notify_(new RepresentationMessage(*rp, RepresentationMessage::ADD_TO_GEOMETRIC_CONTROL));
		}

		bool Scene::exportScene(Renderer &er) const
		{
			if (er.init(*stage_, (float) width(), (float) height()))
			{
				RepresentationList::const_iterator it;
				MainControl *main_control = MainControl::getMainControl(this);

				it = main_control->getRepresentationManager().getRepresentations().begin();
				for (; it != main_control->getRepresentationManager().getRepresentations().end(); it++)
				{
					if (!er.renderOneRepresentation(**it))
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

			material_settings_->rendererChanged();
		}

		void Scene::initializePreferencesTab(Preferences &preferences)
		{
			preferences.insertEntry(light_settings_);

 			stage_settings_->getGLSettings();		
			preferences.insertEntry(stage_settings_);

			preferences.insertEntry(material_settings_);
		}

		void Scene::finalizePreferencesTab(Preferences &preferences)
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
		{
			if (light_settings_ == 0) return;

			light_settings_->apply();

			for (Position i=0; i<renderers_.size(); ++i)
				renderers_[i]->setLights(true);

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

			for (Position i=0; i<renderers_.size(); ++i)
				renderers_[i]->updateBackgroundColor();

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

			updateGL();
		}

		void Scene::writeLights_(INIFile& inifile) const
		{
			String data;
			inifile.appendSection("LIGHTING");

			Position nr = 0;
			list<LightSource>::const_iterator it = stage_->getLightSources().begin();
			for (; it != stage_->getLightSources().end(); it++)
			{
				const LightSource& light = *it;

				Vector3 dir = light.getDirection();
				Vector3 pos = light.getPosition();
				Vector3 att = light.getAttenuation();

				data = vector3ToString(pos);
				inifile.insertValue("LIGHTING", "Light_" + String(nr) + "_Position",  vector3ToString(pos));
				inifile.insertValue("LIGHTING", "Light_" + String(nr) + "_Direction", vector3ToString(dir));
				inifile.insertValue("LIGHTING", "Light_" + String(nr) + "_Attenuation", vector3ToString(att));

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
		{
			stage_->clearLightSources();
			vector<String> strings;
			Position nr = 0;

			try
			{
				while(inifile.hasEntry("LIGHTING", "Light_" + String(nr) + "_Position"))
				{
					LightSource light;
					Vector3 pos, dir, att;

					String data = inifile.getValue("LIGHTING", "Light_" + String(nr) + "_Relative");
					light.setRelativeToCamera(data.toUnsignedInt());

					data = inifile.getValue("LIGHTING", "Light_" + String(nr) + "_Position");
					stringToVector3(data, pos);

					data = inifile.getValue("LIGHTING", "Light_" + String(nr) + "_Direction");
					stringToVector3(data, dir);

					data = inifile.getValue("LIGHTING", "Light_" + String(nr) + "_Attenuation");
					// old ini files may not contain the attenuation. Try to be compatible
					if (data == INIFile::UNDEFINED)
						att = Vector3(1, 0, 0);
					else
						stringToVector3(data, att);

					light.setPosition(pos);
					light.setDirection(dir);
					light.setAttenuation(att);

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
			catch(Exception::GeneralException& e)
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
		{
			IconLoader& loader = IconLoader::instance();
			setMinimumSize(10, 10);

			ignore_pick_ = false;

			main_control.initPopupMenu(MainControl::DISPLAY);

			mode_group_->setExclusive(true);

			ShortcutRegistry* shortcut_registry = ShortcutRegistry::getInstance(0);
			QAction* new_action;

			create_coordinate_system_ = getMainControl()->initPopupMenu(MainControl::DISPLAY)->
				addMenu("Show Coordinate System");
			setMenuHint("Show a coordinate system");

			new_action = create_coordinate_system_->addAction("at origin", this, SLOT(createCoordinateSystemAtOrigin()));
			shortcut_registry->registerShortcut("Shortcut|Display|Show_Coordinate_System|at_origin", new_action);

			new_action = create_coordinate_system_->addAction("here", this, SLOT(createCoordinateSystem()));
			shortcut_registry->registerShortcut("Shortcut|Display|Show_Coordinate_System|here", new_action);

			insertMenuEntry(MainControl::DISPLAY, "Add new GL Window", this, SLOT(addGlWindow()), "Shortcut|Display|Add_new_GL_Window");
			// ======================== Display->Animation ===============================================
			String help_url = "tips.html#animations";

			record_animation_action_ = insertMenuEntry(MainControl::DISPLAY_ANIMATION, "Record", this, 
					SLOT(dummySlot()), "Shortcut|Display|Animation|Record");
			setMenuHint("Record an animation for later processing");
			setMenuHelp(help_url);
			record_animation_action_->setCheckable(true);

			clear_animation_action_ = insertMenuEntry(MainControl::DISPLAY_ANIMATION, "Clear", this, 
					SLOT(clearRecordedAnimation()), "Shortcut|Display|Animation|Clear");
			setMenuHelp(help_url);

			main_control.insertPopupMenuSeparator(MainControl::DISPLAY_ANIMATION);

			start_animation_action_ = insertMenuEntry(MainControl::DISPLAY_ANIMATION, "Start", this, 
					SLOT(startAnimation()), "Shortcut|Display|Animation|Start");
			setMenuHelp(help_url);

			cancel_animation_action_ = insertMenuEntry(MainControl::DISPLAY_ANIMATION, "Stop", this, 
					SLOT(stopAnimation()), "Shortcut|Display|Animation|Stop");
			cancel_animation_action_->setEnabled(false);
			setMenuHelp(help_url);

			main_control.insertPopupMenuSeparator(MainControl::DISPLAY_ANIMATION);

			animation_export_PNG_action_ = insertMenuEntry(MainControl::DISPLAY_ANIMATION, "Export PNG", this, 
					SLOT(dummySlot()), "Shortcut|Display|Animation|Export_PNG");
			setMenuHelp(help_url);
			animation_export_PNG_action_->setCheckable(true);

			animation_export_POV_action_ = insertMenuEntry(MainControl::DISPLAY_ANIMATION, "Export POV", this, 
					SLOT(dummySlot()), "Shortcut|Display|Animation|Export_POV");
			setMenuHelp(help_url);
			animation_export_POV_action_->setCheckable(true);

			animation_repeat_action_ = insertMenuEntry(MainControl::DISPLAY_ANIMATION, "Repeat", this, 
					SLOT(dummySlot()), "Shortcut|Display|Animation|Repeat");
			setMenuHelp(help_url);
			animation_repeat_action_->setCheckable(true);

			// ======================== Display->Stereo ===============================================
			main_control.insertPopupMenuSeparator(MainControl::DISPLAY);

			no_stereo_action_ = insertMenuEntry(MainControl::DISPLAY_STEREO, "No Stereo", this, 
					SLOT(exitStereo()), "Shortcut|Display|Stereo|No_Stereo");
			no_stereo_action_->setCheckable(true);
			no_stereo_action_->setChecked(true);
			setMenuHelp("tips.html#3D");

			active_stereo_action_ = insertMenuEntry(MainControl::DISPLAY_STEREO, "Shutter Glasses", this, 
					SLOT(enterActiveStereo()), "Shortcut|Display|Stereo|Shutter_Glasses");
			setMenuHelp("tips.html#3D");
			active_stereo_action_->setCheckable(true);

			dual_stereo_action_ = insertMenuEntry(MainControl::DISPLAY_STEREO, "Side by Side", this,
					SLOT(enterDualStereo()), "Shortcut|Display|Stereo|Side_by_Side");
			setMenuHelp("tips.html#3D");
			dual_stereo_action_->setCheckable(true);

			dual_stereo_different_display_action_ = insertMenuEntry(MainControl::DISPLAY_STEREO, 
					"Side by Side on Different Displays", this, 
					SLOT(enterDualStereoDifferentDisplays()),
					"Shortcut|Display|Stereo|Side_by_Side_on_Different_Displays");
			setMenuHelp("tips.html#3D");
			dual_stereo_different_display_action_->setCheckable(true);

			// ======================== Display->Viewpoint ===============================================
			getMainControl()->insertPopupMenuSeparator(MainControl::DISPLAY_VIEWPOINT);

			insertMenuEntry(MainControl::DISPLAY_VIEWPOINT, "&Store Viewpoint", this, 
					SLOT(storeViewPoint()), "Shortcut|Display|Viewpoint|Store");
			setMenuHint("Store the current viewpoint");

			insertMenuEntry(MainControl::DISPLAY_VIEWPOINT, "&Restore Viewpoint", this, 
					SLOT(restoreViewPoint()), "Shortcut|Display|Viewpoint|Restore");
			setMenuHint("Restore the viewpoint");

			getMainControl()->insertPopupMenuSeparator(MainControl::DISPLAY_VIEWPOINT);

			String description("Shortcut|Display|Viewpoint|Show_Viewpoint");
			insertMenuEntry(MainControl::DISPLAY_VIEWPOINT, "Show Vie&wpoint", this, 
					SLOT(showViewPoint_()), description, QKeySequence("Ctrl+W"));
			setMenuHint("Print the coordinates of the current viewpoint");


			description = "Shortcut|Display|Viewpoint|Set_Viewpoint";
			insertMenuEntry(MainControl::DISPLAY_VIEWPOINT, "Set Viewpoi&nt", this, 
					SLOT(setViewPoint_()), description, QKeySequence("Ctrl+N"));
			setMenuHint("Move the viewpoint to the given coordinates");

			insertMenuEntry(MainControl::DISPLAY_VIEWPOINT, "Rese&t Camera", this, 
					SLOT(resetCamera_()), "Shortcut|Display|Viewpoint|Reset_Camera");
			setMenuHint("Reset the camera to the orgin (0,0,0)");

			getMainControl()->insertPopupMenuSeparator(MainControl::DISPLAY_VIEWPOINT);
			insertMenuEntry(MainControl::DISPLAY_VIEWPOINT, "Limit View Volume", this, 
					SLOT(setupViewVolume()), "Shortcut|Display|Viewpoint|Limit_View_Volume");

			description = "Shortcut|File|Export|PNG";
			QAction* screenshot_action = insertMenuEntry(MainControl::FILE_EXPORT, "PNG...", this, 
					SLOT(showExportPNGDialog()), description, QKeySequence("Alt+P"));
			setMenuHint("Export a PNG image file from the Scene");
			setIcon("actions/screenshot", false);

			description = "Shortcut|File|Export|POVRay";
			insertMenuEntry(MainControl::FILE_EXPORT, "POVRa&y scene", this, 
					SLOT(exportPOVRay()), description, QKeySequence("Ctrl+Y"));
			setIcon("mimetype/text-x-povray", false);
			setMenuHint("tips.html#povray");

			description = "Shortcut|File|Export|VRML";
			insertMenuEntry(MainControl::FILE_EXPORT, "3D Prototyping Export", this, 
					SLOT(showExportVRMLDialog()), description);
			setMenuHint("Export a VRML or stl file from the scene");

			// ====================================== MODES =====================================
			description = "Shortcut|Display|Rotate_Mode";
			main_control.insertPopupMenuSeparator(MainControl::DISPLAY);
			rotate_action_ = insertMenuEntry(MainControl::DISPLAY, "&Rotate Mode", this, 
					SLOT(rotateMode_()), description, QKeySequence("Ctrl+R"));

			setMenuHint("Switch to rotate/zoom mode");
			setMenuHelp("scene.html#rotate_mode");
			rotate_action_->setCheckable(true);
			setIcon("actions/transform-rotate", false);
			toolbar_actions_view_controls_.push_back(rotate_action_);
			mode_group_->addAction(rotate_action_);

			description = "Shortcut|Display|Picking_Mode";
			picking_action_ = insertMenuEntry(MainControl::DISPLAY, "&Picking Mode", this, 
					SLOT(pickingMode_()), description, QKeySequence("Ctrl+P"));

			setMenuHint("Switch to picking mode, e.g. to identify single atoms or groups");
			setMenuHelp("scene.html#identify_atoms");
			setIcon("actions/select-rectangular", false);
			picking_action_->setCheckable(true);
			toolbar_actions_view_controls_.push_back(picking_action_);
			mode_group_->addAction(picking_action_);

			description = "Shortcut|Display|Move_Mode";
			move_action_ = insertMenuEntry(MainControl::DISPLAY, "Move Mode", this, 
					SLOT(moveMode_()), description);

			setMenuHint("Switch to move mode, e.g. move selected items");
			setMenuHelp("molecularControl.html#move_molecule");
			setIcon("actions/transform-move", false);
			move_action_->setCheckable(true);
			toolbar_actions_view_controls_.push_back(move_action_);
			mode_group_->addAction(move_action_);

			description = "Shortcut|ToggleFullscreen";
			fullscreen_action_ = new QAction("Fullscreen", this);
			fullscreen_action_->setObjectName(fullscreen_action_->text());
			connect(fullscreen_action_, SIGNAL(triggered()), getMainControl(), SLOT(toggleFullScreen()));
			fullscreen_action_->setIcon(loader.getIcon("actions/view-fullscreen"));
			toolbar_actions_view_controls_.push_back(fullscreen_action_);
			shortcut_registry->registerShortcut(description, fullscreen_action_);

			description = "Shortcut|ShowRuler";
			switch_grid_ = new QAction("Show ruler", this);
			switch_grid_->setObjectName(switch_grid_->text());
			connect(switch_grid_, SIGNAL(triggered()), this, SLOT(switchShowGrid()));
			switch_grid_->setCheckable(true);
			switch_grid_->setChecked(false);
			switch_grid_->setIcon(loader.getIcon("actions/measure"));
			toolbar_actions_view_controls_.push_back(switch_grid_);
			shortcut_registry->registerShortcut(description, switch_grid_);

			// and push the icons whose actions are defined somewhere else
			// into the toolbar_actions_view 
			toolbar_actions_view_controls_.push_back(screenshot_action);

#ifdef BALL_HAS_RTFACT
			description = "Shortcut|Display|ContinuousLoop|Toggle";
			toggle_continuous_loop_action_ = new QAction("Toggle continuous loop", this);
			toggle_continuous_loop_action_->setObjectName(toggle_continuous_loop_action_->text());
			connect(toggle_continuous_loop_action_, SIGNAL(triggered()), this, SLOT(toggleContinuousLoop()));
			toggle_continuous_loop_action_->setCheckable(true);
			toggle_continuous_loop_action_->setChecked(false);
			toggle_continuous_loop_action_->setIcon(loader.getIcon("actions/continuous-loop"));
			toolbar_actions_view_controls_.push_back(toggle_continuous_loop_action_);
			toggle_continuous_loop_action_->setShortcut(QKeySequence(Qt::Key_Space));
			shortcut_registry->registerShortcut(description, toggle_continuous_loop_action_);
#endif
			// end of the toolbar entries

			description = "Shortcut|File|Print";
			insertMenuEntry(MainControl::FILE, "Print", this, SLOT(printScene()), description);

			window_menu_entry_ = insertMenuEntry(MainControl::WINDOWS, "Scene", this, SLOT(switchShowWidget()));
			window_menu_entry_->setCheckable(true);
			setMenuHelp("scene.html");

			setCursor(QCursor(Qt::ArrowCursor));

			setFocusPolicy(Qt::StrongFocus);
			registerForHelpSystem(this, "scene.html");

			toolbar_view_controls_->setObjectName("3D View Control toolbar");
			toolbar_view_controls_->setIconSize(QSize(22,22));
			toolbar_view_controls_->layout()->setMargin(2);
			toolbar_view_controls_->layout()->setSpacing(2);
		}

		void Scene::checkMenu(MainControl& main_control)
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
		{
			if (animation_thread_ != 0 && animation_thread_->isRunning())
			{
				return true;
			}

			return false;
		}

		//##########################EVENTS#################################

		void Scene::customEvent(QEvent* evt)
		{
			switch(static_cast<EventsIDs>(evt->type())) {
				case MOTIONTRACKING_EVENT:
					motionTrackingEvent(static_cast<MotionTrackingEvent*>(evt));
					break;
				case TRANSFORMATION_EVENT_6D:
					transformationEvent6D(static_cast<TransformationEvent6D*>(evt));
					break;
				case BUTTON_RELEASE_EVENT:
					buttonReleaseEvent(static_cast<ButtonEvent*>(evt));
					break;
				case BUTTON_PRESS_EVENT:
					buttonPressEvent(static_cast<ButtonEvent*>(evt));
					break;
				case RENDER_TO_BUFFER_FINISHED_EVENT:
					RenderSetup* renderer = static_cast<RenderToBufferFinishedEvent*>(evt)->getRenderer();

					renderer->makeCurrent();
					// NOTE: GLRenderers currently render in the GUI thread!
					//if (RTTI::isKindOf<GLRenderer>(*(renderer->renderer)))
					//	renderer->renderToBuffer();
					//else
						renderer->target->refresh();

					if (RTTI::isKindOf<GLRenderWindow>(*(renderer->target)))
						static_cast<GLRenderWindow*>(renderer->target)->swapBuffers();

					// has the renderer reached the end of its live span?
					if (renderer->getTimeToLive() == 0)
					{
						renderer->useContinuousLoop(false);
						renderer->stop();

						renderer->loop_mutex.lock();
						renderer->wait_for_render.wakeAll();
						renderer->loop_mutex.unlock();

						renderer->wait(100);

						std::vector<boost::shared_ptr<RenderSetup> >::iterator it;

						for (it = renderers_.begin(); it != renderers_.end(); ++it)
						{
							if (it->get() == renderer)
							{
								delete((*it)->renderer);
								delete((*it)->target);

								renderers_.erase(it);
								
								break;
							}
						}
					} 
					else if (renderer->isContinuous())
					{
						renderer->loop_mutex.lock();
						renderer->wait_for_render.wakeAll();
						renderer->loop_mutex.unlock();
					}
					break;
			}
		}

		void Scene::transformationEvent6D(TransformationEvent6D* evt)
		{
			Vector3 tmp = evt->getTranslation() * 0.01;
			tmp.z = -tmp.z;
			move(-tmp);

			Camera& camera = stage_->getCamera();

			Quaternion q1;
			q1.set(camera.getLookUpVector(), 0.3 * Angle(-evt->getRotation().y, false).toRadian());

			Quaternion q2;
			q2.set(camera.getRightVector(),  0.3 * Angle(-evt->getRotation().x, false).toRadian());

			Quaternion q3;
			q3.set(camera.getViewVector(),   0.3 * Angle(-evt->getRotation().z, false).toRadian());

			q1 += q2 + q3;

			camera.rotate(q1, system_origin_);
		}
		
		void Scene::motionTrackingEvent(MotionTrackingEvent* evt)
		{

			if (!tracking_initialized_)
			{
				old_trackorigin_ = evt->getOrigin();
				old_trackrotation_ = evt->getTransform();
				tracking_initialized_ = true;
				return;
			}

			Vector3 new_trackorigin = evt->getOrigin();
			Vector3 movement = new_trackorigin - old_trackorigin_;

			Quaternion new_trackrotation = evt->getTransform();
			Quaternion track_rotation = new_trackrotation - old_trackrotation_;

			if (!mouse_button_is_pressed_)
			{
				old_trackorigin_ = new_trackorigin;
				old_trackrotation_ = new_trackrotation;
				return;
			}
			
			Camera& camera = stage_->getCamera();

			camera.moveRight(movement.x);
			camera.moveUp(movement.y);
			camera.moveForward(movement.z);
			old_trackorigin_ = new_trackorigin;

			//Vector3 v1, v2, v3, v4, v5;
			//v1 = camera.getRightVector();
			//v2 = camera.getLookUpVector();
			//v3 = -(camera.getViewVector());
			//v4 = camera.getViewPoint();
			//v5 = v4 - v3;
			//if (v3.getLength() == 0)
			//	return;
			//
			//v3.normalize();

			//Matrix4x4 to_origin(v1.x, v1.y, v1.z, 0,
			//										v2.x, v2.y, v2.z, 0,
			//										v3.x, v3.y, v3.z, 0,
			//											 0,    0,    0, 1);

			//if (  (track_rotation.getAxis().getSquareLength() < 1e-6) 
			//		|| fabs(track_rotation.getAngle()) < 1e-4)
			//	return;

			//Matrix4x4 qmat;
			//track_rotation.getRotationMatrix(qmat);
			//Matrix4x4 rotation = to_origin;	
			//rotation *= qmat;
			//to_origin.transpose();
			//rotation *= to_origin;
			//camera.rotate(rotation, v5);

			//old_trackrotation_ = new_trackrotation;

			//Quaternion new_trackrotation = evt->getTransform();
			//Quaternion rot  = new_trackrotation - old_trackrotation_;
			//camera.rotate(new_trackrotation, system_origin_);
			//old_trackrotation_ = new_trackrotation;
			gl_renderer_->updateCamera();
			updateGL();
		}
		
		void Scene::buttonPressEvent(ButtonEvent* /*evt*/)
		{
		}

		void Scene::buttonReleaseEvent(ButtonEvent* /*evt*/)
		{
		}

		void Scene::mouseMoveEvent(QMouseEvent* e)
		{
			if (isAnimationRunning()) return;

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
			// TODO make keys configurable in shortcutRegistry 
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
				if (e.modifiers() == (Qt::ShiftModifier | Qt::ControlModifier)) return ROTATE_CLOCKWISE_ACTION;
			}
			else if (e.buttons() == (Qt::LeftButton | Qt::RightButton))
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

			switch ((Index)(e->buttons() | e->modifiers()))
			{
				case (Qt::SHIFT | Qt::LeftButton): 
				case  Qt::MidButton:
					zoomSystem_();
					break;

				case (Qt::CTRL | Qt::LeftButton):
				case  Qt::RightButton:
					translateSystem_();
					break;

				case ((Index)Qt::LeftButton | Qt::RightButton):
				case (Qt::SHIFT | Qt::CTRL | Qt::LeftButton):
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
				list<Composite*> composites;
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

		// TODO: make renderer configurable
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

			list<GeometricObject*> objects;

			// ok, do the picking, until we find something
			for (Position p = 0; p < 8; p++)
			{
				renderers_[0]->pickObjects(pos_x - p, pos_y - p, pos_x + p, pos_y + p, objects);
				if (!objects.empty()) break;
			}

			if (objects.empty())
			{
				unlockComposites();
				return;
			}

			// get the description
			String string;
			MolecularInformation info;

			GeometricObject* object = 0;
			list<GeometricObject*>::iterator git = objects.begin();
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
			info_string_ = "";

			qmouse_event->accept();

			y_window_pos_new_ = (Position)(y_window_pos_old_ + (qmouse_event->delta() / 120 * mouse_wheel_sensitivity_));
			zoomSystem_();
			y_window_pos_old_ = y_window_pos_new_;
		}

		void Scene::keyPressEvent(QKeyEvent* e)
		{
			// TODO make keys configurable in shortcutRegistry 
			if ((gl_renderer_->getStereoMode() == GLRenderer::NO_STEREO) &&
			    (e->key() == Qt::Key_Escape))
			{
				switchToLastMode();
				return;
			}
			
			// TODO make keys configurable in shortcutRegistry 
			if (e->key() == Qt::Key_R)
			{
				setMode(ROTATE__MODE);
				return;
			}
			
			// TODO make keys configurable in shortcutRegistry 
			if (e->key() == Qt::Key_W)
			{
				setMode(MOVE__MODE);
				return;
			}

			// TODO make keys configurable in shortcutRegistry 
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
			
			// TODO make keys configurable in shortcutRegistry 
			if ((e->key() == Qt::Key_Y && e->modifiers() == Qt::AltModifier) ||
					 e->key() == Qt::Key_Escape)
			{
				exitStereo();
				return;
			}
			
			// TODO make keys configurable in shortcutRegistry 
			// setting of eye and focal distance
			if (e->key() != Qt::Key_Left  &&
					e->key() != Qt::Key_Right &&
					e->key() != Qt::Key_Up    &&
					e->key() != Qt::Key_Down)
			{
				e->ignore();
				return;
			}
			
			// TODO make keys configurable in shortcutRegistry 
			// setting of eye distance
			if (e->key() == Qt::Key_Left ||
					e->key() == Qt::Key_Right)
			{
				float new_distance = stage_->getEyeDistance();
				
				// TODO make keys configurable in shortcutRegistry 
				float modifier = 1;
				if (e->key() == Qt::Key_Left)
				{
					modifier = -0.1;
				}
				else
				{
					modifier = +0.1;
				}
				
				// TODO make keys configurable in shortcutRegistry 
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
				
				// TODO make keys configurable in shortcutRegistry 
				float modifier = 1;
				if (e->key() == Qt::Key_Down)
				{
					modifier = -1;
				}
				else
				{
					modifier = +1;
				}
				
				// TODO make keys configurable in shortcutRegistry 
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
				
				if (new_focal_distance > 1000) 
				{
					new_focal_distance = 1000;
				}

				stage_->setFocalDistance(new_focal_distance);
			}

			stage_settings_->updateFromStage();
			updateGL();
		}

		void Scene::setMode(ModeType mode)
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
			QString vrml_end = ".wrl";
			QString stl_end = ".stl"; 
			filename.remove( vrml_end );
			filename.remove( stl_end );

			MainControl* mc = getMainControl();
			RepresentationManager& rm = mc->getRepresentationManager();
			RepresentationList::const_iterator rit;
			
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
							vtemp.append(".wrl");
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
						vtemp.append(".wrl");
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

			// TODO: push into renderSetup
			QImage pic = main_display_->grabFrameBuffer();
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
												"Export PNG File",
												(getWorkingDir() + String(FileSystem::PATH_SEPARATOR) + start).c_str(),
												"*.png");

			if (qresult == QString::null) return;

			String result = ascii(qresult);
			if (!offscreen_rendering_)
			{
				updateGL();
				getMainControl()->processEvents(9999);
			}
			exportPNG(result);
		}

		bool Scene::exportPNG(const String& filename)
		{
			bool ok = false;

			// TODO: currently, we always use the first renderer in the list for exporting;
			// 			 we should decide this in a sensible way instead

			// first find out if we need to render offscreen or whether we can just use the current image
			if (!offscreen_rendering_)
				return renderers_[0]->exportPNG(filename);

			// ok, we have to do this the hard way...	

			// What kind of renderer do we have to encapsulate?
			if (RTTI::isKindOf<GLRenderer>(*(renderers_[0]->renderer)))
			{
				// it's a GLRenderer => use tiling
				GLOffscreenTarget* new_widget = new GLOffscreenTarget(main_display_, filename);
				new_widget->init();
				new_widget->resize(width(), height());
				new_widget->prepareRendering();

				GLRenderer* gr = new GLRenderer;
				gr->init(*this);
				gr->enableVertexBuffers(want_to_use_vertex_buffer_);
				gr->setSize(width(), height());

				TilingRenderer *tr = new TilingRenderer(gr, offscreen_factor_*width(), offscreen_factor_*height());
				tr->init(*this);
				tr->setSize(width(), height());

				boost::shared_ptr<RenderSetup> tr_rs(new RenderSetup(tr, new_widget, this, stage_));

				resetRepresentationsForRenderer_(*tr_rs);

				renderers_.push_back(tr_rs);
				updateGL();
				renderers_.pop_back();

				delete(tr);
				delete(gr);
				delete(new_widget);

				// TODO: we should not rely on the first renderer being the one
				// related to the main_display_!
				renderers_[0]->resize(width(), height());
				updateGL();

			}
			else if (RTTI::isKindOf<t_RaytracingRenderer>(*(renderers_[0]->renderer)))
			{
				// create a new renderer
				t_RaytracingRenderer* renderer = new t_RaytracingRenderer;

				// build a new offscreen target for our renderer
				RenderWindow<BALL_DEFAULT_PIXEL_TYPE>* target = new RenderWindow<BALL_DEFAULT_PIXEL_TYPE>;

				// and combine it into a new render setup
				boost::shared_ptr<RenderSetup> tr_rs(new RenderSetup(renderer, target, this, stage_));
				renderers_.push_back(tr_rs);

				tr_rs->init();

				applyPreferences();

				resetRepresentationsForRenderer_(*tr_rs);

				// iterate 10 times for antialiasing
				tr_rs->useContinuousLoop(true);
				tr_rs->setTimeToLive(20);
				tr_rs->exportPNGAfterTTL(filename);

				tr_rs->resize(offscreen_factor_ * width(), offscreen_factor_ * height());
				tr_rs->start();

				updateGL();
			}

			ok = true;

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
				main_display_->makeCurrent();
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
		{
			if (window_menu_entry_ == 0) return;

			bool vis = isVisible();
			setVisible(!vis);
			window_menu_entry_->setChecked(vis);
		}

		// TODO: make renderer configurable
		void Scene::exitStereo()
		{
			no_stereo_action_->setChecked(true);
			active_stereo_action_->setChecked(false);
			dual_stereo_action_->setChecked(false);
			dual_stereo_different_display_action_->setChecked(false);

			if (gl_renderer_->getStereoMode() == GLRenderer::NO_STEREO) return;

			// remember pointers to the left and right windows
			RenderTarget* left_window  = (stereo_left_eye_  != -1) ? renderers_[stereo_left_eye_ ]->target : 0;
			RenderTarget* right_window = (stereo_right_eye_ != -1) ? renderers_[stereo_right_eye_]->target : 0;

			// note: it is important to erase the right eye first, because then the index of the
			// left eye will still be valid (being smaller than the erased right one)
			if (stereo_right_eye_ != -1)
				renderers_.erase(renderers_.begin()+stereo_right_eye_);
			stereo_right_eye_ = -1;

			if (stereo_left_eye_ != -1)
				renderers_.erase(renderers_.begin()+stereo_left_eye_);
			stereo_left_eye_ = -1;

			delete(left_window);
			delete(right_window);

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
				updateGL(); // TODO: update() or updateGL()???
				return;
			}

			hide();
 			setParent((QWidget*)getMainControl());
			show();
			getMainControl()->setCentralWidget(this);
			getMainControl()->restoreState(last_state_);
			updateGL(); // TODO: update() or updateGL()???
		}

		void Scene::addGlWindow()
		{
			GLRenderWindow* new_widget = new GLRenderWindow(0, "Scene", Qt::Window);
			new_widget->init();
			new_widget->makeCurrent();
			new_widget->resize(width(), height());

			new_widget->installEventFilter(this);

///////// Variante 1 ////
//			//GLRenderer*   new_renderer = new GLRenderer;
//			//new_renderer->init(*this);
//			//new_renderer->enableVertexBuffers(want_to_use_vertex_buffer_);
//
//			new_widget->show();
//
//			boost::shared_ptr<RenderSetup> new_rs(new RenderSetup(&*rt_renderer_, new_widget, this, stage_));
//			new_rs->setReceiveBufferUpdates(false);	
//
//			//resetRepresentationsForRenderer_(*new_rs);


///////// Variante 2 ////
			t_RaytracingRenderer* new_renderer = new t_RaytracingRenderer();
			new_renderer->init(*this);
			new_renderer->setFrameBufferFormat(new_widget->getFormat());

			boost::shared_ptr<RenderSetup> new_rs(new RenderSetup(new_renderer, new_widget, this, stage_));
			new_rs->setReceiveBufferUpdates(true);	
			resetRepresentationsForRenderer_(*new_rs);
			new_widget->show();


			renderers_.push_back(new_rs);
			// NOTE: *don't* try to start new_rs, since this is an automatic variable
			//       that will be destroyed soon afterwards
			renderers_[renderers_.size()-1]->start();
		}

		void Scene::enterActiveStereo()
		{
			// first clean up
			exitStereo();

return; //TODO Why??
			GLRenderWindow* new_widget = new GLRenderWindow(0, "blubb", Qt::Window);
			new_widget->makeCurrent();
			new_widget->init();
			new_widget->resize(width(), height());
			new_widget->show();

//			RenderSetup rs(&*rt_renderer_, new_widget, this, stage_);
//			rs.setReceiveBufferUpdates(false);
//			rs.setOffset(Vector3(5, 0, 5)); // TEST

//			renderers_.push_back(rs);
			return;

			gl_renderer_->setStereoMode(GLRenderer::ACTIVE_STEREO);
			setFullScreen(true);

			no_stereo_action_->setChecked(false);
			active_stereo_action_->setChecked(true);
			dual_stereo_action_->setChecked(false);
			dual_stereo_different_display_action_->setChecked(false);
		}

		void Scene::enterDualStereo()
		{
			// first clean up
			exitStereo();

			GLRenderWindow* left_widget = new GLRenderWindow(0, "left eye");
			left_widget->makeCurrent();
			left_widget->init();
			left_widget->resize(width(), height());

			GLRenderer*   left_renderer = new GLRenderer;
			left_renderer->init(*this);
			left_renderer->enableVertexBuffers(want_to_use_vertex_buffer_);

			left_widget->showFullScreen();
			left_renderer->setSize(left_widget->width(), left_widget->height());

			boost::shared_ptr<RenderSetup> left_rs(new RenderSetup(left_renderer, left_widget, this, stage_));

			resetRepresentationsForRenderer_(*left_rs);
			left_rs->setStereoMode(RenderSetup::LEFT_EYE);

			renderers_.push_back(left_rs);
			stereo_left_eye_ = renderers_.size()-1;
			left_rs->start();

			GLRenderWindow* right_widget = new GLRenderWindow(0, "right eye");
			right_widget->makeCurrent();
			right_widget->init();
			right_widget->resize(width(), height());

			GLRenderer*   right_renderer = new GLRenderer;
			right_renderer->init(*this);
			right_renderer->enableVertexBuffers(want_to_use_vertex_buffer_);

			right_widget->showFullScreen();
			right_renderer->setSize(right_widget->width(), right_widget->height());

			boost::shared_ptr<RenderSetup> right_rs(new RenderSetup(right_renderer, right_widget, this, stage_));

			resetRepresentationsForRenderer_(*right_rs);
			right_rs->setStereoMode(RenderSetup::RIGHT_EYE);

			renderers_.push_back(right_rs);
			stereo_right_eye_ = renderers_.size()-1;
			right_rs->start();

			gl_renderer_->setStereoMode(GLRenderer::DUAL_VIEW_STEREO);

			setFullScreen(false);
			updateGL();

			no_stereo_action_->setChecked(false);
			active_stereo_action_->setChecked(false);
			dual_stereo_action_->setChecked(true);
			dual_stereo_different_display_action_->setChecked(false);
		}

		void Scene::enterDualStereoDifferentDisplays()
		{
			// first clean up
			exitStereo();

			GLRenderWindow* left_widget = new GLRenderWindow(QApplication::desktop()->screen(0), "left eye");
			left_widget->makeCurrent();
			left_widget->init();
			left_widget->resize(QApplication::desktop()->screen(0)->width(), QApplication::desktop()->screen(0)->height());

#ifndef BALL_HAS_RTFACT
			GLRenderer*   left_renderer = new GLRenderer;
			left_renderer->init(*this);
			left_renderer->enableVertexBuffers(want_to_use_vertex_buffer_);
#else
			t_RaytracingRenderer*  left_renderer = new t_RaytracingRenderer();
			left_renderer->init(*this);
			left_renderer->setFrameBufferFormat(left_widget->getFormat());
#endif

			left_widget->show();
			left_renderer->setSize(left_widget->width(), left_widget->height());

			boost::shared_ptr<RenderSetup> left_rs(new RenderSetup(left_renderer, left_widget, this, stage_));

			resetRepresentationsForRenderer_(*left_rs);
			left_rs->setStereoMode(RenderSetup::LEFT_EYE);

			renderers_.push_back(left_rs);
			stereo_left_eye_ = renderers_.size()-1;
			left_rs->start();

			GLRenderWindow* right_widget = new GLRenderWindow(QApplication::desktop()->screen(1), "right eye");
			right_widget->makeCurrent();
			right_widget->init();
			right_widget->resize(QApplication::desktop()->screen(1)->width(), QApplication::desktop()->screen(1)->height());

#ifndef BALL_HAS_RTFACT
			GLRenderer*   right_renderer = new GLRenderer;
			right_renderer->init(*this);
			right_renderer->enableVertexBuffers(want_to_use_vertex_buffer_);
#else
			t_RaytracingRenderer*  right_renderer = new t_RaytracingRenderer();
			right_renderer->init(*this);
			right_renderer->setFrameBufferFormat(right_widget->getFormat());
#endif

			right_widget->show();
			right_renderer->setSize(right_widget->width(), right_widget->height());

			boost::shared_ptr<RenderSetup> right_rs(new RenderSetup(right_renderer, right_widget, this, stage_));

			resetRepresentationsForRenderer_(*right_rs);
			right_rs->setStereoMode(RenderSetup::RIGHT_EYE);

			renderers_.push_back(right_rs);
			stereo_right_eye_ = renderers_.size()-1;
			right_rs->start();

			gl_renderer_->setStereoMode(GLRenderer::DUAL_VIEW_DIFFERENT_DISPLAY_STEREO);

			setFullScreen(false);
			applyPreferences();
			updateGL();

			no_stereo_action_->setChecked(false);
			active_stereo_action_->setChecked(false);
			dual_stereo_action_->setChecked(false);
			dual_stereo_different_display_action_->setChecked(true);
		}

		void Scene::setCamera(const Camera& camera)
		{
			stage_->getCamera() = camera;
 			light_settings_->updateFromStage();

			updateGL();
		}

		void Scene::clearRecordedAnimation()
		{
			animation_points_.clear();
		}

		void Scene::startAnimation()
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
		{
			stop_animation_ = true;
		}

#ifdef BALL_HAS_RTFACT		
		void Scene::startContinuousLoop()
		{	
			continuous_loop_ = true;

			for (Position i=0; i<renderers_.size(); ++i)
			{
				if (!renderers_[i]->isContinuous())
				{
					renderers_[i]->useContinuousLoop(true);
					renderers_[i]->loop_mutex.lock();
					renderers_[i]->wait_for_render.wakeAll();
					renderers_[i]->loop_mutex.unlock();
					// set the menu buttons correctly
					//stop_continuous_loop_action_->setEnabled(true);
					//start_continuous_loop_action_->setEnabled(false);	
					// set the icon
					//toggle_continuous_loop_action_->setChecked(true);
				}
			}
			setStatusbarText("Switched continuous loop on", true);
		}

		void Scene::stopContinuousLoop()
		{		
			continuous_loop_ = false;	

			for (Position i=0; i<renderers_.size(); ++i)
			{
				if (renderers_[i]->isContinuous())
				{
					renderers_[i]->useContinuousLoop(false);
					// set the menu buttons correctly
					//stop_continuous_loop_action_->setEnabled(false);
					//start_continuous_loop_action_->setEnabled(true);
					// set the icon
					//toggle_continuous_loop_action_->setChecked(false);
				}
			}
			setStatusbarText("Switched continuous loop off", true);
		}

		void Scene::toggleContinuousLoop()
		{
			if (continuous_loop_)
			{
				stopContinuousLoop();
			}
			else
			{
				startContinuousLoop();
			}
		}
#endif


		void Scene::animate_()
		{
			bool export_PNG = animation_export_PNG_action_->isChecked();
			bool export_POV = animation_export_POV_action_->isChecked();
			bool repeat     = animation_repeat_action_->isChecked();

			do
			{
				list<Camera>::iterator it = getAnimationPoints().begin();
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
			// TODO: push into renderTarget!
			/*
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
			*/
            return false;
		}

		void Scene::setWidgetVisible(bool state)
		{
			// only for Python needed
			QWidget::setVisible(state);
		}

		void Scene::updateGL()
		{
			// TODO: remove!
			paintGL();
		}

		void Scene::setOffScreenRendering(bool enabled, Size factor)
		{
			offscreen_rendering_ = enabled;
			offscreen_factor_ = factor;
		}

		void Scene::showText(const String& /*text*/, Size /*font_size*/)
		{
			// TODO: reactivate!
			/*
			info_text_ = text; 
			font_size_= font_size;

			updateGL(); // TODO: update() or updateGL()???
			*/
		}

		void Scene::addToolBarEntries(QToolBar* tb)
		{
			toolbar_view_controls_->addActions(toolbar_actions_view_controls_);
			toolbar_view_controls_->insertSeparator(fullscreen_action_);
			getMainControl()->addToolBar(Qt::TopToolBarArea, toolbar_view_controls_);
			ModularWidget::addToolBarEntries(tb);
			getMainControl()->initPopupMenu(MainControl::WINDOWS)->addAction(toolbar_view_controls_->toggleViewAction());	
		}

#ifdef BALL_HAS_RTFACT
		void Scene::updateAllRTMaterials()
		{
			RepresentationManager& pm = getMainControl()->getRepresentationManager();

			RepresentationList::const_iterator it = pm.getRepresentations().begin();
			for (; it != pm.getRepresentations().end(); ++it)
			{
				for (Position i=0; i<renderers_.size(); ++i)
				{
					renderers_[i]->updateMaterialForRepresentation(*it);
				}
			}

			updateGL();
		}

		void Scene::updateRTMaterialForRepresentation(Representation* rep, const Stage::RaytracingMaterial& new_material)
		{
			rep->clearProperty("RTFact::Material");
			boost::shared_ptr<PersistentObject> p(new Stage::RaytracingMaterial(new_material));
			NamedProperty rt_mat_property("RTFact::Material", p);

			rep->setProperty(rt_mat_property);

			rt_renderer_->updateMaterialForRepresentation(rep);

			updateGL();
		}
#endif

		void Scene::setupEnvironmentMap(const QImage& image)
		{
			for (Position i=0; i<renderers_.size(); ++i)
			{
				if (renderers_[i]->receivesBufferUpdates())
					renderers_[i]->setupEnvironmentMap(image);
			}
		}

		Position Scene::prepareGridTextures(const RegularData3D& grid, const ColorMap& map)
		{
			// NOTE: this implementation has a slight disadvantage:
			// 			 if you want 3d textures for stereo with different
			// 			 half images, you need to ensure that this function
			// 			 is called *after* switching to stereo!
			//
			// TODO: - change this to something more sensible!
			// 			 - call something in RenderSetup instead!	
			Position texname;

			for (Position i=0; i<renderers_.size(); ++i)
				texname = renderers_[i]->prepareGridTextures(grid, map);

			return texname;
		}

		void Scene::switchShowGrid()
		{
			// TODO: maybe we want this to be local to the targets?
			for (Position i=0; i<renderers_.size(); ++i)
				renderers_[i]->showRuler(switch_grid_->isChecked());

			updateGL();
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

		// TODO: make renderer configurable
		void Scene::pickParent_(QPoint p)
		{
			ignore_pick_ = true;
			list<GeometricObject*> objects;
			renderers_[0]->pickObjects((Position) p.x(), (Position) p.y(), 
																 (Position) p.x(), (Position) p.y(), objects);

			if (objects.empty()) return;
		
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

		void Scene::setupViewVolume()
		{
			bool ok;
			float value = QInputDialog::getDouble(this, "Setup view volume",
																						"Enter volume length:", 
																						20, 5, 200, 1, &ok);
			if (!ok) return;

			// find out if the visible viewing volume has to be cut
			RepresentationManager& pm = getMainControl()->getRepresentationManager();

			if (value != FLT_MAX)
			{
				const Camera& camera = stage_->getCamera();
				Vector3 n(-camera.getViewVector());
				n.normalize();
				Vector3 nr(camera.getRightVector());
				nr.normalize();
				Vector3 nu(camera.getLookUpVector());
				nu.normalize();

				for (Position p = 1; p < 6; p++)
				{
					ClippingPlane *plane = new ClippingPlane;

					if (p == 1)
					{
						plane->setPoint(camera.getLookAtPosition() - n * value);
						plane->setNormal(n);
					}
					else if (p == 2)
					{
						plane->setPoint(camera.getLookAtPosition() - nr * value);
						plane->setNormal(nr);
					}
					else if (p == 3)
					{
						plane->setPoint(camera.getLookAtPosition() + nr * value);
						plane->setNormal(-nr);
					}
					else if (p == 4)
					{
						plane->setPoint(camera.getLookAtPosition() - nu * value);
						plane->setNormal(nu);
					}
					else if (p == 5)
					{
						plane->setPoint(camera.getLookAtPosition() + nu * value);
						plane->setNormal(-nu);
					}

					pm.insertClippingPlane(plane);
				}
			}

			updateGL();
		}

		void Scene::restoreViewPoint()
		{
			getStage()->setCamera(stored_camera_);
			updateGL();
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

#ifdef BALL_HAS_RTFACT
		Scene::RaytracingWindowPtr Scene::getWindow(WindowType aWindowType)
		{
			switch(aWindowType)
			{
				case CONTROL_WINDOW:
				case LEFT_EYE_WINDOW:
				case RIGHT_EYE_WINDOW:
					return RaytracingWindowPtr(main_display_);
					break;

				default: 
					return RaytracingWindowPtr(main_display_);
					break;
			}
		}
#endif

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
