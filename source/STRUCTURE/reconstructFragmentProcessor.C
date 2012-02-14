// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/STRUCTURE/reconstructFragmentProcessor.h>

#include <list>
#include <vector>

#include <BALL/KERNEL/PTE.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/MATHS/matrix44.h>
#include <BALL/DATATYPE/stringHashMap.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/STRUCTURE/structureMapper.h>
	
using namespace std;

// #define BALL_DEBUG_RECONSTRUCTFRAGMENTPROCESSOR

#ifdef BALL_DEBUG_RECONSTRUCTFRAGMENTPROCESSOR
#	define DEBUG(a) Log.info() << "ReconstructFragmentProcessor: " << a << std::endl;
#else
#	define DEBUG(a)
#endif

namespace BALL 
{

	/////////////////////////////////////////////////////////////////
	//		ReconstructFragmentProcessor												     //
	/////////////////////////////////////////////////////////////////	

	void ReconstructFragmentProcessor::setFragmentDB(const FragmentDB& db)
	{
		fragment_db_ = &const_cast<FragmentDB&>(db);
	}

	const FragmentDB* ReconstructFragmentProcessor::getFragmentDB() const
	{
		return fragment_db_;
	}

	/**	Identify two reference atoms.
			Performs a breadth-first search for two additional heavy atoms
			starting from the center atom. These atoms are used as 
			anchor points for attaching the next atom.
	*/
	Triple<bool, const Atom*, const Atom*> 
	ReconstructFragmentProcessor::getTwoReferenceAtoms
		(const Atom& ref_center_atom, const HashSet<const Atom*>& allowed)
		
	{
		Triple<bool, const Atom*, const Atom*> result(false, 0, 0);
		
		// a hash set to remember all those atoms we have already visited
		list<const Atom*> atom_list;
		atom_list.push_back(&ref_center_atom);
		
		// abort if we found the three first atoms (beyond the center atom)
		// or we are running out of fresh atoms
		list<const Atom*>::iterator current(atom_list.begin());
		while ((atom_list.size() < 3) && (current != atom_list.end()))
		{
			Atom::BondConstIterator bond((*current)->beginBond());
			for (; +bond; ++bond)
			{
				const Atom* next_atom = bond->getPartner(**current);
				if (allowed.has(next_atom) 
						&& (find(atom_list.begin(), atom_list.end(), next_atom) == atom_list.end()))
				{
					atom_list.push_back(next_atom);
					if (atom_list.size() > 2)
					{
						bond = (*current)->endBond();
						break;
					}
				}
			}
			
			// try the bonds of the next atom in the list
			current++;
		}
		
		// copy the two  reference atoms to the result 
		// (omit the first atom, which is the center atom!)
		result.first = (atom_list.size() == 3);
		current = atom_list.begin();
		current++;
		if (current != atom_list.end())
		{
			result.second = *current;
			current++;
		}
		if (current != atom_list.end())
		{
			result.third  = *current;
		}

		return result;
	}
	
	// start function of ReconstructFragmentProcessor
	// nothing important is done here
	bool ReconstructFragmentProcessor::start()
	{
		inserted_atoms_.clear();
		
		if (fragment_db_ == 0)
		{
			Log.error() << "ReconstructFragmentProcessor: no FragmentDB defined. "
									<< "Use setFragmentDB() to associate a fragment database." << std::endl;
			return false;
		}
		
		return true;
	}
	
	// Processor finish method
	bool ReconstructFragmentProcessor::finish()
	{
		return true;
	}

	// Processor application method
	Processor::Result ReconstructFragmentProcessor::operator () (Fragment& object)
	{
		// abort if the object is not a residue (only residues are 
		// contained in the fragment DB)																				
		if (!RTTI::isKindOf<Residue>(object))
		{
			return Processor::CONTINUE;
		}

		// cast the object to a residue
		Residue& residue = dynamic_cast<Residue&>(object);

		// get the reference fragment from the fragment DB
		const Fragment* reference_fragment = fragment_db_->getReferenceFragment(residue);

		// complain if no reference fragment could be found
		if (reference_fragment == 0)
		{
			Log.warn() << "ReconstructFragmentProcessor: no reference fragment found for " 
							   << residue.getName() << ":" << residue.getID() << std::endl;
			return Processor::CONTINUE;
		}

		// Reconstruct the atoms and count the number of new atoms.
		// number_of_inserted_atoms_ += reconstructFragment(residue, *reference_fragment);
		list<Atom*> inserted_atoms;
		list<Atom*>::iterator it;

		inserted_atoms = reconstructFragment(residue, *reference_fragment);

		for (it = inserted_atoms.begin(); it != inserted_atoms.end(); ++it)
		{
			inserted_atoms_.push_back(*it);
		}

		// Next residue.
		return Processor::CONTINUE;
	}

	ReconstructFragmentProcessor::ReconstructFragmentProcessor(const FragmentDB& db)
		:	fragment_db_(&db),
			inserted_atoms_()
	{
	}
	
	// copy constructor	
	ReconstructFragmentProcessor::ReconstructFragmentProcessor(const ReconstructFragmentProcessor& rfp)
		:	UnaryProcessor<Fragment>(rfp),
			fragment_db_(rfp.fragment_db_),
			inserted_atoms_(rfp.inserted_atoms_)
	{
	}
	
	// default constructor	
	ReconstructFragmentProcessor::ReconstructFragmentProcessor()
		:	fragment_db_(0),
			inserted_atoms_()
	{
	}
	
	// destructor	
	ReconstructFragmentProcessor::~ReconstructFragmentProcessor()
	{
		fragment_db_ = 0;
	}

	list<Atom*>& ReconstructFragmentProcessor::getInsertedAtoms()
	{
		return inserted_atoms_;
	}

	// return the number of inserted atoms
	Size ReconstructFragmentProcessor::getNumberOfInsertedAtoms() const
	{
		return inserted_atoms_.size();
	}
 
	list<Atom*> ReconstructFragmentProcessor::reconstructFragment
		(Fragment& fragment, const Fragment& tplate)
	{
		// We count the number of atoms created.
		Size number_of_inserted_atoms = 0;
		list<Atom*> inserted_atoms;
		
		// Get a copy of the atom names occurring in the current reference fragment....
		StringHashMap<Atom*> name_to_atom;
		AtomIterator it = fragment.beginAtom();
		for (; +it; ++it)
		{
			name_to_atom.insert(pair<String, Atom*>(it->getName(), const_cast<Atom*>(&*it)));
			DEBUG("inserting " << it->getName() << " = " << (void*)(&*it) << " into name_to_atom map. ")
		}
		
		// ...and remove the names of existing atoms.
		// At the same time, we construct a hash map relating the atom names
		// of the residues to the corresponding atom pointers.
		HashMap<const Atom*, Atom*> tpl_to_res;
		HashSet<const Atom*> transformed;
		AtomConstIterator cit = tplate.beginAtom();
		for (; +cit; ++cit)
		{	
			if (name_to_atom.has(cit->getName()))
			{
				// remember that the coordinates of this one are correct
				Atom* res_atom = name_to_atom[cit->getName()];
				transformed.insert(&*cit);
				tpl_to_res.insert(std::pair<const Atom*, Atom*>(&*cit, res_atom));
				DEBUG("inserting " << (void*)&*cit << " = " << (void*)(res_atom) << " into tpl_to_res map. ")
			}
			else
			{
				// We create a copy of the existing atom and insert it into
				// the residue. Coordinates are bogus, but we'll correct that
				// later on.
				Atom* new_atom = reinterpret_cast<Atom*>(cit->create(false));
				fragment.insert(*new_atom);
				tpl_to_res.insert(std::pair<const Atom*, Atom*>(&*cit, new_atom));
				// update the atom count
				number_of_inserted_atoms++;
				inserted_atoms.push_back(new_atom);
				DEBUG("creating atom " << (void*)(new_atom) << " for tpl atom " << cit->getName() << " (" << (void*)&*cit << ")")
			}
		}

		// We've now made sure that all atoms of the tplate exist in the 
    // reconstructed residue as well (careful, not the other way round!)
		// we can now start to adjust the atom coordinates.

		// If no atoms were in common, there's not much we can do...
		// Trivial solution: no atoms are actually matched to each 
		// other, so we just leave the coordinates the way they
		// are (copy of the tpl coordinates) and return.
		if (!transformed.isEmpty())
		{
			// Otherwise, we start adjusting coordinates
			// We use a hash set for BFS
			HashSet<const Atom*> visited;
			list<const Atom*> stack;
			stack.push_back(*transformed.begin());
			while (!stack.empty())
			{
				const Atom* current = stack.front();
				stack.erase(stack.begin());
				visited.insert(current);

				DEBUG("center is " << (void*)current << " (" << current->getFullName() << ") visited = " 
							<< (visited.has(current)) << " transformed = " << transformed.has(current) 
							<< " @ " << current->getPosition())
				DEBUG("residue atom is @ " << tpl_to_res[current]->getPosition()  << " (dist = " 
							<< tpl_to_res[current]->getPosition().getDistance(current->getPosition()) << ")")

				for (Atom::BondConstIterator bond = current->beginBond(); +bond; ++ bond)
				{
					const Atom* next = bond->getPartner(*current);
					DEBUG("examining " << (void*)next << " (" << next->getFullName() << ") visited = " 
								<< (visited.has(next)) << " transformed = " << transformed.has(next))
					if (!visited.has(next))
					{
						stack.push_back(next);
						visited.insert(next);
						if (!transformed.has(next))
						{
							DEBUG("searching reference atoms for "<< next->getFullName())
							Triple<bool, const Atom*, const Atom*> ref_atoms;
							ref_atoms = getTwoReferenceAtoms(*current, transformed);
							DEBUG("reference atoms: " << (ref_atoms.second == 0 ? String("-") : ref_atoms.second->getFullName())
										<< " / " << (ref_atoms.third == 0 ? String("-") : ref_atoms.third->getFullName()))

							Matrix4x4 T;
							if (ref_atoms.first == true)
							{
								// we can map all three atoms, great!
								DEBUG("mapping three atoms: " << tpl_to_res[current]->getFullName() << "/" 
											<< tpl_to_res[ref_atoms.second]->getFullName() << "/" << tpl_to_res[ref_atoms.third]->getFullName())
								DEBUG("onto:                " << current->getFullName() << "/" 
											<< ref_atoms.second->getFullName() << "/" << ref_atoms.third->getFullName())
								DEBUG("from: " << tpl_to_res[current]->getPosition() << "/" 
											<< tpl_to_res[ref_atoms.second]->getPosition() << "/" << tpl_to_res[ref_atoms.third]->getPosition())
								DEBUG("to:   " << current->getPosition() << "/" 
											<< ref_atoms.second->getPosition() << "/" << ref_atoms.third->getPosition())
								T = StructureMapper::matchPoints
											(current->getPosition(), ref_atoms.second->getPosition(), ref_atoms.third->getPosition(),
											 tpl_to_res[current]->getPosition(), 
											 tpl_to_res[ref_atoms.second]->getPosition(), 
											 tpl_to_res[ref_atoms.third]->getPosition());
								DEBUG("found two reference atoms, mapped with T =\n" << T)
							}
							else 
							{
								// We could map the two center atoms only, which corresponds to 
								// a simple translation by the difference of the two atom positions.
								T.setIdentity();
								T.setTranslation(tpl_to_res[current]->getPosition() - current->getPosition());
								DEBUG("translating by " << T)
							}

							// Transform the coordinates of the atom we're interest in
							tpl_to_res[next]->setPosition(T * tpl_to_res[next]->getPosition());

							// Remember that we already took care of that guy.
							transformed.insert(next);
							DEBUG(next->getFullName() << " is transformed: " << tpl_to_res[next]->getPosition() << "/" << next->getPosition())
							DEBUG("distance = " << tpl_to_res[next]->getPosition().getDistance(next->getPosition()))
						}
					}
				}
			}
		}
		
		// Return the number of atoms created.
		// return number_of_inserted_atoms;
		return inserted_atoms;
	}

} // namespace BALL

