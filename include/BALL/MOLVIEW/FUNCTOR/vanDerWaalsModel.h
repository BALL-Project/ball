// $Id: vanDerWaalsModel.h,v 1.3 2000/01/08 20:31:28 hekl Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_VANDERWAALSMODEL_H
#define BALL_MOLVIEW_FUNCTOR_VANDERWAALSMODEL_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_KERNEL_PSE_H
#	include <BALL/KERNEL/PSE.h>
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

			enum ErrorCode
			{
				ERROR__NONE                 = -1,
				ERROR__UNKNOWN              = 0,
				ERROR__NOT_IMPLEMENTED      = 1,
				ERROR__CANNOT_CREATE_SPHERE = 2,

				NUMBER_OF_VANDERWAALSMODEL_ERRORS
			};

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

			virtual Processor::Result operator() (Atom& atom);

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
