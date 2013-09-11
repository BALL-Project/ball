#include <GL/glew.h>

#include <renderObjects/Cylinder.h>
#include <Global.h>

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/STRUCTURE/geometricProperties.h>

#include <BALL/VIEW/PRIMITIVES/tube.h>
#include <BALL/VIEW/PRIMITIVES/twoColoredTube.h>


Cylinder::Cylinder(Camera* camera)
	: Geometry(camera)
{
	vbo_ = new VBOPoint;
	shader_name_ = "cylinder";
	transparency_init_ = "cylinder_transparency_init";
	transparency_peel_ = "cylinder_transparency_peel";

	compileShaderPrograms_();

	shaders_.push_back(shader_name_);

	if(Global::getTransparencySupport())
	{
		shaders_.push_back(transparency_init_);
		shaders_.push_back(transparency_peel_);
	}

	initShaderPrograms_();
}

void Cylinder::initialize_()
{
	Geometry::initialize_();

	glEnable(GL_POINT_SPRITE);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
}

void Cylinder::register_()
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

void Cylinder::unregister_()
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

void Cylinder::setUniforms_()
{
	setClipable(clipable_);
}

//here starts the rendering
void Cylinder::render_()
{
	vbo_->render();
}

void Cylinder::compileShaderPrograms_()
{
	std::vector<const char*> shaders;
	shaders.push_back("point_sprite/cylinder_vertex.glsl");
	shaders.push_back("point_sprite/cylinder_fragment.glsl");
	shaders.push_back("clipping_plane_vertex.glsl");
	shaders.push_back("clipping_plane_fragment.glsl");
	shaders.push_back("opaque/opaque_vertex.glsl");
	shaders.push_back("opaque/opaque_fragment.glsl");
	shaders.push_back("encode_fragment.glsl");

	shader_manager_->createShaderProgram(shader_name_, shaders);

	if(Global::getTransparencySupport())
	{
		std::vector<const char*> shaders;
		shaders.push_back("point_sprite/cylinder_vertex.glsl");
		shaders.push_back("point_sprite/cylinder_fragment.glsl");
		shaders.push_back("transparent/dual_peeling_init_vertex.glsl");
		shaders.push_back("transparent/dual_peeling_init_fragment.glsl");
		shaders.push_back("clipping_plane_vertex.glsl");
		shaders.push_back("clipping_plane_fragment.glsl");
		shader_manager_->createShaderProgram(transparency_init_, shaders);

		std::vector<const char*> shaders2;
		shaders2.push_back("point_sprite/cylinder_vertex.glsl");
		shaders2.push_back("point_sprite/cylinder_fragment.glsl");
		shaders2.push_back("compositing/blinn_phong_fragment.glsl");
		shaders2.push_back("transparent/dual_peeling_peel_vertex.glsl");
		shaders2.push_back("transparent/dual_peeling_peel_fragment.glsl");
		shaders2.push_back("clipping_plane_vertex.glsl");
		shaders2.push_back("clipping_plane_fragment.glsl");
		shader_manager_->createShaderProgram(transparency_peel_, shaders2);
	}
}

void Cylinder::initShaderPrograms_()
{
	midpoint_idx    = 0;
	direction_idx   = 1;
	color1_idx      = 2;
	color2_idx      = 3;
	radius_idx      = 4;
	color_ratio_idx = 5;

	//iterate over all shaders and set the attribute locations
	QVector<const char*>::iterator i = shaders_.begin();

	while (i != shaders_.end())
	{
		shader_manager_->getShader(*i)->setAttributeLocation(midpoint_idx   , "midpoint");
		shader_manager_->getShader(*i)->setAttributeLocation(direction_idx  , "direction");
		shader_manager_->getShader(*i)->setAttributeLocation(color1_idx     , "color1");
		shader_manager_->getShader(*i)->setAttributeLocation(color2_idx     , "color2");
		shader_manager_->getShader(*i)->setAttributeLocation(radius_idx     , "radius");
		shader_manager_->getShader(*i)->setAttributeLocation(color_ratio_idx, "color_ratio");
		++i;
	}
}

void Cylinder::initVBO_ ( std::list< const BALL::VIEW::GeometricObject* >& list )
{

	size_t num_elements = list.size();

	std::vector<Attrib3> midpoint(num_elements);
	std::vector<Attrib3> direction(num_elements);
	std::vector<Attrib4> color1(num_elements);
	std::vector<Attrib4> color2(num_elements);
	std::vector<Attrib1> radius(num_elements);
	std::vector<Attrib1> color_ratio(num_elements);

	std::list<const BALL::VIEW::GeometricObject*>::const_iterator it;
	int ac = 0;

	for (it =  list.begin(); it != list.end(); it++, ac++)
	{
		const BALL::VIEW::TwoColoredTube* two_col_tube = dynamic_cast<const BALL::VIEW::TwoColoredTube*>(*it);

		if(two_col_tube)
		{

			BALL::Vector3 d = ((two_col_tube->getVertex1() - two_col_tube->getVertex2()) * 0.5f) * Global::getScaleFactor();
			BALL::Vector3 p = ((two_col_tube->getVertex2() + two_col_tube->getVertex1()) * 0.5f) * Global::getScaleFactor();

			BALL::VIEW::ColorRGBA const& col1 = two_col_tube->getColor();
			BALL::VIEW::ColorRGBA const& col2 = two_col_tube->getColor2();

			midpoint[ac].a  = p.x;
			midpoint[ac].b  = p.y;
			midpoint[ac].c  = p.z;
			direction[ac].a = d.x;
			direction[ac].b = d.y;
			direction[ac].c = d.z;
			color1[ac].a = (float) col1.getRed();
			color1[ac].b = (float) col1.getGreen();
			color1[ac].c = (float) col1.getBlue();
			color1[ac].d = (float) col1.getAlpha();
			color2[ac].a = (float) col2.getRed();
			color2[ac].b = (float) col2.getGreen();
			color2[ac].c = (float) col2.getBlue();
			color2[ac].d = (float) col2.getAlpha();
			radius[ac].a  = two_col_tube->getRadius() * Global::getScaleFactor();
			color_ratio[ac].a = 0.5;

		}

		const BALL::VIEW::Tube* tube = dynamic_cast<const BALL::VIEW::Tube*>(*it);

		if(tube)
		{

			BALL::Vector3 d = ((tube->getVertex1() - tube->getVertex2()) * 0.5f) * Global::getScaleFactor();
			BALL::Vector3 p = ((tube->getVertex2() + tube->getVertex1()) * 0.5f) * Global::getScaleFactor();

			BALL::VIEW::ColorRGBA const& col = tube->getColor();

			midpoint[ac].a  = p.x;
			midpoint[ac].b  = p.y;
			midpoint[ac].c  = p.z;
			direction[ac].a = d.x;
			direction[ac].b = d.y;
			direction[ac].c = d.z;
			color1[ac].a = (float) col.getRed();
			color1[ac].b = (float) col.getGreen();
			color1[ac].c = (float) col.getBlue();
			color1[ac].d = (float) col.getAlpha();
			color2[ac].a = (float) col.getRed();
			color2[ac].b = (float) col.getGreen();
			color2[ac].c = (float) col.getBlue();
			color2[ac].d = (float) col.getAlpha();
			radius[ac].a  = tube->getRadius() * Global::getScaleFactor();
			color_ratio[ac].a = 0.5;
		}
	}

	vbo_->setNumberOfElements(num_elements);
	vbo_->setAttribute3f(midpoint_idx,midpoint);
	vbo_->setAttribute3f(direction_idx,direction);
	vbo_->setAttribute4f(color1_idx,color1);
	vbo_->setAttribute4f(color2_idx,color2);
	vbo_->setAttribute1f(radius_idx,radius);
	vbo_->setAttribute1f(color_ratio_idx,color_ratio);
}
