// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: atomBondModelBaseProcessor.C,v 1.12 2004/07/12 21:19:27 amoll Exp $

#include <BALL/VIEW/MODELS/atomBondModelBaseProcessor.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/residue.h>

#include <BALL/QSAR/ringPerceptionProcessor.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		AtomBondModelBaseProcessor::AtomBondModelBaseProcessor()
			throw()
			:	ModelProcessor(),
			  used_atoms_(),
				atom_set_()
		{
		}

		AtomBondModelBaseProcessor::AtomBondModelBaseProcessor(const AtomBondModelBaseProcessor& processor)
			throw()
			:	ModelProcessor(processor),
				used_atoms_(),
				atom_set_()
		{
		}

		AtomBondModelBaseProcessor::~AtomBondModelBaseProcessor()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << (void *)this 
						 			 << " of class " << RTTI::getName<AtomBondModelBaseProcessor>() << std::endl;
			#endif 
		}

		void AtomBondModelBaseProcessor::clear()
			throw()
		{
			ModelProcessor::clear();
			atom_set_.clear();
			used_atoms_.clear();
		}

		void AtomBondModelBaseProcessor::set(const AtomBondModelBaseProcessor& processor)
			throw()
		{
			clear();
			ModelProcessor::set(processor);
		}

		const AtomBondModelBaseProcessor& AtomBondModelBaseProcessor::operator = (const AtomBondModelBaseProcessor& processor)
			throw()
		{
			set(processor);
			return *this;
		}

		void AtomBondModelBaseProcessor::swap(AtomBondModelBaseProcessor& processor)
			throw()
		{
			ModelProcessor::swap(processor);
		}

		bool AtomBondModelBaseProcessor::start()
		{
			return ModelProcessor::start();
		}

		void AtomBondModelBaseProcessor::clearComposites()
			throw()
		{
			atom_set_.clear();
			used_atoms_.clear();
		}
				
		void AtomBondModelBaseProcessor::dump(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			ModelProcessor::dump(s, depth + 1);

			BALL_DUMP_DEPTH(s, depth);
			s << "used atoms: " << used_atoms_.size() << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void AtomBondModelBaseProcessor::buildBondModels_()
    {
			// generate bond primitive
			const Atom* second_atom_ptr = 0;
			AtomBondConstIterator bond_it;
			List<const Atom*>::ConstIterator atom_it;

			// for all used atoms
			for (atom_it  = getAtomList_().begin();
					 atom_it != getAtomList_().end(); ++atom_it)
			{
				// for all bonds connected from first- to second atom
				BALL_FOREACH_ATOM_BOND(**atom_it, bond_it)
				{
					if (*atom_it != bond_it->getSecondAtom())
					{
						second_atom_ptr = bond_it->getSecondAtom();
					}
					else
					{
						second_atom_ptr = bond_it->getFirstAtom();
					}

					// use only atoms with greater handles than first atom
					// or
					// second atom not a used atom, but smaller as the first atom
					// process bond between them		
					if (**atom_it < *second_atom_ptr && getAtomSet_().has(second_atom_ptr))
					{
						visualiseBond_(*bond_it);
					}
				}
			}
    }

		void AtomBondModelBaseProcessor::visualiseBond_(const Bond& /*bond*/)
			throw()
		{
		}

		bool AtomBondModelBaseProcessor::createGeometricObjects()
			throw()
		{
			ring_atoms_.clear();

			vector<vector<Atom*> >::iterator it = rings_.begin();
			for(; it != rings_.end(); it++)
			{
				vector<Atom*>& ring = *it;
				if (ring.size() != 5 && ring.size() != 6) continue;

				vector<Atom*>::iterator ait = ring.begin();

				for (; ait != ring.end(); ait++)
				{
					ring_atoms_.insert(*ait);
				}
			}

			buildBondModels_();
			visualiseRings_();
			rings_.clear();
			return true;
		}

		Processor::Result AtomBondModelBaseProcessor::operator () (Composite& composite)
		{
			if (!RTTI::isKindOf<Residue>(composite)) return Processor::CONTINUE;

			RingPerceptionProcessor rpp;
			rpp.calculateSSSR(rings_, (*reinterpret_cast<Residue*>(&composite)));
			return Processor::CONTINUE;
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/MODELS/atomBondModelBaseProcessor.iC>
#		endif

	} // namespace VIEW
} // namespace BALL
