// $Id: lineModel.h,v 1.5 2000/06/25 19:14:42 hekl Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_LINEMODEL_H
#define BALL_MOLVIEW_FUNCTOR_LINEMODEL_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_LINE_H
#	include <BALL/VIEW/PRIMITIV/line.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_POINT_H
#	include <BALL/VIEW/PRIMITIV/point.h>
#endif

#ifndef BALL_MOLVIEW_COMMON_COMMON_H
#	include <BALL/MOLVIEW/COMMON/common.h>
#endif

#ifndef BALL_MOLVIEW_FUNCTOR_ATOMBONDMODELBASEPROCESSOR_H
#	include <BALL/MOLVIEW/FUNCTOR/atomBondModelBaseProcessor.h>
#endif

#ifndef BALL_MOLVIEW_PRIMITIV_TWOCOLOREDLINE_H
#	include <BALL/MOLVIEW/PRIMITIV/twoColoredLine.h>
#endif

namespace BALL
{
			
	namespace MOLVIEW
	{

		using VIEW::Point;
		using VIEW::Line;


		/**
		*/
		class AddLineModel
			: public AtomBondModelBaseProcessor
		{
			public:

			/**	@name	Constructors and Destructors
			*/
			//@{

			AddLineModel();

			AddLineModel(const AddLineModel& add_line_model, bool deep = true);

			virtual ~AddLineModel();

			virtual void clear();

			virtual void destroy();
			//@}


			/**	@name	Assignment
			*/
			//@{

			void set
				(const AddLineModel& add_line_model, bool deep = true);

			AddLineModel& operator =
				(const AddLineModel& add_line_model);

			void get
				(AddLineModel& add_line_model, bool deep = true) const;

			void swap(AddLineModel& add_line_model);
			//@}

			/**	@name	Processor-Related Methods
			*/
			//@{

			virtual bool start();
			
			virtual bool finish();
			
			virtual Processor::Result operator() (Composite& composite);
			//@}

			/**	@name Debugging and Diagnostics
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

			virtual Point* createPoint_();

			virtual Line* createLine_();

			virtual TwoColoredLine* createTwoColoredLine_();
		};

#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/FUNCTOR/lineModel.iC>
#			endif

	} // namespace MOLVIEW

} // namspace BALL

#endif // BALL_MOLVIEW_FUNCTOR_LINEMODEL_H
