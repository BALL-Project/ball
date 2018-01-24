#include <GL/glew.h>

#include <modernGLRenderer.h>
#include <global.h>

#include <renderObjects/box.h>
#include <renderObjects/line.h>
#include <renderObjects/mesh.h>
#include <renderObjects/point.h>
#include <renderObjects/pointVolume.h>
#include <renderObjects/twoColoredLine.h>

#include <BALL/VIEW/PRIMITIVES/gridVisualisation.h>
#include <BALL/VIEW/PRIMITIVES/sphere.h>
#include <BALL/VIEW/PRIMITIVES/twoColoredTube.h>
#include <BALL/VIEW/PRIMITIVES/tube.h>
#include <BALL/VIEW/PRIMITIVES/twoColoredLine.h>
#include <BALL/VIEW/PRIMITIVES/point.h>
#include <BALL/VIEW/PRIMITIVES/disc.h>
#include <BALL/VIEW/PRIMITIVES/line.h>
#include <BALL/VIEW/PRIMITIVES/mesh.h>
#include <BALL/VIEW/PRIMITIVES/box.h>
#include <BALL/VIEW/PRIMITIVES/label.h>
#include <BALL/VIEW/PRIMITIVES/simpleBox.h>
#include <BALL/VIEW/PRIMITIVES/multiLine.h>
#include <BALL/VIEW/PRIMITIVES/quadMesh.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/SYSTEM/path.h>

#include <BALL/VIEW/KERNEL/mainControl.h>


//TODO remove local copies of variables. Store it in camera object

ModernGLRenderer::ModernGLRenderer()
	: BALL::VIEW::Renderer("ModernGLRenderer"),
	  initialized_(false),
	  opaque_(0),
	  transparent_(0),
	  ssao_(0),
	  volume_(0),
	  clipping_plane_(0),
	  light_sources_(0),
	  camera_(0),
	  volume_rep_(0),
	  useAO_(false),
	  has_transparency_(false)
{

	BALL::Path path = BALL::Path();
	BALL::String p = path.find("ModernGLRenderer/shader/");

	shader_manager_ = &ShaderManager::instance();
	shader_manager_->initialize(p.c_str());

	camera_        = new Camera;
}

ModernGLRenderer::~ModernGLRenderer()
{
	delete opaque_;
	delete transparent_;
	delete light_sources_;
	delete volume_;
	delete ssao_;
	delete camera_;
	delete clipping_plane_;
}

bool ModernGLRenderer::init(const BALL::VIEW::Stage& stage, float width, float height)
{
	Renderer::init(stage, width, height);

	if(initialized_)
	{
		return true;
	}

	GLenum result = glewInit();

	if(result != GLEW_OK)
	{
		BALL::Log.error() << "[ModernGLRenderer]: Could not initialize GLEW: " << glewGetErrorString(result) << std::endl;
		return false;
	}

	if(!GLEW_VERSION_2_1)
	{
		BALL::Log.error() << "[ModernGLRenderer] No OpenGL 2.1 support" << std::endl;
		return false;
	}

	BALL::Log.info() << "[ModernGLRenderer] INFO: OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	BALL::Log.info() << "[ModernGLRenderer] INFO: GLEW version: " << glewGetString(GLEW_VERSION) << std::endl;

	Global::setTransparencySupport(checkTransparencySupport_());
	BALL::Log.info() << "[ModernGLRenderer] INFO: Transparency supported: " << (Global::getTransparencySupport() ? "true" : "false") << std::endl;

	glEnable(GL_DEPTH_TEST);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	volume_ = new Volume(camera_);
	ssao_   = new SSAO(camera_);

	opaque_ = new Opaque(camera_);
	opaque_->init();
	opaque_->setupRenderTargets(width, height);
	opaque_->setAO(ssao_->defaultSSAOParameters.useAO);

	light_sources_ = new LightSources(camera_);
	clipping_plane_= new ClippingPlane(camera_);

	ssao_->init();

	if(Global::getTransparencySupport())
	{
		transparent_ = new Transparent(camera_);
		transparent_->init();
		transparent_->setupRenderTargets(width, height);
		transparent_->setAO(ssao_->defaultSSAOParameters.useAO);
	}

	//set textures
	ssao_->setNormalTexture(opaque_->getNormalTexture());
	ssao_->setDepthTexture(opaque_->getDepthTexture());
	opaque_->setSSAOTexture(ssao_->getSSAOTexture());

	if(Global::getTransparencySupport())
	{
		transparent_->setSSAOTexture(ssao_->getSSAOTexture());
		transparent_->setNormalTexture(opaque_->getNormalTexture());
	}

	connectSignalsAndSlots_();

	objects_.clear();

	//be sure that all data is emited correctly
	camera_->emitAll();


	// set the background color according to the stage
	updateBackgroundColor();

	initialized_ = true;

	emit(initialized());

	return true;
}

bool ModernGLRenderer::checkTransparencySupport_()
{
	std::string missing_extensions;

	if(!glewIsSupported("GL_ARB_texture_rectangle"))
	{
		missing_extensions += "\tGL_ARB_texture_rectangle\n";
	}

	if(!glewIsSupported("GL_ARB_texture_float") && !glewIsSupported("GL_NV_float_buffer"))
	{
		missing_extensions += "\tGL_ARB_texture_float or GL_NV_float_buffer\n";
	}

	if(!glewIsSupported("GL_NV_depth_buffer_float") && !glewIsSupported("GL_ARB_depth_buffer_float"))
	{
		missing_extensions += "\tGL_NV_depth_buffer_float or GL_ARB_depth_buffer_float\n";
	}

	if(!missing_extensions.empty())
	{
		BALL::Log.error()
		    << "[ModernGLRenderer] Unable to load the necessary extensions to provide transparency rendering:\n"
		    << missing_extensions
		    << "Support for transparent objects will be disabled."
		    << std::endl;

		return false;
	}

	return true;
}

void ModernGLRenderer::connectSignalsAndSlots_()
{
	connect(this, SIGNAL(useAOChanged(bool)),
	        opaque_, SLOT(setAO(bool)));

	if(Global::getTransparencySupport())
	{
		connect(this, SIGNAL(useAOChanged(bool)),
		        transparent_, SLOT(setAO(bool)));
	}
}

void ModernGLRenderer::setView(const Eigen::Affine3f& view_matrix)
{
	camera_->setViewMatrix(view_matrix);
}

void ModernGLRenderer::setProjection(ProjectionMatrixType matrix_type)
{
	camera_->createProjectionMatrix(matrix_type);
}

void ModernGLRenderer::setSize(float width, float height)
{
	camera_->setViewport(width, height);
}

void ModernGLRenderer::setNearDist ( float near_dist )
{
	camera_->setNearDistance(near_dist);
}

void ModernGLRenderer::setFarDist ( float far_dist )
{
	camera_->setFarDistance(far_dist);
}

float ModernGLRenderer::getNearDist()
{
	return camera_->getNearDistance();
}

float ModernGLRenderer::getFarDist()
{
	return camera_->getFarDistance();
}

void ModernGLRenderer::setZoom ( float zoom )
{
	camera_->setZoom(zoom);
}

void ModernGLRenderer::getFrustum ( float& near_f, float& far_f, float& left_f, float& right_f, float& top_f, float& bottom_f )
{
	camera_->getFrustum(near_f, far_f, left_f, right_f, top_f, bottom_f);
}

void ModernGLRenderer::bufferRepresentation(const BALL::VIEW::Representation& rep)
{
	if (rep.getGeometricObjects().empty())
		return;

	if(rep.needsUpdate())
	{
		removeRepresentation(rep);
	}

	if(rep.isHidden())
	{
		return;
	}

	if (objects_.has(&rep))
	{
		// was the representation previously disabled and now just needs enabling?
		if (objects_[&rep].isDisabled)
		{
			// enable representation
			objects_[&rep].isDisabled = false;
			return;
		}
	}

	RenderObjectList data;
	data.isDisabled = false;


	std::list<BALL::VIEW::GeometricObject const *> sphere_list;
	std::list<BALL::VIEW::GeometricObject const *> two_colored_tube_list;
	std::list<BALL::VIEW::GeometricObject const *> mesh_list;
	std::list<BALL::VIEW::GeometricObject const *> two_colored_line_list;
	std::list<BALL::VIEW::GeometricObject const *> point_list;
	std::list<BALL::VIEW::GeometricObject const *> line_list;
	std::list<BALL::VIEW::GeometricObject const *> point_volume_list;
	std::list<BALL::VIEW::GeometricObject const *> tube_list;
	std::list<BALL::VIEW::GeometricObject const *> box_list;
	std::list<BALL::VIEW::GeometricObject const *> grid_visualisation_list;

	std::list<BALL::VIEW::GeometricObject*>::const_iterator it;

	for (it =  rep.getGeometricObjects().begin();
	     it != rep.getGeometricObjects().end();
	     it++)
	{
		if (BALL::RTTI::isKindOf<BALL::VIEW::Sphere>(*it))
		{
			sphere_list.push_back(*it);
		}
		else if (BALL::RTTI::isKindOf<BALL::VIEW::TwoColoredTube>(*it))
		{
			two_colored_tube_list.push_back(*it);
		}
		else if (BALL::RTTI::isKindOf<BALL::VIEW::Mesh>(*it))
		{
			mesh_list.push_back(*it);
		}
		else if (BALL::RTTI::isKindOf<BALL::VIEW::TwoColoredLine>(*it))
		{
			two_colored_line_list.push_back(*it);
		}
		else if (BALL::RTTI::isKindOf<BALL::VIEW::Point>(*it))
		{
			point_list.push_back(*it);
		}
		else if (BALL::RTTI::isKindOf<BALL::VIEW::Disc>(*it))
		{
			//NOTE Disc do not need to be rendered. Cylinder all have caps.
		}
		else if (BALL::RTTI::isKindOf<BALL::VIEW::Line>(*it))
		{
			line_list.push_back(*it);
		}
		else if (BALL::RTTI::isKindOf<BALL::VIEW::Tube>(*it))
		{
			tube_list.push_back(*it);
		}
		else if (BALL::RTTI::isKindOf<BALL::VIEW::Box>(*it))
		{
			box_list.push_back(*it);
		}
		else if (BALL::RTTI::isKindOf<BALL::VIEW::SimpleBox>(*it))
		{
			BALL::Log.error() << (BALL::String)(qApp->translate("ModernGLRenderer", "Rendering of SimpleBox not supported." )) << std::endl;
		}
		else if (BALL::RTTI::isKindOf<BALL::VIEW::Label>(*it))
		{
			BALL::Log.error() << (BALL::String)(qApp->translate("ModernGLRenderer", "Rendering of Label not supported." )) << std::endl;
		}
		else if (BALL::RTTI::isKindOf<BALL::VIEW::MultiLine>(*it))
		{
			BALL::Log.error() << (BALL::String)(qApp->translate("ModernGLRenderer", "Rendering of Mulitline not supported." )) << std::endl;
		}
		else if (BALL::RTTI::isKindOf<BALL::VIEW::GridVisualisation>(*it))
		{

			const BALL::VIEW::GridVisualisation* vol = dynamic_cast<const BALL::VIEW::GridVisualisation*>(*it);

			// create a box around the grid visualisation if needed
			if (vol->draw_box)
			{
				BALL::VIEW::Box* box = new BALL::VIEW::Box(vol->origin, vol->x, vol->y, vol->z);
				BALL::VIEW::ColorRGBA bgi(stage_->getBackgroundColor().getInverseColor());
				box->setColor(bgi);
				box_list.push_back(box);
			}

			if(vol->type == BALL::VIEW::GridVisualisation::DOTS)
			{
				point_volume_list.push_back(*it);
			}
			else if (vol->type == BALL::VIEW::GridVisualisation::PLANE)
			{
				BALL::Log.error() << (BALL::String)(qApp->translate("ModernGLRenderer", "Rendering of Volume with plane not supported." )) << std::endl;
			}
			else
			{
				grid_visualisation_list.push_back(*it);
			}

		}
		else if (BALL::RTTI::isKindOf<BALL::VIEW::QuadMesh>(*it))
		{
			BALL::Log.error() << (BALL::String)(qApp->translate("ModernGLRenderer", "Rendering of QuadMesh not supported." )) << std::endl;
		}
		// ... add more types of GeometricObjects here
		else
		{
			// unknown type of GeometricObject
			BALL::Log.error() << (BALL::String)(qApp->translate("ModernGLRenderer", "unknown type of GeometricObject in" )) + " ModernGLRenderer::bufferRepresentation: "
			                  << typeid(*it).name() << "  " << *it << std::endl;
			continue;
		}
	}

	if(sphere_list.size() > 0)
	{
		Sphere* sphere = new Sphere(camera_);
		sphere->init(sphere_list);
		data.render_objects.push_back(sphere);
	}

	if(two_colored_tube_list.size() > 0)
	{
		Cylinder* two_colored_cylinder = new Cylinder(camera_);
		two_colored_cylinder->init(two_colored_tube_list);
		data.render_objects.push_back(two_colored_cylinder);
	}

	if(mesh_list.size() > 0)
	{
		Mesh* mesh = new Mesh(camera_);
		mesh->setDrawingMode(rep.getDrawingMode());
		mesh->init(mesh_list);
		data.render_objects.push_back(mesh);
	}

	if(line_list.size() > 0)
	{
		Line* line = new Line(camera_);
		line->init(line_list);
		data.render_objects.push_back(line);
	}

	if(two_colored_line_list.size() > 0)
	{
		TwoColoredLine* two_colored_line = new TwoColoredLine(camera_);
		two_colored_line->init(two_colored_line_list);
		data.render_objects.push_back(two_colored_line);
	}

	if(point_list.size() > 0)
	{
		Point* point = new Point(camera_);
		point->init(point_list);
		data.render_objects.push_back(point);
	}

	if(point_volume_list.size() > 0)
	{
		PointVolume* point_volume = new PointVolume(camera_);
		point_volume->init(point_volume_list);
		data.render_objects.push_back(point_volume);
	}

	if(tube_list.size() > 0)
	{
		Cylinder* cylinder = new Cylinder(camera_);
		cylinder->init(tube_list);
		data.render_objects.push_back(cylinder);
	}

	if(box_list.size() > 0)
	{
		Box* box = new Box(camera_);
		box->init(box_list);
		data.render_objects.push_back(box);
	}

	if(grid_visualisation_list.size() > 0 )
	{
//    Volume* volume = new Volume(camera_);
//    volume->init(grid_visualisation_list);
//    data.render_objects.push_back(volume);

		volume_rep_ = &rep;
		volume_->init(grid_visualisation_list);
		opaque_->setVolumeTexture(volume_->getVolumeTexture());

		if(Global::getTransparencySupport())
		{
			transparent_->setVolumeTexture(volume_->getVolumeTexture());
		}

		volume_->setNormalTexture(opaque_->getNormalTexture());
		volume_->setDepthTexture(opaque_->getDepthTexture());
	}

	objects_[&rep] = data;

	updateHasTransparency_();

	updateMaterialFromStage();

	//be sure that all RenderObjects are up to date (e.g. viewmatrix)
	camera_->emitAll();
}

void ModernGLRenderer::removeRepresentation(BALL::VIEW::Representation const& rep)
{
	if (objects_.has(&rep))
	{
		RenderObjectList obj = objects_[&rep];

		obj.clear();

		objects_.erase(&rep);
	}

	if(volume_rep_ == &rep)
	{
		std::list<BALL::VIEW::GeometricObject const *> grid_visualisation_list;
		//cleanup the current volume
		volume_->init(grid_visualisation_list);
		volume_rep_ = 0;
	}
}


void ModernGLRenderer::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//NOTE we need this here because BALL does not notify when representation will be hidden
	//if it would, call updateHasTransparency_ only when notified and in bufferRepresentation()
	updateHasTransparency_();

	//opaque rendering is used for ssao and volume rendering
	opaque_->prepare_rendering();
	renderGeometry_(OPAQUE);

	renderEffects_();

	if(Global::getTransparencySupport() && has_transparency_)
	{
		renderTransparent_();
	}
	else
	{
		renderOpaque_();
	}

	shader_manager_->release();
}

void ModernGLRenderer::renderEffects_()
{
	if(useAO_)
	{
		ssao_->prepare_rendering();
		ssao_->render();
	}

	if(volume_rep_ != 0 && !volume_rep_->isHidden())
	{
		opaque_->setVolume(true);

		if(Global::getTransparencySupport())
		{
			transparent_->setVolume(true);
		}

		volume_->prepare_rendering();
		volume_->render();
	}
	else
	{
		opaque_->setVolume(false);

		if(Global::getTransparencySupport())
		{
			transparent_->setVolume(false);
		}
	}
}

void ModernGLRenderer::renderOpaque_()
{
	opaque_->render();
}

void ModernGLRenderer::renderTransparent_()
{

	transparent_->prepare_rendering(TRANSPARENCY_INIT);
	renderGeometry_(TRANSPARENCY_INIT);
	transparent_->prepare_rendering(TRANSPARENCY_PEEL);

	bool useOQ = transparent_->getTransparentParameters().useOQ;
	int numPasses = transparent_->getTransparentParameters().numPasses;

	for(int pass = 1; useOQ || pass <= numPasses; pass++)
	{
		transparent_->render(pass, TRANSPARENCY_PEEL);
		setDepthTex_(transparent_->getPrevDepthTex());
		setFrontBlenderTex_(transparent_->getPrevFrontBlenderTex());
		renderGeometry_(TRANSPARENCY_PEEL);

		if(!transparent_->render(pass, TRANSPARENCY_BLEND))
		{
			break;
		}
	}

	transparent_->prepare_rendering(TRANSPARENCY_FINAL);
	transparent_->render(TRANSPARENCY_FINAL);

}

void ModernGLRenderer::renderRenderObjects_(RenderLevel level)
{

	BALL::HashMap<BALL::VIEW::Representation const*, RenderObjectList >::Iterator it;

	for(it = objects_.begin(); it !=objects_.end(); it++)
	{
		std::list<RenderObject*> ob =  it->second.render_objects;

		// do not render representation because it it hidden
		if(it->first->isHidden())
		{
			continue;
		}

		for(std::list<RenderObject*>::iterator it2 = ob.begin(); it2 != ob.end(); it2++)
		{
			(*&*it2)->render(level);
		}
	}
}

void ModernGLRenderer::renderGeometry_(RenderLevel level)
{
	renderRenderObjects_(level);
	clipping_plane_->render(level);

	if(show_light_sources_)
	{
		light_sources_->render(level);
	}
}

void ModernGLRenderer::setDepthTex_(GLuint tex_id)
{

	BALL::HashMap<BALL::VIEW::Representation const*, RenderObjectList >::Iterator it;

	for(it = objects_.begin(); it !=objects_.end(); it++)
	{
		std::list<RenderObject*> ob =  it->second.render_objects;

		for(std::list<RenderObject*>::iterator it2 = ob.begin(); it2 != ob.end(); it2++)
		{
			(*&*it2)->setDepthBlenderTex(tex_id);
		}
	}

	clipping_plane_->setDepthBlenderTex(tex_id);
	light_sources_->setDepthBlenderTex(tex_id);
}

void ModernGLRenderer::setFrontBlenderTex_(GLuint tex_id)
{

	BALL::HashMap<BALL::VIEW::Representation const*, RenderObjectList >::Iterator it;

	for(it = objects_.begin(); it !=objects_.end(); it++)
	{
		std::list<RenderObject*> ob =  it->second.render_objects;

		for(std::list<RenderObject*>::iterator it2 = ob.begin(); it2 != ob.end(); it2++)
		{
			(*&*it2)->setFrontBlenderTex(tex_id);
		}
	}

	clipping_plane_->setFrontBlenderTex(tex_id);
	light_sources_->setFrontBlenderTex(tex_id);
}

void ModernGLRenderer::renderToBuffer ( BALL::VIEW::RenderTarget* /*renderTarget*/ )
{
	std::vector<BALL::VIEW::ClippingPlane*> active_planes;
	std::vector<BALL::VIEW::ClippingPlane*> inactive_planes;

	getClippingPlanes_(&active_planes, &inactive_planes);

	updateClippingPlanes_(inactive_planes);

	setActiveClippingPlanes_(active_planes);

	render();
}

void ModernGLRenderer::setActiveClippingPlanes_( std::vector<BALL::VIEW::ClippingPlane*>& active_planes )
{

	// clear all clipping planes
	BALL::HashMap<BALL::VIEW::Representation const*, RenderObjectList >::Iterator it;

	for(it = objects_.begin(); it !=objects_.end(); it++)
	{
		it->second.clearClippingPlanes();
	}

	std::vector<BALL::VIEW::ClippingPlane*>::const_iterator plane_it = active_planes.begin();

	for (; plane_it != active_planes.end(); plane_it++)
	{

		BALL::VIEW::ClippingPlane& plane = **plane_it;

		const BALL::Vector3& n(plane.getNormal());
		const Eigen::Vector4f planef(n.x, n.y, n.z, plane.getDistance()*Global::getScaleFactor());

		BALL::HashSet<const BALL::VIEW::Representation*> rep = plane.getRepresentations();
		BALL::HashSet<const BALL::VIEW::Representation*>::Iterator it;

		for(it = rep.begin(); it != rep.end(); it++)
		{
			const BALL::VIEW::Representation* r = *&*it;

			if(objects_.has(r))
			{
				objects_[r].addClippingPlane(planef);
			}
		}
	}
}

void ModernGLRenderer::getClippingPlanes_(std::vector<BALL::VIEW::ClippingPlane*>* active_planes, std::vector<BALL::VIEW::ClippingPlane*>* inactive_planes )
{

	BALL::VIEW::RepresentationManager& pm = BALL::VIEW::getMainControl()->getRepresentationManager();

	bool move_mode = scene_->inMoveMode();

	//iterate over clipping planes and set them for the representations
	const std::vector<BALL::VIEW::ClippingPlane*>& vc = pm.getClippingPlanes();
	std::vector<BALL::VIEW::ClippingPlane*>::const_iterator plane_it = vc.begin();

	for (; plane_it != vc.end(); plane_it++)
	{
		BALL::VIEW::ClippingPlane& plane = **plane_it;

		if (plane.isHidden()) continue;

		if (!plane.isActive())
		{
			inactive_planes->push_back(*plane_it);

			if (!move_mode) continue;
		}

		active_planes->push_back(*plane_it);
	}
}

void ModernGLRenderer::updateClippingPlanes_(std::vector<BALL::VIEW::ClippingPlane*>& inactive_planes)
{
	clipping_plane_->init(inactive_planes);
}

void ModernGLRenderer::updateBackgroundColor()
{
	float r = (float) stage_->getBackgroundColor().getRed();
	float g = (float) stage_->getBackgroundColor().getGreen();
	float b = (float) stage_->getBackgroundColor().getBlue();
	float a = (float) stage_->getBackgroundColor().getAlpha();
	glClearColor(r,g,b,0.);
	opaque_->setClearColor(r,g,b,a);

	if(Global::getTransparencySupport())
	{
		if(transparent_ != 0 && transparent_->isInitialized())
		{
			transparent_->setClearColor(r,g,b,a);
		}
	}
}

boost::shared_ptr<BALL::VIEW::RenderSetup> ModernGLRenderer::createRenderSetup(BALL::VIEW::RenderTarget* target, BALL::VIEW::Scene* scene)
{
	return boost::shared_ptr<BALL::VIEW::RenderSetup>(new ModernGLRenderSetup(this, target, scene));
}

void ModernGLRenderer::updateCamera(const BALL::VIEW::Camera* camera)
{
	camera_->createViewMatrix(camera);

	if(camera->getProjectionMode() == BALL::VIEW::Camera::PERSPECTIVE)
	{
		camera_->createProjectionMatrix(PERSPECTIVE);
	}
	else if (camera->getProjectionMode() == BALL::VIEW::Camera::ORTHOGRAPHIC)
	{
		camera_->createProjectionMatrix(ORTHOGRAPHIC);
	}

	setLights();
}

void ModernGLRenderer::setLights ( bool reset_all )
{
	opaque_->setLights(stage_, reset_all);
	clipping_plane_->setLights(stage_, reset_all);
	light_sources_->setLights(stage_, reset_all);

	if(Global::getTransparencySupport())
	{

		BALL::HashMap<BALL::VIEW::Representation const*, RenderObjectList >::Iterator it;

		for(it = objects_.begin(); it !=objects_.end(); it++)
		{
			std::list<RenderObject*> ob =  it->second.render_objects;

			for(std::list<RenderObject*>::iterator it2 = ob.begin(); it2 != ob.end(); it2++)
			{
				(*&*it2)->setLights(stage_, reset_all);
			}
		}
	}

	light_sources_->init(stage_);
}

void ModernGLRenderer::updateMaterialForRepresentation(const BALL::VIEW::Representation* )
{
	updateMaterialFromStage();
}

void ModernGLRenderer::updateMaterialFromStage()
{
	BALL::VIEW::Stage::Material const& rt_material = stage_->getMaterial();

	Material* material = &Material::instance();

	// ambience

	float red   = (float)rt_material.ambient_color.getRed()   * rt_material.ambient_intensity;
	float green = (float)rt_material.ambient_color.getGreen() * rt_material.ambient_intensity;
	float blue  = (float)rt_material.ambient_color.getBlue()  * rt_material.ambient_intensity;
	float alpha = (float)rt_material.ambient_color.getAlpha() * rt_material.ambient_intensity;

	material->setMaterialAmbient(red, green, blue, alpha);

	// specularity
	red   = (float)rt_material.specular_color.getRed()   * rt_material.specular_intensity;
	green = (float)rt_material.specular_color.getGreen() * rt_material.specular_intensity;
	blue  = (float)rt_material.specular_color.getBlue()  * rt_material.specular_intensity;
	alpha = (float)rt_material.specular_color.getAlpha() * rt_material.specular_intensity;

	material->setMaterialSpecular(red, green, blue, alpha);

	// diffuse
	//  red   = (float)rt_material.reflective_color.getRed()   * rt_material.reflective_intensity;
	//  green = (float)rt_material.reflective_color.getGreen() * rt_material.reflective_intensity;
	//  blue  = (float)rt_material.reflective_color.getBlue()  * rt_material.reflective_intensity;
	//  alpha = (float)rt_material.reflective_color.getAlpha() * rt_material.reflective_intensity;
	//
	//  emit materialDiffuseChanged(red, green, blue, alpha);

	material->setMaterialDiffuse(.8f,  .8f,  .8f, 1.0f);

	//TODO there is no diffuse material!!!!

	//shininess
	float shininess = (float)rt_material.shininess;

	material->setMaterialShininess(shininess);

	//float transparency = (float) (100.f - rt_material.transparency) * 0.01f;
	//  emit materialTransparencyChanged(transparency);
}

void ModernGLRenderer::ssaoParametersChanged(SSAOParameters ssaoParameters)
{
	if(ssao_ != 0)
	{
		if(ssao_->isInitialized())
		{
			useAO_ = ssaoParameters.useAO;
			emit useAOChanged(useAO_);

			ssao_->setSSAOParameters(ssaoParameters);

		}
		else
		{
			ssao_->defaultSSAOParameters = ssaoParameters;
		}
	}
}

void ModernGLRenderer::transparentParametersChanged(TransparentParameters tp)
{
	if(Global::getTransparencySupport())
	{
		if(transparent_ != 0 && transparent_->isInitialized())
		{
			transparent_->setTransparentParameters(tp);
		}
	}
}

bool ModernGLRenderer::isInitialized() const
{
	return initialized_;
}

void ModernGLRenderer::updateHasTransparency_()
{
	has_transparency_ = false;

	BALL::HashMap<BALL::VIEW::Representation const*, RenderObjectList >::Iterator it;

	for(it = objects_.begin(); it !=objects_.end(); it++)
	{
		std::list<RenderObject*> ob =  it->second.render_objects;

		// do not check representation because it it hidden
		if(it->first->isHidden())
		{
			continue;
		}

		if(it->first->getTransparency() != 0)
		{
			has_transparency_ = true;
			return;
		}
	}

	if(clipping_plane_->getNumberOfClippingPlanes())
	{
		has_transparency_ = true;
	}
}
