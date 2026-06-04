// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/STRUCTURE/atomTyper.h>
#include <BALL/KERNEL/atomHandle.h>      // v2.2 H3d.A: D-H3.8 opt-in

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/lineBasedFile.h>
#include <BALL/STRUCTURE/smartsMatcher.h>
#include <BALL/KERNEL/forEach.h>

#include <BALL/SYSTEM/timer.h>
//    #define BALL_MMFF94_TEST
#define BALL_DEBUG_TEST

using namespace std;

namespace BALL 
{


StringHashMap<float> AtomTyper::rule_times = StringHashMap<float>();

AtomTyper::AtomTyper()
	: number_expected_fields_(4)
{
}

AtomTyper::AtomTyper(const AtomTyper& t)
{
	names_ = t.names_;
	rules_ = t.rules_;
	types_ = t.types_;
}
		
bool AtomTyper::setup(const String& filename)
{
	names_.clear();
	rules_.clear();
	types_.clear();
	element_to_rules_.clear();

	LineBasedFile infile(filename);

	vector<vector<String> > lines;
	vector<String> fields;
	
	while (infile.readLine())
	{
		const String line = infile.getLine();

		// comments and empty lines
		if (line.size() < 2 || line[0] == '*') continue;
		
		if (line.split(fields, "|") < number_expected_fields_)
		{
			Log.error() << "Error in " << __FILE__ << " " << __LINE__ << " : " 
									<< filename << " Not enough fields in one line " 
									<< line << std::endl;
			
			continue;
		}

		lines.push_back(vector<String>());

		const Position pos = lines.size() - 1;
		vector<String>& vs = lines[pos];
		vs.resize(number_expected_fields_);

		for (Position p = 0; p < number_expected_fields_; p++)
		{
			fields[p].trim();
			vs[p] = fields[p];
		}

		if (fields[3] == "") continue;

		names_.push_back(fields[1]);
		types_.push_back(fields[2].toInt());
		rules_.push_back(fields[3]);

		// the element for this rule:
		String element = fields[0];
		if (element == "") element = "X";
		if (!element_to_rules_.has(element)) 
		{
			element_to_rules_[element] = vector<Position>();
		}
		element_to_rules_[element].push_back(rules_.size() - 1);
	}

	if (!specificSetup_()) 
	{
		Log.error() << "Error while parsing line in File " << filename << std::endl;
		return false;
	}

	return true;
}

void AtomTyper::assignTo(System& s)
{
	MoleculeIterator mit = s.beginMolecule();
	for (;+mit; ++mit)
	{
		assignTo(*mit);
	}
}

// v2.2 H3d.A: AtomTyper::setRings / AtomTyper::setAromaticRings are defined
// in atomTyperBoundary.C (separate TU, kept out of the H3a.5 handle-key-leak
// gate). The boundary translates the upstream v0 ring-set spelling into
// the sid-keyed internal members; this TU consumes them.

void AtomTyper::assignTo(Molecule& mol)
{
	// v2.2 H3d.A (D-H3.8): atoms_ + atoms_to_match keyed by StableId.
	// Single-store discipline: capture the molecule's store at first
	// push; foreign-store atoms skipped. rings_ / aromatic_rings_ are
	// already sid-keyed (setRings/setAromaticRings translate on entry).
	atoms_.clear();
	MoleculeStore* mol_store = nullptr;

	AtomIterator ait = mol.beginAtom();
	for(; +ait; ++ait)
	{
		ait->setType(-1);
		ait->setTypeName(BALL_ATOM_DEFAULT_TYPE_NAME);
		MoleculeStore* s = ait->getStore();
		if (s == nullptr) continue;
		if (mol_store == nullptr) mol_store = s;
		else if (s != mol_store) continue;
		atoms_.insert(s->stable_id(ait->getStoreIndex()));
	}

	// Defense-in-depth: if no atoms or no store, bail out cleanly.
	if (atoms_.size() == 0 || mol_store == nullptr) return;

	SmartsMatcher sm;

	/////////////////////////////////////////////////////////////
	// Give the SmartsMatcher the smallest set of smallest rings via the
	// sid-keyed overload (H3d.A SmartsMatcher).
	vector<vector<MoleculeStore::StableId> > rings_vector;
	rings_vector.reserve(rings_.size());
	for (Position p = 0; p < rings_.size(); p++)
	{
		vector<MoleculeStore::StableId> ring;
		ring.reserve(rings_[p].size());
		HashSet<MoleculeStore::StableId>::Iterator it = rings_[p].begin();
		for (; +it; ++it)
		{
			ring.push_back(*it);
		}
		rings_vector.push_back(std::move(ring));
	}
	sm.setSSSR(rings_vector, *mol_store);

	auto resolve = [mol_store](MoleculeStore::StableId sid) -> Atom*
	{
		MoleculeStore::Index idx = mol_store->atom_idx_by_stable_id(sid);
		if (idx == MoleculeStore::UNKNOWN_STABLE_ID) return nullptr;
		return mol_store->back_ptr(idx);
	};

	/////////////////////////////////////////////////////////////
	// Next we iterate over all rule sets:
	std::set<MoleculeStore::StableId> atoms_to_match;
	HashSet<MoleculeStore::StableId>::Iterator atoms_it;
	HashMap<String, vector<Position> >::Iterator eit = element_to_rules_.begin();
	for (; eit != element_to_rules_.end(); ++eit)
	{
		if (atoms_.size() == 0) break;

		// the element of this rule set:
		String element = eit->first;
		// hetero elements, like ions can share one rule set:
		bool any_atom = element == "X";

		// collect all atoms, which have the same element, as the current rule set:
		atoms_to_match.clear();
		atoms_it = atoms_.begin();
		for (; +atoms_it; ++atoms_it)
		{
			Atom* atom_ptr = resolve(*atoms_it);
			if (atom_ptr == nullptr) continue;
			const String& this_element = atom_ptr->getElement().getSymbol();
			if (!any_atom)
			{
				if (this_element == element)
				{
					atoms_to_match.insert(*atoms_it);
				}
			}
			else
			{
				if (!element_to_rules_.has(this_element))
				{
					atoms_to_match.insert(*atoms_it);
				}
			}
		}

		// if no atoms to be matched: continue to next rule set
		if (atoms_to_match.empty()) continue;

		const vector<Position>& rule_numbers = eit->second;
		for (Index nr = (Index)rule_numbers.size() - 1; nr >= 0; nr--)
		{
			/// Match the given rule from the vector to a given set of atoms
			Position rule = rule_numbers[nr];

			try
			{
				// v2.2 H3d.A: use SmartsMatcher's sid-keyed match overload.
				vector<std::set<MoleculeStore::StableId> > result;
   #ifdef BALL_MMFF94_TEST
				if (!rule_times.has(names_[rule])) rule_times[names_[rule]] = 0;
				Timer t;
				t.start();
   #endif
				sm.match(result, mol, rules_[rule], atoms_to_match, *mol_store);
				if (result.empty()) continue;

				// iterate over all matched atoms, and set values accordingly:
				for (Position pos = 0; pos < result.size(); pos++)
				{
					std::set<MoleculeStore::StableId>& set = result[pos];
					if (set.size() != 1)
					{
						Log.error() << "Problem with smarts expr " << rules_[rule]
												<< " : More than one atom in group matched"
												<< " in " << __FILE__ << " " << __LINE__ << std::endl;
						continue;
					}

					const MoleculeStore::StableId matched_sid = *set.begin();
					Atom* atom_ptr = resolve(matched_sid);
					if (atom_ptr == nullptr) continue;
					Atom& atom = *atom_ptr;
					atom.setType(types_[rule]);
					atom.setTypeName(names_[rule]);
					assignSpecificValues_(atom);
					atoms_to_match.erase(matched_sid);
					atoms_.erase(matched_sid);
				}
   #ifdef BALL_MMFF94_TEST
				t.stop();
				rule_times[names_[rule]] += t.getClockTime();
   #endif
			}
			catch(Exception::ParseError& e)
			{
				Log.error() << e << std::endl;
			}
		} // rules for one element
	} // all element rule sets
}


} // namespace BALL
