// $Id: vanDerWaalsModel.C,v 1.11 2001/07/15 18:50:29 oliver Exp $

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

		AddVanDerWaalsModel::AddVanDerWaalsModel(const AddVanDerWaalsModel& model, bool deep)
			throw()
			: AtomBondModelBaseProcessor(model, deep)
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

			Sphere* sphere_ptr = createSphere_();

			if (sphere_ptr == 0)
			{
					throw Exception::OutOfMemory
						(__FILE__, __LINE__, sizeof(Sphere));
			}

			// carry on selected flag
			sphere_ptr->Selectable::set(*atom);

			sphere_ptr->PropertyManager::set(*this);
			sphere_ptr->PropertyManager::setProperty(PROPERTY__MODEL_VDW);
			sphere_ptr->setRadius((atom->getElement()).getVanDerWaalsRadius());
			sphere_ptr->setVertexAddress(atom->getPosition());

			atom->host(*getColorCalculator());

			sphere_ptr->setColor(getColorCalculator()->getColor());
			
			composite.appendChild(*sphere_ptr);
			
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
