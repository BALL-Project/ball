// $Id: vanDerWaalsModel.h,v 1.6 2000/04/25 15:28:12 hekl Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_VANDERWAALSMODEL_H
#define BALL_MOLVIEW_FUNCTOR_VANDERWAALSMODEL_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_KERNEL_PTE_H
#	include <BALL/KERNEL/PTE.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_SPHERE_H
#	include <BALL/VIEW/PRIMITIV/sphere.h>
#endif

#ifndef BALL_MOLVIEW_COMMON_COMMON_H
#	include <BALL/MOLVIEW/COMMON/common.h>
#endif

#ifndef BALL_MOLVIEW_FUNCTOR_BASEMODEL_H
#include <BALL/MOLVIEW/FUNCTOR/baseModel.h>
#endif

namespace BALL
{

	namespace MOLVIEW
	{


		using VIEW::Sphere;

		/**
		*/
		class AddVanDerWaalsModel
			: public BaseModelProcessor
		{
			public:

			/**	@name	Constructors and Destructors
			*/
			//@{

			AddVanDerWaalsModel();

			AddVanDerWaalsModel
				(const AddVanDerWaalsModel& add_van_der_waals_model, bool deep = true);

			virtual ~AddVanDerWaalsModel();

			virtual void clear();

			virtual void destroy();
			//@}

			
			/**	@name	Processor-Related Methods
			*/
			//@{

			virtual bool start();
			
			virtual bool finish();

			virtual Processor::Result operator() (Composite& composite);

			//@}
			
			private:

			virtual Sphere* createSphere_();
		};

#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/FUNCTOR/vanDerWaalsModel.iC>
#			endif

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_VANDERWAALSMODEL_H
