// $Id: simpleBox.h,v 1.3 2000/12/19 21:54:08 oliver Exp $

#ifndef BALL_VIEW_PRIMITIV_SIMPLEBOX_H
#define BALL_VIEW_PRIMITIV_SIMPLEBOX_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLOR_H
#	include <BALL/VIEW/DATATYPE/color.h>
#endif

#ifndef BALL_VIEW_KERNEL_COLOREXTENSION1_H
#	include <BALL/VIEW/KERNEL/colorExtension1.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_VIEW_KERNEL_VERTEX2_H
#	include <BALL/VIEW/KERNEL/vertex2.h>
#endif


namespace BALL
{
	
	namespace VIEW
	{

		/**
		*/
		class SimpleBox
			: public GeometricObject,
				public ColorExtension,
				public Vertex2
		{
			public:

			/**	@name	Constructors and Destructors
			*/
			//@{

			SimpleBox();

			SimpleBox(const SimpleBox& SimpleBox, bool deep = true);

			SimpleBox(const GeometricObject& geometric_object);

			virtual ~SimpleBox()
				throw();

			virtual void clear()
				throw();

			virtual void destroy()
				throw();
			//@}

			
			/**	@name	Assignment
			*/
			//@{

			void set(const SimpleBox& SimpleBox, bool deep = true);

			SimpleBox& operator = (const SimpleBox& SimpleBox);

			void get(SimpleBox& SimpleBox, bool deep = true) const;

			void swap(SimpleBox& SimpleBox);
			//@}


			/**	@name	Debugging and Diagnostics
			*/
			//@{

			virtual bool isValid() const
				throw();

			virtual void dump
				(std::ostream&  s = std::cout, Size depth = 0) const
				throw();
			//@}

			/**	@name	Storers
			*/
			//@{
			virtual void read(std::istream&  s);

			virtual void write(std::ostream& s) const;
			//@}


			protected:

			virtual bool extract();
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/PRIMITIV/simpleBox.iC>
#		endif
		
	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_PRIMITIV_SIMPLEBOX_H
