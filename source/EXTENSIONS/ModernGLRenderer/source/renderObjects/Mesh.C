#include <GL/glew.h>

#include <renderObjects/Mesh.h>
#include <Global.h>

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/STRUCTURE/triangulatedSurface.h>
#include <BALL/STRUCTURE/geometricProperties.h>

#include <BALL/STRUCTURE/surfaceProcessor.h>
#include <BALL/MATHS/surface.h>

Mesh::Mesh(Camera* camera)
	: Geometry(camera)
{
	vbo_ = new VBOTriangle;
	shader_name_ = "mesh";
	transparency_init_ = "mesh_transparency_init";
	transparency_peel_ = "mesh_transparency_peel";

	compileShaderPrograms_();

	shaders_.push_back(shader_name_);

	if(Global::getTransparencySupport())
	{
		shaders_.push_back(transparency_init_);
		shaders_.push_back(transparency_peel_);
	}

	initShaderPrograms_();
}

void Mesh::register_()
{
	camera_->registerViewMatrix(this);
	camera_->registerProjectionMatrix(this);
	camera_->registerClipRange(this);
	material_->registerMaterial(this);
}

void Mesh::unregister_()
{
	camera_->unregisterViewMatrix(this);
	camera_->unregisterProjectionMatrix(this);
	camera_->unregisterClipRange(this);
	material_->unregisterMaterial(this);
}

void Mesh::render_()
{
	glPolygonMode(GL_FRONT_AND_BACK, getGLDrawingMode());
	vbo_->render();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Mesh::compileShaderPrograms_()
{
	std::vector<const char*> shaders;
	shaders.push_back("mesh/mesh_vertex.glsl");
	shaders.push_back("opaque/opaque_vertex.glsl");
	shaders.push_back("encode_fragment.glsl");
	shaders.push_back("opaque/opaque_fragment.glsl");
	shaders.push_back("mesh/mesh_fragment.glsl");
	shaders.push_back("clipping_plane_fragment.glsl");
	shader_manager_->createShaderProgram(shader_name_, shaders);

	if(Global::getTransparencySupport())
	{
		std::vector<const char*> shaders;
		shaders.push_back("mesh/mesh_vertex.glsl");
		shaders.push_back("transparent/dual_peeling_init_vertex.glsl");
		shaders.push_back("transparent/dual_peeling_init_fragment.glsl");
		shaders.push_back("mesh/mesh_fragment.glsl");
		shaders.push_back("clipping_plane_fragment.glsl");
		shader_manager_->createShaderProgram(transparency_init_, shaders);

		std::vector<const char*> shaders2;
		shaders2.push_back("mesh/mesh_vertex.glsl");
		shaders2.push_back("compositing/blinn_phong_fragment.glsl");
		shaders2.push_back("transparent/dual_peeling_peel_vertex.glsl");
		shaders2.push_back("transparent/dual_peeling_peel_fragment.glsl");
		shaders2.push_back("mesh/mesh_fragment.glsl");
		shaders2.push_back("clipping_plane_fragment.glsl");
		shader_manager_->createShaderProgram(transparency_peel_, shaders2);
	}
}

void Mesh::initShaderPrograms_()
{
	vertex_position_idx = 0;
	vertex_normal_idx   = 1;
	color_idx           = 2;

	//iterate over all shaders and set the attribute locations
	QVector<const char*>::iterator i = shaders_.begin();

	while (i != shaders_.end())
	{
		shader_manager_->getShader(*i)->setAttributeLocation(vertex_position_idx, "vertex_position");
		shader_manager_->getShader(*i)->setAttributeLocation(vertex_normal_idx  , "vertex_normal");
		shader_manager_->getShader(*i)->setAttributeLocation(color_idx          , "color");
		++i;
	}
}

void Mesh::initVBO_ ( std::list< const BALL::VIEW::GeometricObject* >& list )
{
	unsigned int n_triangles = 0;
	unsigned int n_vertices = 0;

	std::list<const BALL::VIEW::GeometricObject*>::const_iterator it;

	for (it =  list.begin(); it != list.end(); it++)
	{
		const BALL::VIEW::Mesh* mesh = dynamic_cast<const BALL::VIEW::Mesh*>(*it);

		if(mesh)
		{
			n_triangles += mesh->getNumberOfTriangles();
			n_vertices += mesh->getNumberOfVertices();
		}
	}

	std::vector<Attrib3>  verts(n_vertices);
	std::vector<Attrib3> normal(n_vertices);
	std::vector<Attrib4>  color(n_vertices);
	std::vector<Index> indxs(n_triangles);

	unsigned int index = 0;
	unsigned int vertex = 0;

	for (it =  list.begin(); it != list.end(); it++)
	{
		const BALL::VIEW::Mesh* mesh = dynamic_cast<const BALL::VIEW::Mesh*>(*it);

		if(mesh)
		{
			bool multiple_colors = true;
			BALL::VIEW::ColorRGBA unique_col;

			if (mesh->colors.size() < mesh->vertex.size())
			{
				if (mesh->colors.size() > 0)
				{
					unique_col = mesh->colors.at(0);
				}

				multiple_colors = false;
			}

			for(unsigned int i = 0; i < mesh->getNumberOfTriangles(); ++i, ++index)
			{
				BALL::TSurface< float >::Triangle tri = mesh->getTriangle(i);

				indxs[index].a = vertex + tri.v1;
				indxs[index].b = vertex + tri.v2;
				indxs[index].c = vertex + tri.v3;
			}

			for(unsigned int i = 0; i < mesh->getNumberOfVertices(); ++i, ++vertex)
			{
				BALL::TSurface< float >::Vertex v = mesh->getVertex(i);
				BALL::TSurface< float >::Normal n = mesh->getNormal(i);
				BALL::VIEW::ColorRGBA           c;

				if(multiple_colors)
				{
					c = mesh->colors.at(i);
				}
				else
				{
					c = unique_col;
				}

				BALL::Vector3 p = v * Global::getScaleFactor();

				// increase size of line model a little bit to be drawn onto a solid model
				if(getGLDrawingMode() == GL_LINE)
				{
					p += p*0.002  ;
				}

				verts [vertex].a = p.x;
				verts[vertex].b = p.y;
				verts[vertex].c = p.z;
				normal[vertex].a = n.x;
				normal[vertex].b = n.y;
				normal[vertex].c = n.z;
				color [vertex].a = c.getRed();
				color[vertex].b = c.getGreen();
				color[vertex].c = c.getBlue();
				color[vertex].d = c.getAlpha();
			}
		}
	}

	vbo_->setNumberOfElements(n_triangles * 3);
	vbo_->setAttribute3f(vertex_position_idx,verts);
	vbo_->setAttribute3f(vertex_normal_idx,normal);
	vbo_->setAttribute4f(color_idx,color);

	vbo_->setIndices(indxs);
}
