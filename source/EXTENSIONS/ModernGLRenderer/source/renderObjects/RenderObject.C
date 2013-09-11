#include <GL/glew.h>
#include <renderObjects/RenderObject.h>

RenderObject::RenderObject(Camera* camera)
	: initialized_(false),
	  drawing_mode_(BALL::VIEW::DRAWING_MODE_SOLID),
	  clipable_(true)
{
	shader_manager_ = &ShaderManager::instance();
	material_ = &Material::instance();
	camera_ = camera;
	shader_name_ = "RenderObject";
}

RenderObject::~RenderObject()
{
	cleanup_();
}

void RenderObject::initialize_()
{
	if(initialized_)
	{
		return;
	}

	initialized_ = true;
	register_();
}

bool RenderObject::isInitialized()
{
	return initialized_;
}

void RenderObject::cleanup_()
{
	if(!initialized_)
	{
		return;
	}

	unregister_();
	shader_manager_->release();

	initialized_ = false;
}

void RenderObject::setLights(const BALL::VIEW::Stage* stage, bool reset_all)
{
	QVector<const char*>::iterator i = shaders_.begin();

	while (i != shaders_.end())
	{

		setLights_(stage, reset_all, *i);
		++i;
	}
}

void RenderObject::setView(const Eigen::Affine3f& view_matrix)
{
	clipping_planes_.createFrustumClippingPlanes(view_matrix, camera_->getProjectionMatrix());
	clippingPlanesChanged();

	Eigen::Affine3f mv = Eigen::Affine3f(view_matrix);
	setUniformMatrix4fv_(QString("view_matrix"), mv.data());
}

void RenderObject::setViewInverse(const Eigen::Affine3f& view_matrix_inv)
{
	clipping_planes_.createFrustumClippingPlanes(camera_->getViewMatrix(), camera_->getProjectionMatrix());
	clippingPlanesChanged();

	Eigen::Affine3f mvi = Eigen::Affine3f(view_matrix_inv);
	setUniformMatrix4fv_(QString("view_matrix_inv"), mvi.data());
}

void RenderObject::setProjection(const Eigen::Matrix4f& projection_matrix)
{
	clipping_planes_.createFrustumClippingPlanes(camera_->getViewMatrix(), projection_matrix);
	clippingPlanesChanged();

	Eigen::Affine3f p = Eigen::Affine3f(projection_matrix);
	setUniformMatrix4fv_(QString("projection_matrix"), p.data());
}

void RenderObject::setProjectionInverse(const Eigen::Matrix4f& projection_matrix_inv)
{
	clipping_planes_.createFrustumClippingPlanes(camera_->getViewMatrix(), camera_->getProjectionMatrix());
	clippingPlanesChanged();

	Eigen::Affine3f pi = Eigen::Affine3f(projection_matrix_inv);
	setUniformMatrix4fv_(QString("projection_matrix_inv"), pi.data());
}

void RenderObject::setViewProjectionTranspose(const Eigen::Matrix4f& view_projection_matrix_transp)
{
	clipping_planes_.createFrustumClippingPlanes(camera_->getViewMatrix(), camera_->getProjectionMatrix());
	clippingPlanesChanged();

	Eigen::Affine3f vpt = Eigen::Affine3f(view_projection_matrix_transp);
	setUniformMatrix4fv_(QString("view_proj_matrix_transp"), vpt.data());
}

void RenderObject::setViewport(unsigned int w, unsigned int h, unsigned int /*x*/, unsigned int /*y*/)
{
	setUniform2f_(QString("viewport"), w, h);
}

void RenderObject::setFrustum(float left, float right, float bottom, float top)
{
	setUniform4f_(QString("frustum"), left, right, bottom, top);
}

void RenderObject::setNearDistance(float near)
{
	setUniform1f_(QString("near"), near);
}

void RenderObject::setMaterialShininess(float shininess)
{
	setUniform1f_(QString("material_shininess"), shininess);
}

void RenderObject::setMaterialDiffuse(float r, float g, float b, float a)
{
	setUniform4f_(QString("material_diffuse"), r, g, b, a);
}

void RenderObject::setMaterialAmbient(float r, float g, float b, float a)
{
	setUniform4f_(QString("material_ambient"), r, g, b, a);
}

void RenderObject::setMaterialSpecular(float r, float g, float b, float a)
{
	setUniform4f_(QString("material_specular"), r, g, b, a);
}

void RenderObject::setUniform4fv_(QString uniform_name, int count, float* v1)
{
	QVector<const char*>::iterator i = shaders_.begin();

	while (i != shaders_.end())
	{
		shader_manager_->getShader(*i)->setUniform4fv(uniform_name, count, v1);
		++i;
	}
}

void RenderObject::setUniform1f_(QString uniform_name, float v1)
{
	QVector<const char*>::iterator i = shaders_.begin();

	while (i != shaders_.end())
	{
		shader_manager_->getShader(*i)->setUniform1f(uniform_name, v1);
		++i;
	}
}

void RenderObject::setUniform2f_(QString uniform_name, float v1, float v2)
{
	QVector<const char*>::iterator i = shaders_.begin();

	while (i != shaders_.end())
	{
		shader_manager_->getShader(*i)->setUniform2f(uniform_name, v1, v2);
		++i;
	}
}

void RenderObject::setUniform3f_(QString uniform_name, float v1, float v2, float v3)
{
	QVector<const char*>::iterator i = shaders_.begin();

	while (i != shaders_.end())
	{
		shader_manager_->getShader(*i)->setUniform3f(uniform_name, v1, v2, v3);
		++i;
	}
}

void RenderObject::setUniform4f_(QString uniform_name, float v1, float v2, float v3, float v4)
{
	QVector<const char*>::iterator i = shaders_.begin();

	while (i != shaders_.end())
	{
		shader_manager_->getShader(*i)->setUniform4f(uniform_name, v1, v2, v3, v4);
		++i;
	}
}

void RenderObject::setUniform1i_(QString uniform_name, int v1)
{
	QVector<const char*>::iterator i = shaders_.begin();

	while (i != shaders_.end())
	{
		shader_manager_->getShader(*i)->setUniform1i(uniform_name, v1);
		++i;
	}
}

void RenderObject::setUniform1b_(QString uniform_name, bool v1)
{
	QVector<const char*>::iterator i = shaders_.begin();

	while (i != shaders_.end())
	{
		shader_manager_->getShader(*i)->setUniform1b(uniform_name, v1);
		++i;
	}
}

void RenderObject::setUniformMatrix4fv_(QString uniform_name, float* v1)
{
	QVector<const char*>::iterator i = shaders_.begin();

	while (i != shaders_.end())
	{
		shader_manager_->getShader(*i)->setUniformMatrix4fv(uniform_name, v1);
		++i;
	}
}

bool RenderObject::setClippingPlanes(const std::vector< Eigen::Vector4f>& clippingPlanes)
{
	bool r = clipping_planes_.addClippingPlanes(clippingPlanes);
	clippingPlanesChanged();
	return r;
}

bool RenderObject::setClippingPlane ( const Eigen::Vector4f& clipping_plane )
{
	bool r = clipping_planes_.addClippingPlane(clipping_plane);
	clippingPlanesChanged();
	return r;
}

bool RenderObject::clearClippingPlanes()
{
	bool r =  clipping_planes_.clear();
	clippingPlanesChanged();
	return r;
}

void RenderObject::clippingPlanesChanged()
{
	setUniform4fv_(QString("clipping_planes[0]"), ClippingPlanes::MAX_CLIPPING_PLANES, clipping_planes_.getClippingPlanes()); //for nvidia
	setUniform4fv_(QString("clipping_planes"), ClippingPlanes::MAX_CLIPPING_PLANES, clipping_planes_.getClippingPlanes()); //for intel
	setUniform1i_(QString("used_clipping_planes"), clipping_planes_.countClippingPlanes());
}

void RenderObject::setClipRange ( ClipRange clipRange )
{
	setUniform4f_(QString("clip_range"), clipRange.near, clipRange.x_range, clipRange.y_range, clipRange.far);
}

void RenderObject::setDepthBlenderTex(GLuint tex)
{
	depth_tex_idx = tex;
}

void RenderObject::setFrontBlenderTex(GLuint tex)
{
	front_blender_tex_idx = tex;
}

void RenderObject::setBackBlenderTex ( GLuint tex )
{
	back_blender_tex_idx = tex;
}

void RenderObject::init ()
{
	cleanup_();
	initialize_();
}

void RenderObject::init ( std::list< const BALL::VIEW::GeometricObject* >& list )
{
	init();
	initVBO_(list);
}

void RenderObject::setCamera ( Camera* camera )
{
	camera_ = camera;
}

void RenderObject::setDrawingMode (BALL::VIEW::DrawingMode drawing_mode)
{
	drawing_mode_ = drawing_mode;
}

BALL::VIEW::DrawingMode RenderObject::getDrawingMode()
{
	return drawing_mode_;
}

GLuint RenderObject::getGLDrawingMode()
{
	switch (drawing_mode_)
	{
		case BALL::VIEW::DRAWING_MODE_SOLID:
			return GL_FILL;

		case BALL::VIEW::DRAWING_MODE_DOTS:
			return GL_POINT;

		case BALL::VIEW::DRAWING_MODE_WIREFRAME:
			return GL_LINE;
		default:
			return GL_FILL;
	}
}

void RenderObject::setClipable( bool clipable)
{
	clipable_ = clipable;
	setUniform1i_(QString("clipable"), clipable_?1:0);
}

void RenderObject::setLights_(const BALL::VIEW::Stage* stage, bool reset_all, const char* shader_name)
{
	Shader* shader = shader_manager_->getShader(shader_name);
	shader->setUniform4f("scene_ambient", 0.1, 0.1, 0.1, 1);

	if(reset_all)
	{
		//nothing to do here
	}

	BALL::Vector3 direction, light_pos, attenuation;

	size_t current_light = 0;
	size_t num_lights = stage->getLightSources().size();

	shader->setUniform1f("number_of_lights", std::min(MAX_NUMBER_OF_LIGHTS,(int) num_lights));

	std::list<BALL::VIEW::LightSource>::const_iterator it = stage->getLightSources().begin();

	std::stringstream sstm;
	std::string s;

	BALL::VIEW::ColorRGBA ambient_color;


	// iterate over lightsources
	// renderer needs lightsources in view space. Thus, transform absolute positioned lightsources to
	// relative coordinates.
	for (; it != stage->getLightSources().end(); ++it, ++current_light)
	{
		if(current_light < MAX_NUMBER_OF_LIGHTS)
		{
			switch (it->getType())
			{
				case BALL::VIEW::LightSource::DIRECTIONAL:
					direction = it->getDirection();

					if (!it->isRelativeToCamera())
					{
						direction = stage->calculateRelativeCoordinates(direction);
					}

					sstm << "lights[" << current_light << "].position";
					sstm >> s;
					sstm.clear();
					shader->setUniform4f(s.c_str(), direction.x, direction.y, direction.z, 0.);
					break;

				case BALL::VIEW::LightSource::POSITIONAL:

					light_pos = it->getPosition();

					if (!it->isRelativeToCamera())
					{
						light_pos = stage->calculateRelativeCoordinates(light_pos);
					}

					light_pos *= Global::getScaleFactor();

					sstm << "lights[" << current_light << "].position";
					sstm >> s;
					sstm.clear();
					shader->setUniform4f(s.c_str(), light_pos.x, light_pos.y, -light_pos.z, 1.);

					attenuation = it->getAttenuation();

					sstm << "lights[" << current_light << "].constant_attenuation";
					sstm >> s;
					sstm.clear();
					shader->setUniform1f(s.c_str(), attenuation.x);

					sstm << "lights[" << current_light << "].linear_attenuation";
					sstm >> s;
					sstm.clear();
					shader->setUniform1f(s.c_str(), attenuation.y);

					sstm << "lights[" << current_light << "].quadratic_attenuation";
					sstm >> s;
					sstm.clear();
					shader->setUniform1f(s.c_str(), attenuation.z);
					break;

				case BALL::VIEW::LightSource::AMBIENT:
					BALL::VIEW::ColorRGBA const& c = it->getColor();
					float i = it->getIntensity();
					ambient_color.setRed  ((float)ambient_color.getRed()   + (float)c.getRed()   * i);
					ambient_color.setGreen((float)ambient_color.getGreen() + (float)c.getGreen() * i);
					ambient_color.setBlue ((float)ambient_color.getBlue()  + (float)c.getBlue()  * i);
					ambient_color.setAlpha((float)ambient_color.getAlpha() + (float)c.getAlpha() * i);
					break;
					//        default:
					//          std::cerr << "Light source type not supported!" << std::endl;
					//          break;
			}

			float intensity = it->getIntensity();
			BALL::VIEW::ColorRGBA const& color = it->getColor();

			sstm << "lights[" << current_light << "].diffuse";
			sstm >> s;
			sstm.clear();

			float r = (float)color.getRed()   * intensity;
			float g = (float)color.getGreen() * intensity;
			float b = (float)color.getBlue()  * intensity;
			float a = (float)color.getAlpha() * intensity;
			shader->setUniform4f(s.c_str(), r, g, b, a);

			sstm << "lights[" << current_light << "].specular";
			sstm >> s;
			sstm.clear();
			shader->setUniform4f(s.c_str(), r, g, b, a);

		}
		else
		{
			BALL::Log << "Only up to " << MAX_NUMBER_OF_LIGHTS << " light sources are supported by this renderer." << std::endl;
			break;
		}
	}

	float r = (float)ambient_color.getRed();
	float g = (float)ambient_color.getGreen();
	float b = (float)ambient_color.getBlue();
	float a = (float)ambient_color.getAlpha();
	shader->setUniform4f("ambient_light", r, g, b, a);
}
