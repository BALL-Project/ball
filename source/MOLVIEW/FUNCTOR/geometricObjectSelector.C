// $Id: geometricObjectSelector.C,v 1.6 2001/01/26 00:43:32 amoll Exp $

#include <BALL/MOLVIEW/FUNCTOR/geometricObjectSelector.h>

using namespace std;

namespace BALL
{

	namespace MOLVIEW
	{

		GeometricObjectSelector::GeometricObjectSelector()
			: AtomBondModelBaseProcessor(),
				selection_(true),
				selection_color_(255,255,0,255)
		{
		}

		GeometricObjectSelector::GeometricObjectSelector
			(const GeometricObjectSelector &selector, bool deep)
			: AtomBondModelBaseProcessor(selector, deep),
				selection_(selector.selection_),
				selection_color_(selector.selection_color_)
		{
		}

		GeometricObjectSelector::~GeometricObjectSelector()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
			 << " of class " << getBallClass().getName() << endl;
			#endif 

			destroy();
		}

		void GeometricObjectSelector::clear()
			throw()
		{
			AtomBondModelBaseProcessor::clear();
		}

		void GeometricObjectSelector::destroy()
			throw()
		{
			AtomBondModelBaseProcessor::destroy();

			selection_ = true;
			selection_color_.set(255,255,0,255);
		}

		void GeometricObjectSelector::set(const GeometricObjectSelector &selector, bool deep)
		{
			AtomBondModelBaseProcessor::set(selector, deep);

			selection_ = selector.selection_;
			selection_color_ = selector.selection_color_;
		}

		GeometricObjectSelector& GeometricObjectSelector::operator = (const GeometricObjectSelector &selector)
		{
			set(selector);

			return *this;
		}

		void GeometricObjectSelector::get(GeometricObjectSelector &selector, bool deep) const
		{
			selector.set(*this, deep);
		}

		void GeometricObjectSelector::swap (GeometricObjectSelector &selector)
		{
			AtomBondModelBaseProcessor::swap(selector);

			bool _bool = selection_;
			selection_ = selector.selection_;
			selector.selection_ = _bool;

			selection_color_.swap(selector.selection_color_);
		}

		bool GeometricObjectSelector::start()
		{
			getSearcher_().clear();

			return AtomBondModelBaseProcessor::start();
		}
				
		bool GeometricObjectSelector::finish()
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
				
		Processor::Result GeometricObjectSelector::operator() (Composite &composite)
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

		void GeometricObjectSelector::dump(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			cout << "use Selection Color: " << ((selection_ == true) ? "Yes" : "No") << endl;

			BALL_DUMP_DEPTH(s, depth);
			cout << "use Object Color:    " << ((selection_ == false) ? "Yes" : "No") << endl;

			BALL_DUMP_DEPTH(s, depth);
			cout << "Selection Color:     " << selection_color_ << endl;

			AtomBondModelBaseProcessor::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void GeometricObjectSelector::read(istream & /* s */)
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void GeometricObjectSelector::write(ostream & /* s */) const
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}


#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/FUNCTOR/geometricObjectSelector.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
