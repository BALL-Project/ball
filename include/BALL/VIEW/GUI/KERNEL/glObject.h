// $Id: glObject.h,v 1.1 2000/09/23 12:49:55 hekl Exp $

#ifndef BALL_VIEW_OPENGL_KERNEL_GLOBJECT_H
#define BALL_VIEW_OPENGL_KERNEL_GLOBJECT_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_OPENGL_KERNEL_GLPRIMITIVEMANAGER_H
#	include <BALL/VIEW/OPENGL/KERNEL/glPrimitiveManager.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif


namespace BALL
{

	namespace VIEW
	{

		/* forward */
		class CompositeDescriptor;
		
		/**
		*/
		class GLObject
		{
			friend class CompositeDescriptor;

			public:

			/**	@name	Constructors and Destructors
			*/
			//@{

			GLObject();
			
			GLObject(const GLObject& GL_object, bool deep = true);

			virtual ~GLObject();

			virtual void clear();

			virtual void destroy();
			//@}

			/**	@name	Assignment
			*/
			//@{

			void set(const GLObject& GL_object, bool deep = true);

			GLObject& operator = (const GLObject& GL_object);

			void get(GLObject& GL_object, bool deep = true) const;

			void swap(GLObject& GL_object);
			//@}

			/**	@name	Debugging and Diagnostics
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

			

			protected:

			virtual bool draw(bool with_names = false);

			virtual bool drawUserDefined();

			virtual bool extract();

			void setGLPrimitiveManager
				(const GLPrimitiveManager& GL_primitive_manager);
			
			GLPrimitiveManager* getGLPrimitiveManager() const;

				
				
			private:

			/* pointer to the DisplayListManager */
			GLPrimitiveManager* GL_primitive_manager_;
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/OPENGL/KERNEL/glObject.iC>
#		endif

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_OPENGL_KERNEL_GLOBJECT_H

