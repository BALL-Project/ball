// $Id: label.h,v 1.3 2000/12/19 21:54:08 oliver Exp $

#ifndef BALL_VIEW_PRIMITIV_LABEL_H
#define BALL_VIEW_PRIMITIV_LABEL_H

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

#ifndef BALL_VIEW_KERNEL_VERTEX1_H
#	include <BALL/VIEW/KERNEL/vertex1.h>
#endif


namespace BALL
{
	
	namespace VIEW
	{

		/**
		*/
		class Label
			: public GeometricObject,
				public ColorExtension,
				public Vertex
		{
			public:

			/**	@name	Constructors and Destructors
			*/
			//@{

			Label();

			Label(const Label& Label, bool deep = true);

			Label(const GeometricObject& geometric_object);

			virtual ~Label()
				throw();

			virtual void clear()
				throw();

			virtual void destroy()
				throw();
			//@}

			
			/**	@name	Assignment
			*/
			//@{

			void set(const Label& Label, bool deep = true);

			Label& operator = (const Label& Label);

			void get(Label& Label, bool deep = true) const;

			void swap(Label& Label);
			//@}

			/**	@name	Inspectors, Mutators, Accessors
			*/
			//@{
			void setText(const String text);

			String getText() const;

			virtual String getTypeName() const;
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

  		private:	
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/PRIMITIV/label.iC>
#		endif
		
	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_PRIMITIV_LABEL_H
