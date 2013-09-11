#include <GL/glew.h>

#include <renderObjects/PointVolume.h>
#include <Global.h>

#include <BALL/STRUCTURE/geometricProperties.h>

#include <BALL/VIEW/MODELS/standardColorProcessor.h>
#include <BALL/VIEW/PRIMITIVES/gridVisualisation.h>

#define BYTES_PER_TEXEL 4

PointVolume::PointVolume(Camera* camera)
	: Geometry(camera)
{
	vbo_ = new VBOPoint;
	shader_name_ = "pointvolume";
	transparency_init_ = "pointvolume_transparency_init";
	transparency_peel_ = "pointvolume_transparency_peel";

	compileShaderPrograms_();

	shaders_.push_back(shader_name_);

	if(Global::getTransparencySupport())
	{
		shaders_.push_back(transparency_init_);
		shaders_.push_back(transparency_peel_);
	}

	initShaderPrograms_();
}

void PointVolume::initialize_()
{
	Geometry::initialize_();

	glEnable(GL_POINT_SPRITE);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
}

void PointVolume::register_()
{
	camera_->registerViewMatrix(this);
	camera_->registerProjectionMatrix(this);
	camera_->registerClipRange(this);
}

void PointVolume::unregister_()
{
	camera_->unregisterViewMatrix(this);
	camera_->unregisterProjectionMatrix(this);
	camera_->unregisterClipRange(this);
}

//here starts the rendering
void PointVolume::render_()
{
	vbo_->render();
}

void PointVolume::bindTexturesTransparencyPeel_()
{
	shader_manager_->getShader(transparency_peel_)->bindTexture3D("volume_tex", volume_texture_, 2);
}

void PointVolume::bindTexturesOpaque_()
{
	shader_manager_->getShader(shader_name_)->bindTexture3D("volume_tex", volume_texture_, 2);
}

void PointVolume::createTextureFromGrid_(const BALL::VIEW::GridVisualisation* vis)
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

	BALL::Log.info() << "[Volume] Volume texture created" << std::endl;
}

void PointVolume::compileShaderPrograms_()
{
	std::vector<const char*> shaders;
	shaders.push_back("volume/point_vertex.glsl");
	shaders.push_back("volume/point_fragment.glsl");
	shaders.push_back("opaque/opaque_vertex.glsl");
	shaders.push_back("opaque/opaque_fragment.glsl");
	shaders.push_back("clipping_plane_vertex.glsl");
	shaders.push_back("clipping_plane_fragment.glsl");
	shaders.push_back("encode_fragment.glsl");
	shader_manager_->createShaderProgram(shader_name_, shaders);

	if(Global::getTransparencySupport())
	{
		std::vector<const char*> shaders;
		shaders.push_back("volume/point_vertex.glsl");
		shaders.push_back("transparent/dual_peeling_init_vertex.glsl");
		shaders.push_back("transparent/dual_peeling_init_fragment.glsl");
		shaders.push_back("volume/point_fragment.glsl");
		shaders.push_back("clipping_plane_fragment.glsl");
		shader_manager_->createShaderProgram(transparency_init_, shaders);

		std::vector<const char*> shaders2;
		shaders2.push_back("volume/point_vertex.glsl");
		shaders2.push_back("compositing/blinn_phong_fragment.glsl");
		shaders2.push_back("transparent/dual_peeling_peel_vertex.glsl");
		shaders2.push_back("transparent/dual_peeling_peel_fragment.glsl");
		shaders2.push_back("volume/point_fragment.glsl");
		shaders2.push_back("clipping_plane_fragment.glsl");
		shader_manager_->createShaderProgram(transparency_peel_, shaders2);
	}
}

void PointVolume::initShaderPrograms_()
{
	midpoint_idx   = 0;
	texcoords_idx  = 1;
	radius_idx     = 2;

	//iterate over all shaders and set the attribute locations
	QVector<const char*>::iterator i = shaders_.begin();

	while (i != shaders_.end())
	{
		shader_manager_->getShader(*i)->setAttributeLocation(midpoint_idx , "midpoint");
		shader_manager_->getShader(*i)->setAttributeLocation(texcoords_idx, "texcoords");
		shader_manager_->getShader(*i)->setAttributeLocation(radius_idx   , "radius");
		++i;
	}
}

void PointVolume::initVBO_ ( std::list< const BALL::VIEW::GeometricObject* >& list )
{

	//TODO currently we only support one GeometricObject per representation

	size_t num_elements = 0;

	//find the first geometric object which is a grid visualisation
	std::list<const BALL::VIEW::GeometricObject*>::const_iterator it;

	for (it =  list.begin(); it != list.end(); it++)
	{
		const BALL::VIEW::GridVisualisation* grid = dynamic_cast<const BALL::VIEW::GridVisualisation*>(*it);

		if(grid)
		{
			if(num_elements)
			{
				BALL::Log.error() << "[PointVolume] Only one volume per representation is supported." << std::endl;
				break;
			}

			num_elements += grid->points.size();
		}
	}

	std::vector<Attrib3> verts(num_elements);
	std::vector<Attrib3> texcoords(num_elements);
	std::vector<Attrib1> radius(num_elements);

	int ac = 0;

	for (it =  list.begin(); it != list.end(); it++)
	{
		const BALL::VIEW::GridVisualisation* grid = dynamic_cast<const BALL::VIEW::GridVisualisation*>(*it);

		if(grid)
		{
			//we have found a grid visualisation previously, now stop
			if(ac)
			{
				break;
			}

			createTextureFromGrid_(grid);

			BALL::Vector3 o = grid->origin * Global::getScaleFactor();
			BALL::Vector3 x = grid->x * Global::getScaleFactor();
			BALL::Vector3 y = grid->y * Global::getScaleFactor();
			BALL::Vector3 z = grid->z * Global::getScaleFactor();

			for(unsigned int i = 0; i < grid->points.size(); i++, ++ac)
			{
				BALL::Vector3 p = grid->points[i]*Global::getScaleFactor();

				BALL::Vector3 t;
				t.x = fabs((o.x-p.x) / x.x);
				t.y = fabs((o.y-p.y) / y.y);
				t.z = fabs((o.z-p.z) / z.z);

				verts    [ac].a = p.x;
				verts    [ac].b = p.y;
				verts    [ac].c = p.z;
				texcoords[ac].a = t.x;
				texcoords[ac].b = t.y;
				texcoords[ac].c = t.z;
				radius[ac].a = grid->getDotSize() /** Global::getScaleFactor()*/;
			}
		}
	}

	vbo_->setNumberOfElements(num_elements);
	vbo_->setAttribute3f(midpoint_idx,verts);
	vbo_->setAttribute3f(texcoords_idx,texcoords);
	vbo_->setAttribute1f(radius_idx,radius);
}