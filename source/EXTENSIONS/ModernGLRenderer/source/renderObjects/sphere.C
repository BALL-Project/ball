#include <GL/glew.h>

#include <renderObjects/sphere.h>
#include <vbo/VBOPoint.h>
#include <global.h>

#include <BALL/VIEW/MODELS/standardColorProcessor.h>
#include <BALL/VIEW/PRIMITIVES/sphere.h>


Sphere::Sphere(Camera* camera)
	: Geometry(camera)
{
	vbo_ = new VBOPoint;
	shader_name_ = "sphere";
	transparency_init_ = "sphere_transparency_init";
	transparency_peel_ = "sphere_transparency_peel";

	compileShaderPrograms_();

	shaders_.push_back(shader_name_);

	if(Global::getTransparencySupport())
	{
		shaders_.push_back(transparency_init_);
		shaders_.push_back(transparency_peel_);
	}

	initShaderPrograms_();
}

void Sphere::initialize_()
{
	Geometry::initialize_();

	glEnable(GL_POINT_SPRITE);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
}

void Sphere::register_()
{
	camera_->registerViewMatrix(this);
	camera_->registerViewMatrixInverse(this);
	camera_->registerProjectionMatrix(this);
	camera_->registerProjectionMatrixInverse(this);
	camera_->registerViewProjectionMatrixTranspose(this);
	camera_->registerViewport(this);
	camera_->registerClipRange(this);
	material_->registerMaterial(this);
}

void Sphere::unregister_()
{
	camera_->unregisterViewMatrix(this);
	camera_->unregisterViewMatrixInverse(this);
	camera_->unregisterProjectionMatrix(this);
	camera_->unregisterProjectionMatrixInverse(this);
	camera_->unregisterViewProjectionMatrixTranspose(this);
	camera_->unregisterViewport(this);
	camera_->unregisterClipRange(this);
	material_->unregisterMaterial(this);
}

void Sphere::setUniforms_()
{
	setClipable(clipable_);
}

//here starts the rendering
void Sphere::render_()
{
	vbo_->render();
}

void Sphere::compileShaderPrograms_()
{
	std::vector<const char*> shaders;
	shaders.push_back("point_sprite/sphere_vertex.glsl");
	shaders.push_back("point_sprite/sphere_fragment.glsl");
	shaders.push_back("clipping_plane_vertex.glsl");
	shaders.push_back("clipping_plane_fragment.glsl");
	shaders.push_back("opaque/opaque_vertex.glsl");
	shaders.push_back("opaque/opaque_fragment.glsl");
	shaders.push_back("encode_fragment.glsl");

	shader_manager_->createShaderProgram(shader_name_, shaders);

	if(Global::getTransparencySupport())
	{
		std::vector<const char*> shaders;
		shaders.push_back("point_sprite/sphere_vertex.glsl");
		shaders.push_back("point_sprite/sphere_fragment.glsl");
		shaders.push_back("transparent/dual_peeling_init_vertex.glsl");
		shaders.push_back("transparent/dual_peeling_init_fragment.glsl");
		shaders.push_back("clipping_plane_vertex.glsl");
		shaders.push_back("clipping_plane_fragment.glsl");
		shader_manager_->createShaderProgram(transparency_init_, shaders);

		std::vector<const char*> shaders2;
		shaders2.push_back("point_sprite/sphere_vertex.glsl");
		shaders2.push_back("point_sprite/sphere_fragment.glsl");
		shaders2.push_back("compositing/blinn_phong_fragment.glsl");
		shaders2.push_back("transparent/dual_peeling_peel_vertex.glsl");
		shaders2.push_back("transparent/dual_peeling_peel_fragment.glsl");
		shaders2.push_back("clipping_plane_vertex.glsl");
		shaders2.push_back("clipping_plane_fragment.glsl");
		shader_manager_->createShaderProgram(transparency_peel_, shaders2);
	}
}

void Sphere::initShaderPrograms_()
{
	midpoint_idx = 0;
	color_idx    = 1;
	radius_idx   = 2;

	//iterate over all shaders and set the attribute locations
	QVector<const char*>::iterator i = shaders_.begin();

	while (i != shaders_.end())
	{
		shader_manager_->getShader(*i)->setAttributeLocation(midpoint_idx, "midpoint");
		shader_manager_->getShader(*i)->setAttributeLocation(color_idx   , "color");
		shader_manager_->getShader(*i)->setAttributeLocation(radius_idx  , "radius");
		++i;
	}
}

void Sphere::initVBO_ ( std::list< const BALL::VIEW::GeometricObject* >& list )
{

	size_t num_elements = list.size();

	std::vector<Attrib3> verts(num_elements);
	std::vector<Attrib1> radius(num_elements);
	std::vector<Attrib4> color(num_elements);

	int ac = 0;
	std::list<const BALL::VIEW::GeometricObject*>::const_iterator it;

	for (it =  list.begin(); it != list.end(); it++, ++ac)
	{
		const BALL::VIEW::Sphere* sphere = dynamic_cast<const BALL::VIEW::Sphere*>(*it);

		if(sphere)
		{
			BALL::VIEW::ElementColorProcessor ecp;
			BALL::VIEW::ColorRGBA col = sphere->getColor();

			BALL::Vector3 p = sphere->getPosition()*Global::getScaleFactor();

			verts[ac].a = p.x;
			verts[ac].b = p.y;
			verts[ac].c = p.z;
			radius[ac].a = sphere->getRadius() * Global::getScaleFactor();
			color[ac].a = (float) col.getRed();
			color[ac].b = (float) col.getGreen();
			color[ac].c = (float) col.getBlue();
			color[ac].d = (float) col.getAlpha();
		}
	}

	vbo_->setNumberOfElements(num_elements);
	vbo_->setAttribute3f(midpoint_idx,verts);
	vbo_->setAttribute1f(radius_idx,radius);
	vbo_->setAttribute4f(color_idx,color);
}