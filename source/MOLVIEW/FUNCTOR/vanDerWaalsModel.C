// $Id: vanDerWaalsModel.C,v 1.9 2001/05/13 15:02:40 hekl Exp $

#include <BALL/MOLVIEW/FUNCTOR/vanDerWaalsModel.h>

using namespace std;

namespace BALL
{
	namespace MOLVIEW
	{

		AddVanDerWaalsModel::AddVanDerWaalsModel()
			throw()
			: AtomBondModelBaseProcessor()
		{
 		}

		AddVanDerWaalsModel::AddVanDerWaalsModel(const AddVanDerWaalsModel& _rAddVanDerWaalsModel, bool deep)
			throw()
			: AtomBondModelBaseProcessor(_rAddVanDerWaalsModel, deep)
		{
		}

		AddVanDerWaalsModel::~AddVanDerWaalsModel()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
						 << " of class " << RTTI::getName<AddVanDerWaalsModel>() << endl;
			#endif 

			destroy();
		}

		void AddVanDerWaalsModel::clear()
			throw()
		{
			AtomBondModelBaseProcessor::clear();
			setProperty(PROPERTY__MODEL_VDW);
		}

		void AddVanDerWaalsModel::destroy()
			throw()
		{
		}

		bool AddVanDerWaalsModel::start()
			throw()
		{
			// init model connector
			getModelConnector()->setProperties(*this);

			return AtomBondModelBaseProcessor::start();
		}
				
		bool AddVanDerWaalsModel::finish()
			throw()
		{
			buildBondModels_();

			return true;
		}
				
		Processor::Result AddVanDerWaalsModel::operator() (Composite &composite)
			throw(Exception::OutOfMemory)
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

			if (pSphere == 0)
			{
					throw Exception::OutOfMemory
						(__FILE__, __LINE__, sizeof(Sphere));
			}

			// carry on selected flag
			pSphere->Selectable::set(*atom);

			pSphere->PropertyManager::set(*this);
			pSphere->PropertyManager::setProperty(PROPERTY__MODEL_VDW);
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

		Sphere* AddVanDerWaalsModel::createSphere_()
		{
			return (Sphere *)(new Sphere());
		}


#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/FUNCTOR/vanDerWaalsModel.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
