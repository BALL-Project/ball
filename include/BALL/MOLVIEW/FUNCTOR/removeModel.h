// $Id: removeModel.h,v 1.2 1999/12/28 18:38:07 oliver Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_REMOVEMODEL_H
#define BALL_MOLVIEW_FUNCTOR_REMOVEMODEL_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_LIST_H
#	include <BALL/DATATYPE/list.h>
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

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_MOLVIEW_COMMON_COMMON_H
#	include <BALL/MOLVIEW/COMMON/common.h>
#endif

#ifndef BALL_MOLVIEW_FUNCTOR_BASEMODEL_H
#	include <BALL/MOLVIEW/FUNCTOR/baseModel.h>
#endif



namespace BALL
{
	
	namespace MOLVIEW
	{
		/**
		*/
		class RemoveModel
			: public BaseModelProcessor
		{
			public:

			enum ErrorCode
			{
				ERROR__NONE                             = -1,
				ERROR__UNKNOWN                          = 0,
				ERROR__NOT_IMPLEMENTED                  = 1,

				NUMBER_OF_REMOVEMODEL_ERRORS
			};

			/**	@name	Constructors and Destructors
			*/
			//@{

			RemoveModel();

			RemoveModel
				(const RemoveModel& model, bool deep = true);

			virtual ~RemoveModel();

			virtual void clear();

			virtual void destroy();
			//@}


			/**	@name	Assignment
			*/
			//@{

			void set
				(const RemoveModel& model, bool deep = true);

			RemoveModel& operator =
				(const RemoveModel& model);

			void get
				(RemoveModel& model, bool deep = true) const;

			void swap(RemoveModel& model);
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
			
			virtual Processor::Result operator() (Atom& atom);
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

			List<Atom*> used_atoms_;
		};

#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/FUNCTOR/removeModel.iC>
#			endif

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_REMOVEMODEL_H
