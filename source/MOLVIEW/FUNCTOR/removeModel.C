// $Id: removeModel.C,v 1.10 2001/07/01 21:45:27 oliver Exp $

#include <BALL/MOLVIEW/FUNCTOR/removeModel.h>

using namespace std;

namespace BALL
{
	namespace MOLVIEW
	{

		RemoveModel::RemoveModel()
			throw()
			: AtomBondModelBaseProcessor()
		{
		}

		RemoveModel::RemoveModel(const RemoveModel &model, bool deep)
			throw()
			: AtomBondModelBaseProcessor(model, deep)
		{
		}

		RemoveModel::~RemoveModel()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
						 << " of class " << RTTI::getName<RemoveModel>() << endl;
			#endif 

			destroy();
		}

		void 	RemoveModel::clear()
			throw()
		{
			AtomBondModelBaseProcessor::clear();
		}

		void RemoveModel::destroy()
			throw()
		{
		}

		bool RemoveModel::start()
			throw()
		{
			getSearcher_().clear();

			return AtomBondModelBaseProcessor::start();
		}
				
		bool RemoveModel::finish()
			throw()
		{
			// generate StickPrimitives
			Atom* first_atom = 0;
			Atom* second_atom = 0;
			Bond* pBond = 0;
			AtomBondIterator bond_Iterator;

			List<Atom*>::Iterator list_iterator;

			// for all used atoms
			for (list_iterator = getAtomList_().begin();
					 list_iterator != getAtomList_().end(); ++list_iterator)
			{
				first_atom = *list_iterator;

				// for all bonds connected from first- to second atom
				BALL_FOREACH_ATOM_BOND(*first_atom, bond_Iterator)
				{
					pBond = &(*bond_Iterator);
					second_atom = const_cast<Atom*>(pBond->getSecondAtom());

					// use only atoms with greater handles than first atom
					if (*first_atom < *second_atom)
					{
						// remove models
						removeGeometricObjects_(*pBond);
					}
				}

				// remove atom models
				removeGeometricObjects_(*first_atom);
			}
			
			return true;
		}
				
		Processor::Result RemoveModel::operator() (Composite &composite)
			throw()
		{
			// composite is an atom ?
			if (!RTTI::isKindOf<Atom>(composite))
			{
				return Processor::CONTINUE;
			}

			Atom *atom = RTTI::castTo<Atom>(composite);

			// check if there are already models appended
			atom->applyChild(getSearcher_());

			// geometric object is not existent => do nothing
			if (getSearcher_().geometricObjectsFound() == false)
			{
				return Processor::CONTINUE;
			}

			// collect atom with geometric object for deletion
			insertAtom_(atom);

			return Processor::CONTINUE;
		}

		void RemoveModel::dump(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			AtomBondModelBaseProcessor::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}


#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/FUNCTOR/removeModel.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
