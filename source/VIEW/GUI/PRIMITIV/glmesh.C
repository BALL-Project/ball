// $Id: glmesh.C,v 1.5.4.2 2002/09/25 15:03:00 anhi Exp $

#include <BALL/VIEW/GUI/PRIMITIV/glmesh.h>
#include <GL/gl.h>


namespace BALL
{

	namespace VIEW
	{

		GLMesh::GLMesh()
			throw()
			:	Mesh(),
				GLObject()
		{
		}

		GLMesh::GLMesh(const GLMesh& mesh, bool deep)
			throw()
			:	Mesh(mesh, deep),
				GLObject(mesh)
		{
		}

		GLMesh::GLMesh(const GeometricObject& geometric_object)
			throw()
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

		void GLMesh::set(const GLMesh& mesh, bool deep)
			throw()
		{
			Mesh::set(mesh, deep);
			GLObject::set(mesh);
		}

		const GLMesh& GLMesh::operator = (const GLMesh& mesh)
			throw()
		{
			set(mesh);
			return *this;
		}

		void GLMesh::get(GLMesh& mesh, bool deep) const
			throw()
		{
			mesh.set(*this, deep);
		}

		void GLMesh::swap(GLMesh& mesh)
			throw()
		{
			Mesh::swap(mesh);
			GLObject::swap(mesh);
		}

		bool GLMesh::draw(bool with_names)
			throw()
		{
			if (hasProperty(GeometricObject::PROPERTY__OBJECT_HIDDEN) == true)
			{
				return true;
			}

			if (isSelected() == false)
			{
				glColor4ub((unsigned char)getColor().getRed(),
									 (unsigned char)getColor().getGreen(),
									 (unsigned char)getColor().getBlue(),
									 (unsigned char)getColor().getAlpha());
			}
			else
			{
				glColor4ub((unsigned char)getSelectedColor().getRed(),
									 (unsigned char)getSelectedColor().getGreen(),
									 (unsigned char)getSelectedColor().getBlue(),
									 (unsigned char)getSelectedColor().getAlpha());
			}

			if (with_names)
			{
				glLoadName(getGLPrimitiveManager()->getName(*this));
			}

			glPushMatrix();
			// If we have only one color for the whole mesh, this can
			// be assigned efficiently
			if (colorList.size() < vertex.size())
			{	
				if (colorList.size() == 0)
				{
					glColor4ub(255,255,255,255);
				}
				else
				{
					glColor4ub((unsigned char)colorList[0].getRed(),
										 (unsigned char)colorList[0].getGreen(),
										 (unsigned char)colorList[0].getBlue(),
										 (unsigned char)colorList[0].getAlpha());
				}
					
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
			}
			else // we have a different color for each vertex
			{	
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
						// set the color
						ColorRGBA col = colorList[t.v1];
						glColor4ub((unsigned char)col.getRed(),
	 						 				 (unsigned char)col.getGreen(),
											 (unsigned char)col.getBlue(),
											 (unsigned char)col.getAlpha());
		
						glVertex3f((GLfloat)v.x, (GLfloat)v.y, (GLfloat)v.z);
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

						ColorRGBA col = colorList[t.v1];
						glColor4ub((unsigned char)col.getRed(),
	 						 				 (unsigned char)col.getGreen(),
											 (unsigned char)col.getBlue(),
											 (unsigned char)col.getAlpha());
	
						glVertex3f((GLfloat)v.x, (GLfloat)v.y, (GLfloat)v.z);

						v = vertex[t.v2];
						
						col = colorList[t.v2];
						glColor4ub((unsigned char)col.getRed(),
	 						 				 (unsigned char)col.getGreen(),
											 (unsigned char)col.getBlue(),
											 (unsigned char)col.getAlpha());
	
						glVertex3f((GLfloat)v.x, (GLfloat)v.y, (GLfloat)v.z);

						v = vertex[t.v3];

						col = colorList[t.v3];
						glColor4ub((unsigned char)col.getRed(),
	 						 				 (unsigned char)col.getGreen(),
											 (unsigned char)col.getBlue(),
											 (unsigned char)col.getAlpha());
	
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
						ColorRGBA col = colorList[t.v1];
						glColor4ub((unsigned char)col.getRed(),
	 						 				 (unsigned char)col.getGreen(),
											 (unsigned char)col.getBlue(),
											 (unsigned char)col.getAlpha());
	
						glNormal3f((GLfloat)n.x, (GLfloat)n.y, (GLfloat)n.z);
						glVertex3f((GLfloat)v.x, (GLfloat)v.y, (GLfloat)v.z);

						v = vertex[t.v2];
						n = normal[t.v2];
						col = colorList[t.v2];
						glColor4ub((unsigned char)col.getRed(),
	 						 				 (unsigned char)col.getGreen(),
											 (unsigned char)col.getBlue(),
											 (unsigned char)col.getAlpha());
	
						glNormal3f((GLfloat)n.x, (GLfloat)n.y, (GLfloat)n.z);
						glVertex3f((GLfloat)v.x, (GLfloat)v.y, (GLfloat)v.z);

						v = vertex[t.v3];
						n = normal[t.v3];
						col = colorList[t.v3];
						glColor4ub((unsigned char)col.getRed(),
	 						 				 (unsigned char)col.getGreen(),
											 (unsigned char)col.getBlue(),
											 (unsigned char)col.getAlpha());
	
						glNormal3f((GLfloat)n.x, (GLfloat)n.y, (GLfloat)n.z);
						glVertex3f((GLfloat)v.x, (GLfloat)v.y, (GLfloat)v.z);

						glEnd();
					}
					// ------------------
				}
			}
			glPopMatrix();

			return true;
		}

		bool GLMesh::drawUserDefined()
			throw()
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
