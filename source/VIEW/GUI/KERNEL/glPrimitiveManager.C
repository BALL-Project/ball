// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: glPrimitiveManager.C,v 1.11 2002/12/17 15:43:48 amoll Exp $

#include <BALL/VIEW/GUI/KERNEL/glPrimitiveManager.h>
#include <BALL/VIEW/GUI/KERNEL/glQuadricObject.h>

using std::endl;
using std::ostream;
using std::istream;

namespace BALL
{
	namespace VIEW
	{

		GLDisplayListObject_::NoGLDisplayListsAvailable::NoGLDisplayListsAvailable(const char* file, int line)
			throw()
			:	Exception::GeneralException(file, line, string("NoGLDisplayListsAvailable"), string("memory allocation for display lists failed"))
		{
		}

		GLDisplayListObject_::WrongModes::WrongModes(const char* file, int line, int mode, int precision)
			throw()
			:	Exception::GeneralException(file, line, string("WrongModes"), string("the drawing precision " + String(precision) + 
																											 " or the drawing mode " + String(mode) + " are not allowed."))
		{
		}


		GLDisplayListObject_::GLDisplayListObject_()
			throw()
		{
		}

		GLDisplayListObject_::~GLDisplayListObject_()
			throw()
		{
		}


#		define BALL_VIEW_DISPLAY_LIST_MANAGER__X .525731112119133606
#		define BALL_VIEW_DISPLAY_LIST_MANAGER__Z .850650808352039932

		const float GLSphereDisplayLists_::vertices_[12][3] =
		{
			{-BALL_VIEW_DISPLAY_LIST_MANAGER__X, 0.0, BALL_VIEW_DISPLAY_LIST_MANAGER__Z},
			{ BALL_VIEW_DISPLAY_LIST_MANAGER__X, 0.0, BALL_VIEW_DISPLAY_LIST_MANAGER__Z},
			{-BALL_VIEW_DISPLAY_LIST_MANAGER__X, 0.0,-BALL_VIEW_DISPLAY_LIST_MANAGER__Z},
			{ BALL_VIEW_DISPLAY_LIST_MANAGER__X, 0.0,-BALL_VIEW_DISPLAY_LIST_MANAGER__Z},
			{ 0.0, BALL_VIEW_DISPLAY_LIST_MANAGER__Z, BALL_VIEW_DISPLAY_LIST_MANAGER__X},
			{ 0.0, BALL_VIEW_DISPLAY_LIST_MANAGER__Z,-BALL_VIEW_DISPLAY_LIST_MANAGER__X},
			{ 0.0,-BALL_VIEW_DISPLAY_LIST_MANAGER__Z, BALL_VIEW_DISPLAY_LIST_MANAGER__X},
			{ 0.0,-BALL_VIEW_DISPLAY_LIST_MANAGER__Z,-BALL_VIEW_DISPLAY_LIST_MANAGER__X},
			{ BALL_VIEW_DISPLAY_LIST_MANAGER__Z, BALL_VIEW_DISPLAY_LIST_MANAGER__X, 0.0},
			{-BALL_VIEW_DISPLAY_LIST_MANAGER__Z, BALL_VIEW_DISPLAY_LIST_MANAGER__X, 0.0},
			{ BALL_VIEW_DISPLAY_LIST_MANAGER__Z,-BALL_VIEW_DISPLAY_LIST_MANAGER__X, 0.0},
			{-BALL_VIEW_DISPLAY_LIST_MANAGER__Z,-BALL_VIEW_DISPLAY_LIST_MANAGER__X, 0.0}
		};

		const int GLSphereDisplayLists_::indices_[20][3] =
		{
			{ 0, 4, 1},
			{ 0, 9, 4},
			{ 9, 5, 4},
			{ 4, 5, 8},
			{ 4, 8, 1},
			{ 8,10, 1},
			{ 8, 3,10},
			{ 5, 3, 8},
			{ 5, 2, 3},
			{ 2, 7, 3},
			{ 7,10, 3},
			{ 7, 6,10},
			{ 7,11, 6},
			{11, 0, 6},
			{ 0, 1, 6},
			{ 6, 1,10},
			{ 9, 0,11},
			{ 9,11, 2},
			{ 9, 2, 5},
			{ 7, 2,11}
		};

		GLSphereDisplayLists_::GLSphereDisplayLists_()
			throw()
			:	GL_display_list_(0)
		{
		}

		GLSphereDisplayLists_::~GLSphereDisplayLists_()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
						 << " of class " << RTTI::getName<GLSphereDisplayLists_>() << endl;
			#endif 

			destroy();
		}

		void GLSphereDisplayLists_::destroy()
			throw()
		{
			if (GL_display_list_ != 0)
			{
				delete [] GL_display_list_;

				GL_display_list_ = 0;
			}
		}

		void GLSphereDisplayLists_::init()
			throw(GLSphereDisplayLists_::NoGLDisplayListsAvailable)
		{
			if (GL_display_list_ == 0)
			{
				GL_display_list_ = new GLDisplayList[BALL_VIEW_MAXIMAL_DISPLAY_LIST_OBJECT_SIZE]();

				if (GL_display_list_ == 0)
				{
					throw NoGLDisplayListsAvailable(__FILE__, __LINE__);
				}

				create_();
			}
		}

		GLDisplayList& GLSphereDisplayLists_::operator () (unsigned int drawing_mode, unsigned int drawing_precision)
			throw(GLSphereDisplayLists_::WrongModes)
		{
			if (drawing_mode >= 3 || drawing_precision >= 5)
			{
				throw WrongModes(__FILE__, __LINE__, drawing_mode, drawing_precision);
			}
			
			return GL_display_list_[drawing_mode * 4 + drawing_precision];
		}

		bool GLSphereDisplayLists_::isValid() const
			throw()
		{
			if (GL_display_list_ == 0) return false;
			
			for (int i = 0; i < BALL_VIEW_MAXIMAL_DISPLAY_LIST_OBJECT_SIZE; ++i)
			{
				if (!GL_display_list_[i].isValid())
				{
					return false;
				}
			}

			return true;
		}

		void GLSphereDisplayLists_::dump(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			for (int i = 0; i < BALL_VIEW_MAXIMAL_DISPLAY_LIST_OBJECT_SIZE; ++i)
			{
				GL_display_list_[i].dump(s, depth + 1);
			}

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void GLSphereDisplayLists_::create_()
			throw()
		{
			// building point display list
			GL_display_list_[0 * 4 + 0].startDefinition();
			buildDottedSphere_(1); // Precision 0 is far too evil here
			GL_display_list_[0 * 4 + 0].endDefinition();

			GL_display_list_[0 * 4 + 1].startDefinition();
			buildDottedSphere_(1);
			GL_display_list_[0 * 4 + 1].endDefinition();

			GL_display_list_[0 * 4 + 2].startDefinition();
			buildDottedSphere_(2);
			GL_display_list_[0 * 4 + 2].endDefinition();

			GL_display_list_[0 * 4 + 3].startDefinition();
			buildDottedSphere_(3);
			GL_display_list_[0 * 4 + 3].endDefinition();

			// create quadric object
			GLQuadricObject GL_quadric_object;

			GL_quadric_object.setDrawStyle(GLU_LINE);
			GL_quadric_object.setNormals(GLU_SMOOTH);
			GL_quadric_object.setOrientation(GLU_OUTSIDE);

			// building wireframe display list
			GL_display_list_[1 * 4 + 0].startDefinition();
			GL_quadric_object.drawSphere(1, 5, 3);
			GL_display_list_[1 * 4 + 0].endDefinition();
			
			GL_display_list_[1 * 4 + 1].startDefinition();
			GL_quadric_object.drawSphere(1, 6, 4);
			GL_display_list_[1 * 4 + 1].endDefinition();
			
			GL_display_list_[1 * 4 + 2].startDefinition();
			GL_quadric_object.drawSphere(1, 14, 8);
			GL_display_list_[1 * 4 + 2].endDefinition();
			
			GL_display_list_[1 * 4 + 3].startDefinition();
			GL_quadric_object.drawSphere(1, 24, 16);
			GL_display_list_[1 * 4 + 3].endDefinition();
			
			GL_quadric_object.setDrawStyle(GLU_FILL);

			// building solid display list
			GL_display_list_[2 * 4 + 0].startDefinition();
			GL_quadric_object.drawSphere(1, 5, 3);
			GL_display_list_[2 * 4 + 0].endDefinition();
			
			GL_display_list_[2 * 4 + 1].startDefinition();
			GL_quadric_object.drawSphere(1, 6, 4);
			GL_display_list_[2 * 4 + 1].endDefinition();
			
			GL_display_list_[2 * 4 + 2].startDefinition();
			GL_quadric_object.drawSphere(1, 14, 8);
			GL_display_list_[2 * 4 + 2].endDefinition();
			
			GL_display_list_[2 * 4 + 3].startDefinition();
			GL_quadric_object.drawSphere(1, 24, 16);
			GL_display_list_[2 * 4 + 3].endDefinition();
		}

		void GLSphereDisplayLists_::buildDottedSphere_(int precision)
			throw()
		{
			Vector3 v1;
			Vector3 v2;
			Vector3 v3;

			glBegin(GL_POINTS);

			for (int i = 0; i < 20; ++i)
			{
				v1.set(vertices_[indices_[i][0]][0],
							 vertices_[indices_[i][0]][1],
							 vertices_[indices_[i][0]][2]);
				
				v2.set(vertices_[indices_[i][1]][0],
							 vertices_[indices_[i][1]][1],
							 vertices_[indices_[i][1]][2]);
				
				v3.set(vertices_[indices_[i][2]][0],
							 vertices_[indices_[i][2]][1],
							 vertices_[indices_[i][2]][2]);
				
				subdivideTriangle_(v1, v2, v3, precision);
			}

			glEnd();
		}

		void GLSphereDisplayLists_::subdivideTriangle_(Vector3& v1, Vector3& v2, Vector3& v3, int precision)
			throw()
		{
			if (precision == 0)
			{
				Vector3 result = v1 + v2 + v3;
				result.normalize();

				drawPoint_(result);

				return;
			}

			Vector3 v12 = v1 + v2;
			Vector3 v23 = v2 + v3;
			Vector3 v31 = v3 + v1;
			
			v12.normalize();
			v23.normalize();
			v31.normalize();

			subdivideTriangle_(v1, v12, v31, precision - 1);
			subdivideTriangle_(v2, v23, v12, precision - 1);
			subdivideTriangle_(v3, v31, v23, precision - 1);
			subdivideTriangle_(v12, v23, v31, precision - 1);
		}



		GLTubeDisplayLists_::GLTubeDisplayLists_()
			throw()
			:	GL_display_list_(0)
		{
		}

		GLTubeDisplayLists_::~GLTubeDisplayLists_()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
						 << " of class " << RTTI::getName<GLTubeDisplayLists_>() << endl;
			#endif 

			destroy();
		}

		void GLTubeDisplayLists_::destroy()
			throw()
		{
			if (GL_display_list_ != 0)
			{
				delete [] GL_display_list_;

				GL_display_list_ = 0;
			}
		}

		void GLTubeDisplayLists_::init()
			throw(GLTubeDisplayLists_::NoGLDisplayListsAvailable)
		{
			if (GL_display_list_ == 0)
			{
				GL_display_list_ = new GLDisplayList[BALL_VIEW_MAXIMAL_DISPLAY_LIST_OBJECT_SIZE]();

				if (GL_display_list_ == 0)
				{
					throw NoGLDisplayListsAvailable(__FILE__, __LINE__);
				}

				create_();
			}
		}

		GLDisplayList& GLTubeDisplayLists_::operator ()
			 (unsigned int drawing_mode, unsigned int drawing_precision)
			throw(GLTubeDisplayLists_::WrongModes)
		{
			if (drawing_mode >= 3 || drawing_precision >= 5)
			{
				throw WrongModes(__FILE__, __LINE__, drawing_mode, drawing_precision);
			}
			
			return GL_display_list_[drawing_mode * 4 + drawing_precision];
		}

		bool GLTubeDisplayLists_::isValid() const
			throw()
		{
			if (GL_display_list_ != 0)
			{
				for (int i = 0; i < BALL_VIEW_MAXIMAL_DISPLAY_LIST_OBJECT_SIZE; ++i)
				{
					if (!GL_display_list_[i].isValid())
					{
						return false;
					}
				}

				return true;
			}
			
			return false;
		}

		void GLTubeDisplayLists_::dump(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			for (int i = 0; i < BALL_VIEW_MAXIMAL_DISPLAY_LIST_OBJECT_SIZE; ++i)
			{
				GL_display_list_[i].dump(s, depth + 1);
			}

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void GLTubeDisplayLists_::create_()
			throw()
		{
			// create quadric object
			GLQuadricObject GL_quadric_object;

			GL_quadric_object.setDrawStyle(GLU_POINT);
			GL_quadric_object.setNormals(GLU_SMOOTH);
			GL_quadric_object.setOrientation(GLU_OUTSIDE);

			// building point display list
			GL_display_list_[0 * 4 + 0].startDefinition();
			GL_quadric_object.drawCylinder(1, 1, 1, 3, 1);
			GL_display_list_[0 * 4 + 0].endDefinition();

			GL_display_list_[0 * 4 + 1].startDefinition();
			GL_quadric_object.drawCylinder(1, 1, 1, 6, 1);
			GL_display_list_[0 * 4 + 1].endDefinition();

			GL_display_list_[0 * 4 + 2].startDefinition();
			GL_quadric_object.drawCylinder(1, 1, 1, 10, 1);
			GL_display_list_[0 * 4 + 2].endDefinition();

			GL_display_list_[0 * 4 + 3].startDefinition();
			GL_quadric_object.drawCylinder(1, 1, 1, 20, 1);
			GL_display_list_[0 * 4 + 3].endDefinition();

			// building wireframe display list
			GL_quadric_object.setDrawStyle(GLU_LINE);

			GL_display_list_[1 * 4 + 0].startDefinition();
			GL_quadric_object.drawCylinder(1, 1, 1, 3, 1);
			GL_display_list_[1 * 4 + 0].endDefinition();
			
			GL_display_list_[1 * 4 + 1].startDefinition();
			GL_quadric_object.drawCylinder(1, 1, 1, 6, 1);
			GL_display_list_[1 * 4 + 1].endDefinition();
			
			GL_display_list_[1 * 4 + 2].startDefinition();
			GL_quadric_object.drawCylinder(1, 1, 1, 10, 1);
			GL_display_list_[1 * 4 + 2].endDefinition();
			
			GL_display_list_[1 * 4 + 3].startDefinition();
			GL_quadric_object.drawCylinder(1, 1, 1, 20, 1);
			GL_display_list_[1 * 4 + 3].endDefinition();
			

			// building solid display list
			GL_quadric_object.setDrawStyle(GLU_FILL);

			GL_display_list_[2 * 4 + 0].startDefinition();
			GL_quadric_object.drawCylinder(1, 1, 1, 3, 1);
			GL_display_list_[2 * 4 + 0].endDefinition();
			
			GL_display_list_[2 * 4 + 1].startDefinition();
			GL_quadric_object.drawCylinder(1, 1, 1, 6, 1);
			GL_display_list_[2 * 4 + 1].endDefinition();
			
			GL_display_list_[2 * 4 + 2].startDefinition();
			GL_quadric_object.drawCylinder(1, 1, 1, 10, 1);
			GL_display_list_[2 * 4 + 2].endDefinition();
			
			GL_display_list_[2 * 4 + 3].startDefinition();
			GL_quadric_object.drawCylinder(1, 1, 1, 20, 1);
			GL_display_list_[2 * 4 + 3].endDefinition();
		}




		GLSimpleBoxDisplayLists_::GLSimpleBoxDisplayLists_()
			throw()
			:	GL_display_list_(0)
		{
		}

		GLSimpleBoxDisplayLists_::~GLSimpleBoxDisplayLists_()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<GLSimpleBoxDisplayLists_>() << endl;
			#endif 

			destroy();
		}

		void GLSimpleBoxDisplayLists_::destroy()
			throw()
		{
			if (GL_display_list_ != 0)
			{
				delete [] GL_display_list_;

				GL_display_list_ = 0;
			}
		}

		void GLSimpleBoxDisplayLists_::init()
			throw(GLSimpleBoxDisplayLists_::NoGLDisplayListsAvailable)
		{
			if (GL_display_list_ == 0)
			{
				GL_display_list_ = new GLDisplayList[BALL_VIEW_MAXIMAL_DISPLAY_LIST_OBJECT_SIZE]();

				if (GL_display_list_ == 0)
				{
					throw NoGLDisplayListsAvailable(__FILE__, __LINE__);
				}

				create_();
			}
		}

		GLDisplayList& GLSimpleBoxDisplayLists_::operator ()
			 (unsigned int drawing_mode, unsigned int drawing_precision)
			throw(GLSimpleBoxDisplayLists_::WrongModes)
		{
			if (drawing_mode >= 3 || drawing_precision >= 5)
			{
				throw WrongModes(__FILE__, __LINE__, drawing_mode, drawing_precision);
			}
			
			return GL_display_list_[drawing_mode * 4 + drawing_precision];
		}

		bool GLSimpleBoxDisplayLists_::isValid() const
			throw()
		{
			if (GL_display_list_ != 0)
			{
				for (int i = 0; i < BALL_VIEW_MAXIMAL_DISPLAY_LIST_OBJECT_SIZE; ++i)
				{
					if (!GL_display_list_[i].isValid())
					{
						return false;
					}
				}

				return true;
			}
			
			return false;
		}

		void GLSimpleBoxDisplayLists_::dump
			(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			for (int i = 0; i < BALL_VIEW_MAXIMAL_DISPLAY_LIST_OBJECT_SIZE; ++i)
			{
				GL_display_list_[i].dump(s, depth + 1);
			}

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void GLSimpleBoxDisplayLists_::create_()
			throw()
		{
			// building point display list
			GL_display_list_[0 * 4 + 0].startDefinition();
			createDotBox_();
			GL_display_list_[0 * 4 + 0].endDefinition();

			GL_display_list_[0 * 4 + 1].startDefinition();
			createDotBox_();
			GL_display_list_[0 * 4 + 1].endDefinition();

			GL_display_list_[0 * 4 + 2].startDefinition();
			createDotBox_();
			GL_display_list_[0 * 4 + 2].endDefinition();

			GL_display_list_[0 * 4 + 3].startDefinition();
			createDotBox_();
			GL_display_list_[0 * 4 + 3].endDefinition();

			// building wireframe display list
			GL_display_list_[1 * 4 + 0].startDefinition();
			createLineBox_();
			GL_display_list_[1 * 4 + 0].endDefinition();
			
			GL_display_list_[1 * 4 + 1].startDefinition();
			createLineBox_();
			GL_display_list_[1 * 4 + 1].endDefinition();
			
			GL_display_list_[1 * 4 + 2].startDefinition();
			createLineBox_();
			GL_display_list_[1 * 4 + 2].endDefinition();
			
			GL_display_list_[1 * 4 + 3].startDefinition();
			createLineBox_();
			GL_display_list_[1 * 4 + 3].endDefinition();
			

			// building solid display list
			GL_display_list_[2 * 4 + 0].startDefinition();
			createSolidBox_();
			GL_display_list_[2 * 4 + 0].endDefinition();
			
			GL_display_list_[2 * 4 + 1].startDefinition();
			createSolidBox_();
			GL_display_list_[2 * 4 + 1].endDefinition();
			
			GL_display_list_[2 * 4 + 2].startDefinition();
			createSolidBox_();
			GL_display_list_[2 * 4 + 2].endDefinition();
			
			GL_display_list_[2 * 4 + 3].startDefinition();
			createSolidBox_();
			GL_display_list_[2 * 4 + 3].endDefinition();
		}

		void GLSimpleBoxDisplayLists_::createLineBox_()
			throw()
		{
			glBegin(GL_LINES);
			
			glVertex3f((GLfloat)0, (GLfloat)0, (GLfloat)0);
			glVertex3f((GLfloat)1, (GLfloat)0, (GLfloat)0);
			
			glVertex3f((GLfloat)1, (GLfloat)0, (GLfloat)0);
			glVertex3f((GLfloat)1, (GLfloat)0, (GLfloat)1);
			
			glVertex3f((GLfloat)1, (GLfloat)0, (GLfloat)1);
			glVertex3f((GLfloat)0, (GLfloat)0, (GLfloat)1);
			
			glVertex3f((GLfloat)0, (GLfloat)0, (GLfloat)1);
			glVertex3f((GLfloat)0, (GLfloat)0, (GLfloat)0);
			
			glVertex3f((GLfloat)0, (GLfloat)1, (GLfloat)0);
			glVertex3f((GLfloat)1, (GLfloat)1, (GLfloat)0);
			
			glVertex3f((GLfloat)1, (GLfloat)1, (GLfloat)0);
			glVertex3f((GLfloat)1, (GLfloat)1, (GLfloat)1);
			
			glVertex3f((GLfloat)1, (GLfloat)1, (GLfloat)1);
			glVertex3f((GLfloat)0, (GLfloat)1, (GLfloat)1);
			
			glVertex3f((GLfloat)0, (GLfloat)1, (GLfloat)1);
			glVertex3f((GLfloat)0, (GLfloat)1, (GLfloat)0);
			
			glVertex3f((GLfloat)0, (GLfloat)0, (GLfloat)0);
			glVertex3f((GLfloat)0, (GLfloat)1, (GLfloat)0);

			glVertex3f((GLfloat)1, (GLfloat)0, (GLfloat)0);
			glVertex3f((GLfloat)1, (GLfloat)1, (GLfloat)0);
			
			glVertex3f((GLfloat)0, (GLfloat)0, (GLfloat)1);
			glVertex3f((GLfloat)0, (GLfloat)1, (GLfloat)1);
			
			glVertex3f((GLfloat)1, (GLfloat)0, (GLfloat)1);
			glVertex3f((GLfloat)1, (GLfloat)1, (GLfloat)1);

			glEnd();
		}

		void GLSimpleBoxDisplayLists_::createDotBox_()
			throw()
		{
			glBegin(GL_POINTS);
			
			glVertex3f((GLfloat)0, (GLfloat)0, (GLfloat)0);
			glVertex3f((GLfloat)1, (GLfloat)0, (GLfloat)0);
			glVertex3f((GLfloat)1, (GLfloat)0, (GLfloat)1);
			glVertex3f((GLfloat)0, (GLfloat)0, (GLfloat)1);
			
			glVertex3f((GLfloat)0, (GLfloat)1, (GLfloat)0);
			glVertex3f((GLfloat)1, (GLfloat)1, (GLfloat)0);
			glVertex3f((GLfloat)1, (GLfloat)1, (GLfloat)1);
			glVertex3f((GLfloat)0, (GLfloat)1, (GLfloat)1);

			glEnd();
		}

		void GLSimpleBoxDisplayLists_::createSolidBox_()
			throw()
		{
			glBegin(GL_QUADS);

			// back
			glNormal3f((GLfloat)0, (GLfloat)0, (GLfloat)-1);
			glVertex3f((GLfloat)0, (GLfloat)0, (GLfloat)0);
			glVertex3f((GLfloat)0, (GLfloat)1, (GLfloat)0);
			glVertex3f((GLfloat)1, (GLfloat)1, (GLfloat)0);
			glVertex3f((GLfloat)1, (GLfloat)0, (GLfloat)0);

			// above
			glNormal3f((GLfloat)0, (GLfloat)1, (GLfloat)0);
			glVertex3f((GLfloat)0, (GLfloat)1, (GLfloat)0);
			glVertex3f((GLfloat)0, (GLfloat)1, (GLfloat)1);
			glVertex3f((GLfloat)1, (GLfloat)1, (GLfloat)1);
			glVertex3f((GLfloat)1, (GLfloat)1, (GLfloat)0);

			// front
			glNormal3f((GLfloat)0, (GLfloat)0, (GLfloat)1);
			glVertex3f((GLfloat)0, (GLfloat)0, (GLfloat)1);
			glVertex3f((GLfloat)1, (GLfloat)0, (GLfloat)1);
			glVertex3f((GLfloat)1, (GLfloat)1, (GLfloat)1);
			glVertex3f((GLfloat)0, (GLfloat)1, (GLfloat)1);

			// under
			glNormal3f((GLfloat)0, (GLfloat)-1, (GLfloat)0);
			glVertex3f((GLfloat)0, (GLfloat)0, (GLfloat)1);
			glVertex3f((GLfloat)0, (GLfloat)0, (GLfloat)0);
			glVertex3f((GLfloat)1, (GLfloat)0, (GLfloat)0);
			glVertex3f((GLfloat)1, (GLfloat)0, (GLfloat)1);

			// left
			glNormal3f((GLfloat)-1, (GLfloat)0, (GLfloat)0);
			glVertex3f((GLfloat)0, (GLfloat)1, (GLfloat)0);
			glVertex3f((GLfloat)0, (GLfloat)0, (GLfloat)0);
			glVertex3f((GLfloat)0, (GLfloat)0, (GLfloat)1);
			glVertex3f((GLfloat)0, (GLfloat)1, (GLfloat)1);

			// right
			glNormal3f((GLfloat)1, (GLfloat)0, (GLfloat)0);
			glVertex3f((GLfloat)1, (GLfloat)1, (GLfloat)1);
			glVertex3f((GLfloat)1, (GLfloat)0, (GLfloat)1);
			glVertex3f((GLfloat)1, (GLfloat)0, (GLfloat)0);
			glVertex3f((GLfloat)1, (GLfloat)1, (GLfloat)0);

			glEnd();
		}



		GLPrimitiveManager::GLPrimitiveManager()
			throw()
			:	Sphere(),
				Tube(),
				SimpleBox(),
				name_to_object_(),
				object_to_name_(),
				all_names_(0)
		{
		}
		 
		GLPrimitiveManager::~GLPrimitiveManager()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					   << " of class " << RTTI::getName<GLPrimitiveManager>() << endl;
			#endif 

			destroy();
		}

		void GLPrimitiveManager::destroy()
			throw()
		{
			Sphere.destroy();
			Tube.destroy();
			SimpleBox.destroy();

			clearNames();
		}

		GLPrimitiveManager::Name GLPrimitiveManager::getName(const GeometricObject& object)
			throw()
		{
			GeometricObject* obj = const_cast<GeometricObject*>(&object);

			NameHashMap::Iterator name_iterator = object_to_name_.find(obj);

			if (name_iterator != object_to_name_.end())
			{
				return name_iterator->second;
			}

			Name name = ++all_names_;

			object_to_name_.insert(NameHashMap::ValueType(obj, name));
			name_to_object_.insert(GeometricObjectHashMap::ValueType(name, obj));

			return name;
		}

		GeometricObject* GLPrimitiveManager::getObject(GLPrimitiveManager::Name name) const
			throw()
		{
			if (!name_to_object_.has(name))
			{
				return 0;
			}

			GeometricObjectHashMap::ConstIterator it = name_to_object_.find(name);

			return it->second;
		}

		void GLPrimitiveManager::dump(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			Sphere.dump(s, depth + 1);
			Tube.dump(s, depth + 1);
			SimpleBox.dump(s, depth + 1);
			
			BALL_DUMP_STREAM_SUFFIX(s);
		}


#ifdef BALL_NO_INLINE_FUNCTIONS
#	include <BALL/VIEW/GUI/KERNEL/glPrimitiveManager.iC>
#	endif 

	} // namespace VIEW

} // namespace BALL
