#include <GL/glew.h>

#include <renderObjects/Box.h>
#include <vbo/VBOQuad.h>
#include <Global.h>

#include <BALL/VIEW/PRIMITIVES/box.h>


Box::Box(Camera* camera)
	: Geometry(camera)
{
	vbo_ = new VBOQuad;
	shader_name_ = "box";
	transparency_init_ = "box_transparency_init";
	transparency_peel_ = "box_transparency_peel";

	compileShaderPrograms_();

	shaders_.push_back(shader_name_);

	if(Global::getTransparencySupport())
	{
		shaders_.push_back(transparency_init_);
		shaders_.push_back(transparency_peel_);
	}

	initShaderPrograms_();
}

void Box::register_()
{
	camera_->registerViewMatrix(this);
	camera_->registerProjectionMatrix(this);
	camera_->registerClipRange(this);
}

void Box::unregister_()
{
	camera_->unregisterViewMatrix(this);
	camera_->unregisterProjectionMatrix(this);
	camera_->unregisterClipRange(this);
}

void Box::render_()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	vbo_->render();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}


void Box::compileShaderPrograms_()
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

void Box::initShaderPrograms_()
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

void Box::initVBO_ ( std::list< const BALL::VIEW::GeometricObject* >& list )
{
	std::list<const BALL::VIEW::GeometricObject*>::const_iterator it;
	int ac = 0;

	for (it =  list.begin(); it != list.end(); it++, ac++)
	{
		const BALL::VIEW::Box* box = dynamic_cast<const BALL::VIEW::Box*>(*it);

		if(box)
		{
			int n_quads = 6;
			int n_vertices = 8;

			std::vector<Attrib3> verts(n_vertices);
			std::vector<Attrib4> color(n_vertices);
			std::vector<QuadIndex> indxs(n_quads);

			BALL::Vector3 o = box->getPoint() * Global::getScaleFactor();
			BALL::Vector3 x = box->getRightVector() * Global::getScaleFactor();
			BALL::Vector3 y = box->getHeightVector() * Global::getScaleFactor();
			BALL::Vector3 z = box->getDepthVector() * Global::getScaleFactor();

			BALL::Vector3 v = o;
			verts[0].a = v.x;
			verts[0].b = v.y;
			verts[0].c = v.z;
			v = o + x;
			verts[1].a = v.x;
			verts[1].b = v.y;
			verts[1].c = v.z;
			v = o + x +     z;
			verts[2].a = v.x;
			verts[2].b = v.y;
			verts[2].c = v.z;
			v = o +         z;
			verts[3].a = v.x;
			verts[3].b = v.y;
			verts[3].c = v.z;
			v = o +     y;
			verts[4].a = v.x;
			verts[4].b = v.y;
			verts[4].c = v.z;
			v = o + x + y;
			verts[5].a = v.x;
			verts[5].b = v.y;
			verts[5].c = v.z;
			v = o + x + y + z;
			verts[6].a = v.x;
			verts[6].b = v.y;
			verts[6].c = v.z;
			v = o +     y + z;
			verts[7].a = v.x;
			verts[7].b = v.y;
			verts[7].c = v.z;

			const BALL::VIEW::ColorRGBA c = box->getColor();

			for(int i=0; i < 8; i++)
			{
				color[i].a = c.getRed();
				color[i].b = c.getGreen();
				color[i].c = c.getBlue();
				color[i].d = c.getAlpha();
			}

			indxs[0].a = 1;
			indxs[0].b = 2;
			indxs[0].c = 3;
			indxs[0].d = 0;
			indxs[1].a = 5;
			indxs[1].b = 1;
			indxs[1].c = 0;
			indxs[1].d = 4;
			indxs[2].a = 6;
			indxs[2].b = 2;
			indxs[2].c = 1;
			indxs[2].d = 5;
			indxs[3].a = 7;
			indxs[3].b = 3;
			indxs[3].c = 2;
			indxs[3].d = 6;
			indxs[4].a = 4;
			indxs[4].b = 0;
			indxs[4].c = 3;
			indxs[4].d = 7;
			indxs[5].a = 6;
			indxs[5].b = 5;
			indxs[5].c = 4;
			indxs[5].d = 7;

			vbo_->setNumberOfElements(n_quads * 4);
			vbo_->setAttribute3f(vertex_position_idx,verts);
			vbo_->setAttribute4f(color_idx,color);

			vbo_->setIndices(indxs);
		}
	}
}
