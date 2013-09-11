#include <GL/glew.h>

#include <renderObjects/Geometry.h>

Geometry::Geometry(Camera* camera)
	: RenderObject(camera),
	  vbo_(0)
{
}

Geometry::~Geometry()
{
	delete vbo_;
}

void Geometry::cleanup_()
{
	RenderObject::cleanup_();
	vbo_->setup();
}

void Geometry::render(RenderLevel level)
{
	setUniforms_();

	switch (level)
	{
		case TRANSPARENCY_INIT:
			bindTexturesTransparencyInit_();
			shader_manager_->getShader(transparency_init_)->bind();
			break;
		case TRANSPARENCY_PEEL:
			shader_manager_->getShader(transparency_peel_)->bind();
			shader_manager_->getShader(transparency_peel_)->bindTextureRECT("DepthBlenderTex", depth_tex_idx, 0);
			shader_manager_->getShader(transparency_peel_)->bindTextureRECT("FrontBlenderTex", front_blender_tex_idx, 1);
			bindTexturesTransparencyPeel_();
			break;
		case OPAQUE:
			shader_manager_->getShader(shader_name_)->bind();
			bindTexturesOpaque_();
			break;
		default:
			BALL::Log.error() << "[Geometry]: Renderobject does not support this kind of renderlevel. Skip this representation." << std::endl;
			return;
	}

	render_();
	shader_manager_->release();
}
