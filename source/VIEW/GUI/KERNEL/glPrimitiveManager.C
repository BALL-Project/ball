// $Id: glPrimitiveManager.C,v 1.6 2001/05/27 10:31:24 hekl Exp $

#include <BALL/VIEW/GUI/KERNEL/glPrimitiveManager.h>

#include <math.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		GLDisplayListObject_::NoGLDisplayListsAvailable::NoGLDisplayListsAvailable(const char* file, int line)
			throw()
			:	Exception::GeneralException(file, line, string("NoGLDisplayListsAvailable"), string("memory allocation for display lists failed"))
		{
		}

		GLDisplayListObject_::WrongModes::WrongModes(const char* file, int line)
			throw()
			:	Exception::GeneralException(file, line, string("WrongModes"), string("the drawing precision or the drawing mode are not allowed."))
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

		const GLDisplayList& 
		GLDisplayListObject_::operator ()
			(unsigned int drawing_mode, unsigned int drawing_precision) const
			throw(WrongModes)
		{
			return ((GLDisplayListObject_ *)this)->operator()
								(drawing_mode, drawing_precision);
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
			throw(NoGLDisplayListsAvailable)
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

		GLDisplayList& GLSphereDisplayLists_::operator ()
			 (unsigned int drawing_mode, unsigned int drawing_precision)
			throw(WrongModes)
		{
			if (drawing_mode >= 3
					|| drawing_precision >= 4)
			{
				throw WrongModes(__FILE__, __LINE__);
			}
			
			return GL_display_list_[drawing_mode * 4 + drawing_precision];
		}

		bool GLSphereDisplayLists_::isValid() const
			throw()
		{
			if (GL_display_list_ != 0)
			{
				for (int i = 0; i < BALL_VIEW_MAXIMAL_DISPLAY_LIST_OBJECT_SIZE; ++i)
				{
					if (GL_display_list_[i].isValid() == false)
					{
						return false;
					}
				}

				return true;
			}
			
			return false;
		}

		void GLSphereDisplayLists_::dump
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

		void GLSphereDisplayLists_::create_()
			throw()
		{
			// building point display list
			GL_display_list_[0 * 4 + 0].startDefinition();
			buildDottedSphere_(1);
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
			GL_quadric_object.setNormals(GLU_SMOOTH);
			GL_quadric_object.setOrientation(GLU_OUTSIDE);

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

		void GLSphereDisplayLists_::buildDottedSphere_(int precisioni)
			throw()
		{
			Vector3 v1;
			Vector3 v2;
			Vector3 v3;

			glBegin(GL_POINTS);

			for (register int i = 0; i < 20; ++i)
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
				
				subdivideTriangle_(v1, v2, v3, precisioni);
			}

			glEnd();
		}

		void GLSphereDisplayLists_::drawPoint_
			(Vector3& v)
			throw()
		{
			/*
			glNormal3f((GLfloat)v[0], (GLfloat)v[1], (GLfloat)v[2]);
			*/
			glVertex3f((GLfloat)v[0], (GLfloat)v[1], (GLfloat)v[2]);
		}

		void GLSphereDisplayLists_::subdivideTriangle_
			(Vector3& v1, Vector3& v2, Vector3& v3, int precision)
			throw()
		{
			if (precision == 0)
			{
				Vector3 result;

				result = v1 + v2 + v3;
				result.normalize();

				drawPoint_(result);

				return;
			}

			Vector3 v12;
			Vector3 v23;
			Vector3 v31;

			v12 = v1 + v2;
			v23 = v2 + v3;
			v31 = v3 + v1;
			
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
			throw(NoGLDisplayListsAvailable)
		{
			if (GL_display_list_ == 0)
			{
				GL_display_list_ 
					= new GLDisplayList[BALL_VIEW_MAXIMAL_DISPLAY_LIST_OBJECT_SIZE]();

				if (GL_display_list_ == 0)
				{
					throw NoGLDisplayListsAvailable(__FILE__, __LINE__);
				}

				create_();
			}
		}

		GLDisplayList& GLTubeDisplayLists_::operator ()
			 (unsigned int drawing_mode, unsigned int drawing_precision)
			throw(WrongModes)
		{
			if (drawing_mode >= 3
					|| drawing_precision >= 4)
			{
				throw WrongModes(__FILE__, __LINE__);
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
					if (GL_display_list_[i].isValid() == false)
					{
						return false;
					}
				}

				return true;
			}
			
			return false;
		}

		void GLTubeDisplayLists_::dump
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
			throw(NoGLDisplayListsAvailable)
		{
			if (GL_display_list_ == 0)
			{
				GL_display_list_ 
					= new GLDisplayList[BALL_VIEW_MAXIMAL_DISPLAY_LIST_OBJECT_SIZE]();

				if (GL_display_list_ == 0)
				{
					throw NoGLDisplayListsAvailable(__FILE__, __LINE__);
				}

				create_();
			}
		}

		GLDisplayList& GLSimpleBoxDisplayLists_::operator ()
			 (unsigned int drawing_mode, unsigned int drawing_precision)
			throw(WrongModes)
		{
			if (drawing_mode >= 3
					|| drawing_precision >= 4)
			{
				throw WrongModes(__FILE__, __LINE__);
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
					if (GL_display_list_[i].isValid() == false)
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

		void GLPrimitiveManager::init()
			throw()
		{
			Sphere.init();
			Tube.init();
			SimpleBox.init();
		}
		
		void GLPrimitiveManager::clearNames()
			throw()
		{
			name_to_object_.clear();
			object_to_name_.clear();

			all_names_ = 0;
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

			GeometricObjectHashMap::Iterator it = name_to_object_.find(name);

			return it->second;
		}

		bool GLPrimitiveManager::isValid() const
			throw()
		{
			return (Sphere.isValid() &&
					    Tube.isValid()   &&
							SimpleBox.isValid());
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

		void GLPrimitiveManager::read(istream & /* s */)
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void GLPrimitiveManager::write(ostream & /* s */) const
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		/*
		::System::GLlist GLPrimitiveManager::buildSphereDisplayLists2_
			(int slices, int stacks)
		{
			int i, j;
			GLfloat theta, dtheta, rho, drho;
			GLfloat x, y, z;
			::System::GLlist GL_list = glGenLists(1);

			#ifdef BALL_DEBUG

				BALL_PRECONDITION
					(GL_list != ::System::DISPLAYLIST_NOT_DEFINED,
					 BALL_VIEW_GLPRIMITIVEMANAGER_ERROR_HANDLER
						 (GLPrimitiveManager::ERROR__NO_DISPLAY_LIST_AVAILABLE),
					 if (GL_list == ::System::DISPLAYLIST_NOT_DEFINED)
					 {
						 return ::System::DISPLAYLIST_NOT_DEFINED;
					 });

			#endif

			drho = (GLfloat)180 / (GLfloat) stacks;
			dtheta = (GLfloat)360 / (GLfloat) slices;
		 
			glNewList(GL_list, GL_COMPILE);

			// Triangle Fan
			glBegin( GL_TRIANGLE_FAN );

			glNormal3f( 0.0, 0.0, 1.0 );
			glVertex3f( 0.0, 0.0, 1.0 );

			for (j = 0; j <= slices ; ++j)
			{
				x = -sin(j * dtheta * M_PI / 180.0);
				y = cos(j * dtheta * M_PI / 180.0);
				z = cos(drho * M_PI / 180.0);

				glNormal3f(x, y, z);
				glVertex3f(x, y, z);
			}

			glEnd();

			GLfloat theta_step;
		 
			// Triangle Strip 
			// how many strips (stacks minus 2) 

			calcStrips_(drho, 2 * drho, dtheta, 0, slices);

			drho += drho;// * (stacks - 2);
			theta_step = dtheta / 2;// * (stacks & 1);

			glBegin( GL_TRIANGLE_FAN );

			glNormal3f( 0.0, 0.0, -1.0 );
			glVertex3f( 0.0, 0.0, -1.0 );

			for (j = 0; j <= slices ; ++j)
			{
				x = sin(j * dtheta * M_PI / 180.0 + theta_step * M_PI / 180.0);
				y = cos(j * dtheta * M_PI / 180.0 + theta_step * M_PI / 180.0);
				z = cos(drho * M_PI / 180.0);

				glNormal3f(x, y, z);
				glVertex3f(x, y, z);
			}

			glEnd();

			glEndList();
		 
			return GL_list;
		}


		void GLPrimitiveManager::calcStrips_
			(const GLfloat rho1, const GLfloat rho2,
			 const GLfloat theta, const GLfloat theta_step,
			 const unsigned int slices)
		{
			GLfloat x, y, z;
			unsigned int j;
			
			glBegin(GL_TRIANGLE_STRIP);
			
			for (j = 0; j <= slices ; ++j)
			{
				x = -sin(j * theta * M_PI / 180.0 + theta_step * M_PI / 180.0);
				y = cos(j * theta * M_PI / 180.0 + theta_step * M_PI / 180.0);
				z = cos(rho1 * M_PI / 180.0);
				
				glNormal3f(x, y, z);
				glVertex3f(x, y, z);
				
				x = -sin(j * theta * M_PI / 180.0 + theta / 2* M_PI / 180.0 + theta_step * M_PI / 180.0);
				y = cos(j * theta * M_PI / 180.0 + theta / 2* M_PI / 180.0 + theta_step * M_PI / 180.0);
				z = cos(rho2 * M_PI / 180.0);
				
				glNormal3f(x, y, z);
				glVertex3f(x, y, z);
			}    
			
			glEnd();
		}


		::System::GLlist GLPrimitiveManager::buildTubeDisplayLists_
			(GLUquadricObj* GLU_quadric_object,
			 int slices, int stacks)
		{
			::System::GLlist GL_list = glGenLists(1);

			BALL_PRECONDITION
				(GL_list != ::System::DISPLAYLIST_NOT_DEFINED,
				 BALL_VIEW_GLPRIMITIVEMANAGER_ERROR_HANDLER
					 (GLPrimitiveManager::ERROR__NO_DISPLAY_LIST_AVAILABLE));

			glNewList(GL_list, GL_COMPILE);
			gluCylinder(GLU_quadric_object, 1.0, 1.0, 1.0, slices, stacks);
			glEndList();

			return GL_list;
		}


		::System::GLlist GLPrimitiveManager::buildFilledTubeDisplayLists_
			(int slices, bool closed)
		{
			int i;
			::System::GLlist GL_list = glGenLists(1);

			BALL_PRECONDITION
				(GL_list != ::System::DISPLAYLIST_NOT_DEFINED,
				 BALL_VIEW_GLPRIMITIVEMANAGER_ERROR_HANDLER
					 (GLPrimitiveManager::ERROR__NO_DISPLAY_LIST_AVAILABLE));

			glNewList(GL_list, GL_COMPILE);
			glBegin( GL_QUAD_STRIP );

			GLfloat phi = (GLfloat)2.0 * M_PI / (GLfloat)slices;

			for (i = 0; i <= slices; i++)
			{
				GLfloat x = -sin((GLfloat)i * phi);
				GLfloat y = cos((GLfloat)i * phi);
				GLdouble length = sqrt( x * x + y * y );

				if (length > 0.00001F)
				{
					glNormal3f( x / length, y / length, 0.0 );   
				}

				glVertex3f( x, y, 1.0 );
				glVertex3f( x, y, 0.0 );   
			}                         
			
			glEnd();

			if (closed == true)
			{
				glBegin(GL_TRIANGLE_FAN);

				glNormal3f( 0.0, 0.0, -1.0 );   
				glVertex3f(0.0, 0.0, 0.0);

				for (i = slices; i >= 0; i--)
				{
					GLfloat x = -sin((GLfloat)i * phi);
					GLfloat y = cos((GLfloat)i * phi);

					glVertex3f( x, y, 0.0 );
				}        

				glEnd();

				glBegin(GL_TRIANGLE_FAN);

				glNormal3f( 0.0, 0.0, 1.0 );   
				glVertex3f(0.0, 0.0, 1.0);

				for (i = 0; i <= slices; i++)
				{
					GLfloat x = -sin((GLfloat)i * phi);
					GLfloat y = cos((GLfloat)i * phi);

					glVertex3f( x, y, 1.0 );
				}        

				glEnd();
			}                 

			glEndList();

			return GL_list;
		}


		::System::GLlist GLPrimitiveManager::buildConeDisplayLists_
			(GLUquadricObj* GLU_quadric_object,
			 int slices, int stacks)
		{
			::System::GLlist GL_list = glGenLists(1);

			BALL_PRECONDITION
				(GL_list != ::System::DISPLAYLIST_NOT_DEFINED,
				 BALL_VIEW_GLPRIMITIVEMANAGER_ERROR_HANDLER
					 (GLPrimitiveManager::ERROR__NO_DISPLAY_LIST_AVAILABLE));
			
			glNewList(GL_list, GL_COMPILE);
			gluCylinder(GLU_quadric_object, 1.0, 0.0, 1.0, slices, stacks);
			glEndList();

			return GL_list;
		}


		::System::GLlist GLPrimitiveManager::buildFilledConeDisplayLists_
			(int slices, bool closed)
		{
			::System::GLlist GL_list = glGenLists(1);

			BALL_PRECONDITION
				(GL_list != ::System::DISPLAYLIST_NOT_DEFINED,
				 BALL_VIEW_GLPRIMITIVEMANAGER_ERROR_HANDLER
					 (GLPrimitiveManager::ERROR__NO_DISPLAY_LIST_AVAILABLE));

			glNewList(GL_list, GL_COMPILE);

			GLfloat phi = (GLfloat)2.0 * M_PI / (GLfloat)slices;

			glBegin(GL_TRIANGLE_FAN);

			glNormal3f( 0.0, 0.0, 1.0 );   
			glVertex3f(0.0, 0.0, 1.0);
			
			for (int  = 0; i <= slices; i++)
			{
				GLfloat x = -sin((GLfloat)i * phi);
				GLfloat y = cos((GLfloat)i * phi);
				
				GLdouble length = sqrt( x * x + y * y );

				if (length > 0.00001F)
				{
					glNormal3f( x / length, y / length, 0.0 );   
				}

				glVertex3f( x, y, 0.0 );
			}        
			
			glEnd();
		 
			if (closed == true)
			{
				glBegin(GL_TRIANGLE_FAN);

				glNormal3f( 0.0, 0.0, -1.0 );   
				glVertex3f(0.0, 0.0, 0.0);

				for (int i = slices; i >= 0; i--)
				{
					GLfloat x = -sin((GLfloat)i * phi);
					GLfloat y = cos((GLfloat)i * phi);

					glVertex3f( x, y, 0.0 );
				}        

				glEnd();
			}                 

			glEndList();

			return GL_list;
		}
		*/


#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/KERNEL/glPrimitiveManager.iC>
#		endif

	} // namespace VIEW

} // namespace BALL
