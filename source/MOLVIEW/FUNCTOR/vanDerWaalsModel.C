// $Id: vanDerWaalsModel.C,v 1.5 2000/06/18 16:33:38 hekl Exp $

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
			getSearcher_().setProperty(GeometricObject::PROPERTY__MODEL_VDW);

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

			// remove all models appended to atom
			removeGeometricObjects_(*atom, true);

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
