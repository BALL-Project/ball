#include <GL/glew.h>

#include <renderObjects/TwoColoredLine.h>
#include <Global.h>

#include <BALL/VIEW/PRIMITIVES/twoColoredLine.h>

TwoColoredLine::TwoColoredLine(Camera* camera)
	: Geometry(camera)
{
	vbo_ = new VBOLine;
	shader_name_ = "two_colored_line";
	transparency_init_ = "two_colored_line_transparency_init";
	transparency_peel_ = "two_colored_line_transparency_peel";

	compileShaderPrograms_();

	shaders_.push_back(shader_name_);

	if(Global::getTransparencySupport())
	{
		shaders_.push_back(transparency_init_);
		shaders_.push_back(transparency_peel_);
	}

	initShaderPrograms_();
}

void TwoColoredLine::register_()
{
	camera_->registerViewMatrix(this);
	camera_->registerProjectionMatrix(this);
	camera_->registerClipRange(this);
}

void TwoColoredLine::unregister_()
{
	camera_->unregisterViewMatrix(this);
	camera_->unregisterProjectionMatrix(this);
	camera_->unregisterClipRange(this);
}

void TwoColoredLine::render_()
{
	vbo_->render();
}

void TwoColoredLine::compileShaderPrograms_()
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

void TwoColoredLine::initShaderPrograms_()
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

void TwoColoredLine::initVBO_ ( std::list< const BALL::VIEW::GeometricObject* >& list )
{

	size_t num_elements = list.size();
	std::vector<Attrib3> verts(num_elements * 4);
	std::vector<Attrib4> color(num_elements * 4);

	std::list<const BALL::VIEW::GeometricObject*>::const_iterator it;
	int ac = 0;

	for (it =  list.begin(); it != list.end(); it++, ac++)
	{
		const BALL::VIEW::TwoColoredLine* line = dynamic_cast<const BALL::VIEW::TwoColoredLine*>(*it);

		if(line)
		{
			BALL::Vector3 v1 = line->getVertex1() * Global::getScaleFactor();
			BALL::Vector3 v2 = line->getVertex2() * Global::getScaleFactor();
			BALL::Vector3 vm = line->getMiddleVertex() * Global::getScaleFactor();
			BALL::VIEW::ColorRGBA c1 = line->getColor();
			BALL::VIEW::ColorRGBA c2 = line->getColor2();

			verts [4*ac  ].a = v1.x;
			verts[4*ac  ].b = v1.y;
			verts[4*ac  ].c = v1.z;
			verts [4*ac+1].a = vm.x;
			verts[4*ac+1].b = vm.y;
			verts[4*ac+1].c = vm.z;
			verts [4*ac+2].a = vm.x;
			verts[4*ac+2].b = vm.y;
			verts[4*ac+2].c = vm.z;
			verts [4*ac+3].a = v2.x;
			verts[4*ac+3].b = v2.y;
			verts[4*ac+3].c = v2.z;
			color [4*ac  ].a = c1.getRed();
			color[4*ac  ].b = c1.getGreen();
			color[4*ac  ].c = c1.getBlue();
			color[4*ac  ].d = c1.getAlpha();
			color [4*ac+1].a = c1.getRed();
			color[4*ac+1].b = c1.getGreen();
			color[4*ac+1].c = c1.getBlue();
			color[4*ac+1].d = c1.getAlpha();
			color [4*ac+2].a = c2.getRed();
			color[4*ac+2].b = c2.getGreen();
			color[4*ac+2].c = c2.getBlue();
			color[4*ac+2].d = c2.getAlpha();
			color [4*ac+3].a = c2.getRed();
			color[4*ac+3].b = c2.getGreen();
			color[4*ac+3].c = c2.getBlue();
			color[4*ac+3].d = c2.getAlpha();
		}
	}

	vbo_->setNumberOfElements(num_elements * 4);
	vbo_->setAttribute3f(vertex_position_idx,verts);
	vbo_->setAttribute4f(color_idx,color);
}
