#include <GL/glew.h>

#include <renderObjects/SSAO.h>

#include <BALL/SYSTEM/path.h>


SSAO::SSAO(Camera* camera)
	: RenderObject(camera)
{
	shader_name_ = "ssao";

	defaultSSAOParameters.sample_radius = 0.3;  // 20.
	defaultSSAOParameters.intensity     = 1.;   // 2
	defaultSSAOParameters.scale         = 0.5;  // 0.1
	defaultSSAOParameters.bias          = 0.2;  // 0.05
	defaultSSAOParameters.useAO         = true;
	defaultSSAOParameters.downsampling  = 1;

	compileShaderPrograms_();

	shaders_.push_back(shader_name_);

	initShaderPrograms_();
}

SSAO::~SSAO()
{
	cleanup_();
}

void SSAO::initialize_()
{
	RenderObject::initialize_();

	glGenTextures(1, &ssao_texture_);
	glGenFramebuffers(1, &rendertarget_fboid_);

	setSSAOUniforms();

}

void SSAO::cleanup_()
{
	RenderObject::cleanup_();

	glDeleteTextures(1, &ssao_texture_);
	glDeleteFramebuffers(1, &rendertarget_fboid_);

	vbo.setup();
}

void SSAO::register_()
{
	camera_->registerViewMatrix(this);
	camera_->registerProjectionMatrixInverse(this);
	camera_->registerViewport(this);
	camera_->registerClipRange(this);
	camera_->registerFrustum(this);
}

void SSAO::unregister_()
{
	camera_->unregisterViewMatrix(this);
	camera_->unregisterProjectionMatrixInverse(this);
	camera_->unregisterViewport(this);
	camera_->unregisterClipRange(this);
	camera_->unregisterFrustum(this);
}

void SSAO::prepare_rendering()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, rendertarget_fboid_);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glClear(GL_COLOR_BUFFER_BIT);
}

//here starts the rendering
void SSAO::render(RenderLevel /*level*/)
{
	Shader* s = shader_manager_->getShader(shader_name_);
	s->bind();
	s->bindTexture2D("rand_norm" , noise_texture_ , 0);
	s->bindTexture2D("normal_map", normal_texture_, 1);
	s->bindTexture2D("depth_map" , depth_texture_ , 2);

	vbo.render();

	s->release();

	s->releaseTexture(GL_TEXTURE_2D,2);
	s->releaseTexture(GL_TEXTURE_2D,1);
	s->releaseTexture(GL_TEXTURE_2D,0);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void SSAO::setSSAOUniforms()
{
	setSSAOParameters(defaultSSAOParameters);
}

void SSAO::setViewport(unsigned int w, unsigned int h, unsigned int x, unsigned int y )
{
	float s = defaultSSAOParameters.downsampling;
	RenderObject::setViewport(w/s, h/s, x, y);

	setupRenderTargets(w/s,h/s);
}

void SSAO::setSSAOParameters(SSAOParameters ssaoParameters)
{
	Shader* s = shader_manager_->getShader(shader_name_);
	s->bind();

	s->setUniform1f("random_size" , 64.);
	s->setUniform1f("g_sample_rad", ssaoParameters.sample_radius);
	s->setUniform1f("g_intensity" , ssaoParameters.intensity);
	s->setUniform1f("g_scale"     , ssaoParameters.scale);
	s->setUniform1f("g_bias"      , ssaoParameters.bias);

	s->release();

	if(ssaoParameters.downsampling != defaultSSAOParameters.downsampling)
	{
		defaultSSAOParameters.downsampling = ssaoParameters.downsampling;
		//update vbo and viewport because downsampling has changed
		Viewport v = camera_->getViewport();
		setViewport(v.width, v.height, v.x, v.y);
		initVBO_();
	}
}

void SSAO::init ()
{

	cleanup_();
	initialize_();

	setupTextures_();

	initVBO_();
}


void SSAO::setupTextures_()
{
	QImage GLFormatImage;
	QImage QFormatImage;

	BALL::Path path = BALL::Path();
	BALL::String p = path.find("ModernGLRenderer/textures/");

	QString qpath((p + "noise.png").c_str());

	if (!QFormatImage.load(qpath))
	{
		std::cerr << "[SSAO] Error: Could not load texture " << qpath.toStdString() << std::endl;
	}

	GLFormatImage = QGLWidget::convertToGLFormat(QFormatImage);

	glGenTextures(1, &noise_texture_);
	glBindTexture(GL_TEXTURE_2D, noise_texture_);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, GLFormatImage.width(), GLFormatImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, GLFormatImage.bits());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


void SSAO::initVBO_()
{

	int n_vertices = 4;
	int n_quads = 1;
	float s = defaultSSAOParameters.downsampling;

	std::vector<QuadIndex> indxs(n_quads);
	std::vector<Attrib3> verts(n_vertices);
	std::vector<Attrib2> texcoords(n_vertices);

	verts[0].a = -1.0f;
	verts[0].b =  1.0f;
	verts[0].c = 0.0f;
	texcoords[0].a =  0.0f*s;
	texcoords[0].b =  1.0f*s;

	verts[1].a = -1.0f;
	verts[1].b = -1.0f;
	verts[1].c = 0.0f;
	texcoords[1].a =  0.0f*s;
	texcoords[1].b =  0.0f*s;

	verts[2].a =  1.0f;
	verts[2].b = -1.0f;
	verts[2].c = 0.0f;
	texcoords[2].a =  1.0f*s;
	texcoords[2].b =  0.0f*s;

	verts[3].a =  1.0f;
	verts[3].b =  1.0f;
	verts[3].c = 0.0f;
	texcoords[3].a =  1.0f*s;
	texcoords[3].b =  1.0f*s;

	indxs[0].a = 0;
	indxs[0].b = 1;
	indxs[0].c = 2;
	indxs[0].d = 3;

	vbo.setup();
	vbo.setNumberOfElements(n_quads * 4);
	vbo.setAttribute3f(position_idx,verts);
	vbo.setAttribute2f(texcoord_idx,texcoords);

	vbo.setIndices(indxs);

}

void SSAO::compileShaderPrograms_()
{
	std::vector<const char*> shaders;
	shaders.push_back("AO/SSAO_vertex.glsl");
	shaders.push_back("AO/SSAO_fragment.glsl");
	shaders.push_back("encode_fragment.glsl");
	shaders.push_back("position_reconstruction_fragment.glsl");
	shader_manager_->createShaderProgram(shader_name_, shaders);
}

void SSAO::initShaderPrograms_()
{
	position_idx         = 0;
	texcoord_idx         = 1;

	//iterate over all shaders and set the attribute locations
	QVector<const char*>::iterator i = shaders_.begin();

	while (i != shaders_.end())
	{
		shader_manager_->getShader(*i)->setAttributeLocation(position_idx, "position");
		shader_manager_->getShader(*i)->setAttributeLocation(texcoord_idx, "texture_coordinates");
		++i;
	}
}

void SSAO::setupRenderTargets(unsigned int w, unsigned int h)
{

	//Setup the ssao texture
	glBindTexture(GL_TEXTURE_2D, ssao_texture_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_INT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, rendertarget_fboid_);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssao_texture_, 0);

	if( glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "\n Error:: FrameBufferObject::Initialize() :: FBO loading not complete (SSAO) \n";
		std::cout << w << " " << h << "\n";
		std::cout << "Errorcode: " << glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) << " "
		          << GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT << " "
		          << GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT << " "
		          << GL_FRAMEBUFFER_UNSUPPORTED << "\n";
	}

	GLenum buffers[] = {GL_COLOR_ATTACHMENT0};

	glDrawBuffers(1, buffers);

	//break the existing binding.
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

GLuint SSAO::getSSAOTexture()
{
	return ssao_texture_;
}

void SSAO::setDepthTexture(GLuint depth_texture)
{
	depth_texture_ = depth_texture;
}

void SSAO::setNormalTexture(GLuint normal_texture)
{
	normal_texture_ = normal_texture;
}
