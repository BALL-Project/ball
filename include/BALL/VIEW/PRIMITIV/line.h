// $Id: line.h,v 1.6 2000/12/21 17:03:45 amoll Exp $

#ifndef BALL_VIEW_PRIMITIV_LINE_H
#define BALL_VIEW_PRIMITIV_LINE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_KERNEL_COLOLEXTENSION1_H
#	include <BALL/VIEW/KERNEL/colorExtension1.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
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
		class Line
			: public GeometricObject,
				public ColorExtension,
				public Vertex2
		{
			public:

			/**	@name	Constructors and Destructors
			*/
			//@{

			Line()
				throw();

			Line(const Line& line, bool deep = true)
				throw();

			Line(const GeometricObject& geometric_object)
				throw();

			virtual ~Line()
				throw();

			virtual void clear()
				throw();

			virtual void destroy()
				throw();
			//@}

			
			/**	@name	Assignment
			*/
			//@{

			void set(const Line& line, bool deep = true)
				throw();

			const Line& operator = (const Line& line)
				throw();

			void get(Line& line, bool deep = true) const
				throw();

			void swap(Line& line)
				throw();
			//@}


			/**	@name	Debugging adn Diagnostics
			*/
			//@{

			virtual bool isValid() const
				throw();

			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();
			//@}

			/**	@name	Storers
			*/
			//@{

			virtual void read(std::istream& s)
				throw();

			virtual void write(std::ostream& s) const
				throw();
			//@}


			protected:

			virtual bool extract()
				throw();
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/PRIMITIV/line.iC>
#		endif

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_PRIMITIV_LINE_H
