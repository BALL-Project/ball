// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: atomBondModelBaseProcessor.C,v 1.15.18.1 2007/03/25 22:02:28 oliver Exp $
//

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
			:	ModelProcessor(),
			  used_atoms_(),
				atom_set_()
		{
		}

		AtomBondModelBaseProcessor::AtomBondModelBaseProcessor(const AtomBondModelBaseProcessor& processor)
			:	ModelProcessor(processor),
				used_atoms_(),
				atom_set_()
		{
		}

		AtomBondModelBaseProcessor::~AtomBondModelBaseProcessor()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << this << " of class AtomBondModelBaseProcessor" << std::endl;
			#endif 
		}

		void AtomBondModelBaseProcessor::clear()
		{
			ModelProcessor::clear();
			atom_set_.clear();
			used_atoms_.clear();
			rings_.clear();
			ring_atoms_.clear();
		}

		void AtomBondModelBaseProcessor::set(const AtomBondModelBaseProcessor& processor)
		{
			clear();
			ModelProcessor::set(processor);
		}

		const AtomBondModelBaseProcessor& AtomBondModelBaseProcessor::operator = (const AtomBondModelBaseProcessor& processor)
		{
			set(processor);
			return *this;
		}

		void AtomBondModelBaseProcessor::clearComposites()
		{
			atom_set_.clear();
			used_atoms_.clear();
		}
				
		void AtomBondModelBaseProcessor::dump(ostream& s, Size depth) const
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
			list<const Atom*>::const_iterator atom_it;

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
		{
		}

		bool AtomBondModelBaseProcessor::createGeometricObjects()
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
			Fragment* residue = dynamic_cast<Fragment*>(&composite);
			if (residue == 0) 
			{
				return Processor::CONTINUE;
			}

			RingPerceptionProcessor rpp;
			try
			{
				rpp.calculateSSSR(rings_, *residue);
			}
			catch (Exception::GeneralException e)
			{
				Log.warn() << "Exception of type " << e.getName() << " occured in line "
					         << e.getLine() << " of " << e.getFile() << endl;
				Log.warn() << "Error message: " << e.getMessage() << endl;
			}
			return Processor::CONTINUE;
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/MODELS/atomBondModelBaseProcessor.iC>
#		endif

	} // namespace VIEW
} // namespace BALL
