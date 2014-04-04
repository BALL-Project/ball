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

#include <BALL/VIEW/DATATYPE/standardDatasets.h>

#include <BALL/VIEW/RENDERING/RENDERERS/POVRenderer.h>
#include <BALL/VIEW/RENDERING/RENDERERS/VRMLRenderer.h>
#include <BALL/VIEW/RENDERING/RENDERERS/XML3DRenderer.h>
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

using namespace std;

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

		Scene::Scene(QWidget* parent_widget, const char* name, Qt::WFlags w_flags)
			:	QWidget(parent_widget, w_flags),
				ModularWidget(name),
				system_origin_(0.0, 0.0, 0.0),
				update_running_(false),
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
				use_preview_(true),
				show_fps_(false),
				toolbar_view_controls_(new QToolBar(tr("3D View Controls"), this)),
				toolbar_edit_controls_(new QToolBar(tr("Edit Controls"), this)),
				main_display_(new GLRenderWindow(this)),
				main_renderer_(0),
				stereo_left_eye_(-1),
				stereo_right_eye_(-1),
				mode_manager_(this)
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
				toolbar_edit_controls_(new QToolBar(tr("Edit Controls"), this)),
				main_display_(new GLRenderWindow(this)),
				main_renderer_(0),
				stereo_left_eye_(-1),
				stereo_right_eye_(-1),
				mode_manager_(this)
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
				//delete(renderers_[i]->renderer);
				delete(renderers_[i]->target);
			}

			if (animation_thread_ != 0) delete animation_thread_;
		}

		void Scene::registerRenderers_()
		{
#ifndef BALL_HAS_RTFACT
			renderers_.push_back(boost::shared_ptr<RenderSetup>(new RenderSetup(gl_renderer_, main_display_, this, stage_)));
#else
			renderers_.push_back(boost::shared_ptr<RenderSetup>(new RenderSetup(rt_renderer_, main_display_, this, stage_)));
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
							{
								renderers_[i]->bufferRepresentation(*rep);
							}

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
				lightsUpdated(true);
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
					exportNextPOVRay();
					return;

				case SceneMessage::ENTER_ROTATE_MODE:
					return;

				case SceneMessage::ENTER_PICKING_MODE:
					return;

				case SceneMessage::ENTER_MOVE_MODE:
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

		String Scene::createFPSInfo_(Renderer* renderer)
		{
			String fps_string;
			float fps = -1;

			// gather renderer specific fps
			if(renderer->hasFPScounter())
			{
				fps = (float)renderer->getFPS();
			}
			// if fps not available render gui fps
			else
			{
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

				list<float>::iterator lit = fps_.begin();
				for (; lit != fps_.end(); lit++)
				{
					fps += *lit;
				}

				fps /= fps_.size();
			}

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

		void Scene::renderText_(QPointF const& point, QString const& text, QPaintDevice* current_dev)
		{
			ColorRGBA text_color = stage_->getBackgroundColor().getInverseColor();

			QPainter p(current_dev);

			QPen pen(QColor((int)text_color.getRed(),  (int)text_color.getGreen(),
						          (int)text_color.getBlue(), (int)text_color.getAlpha()));

			p.setPen(pen);
			p.setRenderHint(QPainter::Antialiasing, true);
			p.setRenderHint(QPainter::TextAntialiasing, true);

			p.drawText(point, text);

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

		// picking routine ------
		// TODO: make renderer configurable

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

			light_settings_->updateFromStage();

			lightsUpdated(update_GL);
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
			if (window_menu_entry_)
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
			stage_settings_->apply();

			if (light_settings_ == 0) return;

			light_settings_->apply();
			lightsUpdated(false);

			bool showed_coordinate = stage_->coordinateSystemEnabled();
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

			main_control.initPopupMenu(MainControl::DISPLAY);

			ShortcutRegistry* shortcut_registry = ShortcutRegistry::getInstance(0);
			QAction* new_action;

			create_coordinate_system_ = NULL;
			QMenu* menu = getMainControl()->initPopupMenu(MainControl::DISPLAY, UIOperationMode::MODE_ADVANCED);
			if (menu)
				create_coordinate_system_ = menu->addMenu(tr("Show Coordinate System"));

			if (create_coordinate_system_)
			{
				create_coordinate_system_->setToolTip(tr("Show a coordinate system"));

				new_action = create_coordinate_system_->addAction(tr("at origin"), this, SLOT(createCoordinateSystemAtOrigin()));
				shortcut_registry->registerShortcut("Shortcut|Display|Show_Coordinate_System|at_origin", new_action);

				new_action = create_coordinate_system_->addAction(tr("here"), this, SLOT(createCoordinateSystem()));
				shortcut_registry->registerShortcut("Shortcut|Display|Show_Coordinate_System|here", new_action);
			}

			insertMenuEntry(MainControl::DISPLAY, tr("Add new GL Window"), this, SLOT(addGlWindow()),
			                "Shortcut|Display|Add_new_GL_Window", QKeySequence(), tr(""), UIOperationMode::MODE_ADVANCED);
#ifdef BALL_HAS_RTFACT
			insertMenuEntry(MainControl::DISPLAY, tr("Add new RTfact Window"), this, SLOT(addRTfactWindow()),
			                "Shortcut|Display|Add_new_RTfact_Window", QKeySequence(), tr(""), UIOperationMode::MODE_ADVANCED);
#endif
			// ======================== Display->Animation ===============================================
			String help_url = "tips.html#animations";

			record_animation_action_ = insertMenuEntry(MainControl::DISPLAY_ANIMATION, tr("Record"), this,
			                                           SLOT(dummySlot()), "Shortcut|Display|Animation|Record", QKeySequence(),
																								 tr("Record an animation for later processing"),
																								 UIOperationMode::MODE_ADVANCED);
			if (record_animation_action_)
			{
				setMenuHelp(record_animation_action_, help_url);
				record_animation_action_->setCheckable(true);
			}

			clear_animation_action_ = insertMenuEntry(MainControl::DISPLAY_ANIMATION, tr("Clear"), this,
			                                          SLOT(clearRecordedAnimation()), "Shortcut|Display|Animation|Clear",
																								QKeySequence(), tr(""), UIOperationMode::MODE_ADVANCED);
			if (clear_animation_action_)
			{
				setMenuHelp(clear_animation_action_, help_url);
			}

			main_control.insertPopupMenuSeparator(MainControl::DISPLAY_ANIMATION, UIOperationMode::MODE_ADVANCED);

			start_animation_action_ = insertMenuEntry(MainControl::DISPLAY_ANIMATION, tr("Start"), this,
			                                          SLOT(startAnimation()), "Shortcut|Display|Animation|Start",
																								QKeySequence(), tr(""), UIOperationMode::MODE_ADVANCED);
			if (start_animation_action_)
			{
				setMenuHelp(start_animation_action_, help_url);
			}

			cancel_animation_action_ = insertMenuEntry(MainControl::DISPLAY_ANIMATION, tr("Stop"), this,
			                                           SLOT(stopAnimation()), "Shortcut|Display|Animation|Stop",
																								 QKeySequence(), tr(""), UIOperationMode::MODE_ADVANCED);

			if (cancel_animation_action_)
			{
				cancel_animation_action_->setEnabled(false);
				setMenuHelp(cancel_animation_action_, help_url);
			}

			main_control.insertPopupMenuSeparator(MainControl::DISPLAY_ANIMATION, UIOperationMode::MODE_ADVANCED);

			animation_export_PNG_action_ = insertMenuEntry(MainControl::DISPLAY_ANIMATION, tr("Export PNG"), this,
			                                               SLOT(dummySlot()), "Shortcut|Display|Animation|Export_PNG",
																										 QKeySequence(), tr(""), UIOperationMode::MODE_ADVANCED);

			if (animation_export_PNG_action_)
			{
				setMenuHelp(animation_export_PNG_action_, help_url);
				animation_export_PNG_action_->setCheckable(true);
			}

			animation_export_POV_action_ = insertMenuEntry(MainControl::DISPLAY_ANIMATION, tr("Export POV"), this,
			                                               SLOT(dummySlot()), "Shortcut|Display|Animation|Export_POV",
																										 QKeySequence(), tr(""), UIOperationMode::MODE_ADVANCED);

			if (animation_export_POV_action_)
			{
				setMenuHelp(animation_export_POV_action_, help_url);
				animation_export_POV_action_->setCheckable(true);
			}

			animation_repeat_action_ = insertMenuEntry(MainControl::DISPLAY_ANIMATION, tr("Repeat"), this,
			                                           SLOT(dummySlot()), "Shortcut|Display|Animation|Repeat",
																								 QKeySequence(), tr(""), UIOperationMode::MODE_ADVANCED);

			if (animation_repeat_action_)
			{
				setMenuHelp(animation_repeat_action_, help_url);
				animation_repeat_action_->setCheckable(true);
			}

			// ======================== Display->Stereo ===============================================
			main_control.insertPopupMenuSeparator(MainControl::DISPLAY, UIOperationMode::MODE_ADVANCED);

			no_stereo_action_ = insertMenuEntry(MainControl::DISPLAY_STEREO, tr("No Stereo"), this,
			                                    SLOT(exitStereo()), "Shortcut|Display|Stereo|No_Stereo",
																					QKeySequence(), tr(""), UIOperationMode::MODE_ADVANCED);

			if (no_stereo_action_)
			{
				no_stereo_action_->setCheckable(true);
				no_stereo_action_->setChecked(true);
				setMenuHelp(no_stereo_action_, "tips.html#3D");
			}

			enter_stereo_action_ = insertMenuEntry(MainControl::DISPLAY_STEREO, tr("Enter Stereo"), this,
			                                       SLOT(enterStereo()), "Shortcut|Display|Stereo|Enter_Stereo",
																						 QKeySequence(), tr(""), UIOperationMode::MODE_ADVANCED);

			if (enter_stereo_action_)
			{
				enter_stereo_action_->setCheckable(true);
				setMenuHelp(enter_stereo_action_, "tips.html#3D");
			}

			//active_stereo_action_ = insertMenuEntry(MainControl::DISPLAY_STEREO, tr("Shutter Glasses"), this,
			//                                        SLOT(enterActiveStereo()), "Shortcut|Display|Stereo|Shutter_Glasses",
			//																				QKeySequence(), tr(""), UIOperationMode::MODE_ADVANCED);

			//if (active_stereo_action_)
			//{
			//	setMenuHelp(active_stereo_action_, "tips.html#3D");
			//	active_stereo_action_->setCheckable(true);
			//}

			//dual_stereo_action_ = insertMenuEntry(MainControl::DISPLAY_STEREO, tr("Side by Side"), this,
			//                                      SLOT(enterDualStereo()), "Shortcut|Display|Stereo|Side_by_Side",
			//																			QKeySequence(), tr(""), UIOperationMode::MODE_ADVANCED);

			//if (dual_stereo_action_)
			//{
			//	setMenuHelp(dual_stereo_action_, "tips.html#3D");
			//	dual_stereo_action_->setCheckable(true);
			//}

			//dual_stereo_different_display_action_ = insertMenuEntry(MainControl::DISPLAY_STEREO,
			//                                                        tr("Side by Side on Different Displays"), this,
			//																												SLOT(enterDualStereoDifferentDisplays()),
			//																												"Shortcut|Display|Stereo|Side_by_Side_on_Different_Displays",
			//																												QKeySequence(), tr(""),
			//																											  UIOperationMode::MODE_ADVANCED);
			//if (dual_stereo_different_display_action_)
			//{
			//	setMenuHelp(dual_stereo_different_display_action_, "tips.html#3D");
			//	dual_stereo_different_display_action_->setCheckable(true);
			//}

			// ======================== Display->Viewpoint ===============================================
			getMainControl()->insertPopupMenuSeparator(MainControl::DISPLAY_VIEWPOINT, UIOperationMode::MODE_ADVANCED);

			insertMenuEntry(MainControl::DISPLAY_VIEWPOINT, tr("&Store Viewpoint"), this,
			                SLOT(storeViewPoint()), "Shortcut|Display|Viewpoint|Store",
											QKeySequence(), tr("Store the current viewpoint"), UIOperationMode::MODE_ADVANCED);

			insertMenuEntry(MainControl::DISPLAY_VIEWPOINT, tr("&Restore Viewpoint"), this,
			                SLOT(restoreViewPoint()), "Shortcut|Display|Viewpoint|Restore", QKeySequence(),
											tr("Restore the viewpoint"), UIOperationMode::MODE_ADVANCED);

			getMainControl()->insertPopupMenuSeparator(MainControl::DISPLAY_VIEWPOINT, UIOperationMode::MODE_ADVANCED);

			String description("Shortcut|Display|Viewpoint|Show_Vie&wpoint");
			insertMenuEntry(MainControl::DISPLAY_VIEWPOINT, tr("Show Viewpoint"), this,
			                SLOT(showViewPoint_()), description, QKeySequence("Ctrl+W"),
											tr("Print the coordinates of the current viewpoint"), UIOperationMode::MODE_ADVANCED);


			description = "Shortcut|Display|Viewpoint|Set_Viewpoint";
			insertMenuEntry(MainControl::DISPLAY_VIEWPOINT, tr("Set Viewpoi&nt"), this,
			                SLOT(setViewPoint_()), description, QKeySequence("Ctrl+N"),
											tr("Move the viewpoint to the given coordinates"), UIOperationMode::MODE_ADVANCED);

			description = "Shortcut|Display|Viewpoint|Reset_Camera";
			insertMenuEntry(MainControl::DISPLAY_VIEWPOINT, tr("Rese&t Camera"), this,
			                SLOT(resetCamera_()), description, QKeySequence(),
											tr("Reset the camera to the orgin (0,0,0)"), UIOperationMode::MODE_ADVANCED);

			getMainControl()->insertPopupMenuSeparator(MainControl::DISPLAY_VIEWPOINT, UIOperationMode::MODE_ADVANCED);

			description = "Shortcut|Display|Viewpoint|Limit_View_Volume";
			insertMenuEntry(MainControl::DISPLAY_VIEWPOINT, tr("Limit View Volume"), this,
			                SLOT(setupViewVolume()), description, QKeySequence(),
											tr(""), UIOperationMode::MODE_ADVANCED);

			description = "Shortcut|File|Export|PNG";
			QAction* screenshot_action = insertMenuEntry(MainControl::FILE_EXPORT, tr("PNG..."), this,
			                                             SLOT(showExportPNGDialog()), description, QKeySequence("Alt+P"),
																									 tr("Export a PNG image file from the Scene"),
																									 UIOperationMode::MODE_ADVANCED);
			setIcon(screenshot_action, "actions/screenshot", false);

			description = "Shortcut|File|Export|POVRay";
			QAction* pov_action = insertMenuEntry(MainControl::FILE_EXPORT, tr("POVRa&y scene"), this,
 			                                      SLOT(exportPOVRay()), description, QKeySequence("Ctrl+Y"),
																						tr("Export a POVRay file from the Scene"),
																						UIOperationMode::MODE_ADVANCED);
			setIcon(pov_action, "mimetype/text-x-povray", false);
			setMenuHelp(pov_action, "tips.html#povray");

			description = "Shortcut|File|Export|XML3D";
			QAction* xml3d_action = insertMenuEntry(MainControl::FILE_EXPORT, tr("XML3D as XHTML"), this,
																						SLOT(exportXML3D()), description, QKeySequence(""),
																						tr("Export a XML3D/XHTML file from the Scene"),
																						UIOperationMode::MODE_ADVANCED);
			setMenuHelp(xml3d_action, "tips.html#XML3D");

			description = "Shortcut|File|Export|VRML";
			insertMenuEntry(MainControl::FILE_EXPORT, tr("3D Prototyping Export"), this,
			                SLOT(showExportVRMLDialog()), description, QKeySequence(),
											tr("Export a VRML or stl file from the scene"),
											UIOperationMode::MODE_ADVANCED);

			// ====================================== MODES =====================================
			switch_grid_ = NULL;
			if (UIOperationMode::instance().getMode() <= UIOperationMode::MODE_ADVANCED)
			{
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
			}

#ifdef BALL_HAS_RTFACT
			toggle_continuous_loop_action_ = NULL;
			if (UIOperationMode::instance().getMode() <= UIOperationMode::MODE_ADVANCED)
			{
				description = "Shortcut|Display|ContinuousLoop|Toggle";
				toggle_continuous_loop_action_ = new QAction(tr("Toggle continuous loop"), this);
				toggle_continuous_loop_action_->setObjectName(toggle_continuous_loop_action_->text());
				connect(toggle_continuous_loop_action_, SIGNAL(triggered()), this, SLOT(toggleContinuousLoop()));
                toggle_continuous_loop_action_->setCheckable(true);
				toggle_continuous_loop_action_->setChecked(false);
				toggle_continuous_loop_action_->setIcon(loader.getIcon("actions/continuous-loop"));
				toolbar_actions_view_controls_.push_back(toggle_continuous_loop_action_);
				toggle_continuous_loop_action_->setShortcut(QKeySequence(Qt::Key_Space));
				shortcut_registry->registerShortcut(description, toggle_continuous_loop_action_);
			}
			// end of the toolbar entries
#endif

			description = "Shortcut|File|Print";
			insertMenuEntry(MainControl::FILE, tr("Print"), this, SLOT(printScene()), description, QKeySequence(),
			                tr("Print the scene"), UIOperationMode::MODE_ADVANCED);

			window_menu_entry_ = insertMenuEntry(MainControl::WINDOWS, tr("Scene"), this, SLOT(switchShowWidget()), "",
			                                     QKeySequence(), tr(""), UIOperationMode::MODE_ADVANCED);
			if (window_menu_entry_)
			{
				window_menu_entry_->setCheckable(true);
				setMenuHelp(window_menu_entry_, "scene.html");
			}

			QWidget::setCursor(QCursor(Qt::ArrowCursor));

			setFocusPolicy(Qt::StrongFocus);
			registerForHelpSystem(this, "scene.html");

			toolbar_view_controls_->setObjectName("3D View Control toolbar");
			toolbar_view_controls_->setIconSize(QSize(22,22));
			toolbar_view_controls_->layout()->setMargin(2);
			toolbar_view_controls_->layout()->setSpacing(2);

			/// EDITABLE SCENE STUFF
			main_control.insertPopupMenuSeparator(MainControl::DISPLAY, UIOperationMode::MODE_ADVANCED);

			bondorders_action_ = NULL;
			if (UIOperationMode::instance().getMode() <= UIOperationMode::MODE_ADVANCED)
			{
				description = "Shortcut|QuicklyAssignBondOrders";
				bondorders_action_ = new QAction(loader.getIcon("actions/molecule-assign-bond-orders"), tr("Quickly optimize bond orders"), this);
				bondorders_action_->setObjectName(bondorders_action_->text());
				bondorders_action_->setToolTip(tr("Compute the bond orders of the highlighted structures"));
				//TODO
				//registerForHelpSystem(bondorders_action_, "scene.html#bondorders");
				connect(bondorders_action_, SIGNAL(triggered()), this, SLOT(computeBondOrders()));
				getMainControl()->getShortcutRegistry().registerShortcut(description, bondorders_action_);
			}

			optimize_action_ = NULL;
			if (UIOperationMode::instance().getMode() <= UIOperationMode::MODE_ADVANCED)
			{
				description = "Shortcut|QuicklyOptimizeStructure";
				optimize_action_ = new QAction(loader.getIcon("actions/molecule-minimize"), tr("Quickly optimize structure"), this);
				optimize_action_->setObjectName(optimize_action_->text());
				optimize_action_->setToolTip(tr("Quickly optimize the highlighted structure"));
				registerForHelpSystem(optimize_action_, "scene.html#optimize");
				connect(optimize_action_, SIGNAL(triggered()), this, SLOT(optimizeStructure()));
				getMainControl()->getShortcutRegistry().registerShortcut(description, optimize_action_);
			}

			add_hydrogens_action_ = NULL;
			if (UIOperationMode::instance().getMode() <= UIOperationMode::MODE_ADVANCED)
			{
				description = "Shortcut|SaturateWithHydrogens";
				add_hydrogens_action_ = new QAction(loader.getIcon("actions/molecule-add-hydrogens"), tr("Saturate with Hydrogens"), this);
				add_hydrogens_action_->setToolTip(tr("Saturate the highlighted structure with hydrogens (with regards to formal charges)"));
				add_hydrogens_action_->setObjectName(add_hydrogens_action_->text());
				registerForHelpSystem(add_hydrogens_action_, "scene.html#saturate");
				connect(add_hydrogens_action_, SIGNAL(triggered()), this, SLOT(saturateWithHydrogens()));
				getMainControl()->getShortcutRegistry().registerShortcut(description, add_hydrogens_action_);
			}

			menu = getMainControl()->initPopupMenu(MainControl::BUILD);
			if (menu)
				menu->addAction(add_hydrogens_action_);


			//TODO create an icon
			new_molecule_action_ = insertMenuEntry(MainControl::BUILD, tr("Create new molecule"),
			                                       this, SLOT(createNewMolecule()), "Shortcut|Build|Create_new_molecule",
																						 QKeySequence(), tr("Create a new molecule for editing"), UIOperationMode::MODE_ADVANCED);

			if (UIOperationMode::instance().getMode() <= UIOperationMode::MODE_ADVANCED)
			{
				toolbar_edit_controls_->setObjectName(tr("Edit Control toolbar"));
				toolbar_edit_controls_->setIconSize(QSize(23,23));
				toolbar_edit_controls_->layout()->setMargin(2);
				toolbar_edit_controls_->layout()->setSpacing(2);
			}
		}

		void Scene::checkMenu(MainControl& main_control)
		{
			bool busy = main_control.compositesAreLocked();

			if (create_coordinate_system_)
				create_coordinate_system_->setEnabled(!busy);

			bool animation_running = (animation_thread_ != 0 && animation_thread_->isRunning());

			if (start_animation_action_)
			{
				start_animation_action_->setEnabled(animation_points_.size() > 0 && !busy && !animation_running);
			}

			if (clear_animation_action_)
				clear_animation_action_->setEnabled(animation_points_.size() > 0 && !animation_running);

			if (window_menu_entry_)
				window_menu_entry_->setChecked(isVisible());
			bool selected_system = !busy && main_control.getSelectedSystem();

			if (optimize_action_)
				optimize_action_->setEnabled(selected_system);
			if (add_hydrogens_action_)
				add_hydrogens_action_->setEnabled(selected_system);

			list<Composite*> highl = getMainControl()->getMolecularControlSelection();
			list<Composite*>::iterator lit = highl.begin();
			bool selected_system_or_molecule =   (highl.size() == 1)
				&& (RTTI::isKindOf<System>(**lit) || RTTI::isKindOf<Molecule>(**lit) ) ;

			if (bondorders_action_)
				bondorders_action_->setEnabled(selected_system_or_molecule && !busy);
			if (optimize_action_)
				optimize_action_->setEnabled(selected_system_or_molecule && !busy);
			if (add_hydrogens_action_)
				add_hydrogens_action_->setEnabled(selected_system_or_molecule && !busy);

			if (new_molecule_action_)
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
			boost::shared_ptr<RenderSetup> renderer = evt->getRenderer();

			if (renderer->isAboutToQuit())
			{
				// stopped renderers need to be ignored
				return;
			}

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
				fps_string = createFPSInfo_(renderer->renderer);

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

				renderText_(QPointF(info_point_.x()*xscale, info_point_.y()*yscale), info_string_, current_dev);
			}

			// and paint our overlay, if we have one
			if (has_overlay_ && current_dev)
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
			if (renderer->isReadyToSwap())
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
					if (*it == renderer)
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
			// strangely enough, this can actually happen...
			if (!evt)
				return;

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

			//Fix this!
		//	if (!mouse_button_is_pressed_)
		//	{
		//		old_trackorigin_ = new_trackorigin;
		//		old_trackrotation_ = new_trackrotation;
		//		return;
		//	}

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
			has_overlay_ = true;

			mode_manager_.mouseMoveEvent(e);
		}

		void Scene::mousePressEvent(QMouseEvent* e)
		{
			mode_manager_.mousePressEvent(e);
		}

		void Scene::mouseReleaseEvent(QMouseEvent* e)
		{
			has_overlay_ = false;

			mode_manager_.mouseReleaseEvent(e);
		}

		void Scene::mouseDoubleClickEvent(QMouseEvent* e)
		{
			mode_manager_.mouseDoubleClickEvent(e);
		}

		void Scene::keyPressEvent(QKeyEvent* evt)
		{
			mode_manager_.keyPressEvent(evt);
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
				pickObjects(QPoint(pos_x - p, pos_y - p), QPoint(pos_x + p, pos_y + p), objects);
				if (!objects.empty()) break;
			}

			if (objects.empty())
			{
				unlockComposites();
				return;
			}

			// get the description
			QString string;
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
				QString this_string(info.getName().c_str());
				if (composite->getParent() != 0 &&
						RTTI::isKindOf<Residue>(*composite->getParent()))
				{
					info.visit(*composite->getParent());
					this_string = QString(info.getName().c_str()) + " : " + this_string;
				}

				if (this_string == "UNKNOWN") continue;

				if (RTTI::isKindOf<Atom>(*composite))
				{
					this_string += "[T:";
					this_string += ((Atom*)composite)->getTypeName().c_str();
					this_string += ",FC:";
					this_string += QString::number(((Atom*)composite)->getFormalCharge());
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

			QString string2 = tr("Object at cursor is ") + string;

			if (string2 != getMainControl()->getStatusbarText().c_str())
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

		void Scene::projectionModeChanged()
		{
			for (Position i=0; i<renderers_.size(); ++i)
			{
				renderers_[i]->projectionModeChanged();
			}

			updateGL();
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

		void Scene::exportXML3D()
		{
			QString qresult = QFileDialog::getSaveFileName(
												0,
												"Export XML3D as XHTML File",
												getWorkingDir().c_str(),
												"*.xhtml");

			if (qresult == QString::null) return;

			String result = ascii(qresult);

			if (!result.hasSuffix(".xhtml"))
			{
				result += ".xhtml";
			}

			bool ok = false;

			try
			{
				main_display_->makeCurrent();
				XML3DRenderer pr(result);
				if (exportScene(pr)) ok = true;
			}
			catch(...)
			{
			}

			if (!ok)
			{
				setStatusbarText("Could not export XML3D/XHTML to " + result, true);
			}
			else
			{
				setStatusbarText("Exported XML3D/XHTML  to " + result);
				setWorkingDirFromFilename_(result);
			}
		}

		void Scene::dumpXML3D(std::ostream& xml3ddump)
		{
			bool ok = false;

			try
			{
//				main_display_->makeCurrent();
				XML3DRenderer pr(xml3ddump);

				if (exportScene(pr)) ok = true;
			}
			catch(...)
			{
			}

			if (!ok)
			{
				setStatusbarText((String)tr("Could not export XML3D/XHTML to string"), true);
			}
			else
			{
				setStatusbarText((String)tr("Dumped XML3D/XHTML to string"));
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
			if (no_stereo_action_) no_stereo_action_->setChecked(true);
			if (enter_stereo_action_) enter_stereo_action_->setChecked(false);
			//if (active_stereo_action_) active_stereo_action_->setChecked(false);
			//if (dual_stereo_action_) dual_stereo_action_->setChecked(false);
			//if (dual_stereo_different_display_action_) dual_stereo_different_display_action_->setChecked(false);

			if (stereo_left_eye_ == -1 || stereo_right_eye_ == -1) return;
//			if (gl_renderer_->getStereoMode() == Renderer::NO_STEREO) return;

			// remember pointers to the left and right windows
			RenderTarget* left_window  = (stereo_left_eye_  != -1) ? renderers_[stereo_left_eye_ ]->target : 0;
			RenderTarget* right_window = (stereo_right_eye_ != -1) ? renderers_[stereo_right_eye_]->target : 0;

			// stop rendering
			stopContinuousLoop();
			renderers_[stereo_left_eye_]->stop();
			renderers_[stereo_left_eye_]->loop_mutex.lock();
			renderers_[stereo_left_eye_]->wait_for_render.wakeAll();
			renderers_[stereo_left_eye_]->loop_mutex.unlock();

			renderers_[stereo_right_eye_]->stop();
			renderers_[stereo_right_eye_]->loop_mutex.lock();
			renderers_[stereo_right_eye_]->wait_for_render.wakeAll();
			renderers_[stereo_right_eye_]->loop_mutex.unlock();

			renderers_[stereo_left_eye_]->wait(100);
			renderers_[stereo_right_eye_]->wait(100);

			delete(renderers_[stereo_left_eye_]->renderer);
			delete(renderers_[stereo_right_eye_]->renderer);

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

			gl_renderer_->setStereoMode(Renderer::NO_STEREO);
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
			if (main_renderer_ >= renderers_.size())
			{
				Log.warn() << "SwitchRenderer: invalid renderer requested";
				return;
			}

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

			stopContinuousLoop();
#ifdef BALL_HAS_RTFACT
			if (UIOperationMode::instance().getMode() <= UIOperationMode::MODE_ADVANCED)
			{
				toggle_continuous_loop_action_->setEnabled(new_type == RenderSetup::RTFACT_RENDERER);
			}
#endif

			main_renderer_ptr->stop();

			main_renderer_ptr->loop_mutex.lock();
			main_renderer_ptr->wait_for_render.wakeAll();
			main_renderer_ptr->loop_mutex.unlock();

			main_renderer_ptr->wait(100);

			delete(main_renderer_ptr->renderer);

			renderers_.erase(renderers_.begin() + main_renderer_);

			main_display_->makeCurrent();

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

			if (stereo_left_eye_  > main_renderer_) stereo_left_eye_--;
			if (stereo_right_eye_ > main_renderer_) stereo_right_eye_--;

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

		void Scene::enterStereo()
		{
			// first clean up
			exitStereo();

			// get the correct screens for control, left, and right eye
			// TODO: handle the control screen! currently, we just leave it alone
			int control_screen_index = stage_settings_->getControlScreenNumber();
			int left_screen_index = stage_settings_->getLeftEyeScreenNumber();
			int right_screen_index = stage_settings_->getRightEyeScreenNumber();

			if (left_screen_index == -1 || right_screen_index == -1)
			{
				QMessageBox *box = new QMessageBox;
				box->setText("Stereo setup invalid");
				box->setInformativeText("Please assign the displays to valid screens in Preferences->Main->Stereo->Display Settings");
				box->show();

				return;
			}

			QDesktopWidget* desktop = QApplication::desktop();

			QWidget* left_screen  = desktop->screen(left_screen_index);
			QWidget* right_screen = desktop->screen(right_screen_index);

			QRect left_geometry  = stage_settings_->getLeftEyeGeometry();
			QRect right_geometry = stage_settings_->getRightEyeGeometry();

			RenderSetup::RendererType control_renderer_type = stage_settings_->getControlScreenRendererType();
			RenderSetup::RendererType stereo_renderer_type = stage_settings_->getStereoScreensRendererType();

#ifndef BALL_HAS_RTFACT
			if (control_renderer_type == RenderSetup::RTFACT_RENDERER || stereo_renderer_type == RenderSetup::RTFACT_RENDERER)
			{
				QMessageBox *box = new QMessageBox;
				box->setText("Stereo setup invalid");
				box->setInformativeText("BALLView was compiled without RTfact support! Please run stereo setup again!");
				box->show();

				return;
			}
#endif
			Renderer::StereoMode mode = stage_settings_->getStereoMode();

			if (mode == Renderer::DUAL_VIEW_STEREO || mode == Renderer::TOP_BOTTOM_STEREO)
			{
				// in both cases (side by side and top bottom), we can
				// use the same code
				GLRenderWindow* left_widget = new GLRenderWindow(left_screen, "left eye", Qt::FramelessWindowHint);
				left_widget->setDoNotResize(true);
				left_widget->makeCurrent();
				left_widget->init();
				left_widget->resize(left_geometry.width(), left_geometry.height());
				left_widget->move(left_geometry.x(), left_geometry.y());

				GLRenderWindow* right_widget = new GLRenderWindow(right_screen, "right eye", Qt::FramelessWindowHint);
				right_widget->setDoNotResize(true);
				right_widget->makeCurrent();
				right_widget->init();
				right_widget->resize(right_geometry.width(), right_geometry.height());
				right_widget->move(right_geometry.x(), right_geometry.y());

				Renderer* left_renderer;
				Renderer* right_renderer;

				if (stereo_renderer_type == RenderSetup::OPENGL_RENDERER)
				{
					left_widget->makeCurrent();
					left_renderer = new GLRenderer;
					left_renderer->init(*this);
					left_renderer->setStereoMode(Renderer::DUAL_VIEW_STEREO);
					static_cast<GLRenderer*>(left_renderer)->enableVertexBuffers(want_to_use_vertex_buffer_);

					right_widget->makeCurrent();
					right_renderer = new GLRenderer;
					right_renderer->init(*this);
					right_renderer->setStereoMode(Renderer::DUAL_VIEW_STEREO);
					static_cast<GLRenderer*>(right_renderer)->enableVertexBuffers(want_to_use_vertex_buffer_);
				}
				else if (stereo_renderer_type == RenderSetup::RTFACT_RENDERER)
				{
#ifdef BALL_HAS_RTFACT
					left_renderer = new t_RaytracingRenderer();
					left_renderer->init(*this);
					static_cast<t_RaytracingRenderer*>(left_renderer)->setFrameBufferFormat(left_widget->getFormat());

					right_renderer = new t_RaytracingRenderer();
					right_renderer->init(*this);
					static_cast<t_RaytracingRenderer*>(right_renderer)->setFrameBufferFormat(right_widget->getFormat());
#endif
				}

				left_widget->show();
				right_widget->show();

				left_renderer->setSize(left_widget->width(),   left_widget->height());
				right_renderer->setSize(right_widget->width(), right_widget->height());

				// we may need to account for differences in the frusta
				QRect left_screen_size = QApplication::desktop()->screenGeometry(left_screen_index);
				left_renderer->setStereoFrustumConversion(left_screen_size.width()  / left_geometry.width(),
                                                  left_screen_size.height() / left_geometry.height());

				QRect right_screen_size = QApplication::desktop()->screenGeometry(right_screen_index);
				right_renderer->setStereoFrustumConversion(right_screen_size.width()  / right_geometry.width(),
                                                   right_screen_size.height() / right_geometry.height());

				boost::shared_ptr<RenderSetup> left_rs(new RenderSetup(left_renderer, left_widget, this, stage_));

				resetRepresentationsForRenderer_(*left_rs);
				left_rs->setStereoMode(RenderSetup::LEFT_EYE);

				renderers_.push_back(left_rs);
				stereo_left_eye_ = renderers_.size()-1;
				left_rs->start();

				boost::shared_ptr<RenderSetup> right_rs(new RenderSetup(right_renderer, right_widget, this, stage_));

				resetRepresentationsForRenderer_(*right_rs);
				right_rs->setStereoMode(RenderSetup::RIGHT_EYE);

				renderers_.push_back(right_rs);
				stereo_right_eye_ = renderers_.size()-1;
				right_rs->start();

				left_rs->makeDependentOn(right_rs);
				right_rs->makeDependentOn(left_rs);
			}
			else
			{
				Log.error() << "The chosen stereo mode is currently not implemented! Sorry!" << std::endl;
			}

			switchRenderer(control_renderer_type);

			// why do we need *this*?
			applyPreferences();
			updateGL();

			if (no_stereo_action_) no_stereo_action_->setChecked(false);
			//if (active_stereo_action_)  active_stereo_action_->setChecked(false);
			if (enter_stereo_action_) enter_stereo_action_->setChecked(true);
			//if (dual_stereo_action_) dual_stereo_action_->setChecked(false);
			//if (dual_stereo_different_display_action_) dual_stereo_different_display_action_->setChecked(false);
		}

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

//			left_renderer->setStereoMode(Renderer::ACTIVE_STEREO);
//			right_renderer->setStereoMode(Renderer::ACTIVE_STEREO);

			setFullScreen(true);

			no_stereo_action_->setChecked(false);
			active_stereo_action_->setChecked(true);
			dual_stereo_action_->setChecked(false);
			dual_stereo_different_display_action_->setChecked(false);
		}

		// TODO: merge with enterDualStereoDifferentDisplays, decide through the stage_settings
		void Scene::enterDualStereo()
		{
			// first clean up
			exitStereo();

			// get the correct screens for control, left, and right eye
			// TODO: handle the control screen! currently, we just leave it alone
			int control_screen_index = stage_settings_->getControlScreenNumber();
			int left_screen_index = stage_settings_->getLeftEyeScreenNumber();
			int right_screen_index = stage_settings_->getRightEyeScreenNumber();

			if (left_screen_index == -1 || right_screen_index == -1)
			{
				QMessageBox *box = new QMessageBox;
				box->setText("Stereo setup invalid");
				box->setInformativeText("Please assign the displays to valid screens in Preferences->Main->Stereo->Display Settings");
				box->show();

				return;
			}

			if (left_screen_index != right_screen_index)
			{
				QMessageBox *box = new QMessageBox;
				box->setText("Stereo setup invalid");
				box->setInformativeText("Currently, this stereo mode requires both eyes to be located on the same screen!");
				box->show();

				return;
			}

			QDesktopWidget* desktop = QApplication::desktop();
			QRect screen_geom = QApplication::desktop()->screenGeometry(left_screen_index);

			QWidget* left_screen = QApplication::desktop()->screen(left_screen_index);
			QWidget* right_screen = left_screen;

			GLRenderWindow* left_widget = new GLRenderWindow(left_screen, "left eye", Qt::FramelessWindowHint);
			left_widget->makeCurrent();
			left_widget->init();
			left_widget->resize(screen_geom.width() / 2, screen_geom.height());
			left_widget->move(screen_geom.x(), screen_geom.y());

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

			GLRenderWindow* right_widget = new GLRenderWindow(right_screen, "right eye", Qt::FramelessWindowHint);
			right_widget->makeCurrent();
			right_widget->init();
			right_widget->resize(screen_geom.width() / 2, screen_geom.height());
			right_widget->move(screen_geom.x() + screen_geom.width() / 2, screen_geom.y());

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

			left_renderer->setStereoMode(Renderer::DUAL_VIEW_STEREO);
			right_renderer->setStereoMode(Renderer::DUAL_VIEW_STEREO);

			setFullScreen(false);
			applyPreferences();
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

			// get the correct screens for control, left, and right eye
			// TODO: handle the control screen! currently, we just leave it alone
			int control_screen_index = stage_settings_->getControlScreenNumber();
			int left_screen_index = stage_settings_->getLeftEyeScreenNumber();
			int right_screen_index = stage_settings_->getRightEyeScreenNumber();

			if (left_screen_index == -1 || right_screen_index == -1)
			{
				QMessageBox *box = new QMessageBox;
				box->setText("Stereo setup invalid");
				box->setInformativeText("Please assign the displays to valid screens in Preferences->Main->Stereo->Display Settings");
				box->show();

				return;
			}

			QWidget* left_screen = QApplication::desktop()->screen(left_screen_index);
			GLRenderWindow* left_widget = new GLRenderWindow(left_screen, String(tr("left eye")).c_str());
			left_widget->makeCurrent();
			left_widget->init();
			left_widget->resize(left_screen->width(), left_screen->height());

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

			QWidget* right_screen = QApplication::desktop()->screen(right_screen_index);
			GLRenderWindow* right_widget = new GLRenderWindow(right_screen, String(tr("right eye")).c_str());
			right_widget->makeCurrent();
			right_widget->init();
			right_widget->resize(right_screen->width(), right_screen->height());

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

			left_renderer->setStereoMode(Renderer::DUAL_VIEW_DIFFERENT_DISPLAY_STEREO);
			right_renderer->setStereoMode(Renderer::DUAL_VIEW_DIFFERENT_DISPLAY_STEREO);

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
				// TODO: OpenGL does *not* like the continuous loop
				if (!renderers_[i]->isContinuous() && (renderers_[i]->getRendererType() != RenderSetup::OPENGL_RENDERER))
				{
					renderers_[i]->useContinuousLoop(true);
					if (UIOperationMode::instance().getMode() <= UIOperationMode::MODE_ADVANCED)
					{
						toggle_continuous_loop_action_->setChecked(true);
					}

					renderers_[i]->loop_mutex.lock();
					renderers_[i]->wait_for_render.wakeAll();
					renderers_[i]->loop_mutex.unlock();
        }
			}
			setStatusbarText(tr("Switched continuous loop on"), true);
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
					if (UIOperationMode::instance().getMode() <= UIOperationMode::MODE_ADVANCED)
					{
						toggle_continuous_loop_action_->setChecked(false);
					}
				}
			}
			setStatusbarText(tr("Switched continuous loop off"), true);
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

		void Scene::applyStereoDefaults()
		{
			stage_settings_->computeDefaultPressed();
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
			mode_manager_.switchToLastMode();
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

		bool Scene::inMoveMode() const
		{
			return mode_manager_.getCurrentModeName() == "MoveMode";
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

		void Scene::updateAllMaterials()
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

		void Scene::updateMaterialForRepresentation(Representation* rep, const Stage::Material& new_material)
		{
			rep->clearProperty("Rendering::Material");
			boost::shared_ptr<PersistentObject> p(new Stage::Material(new_material));
			NamedProperty rt_mat_property("Rendering::Material", p);

			rep->setProperty(rt_mat_property);

			for (Position i=0; i<renderers_.size(); ++i)
			{
				renderers_[i]->updateMaterialForRepresentation(rep);
			}

			updateGL();
		}

		void Scene::lightsUpdated(bool redraw)
		{
			for (Position i=0; i<renderers_.size(); ++i)
				renderers_[i]->setLights(true);

			if (redraw)
				updateGL();
		}

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
			current_atom_ = 0;
			current_bond_ = 0;
			edit_settings_ = 0;
			atomic_number_ = 6;
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
			mode_manager_.wheelEvent(e);
		}

		void Scene::changeBondOrder(Index delta)
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
		list<AtomContainer*> Scene::getContainers()
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

		void Scene::enterPickingMode()
		{
			gl_renderer_->enterPickingMode();
		}

		void Scene::exitPickingMode()
		{
			gl_renderer_->exitPickingMode();
		}

		void Scene::getClickedItems(const QPoint& p)
		{
			current_bond_ = 0;
			current_atom_ = 0;

			list<GeometricObject*> objects;
			pickObjects(p, p, objects);

			if (objects.size() > 0)
			{
				Composite* c = (Composite*)(**objects.begin()).getComposite();
				if (c == 0) return;

				current_bond_ = dynamic_cast<Bond*>(c);
				current_atom_ = dynamic_cast<Atom*>(c);
			}

		}

		// TODO: make renderer / target configurable
		void Scene::setElementCursor(int number)
		{
			String s = PTE[number].getSymbol();
			setCursor(s.c_str());
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
			setElementCursor(atomic_number_);
		}

		// Get the element for the next insert operations
		int Scene::getEditElementType()
		{
			return atomic_number_;
		}

		void Scene::showContextMenu(QPoint pos)
		{
			QMenu menu;
			mode_manager_.populateContextMenu(&menu);

			menu.addSeparator();

			menu.exec(mapToGlobal(pos));

			// if we switched to move mode, let the user move the atom:
			if (inMoveMode()) return;

			// otherwise deselect all selected items
			deselect();
		}

		void Scene::setFormalCharge_()
		{
			deselect();

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

		void Scene::deselect(bool update)
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

		void Scene::setInfo(const QString& str)
		{
			info_string_ = str;
		}

		Vector3 Scene::mapViewportTo3D(const Vector2& coords)
		{
			return renderers_[main_renderer_]->mapViewportTo3D(coords.x, coords.y);
		}

		Vector3 Scene::mapViewportTo3D(int x, int y)
		{
			return renderers_[main_renderer_]->mapViewportTo3D(x, y);
		}

		Vector2 Scene::map3DToViewport(const Vector3& coords)
		{
			return renderers_[main_renderer_]->map3DToViewport(coords);
		}

		void Scene::pickObjects(const QPoint& pos, list<GeometricObject*>& objects)
		{
			pickObjects(pos, pos, objects);
		}

		void Scene::pickObjects(const QPoint& pos1, const QPoint& pos2, list<GeometricObject*>& objects)
		{
			renderers_[main_renderer_]->pickObjects((Position) pos1.x(), (Position) pos1.y(),
			                                        (Position) pos2.x(), (Position) pos2.y(), objects);
		}

		void Scene::setRubberBandGeometry(const QRect& rect)
		{
			rb_->setGeometry(rect);
		}

		void Scene::setRubberBandVisible(bool show)
		{
			rb_->setVisible(show);
		}

		bool Scene::isRecording() const
		{
			if (!record_animation_action_)
				return false;
			else
				return record_animation_action_->isChecked();
		}

		void Scene::captureCamera()
		{
			animation_points_.push_back(getStage()->getCamera());
		}

		void Scene::drawLine(const QPointF& a, const QPointF& b, QPainter* painter)
		{
			QPainter* p = painter;

			if(!p)
			{
				p = new QPainter(&overlay_);
			}

			QColor color;
			getStage()->getBackgroundColor().getInverseColor().get(color);

			QPen pen(color);
			pen.setWidth(3);
			pen.setStyle(Qt::DashLine);

			p->setPen(pen);
			p->setRenderHint(QPainter::Antialiasing, true);
			p->drawLine(a, b);

			has_overlay_ = true;

			if(!painter)
			{
				delete p;
			}
		}

		void Scene::drawLine(const Vector2& a, const Vector2& b, QPainter* painter)
		{
			drawLine(QPointF(a.x, a.y), QPointF(b.x, b.y), painter);
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

		// TODO: make the renderer dependent on the current target!

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
		}

		void Scene::saturateWithHydrogens()
		{
			if (getMainControl()->isBusy()) return;

			deselect(false);
			list<AtomContainer*> containers = getContainers();
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

			deselect(false);

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

			deselect();
			list<AtomContainer*> containers = getContainers();
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
			
			Position current_ff_id = ms->getForceFieldID();
			ms->chooseForceField(current_ff_id);
			
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
			mode_manager_.init();

			if (UIOperationMode::instance().getMode() <= UIOperationMode::MODE_ADVANCED)
			{
				toolbar_view_controls_->addActions(toolbar_actions_view_controls_);
				toolbar_view_controls_->insertSeparator(switch_grid_);
				getMainControl()->addToolBar(Qt::TopToolBarArea, toolbar_view_controls_);
				ModularWidget::addToolBarEntries(tb);
				getMainControl()->initPopupMenu(MainControl::WINDOWS)->addAction(toolbar_view_controls_->toggleViewAction());

				mode_manager_.addToolBarEntries(toolbar_edit_controls_);

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
		}

		void Scene::merge(Composite* a1, Composite* a2)
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
