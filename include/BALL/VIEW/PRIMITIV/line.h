// $Id: line.h,v 1.1 1999/08/26 07:53:16 oliver Exp $

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

			Line();

			Line(const Line& line, bool deep = true);

			Line(const GeometricObject& geometric_object);

			virtual ~Line();

			virtual void clear();

			virtual void destroy();
			//@}

			
			/**	@name	Assignment
			*/
			//@{

			void set(const Line& line, bool deep = true);

			Line& operator = (const Line& line);

			void get(Line& line, bool deep = true) const;

			void swap(Line& line);
			//@}


			/**	@name	Debugging adn Diagnostics
			*/
			//@{

			virtual bool isValid() const;

			virtual void dump
				(ostream& s = cout, unsigned long depth = 0) const;
			//@}

			/**	@name	Storers
			*/
			//@{

			virtual void read(istream& s);

			virtual void write(ostream& s) const;
			//@}


			protected:

			virtual bool export();
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/PRIMITIV/line.iC>
#		endif

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_PRIMITIV_LINE_H
