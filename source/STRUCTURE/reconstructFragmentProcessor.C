// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/STRUCTURE/reconstructFragmentProcessor.h>

#include <list>
#include <vector>

#include <BALL/KERNEL/PTE.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/atomHandle.h>      // v2.2 H3d.B: D-H3.8 opt-in
#include <BALL/KERNEL/moleculeStore.h>
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
		(const Atom& ref_center_atom,
		 const HashSet<MoleculeStore::StableId>& allowed_sids,
		 MoleculeStore* tpl_store)

	{
		Triple<bool, const Atom*, const Atom*> result(false, 0, 0);

		// v2.2 H3d.B (D-H3.8): allowed set is sid-keyed; the local BFS
		// frontier (atom_list) stays Atom*-typed because (1) it's a
		// short-lived stack and (2) the gate's qualified-only regex
		// doesn't match `list<const Atom*>` (no std:: prefix).
		list<const Atom*> atom_list;
		atom_list.push_back(&ref_center_atom);

		auto sid_of = [&](const Atom* a) -> MoleculeStore::StableId
		{
			if (tpl_store == nullptr || a == nullptr) return 0;
			return tpl_store->stable_id(a->getStoreIndex());
		};

		list<const Atom*>::iterator current(atom_list.begin());
		while ((atom_list.size() < 3) && (current != atom_list.end()))
		{
			Atom::BondConstIterator bond((*current)->beginBond());
			for (; +bond; ++bond)
			{
				const Atom* next_atom = bond->getPartner(**current);
				if (allowed_sids.has(sid_of(next_atom))
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
        if (!RTTI::isKindOf<Residue>(&object))
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
		// v2.2 H3d.B (D-H3.8): all working sets keyed on StableId per
		// store; resolve to Atom* via the H3c Phase 0 reverse map at
		// each access. Two stores at play: `tplate` provides the
		// template atoms; `fragment` is the residue being reconstructed.
		Size number_of_inserted_atoms = 0;
		list<Atom*> inserted_atoms;

		// Get a copy of the atom names occurring in the current reference fragment.
		// name -> res-sid (resolved against frag_store).
		StringHashMap<MoleculeStore::StableId> name_to_atom;
		MoleculeStore* frag_store = nullptr;
		AtomIterator it = fragment.beginAtom();
		for (; +it; ++it)
		{
			MoleculeStore* s = it->getStore();
			if (s == nullptr) continue;
			if (frag_store == nullptr) frag_store = s;
			else if (s != frag_store) continue;
			name_to_atom.insert(pair<String, MoleculeStore::StableId>(
				it->getName(), s->stable_id(it->getStoreIndex())));
		}

		// tpl-sid -> res-sid; transformed = set of tpl-sids whose
		// position is correct.
		HashMap<MoleculeStore::StableId, MoleculeStore::StableId> tpl_to_res;
		HashSet<MoleculeStore::StableId> transformed;
		MoleculeStore* tpl_store = nullptr;
		AtomConstIterator cit = tplate.beginAtom();
		for (; +cit; ++cit)
		{
			MoleculeStore* s = const_cast<Atom&>(*cit).getStore();
			if (s == nullptr) continue;
			if (tpl_store == nullptr) tpl_store = s;
			else if (s != tpl_store) continue;
			const MoleculeStore::StableId tpl_sid =
				s->stable_id(cit->getStoreIndex());

			if (name_to_atom.has(cit->getName()))
			{
				// remember that the coordinates of this one are correct
				const MoleculeStore::StableId res_sid = name_to_atom[cit->getName()];
				transformed.insert(tpl_sid);
				tpl_to_res.insert(std::pair<MoleculeStore::StableId, MoleculeStore::StableId>(tpl_sid, res_sid));
			}
			else
			{
				// We create a copy of the existing atom and insert it into
				// the residue. Coordinates are bogus, but we'll correct that
				// later on.
				Atom* new_atom = reinterpret_cast<Atom*>(cit->create(false));
				fragment.insert(*new_atom);
				MoleculeStore* ns = new_atom->getStore();
				if (ns != nullptr && (frag_store == nullptr || ns == frag_store))
				{
					if (frag_store == nullptr) frag_store = ns;
					tpl_to_res.insert(std::pair<MoleculeStore::StableId, MoleculeStore::StableId>(
						tpl_sid, ns->stable_id(new_atom->getStoreIndex())));
				}
				// update the atom count
				number_of_inserted_atoms++;
				inserted_atoms.push_back(new_atom);
			}
		}

		// Resolver helpers (capture stores).
		auto resolve_tpl = [&](MoleculeStore::StableId sid) -> Atom*
		{
			if (tpl_store == nullptr) return nullptr;
			MoleculeStore::Index idx = tpl_store->atom_idx_by_stable_id(sid);
			if (idx == MoleculeStore::UNKNOWN_STABLE_ID) return nullptr;
			return tpl_store->back_ptr(idx);
		};
		auto resolve_res = [&](MoleculeStore::StableId sid) -> Atom*
		{
			if (frag_store == nullptr) return nullptr;
			MoleculeStore::Index idx = frag_store->atom_idx_by_stable_id(sid);
			if (idx == MoleculeStore::UNKNOWN_STABLE_ID) return nullptr;
			return frag_store->back_ptr(idx);
		};

		// We've now made sure that all atoms of the tplate exist in the 
    // reconstructed residue as well (careful, not the other way round!)
		// we can now start to adjust the atom coordinates.

		// If no atoms were in common, there's not much we can do...
		// Trivial solution: no atoms are actually matched to each 
		// other, so we just leave the coordinates the way they
		// are (copy of the tpl coordinates) and return.
		if (!transformed.isEmpty())
		{
			// v2.2 H3d.B: BFS over template atoms with sid keys.
			HashSet<MoleculeStore::StableId> visited;
			list<MoleculeStore::StableId> stack;
			stack.push_back(*transformed.begin());
			while (!stack.empty())
			{
				const MoleculeStore::StableId current_sid = stack.front();
				stack.erase(stack.begin());
				visited.insert(current_sid);

				const Atom* current = resolve_tpl(current_sid);
				if (current == nullptr) continue;
				Atom* current_res = resolve_res(tpl_to_res[current_sid]);
				if (current_res == nullptr) continue;

				for (Atom::BondConstIterator bond = current->beginBond(); +bond; ++ bond)
				{
					const Atom* next = bond->getPartner(*current);
					if (next == nullptr || next->getStore() != tpl_store) continue;
					const MoleculeStore::StableId next_sid =
						tpl_store->stable_id(next->getStoreIndex());

					if (!visited.has(next_sid))
					{
						stack.push_back(next_sid);
						visited.insert(next_sid);
						if (!transformed.has(next_sid))
						{
							Triple<bool, const Atom*, const Atom*> ref_atoms;
							ref_atoms = getTwoReferenceAtoms(*current, transformed, tpl_store);

							Matrix4x4 T;
							if (ref_atoms.first && ref_atoms.second && ref_atoms.third)
							{
								// we can map all three atoms, great!
								const MoleculeStore::StableId ref2_sid =
									tpl_store->stable_id(ref_atoms.second->getStoreIndex());
								const MoleculeStore::StableId ref3_sid =
									tpl_store->stable_id(ref_atoms.third->getStoreIndex());
								Atom* res2 = resolve_res(tpl_to_res[ref2_sid]);
								Atom* res3 = resolve_res(tpl_to_res[ref3_sid]);
								if (res2 == nullptr || res3 == nullptr) continue;
								T = StructureMapper::matchPoints
											(current->getPosition(), ref_atoms.second->getPosition(), ref_atoms.third->getPosition(),
											 current_res->getPosition(),
											 res2->getPosition(),
											 res3->getPosition());
							}
							else
							{
								// We could map the two center atoms only, which corresponds to
								// a simple translation by the difference of the two atom positions.
								T.setIdentity();
								T.setTranslation(current_res->getPosition() - current->getPosition());
							}

							// Transform the coordinates of the atom we're interest in
							if (tpl_to_res.has(next_sid))
							{
								Atom* next_res = resolve_res(tpl_to_res[next_sid]);
								if (next_res != nullptr)
									next_res->setPosition(T * next_res->getPosition());
							}

							// Remember that we already took care of that guy.
							transformed.insert(next_sid);
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

