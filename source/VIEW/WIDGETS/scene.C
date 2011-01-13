/* TRANSLATOR BALL::VIEW::Scene

		Necessary for lupdate.
*/

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/WIDGETS/molecularStructure.h>
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
#include <BALL/VIEW/DIALOGS/PTEDialog.h>
#include <BALL/VIEW/DIALOGS/compositeProperties.h>

#include <BALL/VIEW/DATATYPE/standardDatasets.h>

#include <BALL/VIEW/RENDERING/RENDERERS/POVRenderer.h>
#include <BALL/VIEW/RENDERING/RENDERERS/VRMLRenderer.h>
#include <BALL/VIEW/RENDERING/RENDERERS/STLRenderer.h>
#include <BALL/VIEW/RENDERING/RENDERERS/tilingRenderer.h>
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
#include <BALL/VIEW/INPUT/notificationEvent.h>
#include <BALL/VIEW/INPUT/buttonEvent.h>

#include <BALL/SYSTEM/timer.h>
#include <BALL/SYSTEM/systemCalls.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/SYSTEM/directory.h>
#include <BALL/MATHS/quaternion.h>
#include <BALL/QSAR/ringPerceptionProcessor.h>

#include <BALL/STRUCTURE/assignBondOrderProcessor.h>
#include <BALL/STRUCTURE/geometricTransformations.h>
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/STRUCTURE/addHydrogenProcessor.h>

#include <BALL/VIEW/WIDGETS/datasetControl.h>
#include <BALL/VIEW/DATATYPE/colorMap.h>

#include <BALL/VIEW/RENDERING/RENDERERS/bufferedRenderer.h>

#ifdef BALL_HAS_RTFACT
    
#include <BALL/VIEW/RENDERING/glRenderWindow.h>
//#include <BALL/VIEW/RENDERING/RENDERERS/cudaVolumeRenderer.h>    
#include <BALL/VIEW/RENDERING/RENDERERS/rtfactRenderer.h>    

#endif

#include <QtGui/QMenuBar>
#include <QtGui/QPrinter>
#include <QtGui/QPrintDialog>
#include <QtGui/QPainter>
#include <QtGui/QImage>
#include <QtGui/QCursor>
#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#include <QtGui/QFileDialog>
#include <QtGui/QInputDialog>
#include <QtGui/QProgressBar>
#include <QtOpenGL/QGLPixelBuffer>
#include <QtGui/QMessageBox>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real.hpp>

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
				toolbar_view_controls_(new QToolBar(tr("3D View Controls"), this)),
				mode_group_(new QActionGroup(this)),
				main_display_(new GLRenderWindow(this)),
				main_renderer_(0),
				stereo_left_eye_(-1),
				stereo_right_eye_(-1),
				toolbar_edit_controls_(new QToolBar(tr("Edit Controls"), this))
			{
				initializeMembers_();

				stage_settings_ = new StageSettings(this);

				registerRenderers_();

				init();
				renderers_[main_renderer_]->resize(width(), height());
				renderers_[main_renderer_]->start();

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
				toolbar_view_controls_(new QToolBar(tr("3D View Controls"), this)),
				mode_group_(new QActionGroup(this)),
				main_display_(new GLRenderWindow(this)),
				main_renderer_(0),
				stereo_left_eye_(-1),
				stereo_right_eye_(-1),
				fragment_db_(),
				fragment_db_initialized_(false),
				toolbar_edit_controls_(new QToolBar(tr("Edit Controls"), this))
			{
				initializeMembers_();

				stage_settings_=new StageSettings(this);
#ifdef BALL_VIEW_DEBUG
				Log.error() << "new Scene (2) " << this << std::endl;
#endif

				registerRenderers_();

				// the widget with the MainControl
				registerWidget(this);

				setObjectName(name);
				setAcceptDrops(true);

				init();
				renderers_[main_renderer_]->resize(width(), height());
				renderers_[main_renderer_]->start();

				registerWidget(this);
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
				toolbar_view_controls_(new QToolBar(tr("3D View Controls"), this)),
				mode_group_(new QActionGroup(this)),
				main_display_(new GLRenderWindow(this)),
				main_renderer_(0),
				stereo_left_eye_(-1),
				stereo_right_eye_(-1)
			{
				initializeMembers_();

				stage_settings_=new StageSettings(this);
#ifdef BALL_VIEW_DEBUG
				Log.error() << "new Scene (3) " << this << std::endl;
#endif
				registerRenderers_();

				setObjectName(name);

				// the widget with the MainControl
				ModularWidget::registerWidget(this);
				setAcceptDrops(true);

				init();

				resize((Size) scene.renderers_[main_renderer_]->renderer->getWidth(),
				       (Size) scene.renderers_[main_renderer_]->renderer->getHeight());

				renderers_[main_renderer_]->resize(width(), height());
				renderers_[main_renderer_]->start();
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

			if (animation_thread_ != 0) delete animation_thread_;
		}

		void Scene::registerRenderers_()
		{
#ifndef BALL_HAS_RTFACT
			renderers_.push_back(boost::shared_ptr<RenderSetup>(new RenderSetup(gl_renderer_, main_display_, this, stage_)));
#else
			renderers_.push_back(boost::shared_ptr<RenderSetup>(new RenderSetup(&*rt_renderer_, main_display_, this, stage_)));
#endif
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

		// #################### Messaging ############################################
		void Scene::handleRepresentationMessage_(RepresentationMessage* rm)
		{
			bool needs_updategl = true;

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
		}

		void Scene::handleDatasetMessage_(DatasetMessage* dm)
		{
			if (!dm->isValid()) return;

			RegularData3DDataset* set = dynamic_cast<RegularData3DDataset*>(dm->getDataset());
			if (set == 0) return;

			switch (dm->getType())
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
		}

		void Scene::handleSceneMessage_(SceneMessage *sm)
		{
			switch (sm->getType())
			{
				case SceneMessage::REDRAW:
					updateGL();
					return;

				case SceneMessage::REBUILD_DISPLAY_LISTS:
					// TODO: this needs to a complete rebuffering!!!!
					updateGL();
					return;

				case SceneMessage::UPDATE_CAMERA:
					stage_->getCamera() = sm->getStage().getCamera();
					system_origin_ = sm->getStage().getCamera().getLookAtPosition();
					light_settings_->updateFromStage();
					updateGL();
					return;

				case SceneMessage::REMOVE_COORDINATE_SYSTEM:
					stage_->showCoordinateSystem(false);
					stage_settings_->updateFromStage();
					return;

				case SceneMessage::EXPORT_PNG:
					{
						// did we get a filename with the message?
						String filename = "";
						if (sm->data().type() == typeid(String))
						{
							filename = boost::any_cast<String>(sm->data());
						}

						if (filename != "")
							exportPNG(filename);
						else
							filename = exportPNG();

						SceneMessage *finished_message = new SceneMessage(SceneMessage::EXPORT_FINISHED);
						finished_message->data() = filename;

						notify_(finished_message);

						return;
					}

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
					Log.error() << (String)tr("Unknown type of SceneMessage in ") << __FILE__ << __LINE__ << std::endl;
				default:
					break;
			}
		}

		void Scene::handleCompositeMessage_(CompositeMessage* cm)
		{
			switch (cm->getType())
			{
				case CompositeMessage::REMOVED_COMPOSITE:
				case CompositeMessage::CHANGED_COMPOSITE_HIERARCHY:
					current_atom_ = 0;
					current_bond_ = 0;
					break;
				default:
					break;
			}
		}

		void Scene::handleControlSelectionMessage_(ControlSelectionMessage* /*csm*/)
		{
			checkMenu(*getMainControl());
		}	

		void Scene::onNotify(Message *message)
		{
#ifdef BALL_VIEW_DEBUG
			Log.error() << "Scene " << this  << "onNotify " << message << std::endl;
#endif

			if (RTTI::isKindOf<CompositeMessage>(*message))
			{
				CompositeMessage* cm = RTTI::castTo<CompositeMessage>(*message);
				handleCompositeMessage_(cm);
			}
			else if (RTTI::isKindOf<ControlSelectionMessage>(*message))
			{
				ControlSelectionMessage* csm = RTTI::castTo<ControlSelectionMessage>(*message);
				handleControlSelectionMessage_(csm);
			}
			else if (RTTI::isKindOf<RepresentationMessage>(*message)) 
			{
				RepresentationMessage* rm = RTTI::castTo<RepresentationMessage>(*message);
				handleRepresentationMessage_(rm);
			} 
			else if (RTTI::isKindOf<DatasetMessage>(*message))
			{
				DatasetMessage* dm = RTTI::castTo<DatasetMessage>(*message);
				handleDatasetMessage_(dm);
			}
			else if (RTTI::isKindOf<SceneMessage>(*message))
			{
				SceneMessage *sm = RTTI::castTo<SceneMessage>(*message);
				handleSceneMessage_(sm);
			}

			// all other messages are ignored
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

			fps_string = String(tr("FPS ")) + fps_string;

			return fps_string;
		}

		void Scene::renderText_(QPointF const& point, String const& text, QPaintDevice* current_dev)
		{
			ColorRGBA text_color = stage_->getBackgroundColor().getInverseColor();

			QPainter p(current_dev);

			QPen pen(QColor((int)text_color.getRed(),  (int)text_color.getGreen(), 
						          (int)text_color.getBlue(), (int)text_color.getAlpha()));

			p.setPen(pen);
			p.setRenderHint(QPainter::Antialiasing, true);
			p.setRenderHint(QPainter::TextAntialiasing, true);

			p.drawText(point, text.c_str());

			p.end();
		}

		void Scene::paintGL()
		{
			update_running_ = true;

			// This function tries to sync renderers by letting (a) all renderers
			// perform their updates and then (b) swap in the newly created buffers
			// of all render targets

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

			update_running_ = false;
		}

		void Scene::paintEvent(QPaintEvent* /*e*/)
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

		void Scene::rotateClockwise(float degree)
		{
			stage_->getCamera().rotateAboutView(degree);
			updateGL();
		}

		void Scene::move(Vector3 v)
		{
			Camera& camera = stage_->getCamera();

			camera.translate(-camera.convertCameraToSceneCoordinates(v));
			updateGL();
		}

		void Scene::rotate(float degree_right, float degree_up)
		{
			Camera& camera = stage_->getCamera();

			Quaternion q1;
			q1.fromAxisAngle(camera.getLookUpVector(), Angle(degree_right, false).toRadian());

			Quaternion q2;
			q2.fromAxisAngle(camera.getRightVector(), Angle(degree_up, false).toRadian());

			q1 += q2;

			camera.rotate(q1, system_origin_);
			updateGL();
		}

		void Scene::moveComposites(const list<Composite*>& composites, Vector3 v)
		{
			HashSet<Composite*> roots;

			Vector3 x = stage_->getCamera().convertCameraToSceneCoordinates(v);

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

			if (stage_->getCamera().getProjectionMode() == Camera::PERSPECTIVE)
			{
				move(Vector3(0, 0, y));
			}
			else
			{
				float scale = std::max(0.05, 1.-y);

				gl_renderer_->setOrthographicZoom(gl_renderer_->getOrthographicZoom() * scale);
				gl_renderer_->initPerspective();

				updateGL();
			}
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
			renderers_[main_renderer_]->pickObjects((Position)p0.x(), (Position)p0.y(), 
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

			String text((String)tr("ViewPoint:") + " ("
					+ String(camera.getViewPoint().x) + "|" 
					+ String(camera.getViewPoint().y) + "|" 
					+ String(camera.getViewPoint().z) 
					+ ")   " + (String)tr("LookAt:") + " (" 
					+ String(camera.getLookAtPosition().x) + "|" 
					+ String(camera.getLookAtPosition().y) + "|" 
					+ String(camera.getLookAtPosition().z) 
					+ ")   " + (String)tr("LookUp") + ": (" 
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
				MainControl *main_control = MainControl::getMainControl(this);
				RepresentationManager& rman = main_control->getRepresentationManager();

				QProgressBar* progress_bar_ = new QProgressBar();
				progress_bar_->setWindowFlags(Qt::FramelessWindowHint);

				progress_bar_->setMaximum(rman.getRepresentations().size());
				progress_bar_->setValue(0);
				progress_bar_->move(
					main_control->width()  / 2 - 100,
					main_control->height() / 2 - 20
				);
				progress_bar_->setFixedWidth(200);
				progress_bar_->setFixedHeight(40);
				progress_bar_->show();

				RepresentationList::const_iterator it = rman.getRepresentations().begin();
				for (; it != rman.getRepresentations().end(); it++)
				{
					if (!er.renderOneRepresentation(**it))
					{
						getMainControl()->setStatusbarText((String)tr("Error rendering representation..."));
						return false;
					}

					progress_bar_->setValue(progress_bar_->value() + 1);
					QApplication::processEvents();
				}

				delete progress_bar_;

				if (er.finish())
				{
					// cant call Scene::setStatusbarText(..), no idea why!!!
					getMainControl()->setStatusbarText((String)tr("Successfully exported Scene..."));
					return true;
				}
			}

			getMainControl()->setStatusbarText((String)tr("Error while exporting Scene..."));
			return false;
		}

		//##########################PREFERENCES#################################
		// Preferences shall remain untranslated
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

			edit_settings_ = new EditSettings(this);
			preferences.insertEntry(edit_settings_);
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

			if (edit_settings_)
			{
				preferences.removeEntry(edit_settings_);
				edit_settings_ = 0;
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

			// currently, only the GLRenderer knows how to handle fog...
			// TODO: Fog in other renderers!
			for (Position i=0; i<renderers_.size(); ++i)
			{
				if (renderers_[i]->getRendererType() == RenderSetup::OPENGL_RENDERER)
					dynamic_cast<GLRenderer*>(renderers_[i]->renderer)->setFogIntensity((float)stage_->getFogIntensity());
			}

			updateGL();

			if (edit_settings_ == 0) return;
			only_highlighted_ = edit_settings_->only_highlighted->isChecked();

			if ((int)current_mode_ == (int)EDIT__MODE)
			{
				setElementCursor();
			}
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
				Log.error() << (String)tr("Could not read lighting settings from Inifile") << std::endl;
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
				addMenu(tr("Show Coordinate System"));
			setMenuHint((String)tr("Show a coordinate system"));

			new_action = create_coordinate_system_->addAction(tr("at origin"), this, SLOT(createCoordinateSystemAtOrigin()));
			shortcut_registry->registerShortcut("Shortcut|Display|Show_Coordinate_System|at_origin", new_action);

			new_action = create_coordinate_system_->addAction(tr("here"), this, SLOT(createCoordinateSystem()));
			shortcut_registry->registerShortcut("Shortcut|Display|Show_Coordinate_System|here", new_action);

			insertMenuEntry(MainControl::DISPLAY, (String)tr("Add new GL Window"), this, SLOT(addGlWindow()), "Shortcut|Display|Add_new_GL_Window");
#ifdef BALL_HAS_RTFACT
			insertMenuEntry(MainControl::DISPLAY, (String)tr("Add new RTfact Window"), this, SLOT(addRTfactWindow()), "Shortcut|Display|Add_new_RTfact_Window");
#endif
			// ======================== Display->Animation ===============================================
			String help_url = "tips.html#animations";

			record_animation_action_ = insertMenuEntry(MainControl::DISPLAY_ANIMATION, (String)tr("Record"), this, 
					SLOT(dummySlot()), "Shortcut|Display|Animation|Record");
			setMenuHint((String)tr("Record an animation for later processing"));
			setMenuHelp(help_url);
			record_animation_action_->setCheckable(true);

			clear_animation_action_ = insertMenuEntry(MainControl::DISPLAY_ANIMATION, (String)tr("Clear"), this, 
					SLOT(clearRecordedAnimation()), "Shortcut|Display|Animation|Clear");
			setMenuHelp(help_url);

			main_control.insertPopupMenuSeparator(MainControl::DISPLAY_ANIMATION);

			start_animation_action_ = insertMenuEntry(MainControl::DISPLAY_ANIMATION, (String)tr("Start"), this, 
					SLOT(startAnimation()), "Shortcut|Display|Animation|Start");
			setMenuHelp(help_url);

			cancel_animation_action_ = insertMenuEntry(MainControl::DISPLAY_ANIMATION, (String)tr("Stop"), this, 
					SLOT(stopAnimation()), "Shortcut|Display|Animation|Stop");
			cancel_animation_action_->setEnabled(false);
			setMenuHelp(help_url);

			main_control.insertPopupMenuSeparator(MainControl::DISPLAY_ANIMATION);

			animation_export_PNG_action_ = insertMenuEntry(MainControl::DISPLAY_ANIMATION, (String)tr("Export PNG"), this, 
					SLOT(dummySlot()), "Shortcut|Display|Animation|Export_PNG");
			setMenuHelp(help_url);
			animation_export_PNG_action_->setCheckable(true);

			animation_export_POV_action_ = insertMenuEntry(MainControl::DISPLAY_ANIMATION, (String)tr("Export POV"), this, 
					SLOT(dummySlot()), "Shortcut|Display|Animation|Export_POV");
			setMenuHelp(help_url);
			animation_export_POV_action_->setCheckable(true);

			animation_repeat_action_ = insertMenuEntry(MainControl::DISPLAY_ANIMATION, (String)tr("Repeat"), this, 
					SLOT(dummySlot()), "Shortcut|Display|Animation|Repeat");
			setMenuHelp(help_url);
			animation_repeat_action_->setCheckable(true);

			// ======================== Display->Stereo ===============================================
			main_control.insertPopupMenuSeparator(MainControl::DISPLAY);

			no_stereo_action_ = insertMenuEntry(MainControl::DISPLAY_STEREO, (String)tr("No Stereo"), this, 
					SLOT(exitStereo()), "Shortcut|Display|Stereo|No_Stereo");
			no_stereo_action_->setCheckable(true);
			no_stereo_action_->setChecked(true);
			setMenuHelp("tips.html#3D");

			active_stereo_action_ = insertMenuEntry(MainControl::DISPLAY_STEREO, (String)tr("Shutter Glasses"), this, 
					SLOT(enterActiveStereo()), "Shortcut|Display|Stereo|Shutter_Glasses");
			setMenuHelp("tips.html#3D");
			active_stereo_action_->setCheckable(true);

			dual_stereo_action_ = insertMenuEntry(MainControl::DISPLAY_STEREO, (String)tr("Side by Side"), this,
					SLOT(enterDualStereo()), "Shortcut|Display|Stereo|Side_by_Side");
			setMenuHelp("tips.html#3D");
			dual_stereo_action_->setCheckable(true);

			dual_stereo_different_display_action_ = insertMenuEntry(MainControl::DISPLAY_STEREO, 
					(String)tr("Side by Side on Different Displays"), this, 
					SLOT(enterDualStereoDifferentDisplays()),
					"Shortcut|Display|Stereo|Side_by_Side_on_Different_Displays");
			setMenuHelp("tips.html#3D");
			dual_stereo_different_display_action_->setCheckable(true);

			// ======================== Display->Viewpoint ===============================================
			getMainControl()->insertPopupMenuSeparator(MainControl::DISPLAY_VIEWPOINT);

			insertMenuEntry(MainControl::DISPLAY_VIEWPOINT, (String)tr("&Store Viewpoint"), this, 
					SLOT(storeViewPoint()), "Shortcut|Display|Viewpoint|Store");
			setMenuHint((String)tr("Store the current viewpoint"));

			insertMenuEntry(MainControl::DISPLAY_VIEWPOINT, (String)tr("&Restore Viewpoint"), this, 
					SLOT(restoreViewPoint()), "Shortcut|Display|Viewpoint|Restore");
			setMenuHint((String)tr("Restore the viewpoint"));

			getMainControl()->insertPopupMenuSeparator(MainControl::DISPLAY_VIEWPOINT);

			String description("Shortcut|Display|Viewpoint|Show_Vie&wpoint");
			insertMenuEntry(MainControl::DISPLAY_VIEWPOINT, (String)tr("Show Viewpoint"), this, 
					SLOT(showViewPoint_()), description, QKeySequence("Ctrl+W"));
			setMenuHint((String)tr("Print the coordinates of the current viewpoint"));


			description = "Shortcut|Display|Viewpoint|Set_Viewpoint";
			insertMenuEntry(MainControl::DISPLAY_VIEWPOINT, (String)tr("Set Viewpoi&nt"), this, 
					SLOT(setViewPoint_()), description, QKeySequence("Ctrl+N"));
			setMenuHint((String)tr("Move the viewpoint to the given coordinates"));

			insertMenuEntry(MainControl::DISPLAY_VIEWPOINT, (String)tr("Rese&t Camera"), this, 
					SLOT(resetCamera_()), "Shortcut|Display|Viewpoint|Reset_Camera");
			setMenuHint((String)tr("Reset the camera to the orgin (0,0,0)"));

			getMainControl()->insertPopupMenuSeparator(MainControl::DISPLAY_VIEWPOINT);
			insertMenuEntry(MainControl::DISPLAY_VIEWPOINT, (String)tr("Limit View Volume"), this, 
					SLOT(setupViewVolume()), "Shortcut|Display|Viewpoint|Limit_View_Volume");

			description = "Shortcut|File|Export|PNG";
			QAction* screenshot_action = insertMenuEntry(MainControl::FILE_EXPORT, (String)tr("PNG..."), this, 
					SLOT(showExportPNGDialog()), description, QKeySequence("Alt+P"));
			setMenuHint((String)tr("Export a PNG image file from the Scene"));
			setIcon("actions/screenshot", false);

			description = "Shortcut|File|Export|POVRay";
			insertMenuEntry(MainControl::FILE_EXPORT, (String)tr("POVRa&y scene"), this, 
					SLOT(exportPOVRay()), description, QKeySequence("Ctrl+Y"));
			setIcon("mimetype/text-x-povray", false);

			setMenuHint((String)tr("Export a POVRay file from the Scene"));
			setMenuHelp("tips.html#povray");

			description = "Shortcut|File|Export|VRML";
			insertMenuEntry(MainControl::FILE_EXPORT, (String)tr("3D Prototyping Export"), this, 
					SLOT(showExportVRMLDialog()), description);
			setMenuHint((String)tr("Export a VRML or stl file from the scene"));

			// ====================================== MODES =====================================
			description = "Shortcut|Display|Rotate_Mode";
			main_control.insertPopupMenuSeparator(MainControl::DISPLAY);
			rotate_action_ = insertMenuEntry(MainControl::DISPLAY, (String)tr("&Rotate Mode"), this, 
					SLOT(rotateMode_()), description, QKeySequence("Ctrl+R"));

			setMenuHint((String)tr("Switch to rotate/zoom mode"));
			setMenuHelp("scene.html#rotate_mode");
			rotate_action_->setCheckable(true);
			setIcon("actions/transform-rotate", false);
			toolbar_actions_view_controls_.push_back(rotate_action_);
			mode_group_->addAction(rotate_action_);

			description = "Shortcut|Display|Picking_Mode";
			picking_action_ = insertMenuEntry(MainControl::DISPLAY, (String)tr("&Picking Mode"), this, 
					SLOT(pickingMode_()), description, QKeySequence("Ctrl+P"));

			setMenuHint((String)tr("Switch to picking mode, e.g. to identify single atoms or groups"));
			setMenuHelp("scene.html#identify_atoms");
			setIcon("actions/select-rectangular", false);
			picking_action_->setCheckable(true);
			toolbar_actions_view_controls_.push_back(picking_action_);
			mode_group_->addAction(picking_action_);

			description = "Shortcut|Display|Move_Mode";
			move_action_ = insertMenuEntry(MainControl::DISPLAY, (String)tr("Move Mode"), this, 
					SLOT(moveMode_()), description);

			setMenuHint((String)tr("Switch to move mode, e.g. move selected items"));
			setMenuHelp("molecularControl.html#move_molecule");
			setIcon("actions/transform-move", false);
			move_action_->setCheckable(true);
			toolbar_actions_view_controls_.push_back(move_action_);
			mode_group_->addAction(move_action_);

			description = "Shortcut|ShowRuler";
			switch_grid_ = new QAction(tr("Show ruler"), this);
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
			// end of the toolbar entries
#endif

			description = "Shortcut|File|Print";
			insertMenuEntry(MainControl::FILE, (String)tr("Print"), this, SLOT(printScene()), description);

			window_menu_entry_ = insertMenuEntry(MainControl::WINDOWS, (String)tr("Scene"), this, SLOT(switchShowWidget()));
			window_menu_entry_->setCheckable(true);
			setMenuHelp("scene.html");

			QWidget::setCursor(QCursor(Qt::ArrowCursor));

			setFocusPolicy(Qt::StrongFocus);
			registerForHelpSystem(this, "scene.html");

			toolbar_view_controls_->setObjectName("3D View Control toolbar");
			toolbar_view_controls_->setIconSize(QSize(22,22));
			toolbar_view_controls_->layout()->setMargin(2);
			toolbar_view_controls_->layout()->setSpacing(2);

			/// EDITABLE SCENE STUFF
			help_url = "scene.html#editing";

			description = "Shortcut|Display|Edit_Mode";
			edit_id_ = insertMenuEntry(MainControl::DISPLAY, (String)tr("Edit Mode"), this,
					SLOT(editMode_()), description, QKeySequence("Ctrl+E"));
			setMenuHint((String)tr("Create and modify molecular structures"));
			edit_id_->setToolTip(tr("Switch to edit mode, e.g. draw your own molecule"));

			edit_id_->setCheckable(true);
			setIcon("actions/mode-edit", false);
			setMenuHelp(help_url);
			mode_group_->addAction(edit_id_);

			main_control.insertPopupMenuSeparator(MainControl::DISPLAY);

			description = "Shortcut|QuicklyAssignBondOrders";
			bondorders_action_ = new QAction(loader.getIcon("actions/molecule-assign-bond-orders"), tr("Quickly optimize bond orders"), this);
			bondorders_action_->setObjectName(bondorders_action_->text());
			bondorders_action_->setToolTip(tr("Compute the bond orders of the highlighted structures"));
			//TODO
			//registerForHelpSystem(bondorders_action_, "scene.html#bondorders");
			connect(bondorders_action_, SIGNAL(triggered()), this, SLOT(computeBondOrders()));
			getMainControl()->getShortcutRegistry().registerShortcut(description, bondorders_action_);

			description = "Shortcut|QuicklyOptimizeStructure";
			optimize_action_ = new QAction(loader.getIcon("actions/molecule-minimize"), tr("Quickly optimize structure"), this);
			optimize_action_->setObjectName(optimize_action_->text());
			optimize_action_->setToolTip(tr("Quickly optimize the highlighted structure"));
			registerForHelpSystem(optimize_action_, "scene.html#optimize");
			connect(optimize_action_, SIGNAL(triggered()), this, SLOT(optimizeStructure()));
			getMainControl()->getShortcutRegistry().registerShortcut(description, optimize_action_);

			description = "Shortcut|SaturateWithHydrogens";
			add_hydrogens_action_ = new QAction(loader.getIcon("actions/molecule-add-hydrogens"), tr("Saturate with Hydrogens"), this);
			add_hydrogens_action_->setToolTip(tr("Saturate the highlighted structure with hydrogens (with regards to formal charges)"));
			add_hydrogens_action_->setObjectName(add_hydrogens_action_->text());
			registerForHelpSystem(add_hydrogens_action_, "scene.html#saturate");
			connect(add_hydrogens_action_, SIGNAL(triggered()), this, SLOT(saturateWithHydrogens()));
			getMainControl()->getShortcutRegistry().registerShortcut(description, add_hydrogens_action_);

			getMainControl()->initPopupMenu(MainControl::BUILD)->addAction(add_hydrogens_action_);

			description = "Shortcut|EditMode|SetElement";
			element_action_ = new QAction(loader.getIcon("actions/molecule-set-element"), tr("Set element"), this);
			element_action_->setToolTip(tr("Edit mode: Choose element for next atom, to modify atom under cursor: Double left click"));
			element_action_->setObjectName(element_action_->text());
			registerForHelpSystem(element_action_, "scene.html#choose_element");
			connect(element_action_, SIGNAL(triggered()), this, SLOT(changeElement_()));
			getMainControl()->getShortcutRegistry().registerShortcut(description, element_action_);
			QMenu* qmenu = getMainControl()->initPopupMenu(MainControl::BUILD);
			qmenu->addAction(element_action_);

			description = "Shortcut|EditMode|CreateBond";
			bond_action_ = new QAction(loader.getIcon("actions/create-bond"), tr("Create Bond"), this);
			bond_action_->setToolTip(tr("Edit mode: If two atoms are selected, create a single bond between them"));
			bond_action_->setObjectName(bond_action_->text());
			registerForHelpSystem(bond_action_, "scene.html#create_bond");
			//TODO registerForHelpSystem not done yet
			connect(bond_action_, SIGNAL(triggered()), this, SLOT(createBond_()));
			getMainControl()->getShortcutRegistry().registerShortcut(description, bond_action_);
			// 			qmenu = getMainControl()->initPopupMenu(MainControl::BUILD);
			qmenu->addAction(bond_action_);


			//TODO create an icon
			new_molecule_action_ = insertMenuEntry(MainControl::BUILD, (String)tr("Create new molecule"),
					this, SLOT(createNewMolecule()), "Shortcut|Build|Create_new_molecule");
			setMenuHint((String)tr("Create a new molecule for editing"));

			toolbar_edit_controls_->setObjectName(tr("Edit Control toolbar"));
			toolbar_edit_controls_->setIconSize(QSize(23,23));
			toolbar_edit_controls_->layout()->setMargin(2);
			toolbar_edit_controls_->layout()->setSpacing(2);
		}

		void Scene::checkMenu(MainControl& main_control)
		{
			bool busy = main_control.compositesAreLocked();
			edit_id_->setChecked(current_mode_ == (Scene::ModeType)EDIT__MODE);
			edit_id_->setEnabled(!busy);
			rotate_action_->setChecked(current_mode_ == ROTATE__MODE);
			picking_action_->setChecked(current_mode_ == PICKING__MODE);
			picking_action_->setEnabled(!busy);
			move_action_->setChecked(current_mode_ == MOVE__MODE);
			move_action_->setEnabled(!busy);

			create_coordinate_system_->setEnabled(!busy);

			bool animation_running = (animation_thread_ != 0 && animation_thread_->isRunning());

			start_animation_action_->setEnabled(animation_points_.size() > 0 && 
					!busy && !animation_running);

			clear_animation_action_->setEnabled(animation_points_.size() > 0 && !animation_running);

			window_menu_entry_->setChecked(isVisible());
			bool edit_mode = (current_mode_ == (Scene::ModeType)EDIT__MODE);
			bool selected_system = !busy && main_control.getSelectedSystem();

			optimize_action_->setEnabled(selected_system);
			add_hydrogens_action_->setEnabled(selected_system);

			list<Composite*> highl = getMainControl()->getMolecularControlSelection();
			list<Composite*>::iterator lit = highl.begin();
			bool selected_system_or_molecule =   (highl.size() == 1)
				&& (RTTI::isKindOf<System>(**lit) || RTTI::isKindOf<Molecule>(**lit) ) ;

			bondorders_action_->setEnabled(selected_system_or_molecule && !busy);
			optimize_action_->setEnabled(selected_system_or_molecule && !busy);
			add_hydrogens_action_->setEnabled(selected_system_or_molecule && !busy);

			element_action_->setEnabled(!busy && edit_mode);

			new_molecule_action_->setEnabled(!busy);

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

		void Scene::handleRenderToBufferFinishedEvent_(RenderToBufferFinishedEvent* evt)
		{
			RenderSetup* renderer = evt->getRenderer();

			renderer->makeCurrent();
			// NOTE: GLRenderers currently render in the GUI thread!
			if (RTTI::isKindOf<GLRenderer>(*(renderer->renderer)))
				renderer->renderToBuffer();
			else
				renderer->target->refresh();

			renderer->setBufferReady(true);

			// then, estimate fps if necessary and add to the render target
			String fps_string;
			QPoint fps_point;

			if (show_fps_)
				fps_string = createFPSInfo_();

			// draw all renderable texts 
			// TODO: does this work for dependent renderers?
			QPaintDevice* current_dev = dynamic_cast<QPaintDevice*>(renderer->target);

			if (show_fps_ && current_dev)
			{
				QFontMetrics fm(default_font_);
				QRect r = fm.boundingRect(fps_string.c_str());
				QPointF fps_point((float)current_dev->width() - 20 - r.width(), 20);

				renderText_(fps_point, fps_string.c_str(), current_dev);
			}

			if (info_string_ != "")
			{
				// account for differently sized windows
				float xscale = current_dev->width()  / width();
				float yscale = current_dev->height() / height();

				renderText_(QPointF(info_point_.x()*xscale, info_point_.y()*yscale), info_string_.c_str(), current_dev);
			}

			// and paint our overlay, if we have one
			if (has_overlay_)
			{
				QPainter painter(current_dev);
				painter.drawPicture(0, 0, overlay_);
				painter.end();
			}
			// implements a trivial synchronization mechanism: if
			// two renderers depend on one another, their images will
			// be swapped in at the same time
			std::deque<boost::shared_ptr<RenderSetup> >& dependent_renderers = renderer->getDependentRenderers();

			// find out if all renderers are ready
			bool ready_to_swap = true;
			for (std::deque<boost::shared_ptr<RenderSetup> >::iterator render_it  = dependent_renderers.begin();
					render_it != dependent_renderers.end(); ++render_it)
			{
				ready_to_swap &= (*render_it)->bufferIsReady();
			}

			if (ready_to_swap)
			{
				// paint all buffers
				if (RTTI::isKindOf<GLRenderWindow>(*(renderer->target)))
					static_cast<GLRenderWindow*>(renderer->target)->swapBuffers();

				if (renderer->isContinuous() && (renderer->getTimeToLive() != 0))
				{
					renderer->loop_mutex.lock();
					renderer->wait_for_render.wakeAll();
					renderer->loop_mutex.unlock();
				}

				for (std::deque<boost::shared_ptr<RenderSetup> >::iterator render_it  = dependent_renderers.begin();
						render_it != dependent_renderers.end(); ++render_it)
				{
					(*render_it)->makeCurrent();

					if (RTTI::isKindOf<GLRenderWindow>(*((*render_it)->target)))
						static_cast<GLRenderWindow*>((*render_it)->target)->swapBuffers();

					if ((*render_it)->isContinuous() && ((*render_it)->getTimeToLive() != 0))
					{
						(*render_it)->loop_mutex.lock();
						(*render_it)->wait_for_render.wakeAll();
						(*render_it)->loop_mutex.unlock();
					}
				}
			}

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
		}

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
				case NOTIFICATION_EVENT:
					onNotify(static_cast<NotificationEvent*>(evt)->getMessage());
					notify_(static_cast<NotificationEvent*>(evt)->getMessage());
					break;
				case RENDER_TO_BUFFER_FINISHED_EVENT: 
					handleRenderToBufferFinishedEvent_(static_cast<RenderToBufferFinishedEvent*>(evt));
					break;
				default:
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
			q1.fromAxisAngle(camera.getLookUpVector(), 0.3 * Angle(-evt->getRotation().y, false).toRadian());

			Quaternion q2;
			q2.fromAxisAngle(camera.getRightVector(),  0.3 * Angle(-evt->getRotation().x, false).toRadian());

			Quaternion q3;
			q3.fromAxisAngle(camera.getViewVector(),   0.3 * Angle(-evt->getRotation().z, false).toRadian());

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

		// TODO: make the renderer dependent on the target
		void Scene::mouseMoveEvent(QMouseEvent* e)
		{
			if (current_mode_ != (Scene::ModeType) EDIT__MODE)
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

				return;
			}

			if (isAnimationRunning() || getMainControl()->isBusy()) return;

			if (temp_move_)
			{
				x_window_pos_new_ = e->globalX();
				y_window_pos_new_ = e->globalY();

				processMoveModeMouseEvents_(e);

				x_window_pos_old_ = x_window_pos_new_;
				y_window_pos_old_ = y_window_pos_new_;
				return;
			}

			// create a new bond
			//
			// is there an atom nearby the actual mouse position?
			Atom* last_atom = current_atom_;
			getClickedItems_(e->x(), e->y());

			Vector2 draw_to;

			// have we found such an atom? if so, is it different from the one we started with?
			// (self bonds make no sense)
			if (last_atom &&  current_atom_ &&
					last_atom != current_atom_ &&
					// workaround against crashes:
					&last_atom->getRoot() == &current_atom_->getRoot())
			{
				// if we are really close to an atom, the endpoints of the line we draw will be set to
				// its center, so that the user has a drop in effect for the bonds
				atom_pos_ = current_atom_->getPosition();
				draw_to   = renderers_[main_renderer_]->map3DToViewport(atom_pos_);
			}
			else
			{
				draw_to.x = e->x();
				draw_to.y = e->y();
			}

			current_atom_ = last_atom;

			updateGL();

			if (current_atom_ == 0)
				return;

			Vector2 draw_from = renderers_[main_renderer_]->map3DToViewport(current_atom_->getPosition());

			// paint the line representing the offered bond into a suitable QPicture which will
			// later be painted on top of our display
			QPainter p(&overlay_);

			QColor color;
			stage_->getBackgroundColor().getInverseColor().get(color);

			QPen pen(color);
			pen.setWidth(3);
			pen.setStyle(Qt::DashLine);

			p.setPen(pen);
			p.setRenderHint(QPainter::Antialiasing, true);
			p.drawLine(QPointF(draw_from.x, draw_from.y), QPointF(draw_to.x, draw_to.y));
			p.end();

			has_overlay_ = true;

			x_window_pos_old_ = x_window_pos_new_;
			y_window_pos_old_ = y_window_pos_new_;
			x_window_pos_new_ = e->x();
			y_window_pos_new_ = e->y();

			///////////////////////////////////////////////////
		}

		void Scene::mousePressEvent(QMouseEvent* e)
		{
			if (current_mode_ != (Scene::ModeType) EDIT__MODE)
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

				return;
			}

			if (isAnimationRunning() || getMainControl()->isBusy()) return;

			deselect_();

			x_window_pos_old_ = x_window_pos_new_;
			y_window_pos_old_ = y_window_pos_new_;
			x_window_pos_new_ = e->x();
			y_window_pos_new_ = e->y();
			mouse_button_is_pressed_ = true;
			last_buttons_ = e->buttons();

			if (e->button() != Qt::RightButton)
			{
				if (only_highlighted_ &&
						getMainControl()->getMolecularControlSelection().size() == 0)
				{
					setStatusbarText((String)tr("Warning: no AtomContainer highlighted"), true);
					return;
				}
			}

			getClickedItems_(e->x(), e->y());

			/////////////////////////////////////////
			// right button -> context menu
			if (e->button() == Qt::RightButton)
			{
				if (current_atom_)
				{
					current_atom_->select();
					notify_(new CompositeMessage(*current_atom_, CompositeMessage::SELECTED_COMPOSITE));
				}

				if (current_bond_)
				{
					current_bond_->select();
					Atom* a1 = (Atom*)current_bond_->getFirstAtom();
					Atom* a2 = (Atom*)current_bond_->getSecondAtom();
					a1->select();
					a2->select();
					notify_(new CompositeMessage(*a1, CompositeMessage::SELECTED_COMPOSITE));
					notify_(new CompositeMessage(*a2, CompositeMessage::SELECTED_COMPOSITE));
				}

				// we open a context menu at this point
				showContextMenu(QPoint(e->x(), e->y()));
				return;
			}

			////////////////////////////////////////////////
			// left button -> add atom or move existing atom
			if (e->button() == Qt::LeftButton && e->modifiers() != Qt::ControlModifier)
			{
				if (current_bond_ != 0) return;

				if (current_atom_ != 0)
				{
					getMainControl()->selectCompositeRecursive(current_atom_, true);
					x_window_pos_new_ = x_window_pos_old_ = e->globalX();
					y_window_pos_new_ = y_window_pos_old_ = e->globalY();
					temp_move_ = true;
					return;
				}

				/////////////////////////////////////////
				// insert a new atom:
				String name = PTE[atomic_number_].getSymbol();
				name += String(atom_number_);
				atom_number_ ++;
				PDBAtom* a = new PDBAtom(PTE[atomic_number_], name);
				insert_(e->x(), e->y(), *a);
				current_atom_ = a;

				//store the Operation in undo_
				Vector3 atom_position = a->getPosition();

				EditOperation eo(a, NULL, (String)tr("Added atom of type ") + PTE[atomic_number_].getName() + (String)tr(" at position (")
						+ String(atom_position.x) + ", "
						+ String(atom_position.y) + ", "
						+ String(atom_position.z) + ")", EditOperation::ADDED__ATOM);
				undo_.push_back(eo);

				// tell about the new undo operation
				emit newEditOperation(eo);
				return;
			}

			//////////////////////////////////////////////
			// middle button -> add bond
			if (e->button() == Qt::MidButton ||
					(e->button() == Qt::LeftButton && e->modifiers() == Qt::ControlModifier))
			{
				if (current_atom_)
				{
					current_atom_->select();
					notify_(new CompositeMessage(*current_atom_, CompositeMessage::SELECTED_COMPOSITE));
					atom_pos_ = current_atom_->getPosition();
				}

				return;
			}

			///////////////////////////////////////
			///////////////////////////////////////
		}

		Index Scene::getMoveModeAction_(const QMouseEvent& e)
		{
			// TODO make keys configurable in shortcutRegistry 
			if (e.modifiers() == Qt::NoModifier)
			{
				if (e.buttons() == Qt::LeftButton)  return TRANSLATE_ACTION;
				if (e.buttons() == Qt::MidButton)   return ZOOM_ACTION;
				if (e.buttons() == Qt::RightButton) return ROTATE_ACTION;
			}
			else if (e.buttons() == Qt::LeftButton)
			{
				if (e.modifiers() == Qt::ShiftModifier) return ZOOM_ACTION;
				if (e.modifiers() == Qt::ControlModifier) return ROTATE_ACTION;
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


		void Scene::defaultMouseReleaseEvent(QMouseEvent* e)
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
				renderers_[main_renderer_]->pickObjects(pos_x - p, pos_y - p, pos_x + p, pos_y + p, objects);
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

			String string2 = String(tr("Object at cursor is ")) + string;

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

		void Scene::defaultWheelHandling_(QWheelEvent* qmouse_event)
		{
			info_string_ = "";

			qmouse_event->accept();

			y_window_pos_new_ = (Position)(y_window_pos_old_ + (qmouse_event->delta() / 120. * mouse_wheel_sensitivity_));
			zoomSystem_();
			y_window_pos_old_ = y_window_pos_new_;
		}

		void Scene::defaultKeyPressEvent(QKeyEvent* e)
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
			switch(mode)
			{
				case ROTATE__MODE:  rotateMode_();  break;
				case PICKING__MODE: pickingMode_(); break;
				case MOVE__MODE:    moveMode_();    break;
				case EDIT__MODE:    editMode_();    break;
			}

			update();
		}

		void Scene::projectionModeChanged()
		{
			for (Position i=0; i<renderers_.size(); ++i)
			{
				renderers_[i]->projectionModeChanged();
			}

			updateGL();
		}

		void Scene::rotateMode_()
		{
			if (current_mode_ == ROTATE__MODE) return;

			gl_renderer_->exitPickingMode();
			last_mode_ = current_mode_;
			current_mode_ = ROTATE__MODE;		
			QWidget::setCursor(QCursor(Qt::ArrowCursor));
			rotate_action_->setChecked(true);
			checkMenu(*getMainControl());
		}

		void Scene::pickingMode_()
		{
			if (current_mode_ == PICKING__MODE) return;

			gl_renderer_->enterPickingMode();
			last_mode_ = current_mode_;
			current_mode_ = PICKING__MODE;
			QWidget::setCursor(QCursor(Qt::CrossCursor));
			picking_action_->setChecked(true);
			checkMenu(*getMainControl());
		}

		void Scene::moveMode_()
		{
			if (current_mode_ == MOVE__MODE) return;

			gl_renderer_->exitPickingMode();
			last_mode_ = current_mode_;
			current_mode_ = MOVE__MODE;
			QWidget::setCursor(QCursor(Qt::SizeAllCursor));
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
								}
							}

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

			setStatusbarText((String)tr("printing.."));

			QPainter p;
			if(!p.begin(&printer)) return; 

			// TODO: push into renderSetup
			QImage pic = main_display_->grabFrameBuffer();
			p.drawImage(0,0, pic);	
			p.end();

			setStatusbarText((String)tr("finished printing"));
		}


		String Scene::exportPNG()
		{
			String filename = String(tr("BALLView_screenshot")) + String(screenshot_nr_) +".png";
			screenshot_nr_ ++;

			exportPNG(filename);
			Log.info() << (String)tr("Exporting PNG to ") 
				<< Directory().getPath() << FileSystem::PATH_SEPARATOR
				<< filename << std::endl;

			return filename;
		}

		void Scene::showExportPNGDialog()
		{
			String start = String(screenshot_nr_) + ".png";
			screenshot_nr_ ++;
			QString qresult = QFileDialog::getSaveFileName( 0, tr("Export PNG File"),
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

			// TODO: currently, we always use the renderer for the main display for exporting;
			//       we should decide this in a sensible way instead

			// first find out if we need to render offscreen or whether we can just use the current image
			if (!offscreen_rendering_)
				return renderers_[main_renderer_]->exportPNG(filename);

			// ok, we have to do this the hard way...	

			// What kind of renderer do we have to encapsulate?
			if (RTTI::isKindOf<GLRenderer>(*(renderers_[main_renderer_]->renderer)))
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

				TilingRenderer *trenderer = new TilingRenderer(gr, offscreen_factor_*width(), offscreen_factor_*height());
				trenderer->init(*this);
				trenderer->setSize(width(), height());

				boost::shared_ptr<RenderSetup> tr_rs(new RenderSetup(trenderer, new_widget, this, stage_));

				resetRepresentationsForRenderer_(*tr_rs);

				renderers_.push_back(tr_rs);
				updateGL();
				renderers_.pop_back();

				delete(trenderer);
				delete(gr);
				delete(new_widget);

				renderers_[main_renderer_]->resize(width(), height());
				updateGL();

			}
#ifdef BALL_HAS_RTFACT
			else if (RTTI::isKindOf<t_RaytracingRenderer>(*(renderers_[main_renderer_]->renderer)))
			{
				// create a new renderer
				t_RaytracingRenderer* renderer = new t_RaytracingRenderer;

				// build a new offscreen target for our renderer
				TRenderWindow<BALL_DEFAULT_PIXEL_TYPE>* target = new TRenderWindow<BALL_DEFAULT_PIXEL_TYPE>;

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
#endif

			ok = true;

			setWorkingDirFromFilename_(filename);

			if (ok) setStatusbarText((String)(tr("Saved PNG to ")) + filename);
			else setStatusbarText((String)tr("Could not save PNG"), true);

			return ok;
		}

		void Scene::exportNextPOVRay()
		{
			String filename = String((String)tr("BALLView_pov_") + String(pov_nr_) +".pov");

			POVRenderer pr(filename);
			bool result = exportScene(pr);
			pov_nr_ ++;

			if (result) setStatusbarText((String)tr("Saved POVRay to ") + filename);
			else setStatusbarText((String)tr("Could not save POVRay to ") + filename);
		}

		void Scene::exportPOVRay()
		{
			QString qresult = QFileDialog::getSaveFileName( 0,
					tr("Export POVRay File"),
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
				setStatusbarText((String)tr("Could not export POV to ") + result, true);
			}
			else
			{
				setStatusbarText((String)tr("Exported POV to ") + result);
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

			gl_renderer_->initPerspective();

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

		void Scene::switchRenderer(RenderSetup::RendererType new_type)
		{
			if (new_type == renderers_[main_renderer_]->getRendererType())
				return; // nothing to see here....

			// let's see if we support the requested type at all...
#ifdef BALL_HAS_RTFACT
			if (new_type != RenderSetup::OPENGL_RENDERER && new_type != RenderSetup::RTFACT_RENDERER)
#else
				if (new_type != RenderSetup::OPENGL_RENDERER)
#endif
				{
					Log.warn() << "Scene::switchRenderer(): Sorry, BALLView currently does not support renderers of this type!" << std::endl;
					return;
				}

			// ok, we have some work to do...
			boost::shared_ptr<RenderSetup> main_renderer_ptr = renderers_[main_renderer_];

			main_renderer_ptr->useContinuousLoop(false);
			main_renderer_ptr->stop();

			main_renderer_ptr->loop_mutex.lock();
			main_renderer_ptr->wait_for_render.wakeAll();
			main_renderer_ptr->loop_mutex.unlock();

			main_renderer_ptr->wait(100);

			delete(main_renderer_ptr->renderer);

			renderers_.erase(renderers_.begin() + main_renderer_);

			// now, create a new renderer
			if (new_type == RenderSetup::OPENGL_RENDERER)
			{
				GLRenderer*   new_renderer = new GLRenderer;
				new_renderer->init(*this);
				new_renderer->enableVertexBuffers(want_to_use_vertex_buffer_);

				main_renderer_ptr = boost::shared_ptr<RenderSetup>(new RenderSetup(new_renderer, main_display_, this, stage_));
			}
			else if (new_type == RenderSetup::RTFACT_RENDERER)
			{
#ifdef BALL_HAS_RTFACT
				t_RaytracingRenderer* new_renderer = new t_RaytracingRenderer();
				new_renderer->init(*this);
				new_renderer->setFrameBufferFormat(main_display_->getFormat());

				main_renderer_ptr = boost::shared_ptr<RenderSetup>(new RenderSetup(new_renderer, main_display_, this, stage_));
#endif
			}

			main_renderer_ptr->setReceiveBufferUpdates(true);
			resetRepresentationsForRenderer_(*main_renderer_ptr);

			renderers_.push_back(main_renderer_ptr);
			main_renderer_ = renderers_.size()-1;
			// NOTE: *don't* try to start new_rs, since this is an automatic variable
			//       that will be destroyed soon afterwards
			renderers_[main_renderer_]->start();
		}

		void Scene::addGlWindow()
		{
			GLRenderWindow* new_widget = new GLRenderWindow(0, ((String)tr("Scene")).c_str(), Qt::Window);
			new_widget->init();
			new_widget->makeCurrent();
			new_widget->resize(width(), height());

			new_widget->installEventFilter(this);

			GLRenderer*   new_renderer = new GLRenderer;
			new_renderer->init(*this);
			new_renderer->enableVertexBuffers(want_to_use_vertex_buffer_);

			boost::shared_ptr<RenderSetup> new_rs(new RenderSetup(new_renderer, new_widget, this, stage_));
			new_rs->setReceiveBufferUpdates(true);

			resetRepresentationsForRenderer_(*new_rs);
			new_widget->show();

			renderers_.push_back(new_rs);
			// NOTE: *don't* try to start new_rs, since this is an automatic variable
			//       that will be destroyed soon afterwards
			renderers_[renderers_.size()-1]->start();
		}

#ifdef BALL_HAS_RTFACT
		void Scene::addRTfactWindow()
		{
			GLRenderWindow* new_widget = new GLRenderWindow(0, ((String)tr("Scene")).c_str(), Qt::Window);
			new_widget->init();
			new_widget->makeCurrent();
			new_widget->resize(width(), height());

			new_widget->installEventFilter(this);

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
#endif

		void Scene::enterActiveStereo()
		{
			// first clean up
			exitStereo();

			return; // TODO: reactivate
			// AKD: what is happening here?
			GLRenderWindow* new_widget = new GLRenderWindow(0, String(tr("blubb")).c_str(), Qt::Window);
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

			GLRenderWindow* left_widget = new GLRenderWindow(0, String(tr("left eye")).c_str());
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

			GLRenderWindow* right_widget = new GLRenderWindow(0, String(tr("right eye")).c_str());
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

			GLRenderWindow* left_widget = new GLRenderWindow(QApplication::desktop()->screen(0), String(tr("left eye")).c_str());
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

			left_widget->showFullScreen();
			left_renderer->setSize(left_widget->width(), left_widget->height());

			boost::shared_ptr<RenderSetup> left_rs(new RenderSetup(left_renderer, left_widget, this, stage_));

			resetRepresentationsForRenderer_(*left_rs);
			left_rs->setStereoMode(RenderSetup::LEFT_EYE);

			renderers_.push_back(left_rs);
			stereo_left_eye_ = renderers_.size()-1;
			left_rs->start();

			GLRenderWindow* right_widget = new GLRenderWindow(QApplication::desktop()->screen(1), String(tr("right eye")).c_str());
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

			right_widget->showFullScreen();
			right_renderer->setSize(right_widget->width(), right_widget->height());

			boost::shared_ptr<RenderSetup> right_rs(new RenderSetup(right_renderer, right_widget, this, stage_));

			resetRepresentationsForRenderer_(*right_rs);
			right_rs->setStereoMode(RenderSetup::RIGHT_EYE);

			renderers_.push_back(right_rs);
			stereo_right_eye_ = renderers_.size()-1;
			right_rs->start();

			gl_renderer_->setStereoMode(GLRenderer::DUAL_VIEW_DIFFERENT_DISPLAY_STEREO);

			right_rs->makeDependentOn(left_rs);
			left_rs->makeDependentOn(right_rs);

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

		void Scene::startContinuousLoop()
		{
#ifdef BALL_HAS_RTFACT
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
#endif
		}

		void Scene::stopContinuousLoop()
		{
#ifdef BALL_HAS_RTFACT
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
#endif
		}

		void Scene::toggleContinuousLoop()
		{
#ifdef BALL_HAS_RTFACT
			if (continuous_loop_)
			{
				stopContinuousLoop();
			}
			else
			{
				startContinuousLoop();
			}
#endif
		}

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
					Vector3 diff_viewpoint = (camera.getViewPoint()      - (*it).getViewPoint());
					Vector3 diff_up        = (camera.getLookUpVector()   - (*it).getLookUpVector());
					Vector3 diff_look_at   = (camera.getLookAtPosition() - (*it).getLookAtPosition());

					Vector3 max = diff_viewpoint;
					if (diff_look_at.getLength() > max.getLength()) max = diff_look_at;

					Size steps = (Size) (max.getLength() * animation_smoothness_);
					if (steps == 0) steps = 1;

					diff_viewpoint  /= steps;
					diff_up         /= steps;
					diff_look_at    /= steps;

					for (Size i = 0; i < steps && !stop_animation_; i++)
					{
						animation_thread_->mySleep(30);

						camera.setViewPoint(camera.getViewPoint() - diff_viewpoint);
						camera.setLookUpVector(camera.getLookUpVector() - diff_up);
						camera.setLookAtPosition(camera.getLookAtPosition() - diff_look_at);
						camera.setProjectionMode(stage_->getCamera().getProjectionMode());

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
			//       if you want 3d textures for stereo with different
			//       half images, you need to ensure that this function
			//       is called *after* switching to stereo!
			//
			// TODO: - change this to something more sensible!
			//       - call something in RenderSetup instead!	
			Position texname = 0;

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

		// TODO: make renderer configurable
		void Scene::pickParent_(QPoint p)
		{
			ignore_pick_ = true;
			list<GeometricObject*> objects;
			renderers_[main_renderer_]->pickObjects((Position) p.x(), (Position) p.y(), 
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
			float value = QInputDialog::getDouble(this, tr("Setup view volume"), tr("Enter volume length:"),	20, 5, 200, 1, &ok);
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

		//
		//-------------------- Scene -----------------------

		bool Scene::only_highlighted_ = true;

		void Scene::initializeMembers_()
		{
			edit_id_ = 0;
			current_atom_ = 0;
			current_bond_ = 0;
			edit_settings_ = 0;
			bond_order_ = Bond::ORDER__SINGLE;
			atomic_number_ = 6;
			atom_number_ = 0;
			temp_move_ = false;
			default_font_ = QFont("Arial", 16., QFont::Bold);
			has_overlay_ = false;
		}

		void Scene::setCursor(String c)
		{
			Path path;
			String filename = path.find("graphics/cursor.png");
			if (filename == "") return;

			QPainter p;
			QImage pm(filename.c_str());
			p.begin(&pm);
			p.setFont(QFont("Arial", 5, QFont::Bold));

			QColor color;
			stage_->getBackgroundColor().getInverseColor().get(color);
			color.setAlpha(255);
			p.setRenderHint(QPainter::TextAntialiasing, true);
			p.setRenderHint(QPainter::Antialiasing, true);
			p.setPen(color);
			int text_pos_x = 12;
			int text_pos_y = 22;
			if (c.size()==2)
			{
				text_pos_x = 0;
				text_pos_y = 23;
			}
			p.drawText(text_pos_x, text_pos_y, c.c_str());
			p.end();

			pm.createAlphaMask();
			QCursor cursor(QPixmap::fromImage(pm), 0, 0);
			QWidget::setCursor(cursor);
		}

		void Scene::wheelEvent(QWheelEvent* e)
		{
			Index delta = e->delta();
			if (delta == 0) return;

			if (current_mode_ != (Scene::ModeType) EDIT__MODE)
			{
				Scene::defaultWheelHandling_(e);
				return;
			}

			e->accept();

			if (isAnimationRunning() || getMainControl()->isBusy()) return;

			if (delta > 1) delta = 1;
			if (delta < -1) delta = -1;

			if (e->modifiers() == Qt::ShiftModifier)
			{
				getClickedItems_(e->x(), e->y());
				changeBondOrder_(delta);
			}
			else
			{
				Scene::defaultWheelHandling_(e);
			}
		}

		void Scene::changeBondOrder_(Index delta)
		{
			if (current_bond_ == 0) return;

			Index order = current_bond_->getOrder();
			order += delta;
			order = BALL_MAX((Index)Bond::ORDER__SINGLE, order);
			if (order > Bond::ORDER__AROMATIC) order = Bond::ORDER__SINGLE;
			if (current_bond_->getOrder() == order) return;

			current_bond_->setOrder(order);
			getMainControl()->update(*(Atom*)current_bond_->getFirstAtom(), true);
			String txt(tr("Set bond order to "));
			txt += getBondOrderString_(order);
			setStatusbarText(txt);
		}

		// TODO: make renderer dependent on current target
		void Scene::mouseReleaseEvent(QMouseEvent* e)
		{
			has_overlay_ = false;

			if (temp_move_)
			{
				deselect_();
				temp_move_ = false;
				return;
			}

			if ((int)current_mode_ < (int) EDIT__MODE)
			{
				Scene::defaultMouseReleaseEvent(e);
				return;
			}

			if (last_buttons_ == Qt::RightButton)
			{
				deselect_();
				return;
			}

			if (isAnimationRunning() || getMainControl()->isBusy()) return;

			mouse_button_is_pressed_ = false;

			// if we didnt find first atom: abort
			if (!current_atom_)
			{
				deselect_();
				return;
			}

			current_atom_->deselect();
			notify_(new CompositeMessage(*current_atom_, CompositeMessage::DESELECTED_COMPOSITE));

			Atom* atom = current_atom_;
			getClickedItems_(e->x(), e->y());

			// decide what to do... did we find an atom at all?
			if (current_atom_)
			{
				// is it the atom we started with?
				if (atom == current_atom_ || // workaround against crashes:
						&atom->getRoot() != &current_atom_->getRoot())
				{
					// in this case, we assume that the user does not want to set a bond
					deselect_();
					return;
				}

				// we found _another_ atom: set the bond
				Bond* c = new Bond("Bond", *current_atom_, *atom, Bond::ORDER__SINGLE);

				EditOperation eo(0, c, "Added bond of type single" , EditOperation::ADDED__BOND);
				undo_.push_back(eo);

				// tell about the new undo operation
				emit newEditOperation(eo);

				merge_(current_atom_, atom);

				//update representation
				getMainControl()->update(*atom, true);
				setStatusbarText((String)tr("Added a bond"));
			}
			else // no atom found -> create one
			{
				// project the new atom on the plane of the old atom
				current_atom_ = atom;
				Vector3 new_pos = renderers_[main_renderer_]->mapViewportTo3D(e->x(), e->y());

				// test if the two atoms would have the same position
				if (fabs((current_atom_->getPosition() - new_pos).getLength()) < 0.02)
				{
					setStatusbarText((String)tr("Aborting, since both atoms would have the same location!"), true);
					return;
				}

				// build a new atom...
				String name(PTE[atomic_number_].getSymbol());
				name += String(atom_number_);
				atom_number_++;
				PDBAtom* a = new PDBAtom(PTE[atomic_number_], name);
				a->setPosition(new_pos);
				current_atom_->getParent()->appendChild(*a);

				//store the Operation in undo_
				Vector3 atom_position = a->getPosition();

				EditOperation eo(a, NULL, (String)tr("Added atom of type ") + PTE[atomic_number_].getName() + (String)tr(" at position (")
						+ String(atom_position.x) + ", "
						+ String(atom_position.y) + ", "
						+ String(atom_position.z) + ")", EditOperation::ADDED__ATOM);
				undo_.push_back(eo);

				// tell about the new undo operation
				emit newEditOperation(eo);

				//set the bond
				Bond* c = new Bond("Bond", *current_atom_, *a, Bond::ORDER__SINGLE);

				// tell about the new undo operation
				String bond_string = getBondOrderString_(bond_order_);
				EditOperation eo2(0, c, (String)tr("Added bond of type ") + bond_string, EditOperation::ADDED__BOND);
				undo_.push_back(eo2);
				emit newEditOperation(eo2);

				getMainControl()->update(*a->getParent(), true);
				setStatusbarText((String)tr("Added a bond and an atom"));
			}

			deselect_();
		}

		String Scene::getBondOrderString_(Index order)
		{
			String bond_string;
			switch (order)
			{
				case Bond::ORDER__SINGLE:
					bond_string = (String)tr("single");
					break;
				case Bond::ORDER__DOUBLE:
					bond_string = (String)tr("double");
					break;
				case Bond::ORDER__TRIPLE:
					bond_string = (String)tr("triple");
					break;
				case Bond::ORDER__QUADRUPLE:
					bond_string = (String)tr("quadruple");
					break;
				case Bond::ORDER__AROMATIC:
					bond_string = (String)tr("aromatic");
					break;
				default:
					bond_string = (String)tr("unknown");
					break;
			}

			return bond_string;
		}

		/// ******************** Helper Functions *************************
		list<AtomContainer*> Scene::getContainers_()
		{
			list<AtomContainer*> containers;
			if (only_highlighted_)
			{
				list<Composite*> highl = getMainControl()->getMolecularControlSelection();
				list<Composite*>::iterator lit = highl.begin();
				for (; lit != highl.end(); ++lit)
				{
					AtomContainer* ac = dynamic_cast<AtomContainer*>(*lit);
					if (ac != 0) containers.push_back(ac);
				}
			}

			if (containers.size() > 0) return containers;

			HashSet<Composite*> composites = getMainControl()->getCompositeManager().getComposites();
			HashSet<Composite*>::Iterator sit = composites.begin();
			for (; +sit; ++sit)
			{
				AtomContainer* ac = dynamic_cast<AtomContainer*>(*sit);
				if (ac != 0) containers.push_back(ac);
			}

			return containers;
		}

		// TODO: make renderer / target configurable
		void Scene::getClickedItems_(int x, int y)
		{
			current_bond_ = 0;
			current_atom_ = 0;

			QPoint p(x,y);
			list<GeometricObject*> objects;
			renderers_[main_renderer_]->pickObjects((Position)p.x(), (Position)p.y(),
					(Position)p.x(), (Position)p.y(), objects);

			if (objects.size() > 0)
			{
				Composite* c = (Composite*)(**objects.begin()).getComposite();
				if (c == 0) return;

				current_bond_ = dynamic_cast<Bond*>(c);
				current_atom_ = dynamic_cast<Atom*>(c);
			}
		}

		void Scene::setElementCursor()
		{
			String s = PTE[atomic_number_].getSymbol();
			setCursor(s.c_str());
		}

		// Slot to change to EDIT__MODE
		// TODO: popup a message box if the currently highlighted atom container does not have a
		//			 suitable representation
		void Scene::editMode_()
		{
			if (!fragment_db_initialized_)
			{
				fragment_db_.setFilename("fragments/Editing-Fragments.db");
				fragment_db_.init();
				fragment_db_initialized_ = true;
			}

			list<AtomContainer*> acs = getContainers_();

			list<Composite*> sel;
			list<AtomContainer*>::iterator lit = acs.begin();
			for (; lit != acs.end(); lit++)
			{
				sel.push_back(*lit);
			}
			ControlSelectionMessage* msg = new ControlSelectionMessage();
			msg->setSelection(sel);
			notify_(msg);

			last_mode_ = current_mode_;
			current_mode_ = (Scene::ModeType)EDIT__MODE;
			edit_id_->setChecked(true);
			setElementCursor();
			checkMenu(*getMainControl());

			HashSet<Composite*> selection = getMainControl()->getSelection();
			HashSet<Composite*>::Iterator it = selection.begin();
			for (; +it; ++it)
			{
				if (!(**it).containsSelection()) continue;
				getMainControl()->deselectCompositeRecursive(*it, true);
				getMainControl()->update(**it, false);
			}
			notify_(new NewSelectionMessage);
		}

		// insert an atom at screen positions (x,y) on the view plane
		// TODO: make the renderer dependent on the current target!
		void Scene::insert_(int x, int y, PDBAtom &atom)
		{
			// find the 3D coordinates of screen position (x,y) on the view plane
			// move the atom to that position
			atom.setPosition(renderers_[main_renderer_]->mapViewportTo3D(x,y));

			// now we need to find the AtomContainer into which we will insert the atom.
			// get all highlighted composites
			list<Composite*> composite_list = getMainControl()->getMolecularControlSelection();

			Size nr_high = composite_list.size();
			if (nr_high > 1 || (only_highlighted_ && nr_high == 0))
			{
				setStatusbarText((String)tr("Please highlight exactly one AtomContainer for insertion of the created atoms!"), true);
				return;
			}

			// exactly one highlighted composite
			if (nr_high == 1)
			{
				// is it an AtomContainer?
				AtomContainer* ai = dynamic_cast<AtomContainer*>(*composite_list.begin());
				if (ai == 0)
				{
					// is the parent an AtomContainer?
					Composite* parent = (**composite_list.begin()).getParent();
					if (parent != 0)
					{
						ai = dynamic_cast<AtomContainer*>(parent);
					}

					if (ai == 0)
					{
						setStatusbarText((String)tr("Please highlight exactly one AtomContainer for insertion of the created atoms!"), true);
						return;
					}
				}

				// prevent adding of atoms to a System:
				// some forcefields will go havoc otherwise
				if (RTTI::isKindOf<System>(*ai))
				{
					System* system = (System*) ai;
					Molecule* mol = system->getMolecule(0);
					if (mol == 0)
					{
						mol = new Molecule();
						system->insert(*mol);
					}
					ai = mol;
				}

				// we do not need to create our own system
				ai->insert(atom);
				getMainControl()->update(*ai, true);
				return;
			}

			/////////////////////////////////////////////////////////
			// no atom container highlighted:

			HashSet<Composite*> composites = getMainControl()->getCompositeManager().getComposites();

			// no System exists? -> create one
			if (composites.size() == 0)
			{
				System *system = new System();
				Molecule* current_molecule = new Molecule();
				system->insert(*current_molecule);
				current_molecule->insert(atom);
				getMainControl()->insert(*system);
				getMainControl()->update(*system);
				return;
			}

			// add to first Molecule in first System
			System* system = dynamic_cast<System*>(*composites.begin());
			Molecule* mol = system->getMolecule(0);
			if (mol == 0)
			{
				mol = new Molecule();
				system->insert(*mol);
			}

			mol->appendChild(atom);
			getMainControl()->update(*mol, true);
		}


		// Set the element for the next insert operations
		void Scene::setEditElementType(int element_number)
		{
			atomic_number_ = element_number;
			setElementCursor();
		}

		// Get the element for the next insert operations
		int Scene::getEditElementType()
		{
			return atomic_number_;
		}

		void Scene::showContextMenu(QPoint pos)
		{
			menu_point_ = pos;
			QMenu menu;
			IconLoader& loader = IconLoader::instance();

			QAction* rotate_mode = menu.addAction(tr("Rotate Mode"), this, SLOT(rotateMode_()));
			rotate_mode->setCheckable(true);
			rotate_mode->setChecked(current_mode_ == (Scene::ModeType) MOVE__MODE);

			QAction* picking_mode = menu.addAction(tr("Picking Mode"), this, SLOT(pickingMode_()));
			picking_mode->setCheckable(true);
			picking_mode->setChecked(current_mode_ == (Scene::ModeType) PICKING__MODE);

			QAction* move_mode = menu.addAction(tr("Move Mode"), this, SLOT(moveMode_()));
			move_mode->setCheckable(true);
			move_mode->setChecked(current_mode_ == (Scene::ModeType) MOVE__MODE);

			QAction* edit_mode = menu.addAction(tr("Edit Mode"), this, SLOT(editMode_()));
			edit_mode->setCheckable(true);
			edit_mode->setChecked(current_mode_ == (Scene::ModeType) EDIT__MODE);

			menu.addSeparator();

			if (current_mode_ == (Scene::ModeType) EDIT__MODE)
			{
				menu.addAction(tr("Atom Properties"), this, SLOT(atomProperties_()))->setEnabled(current_atom_ != 0);
				menu.addAction(tr("Move Atom"), this, SLOT(moveAtom_()))->setEnabled(current_atom_ != 0);
				menu.addAction(tr("Delete Atom"), this, SLOT(deleteAtom_()))->setEnabled(current_atom_ != 0);
				menu.addAction(loader.getIcon("actions/molecule-set-element"), tr("Change Atom Element"), this, SLOT(changeAtomElement_()))->setEnabled(current_atom_ != 0);

				QMenu* charge = new QMenu();
				QAction* change_charge = menu.addMenu(charge);
				change_charge->setText(tr("Set formal charge"));
				Index charge_value = 0;
				if (current_atom_ != 0) charge_value = current_atom_->getFormalCharge();
				for (Index p = +6; p > -7; p--)
				{
					String s(p);
					if (p > 0) s = String("+") + s;

					QAction* action = charge->addAction(s.c_str(), this, SLOT(setFormalCharge_()));
					action->setCheckable(true);
					action->setChecked(p == charge_value);
				}
				change_charge->setEnabled(current_atom_ != 0);

				menu.addSeparator();

				menu.addAction(tr("Delete Bond"), this, SLOT(deleteBond_()))->setEnabled(current_bond_ != 0);

				QMenu* order = new QMenu();
				QAction* change_order = menu.addMenu(order);
				connect(order, SIGNAL(hovered(QAction*)), this, SLOT(activatedOrderItem_(QAction*)));
				change_order->setText(tr("Change bond order"));
				vector<QAction*> oas;
				oas.push_back(order->addAction(tr("Single"),    this, SLOT(changeBondOrder_())));
				oas.push_back(order->addAction(tr("Double"),    this, SLOT(changeBondOrder_())));
				oas.push_back(order->addAction(tr("Triple"),    this, SLOT(changeBondOrder_())));
				oas.push_back(order->addAction(tr("Quadruple"), this, SLOT(changeBondOrder_())));
				oas.push_back(order->addAction(tr("Aromatic"),  this, SLOT(changeBondOrder_())));
				oas.push_back(order->addAction(tr("Unknown"),   this, SLOT(changeBondOrder_())));

				Index bo = 0;
				if (current_bond_) bo = ((Index)current_bond_->getOrder());
				for (Index p = 0; p < (Index) oas.size(); p++)
				{
					oas[p]->setCheckable(true);
				}

				switch (bo)
				{
					case Bond::ORDER__SINGLE:
						oas[0]->setChecked(true);
						break;
					case Bond::ORDER__DOUBLE:
						oas[1]->setChecked(true);
						break;
					case Bond::ORDER__TRIPLE:
						oas[2]->setChecked(true);
						break;
					case Bond::ORDER__QUADRUPLE:
						oas[3]->setChecked(true);
						break;
					case Bond::ORDER__AROMATIC:
						oas[4]->setChecked(true);
						break;
					default:
						oas[5]->setChecked(true);
						break;
				}

				change_order->setEnabled(current_bond_ != 0);

				menu.addSeparator();

				QMenu* add_menu = new QMenu();
				QAction* add_action = menu.addMenu(add_menu);
				add_action->setText(tr("Add"));
				if (getContainers_().size() == 0)
				{
					add_action->setEnabled(false);
				}

				QMenu* rings = new QMenu();
				QAction* ring_action = add_menu->addMenu(rings);
				ring_action->setText(tr("Aromatic rings"));
				rings->addAction(tr("Pyrrole"), this, SLOT(addStructure_()));
				rings->addAction(tr("Benzene"), this, SLOT(addStructure_()));
				rings->addAction(tr("Indole"), this, SLOT(addStructure_()));

				QMenu* aas = new QMenu();
				QAction* aas_action = add_menu->addMenu(aas);
				aas_action->setText(tr("Amino acids"));

				QMenu* nas = new QMenu();
				QAction* nas_action = add_menu->addMenu(nas);
				nas_action->setText(tr("Nucleic acids"));

				HashSet<String> names;
				const std::vector<Residue*>& residues = fragment_db_.getFragments();
				vector<Residue*>::const_iterator rit = residues.begin();
				vector<Residue*> nucleotides;
				for (;rit != residues.end();++rit)
				{
					String name = (**rit).getName();
					if ((**rit).isAminoAcid())
					{
						if (names.has(name)) continue;
						names.insert(name);
						aas->addAction(name.c_str(), this, SLOT(addStructure_()));
						continue;
					}
				}

				nas->addAction(tr("Alanine"), this, SLOT(addStructure_()));
				nas->addAction(tr("Cytosine"), this, SLOT(addStructure_()));
				nas->addAction(tr("Guanine"), this, SLOT(addStructure_()));
				nas->addAction(tr("Thymine"), this, SLOT(addStructure_()));
				nas->addAction(tr("Uracil"), this, SLOT(addStructure_()));

				menu.addSeparator();

				menu.addAction(optimize_action_);
				menu.addAction(add_hydrogens_action_);
				menu.addAction(bondorders_action_);
			}

			menu.exec(mapToGlobal(pos));

			// if we switched to move mode, let the user move the atom:
			if (current_mode_ == MOVE__MODE) return;

			// otherwise deselect all selected items
			deselect_();
		}

		void Scene::setFormalCharge_()
		{
			deselect_();

			QObject* os = sender();
			if (os == 0) return;
			QAction* action = dynamic_cast<QAction*>(os);
			if (action == 0) return;
			String string = ascii(action->text());
			try
			{
				Index fc = string.toInt();
				if (current_atom_ != 0) current_atom_->setFormalCharge(fc);
				getMainControl()->update(*current_atom_, true);
			}
			catch(...)
			{
				BALLVIEW_DEBUG;
			}
		}

		void Scene::deselect_(bool update)
		{
			bool was_mt = getMainControl()->useMultithreading();
			getMainControl()->setMultithreading(false);
			if (current_bond_ != 0 &&
					(current_bond_->isSelected() ||
					 current_bond_->getFirstAtom()->isSelected() ||
					 current_bond_->getSecondAtom()->isSelected()))
			{
				current_bond_->deselect();
				Atom* a1 = (Atom*)current_bond_->getFirstAtom();
				Atom* a2 = (Atom*)current_bond_->getSecondAtom();
				a1->deselect();
				a2->deselect();
				if (update)
				{
					notify_(new CompositeMessage(*a1, CompositeMessage::DESELECTED_COMPOSITE));
					notify_(new CompositeMessage(*a2, CompositeMessage::DESELECTED_COMPOSITE));
				}
			}

			if (current_atom_!= 0 && current_atom_->isSelected())
			{
				current_atom_->deselect();
				if (update)
				{
					notify_(new CompositeMessage(*current_atom_, CompositeMessage::DESELECTED_COMPOSITE));
				}
			}
			getMainControl()->setMultithreading(was_mt);
		}

		void Scene::deleteAtom_()
		{
			if (current_atom_ == 0) return;
			getMainControl()->remove(*current_atom_);
			current_atom_ = 0;
		}

		void Scene::changeElement_()
		{
			if (current_atom_ != 0)
			{
				atomic_number_ = current_atom_->getElement().getAtomicNumber();
			}

			PTEDialog pte;
			pte.exec();
		}

		void Scene::changeAtomElement_()
		{
			if (current_atom_ != 0)
			{
				atomic_number_ = current_atom_->getElement().getAtomicNumber();
			}

			PTEDialog pte;
			pte.exec();

			if ((current_atom_ != 0))
			{
				current_atom_->setElement(PTE[atomic_number_]);
				String new_name = PTE[atomic_number_].getSymbol();
				//get the old atom number
				String old_name = current_atom_->getName();
				old_name.toUpper();
				new_name += old_name.trimLeft("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
				current_atom_->setName(new_name);
				deselect_();
				getMainControl()->update(*current_atom_);
			}
		}

		void Scene::createBond_()
		{
			// this functionaltiy shall be independent from the edit mode
			// check if two atoms are selected
			HashSet<Composite*> selection = getMainControl()->getSelection();

			// by switching into the edit mode recursive selection
			// has already cleaned up
			Atom*  first_atom = 0;
			Atom* second_atom = 0;

			// case 1: one system with exactly two atoms
			if (selection.size() == 1)
			{
				if (RTTI::isKindOf<AtomContainer>(**selection.begin()))
				{
					AtomContainer* ac = reinterpret_cast<AtomContainer*>(*selection.begin());
					if (ac->countAtoms() == 2)
					{
						AtomIterator atom_it = ac->beginAtom();
						for(; +atom_it; ++atom_it)
						{
							if (!first_atom)
							{
								first_atom = &*atom_it;
							}
							else if (!second_atom)
							{
								second_atom = &*atom_it;
							}
							else
							{
								Log.error() << (String)tr("Internal error! Too many atoms selected.") << endl;
							}
						}
					}
					else
					{
						setStatusbarText((String)tr("Please select exactly two atoms."), true);
					}
				}
				else
				{
					setStatusbarText((String)tr("Please select exactly two atoms."), true);
				}
			}
			// case 2: two selected atoms with unselected in
			//         either distinct atom containers
			//         or with unselected in the same container
			else if (selection.size() == 2)
			{
				HashSet<Composite*>::Iterator it = selection.begin();
				for (; +it; ++it)
				{
					if (RTTI::isKindOf<Atom>(**it))
					{
						if (!first_atom)
						{
							first_atom = reinterpret_cast<Atom*>(*it);
						}
						else if (!second_atom)
						{
							second_atom = reinterpret_cast<Atom*>(*it);
						}
					}
					// case 3: a single atom in selected atomcontainer
					else if (RTTI::isKindOf<AtomContainer>(**it))
					{
						AtomContainer* ac = reinterpret_cast<AtomContainer*>(*it);
						if (ac->countAtoms() == 1)
						{
							if (!first_atom)
							{
								first_atom = &*ac->beginAtom();
							}
							else if (!second_atom)
							{
								second_atom = &*ac->beginAtom();
							}
						}
						else
						{
							Log.error() << (String)tr("Scene: Internal error! ") << __LINE__ << endl;
						}
					}
				}
			}

			// we found two atoms
			if (first_atom && second_atom)
			{
				// create a bond
				Bond* bond = first_atom->createBond(*second_atom);
				bond->setOrder(Bond::ORDER__SINGLE); //TODO single bond or current edit mode default bond order?

				// 		TODO:for undo -operation
				// 		EditOperation eo(0, bond, "Added bond of type single" , EditOperation::ADDED__BOND);
				// 		undo_.push_back(eo);
				//
				// 		// tell about the new undo operation
				// 		emit newEditOperation(eo);

				// if the bond is between two molecules, merge them
				merge_(first_atom, second_atom);

				// update representation
				getMainControl()->update(*first_atom, true);
				getMainControl()->update(*second_atom, true);
				setStatusbarText((String)tr("Added a bond"));

				// deselect and delete recursively from the selection set
				HashSet<Composite*>::Iterator it = selection.begin();
				for (; +it; ++it)
				{
					if (!(**it).containsSelection()) continue;
					getMainControl()->deselectCompositeRecursive(*it, true);
					getMainControl()->update(**it, false);
				}
				first_atom->deselect();
				second_atom->deselect();

				// update representation
				getMainControl()->update(*first_atom, true);
				getMainControl()->update(*second_atom, true);
			}
			else
			{
				setStatusbarText((String)tr("Please select exactly two atoms."), true);
			}
		}

		void Scene::deleteBond_()
		{
			if (current_bond_ == 0) return;

			Atom* a1 = (Atom*)current_bond_->getFirstAtom();
			Atom* a2 = (Atom*)current_bond_->getSecondAtom();
			a1->destroyBond(*a2);
			a1->deselect();
			a2->deselect();
			notify_(new CompositeMessage(*a1, CompositeMessage::CHANGED_COMPOSITE_HIERARCHY));
			notify_(new CompositeMessage(*a2, CompositeMessage::CHANGED_COMPOSITE_HIERARCHY));
			notify_(new CompositeMessage(*a1, CompositeMessage::DESELECTED_COMPOSITE));
			notify_(new CompositeMessage(*a2, CompositeMessage::DESELECTED_COMPOSITE));
			current_bond_ = 0;
		}

		void Scene::changeBondOrder_()
		{
			if (current_bond_ == 0) return;
			deselect_();
			if (current_bond_->getOrder() == bond_order_) return;

			current_bond_->setOrder((Bond::BondOrder)bond_order_);
			getMainControl()->update(*(Atom*)current_bond_->getFirstAtom(), true);
		}

		void Scene::activatedOrderItem_(QAction* action)
		{
			if (action == 0) return;
			String text = ascii(action->text());

			if (text == (String)tr("Single")) bond_order_ = Bond::ORDER__SINGLE;
			else if (text == (String)tr("Double")) bond_order_ = Bond::ORDER__DOUBLE;
			else if (text == (String)tr("Triple")) bond_order_ = Bond::ORDER__TRIPLE;
			else if (text == (String)tr("Quadruple")) bond_order_ = Bond::ORDER__QUADRUPLE;
			else if (text == (String)tr("Aromatic")) bond_order_ = Bond::ORDER__AROMATIC;
			else if (text == (String)tr("Unknown")) bond_order_ = Bond::ORDER__UNKNOWN;

		}

		void Scene::moveAtom_()
		{
			if (current_atom_ == 0) return;

			current_atom_->setSelected(true);
			notify_(new CompositeMessage(*current_atom_, CompositeMessage::SELECTED_COMPOSITE));
			setMode(MOVE__MODE);
		}

		void Scene::atomProperties_()
		{
			if (current_atom_ == 0) return;

			CompositeProperties as(current_atom_, this);
			bool apply = as.exec();
			deselect_();

			if (apply) getMainControl()->update(*current_atom_, true);
		}

		void Scene::createMolecule_()
		{
			System *system = new System();
			Molecule* current_molecule = new Molecule();
			system->insert(*current_molecule);
			getMainControl()->insert(*system);

			ControlSelectionMessage* nsm =  new ControlSelectionMessage();
			list<Composite*> selection;
			selection.push_back(current_molecule);
			nsm->setSelection(selection);
			notify_(nsm);
		}

		void Scene::keyPressEvent(QKeyEvent* e)
		{
			if (!reactToKeyEvent_(e))
			{
				defaultKeyPressEvent(e);

				// TODO QShortcut* shortcut
				if (e->key() == Qt::Key_E)
				{
					setMode((Scene::ModeType)EDIT__MODE);
					return;
				}

			}
		}

		bool Scene::reactToKeyEvent_(QKeyEvent* e)
		{
			int key = e->key();

			// TODO QShortcut* shortcut
			if (key == Qt::Key_E)
			{
				setMode((ModeType)EDIT__MODE);
				return true;
			}

			if (current_mode_ != (ModeType)EDIT__MODE) return false;

			if (!getMainControl()->isBusy())
			{
				QPoint point = mapFromGlobal(QCursor::pos());

				// TODO QShortcut* shortcut
				if (key == Qt::Key_D)
				{
					getClickedItems_(point.x(), point.y());
					deleteAtom_();
					return true;
				}

				// TODO QShortcut* shortcut
				if (key == Qt::Key_Backspace)
				{
					getClickedItems_(point.x(), point.y());
					deleteBond_();
					return true;
				}
			}

			// TODO QShortcut* shortcut
			if (key < Qt::Key_A || key > Qt::Key_Z)
			{
				return false;
			}

			// TODO QShortcut* shortcut
			if      (key == Qt::Key_H) atomic_number_ = 1;
			else if (key == Qt::Key_C) atomic_number_ = 6;
			else if (key == Qt::Key_N) atomic_number_ = 7;
			else if (key == Qt::Key_O) atomic_number_ = 8;
			else if (key == Qt::Key_P) atomic_number_ = 15;
			else if (key == Qt::Key_S) atomic_number_ = 16;
			else
			{
				return false;
			}

			setElementCursor();

			String text(tr("Setting element to "));
			text += PTE[atomic_number_].getName();
			setStatusbarText(text);

			return true;
		}

		// TODO: make the renderer dependent on the current target!
		void Scene::addStructure(String name)
		{
			deselect_();

			if (!fragment_db_initialized_)
			{
				fragment_db_.setFilename("fragments/Editing-Fragments.db");
				fragment_db_.init();
				fragment_db_initialized_ = true;
			}

			list<AtomContainer*> containers = getContainers_();
			if (containers.size() == 0) return;

			Residue* residue = fragment_db_.getResidueCopy(name);
			if (residue == 0)
			{
				residue = fragment_db_.getResidueCopy(name + "-Skeleton");
				if (residue == 0) return;
			}

			Vector3 p;
			Size nr = 0;
			AtomIterator ait = residue->beginAtom();
			for (;+ait; ++ait)
			{
				p += ait->getPosition();
				nr++;
			}

			if (nr == 0)
			{
				BALLVIEW_DEBUG
					delete residue;
				return;
			}

			p /= (float) nr;

			Matrix4x4 m;
			Vector3 x = renderers_[main_renderer_]->mapViewportTo3D(menu_point_.x(), menu_point_.y());
			TransformationProcessor tf;

			Vector3 vv = getStage()->getCamera().getViewVector();
			float l = vv.getLength();
			if (!Maths::isZero(l)) vv /= l;
			Vector3 axis = Vector3(1,0,0) % vv;
			if (axis.getSquareLength() != 0)
			{
				Angle a = vv.getAngle(Vector3(1,0,0));
				m.setRotation(a, axis);
				tf.setTransformation(m);
				residue->apply(tf);
			}

			m.setTranslation(x - p);
			tf.setTransformation(m);
			residue->apply(tf);

			AtomContainer* s = *containers.begin();
			if (RTTI::isKindOf<System>(*s))
			{
				System* system = (System*) s;
				Molecule* mol = system->getMolecule(0);
				if (mol == 0)
				{
					mol = new Molecule();
					system->insert(*mol);
				}
				s = mol;
			}

			s->insert(*residue);
			getMainControl()->deselectCompositeRecursive(s, true);
			getMainControl()->selectCompositeRecursive(residue, true);
			getMainControl()->update(*s);
			notify_(new NewSelectionMessage);
			setMode(MOVE__MODE);
		}

		void Scene::addStructure_()
		{
			QObject* os = sender();
			if (os == 0) return;
			QAction* action = dynamic_cast<QAction*>(os);
			if (action == 0) return;
			addStructure(ascii(action->text()));
		}

		void Scene::createNewMolecule()
		{
			if (getMainControl()->isBusy()) return;

			System* s = new System();
			Molecule* m = new Molecule();
			s->insert(*m);
			getMainControl()->insert(*s);
			getMainControl()->update(*s);
			ControlSelectionMessage* msg = new ControlSelectionMessage();
			list<Composite*> sel;
			sel.push_back(m);
			msg->setSelection(sel);
			notify_(msg);

			editMode_();
		}

		void Scene::saturateWithHydrogens()
		{
			if (getMainControl()->isBusy()) return;

			deselect_(false);
			list<AtomContainer*> containers = getContainers_();
			if (containers.size() < 1) return;
			AtomContainer* ac = *containers.begin();
			RingPerceptionProcessor rpp;
			vector<vector<Atom*> > rings;
			rpp.calculateSSSR(rings, *ac);
			rings = rpp.getAllSmallRings();

			AddHydrogenProcessor ahp;
			ahp.setRings(rings);
			ac->apply(ahp);
			String nr = ahp.getNumberOfAddedHydrogens();
			setStatusbarText((String)tr("Added ") + nr + (String)tr(" hydrogens."), true);
			getMainControl()->update(*ac, true);
		}


		void Scene::computeBondOrders()
		{
			if (getMainControl()->isBusy()) return;

			System* system = getMainControl()->getSelectedSystem();
			if (system == 0) { return;};

			deselect_(false);

			// do we have a Molecular Structure?
			MolecularStructure* ms = MolecularStructure::getInstance(0);
			if (ms == 0) return;

			// get the highlighted atomcontainer
			list<AtomContainer*> containers;
			if (only_highlighted_)
			{
				list<Composite*> highl = getMainControl()->getMolecularControlSelection();
				list<Composite*>::iterator lit = highl.begin();
				for (; lit != highl.end(); ++lit)
				{
					AtomContainer* ac = dynamic_cast<AtomContainer*>(*lit);
					if (ac != 0) containers.push_back(ac);
				}
			}

			if (containers.size() != 1)
			{
				setStatusbarText((String)tr("Please highlight exactly one AtomContainer!"), true);
				return;
			}
			AssignBondOrderConfigurationDialog& bond_order_dialog = ms->getBondOrderDialog();

			AssignBondOrderProcessor abop;

			// read the options from the dialog
			// bond_order_dialog.setOptionsForProcessor(abop);
			abop.options[AssignBondOrderProcessor::Option::OVERWRITE_SINGLE_BOND_ORDERS] = bond_order_dialog.overwrite_singleBO_box->isChecked();
			abop.options[AssignBondOrderProcessor::Option::OVERWRITE_DOUBLE_BOND_ORDERS] = bond_order_dialog.overwrite_doubleBO_box->isChecked();
			abop.options[AssignBondOrderProcessor::Option::OVERWRITE_TRIPLE_BOND_ORDERS] = bond_order_dialog.overwrite_tripleBO_box->isChecked();
			abop.options[AssignBondOrderProcessor::Option::OVERWRITE_SELECTED_BONDS]     = bond_order_dialog.overwrite_selected_bonds_box->isChecked();
			abop.options[AssignBondOrderProcessor::Option::KEKULIZE_RINGS]               = bond_order_dialog.kekulizeBonds_button->isChecked();
			abop.options[AssignBondOrderProcessor::Option::ADD_HYDROGENS]                = bond_order_dialog.add_hydrogens_checkBox->isChecked();
			abop.options[AssignBondOrderProcessor::Option::BOND_LENGTH_WEIGHTING]        = (bond_order_dialog.penalty_balance_slider->value()/100.);

			if (bond_order_dialog.ILP_button->isChecked())
			{
				abop.options[AssignBondOrderProcessor::Option::ALGORITHM] = AssignBondOrderProcessor::Algorithm::ILP;
			} 
			else if (bond_order_dialog.ASTAR_button->isChecked())
			{
				abop.options[AssignBondOrderProcessor::Option::ALGORITHM] = AssignBondOrderProcessor::Algorithm::A_STAR;
			}
			else
			{
				abop.options[AssignBondOrderProcessor::Option::ALGORITHM] = AssignBondOrderProcessor::Algorithm::FPT;
			}

			// get the parameter folder
			//
			// does the given INIFile exist?
			String param_edit_value = ascii(bond_order_dialog.parameter_file_edit->text());

			Directory param_dir(FileSystem::path(param_edit_value));
			if (param_dir.isValid() && param_dir.has(FileSystem::baseName(param_edit_value)))
			{
				abop.options[AssignBondOrderProcessor::Option::INIFile] = param_edit_value;
			}
			else
			{
				setStatusbarText((String)tr("The given parameter file does not exist! Using default!"), true);
			}

			// check for valid inputs
			if (bond_order_dialog.max_n_opt_solutions->text().toInt() < 1)
				bond_order_dialog.max_n_opt_solutions->setText(String(1).c_str());

			if (bond_order_dialog.max_n_all_solutions->text().toInt() < 1)
				bond_order_dialog.max_n_all_solutions->setText(String(1).c_str());

			// get the limitations for number of bond order assignment
			if (bond_order_dialog.single_solution_button->isChecked())
			{
				abop.options[AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS]            = 1;
				abop.options[AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS] = false;
			}
			else if (bond_order_dialog.all_optimal_solutions_button->isChecked())
			{
				abop.options[AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS]            = 0;
				abop.options[AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS] = false;
			}
			else if (bond_order_dialog.n_opt_solutions_button->isChecked())
			{
				abop.options[AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS]            = bond_order_dialog.max_n_opt_solutions->text().toInt();
				abop.options[AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS] = false;
			}
			else if (bond_order_dialog.n_all_solutions_button->isChecked())
			{
				abop.options[AssignBondOrderProcessor::Option::MAX_NUMBER_OF_SOLUTIONS]            = bond_order_dialog.max_n_all_solutions->text().toInt();
				abop.options[AssignBondOrderProcessor::Option::COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS] = true;
			}

			// automatically applying a solution might confuse the user --> set to false
			abop.options.setBool(AssignBondOrderProcessor::Option::APPLY_FIRST_SOLUTION, false);

			// apply
			containers.front()->apply(abop);

			// give a message
			if (abop.getNumberOfComputedSolutions() == 0)
			{
				setStatusbarText((String)tr("Could not find a valid bond order assignment."), true);
			}
			else
			{
				String nr = abop.getNumberOfComputedSolutions();
				setStatusbarText((String)tr("Found ") + nr + (String)tr(" bond order assignments."), true);

				Log.info()<< "  > " + (String)tr("Result AssignBondOrderProcessor: ") << endl;

				for (Size i = 0; i < abop.getNumberOfComputedSolutions(); i++)
				{
					ostringstream stream_description;
					stream_description.setf(std::ios_base::fixed);
					stream_description.precision(2);

					stream_description  << String("      ") + (String)tr("Solution ") << i << " "
						<< (String)tr("has penalty") << " " << abop.getTotalPenalty(i);
					//                  << ", charge " << abop.getTotalCharge(i)
					//                  << ", " <<  abop.getNumberOfAddedHydrogens(i) << " added hydrogens.";

					String description = stream_description.str();

					Log.info() << description << endl;
				}

				ms->showBondOrderAssignmentResults(abop);
			}

			getMainControl()->update(*containers.front(), true);
		}


		void Scene::optimizeStructure()
		{
			if (getMainControl()->isBusy()) return;

			deselect_();
			list<AtomContainer*> containers = getContainers_();
			if (containers.size() < 1) return;

			MolecularStructure* ms = MolecularStructure::getInstance(0);
			if (ms == 0) return;

			AtomContainer* ac = *containers.begin();
			System* system = (System*)&ac->getRoot();

			setStatusbarText((String)tr("Optimizing Structure..."), true);

			// highlight System for minimization
			ControlSelectionMessage* nsm =  new ControlSelectionMessage();
			list<Composite*> selection;
			selection.push_back(system);
			nsm->setSelection(selection);
			notify_(nsm);

			float range = 0.05;
			PreciseTime pt;
			boost::mt19937 rng(pt.now().getMicroSeconds());
			boost::uniform_real<double> random(-range, range);

			AtomIterator ait = ac->beginAtom();
			for (; +ait; ++ait)
			{
				ait->getPosition() += Vector3(random(rng), random(rng), random(rng));
			}
			ms->chooseMMFF94();

			MinimizationDialog& md = ms->getMinimizationDialog();
			md.storeValues();
			md.setMaxIterations(30);
			md.setRefresh(25);
			md.setMaxGradient(5);
			ms->runMinimization(false);

			while (getMainControl()->isBusy())
			{
				getMainControl()->processEvents(999);
				sleepFor(40);
			}

			MolecularDynamicsDialog& mdd = ms->getMDSimulationDialog();
			mdd.storeValues();
			mdd.setTemperature(500);
			mdd.setNumberOfSteps(50);
			mdd.setMicroCanonical(false);
			mdd.setDCDFile("");
			ms->MDSimulation(false);

			while (getMainControl()->isBusy())
			{
				getMainControl()->processEvents(999);
				sleepFor(40);
			}

			md.setMaxIterations(200);
			md.setMaxGradient(1);
			ms->runMinimization(false);

			md.restoreValues();
			mdd.restoreValues();
		}

		void Scene::addToolBarEntries(QToolBar* tb)
		{
			toolbar_view_controls_->addActions(toolbar_actions_view_controls_);
			toolbar_view_controls_->insertSeparator(switch_grid_);
			getMainControl()->addToolBar(Qt::TopToolBarArea, toolbar_view_controls_);
			ModularWidget::addToolBarEntries(tb);
			getMainControl()->initPopupMenu(MainControl::WINDOWS)->addAction(toolbar_view_controls_->toggleViewAction());

			toolbar_actions_edit_controls_.push_back(edit_id_);
			toolbar_actions_edit_controls_.push_back(element_action_);
			toolbar_actions_edit_controls_.push_back(bond_action_);
			toolbar_actions_edit_controls_.push_back(add_hydrogens_action_);
			toolbar_actions_edit_controls_.push_back(optimize_action_);
			toolbar_actions_edit_controls_.push_back(bondorders_action_);
			//TODO make icon
			//toolbar_actions_edit_controls_.push_back(new_molecule_action_);

			toolbar_edit_controls_->addActions(toolbar_actions_edit_controls_);
			getMainControl()->addToolBar(Qt::TopToolBarArea, toolbar_edit_controls_);
			ModularWidget::addToolBarEntries(tb);
			getMainControl()->initPopupMenu(MainControl::WINDOWS)->addAction(toolbar_edit_controls_->toggleViewAction());

			// workaround for Qt, when it only would show the first item:
			//	toolbar_view_controls_->hide();
			//	toolbar_view_controls_->show();
			toolbar_edit_controls_->hide();
			toolbar_edit_controls_->show();
		}

		void Scene::mouseDoubleClickEvent(QMouseEvent* e)
		{
			if (current_mode_ != (ModeType) EDIT__MODE)
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

				return;
			}

			getClickedItems_(e->x(), e->y());
			if (current_atom_ != 0)
			{
				current_atom_->setElement(PTE[atomic_number_]);
				String new_name = PTE[atomic_number_].getSymbol();
				//get the old atom number
				String old_name = current_atom_->getName();
				old_name.toUpper();
				new_name += old_name.trimLeft("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
				current_atom_->setName(new_name);
				deselect_();
				getMainControl()->update(*current_atom_);
				return;
			}

			if (current_bond_ != 0)
			{
				Atom* a1 = (Atom*)current_bond_->getFirstAtom();
				Atom* a2 = (Atom*)current_bond_->getSecondAtom();

				if (!a1->getParent() || (a1->getParent() != a2->getParent()))
					return;

				RingPerceptionProcessor rpp;
				vector<vector<Atom*> > rings;
				Composite* comp = a1->getParent();
				AtomContainer* ac = static_cast<AtomContainer*>(comp);
				if (!ac) return;
				rpp.calculateSSSR(rings, *ac);
				rings = rpp.getAllSmallRings();
				vector<Position> rings_to_modify;
				for (Position r = 0; r < rings.size(); r++)
				{
					Size found = 0;
					for (Position a = 0; a < rings[r].size(); a++)
					{
						if (   (rings[r][a] == a1)
								|| (rings[r][a] == a2))
						{
							found++;
						}
					}

					if (found == 2) rings_to_modify.push_back(r);
				}

				for (Position r = 0; r < rings_to_modify.size(); r++)
				{
					HashSet<Atom*> ratoms;
					vector<Atom*>& ring = rings[rings_to_modify[r]];
					for (Position a = 0; a < ring.size(); a++)
					{
						ratoms.insert(ring[a]);
					}

					for (Position a = 0; a < ring.size(); a++)
					{
						AtomBondIterator abit = ring[a]->beginBond();
						for (;+abit; ++abit)
						{
							if (ratoms.has(abit->getPartner(*ring[a])))
							{
								abit->setOrder(Bond::ORDER__AROMATIC);
							}
						}
					}
				}

				if (rings_to_modify.size() == 0)
				{
					changeBondOrder_(1);
				}

				getMainControl()->update(*a1->getParent(), true);
			}
		}

		void Scene::merge_(Composite* a1, Composite* a2)
		{
			if ( !a1 || !a2 || !a1->getParent() || !a2->getParent() )
			{
				return;
			}

			System* s2 = (System*)&a2->getRoot();

			Composite* p1 = a1->getParent();
			Composite* p2 = a2->getParent();

			Size silb1 = p1->getDegree();
			Size silb2 = p2->getDegree();

			if (silb1 == 1)
			{
				p2->appendChild(*a1);
			}
			else if (silb2 == 1)
			{
				p1->appendChild(*a2);
			}

			Molecule dummy_mol;
			Molecule* m1 = a1->getAncestor(dummy_mol);
			Molecule* m2 = a2->getAncestor(dummy_mol);

			if (!m1 || !m2)
			{
				Log.error() << (String)tr("Internal error! ") << __FILE__ << " " << __LINE__ << endl;
				return;
			}

			Composite* anchestor = a1->getLowestCommonAncestor(*a2);
			if (!anchestor)
			{
				m1->spliceBefore(*m2);
				getMainControl()->remove(*s2);
				return;
			}

			if (m1 == p1)
			{
				p2->appendChild(*a1);
			}
			else if (m2 == p2)
			{
				p1->appendChild(*a2);
			}
			else
			{
				Log.error() << (String)tr("Internal error! ") << __FILE__ << " " << __LINE__ << endl;
			}
		}
	} // namespace VIEW
} // namespace BALL
