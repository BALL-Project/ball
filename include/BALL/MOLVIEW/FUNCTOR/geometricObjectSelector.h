// $Id: geometricObjectSelector.h,v 1.4 2000/06/25 19:15:11 hekl Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_GEOMETRICOBJECTSELECTOR_H
#define BALL_MOLVIEW_FUNCTOR_GEOMETRICOBJECTSELECTOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_KERNEL_BOND_H
#	include <BALL/KERNEL/bond.h>
#endif

#ifndef BALL_KERNEL_FOREACH_H
#	include <BALL/KERNEL/forEach.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLOR_H
#	include <BALL/VIEW/DATATYPE/color.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_MOLVIEW_COMMON_COMMON_H
#	include <BALL/MOLVIEW/COMMON/common.h>
#endif

#ifndef BALL_MOLVIEW_FUNCTOR_ATOMBONDMODELBASEPROCESSOR_H
#	include <BALL/MOLVIEW/FUNCTOR/atomBondModelBaseProcessor.h>
#endif


using namespace BALL;
using namespace BALL::VIEW;
using namespace BALL::MOLVIEW;

namespace BALL
{
	
	namespace MOLVIEW
	{
		/**
		*/
		class GeometricObjectSelector
			: public AtomBondModelBaseProcessor
		{
			public:

			BALL_CREATE(GeometricObjectSelector)

			/**	@name	Constructors and Destructors
			*/
			//@{

			GeometricObjectSelector();

			GeometricObjectSelector
				(const GeometricObjectSelector& selector, bool deep = true);

			virtual ~GeometricObjectSelector();

			virtual void clear();

			virtual void destroy();
			//@}


			/**	@name	Assignment
			*/
			//@{

			void set
				(const GeometricObjectSelector& selector, bool deep = true);

			GeometricObjectSelector& operator =
				(const GeometricObjectSelector& selector);

			void get
				(GeometricObjectSelector& selector, bool deep = true) const;

			void swap(GeometricObjectSelector& selector);
			//@}

			/**	@name	Accessors
			*/
			//@{

			void setSelectionColor
				(const ColorRGBA &color);

			const ColorRGBA &getSelectionColor
				(void) const;

			void useSelectionColor();

			void useObjectColor();

			//@}

			/**	@name	Processor-Related Methods
			*/
			//@{

			virtual bool start();
			
			virtual bool finish();
			
			virtual Processor::Result operator() (Composite& composite);
			//@}

			/**	@name	Predicates
			*/
			//@{

			bool isSelectionColorUsed();

			bool isObjectColorUsed();

			//@}


			/**	@name	Debugging and Diagnostics
			*/
			//@{
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

			bool selection_;

			ColorRGBA selection_color_;
		};

#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/FUNCTOR/geometricObjectSelector.iC>
#			endif

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_GEOMETRICOBJECTSELECTOR_H
