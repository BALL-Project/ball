// $Id: geometricObjectSelector.C,v 1.2 2000/04/25 15:17:01 hekl Exp $

#include <BALL/MOLVIEW/FUNCTOR/geometricObjectSelector.h>

using namespace std;

namespace BALL
{

	namespace MOLVIEW
	{

		GeometricObjectSelector::GeometricObjectSelector
			()
				: 
				BaseModelProcessor(),
				selection_(true),
				selection_color_(255,255,0,255),
				hashed_atoms_(),
				used_atoms_()
		{
		}

		GeometricObjectSelector::GeometricObjectSelector
			(const GeometricObjectSelector &selector,
			 bool deep)
				:
				BaseModelProcessor(selector, deep),
				selection_(selector.selection_),
				selection_color_(selector.selection_color_),
				hashed_atoms_(),
				used_atoms_()
		{
		}

		GeometricObjectSelector::~GeometricObjectSelector
			()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
			 << " of class " << getBallClass().getName() << endl;
			#endif 

			destroy();
		}

		void 
		GeometricObjectSelector::clear
			()
		{
			BaseModelProcessor::clear();

			
			used_atoms_.clear();
		}

		void 
		GeometricObjectSelector::destroy
			()
		{
			BaseModelProcessor::destroy();

			selection_ = true;
			selection_color_.set(255,255,0,255);
			hashed_atoms_.destroy();
			used_atoms_.destroy();
		}

		void 
		GeometricObjectSelector::set
			(const GeometricObjectSelector &selector,
			 bool deep)
		{
			BaseModelProcessor::set(selector, deep);

			selection_ = selector.selection_;
			selection_color_ = selector.selection_color_;
			hashed_atoms_.destroy();
			used_atoms_.destroy();
		}

		GeometricObjectSelector &
		GeometricObjectSelector::operator =
			(const GeometricObjectSelector &selector)
		{
			set(selector);

			return *this;
		}

		void 
		GeometricObjectSelector::get
			(GeometricObjectSelector &selector,
			 bool deep) const
		{
			selector.set(*this, deep);
		}

		void 
		GeometricObjectSelector::swap
			(GeometricObjectSelector &selector)
		{
			BaseModelProcessor::swap(selector);

			bool __bool = selection_;
			selection_ = selector.selection_;
			selector.selection_ = __bool;

			selection_color_.swap(selector.selection_color_);
		}

		bool 
		GeometricObjectSelector::start
			()
		{
			hashed_atoms_.clear();
			used_atoms_.clear();
			getSearcher_().clear();

			return BaseModelProcessor::start();
		}
				
		bool 
		GeometricObjectSelector::finish
			()
		{
			Atom *first__pAtom = 0;
			Atom *second__pAtom = 0;
			Bond *__pBond = 0;
			AtomBondIterator bond__Iterator;

			List<Atom*>::Iterator list_iterator;

			// for all used atoms
			for (list_iterator = used_atoms_.begin();
					 list_iterator != used_atoms_.end(); ++list_iterator)
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
						// select bond only if second atom is selected too
						if (hashed_atoms_.has(second__pAtom))
						{
							// get bond model
							__pBond->applyChild(getSearcher_());

							// if found, select bond model 
							if (getSearcher_().geometricObjectFound() == true)
							{
								// get bond model
								::BALL::VIEW::GeometricObject *bond_geometricObject 
									= (::BALL::VIEW::GeometricObject *)(getSearcher_().getGeometricObject());

								bond_geometricObject->setSelectedColor(selection_color_);

								if (selection_ == true)
								{
									bond_geometricObject->select();
								}
								else
								{
									bond_geometricObject->deselect();
								}
							}
						}
					}
				}

				// get the geometric object
				first__pAtom->applyChild(getSearcher_());

				::BALL::VIEW::GeometricObject *atom_geometricObject
					= (::BALL::VIEW::GeometricObject *)(getSearcher_().getGeometricObject());

				atom_geometricObject->setSelectedColor(selection_color_);

				if (selection_ == true)
				{
					atom_geometricObject->select();
				}
				else
				{
					atom_geometricObject->deselect();
				}
			}
			
			return true;
		}
				
		Processor::Result 
		GeometricObjectSelector::operator()
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

			if (selection_ == true)
			{
				atom->select();
			}
			else
			{
				atom->deselect();
			}

			// geometric object is not existent => do nothing
			if (getSearcher_().geometricObjectFound() == false)
			{
				return Processor::CONTINUE;
			}

			// collect atom with geometric object for selection
			hashed_atoms_.insert(atom);
			used_atoms_.push_back(atom);

			return Processor::CONTINUE;
		}

		void 
		GeometricObjectSelector::dump
			(ostream& s,
			 Size depth) const
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

			BaseModelProcessor::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void 
		GeometricObjectSelector::read
			(istream & /* s */)
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void 
		GeometricObjectSelector::write
			(ostream & /* s */) const
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}


#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/FUNCTOR/geometricObjectSelector.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
