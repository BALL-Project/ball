// $Id: vanDerWaalsModel.C,v 1.2 1999/12/19 17:14:26 oliver Exp $
#include <BALL/MOLVIEW/FUNCTOR/vanDerWaalsModel.h>

namespace BALL
{

	namespace MOLVIEW
	{

		AddVanDerWaalsModel::AddVanDerWaalsModel
			()
				:
				BaseModelProcessor()
		{
			setProperty(GeometricObject::PROPERTY__MODEL_VDW);
		}

		AddVanDerWaalsModel::AddVanDerWaalsModel
			(const AddVanDerWaalsModel &__rAddVanDerWaalsModel,
			 bool deep)
				:
				BaseModelProcessor(__rAddVanDerWaalsModel, deep)
		{
		}

		AddVanDerWaalsModel::~AddVanDerWaalsModel
			()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
			 << " of class " << getBallClass().getName() << endl;
			#endif 

			destroy();
		}

		void 
		AddVanDerWaalsModel::clear
			()
		{
			BaseModelProcessor::clear();

			setProperty(GeometricObject::PROPERTY__MODEL_VDW);
		}

		void 
		AddVanDerWaalsModel::destroy
			()
		{
			BaseModelProcessor::destroy();
		}

		bool 
		AddVanDerWaalsModel::start
			()
		{
			return BaseModelProcessor::start();
		}
				
		bool 
		AddVanDerWaalsModel::finish
			()
		{
			return true;
		}
				
		Processor::Result 
		AddVanDerWaalsModel::operator()
			(Atom &__rAtom)
		{
			// check if there are already VanDerWaals models appended
			__rAtom.applyChild(getSearcher());

			// geometric object is already existent => do nothing
			if (getSearcher().geometricObjectFound() == true)
			{
				return Processor::CONTINUE;
			}

			Sphere *__pSphere = createSphere_();

			BALL_PRECONDITION
				(__pSphere != 0,
				 BALL_MOLVIEW_VANDERWAALSMODEL_ERROR_HANDLER
				 (AddVanDerWaalsModel::ERROR__CANNOT_CREATE_SPHERE));

			__pSphere->PropertyManager::set(*this);
			__pSphere->setRadius((__rAtom.getElement()).getVanDerWaalsRadius());
			__pSphere->setVertexAddress(__rAtom.getPosition());

			__rAtom.host(*getColorCalculator());

			__pSphere->setColor(getColorCalculator()->getColor());
			
			__rAtom.Composite::appendChild(*__pSphere);
			
			return Processor::CONTINUE;
		}

		Sphere *
		AddVanDerWaalsModel::createSphere_
			()
		{
			return (Sphere *)(new Sphere());
		}


#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/FUNCTOR/vanDerWaalsModel.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
