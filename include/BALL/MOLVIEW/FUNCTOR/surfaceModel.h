// $Id: surfaceModel.h,v 1.1 2000/04/25 15:28:11 hekl Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_SURFACEMODEL_H
#define BALL_MOLVIEW_FUNCTOR_SURFACEMODEL_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_LIST_H
#	include <BALL/DATATYPE/list.h>
#endif

#ifndef BALL_KERNEL_FOREACH_H
#	include <BALL/KERNEL/forEach.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_MESH_H
#	include <BALL/VIEW/PRIMITIV/mesh.h>
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

		using VIEW::Mesh;

		/**
		*/
		class AddSurfaceModel
			: public BaseModelProcessor
		{
			public:

			/**	@name	Constructors and Destructors
			*/
			//@{

			AddSurfaceModel();

			AddSurfaceModel
				(const AddSurfaceModel& add_surface_model, bool deep = true);

			virtual ~AddSurfaceModel();

			virtual void clear();

			virtual void destroy();
			//@}


			/**	@name	Assignment
			*/
			//@{

			void set
				(const AddSurfaceModel& add_surface_model, bool deep = true);

			AddSurfaceModel& operator =
				(const AddSurfaceModel& add_surface_model);

			void get
				(AddSurfaceModel& add_surface_model, bool deep = true) const;

			void swap(AddSurfaceModel& add_surface_model);
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

			virtual Mesh* createMesh_();

			bool get_composite_;

			Composite* start_composite_;
		};

#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/FUNCTOR/surfaceModel.iC>
#			endif

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_SURFACEMODEL_H
