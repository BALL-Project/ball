// $Id: label.h,v 1.4 2000/12/21 17:03:45 amoll Exp $

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

			Label()
				throw();

			Label(const Label& Label, bool deep = true)
				throw();

			Label(const GeometricObject& geometric_object)
				throw();

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

			void set(const Label& Label, bool deep = true)
				throw();

			const Label& operator = (const Label& Label)
				throw();

			void get(Label& Label, bool deep = true) const
				throw();

			void swap(Label& Label)
				throw();
			//@}

			/**	@name	Inspectors, Mutators, Accessors
			*/
			//@{
			void setText(const String text)
				throw();

			String getText() const
				throw();

			virtual String getTypeName() const
				throw();
			//@}

			/**	@name	Debugging and Diagnostics
			*/
			//@{

			virtual bool isValid() const
				throw();

			virtual void dump(std::ostream&  s = std::cout, Size depth = 0) const
				throw();
			//@}

			/**	@name	Storers
			*/
			//@{
			virtual void read(std::istream&  s)
				throw();

			virtual void write(std::ostream& s) const
				throw();
			//@}


			protected:

			virtual bool extract()
				throw();

  		private:	
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/PRIMITIV/label.iC>
#		endif
		
	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_PRIMITIV_LABEL_H
