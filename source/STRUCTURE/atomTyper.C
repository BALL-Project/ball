// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/STRUCTURE/atomTyper.h>

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

void AtomTyper::assignTo(Molecule& mol)
{
	atoms_.clear();

	AtomIterator ait = mol.beginAtom();
	for(; +ait; ++ait)
	{
		ait->setType(-1);
		ait->setTypeName(BALL_ATOM_DEFAULT_TYPE_NAME);
		atoms_.insert(&*ait);
	}

	SmartsMatcher sm;

	/////////////////////////////////////////////////////////////
	// Give the SmartsMatcher the smallest set of smallest rings:
	vector<vector<Atom*> > rings_vector;
	for (Position p = 0; p < rings_.size(); p++)
	{
		rings_vector.push_back(vector<Atom*>());
		HashSet<Atom*>::Iterator it = rings_[p].begin();
		for (; +it; ++it)
		{
			rings_vector[p].push_back(*it);
		}
	}
 	sm.setSSSR(rings_vector);

	/////////////////////////////////////////////////////////////
	// Next we iterate over all rule sets:
	std::set<const Atom*> atoms_to_match;
	HashSet<Atom*>::Iterator atoms_it;
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
			const String& this_element = (**atoms_it).getElement().getSymbol();
			if (!any_atom) 
			{
				// match only the atoms with the correct element for this rule
				if (this_element == element)
				{
					atoms_to_match.insert(*atoms_it);
				}
			}
			else
			{
				// match all atoms with elements, which share a rule set with other elements
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
				vector<std::set<const Atom*> > result;
//      Log.error() << names_[rule]         << " "  << __FILE__ << "  " << __LINE__<< std::endl;
   #ifdef BALL_MMFF94_TEST
				if (!rule_times.has(names_[rule])) rule_times[names_[rule]] = 0;
				Timer t;
				t.start();
   #endif
				sm.match(result, mol, rules_[rule], atoms_to_match);
				if (result.empty()) continue;

				// iterate over all matched atoms, and set values accordingly:
				for (Position pos = 0; pos < result.size(); pos++)
				{
					std::set<const Atom*>& set = result[pos];
					if (set.size() != 1) 
					{
						Log.error() << "Problem with smarts expr " << rules_[rule]
												<< " : More than one atom in group matched"
												<< " in " << __FILE__ << " " << __LINE__ << std::endl;
						continue;
					}

					Atom& atom = *(Atom*)*set.begin();
					atom.setType(types_[rule]);
					atom.setTypeName(names_[rule]);
					assignSpecificValues_(atom);
					// erase the atom from the sets of atoms, that are still to be matched
					atoms_to_match.erase(&atom);
					atoms_.erase(&atom);
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
