// $Id: objectSelector.C,v 1.3.4.3 2002/11/09 20:58:37 amoll Exp $

#include <BALL/MOLVIEW/FUNCTOR/objectSelector.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/bond.h>

namespace BALL
{
	namespace MOLVIEW
	{

		ObjectSelector::ObjectSelector()
			throw()
			: AtomBondModelBaseProcessor()
		{
		}

		ObjectSelector::ObjectSelector(const ObjectSelector &selector, bool deep)
			throw()
			: AtomBondModelBaseProcessor(selector, deep)
		{
		}

		ObjectSelector::~ObjectSelector()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
						 << " of class " << RTTI::getName<ObjectSelector>() << std::endl;
			#endif 

			destroy();
		}

		void ObjectSelector::clear()
			throw()
		{
			AtomBondModelBaseProcessor::clear();
		}

		void ObjectSelector::destroy()
			throw()
		{
		}

		void ObjectSelector::set(const ObjectSelector &selector, bool deep)
			throw()
		{
			AtomBondModelBaseProcessor::set(selector, deep);
		}

		const ObjectSelector& ObjectSelector::operator = (const ObjectSelector &selector)
			throw()
		{
			set(selector);
			return *this;
		}

		void ObjectSelector::get(ObjectSelector &selector, bool deep) const
			throw()
		{
			selector.set(*this, deep);
		}

		void ObjectSelector::swap (ObjectSelector &selector)
			throw()
		{
			AtomBondModelBaseProcessor::swap(selector);
		}

		bool ObjectSelector::start()
		{
			return true; 
		}
				
		bool ObjectSelector::finish()
		{
			Atom *first_atom = 0;
			Atom *second_atom = 0;
			Bond *bond_ptr = 0;
			AtomBondIterator bond_it;

			List<Atom*>::Iterator list_iterator;

			// for all used atoms
			for (list_iterator = getAtomList_().begin();
					 list_iterator != getAtomList_().end(); ++list_iterator)
			{
				first_atom = *list_iterator;

				// for all bonds connected from first- to second atom
				BALL_FOREACH_ATOM_BOND(*first_atom, bond_it)
				{
					bond_ptr = &(*bond_it);
					second_atom = const_cast<Atom*>(bond_ptr->getSecondAtom());

					// use only atoms with greater handles than first atom
					// select bond only if second atom is selected too
					if (*first_atom < *second_atom  && getAtomSet_().has(second_atom))
					{
						bond_ptr->select();
					}
				}

				first_atom->select();
			}
			
			return true;
		}
				
		Processor::Result ObjectSelector::operator() (Composite &composite)
		{
			// composite is notnot  an atom ?
			if (!RTTI::isKindOf<Atom>(composite))
			{
				return Processor::CONTINUE;
			}

			Atom *atom = RTTI::castTo<Atom>(composite);

			// collect atom with geometric object for selection
			insertAtom_(atom);

			return Processor::CONTINUE;
		}

		void ObjectSelector::dump(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			AtomBondModelBaseProcessor::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		bool ObjectDeselector::finish()
		{
			Atom *first_atom = 0;
			Atom *second_atom = 0;
			Bond *bond_ptr = 0;
			AtomBondIterator bond_it;

			List<Atom*>::Iterator list_iterator;

			// for all used atoms
			for (list_iterator = getAtomList_().begin();
					 list_iterator != getAtomList_().end(); ++list_iterator)
			{
				first_atom = *list_iterator;

				// for all bonds connected from first- to second atom
				BALL_FOREACH_ATOM_BOND(*first_atom, bond_it)
				{
					bond_ptr = &(*bond_it);
					second_atom = const_cast<Atom*>(bond_ptr->getSecondAtom());

					// use only atoms with greater handles than first atom
					// select bond only if second atom is selected too
					if (*first_atom < *second_atom  && getAtomSet_().has(second_atom))
					{
						bond_ptr->deselect();
					}
				}

				first_atom->deselect();
			}
			
			return true;
		}

	} // namespace MOLVIEW
} // namespace BALL
