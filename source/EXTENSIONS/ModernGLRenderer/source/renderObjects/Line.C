#include <renderObjects/Line.h>
#include <vbo/VBOLine.h>
#include <Global.h>

#include <BALL/VIEW/PRIMITIVES/line.h>


Line::Line(Camera* camera)
	: Geometry(camera)
{
	vbo_ = new VBOLine;
	shader_name_ = "line";
	transparency_init_ = "line_transparency_init";
	transparency_peel_ = "line_transparency_peel";

	compileShaderPrograms_();

	shaders_.push_back(shader_name_);

	if(Global::getTransparencySupport())
	{
		shaders_.push_back(transparency_init_);
		shaders_.push_back(transparency_peel_);
	}

	initShaderPrograms_();
}

void Line::register_()
{
	camera_->registerViewMatrix(this);
	camera_->registerProjectionMatrix(this);
	camera_->registerClipRange(this);
}

void Line::unregister_()
{
	camera_->unregisterViewMatrix(this);
	camera_->unregisterProjectionMatrix(this);
	camera_->unregisterClipRange(this);
}

void Line::render_()
{
	vbo_->render();
}

void Line::compileShaderPrograms_()
{
	std::vector<const char*> shaders;
	shaders.push_back("line/line_vertex.glsl");
	shaders.push_back("line/line_fragment.glsl");
	shaders.push_back("opaque/opaque_vertex.glsl");
	shaders.push_back("opaque/opaque_fragment.glsl");
	shaders.push_back("encode_fragment.glsl");
	shaders.push_back("clipping_plane_fragment.glsl");
	shader_manager_->createShaderProgram(shader_name_, shaders);

	if(Global::getTransparencySupport())
	{
		std::vector<const char*> shaders;
		shaders.push_back("line/line_vertex.glsl");
		shaders.push_back("line/line_fragment.glsl");
		shaders.push_back("transparent/dual_peeling_init_vertex.glsl");
		shaders.push_back("transparent/dual_peeling_init_fragment.glsl");
		shaders.push_back("clipping_plane_fragment.glsl");
		shader_manager_->createShaderProgram(transparency_init_, shaders);

		std::vector<const char*> shaders2;
		shaders2.push_back("line/line_vertex.glsl");
		shaders2.push_back("line/line_fragment.glsl");
		shaders2.push_back("compositing/blinn_phong_fragment.glsl");
		shaders2.push_back("transparent/dual_peeling_peel_vertex.glsl");
		shaders2.push_back("transparent/dual_peeling_peel_fragment.glsl");
		shaders2.push_back("clipping_plane_fragment.glsl");
		shader_manager_->createShaderProgram(transparency_peel_, shaders2);
	}
}

void Line::initShaderPrograms_()
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

void Line::initVBO_ ( std::list< const BALL::VIEW::GeometricObject* >& list )
{

	size_t num_elements = list.size();
	std::vector<Attrib3> verts(num_elements * 2);
	std::vector<Attrib4> color(num_elements * 2);

	std::list<const BALL::VIEW::GeometricObject*>::const_iterator it;
	int ac = 0;

	for (it =  list.begin(); it != list.end(); it++, ac++)
	{
		const BALL::VIEW::Line* line = dynamic_cast<const BALL::VIEW::Line*>(*it);

		if(line)
		{
			BALL::Vector3 v1 = line->getVertex1() * Global::getScaleFactor();
			BALL::Vector3 v2 = line->getVertex2() * Global::getScaleFactor();
			BALL::VIEW::ColorRGBA c =line->getColor();

			verts [2*ac  ].a = v1.x;
			verts[2*ac  ].b = v1.y;
			verts[2*ac  ].c = v1.z;
			verts [2*ac+1].a = v2.x;
			verts[2*ac+1].b = v2.y;
			verts[2*ac+1].c = v2.z;
			color [2*ac  ].a = c.getRed();
			color[2*ac  ].b = c.getGreen();
			color[2*ac  ].c = c.getBlue();
			color[2*ac  ].d = c.getAlpha();
			color [2*ac+1].a = c.getRed();
			color[2*ac+1].b = c.getGreen();
			color[2*ac+1].c = c.getBlue();
			color[2*ac+1].d = c.getAlpha();
		}
	}

	vbo_->setNumberOfElements(num_elements * 2);
	vbo_->setAttribute3f(vertex_position_idx,verts);
	vbo_->setAttribute4f(color_idx,color);
}
