// $Id: vanDerWaalsModel.C,v 1.8 2001/01/26 00:43:32 amoll Exp $

#include <BALL/MOLVIEW/FUNCTOR/vanDerWaalsModel.h>

using namespace std;

namespace BALL
{
	namespace MOLVIEW
	{

		AddVanDerWaalsModel::AddVanDerWaalsModel()
			: AtomBondModelBaseProcessor()
		{
 		}

		AddVanDerWaalsModel::AddVanDerWaalsModel(const AddVanDerWaalsModel& _rAddVanDerWaalsModel, bool deep)
			: AtomBondModelBaseProcessor(_rAddVanDerWaalsModel, deep)
		{
		}

		AddVanDerWaalsModel::~AddVanDerWaalsModel()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this << " of class " << getBallClass().getName() << endl;
			#endif 

			destroy();
		}

		void AddVanDerWaalsModel::clear()
			throw()
		{
			AtomBondModelBaseProcessor::clear();
			setProperty(GeometricObject::PROPERTY__MODEL_VDW);
		}

		void AddVanDerWaalsModel::destroy()
			throw()
		{
			AtomBondModelBaseProcessor::destroy();
		}

		bool AddVanDerWaalsModel::start()
		{
			// init model connector
			getModelConnector()->setProperties(*this);

			return AtomBondModelBaseProcessor::start();
		}
				
		bool AddVanDerWaalsModel::finish()
		{
			buildBondModels_();

			return true;
		}
				
		Processor::Result AddVanDerWaalsModel::operator() (Composite &composite)
		{
			// composite is an atom ?
			if (!RTTI::isKindOf<Atom>(composite))
			{
				return Processor::CONTINUE;
			}

			Atom* atom = RTTI::castTo<Atom>(composite);

			// remove all models appended to atom
			removeGeometricObjects_(*atom, true);

			Sphere* pSphere = createSphere_();

			BALL_PRECONDITION
				(pSphere != 0,
				 BALL_MOLVIEW_VANDERWAALSMODEL_ERROR_HANDLER
				 (AddVanDerWaalsModel::ERROR__CANNOT_CREATE_SPHERE));

			// carry on selected flag
			pSphere->Selectable::set(*atom);

			pSphere->PropertyManager::set(*this);
			pSphere->PropertyManager::setProperty(GeometricObject::PROPERTY__MODEL_VDW);
			pSphere->setRadius((atom->getElement()).getVanDerWaalsRadius());
			pSphere->setVertexAddress(atom->getPosition());

			atom->host(*getColorCalculator());

			pSphere->setColor(getColorCalculator()->getColor());
			
			composite.appendChild(*pSphere);
			
			insertAtom_(atom);
			
			return Processor::CONTINUE;
		}

		void AddVanDerWaalsModel::dump(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			AtomBondModelBaseProcessor::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void AddVanDerWaalsModel::read(istream & /* s */)
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void AddVanDerWaalsModel::write(ostream & /* s */) const
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		Sphere* AddVanDerWaalsModel::createSphere_()
		{
			return (Sphere *)(new Sphere());
		}


#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/FUNCTOR/vanDerWaalsModel.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
