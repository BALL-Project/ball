// $Id: atomBondModelBaseProcessor.C,v 1.3 2001/01/25 23:47:55 amoll Exp $

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
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
						 << " of class " << RTTI::getName<AtomBondModelBaseProcessor>() << endl;
			#endif 

			destroy();
		}

		void AtomBondModelBaseProcessor::clear()
			throw()
		{
			BaseModelProcessor::clear();
			clearUsedAtoms_();
		}

		void AtomBondModelBaseProcessor::destroy()
			throw()
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
			throw()
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
			Atom* first_pAtom = 0;
			Atom* second_pAtom = 0;
			Bond* pbond = 0;
			AtomBondIterator bond_Iterator;
			List<Atom*>::Iterator list_iterator;

			// for all used atoms
			for (list_iterator = getAtomList_().begin();
					 list_iterator != getAtomList_().end(); ++list_iterator)
			{
				first_pAtom = *list_iterator;

				// for all bonds connected from first- to second atom
				BALL_FOREACH_ATOM_BOND(*first_pAtom, bond_Iterator)
				{
					pbond = &(*bond_Iterator);
					
					if (first_pAtom != pbond->getSecondAtom())
					{
						second_pAtom = pbond->getSecondAtom();
					}
					else
					{
						second_pAtom = pbond->getFirstAtom();
					}

					// use only atoms with greater handles than first atom
					// or
					// second atom not a used atom, but smaller as the first atom
					// process bond between them		
					if (*first_pAtom < *second_pAtom
							|| !getAtomSet_().has(second_pAtom))
					{
						// remove all models append to bond
						removeGeometricObjects_(*pbond, true);

						// build connection primitive
						pbond->host(*getModelConnector());
					}
				}
			}
    }

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/FUNCTOR/atomBondModelBaseProcessor.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
