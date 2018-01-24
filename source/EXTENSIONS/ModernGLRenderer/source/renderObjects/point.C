#include <renderObjects/point.h>
#include <vbo/VBOPoint.h>
#include <global.h>

#include <BALL/VIEW/PRIMITIVES/point.h>

Point::Point(Camera* camera)
	: Geometry(camera)
{
	vbo_ = new VBOPoint;
	shader_name_ = "point";
	transparency_init_ = "point_transparency_init";
	transparency_peel_ = "point_transparency_peel";

	compileShaderPrograms_();

	shaders_.push_back(shader_name_);

	if(Global::getTransparencySupport())
	{
		shaders_.push_back(transparency_init_);
		shaders_.push_back(transparency_peel_);
	}

	initShaderPrograms_();
}

void Point::register_()
{
	camera_->registerViewMatrix(this);
	camera_->registerProjectionMatrix(this);
	camera_->registerClipRange(this);
}

void Point::unregister_()
{
	camera_->unregisterViewMatrix(this);
	camera_->unregisterProjectionMatrix(this);
	camera_->unregisterClipRange(this);
}

void Point::render_()
{
	vbo_->render();
}

void Point::compileShaderPrograms_()
{
	std::vector<const char*> shaders;
	shaders.push_back("point/point_vertex.glsl");
	shaders.push_back("point/point_fragment.glsl");
	shaders.push_back("opaque/opaque_vertex.glsl");
	shaders.push_back("opaque/opaque_fragment.glsl");
	shaders.push_back("encode_fragment.glsl");
	shaders.push_back("clipping_plane_fragment.glsl");
	shader_manager_->createShaderProgram(shader_name_, shaders);

	if(Global::getTransparencySupport())
	{
		std::vector<const char*> shaders;
		shaders.push_back("point/point_vertex.glsl");
		shaders.push_back("point/point_fragment.glsl");
		shaders.push_back("transparent/dual_peeling_init_vertex.glsl");
		shaders.push_back("transparent/dual_peeling_init_fragment.glsl");
		shaders.push_back("clipping_plane_fragment.glsl");
		shader_manager_->createShaderProgram(transparency_init_, shaders);

		std::vector<const char*> shaders2;
		shaders2.push_back("point/point_vertex.glsl");
		shaders2.push_back("point/point_fragment.glsl");
		shaders2.push_back("compositing/blinn_phong_fragment.glsl");
		shaders2.push_back("transparent/dual_peeling_peel_vertex.glsl");
		shaders2.push_back("transparent/dual_peeling_peel_fragment.glsl");
		shaders2.push_back("clipping_plane_fragment.glsl");
		shader_manager_->createShaderProgram(transparency_peel_, shaders2);
	}
}

void Point::initShaderPrograms_()
{
	vertex_position_idx = 0;
	color_idx           = 1;

	//iterate over all shaders and set the attribute locations
	QVector<const char*>::iterator i = shaders_.begin();

	while (i != shaders_.end())
	{
		shader_manager_->getShader(*i)->setAttributeLocation(vertex_position_idx, "vertex_position");
		shader_manager_->getShader(*i)->setAttributeLocation(color_idx          , "color");
		++i;
	}
}

void Point::initVBO_ ( std::list< const BALL::VIEW::GeometricObject* >& list )
{
	size_t num_elements = list.size();
	std::vector<Attrib3> verts(num_elements);
	std::vector<Attrib4> color(num_elements);

	std::list<const BALL::VIEW::GeometricObject*>::const_iterator it;
	int ac = 0;

	for (it =  list.begin(); it != list.end(); it++, ac++)
	{
		const BALL::VIEW::Point* point = dynamic_cast<const BALL::VIEW::Point*>(*it);

		if(point)
		{
			vector<BALL::Vector3> v;
			point->getVertices(v);

			if(v.size())
			{
				//NOTE we assume that there is only one point per representation and we render the first one
				BALL::Vector3 v1 = v[0] * Global::getScaleFactor();
				BALL::VIEW::ColorRGBA c = point->getColor();

				verts [ac].a = v1.x;
				verts[ac].b = v1.y;
				verts[ac].c = v1.z;
				color [ac].a = c.getRed();
				color[ac].b = c.getGreen();
				color[ac].c = c.getBlue();
				color[ac].d = c.getAlpha();
			}
		}
	}

	vbo_->setNumberOfElements(num_elements);
	vbo_->setAttribute3f(vertex_position_idx,verts);
	vbo_->setAttribute4f(color_idx,color);
}
