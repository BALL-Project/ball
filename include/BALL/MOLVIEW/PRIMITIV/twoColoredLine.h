// $Id: twoColoredLine.h,v 1.2 1999/12/19 17:19:02 oliver Exp $

#ifndef BALL_VIEW_MOLVIEW_PRIMITIV_TWOCOLOREDLINE_H
#define BALL_VIEW_MOLVIEW_PRIMITIV_TWOCOLOREDLINE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_KERNEL_COLOREXTENSION2_H
#	include <BALL/VIEW/KERNEL/colorExtension2.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_VIEW_KERNEL_VERTEX2_H
#	include <BALL/VIEW/KERNEL/vertex2.h>
#endif
namespace BALL
{

	namespace MOLVIEW
	{

		using VIEW::ColorExtension2;
		using VIEW::Vertex2;

		/**
		*/
		class TwoColoredLine
			: public VIEW::GeometricObject,
				public ColorExtension2,
				public Vertex2
		{
			public:

			/**	@name	Constructors and Destructors
			*/
			//@{

			TwoColoredLine();

			TwoColoredLine
				(const TwoColoredLine& rwo_colored_line, bool deep = true);

			TwoColoredLine
				(const VIEW::GeometricObject& geometric_object);

			virtual ~TwoColoredLine();

			virtual void clear();

			virtual void destroy();
			//@}

			/**	@name	Assignment
			*/
			//@{

			void set
				(const TwoColoredLine& two_colored_line, bool deep = true);

			TwoColoredLine& operator =
				(const TwoColoredLine& two_colored_line);

			void get
				(TwoColoredLine& two_colored_line, bool deep = true) const;

			void swap(TwoColoredLine& two_colored_line);
			//@}


			/**	@name	Accessors
			*/
			//@{

			Vector3 getMiddleVertex();
			//@}


			/**	@name	Debugging and Diagnostics
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

			virtual bool extract();
		};

#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/PRIMITIV/twoColoredLine.iC>
#			endif

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_VIEW_MOLVIEW_PRIMITIV_TWOCOLOREDLINE_H
