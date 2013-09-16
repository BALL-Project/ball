#include <GL/glew.h>
#include <renderObjects/Transparent.h>
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

#define MAX_DEPTH 1.0

Transparent::Transparent(Camera* camera)
	: Compositing(camera),
	  background_color_(0,0,0,0)
{
	vbo_ = new VBOQuad;
	shader_blend_ = "transparent_blend";
	shader_final_ = "transparent_final";

	tp_.useOQ = true;
	tp_.numPasses = 4;

	compileShaderPrograms_();

	shaders_.push_back(shader_blend_);
	shaders_.push_back(shader_final_);

	initShaderPrograms_();
}

void Transparent::initialize_()
{
	Compositing::initialize_();

	glGenTextures(2, g_dualDepthTexId);
	glGenTextures(2, g_dualFrontBlenderTexId);
	glGenTextures(2, g_dualBackTempTexId);
	glGenFramebuffersEXT(1, &g_dualPeelingSingleFboId);

	g_drawBuffers[0] = GL_COLOR_ATTACHMENT0_EXT;
	g_drawBuffers[1] = GL_COLOR_ATTACHMENT1_EXT;
	g_drawBuffers[2] = GL_COLOR_ATTACHMENT2_EXT;
	g_drawBuffers[3] = GL_COLOR_ATTACHMENT3_EXT;
	g_drawBuffers[4] = GL_COLOR_ATTACHMENT4_EXT;
	g_drawBuffers[5] = GL_COLOR_ATTACHMENT5_EXT;
	g_drawBuffers[6] = GL_COLOR_ATTACHMENT6_EXT;

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		BALL::Log.error() << "Failed to make complete framebuffer object " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
	}

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	glDisable(GL_NORMALIZE);

	glGenQueries(1, &g_queryId);
}

void Transparent::cleanup_()
{
	Compositing::cleanup_();

	glDeleteFramebuffersEXT(1, &g_dualBackBlenderFboId);
	glDeleteFramebuffersEXT(1, &g_dualPeelingSingleFboId);
	glDeleteTextures(2, g_dualDepthTexId);
	glDeleteTextures(2, g_dualFrontBlenderTexId);
	glDeleteTextures(2, g_dualBackTempTexId);
	glDeleteTextures(1, &g_dualBackBlenderTexId);
}

void Transparent::register_()
{
	camera_->registerViewMatrix(this);
	camera_->registerProjectionMatrixInverse(this);
	camera_->registerViewport(this);
	camera_->registerClipRange(this);
}

void Transparent::unregister_()
{
	camera_->unregisterViewMatrix(this);
	camera_->unregisterProjectionMatrixInverse(this);
	camera_->unregisterViewport(this);
	camera_->unregisterClipRange(this);
}

void Transparent::prepare_rendering(RenderLevel level)
{
	switch (level)
	{
		case TRANSPARENCY_INIT:
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);

			// ---------------------------------------------------------------------
			// 1. Initialize Min-Max Depth Buffer
			// ---------------------------------------------------------------------

			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, g_dualPeelingSingleFboId);

			// Render targets 1 and 2 store the front and back colors
			// Clear to 0.0 and use MAX blending to filter written color
			// At most one front color and one back color can be written every pass
			glDrawBuffers(2, &g_drawBuffers[1]);
			glClearColor(0, 0, 0, 0);
			glClear(GL_COLOR_BUFFER_BIT);

			// Render target 0 stores (-minDepth, maxDepth, alphaMultiplier)
			glDrawBuffer(g_drawBuffers[0]);
			glClearColor(-MAX_DEPTH, -MAX_DEPTH, 0, 0);
			glClear(GL_COLOR_BUFFER_BIT);
			glBlendEquationEXT(GL_MAX_EXT);
			break;
		case TRANSPARENCY_PEEL:
			// ---------------------------------------------------------------------
			// 2. Dual Depth Peeling + Blending
			// ---------------------------------------------------------------------

			// Since we cannot blend the back colors in the geometry passes,
			// we use another render target to do the alpha blending
			//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, g_dualBackBlenderFboId);
			glDrawBuffer(g_drawBuffers[6]);
			glClearColor(background_color_.x, background_color_.y, background_color_.z, 0);
			glClear(GL_COLOR_BUFFER_BIT);

			currId_ = 0;
			break;

		case TRANSPARENCY_FINAL:
			glDisable(GL_BLEND);

			// ---------------------------------------------------------------------
			// 3. Final Pass
			// ---------------------------------------------------------------------

			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
			glDrawBuffer(GL_BACK);
			break;

		default:
			std::cerr << "[Transparent]: RenderLevel " << level << "is not supported" << std::endl;
	}
}

void Transparent::render(RenderLevel level)
{
	render(0, level);
}

//here starts the rendering
bool Transparent::render(unsigned int pass, RenderLevel level)
{
	int bufId = 0;
	Shader* s_blend = shader_manager_->getShader(shader_blend_);
	Shader* s_final = shader_manager_->getShader(shader_final_);

	switch (level)
	{
		case TRANSPARENCY_PEEL:
			currId_ = pass % 2;
			bufId = currId_ * 3;

			//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, g_dualPeelingFboId[currId]);

			glDrawBuffers(2, &g_drawBuffers[bufId+1]);
			glClearColor(0, 0, 0, 0);
			glClear(GL_COLOR_BUFFER_BIT);

			glDrawBuffer(g_drawBuffers[bufId+0]);
			glClearColor(-MAX_DEPTH, -MAX_DEPTH, 0, 0);
			glClear(GL_COLOR_BUFFER_BIT);

			// Render target 0: RG32F MAX blending
			// Render target 1: RGBA MAX blending
			// Render target 2: RGBA MAX blending
			glDrawBuffers(3, &g_drawBuffers[bufId+0]);
			glBlendEquationEXT(GL_MAX_EXT);

			break;
		case TRANSPARENCY_BLEND:
			// Full screen pass to alpha-blend the back color
			glDrawBuffer(g_drawBuffers[6]);

			glBlendEquationEXT(GL_FUNC_ADD);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			if (tp_.useOQ)
			{
				glBeginQuery(GL_SAMPLES_PASSED_ARB, g_queryId);
			}

			s_blend->bind();
			s_blend->bindTextureRECT("TempTex", g_dualBackTempTexId[currId_], 0);
			vbo_->render();
			s_blend->releaseTexture(GL_TEXTURE_RECTANGLE_ARB,0);
			s_blend->release();

			CHECK_GL_ERRORS;

			if (tp_.useOQ)
			{
				glEndQuery(GL_SAMPLES_PASSED_ARB);
				GLuint sample_count;
				glGetQueryObjectuiv(g_queryId, GL_QUERY_RESULT_ARB, &sample_count);

				if (sample_count == 0)
				{
					return false;
				}
			}

			break;

		case TRANSPARENCY_FINAL:
			s_final->bind();
			s_final->bindTextureRECT("FrontBlenderTex", g_dualFrontBlenderTexId[currId_], 1);
			s_final->bindTextureRECT("BackBlenderTex", g_dualBackBlenderTexId, 2);
			s_final->bindTexture2D("ssao_map", ssao_texture_, 3);
			s_final->bindTexture2D("volume_map", volume_texture_, 4);
			s_final->bindTexture2D("normal_map", normal_texture_, 5);

			s_final->bind();

			vbo_->render();

			s_final->releaseTexture(GL_TEXTURE_2D,5);
			s_final->releaseTexture(GL_TEXTURE_2D,4);
			s_final->releaseTexture(GL_TEXTURE_2D,3);
			s_final->releaseTexture(GL_TEXTURE_RECTANGLE_ARB,2);
			s_final->releaseTexture(GL_TEXTURE_RECTANGLE_ARB,1);
			s_final->release();

			CHECK_GL_ERRORS;

			break;
		default:
			std::cerr << "[Transparent]: RenderLevel " << level << "is not supported" << std::endl;
	}

	return true;
}

void Transparent::setViewport(unsigned int w, unsigned int h, unsigned int x, unsigned int y )
{
	Compositing::setViewport(w, h, x, y);
	setupRenderTargets(w,h);
}

void Transparent::setClearColor ( float r, float g, float b, float a )
{
	Compositing::setClearColor(r,g,b,a);
	background_color_ = BALL::Vector4(r,g,b,a);
}

void Transparent::initVBO_()
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

void Transparent::compileShaderPrograms_()
{
	std::vector<const char*> shaders;
	shaders.push_back("transparent/dual_peeling_blend_vertex.glsl");
	shaders.push_back("transparent/dual_peeling_blend_fragment.glsl");
	shaders.push_back("compositing/blinn_phong_fragment.glsl");
	shader_manager_->createShaderProgram(shader_blend_, shaders);

	std::vector<const char*> shaders2;
	shaders2.push_back("transparent/dual_peeling_final_vertex.glsl");
	shaders2.push_back("transparent/dual_peeling_final_fragment.glsl");
	shaders2.push_back("encode_fragment.glsl");
	shader_manager_->createShaderProgram(shader_final_, shaders2);
}

void Transparent::initShaderPrograms_()
{
	quad_position_idx = 0;
	quad_texcoord_idx = 1;

	//iterate over all shaders and set the attribute locations
	QVector<const char*>::iterator i = shaders_.begin();

	while (i != shaders_.end())
	{
		shader_manager_->getShader(*i)->setAttributeLocation(quad_position_idx, "position");
		shader_manager_->getShader(*i)->setAttributeLocation(quad_texcoord_idx, "texture_coordinates");
		++i;
	}
}


void Transparent::setupRenderTargets(unsigned int w, unsigned int h)
{
	for (int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_RECTANGLE_ARB, g_dualDepthTexId[i]);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		if(GL_ARB_texture_float) {
			glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RG32F, w, h, 0, GL_RG, GL_FLOAT, 0);
		} else {
			glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_FLOAT_RG32_NV, w, h, 0, GL_RG, GL_FLOAT, 0);
		}

		glBindTexture(GL_TEXTURE_RECTANGLE_ARB, g_dualFrontBlenderTexId[i]);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_FLOAT, 0);

		glBindTexture(GL_TEXTURE_RECTANGLE_ARB, g_dualBackTempTexId[i]);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_FLOAT, 0);
	}

	glGenTextures(1, &g_dualBackBlenderTexId);
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, g_dualBackBlenderTexId);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGB, w, h, 0, GL_RGB, GL_FLOAT, 0);

	glGenFramebuffersEXT(1, &g_dualBackBlenderFboId);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, g_dualBackBlenderFboId);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
	                          GL_TEXTURE_RECTANGLE_ARB, g_dualBackBlenderTexId, 0);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, g_dualPeelingSingleFboId);

	int j = 0;
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
	                          GL_TEXTURE_RECTANGLE_ARB, g_dualDepthTexId[j], 0);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT,
	                          GL_TEXTURE_RECTANGLE_ARB, g_dualFrontBlenderTexId[j], 0);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT2_EXT,
	                          GL_TEXTURE_RECTANGLE_ARB, g_dualBackTempTexId[j], 0);

	j = 1;
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT3_EXT,
	                          GL_TEXTURE_RECTANGLE_ARB, g_dualDepthTexId[j], 0);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT4_EXT,
	                          GL_TEXTURE_RECTANGLE_ARB, g_dualFrontBlenderTexId[j], 0);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT5_EXT,
	                          GL_TEXTURE_RECTANGLE_ARB, g_dualBackTempTexId[j], 0);

	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT6_EXT,
	                          GL_TEXTURE_RECTANGLE_ARB, g_dualBackBlenderTexId, 0);

	CHECK_GL_ERRORS;

	if( glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "\n Error:: FrameBufferObject::Initialize() :: FBO loading not complete (Transparent) \n";
		std::cout << w << " " << h << "\n";
		std::cout << "Errorcode: " << glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) << " "
		          << GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT << " "
		          << GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT << " "
		          << GL_FRAMEBUFFER_UNSUPPORTED << "\n";
	}
}

void Transparent::setNormalTexture(GLuint normal_texture)
{
	normal_texture_ = normal_texture;
}

GLuint Transparent::getDepthTex()
{
	return g_dualDepthTexId[currId_];
}

GLuint Transparent::getPrevDepthTex()
{
	return g_dualDepthTexId[1 - currId_];
}

GLuint Transparent::getFrontBlenderTex()
{
	return g_dualFrontBlenderTexId[currId_];
}

GLuint Transparent::getPrevFrontBlenderTex()
{
	return g_dualFrontBlenderTexId[1 - currId_];
}

GLuint Transparent::getBackBlenderTex()
{
	return g_dualBackBlenderTexId;
}

void Transparent::setTransparentParameters(TransparentParameters p)
{
	tp_ = p;
}

TransparentParameters Transparent::getTransparentParameters()
{
	return tp_;
}
