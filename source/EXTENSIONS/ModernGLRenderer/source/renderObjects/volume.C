#include <GL/glew.h>

#include <renderObjects/volume.h>
#include <global.h>

#include <BALL/VIEW/DATATYPE/colorMap.h>
#include <BALL/VIEW/PRIMITIVES/gridVisualisation.h>

#define BYTES_PER_TEXEL 4

Volume::Volume(Camera* camera)
	: RenderObject(camera),
	  vbo()
{
	shader_name_ = "cube";
	shader_name_final_image_ = "volume";

	compileShaderPrograms_();

	shaders_.push_back(shader_name_);
	shaders_.push_back(shader_name_final_image_);

	initShaderPrograms_();
}

void Volume::initialize_()
{
	RenderObject::initialize_();

	glGenTextures(1, &backface_color_);
	glGenTextures(1, &backface_pos_);
	glGenTextures(1, &final_image_);

	glGenFramebuffers(1, &rendertarget_fboid_);
}

void Volume::cleanup_()
{
	RenderObject::cleanup_();

	glDeleteTextures(1, &backface_color_);
	glDeleteTextures(1, &backface_pos_);
	glDeleteTextures(1, &final_image_);

	glDeleteFramebuffers(1, &rendertarget_fboid_);

	vbo.setup();
}

void Volume::register_()
{
	camera_->registerViewMatrix(this);
	camera_->registerProjectionMatrix(this);
	camera_->registerClipRange(this);
	camera_->registerViewport(this);
}

void Volume::unregister_()
{
	camera_->unregisterViewMatrix(this);
	camera_->unregisterProjectionMatrix(this);
	camera_->unregisterClipRange(this);
	camera_->unregisterViewport(this);
}

void Volume::prepare_rendering()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, rendertarget_fboid_);
	glClear(GL_COLOR_BUFFER_BIT);
}


//here starts the rendering
void Volume::render(RenderLevel /*level*/)
{
	renderBackface_();
	raycastingPass_();
}

void Volume::setViewport(unsigned int w, unsigned int h, unsigned int x, unsigned int y )
{
	RenderObject::setViewport(w, h, x, y);

	setupRenderTargets(w,h);
}

void Volume::setStepsize ( float stepsize )
{
	shader_manager_->getShader(shader_name_final_image_)->setUniform1f("stepsize", stepsize);
}

void Volume::renderBackface_()
{
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, backface_color_, 0);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, backface_pos_, 0);

	GLenum buffers[] = {GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1};
	glDrawBuffers(2, buffers);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	shader_manager_->getShader(shader_name_)->bind();

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	vbo.render();

	glDisable(GL_CULL_FACE);

	shader_manager_->release();
}

void Volume::raycastingPass_()
{
	Shader* s = shader_manager_->getShader(shader_name_final_image_);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, final_image_, 0);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	s->bind();
	s->bindTexture2D("backface_tex",     backface_color_ , 0);
	s->bindTexture2D("backface_pos_tex", backface_pos_ ,   1);
	s->bindTexture3D("volume_tex",       volume_texture_ , 2);
	s->bindTexture2D("normal_map",       normal_texture_ , 3);
	s->bindTexture2D("depth_map",        depth_texture_ ,  4);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	vbo.render();

	glDisable(GL_CULL_FACE);

	s->releaseTexture(GL_TEXTURE_2D,4);
	s->releaseTexture(GL_TEXTURE_2D,3);
	s->releaseTexture(GL_TEXTURE_3D,2);
	s->releaseTexture(GL_TEXTURE_2D,1);
	s->releaseTexture(GL_TEXTURE_2D,0);

	s->release();
}

void Volume::createTextureFromGrid_(const BALL::VIEW::GridVisualisation* vis)
{
	const BALL::RegularData3D* grid = vis->getGrid();
	const BALL::VIEW::ColorMap* map = vis->getColorMap().get();

	glDeleteTextures(1, (GLuint*)&volume_texture_);
	BALL::RegularData3D::IndexType tex_size = grid->getSize();

	// Generate The Texture
	unsigned int i = 0;
	GLubyte* texels = new GLubyte[tex_size.x * tex_size.y * tex_size.z * BYTES_PER_TEXEL];

	for (unsigned int z = 0; z < tex_size.z; z++)
	{
		for (unsigned int y = 0; y < tex_size.y; y++)
		{
			for (unsigned int x = 0; x < tex_size.x; x++)
			{
				const BALL::VIEW::ColorRGBA& c = map->map(grid->getData(BALL::RegularData3D::IndexType(x,y,z)));
				texels[i + 0] = (unsigned char)c.getRed();
				texels[i + 1] = (unsigned char)c.getGreen();
				texels[i + 2] = (unsigned char)c.getBlue();
				texels[i + 3] = (unsigned char)c.getAlpha();
				i += 4;
			}
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glGenTextures(1, (GLuint*)&volume_texture_);
	glBindTexture(GL_TEXTURE_3D, volume_texture_);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, tex_size.x, tex_size.y, tex_size.z, 0, GL_RGBA, GL_UNSIGNED_BYTE, texels);
	glBindTexture(GL_TEXTURE_3D, 0);
	delete[] texels;

	std::cerr << "[Volume] Volume texture created" << std::endl;
}

void Volume::compileShaderPrograms_()
{
	std::vector<const char*> shaders;
	shaders.push_back("volume/backface_vertex.glsl");
	shaders.push_back("volume/backface_fragment.glsl");
	shaders.push_back("encode_fragment.glsl");
	shader_manager_->createShaderProgram(shader_name_, shaders);

	std::vector<const char*> shaders2;
	shaders2.push_back("volume/volume_vertex.glsl");
	shaders2.push_back("volume/volume_fragment.glsl");
	shaders2.push_back("encode_fragment.glsl");
	shaders2.push_back("clipping_plane_fragment.glsl");
	shader_manager_->createShaderProgram(shader_name_final_image_, shaders2);
}

void Volume::initShaderPrograms_()
{
	vertex_position_idx = 0;
	color_idx           = 1;

	//iterate over all shaders and set the attribute locations
	QVector<const char*>::iterator i = shaders_.begin();

	while (i != shaders_.end())
	{
		shader_manager_->getShader(*i)->setAttributeLocation(vertex_position_idx, "position");
		shader_manager_->getShader(*i)->setAttributeLocation(color_idx          , "color");
		++i;
	}
}

void Volume::setupRenderTargets(unsigned int w, unsigned int h)
{

	//Setup the backface color texture
	glBindTexture(GL_TEXTURE_2D, backface_color_);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_INT, NULL);

	//Setup the backface pos texture
	glBindTexture(GL_TEXTURE_2D, backface_pos_);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, w, h, 0, GL_RGBA, GL_FLOAT, NULL);

	//Setup the final image texture
	glBindTexture(GL_TEXTURE_2D, final_image_);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_INT, NULL);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, rendertarget_fboid_);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, backface_color_, 0);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, backface_pos_, 0);


	if( glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "\n Error:: FrameBufferObject::Initialize() :: FBO loading not complete (Volume) \n";
		std::cout << w << " " << h << "\n";
		std::cout << "Errorcode: " << glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) << " "
		          << GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT << " "
		          << GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT << " "
		          << GL_FRAMEBUFFER_UNSUPPORTED << "\n";
	}

	//break the existing binding.
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

}

GLuint Volume::getVolumeTexture()
{
	return final_image_;
}

void Volume::setDepthTexture(GLuint depth_texture)
{
	depth_texture_ = depth_texture;
}

void Volume::setNormalTexture(GLuint normal_texture)
{
	normal_texture_ = normal_texture;
}

void Volume::initVBO_ ( std::list< const BALL::VIEW::GeometricObject* >& list )
{
	//size_t num_elements = list.size();

	std::list<const BALL::VIEW::GeometricObject*>::const_iterator it;
	int ac = 0;

	for (it =  list.begin();
	     it != list.end();
	     it++, ac++)
	{
		const BALL::VIEW::GridVisualisation* volume = dynamic_cast<const BALL::VIEW::GridVisualisation*>(*it);

		if(volume)
		{

			createTextureFromGrid_(volume);
			setStepsize(1. / (float) volume->slices);

			int n_quads = 6;
			int n_vertices = 8;

			std::vector<Attrib3> verts(n_vertices);
			std::vector<Attrib4> color(n_vertices);
			std::vector<QuadIndex> indxs(n_quads);

			BALL::Vector3 o = volume->origin * Global::getScaleFactor();
			BALL::Vector3 x = volume->x * Global::getScaleFactor();
			BALL::Vector3 y = volume->y * Global::getScaleFactor();
			BALL::Vector3 z = volume->z * Global::getScaleFactor();

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

			color[0].a = 0;
			color[0].b = 0;
			color[0].c = 0;
			color[0].d = 1;
			color[1].a = 1;
			color[1].b = 0;
			color[1].c = 0;
			color[1].d = 1;
			color[2].a = 1;
			color[2].b = 0;
			color[2].c = 1;
			color[2].d = 1;
			color[3].a = 0;
			color[3].b = 0;
			color[3].c = 1;
			color[3].d = 1;
			color[4].a = 0;
			color[4].b = 1;
			color[4].c = 0;
			color[4].d = 1;
			color[5].a = 1;
			color[5].b = 1;
			color[5].c = 0;
			color[5].d = 1;
			color[6].a = 1;
			color[6].b = 1;
			color[6].c = 1;
			color[6].d = 1;
			color[7].a = 0;
			color[7].b = 1;
			color[7].c = 1;
			color[7].d = 1;

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

			vbo.setup();
			vbo.setNumberOfElements(n_quads * 4);

			vbo.setAttribute3f(vertex_position_idx,verts);
			vbo.setAttribute4f(color_idx,color);

			vbo.setIndices(indxs);
		}
	}
}

