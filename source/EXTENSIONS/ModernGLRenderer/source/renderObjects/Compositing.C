#include <GL/glew.h>

#include <renderObjects/Compositing.h>

Compositing::Compositing(Camera* camera)
	: RenderObject(camera),
	  ssao_texture_(-1),
	  volume_texture_(-1)
{
}

Compositing::~Compositing()
{
	delete vbo_;
}

void Compositing::cleanup_()
{
	RenderObject::cleanup_();
	vbo_->setup();
}

void Compositing::setAO(bool value)
{
	setUniform1b_(QString("use_ao"), value);
}

void Compositing::setVolume(bool value)
{
	setUniform1b_("use_volume", value);
}

void Compositing::setClearColor ( float r, float g, float b, float a )
{
	setUniform4f_("clear_color", r, g, b, a);
}


void Compositing::setSSAOTexture ( GLuint textureid )
{
	ssao_texture_ = textureid;
}

void Compositing::setVolumeTexture ( GLuint textureid )
{
	volume_texture_ = textureid;
}

void Compositing::init ()
{
	RenderObject::init();
	initVBO_();
}