// $Id: glmesh.C,v 1.3 2000/12/22 19:12:17 amoll Exp $

#include <BALL/VIEW/GUI/PRIMITIV/glmesh.h>
#include <GL/gl.h>


namespace BALL
{

	namespace VIEW
	{

		GLMesh::GLMesh()
			:	Mesh(),
				GLObject()
		{
		}

		GLMesh::GLMesh(const GLMesh& GL_mesh, bool deep)
			:	Mesh(GL_mesh, deep),
				GLObject(GL_mesh, deep)
		{
		}

		GLMesh::GLMesh(const GeometricObject& geometric_object)
			:	Mesh(geometric_object),
				GLObject()
		{
		}

		GLMesh::~GLMesh()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<GLMesh>() << endl;
			#endif 

			destroy();
		}

		void GLMesh::clear()
			throw()
		{
			Mesh::clear();
			GLObject::clear();
		}

		void GLMesh::destroy()
			throw()
		{
			Mesh::destroy();
			GLObject::destroy();
		}

		bool GLMesh::draw(bool with_names)
		{
			if (hasProperty(GeometricObject::PROPERTY__OBJECT_HIDDEN) == true)
			{
				return true;
			}

			if (isSelected() == false)
			{
				glColor4ub((unsigned char)getColor().red(),
									 (unsigned char)getColor().green(),
									 (unsigned char)getColor().blue(),
									 (unsigned char)getColor().alpha());
			}
			else
			{
				glColor4ub((unsigned char)getSelectedColor().red(),
									 (unsigned char)getSelectedColor().green(),
									 (unsigned char)getSelectedColor().blue(),
									 (unsigned char)getSelectedColor().alpha());
			}

			if (with_names)
			{
				glLoadName(getGLPrimitiveManager()->getName(*this));
			}

			glPushMatrix();

			glColor4ub(255,255,255,255);
			
			unsigned int precision;
			unsigned int mode;

			getDrawingModeAndPrecision(mode, precision);

			Triangle t;
			Vector3 v, n;

			if (mode == GeometricObject::PROPERTY__DRAWING_MODE_DOTS)
			{
				glBegin(GL_POINTS);
					
				// draw the triangles with lines
				for (Size index = 0; index < triangle.size(); ++index)
				{
					t = triangle[index];
					
					v = vertex[t.v1];
					
					glVertex3f((GLfloat)v.x, (GLfloat)v.y, (GLfloat)v.z);
					/*
					v = vertex[t.v2];
					
					glVertex3f((GLfloat)v.x, (GLfloat)v.y, (GLfloat)v.z);
					
					v = vertex[t.v3];

					glVertex3f((GLfloat)v.x, (GLfloat)v.y, (GLfloat)v.z);
					*/
				}

				glEnd();
				// ------------------
			}
			else if (mode == GeometricObject::PROPERTY__DRAWING_MODE_WIREFRAME)
			{
				// draw the triangles with lines
				for (Size index = 0; index < triangle.size(); ++index)
				{
					glBegin(GL_LINE_STRIP);
					
					t = triangle[index];
					
					v = vertex[t.v1];
					
					glVertex3f((GLfloat)v.x, (GLfloat)v.y, (GLfloat)v.z);
					
					v = vertex[t.v2];
					
					glVertex3f((GLfloat)v.x, (GLfloat)v.y, (GLfloat)v.z);
					
					v = vertex[t.v3];

					glVertex3f((GLfloat)v.x, (GLfloat)v.y, (GLfloat)v.z);
					
					glEnd();
				}
				// ------------------
			}
			else
			{
				// draw the triangles
				for (Size index = 0; index < triangle.size(); ++index)
				{
					glBegin(GL_TRIANGLES);
					
					t = triangle[index];
					
					v = vertex[t.v1];
					n = normal[t.v1];
					
					glNormal3f((GLfloat)n.x, (GLfloat)n.y, (GLfloat)n.z);
					glVertex3f((GLfloat)v.x, (GLfloat)v.y, (GLfloat)v.z);
					
					v = vertex[t.v2];
					n = normal[t.v2];
					
					glNormal3f((GLfloat)n.x, (GLfloat)n.y, (GLfloat)n.z);
					glVertex3f((GLfloat)v.x, (GLfloat)v.y, (GLfloat)v.z);
					
					v = vertex[t.v3];
					n = normal[t.v3];
					
					glNormal3f((GLfloat)n.x, (GLfloat)n.y, (GLfloat)n.z);
					glVertex3f((GLfloat)v.x, (GLfloat)v.y, (GLfloat)v.z);
					
					glEnd();
				}
				// ------------------
			}

			glPopMatrix();

			return true;
		}

		bool GLMesh::drawUserDefined()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		bool GLMesh::extract()
			throw()
		{
			return Mesh::extract();
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/PRIMITIV/glmesh.iC>
#		endif

	} // namespace VIEW

} // namespace BALL
