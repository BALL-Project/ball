// $Id: backboneModel.h,v 1.1 2001/01/07 15:44:16 hekl Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_BACKBONEMODEL_H
#define BALL_MOLVIEW_FUNCTOR_BACKBONEMODEL_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_LIST_H
#	include <BALL/DATATYPE/list.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
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

#ifndef BALL_MOLVIEW_FUNCTOR_BASEMODEL_H
#	include <BALL/MOLVIEW/FUNCTOR/baseModel.h>
#endif

#ifndef BALL_MOLVIEW_PRIMITIV_BACKBONE_H
#	include <BALL/MOLVIEW/PRIMITIV/backbone.h>
#endif


namespace BALL
{
	
	namespace MOLVIEW
	{

		/**
		*/
		class AddBackboneModel
			: public BaseModelProcessor
		{
			public:

			/**	@name	Constructors and Destructors
			*/
			//@{

			AddBackboneModel();

			AddBackboneModel
				(const AddBackboneModel& add_Backbone_model, bool deep = true);

			virtual ~AddBackboneModel();

			virtual void clear();

			virtual void destroy();
			//@}


			/**	@name	Assignment
			*/
			//@{

			void set
				(const AddBackboneModel& add_Backbone_model, bool deep = true);

			AddBackboneModel& operator =
				(const AddBackboneModel& add_Backbone_model);

			void get
				(AddBackboneModel& add_Backbone_model, bool deep = true) const;

			void swap(AddBackboneModel& add_Backbone_model);
			//@}

			/**	@name	Accessors
			*/
			//@{

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

			virtual Backbone *createBackbone_();


			bool get_composite_;

			Composite* start_composite_;

			// list of used atoms for the creation of the backbone
			List<Atom*> atoms_;

			// list of colors correlating to the atoms saved in atoms_.
			List<ColorRGBA> atoms_color_;
		};

#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/FUNCTOR/backboneModel.iC>
#			endif

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_BACKBONEMODEL_H
