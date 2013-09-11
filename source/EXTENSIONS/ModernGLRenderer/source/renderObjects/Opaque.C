#include <GL/glew.h>
#include <renderObjects/Opaque.h>
#include <Global.h>

#include <QtCore/QDir>

#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/STRUCTURE/triangulatedSurface.h>
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/VIEW/DATATYPE/colorRGBA.h>

#include <set>
#include <vector>

Opaque::Opaque(Camera* camera)
	: Compositing(camera)
{
	shader_name_ = "opaque";

	vbo_ = new VBOQuad;
	compileShaderPrograms_();
	shaders_.push_back(shader_name_);
	initShaderPrograms_();
}

void Opaque::initialize_()
{
	Compositing::initialize_();

	glGenTextures(5, rendertarget_textures_);
	glGenFramebuffers(1, &rendertarget_fboid_);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		BALL::Log.error() << "Failed to make complete framebuffer object " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
	}

	glGenRenderbuffers(1, &renderbuffer_depth_);
}

void Opaque::cleanup_()
{
	Compositing::cleanup_();

	glDeleteTextures(5, rendertarget_textures_);
	glDeleteFramebuffers(1, &rendertarget_fboid_);
	glDeleteRenderbuffers(1, &renderbuffer_depth_);
}

void Opaque::register_()
{
	camera_->registerViewMatrix(this);
	camera_->registerProjectionMatrixInverse(this);
	camera_->registerViewport(this);
	camera_->registerClipRange(this);
	camera_->registerFrustum(this);
}

void Opaque::unregister_()
{
	camera_->unregisterViewMatrix(this);
	camera_->unregisterProjectionMatrixInverse(this);
	camera_->unregisterViewport(this);
	camera_->unregisterClipRange(this);
	camera_->unregisterFrustum(this);
}

void Opaque::prepare_rendering(RenderLevel /*level*/)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, rendertarget_fboid_);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLenum buffers[] = {GL_COLOR_ATTACHMENT0,
	                    GL_COLOR_ATTACHMENT1,
	                    GL_COLOR_ATTACHMENT2,
	                    GL_COLOR_ATTACHMENT3,
	                    GL_COLOR_ATTACHMENT4
	                   };

	glDrawBuffers(5, buffers);
}

//here starts the rendering
void Opaque::render(RenderLevel /*level*/)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	renderOpaque_();
}

void Opaque::setViewport(unsigned int w, unsigned int h, unsigned int x, unsigned int y )
{
	Compositing::setViewport(w, h, x, y);

	setupRenderTargets(w,h);
}

void Opaque::renderOpaque_()
{
	Shader* shader = shader_manager_->getShader(shader_name_);
	shader->bind();

	shader->bindTexture2D("ambient_map",  rendertarget_textures_[0], 0);
	shader->bindTexture2D("diffuse_map",  rendertarget_textures_[1], 1);
	shader->bindTexture2D("specular_map", rendertarget_textures_[2], 2);
	shader->bindTexture2D("normal_map",   rendertarget_textures_[3], 3);
	shader->bindTexture2D("depth_map",    rendertarget_textures_[4], 4);

	shader->bindTexture2D("ssao_map",     ssao_texture_,             5);
	shader->bindTexture2D("volume_map",   volume_texture_,           6);

	vbo_->render();

	shader->releaseTexture(GL_TEXTURE_2D,6);
	shader->releaseTexture(GL_TEXTURE_2D,5);
	shader->releaseTexture(GL_TEXTURE_2D,4);
	shader->releaseTexture(GL_TEXTURE_2D,3);
	shader->releaseTexture(GL_TEXTURE_2D,2);
	shader->releaseTexture(GL_TEXTURE_2D,1);
	shader->releaseTexture(GL_TEXTURE_2D,0);
	shader->release();

}

void Opaque::initVBO_()
{
	int n_vertices = 4;
	int n_quads = 1;

	std::vector<QuadIndex> indxs(n_quads);
	std::vector<Attrib3> verts(n_vertices);
	std::vector<Attrib2> texcoords(n_vertices);

	verts[0].a = -1.0f;
	verts[0].b =  1.0f;
	verts[0].c = 0.0f;
	texcoords[0].a =  0.0f;
	texcoords[0].b =  1.0f;

	verts[1].a = -1.0f;
	verts[1].b = -1.0f;
	verts[1].c = 0.0f;
	texcoords[1].a =  0.0f;
	texcoords[1].b =  0.0f;

	verts[2].a =  1.0f;
	verts[2].b = -1.0f;
	verts[2].c = 0.0f;
	texcoords[2].a =  1.0f;
	texcoords[2].b =  0.0f;

	verts[3].a =  1.0f;
	verts[3].b =  1.0f;
	verts[3].c = 0.0f;
	texcoords[3].a =  1.0f;
	texcoords[3].b =  1.0f;

	indxs[0].a = 0;
	indxs[0].b = 1;
	indxs[0].c = 2;
	indxs[0].d = 3;

	vbo_->setNumberOfElements(n_quads * 4);
	vbo_->setAttribute3f(quad_position_idx,verts);
	vbo_->setAttribute2f(quad_texcoord_idx,texcoords);

	vbo_->setIndices(indxs);
}

void Opaque::compileShaderPrograms_()
{
	std::vector<const char*> shaders;
	shaders.push_back("compositing/compositing_vertex.glsl");
	shaders.push_back("compositing/compositing_fragment.glsl");
	shaders.push_back("encode_fragment.glsl");
	shaders.push_back("position_reconstruction_fragment.glsl");
	shaders.push_back("compositing/blinn_phong_fragment.glsl");
	shader_manager_->createShaderProgram(shader_name_, shaders);
}

void Opaque::initShaderPrograms_()
{
	quad_position_idx         = 0;
	quad_texcoord_idx         = 1;

	//iterate over all shaders and set the attribute locations
	QVector<const char*>::iterator i = shaders_.begin();

	while (i != shaders_.end())
	{
		shader_manager_->getShader(*i)->setAttributeLocation(quad_position_idx, "position");
		shader_manager_->getShader(*i)->setAttributeLocation(quad_texcoord_idx, "texture_coordinates");
		++i;
	}
}


void Opaque::setupRenderTargets(unsigned int w, unsigned int h)
{

	//Setup the ambient texture
	glBindTexture(GL_TEXTURE_2D, rendertarget_textures_[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_INT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//Setup the diffuse texture
	glBindTexture(GL_TEXTURE_2D, rendertarget_textures_[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_INT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//Setup the specular + shininess texture
	glBindTexture(GL_TEXTURE_2D, rendertarget_textures_[2]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_INT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//Setup the normal texture (we do not use fp textures here, as they are still not commonly available)
	glBindTexture(GL_TEXTURE_2D, rendertarget_textures_[3]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_INT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//Setup the depth texture
	glBindTexture(GL_TEXTURE_2D, rendertarget_textures_[4]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//Setup the renderbuffer for the z-test
	glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer_depth_);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, w, h);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, rendertarget_fboid_);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rendertarget_textures_[0], 0);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, rendertarget_textures_[1], 0);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, rendertarget_textures_[2], 0);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, rendertarget_textures_[3], 0);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, rendertarget_textures_[4], 0);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer_depth_);

	if( glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "\n Error:: FrameBufferObject::Initialize() :: FBO loading not complete (Opaque) \n";
		std::cout << w << " " << h << "\n";
		std::cout << "Errorcode: " << glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) << " "
		          << GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT << " "
		          << GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT << " "
		          << GL_FRAMEBUFFER_UNSUPPORTED << "\n";
	}

	CHECK_GL_ERRORS;

	GLenum buffers[] = {GL_COLOR_ATTACHMENT0,
	                    GL_COLOR_ATTACHMENT1,
	                    GL_COLOR_ATTACHMENT2,
	                    GL_COLOR_ATTACHMENT3,
	                    GL_COLOR_ATTACHMENT4
	                   };

	glDrawBuffers(5, buffers);

	//break the existing binding.
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

}

GLuint Opaque::getDepthTexture()
{
	return rendertarget_textures_[4];
}

GLuint Opaque::getNormalTexture()
{
	return rendertarget_textures_[3];
}
