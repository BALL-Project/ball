// $Id: removeModel.C,v 1.6 2000/06/25 19:06:36 hekl Exp $

#include <BALL/MOLVIEW/FUNCTOR/removeModel.h>

using namespace std;

namespace BALL
{

	namespace MOLVIEW
	{

		RemoveModel::RemoveModel
			()
				: 
				AtomBondModelBaseProcessor()
		{
		}

		RemoveModel::RemoveModel
			(const RemoveModel &model,
			 bool deep)
				:
				AtomBondModelBaseProcessor(model, deep)
		{
		}

		RemoveModel::~RemoveModel
			()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
			 << " of class " << getBallClass().getName() << endl;
			#endif 

			destroy();
		}

		void 
		RemoveModel::clear
			()
		{
			AtomBondModelBaseProcessor::clear();
		}

		void 
		RemoveModel::destroy
			()
		{
			AtomBondModelBaseProcessor::destroy();
		}

		void 
		RemoveModel::set
			(const RemoveModel &model,
			 bool deep)
		{
			AtomBondModelBaseProcessor::set(model, deep);
		}

		RemoveModel &
		RemoveModel::operator =
			(const RemoveModel &model)
		{
			set(model);

			return *this;
		}

		void 
		RemoveModel::get
			(RemoveModel &model,
			 bool deep) const
		{
			model.set(*this, deep);
		}

		void 
		RemoveModel::swap
			(RemoveModel &model)
		{
			AtomBondModelBaseProcessor::swap(model);
		}

		bool 
		RemoveModel::start
			()
		{
			getSearcher_().clear();

			return AtomBondModelBaseProcessor::start();
		}
				
		bool 
		RemoveModel::finish
			()
		{
			// generate StickPrimitives
			Atom *first__pAtom = 0;
			Atom *second__pAtom = 0;
			Bond *__pBond = 0;
			AtomBondIterator bond__Iterator;

			List<Atom*>::Iterator list_iterator;

			// for all used atoms
			for (list_iterator = getAtomList_().begin();
					 list_iterator != getAtomList_().end(); ++list_iterator)
			{
				first__pAtom = *list_iterator;

				// for all bonds connected from first- to second atom
				BALL_FOREACH_ATOM_BOND(*first__pAtom, bond__Iterator)
				{
					__pBond = &(*bond__Iterator);
					second__pAtom = __pBond->getSecondAtom();

					// use only atoms with greater handles than first atom
					if (*first__pAtom < *second__pAtom)
					{
						// remove models
						removeGeometricObjects_(*__pBond);
					}
				}

				// remove atom models
				removeGeometricObjects_(*first__pAtom);
			}
			
			return true;
		}
				
		Processor::Result 
		RemoveModel::operator()
			(Composite &composite)
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

		void 
		RemoveModel::dump
			(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			AtomBondModelBaseProcessor::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void 
		RemoveModel::read
			(istream & /* s */)
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void 
		RemoveModel::write
			(ostream & /* s */) const
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}


#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/FUNCTOR/removeModel.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
