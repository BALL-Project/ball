// $Id: atomBondModelBaseProcessor.C,v 1.1 2000/06/25 19:03:58 hekl Exp $

#include <BALL/MOLVIEW/FUNCTOR/atomBondModelBaseProcessor.h>

using namespace std;

namespace BALL
{

	namespace MOLVIEW
	{

		AtomBondModelBaseProcessor::AtomBondModelBaseProcessor()
			:	BaseModelProcessor(),
			  used_atoms_(),
				hashed_atoms_()
		{
		}

		AtomBondModelBaseProcessor::AtomBondModelBaseProcessor
		  (const AtomBondModelBaseProcessor& processor, bool deep)
			:	BaseModelProcessor(processor, deep),
				used_atoms_(),
				hashed_atoms_()
		{
		}

		AtomBondModelBaseProcessor::~AtomBondModelBaseProcessor()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
						 << " of class " << RTTI::getName<AtomBondModelBaseProcessor>() << endl;
			#endif 

			destroy();
		}

		void AtomBondModelBaseProcessor::clear()
		{
			BaseModelProcessor::clear();
			clearUsedAtoms_();
		}

		void AtomBondModelBaseProcessor::destroy()
		{
			BaseModelProcessor::destroy();
			clearUsedAtoms_();
		}

		void AtomBondModelBaseProcessor::set
			(const AtomBondModelBaseProcessor& processor, bool deep)
		{
			clearUsedAtoms_();

			BaseModelProcessor::set(processor, deep);
		}

		AtomBondModelBaseProcessor& AtomBondModelBaseProcessor::operator =
			(const AtomBondModelBaseProcessor& processor)
		{
			set(processor);

			return *this;
		}

		void AtomBondModelBaseProcessor::get
			(AtomBondModelBaseProcessor& processor, bool deep) const
		{
			processor.set(*this, deep);
		}

		void AtomBondModelBaseProcessor::swap
			(AtomBondModelBaseProcessor& processor)
		{
			BaseModelProcessor::swap(processor);
		}

		bool AtomBondModelBaseProcessor::start()
		{
			clearUsedAtoms_();
			
			return BaseModelProcessor::start();
		}
				
		bool AtomBondModelBaseProcessor::finish()
		{
			return BaseModelProcessor::finish();
		}
				
		Processor::Result AtomBondModelBaseProcessor::operator()(Composite & /* composite */)
		{
			return Processor::CONTINUE;
		}

		bool AtomBondModelBaseProcessor::isValid() const
		{
			return BaseModelProcessor::isValid();
		}

		void AtomBondModelBaseProcessor::dump
			(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BaseModelProcessor::dump(s, depth + 1);

			BALL_DUMP_DEPTH(s, depth);
			s << "used atoms: " << used_atoms_.size() << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void AtomBondModelBaseProcessor::read(istream & /* s */)
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void AtomBondModelBaseProcessor::write(ostream & /* s */) const
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void AtomBondModelBaseProcessor::buildBondModels_()
    {
			// generate bond primitive
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
					
					if (first__pAtom != __pBond->getSecondAtom())
					{
						second__pAtom = __pBond->getSecondAtom();
					}
					else
					{
						second__pAtom = __pBond->getFirstAtom();
					}

					// use only atoms with greater handles than first atom
					// or
					// second atom not a used atom, but smaller as the first atom
					// process bond between them		
					if (*first__pAtom < *second__pAtom
							|| !getAtomSet_().has(second__pAtom))
					{
						// remove all models append to bond
						removeGeometricObjects_(*__pBond, true);

						// build connection primitive
						__pBond->host(*getModelConnector());
					}
				}
			}
    }

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/FUNCTOR/atomBondModelBaseProcessor.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
