// $Id: vanDerWaalsModel.C,v 1.4 2000/04/25 15:17:01 hekl Exp $

#include <BALL/MOLVIEW/FUNCTOR/vanDerWaalsModel.h>

using namespace std;

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
			(Composite &composite)
		{
			// composite is an atom ?
			if (!RTTI::isKindOf<Atom>(composite))
			{
				return Processor::CONTINUE;
			}

			Atom *atom = RTTI::castTo<Atom>(composite);

			// check if there are already VanDerWaals models appended
			atom->applyChild(getSearcher_());

			// geometric object is already existent => do nothing
			if (getSearcher_().geometricObjectFound() == true)
			{
				return Processor::CONTINUE;
			}

			Sphere *__pSphere = createSphere_();

			BALL_PRECONDITION
				(__pSphere != 0,
				 BALL_MOLVIEW_VANDERWAALSMODEL_ERROR_HANDLER
				 (AddVanDerWaalsModel::ERROR__CANNOT_CREATE_SPHERE));

			__pSphere->PropertyManager::set(*this);
			__pSphere->setRadius((atom->getElement()).getVanDerWaalsRadius());
			__pSphere->setVertexAddress(atom->getPosition());

			atom->host(*getColorCalculator());

			__pSphere->setColor(getColorCalculator()->getColor());
			
			composite.appendChild(*__pSphere);
			
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
