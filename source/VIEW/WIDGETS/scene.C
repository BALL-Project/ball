// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: scene.C,v 1.32 2004/01/13 16:04:03 amoll Exp $
//

#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/events.h>
#include <BALL/VIEW/DIALOGS/mouseSensitivity.h>
#include <BALL/VIEW/DIALOGS/setCamera.h>
#include <BALL/VIEW/DIALOGS/preferences.h>
#include <BALL/VIEW/DIALOGS/lightSettings.h>
#include <BALL/VIEW/DIALOGS/stageSettings.h>
#include <BALL/VIEW/PRIMITIVES/simpleBox.h>
#include <BALL/VIEW/PRIMITIVES/label.h>

#include <qpainter.h>
#include <qmenubar.h>
#include <qpixmap.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qcursor.h>

using std::endl;
using std::ostream;
using std::istream;

namespace BALL
{
	namespace VIEW
	{

// ###############CONSTRUCTORS,DESTRUCTORS,CLEAR###################

float Scene::mouse_sensitivity_ = 5;

// values for mouse-sensitivity 
#define  ZOOM_FACTOR 			7 //5
#define  ROTATE_FACTOR    11 //8
#define  TRANSLATE_FACTOR 6 // 4

Scene::Scene()
	throw()
	:	QGLWidget(),
		ModularWidget("<Scene>"),
		events(),
		rotate_id_(-1),
		picking_id_(-1),
		rotate_mode_(true),
		system_origin_(0.0),
		quaternion_(),
		key_pressed_(Scene::KEY_PRESSED__NONE),
		need_update_(false),
		update_running_(false),
		stage_(new Stage),
		light_settings_(0),
		stage_settings_(0),
		screenshot_nr_(10000)
{
	gl_renderer_.setSize(600, 600);
#ifdef BALL_VIEW_DEBUG
	Log.error() << "new Scene (1) " << this << std::endl;
#endif
}

Scene::Scene(QWidget* parent_widget, const char* name, WFlags w_flags)
	throw()
	:	QGLWidget(parent_widget, name, 0, w_flags),
		ModularWidget(name),
		events(this),
		rotate_id_(-1),
		picking_id_(-1),
		rotate_mode_(true),
		system_origin_(0.0, 0.0, 0.0),
		quaternion_(),
		key_pressed_(Scene::KEY_PRESSED__NONE),
		need_update_(false),
		update_running_(false),
		stage_(new Stage),
		light_settings_(0),
		stage_settings_(0),
		screenshot_nr_(1)
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "new Scene (2) " << this << std::endl;
#endif
	events.RotateSystem.registerRotateSystem(&BALL::VIEW::Scene::rotateSystem_);
	events.TranslateSystem.registerTranslateSystem(&BALL::VIEW::Scene::translateSystem_);
	events.ZoomSystem.registerZoomSystem(&BALL::VIEW::Scene::zoomSystem_);

	events.SelectionPressedMoved.registerSelectionPressedMoved(&BALL::VIEW::Scene::selectionPressedMoved_);
	events.SelectionPressed.registerSelectionPressed(&BALL::VIEW::Scene::selectionPressed_);
	events.SelectionReleased.registerSelectionReleased(&BALL::VIEW::Scene::selectionReleased_);

	events.DeselectionPressedMoved.registerDeselectionPressedMoved(&BALL::VIEW::Scene::deselectionPressedMoved_);
	events.DeselectionPressed.registerDeselectionPressed(&BALL::VIEW::Scene::deselectionPressed_);
	events.DeselectionReleased.registerDeselectionReleased(&BALL::VIEW::Scene::deselectionReleased_);

	NotificationRegister(events.MouseLeftButtonPressed & events.MouseMoved, events.RotateSystem);
	NotificationRegister(events.MouseMiddleButtonPressed & events.MouseMoved, events.ZoomSystem);
	NotificationRegister(events.MouseRightButtonPressed & events.MouseMoved, events.TranslateSystem);

	// the widget with the MainControl
	registerWidget(this);
	gl_renderer_.setSize(600, 600);

	if (!QGLWidget::isValid())
	{
		Log.error() << "QGLWidget is not valid in Scene!" << std::endl;
	}
}

Scene::Scene(const Scene& scene, QWidget* parent_widget, const char* name, WFlags w_flags)
	throw()
	:	QGLWidget(parent_widget, name, 0, w_flags),
		ModularWidget(scene),
		events(this),
		system_origin_(scene.system_origin_),
		quaternion_(scene.quaternion_),
		key_pressed_(Scene::KEY_PRESSED__NONE),
		stage_(new Stage(*scene.stage_)),
		light_settings_(new LightSettings(this)),
		stage_settings_(new StageSettings(this)),
		screenshot_nr_(1)
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "new Scene (3) " << this << std::endl;
#endif
	events.RotateSystem.registerRotateSystem(&BALL::VIEW::Scene::rotateSystem_);
	events.TranslateSystem.registerTranslateSystem(&BALL::VIEW::Scene::translateSystem_);
	events.ZoomSystem.registerZoomSystem(&BALL::VIEW::Scene::zoomSystem_);

	events.SelectionPressedMoved.registerSelectionPressedMoved(&BALL::VIEW::Scene::selectionPressedMoved_);
	events.SelectionPressed.registerSelectionPressed(&BALL::VIEW::Scene::selectionPressed_);
	events.SelectionReleased.registerSelectionReleased(&BALL::VIEW::Scene::selectionReleased_);

	events.DeselectionPressedMoved.registerDeselectionPressedMoved(&BALL::VIEW::Scene::deselectionPressedMoved_);
	events.DeselectionPressed.registerDeselectionPressed(&BALL::VIEW::Scene::deselectionPressed_);
	events.DeselectionReleased.registerDeselectionReleased(&BALL::VIEW::Scene::deselectionReleased_);

	NotificationRegister(events.MouseLeftButtonPressed & events.MouseMoved, events.RotateSystem);
	NotificationRegister(events.MouseMiddleButtonPressed & events.MouseMoved, events.ZoomSystem);
	NotificationRegister(events.MouseRightButtonPressed & events.MouseMoved, events.TranslateSystem); 

	resize((Size) scene.gl_renderer_.getWidth(), (Size) scene.gl_renderer_.getHeight());

	// the widget with the MainControl
	ModularWidget::registerWidget(this);
}

Scene::~Scene()
	throw()
{
	#ifdef BALL_VIEW_DEBUG
		Log.info() << "Destructing object Scene " << this << " of class Scene>" << std::endl;
	#endif 

	delete stage_;
}

void Scene::clear()
	throw()
{
	system_origin_.set(0.0, 0.0, 0.0);
	quaternion_.setIdentity();
	key_pressed_ = Scene::KEY_PRESSED__NONE;

	stage_->clear();
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
	return (getParent() != 0 && system_origin_.isValid());
}

void Scene::dump(ostream& s, Size depth) const
	throw()
{
	BALL_DUMP_STREAM_PREFIX(s);

	BALL_DUMP_DEPTH(s, depth);
	BALL_DUMP_HEADER(s, this, this);

	stage_->dump(s, depth);
	
	quaternion_.dump(s, depth + 1);

	BALL_DUMP_DEPTH(s, depth + 1);
	s << "KeyPressed: " << (Index)key_pressed_ << endl;

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
	else
	{
		renderView_(DISPLAY_LISTS_RENDERING);
	}

	updateGL();
	update_running_ = false;
}


void Scene::onNotify(Message *message)
	throw()
{
#ifdef BALL_VIEW_DEBUG
	Log.error() << "Scene " << this  << "onNotify " << message << std::endl;
#endif
	if (RTTI::isKindOf<RepresentationMessage>(*message)) 
	{
		makeCurrent();
		RepresentationMessage* rm = RTTI::castTo<RepresentationMessage>(*message);
		Representation* rep = rm->getRepresentation();
		switch (rm->getType())
		{
			case RepresentationMessage::ADD:
				gl_renderer_.buildDisplayListFor(*rep);
				break;

			case RepresentationMessage::REMOVE:
				gl_renderer_.removeDisplayListFor(*rep);
				break;

			case RepresentationMessage::UPDATE:
				gl_renderer_.rebuildDisplayListFor(*rep);
				break;

			case RepresentationMessage::UNDEFINED:
			case RepresentationMessage::SELECTED:
				break;
		}

		update(false);
		return;
	}

	// react now only to SceneMessage
	if (!RTTI::isKindOf<SceneMessage>(*message)) return;

	makeCurrent();

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
			stage_->moveCameraTo(scene_message->getCamera());
			system_origin_ = scene_message->getCamera().getLookAtPosition();
			updateCamera_();
			return;

		case SceneMessage::REMOVE_COORDINATE_SYSTEM:
			stage_->showCoordinateSystem(false);
			stage_settings_->updateFromStage();
			return;

		case SceneMessage::EXPORT_PNG:
			exportPNG();
			return;

		case SceneMessage::UNDEFINED:
			Log.error() << "Unknown type of SceneMessage in " << __FILE__ << __LINE__ << std::endl;
	}
}


void Scene::initializeGL()
{
	if (!format().rgba())  Log.error() << "no rgba mode for OpenGL available." << endl;

	gl_renderer_.init(*stage_);
	gl_renderer_.initSolid();
	if (stage_->getLightSources().size() == 0) setDefaultLighting(false);
	gl_renderer_.updateCamera();
}

void Scene::paintGL()
{
	makeCurrent();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// cannot call update here, because it calls updateGL
	renderView_(DISPLAY_LISTS_RENDERING);
}

void Scene::resizeGL(int width, int height)
{
	makeCurrent();
	gl_renderer_.setSize(width, height);
	gl_renderer_.updateCamera();
	updateGL();
}

		
void Scene::renderView_(RenderMode mode)
	throw()
{
	//abort if GL was not yet initialised
	if (!gl_renderer_.hasStage()) return;

	if (gl_renderer_.isInStereoMode())
	{
		glDrawBuffer(GL_BACK_LEFT);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();

	Vector3 view_point, diff;
	if (gl_renderer_.isInStereoMode())
	{
		view_point = stage_->getCamera().getViewPoint();
		diff = stage_->getCamera().getRightVector();
		diff.normalize();
		diff *= 2;  // half the distance between the eyepoints
		stage_->getCamera().setViewPoint(view_point - diff);
		gl_renderer_.updateCamera();
	}

	renderRepresentations_(mode);
	glPopMatrix();

	if (!gl_renderer_.isInStereoMode())
	{
		return;
	}

  glDrawBuffer(GL_BACK_RIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	stage_->getCamera().setViewPoint(view_point + (diff * 2));
	gl_renderer_.updateCamera();
	renderRepresentations_(DISPLAY_LISTS_RENDERING);
	glPopMatrix();
	stage_->getCamera().setViewPoint(view_point);
}


void Scene::renderRepresentations_(RenderMode mode)
	throw()
{
	PrimitiveManager::RepresentationList::ConstIterator it;

	// render all "normal" (non always front and non transparent models)
	gl_renderer_.initSolid();
	it = getMainControl()->getPrimitiveManager().getRepresentations().begin();
	for(; it != getMainControl()->getPrimitiveManager().getRepresentations().end(); it++)
	{
		if ((*it)->getTransparency() == 0 &&
				!(*it)->hasProperty(Representation::PROPERTY__ALWAYS_FRONT))
		{
			render_(**it, mode);
		}
	}

	// render all transparent models
	gl_renderer_.initTransparent();
	it = getMainControl()->getPrimitiveManager().getRepresentations().begin();
	for(; it != getMainControl()->getPrimitiveManager().getRepresentations().end(); it++)
	{
		if ((*it)->getTransparency() != 0)
		{
			render_(**it, mode);
		}
	}

	// render all always front models
	gl_renderer_.initSolid();
	glDisable(GL_DEPTH_TEST);
	it = getMainControl()->getPrimitiveManager().getRepresentations().begin();
	for(; it != getMainControl()->getPrimitiveManager().getRepresentations().end(); it++)
	{
		if ((*it)->hasProperty(Representation::PROPERTY__ALWAYS_FRONT))
		{
			render_(**it, mode);
		}
	}
	glEnable(GL_DEPTH_TEST);
}


void Scene::render_(const Representation& rep, RenderMode mode)
	throw()
{
	switch (mode)
	{
		case DIRECT_RENDERING:
			gl_renderer_.render_(rep);
			break;

		case DISPLAY_LISTS_RENDERING:
			gl_renderer_.drawFromDisplayList(rep);
			break;

		case REBUILD_DISPLAY_LISTS:
			gl_renderer_.rebuildDisplayListFor(rep);
			break;
	}
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

	float right1 = (gl_renderer_.getWidth()  - x_window_pos_old_ * 2.0) / 
								 gl_renderer_.getWidth() * mouse_sensitivity_ / -ROTATE_FACTOR;
	float up1 = 		(gl_renderer_.getHeight() - y_window_pos_old_ * 2.0) / 
								 gl_renderer_.getHeight() * mouse_sensitivity_ / -ROTATE_FACTOR;
	float right2 = (gl_renderer_.getWidth()  - x_window_pos_new_ * 2.0) / 
								 gl_renderer_.getWidth() * mouse_sensitivity_ / -ROTATE_FACTOR;
	float up2 = 		(gl_renderer_.getHeight() - y_window_pos_new_ * 2.0) / 
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

	int width  = BALL_ABS((int)x_window_pick_pos_second_ - (int)x_window_pick_pos_first_);
	int height = BALL_ABS((int)y_window_pick_pos_second_ - (int)y_window_pick_pos_first_);
	if (width == 0)	width = 1;
	if (height == 0) height = 1;
	gl_renderer_.pickObjects2(objects, width, height);

	GeometricObjectSelectionMessage* message = new GeometricObjectSelectionMessage;
	message->setSelection(objects);
	message->setSelected(select);
	// sent collected objects
	notify_(message);

	renderView_(REBUILD_DISPLAY_LISTS);
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
	gl_renderer_.updateCamera();
	gl_renderer_.setLights();
	light_settings_->updateFromStage();
	updateGL();
}


// ##################MISC############################

void Scene::setDefaultLighting(bool update_GL)
	throw()
{
	List<LightSource>& lights = stage_->getLightSources();
	lights.clear();
	LightSource light;
	light.setPosition(Vector3(0, 0, 1110));
	light.setIntensity(0.6);
	light.setType(LightSource::POSITIONAL);
	stage_->addLightSource(light);
	gl_renderer_.setLights();
	if (update_GL) updateGL();
	light_settings_->update();
	light_settings_->getDefaultLights();
	light_settings_->updateFromStage();
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
	gl_renderer_.buildDisplayListFor(*rp);
	
	// notify GeometricControl
	RepresentationMessage* message = new RepresentationMessage(*rp, RepresentationMessage::ADD);
	notify_(message);
}


void Scene::mouseSensitivity_()
	throw()
{
	MouseSensitivity dialog;
	dialog.exec();
}


void Scene::exportScene(Renderer &er) const
	throw()
{
	PrimitiveManager::RepresentationList::ConstIterator it;
	MainControl *main_control = MainControl::getMainControl(this);

	it = main_control->getPrimitiveManager().getRepresentations().begin();
	for (; it != main_control->getPrimitiveManager().getRepresentations().end(); it++)
	{
		List<GeometricObject*>::ConstIterator it2;
		for (it2 =  (*it)->getGeometricObjects().begin();
				 it2 != (*it)->getGeometricObjects().end();
				 it2++)
		{
			er.render_(*it2);
		}
	}
}

//##########################PREFERENCES#################################

void Scene::writePreferences(INIFile& inifile)
	throw()
{
	ModularWidget::writePreferences(inifile);
	// write mouse sensitivity
	inifile.insertValue("WINDOWS", "Main::mouseSensitivity", String(mouse_sensitivity_));
	inifile.appendSection("STAGE");

	String data = "(" + String((Index)stage_->getBackgroundColor().getRed()) +
								"," + String((Index)stage_->getBackgroundColor().getGreen()) +
								"," + String((Index)stage_->getBackgroundColor().getBlue()) +
								"," + String((Index)stage_->getBackgroundColor().getAlpha()) + ")";

	inifile.insertValue("STAGE", "BackgroundColor", String(data));
	inifile.insertValue("STAGE", "ShowCoordinateSystem", String(stage_->coordinateSystemEnabled()));
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

	// building of coordinate system doesnt work at this point,
	// so no reading of this option from the inifile
	/*
	if (inifile.hasEntry("STAGE", "ShowCoordinateSystem"))
	{
		stage_->showCoordinateSystem(inifile.getValue("STAGE", "ShowCoordinateSystem").toUnsignedInt());
	}
	if (stage_->coordinateSystemEnabled()) createCoordinateSystem_();
	*/
}


void Scene::initializePreferencesTab(Preferences &preferences)
	throw()
{
	light_settings_ = new LightSettings(this);
	preferences.insertTab(light_settings_, "Lighting");
	stage_settings_= new StageSettings(this);
	preferences.insertTab(stage_settings_, "Stage");
}


void Scene::finalizePreferencesTab(Preferences &preferences)
	throw()
{
	if (light_settings_) 
	{
		preferences.removeTab(light_settings_);
		delete light_settings_;
		light_settings_ = 0;
	}
	if (stage_settings_) 
	{
		preferences.removeTab(stage_settings_);
		delete stage_settings_;
		stage_settings_= 0;
	}
}

void Scene::defaultPreferences(Preferences&)
	throw()
{
	if (light_settings_ == 0) return;
	stage_settings_->setDefaultValues();
	light_settings_->setDefaultValues();
}


void Scene::applyPreferences(Preferences & /* preferences */)
	throw()
{
	if (light_settings_ == 0) return;
	
	light_settings_->apply();
	gl_renderer_.setLights();

	bool showed_coordinate = stage_->coordinateSystemEnabled();
	stage_settings_->apply();
	PrimitiveManager& pm = getMainControl()->getPrimitiveManager();

	if (showed_coordinate && !stage_->coordinateSystemEnabled())
	{
		PrimitiveManager::RepresentationsIterator it = pm.begin();
		List<Representation*> reps;
		for (; it != pm.end(); ++it)
		{
			if ((*it)->hasProperty(Representation::PROPERTY__IS_COORDINATE_SYSTEM))
			{
				reps.push_back(*it);
			}
		}

		it = reps.begin();
		for (; it != reps.end(); it++)
		{
			pm.remove(**it);
			RepresentationMessage* message = new RepresentationMessage(**it, RepresentationMessage::REMOVE);
			notify_(message);
		}
	}
	else if (!showed_coordinate && stage_->coordinateSystemEnabled()) 
	{
		createCoordinateSystem_();
	}
	gl_renderer_.updateBackgroundColor(); 
	renderView_(DISPLAY_LISTS_RENDERING);
	updateGL();
}


void Scene::cancelPreferences(Preferences&)
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
	setDefaultLighting(false);
	light_settings_->getDefaultLights();

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
	main_control.insertPopupMenuSeparator(MainControl::DISPLAY);

	stereo_id_ = main_control.insertMenuEntry(
		MainControl::DISPLAY, "&Stereo Mode", this, SLOT(switchStereo()));

	hint = "Change the mouse sensitivity. (It also depends on how large the group is, you focused on";
	main_control.insertMenuEntry(
		MainControl::DISPLAY, "&Mouse Sensitivity", this, SLOT(mouseSensitivity_()), CTRL+Key_M, -1, hint);

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
	getMainControl()->menuBar()->setItemChecked(window_menu_entry_id_, true);
	getMainControl()->menuBar()->setItemChecked(stereo_id_, false);

	setCursor(QCursor(Qt::SizeAllCursor));
}

void Scene::finalizeWidget(MainControl& main_control)
	throw()
{
	main_control.removeMenuEntry(MainControl::DISPLAY, "&Rotate Mode", this, SLOT(rotateMode_()), CTRL+Key_R);
	main_control.removeMenuEntry(MainControl::DISPLAY, "&Picking Mode", this, SLOT(pickingMode_()), CTRL+Key_P);		
	main_control.removeMenuEntry(MainControl::DISPLAY, "&Mouse Sensitivity", this, 
			SLOT(mouseSensitivity_()), CTRL+Key_M);		
	main_control.removeMenuEntry(MainControl::DISPLAY_VIEWPOINT, "Show Vie&wpoint", this, 
																										SLOT(showViewPoint_()), CTRL+Key_W);		
	main_control.removeMenuEntry(MainControl::DISPLAY_VIEWPOINT, "Set Viewpoi&nt", this, 
																										SLOT(setViewPoint_()), CTRL+Key_N);		
	main_control.removeMenuEntry(MainControl::DISPLAY_VIEWPOINT, "Rese&t Camera", this, 
																										SLOT(resetCamera_()), CTRL+Key_T);		
	main_control.removeMenuEntry(MainControl::DISPLAY, "&Stereo Mode", this, SLOT(switchStereo()), CTRL+Key_T);		
	main_control.removeMenuEntry(MainControl::FILE_EXPORT, "PNG", this, SLOT(exportPNG()), ALT+Key_P);		
	main_control.removeMenuEntry(MainControl::WINDOWS, "Scene", this, SLOT(switchShowWidget()));
}

void Scene::checkMenu(MainControl& main_control)
	throw()
{
	(main_control.menuBar())->setItemChecked(rotate_id_, (rotate_mode_));
	(main_control.menuBar())->setItemChecked(picking_id_, (!rotate_mode_));		
}

//##########################EVENTS#################################

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

	x_window_pos_new_ = mouse_event->x();
	y_window_pos_new_ = mouse_event->y();

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

	x_window_pos_old_ = mouse_event->x();
	y_window_pos_old_ = mouse_event->y();

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

	if (need_update_)
	{
		updateGL();
		need_update_ = false;
	}
}

void Scene::rotateMode_()
{
	rotate_mode_ = true;
	setCursor(QCursor(Qt::SizeAllCursor));

	// unpicking mode controls
	NotificationUnregister(events.MouseLeftButtonPressed);
	NotificationUnregister(events.MouseLeftButtonPressed & events.MouseMoved);
	NotificationUnregister(events.MouseLeftButtonReleased);
	NotificationUnregister(events.MouseRightButtonPressed);
	NotificationUnregister(events.MouseRightButtonPressed & events.MouseMoved);
	NotificationUnregister(events.MouseRightButtonReleased);
	
	// rotation mode controls
	NotificationRegister(events.MouseLeftButtonPressed & events.MouseMoved, events.RotateSystem);
	NotificationRegister(events.MouseMiddleButtonPressed & events.MouseMoved, events.ZoomSystem); 
	NotificationRegister(events.MouseRightButtonPressed & events.MouseMoved, events.TranslateSystem);			
}

void Scene::pickingMode_()
{
	rotate_mode_ = false;
	setCursor(QCursor(Qt::CrossCursor));
	
	// unrotation mode controls
	NotificationUnregister(events.MouseLeftButtonPressed & events.MouseMoved); 
	NotificationUnregister(events.MouseMiddleButtonPressed & events.MouseMoved); 
	NotificationUnregister(events.MouseRightButtonPressed & events.MouseMoved); 
	
	// picking mode controls
	NotificationRegister(events.MouseLeftButtonPressed, events.SelectionPressed); 
	NotificationRegister(events.MouseLeftButtonPressed & events.MouseMoved, events.SelectionPressedMoved); 
	NotificationRegister(events.MouseLeftButtonReleased, events.SelectionReleased); 
	NotificationRegister(events.MouseRightButtonPressed, events.DeselectionPressed); 
	NotificationRegister(events.MouseRightButtonPressed & events.MouseMoved, events.DeselectionPressedMoved); 
	NotificationRegister(events.MouseRightButtonReleased, events.DeselectionReleased);
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

void Scene::deselectionReleased_(Scene* /* scene */)
{
	selectObjects_(false);
	need_update_ = true;
	updateGL();
}


void Scene::selectionPressedMoved_(Scene* /* scene */)
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
void Scene::exportPNG()
{
	QPixmap pix = QPixmap::grabWindow(this->winId());
	String filename = String("molview_screenshot" + String(screenshot_nr_) +".png");
	bool result = pix.save(filename.c_str(), "PNG");
	screenshot_nr_ ++;

	if (result) setStatusbarText("Saved screenshot to " + filename);
	else 				setStatusbarText("Could not save screenshot to " + filename);
}

void Scene::customEvent( QCustomEvent * e )
{
	// It must be a SceneUpdateEvent
	if ( e->type() == (QEvent::Type)SCENE_UPDATE_EVENT) 
	{  
		update(true);
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


void Scene::switchStereo()
	throw()
{
	QMenuBar* menu = getMainControl()->menuBar();
	bool stereo;
	if (menu->isItemChecked(stereo_id_))
	{
		stereo = false;
	}
	else
	{
		stereo = true;
	}

	menu->setItemChecked(stereo_id_, stereo);
	gl_renderer_.setStereoMode(stereo);
	update();
}


#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/VIEW/WIDGETS/scene.iC>
#	endif

} }// namespaces
