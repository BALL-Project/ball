// $Id: objectSelector.C,v 1.1 2001/05/13 15:02:39 hekl Exp $

#include <BALL/MOLVIEW/FUNCTOR/objectSelector.h>

using namespace std;

namespace BALL
{

	namespace MOLVIEW
	{

		ObjectSelector::ObjectSelector()
			throw()
			: AtomBondModelBaseProcessor(),
				selection_(true)
		{
		}

		ObjectSelector::ObjectSelector
			(const ObjectSelector &selector, bool deep)
			throw()
			: AtomBondModelBaseProcessor(selector, deep),
				selection_(selector.selection_)
		{
		}

		ObjectSelector::~ObjectSelector()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
			 << " of class " << RTTI::getName<ObjectSelector>() << endl;
			#endif 

			destroy();
		}

		void ObjectSelector::clear()
			throw()
		{
			AtomBondModelBaseProcessor::clear();

			selection_ = true;
		}

		void ObjectSelector::destroy()
			throw()
		{
		}

		void ObjectSelector::set(const ObjectSelector &selector, bool deep)
			throw()
		{
			AtomBondModelBaseProcessor::set(selector, deep);

			selection_ = selector.selection_;
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

			bool _bool = selection_;
			selection_ = selector.selection_;
			selector.selection_ = _bool;
		}

		bool ObjectSelector::start()
			throw()
		{
			getSearcher_().clear();

			return AtomBondModelBaseProcessor::start();
		}
				
		bool ObjectSelector::finish()
			throw()
		{
			Atom *first_pAtom = 0;
			Atom *second_pAtom = 0;
			Bond *__pBond = 0;
			AtomBondIterator bond__Iterator;

			List<Atom*>::Iterator list_iterator;

			// for all used atoms
			for (list_iterator = getAtomList_().begin();
					 list_iterator != getAtomList_().end(); ++list_iterator)
			{
				first_pAtom = *list_iterator;

				// for all bonds connected from first- to second atom
				BALL_FOREACH_ATOM_BOND(*first_pAtom, bond__Iterator)
				{
					__pBond = &(*bond__Iterator);
					second_pAtom = __pBond->getSecondAtom();

					// use only atoms with greater handles than first atom
					if (*first_pAtom < *second_pAtom)
					{
						// select bond only if second atom is selected too
						if (getAtomSet_().has(second_pAtom))
						{
							if (selection_ == true)
							{
								__pBond->select();
							}
							else
							{
								__pBond->deselect();
							}
						}
					}
				}

				if (selection_ == true)
				{
					first_pAtom->select();
				}
				else
				{
					first_pAtom->deselect();
				}
			}
			
			return true;
		}
				
		Processor::Result ObjectSelector::operator() (Composite &composite)
			throw()
		{
			// composite is an atom ?
			if (!RTTI::isKindOf<Atom>(composite))
			{
				return Processor::CONTINUE;
			}

			Atom *atom = RTTI::castTo<Atom>(composite);

			if (selection_ == true)
			{
				atom->select();
			}
			else
			{
				atom->deselect();
			}

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

			BALL_DUMP_DEPTH(s, depth);
			cout << "use Selection Color: " << ((selection_ == true) ? "Yes" : "No") << endl;

			BALL_DUMP_DEPTH(s, depth);
			cout << "use Object Color:    " << ((selection_ == false) ? "Yes" : "No") << endl;

			AtomBondModelBaseProcessor::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}


#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/FUNCTOR/objectSelector.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
