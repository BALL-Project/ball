// $Id: glPrimitiveManager.h,v 1.3 2000/09/23 14:15:04 hekl Exp $

#ifndef BALL_VIEW_GUI_KERNEL_GLPRIMITIVEMANAGER_H
#define BALL_VIEW_GUI_KERNEL_GLPRIMITIVEMANAGER_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_VIEW_GUI_KERNEL_GLDISPLAYLIST_H
#	include <BALL/VIEW/GUI/KERNEL/glDisplayList.h>
#endif

#ifndef BALL_VIEW_GUI_KERNEL_GLQUADRICOBJECT_H
#	include <BALL/VIEW/GUI/KERNEL/glQuadricObject.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>

#define BALL_VIEW_MAXIMAL_DISPLAY_LIST_OBJECT_SIZE 12

namespace BALL
{

	namespace VIEW
	{


		class GLDisplayListObject_
		{
			friend class GeometricObject;

			public:

			GLDisplayListObject_();

			virtual ~GLDisplayListObject_();


			virtual GLDisplayList& operator ()
				(unsigned int drawing_mode, unsigned int drawing_precision) = 0;

			const GLDisplayList& operator ()
				(unsigned int drawing_mode, unsigned int drawing_precision) const;
		};


		/**
		*/
		class GLSphereDisplayLists_
			: public GLDisplayListObject_
		{
			public:

			/**	@name Constructors and Destructors
			*/
			//@{

			GLSphereDisplayLists_();

			GLSphereDisplayLists_
				(const GLSphereDisplayLists_& GL_sphere_display_lists, bool deep = true);

			virtual ~GLSphereDisplayLists_();

			virtual void clear();

			virtual void destroy();
			//@}

			/**	@name	Accessors
			*/
			//@{

			void init();

			GLDisplayList& operator ()
				(unsigned int drawing_mode, unsigned int drawing_precision);
			//@}

			/**	@name	Debugging and Diagnostics
			*/
			//@{

			virtual bool isValid() const;

			virtual void dump
				(std::ostream& s = std::cout, Size depth = 0) const;
			//@}


			private:

			void create_();

			/* builts the sphere dots display lists */
			void buildDottedSphere_(int precision);

			/* help functions for sphere dots */
			void drawPoint_(Vector3& v);

			/* help functions for sphere dots */
			void subdivideTriangle_
				(Vector3& v1, Vector3& v2, Vector3& v3, int precision);

			/* the display lists */
			GLDisplayList* GL_display_list_;

			/* static array of vertices for sphere dots */
			static const float vertices_[12][3];
			static const int indices_[20][3];
		};



		/**
		*/
		class GLTubeDisplayLists_
			: public GLDisplayListObject_
		{
			public:

			/**	@name	Constructors and Destructors
			*/
			//@{

			GLTubeDisplayLists_();

			GLTubeDisplayLists_
				(const GLTubeDisplayLists_& GL_tube_display_lists_, bool deep = true);

			virtual ~GLTubeDisplayLists_();

			virtual void clear();

			virtual void destroy();
			//@}

			/**	@name	Constructors and Destructors
			*/
			//@{

			void init();

			GLDisplayList& operator ()
				(unsigned int drawing_mode, unsigned int drawing_precision);
			//@}

			/**	@name	Debugging and Diagnostics
			*/
			//@{

			virtual bool isValid() const;

			virtual void dump
				(std::ostream& s = std::cout, Size depth = 0) const;
			//@}


			private:

			void create_();

			/* the display lists */
			GLDisplayList* GL_display_list_;
		};

		/**
		*/
		class GLSimpleBoxDisplayLists_
			: public GLDisplayListObject_
		{
			public:

			/**	@name	Constructors and Destructors
			*/
			//@{

			GLSimpleBoxDisplayLists_();

			GLSimpleBoxDisplayLists_
				(const GLSimpleBoxDisplayLists_& GL_simple_box_display_lists_, bool deep = true);

			virtual ~GLSimpleBoxDisplayLists_();

			virtual void clear();

			virtual void destroy();
			//@}

			/**	@name	Constructors and Destructors
			*/
			//@{

			void init();

			GLDisplayList& operator ()
				(unsigned int drawing_mode, unsigned int drawing_precision);
			//@}

			/**	@name	Debugging and Diagnostics
			*/
			//@{

			virtual bool isValid() const;

			virtual void dump
				(std::ostream& s = std::cout, Size depth = 0) const;
			//@}


			private:

			void create_();

			void createDotBox_();
			void createLineBox_();
			void createSolidBox_();

			/* the display lists */
			GLDisplayList* GL_display_list_;
		};



		/**
		*/
		class GLPrimitiveManager
		{
			public:

			/**     @name   Type Definitions
			*/
			//@{
			typedef unsigned int Name;
			//@}

			/**	@name	Constructors and Destructors
			*/
			//@{

			GLPrimitiveManager();
			
			GLPrimitiveManager
				(const GLPrimitiveManager& GL_primitive_manager, bool deep = true);
			
			virtual ~GLPrimitiveManager();

			virtual void clear();

			virtual void destroy();
			//@}
		
			/**	@name	Accessors
			*/
			//@{

			void init();

			void clearNames();

			Name getName(const GeometricObject& object);

			GeometricObject* getObject(Name name) const;
			//@}

			GLSphereDisplayLists_ Sphere;
			GLTubeDisplayLists_ Tube;
			GLSimpleBoxDisplayLists_ SimpleBox;

			/**	@name Debugging and Diagnostics
			*/
			//@{

			virtual bool isValid() const;

			virtual void dump
				(std::ostream& s = std::cout, Size depth = 0) const;
			//@}

			/**	@name	Storers
			*/
			//@{

			virtual void read(std::istream& s);

			virtual void write(std::ostream& s) const;
			//@}

			private:

			typedef HashMap<GeometricObject*, Name> NameHashMap;
			typedef HashMap<Name, GeometricObject*> GeometricObjectHashMap;

			GeometricObjectHashMap name_to_object_;
			NameHashMap object_to_name_;

			Name all_names_;
		};


#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/KERNEL/glPrimitiveManager.iC>
#		endif

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_GUI_KERNEL_GLPRIMITIVEMANAGER_H
