// $Id: standardPredicates.C,v 1.3 2000/05/22 17:42:39 anker Exp $

#include <BALL/KERNEL/standardPredicates.h>

#include <BALL/CONCEPT/selectable.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/nucleotide.h>
#include <BALL/KERNEL/bond.h>

#include <algorithm>

using namespace std;

namespace BALL 
{

	// Atom name predicate

	bool AtomNamePredicate::operator () (const Atom& atom) const
	{
    return (atom.getName() == argument_);
 	}

	// Atom type predicate

	bool AtomTypePredicate::operator () (const Atom& atom) const
	{
		return (atom.getTypeName() == argument_);
 	}


	// element predicate

	bool ElementPredicate::operator () (const Atom& atom) const
	{
		return (atom.getElement().getSymbol() == argument_);
	}

	// residue predicate

	bool ResiduePredicate::operator () (const Atom& atom) const
	{
		const Residue*	res = atom.getAncestor(RTTI::getDefault<Residue>());
		if (res != 0)
		{
			return (res->getName() == argument_);
		}

		return false;
	}
	
	// residue ID predicate

	bool ResidueIDPredicate::operator () (const Atom& atom) const
	{
    const Residue*	res = atom.getAncestor(RTTI::getDefault<Residue>());
		if (res != 0)
		{
			return (res->getID() == argument_);
		}
		
		return false;
	}
	
	// protein predicate

	bool ProteinPredicate::operator () (const Atom& /* atom */) const
	{
		//BAUSTELLE
		return false;
	}
	
	// chain predicate

	bool ChainPredicate::operator () (const Atom& /* atom */) const
	{
		//BAUSTELLE
		return false;
	}
	
	// secondary structure predicate

	bool SecondaryStructurePredicate::operator () (const Atom& /* atom */) const
	{
		//BAUSTELLE
		return false;
	}
	
	// solvent predicate

	bool SolventPredicate::operator () (const Atom& /* atom */) const
	{
		//BAUSTELLE
		return false;
	}
	
	// backbone predicate

	bool BackBonePredicate::operator () (const Atom& atom) const
	{
		if (atom.hasAncestor(RTTI::getDefault<Residue>()))
		{
			String name = RTTI::castTo<Atom>(atom)->getName();
			if ((name == "C") || (name == "N") || (name == "CA") || (name == "O"))
			{
				return true;
			}
		}

		return false;
	}
	
	// nucleotide predicate

	bool NucleotidePredicate::operator () (const Atom& atom) const
	{
		return RTTI::isKindOf<Nucleotide>(atom);
	}

	Size InRingPredicate::limitcount = 0;
	
	InRingPredicate::InRingPredicate()
	{
		limitcount = 0;
	}

	bool InRingPredicate::dfs(const Atom* atom, const Atom* first_atom,
		const int limit) const
	{
		// the following recursive function performs an ad-hoc dfs and returns
		// true, if a ring was found and false otherwise.

		if (++limitcount > limit)
		{
			return false;
		}
		else
		{
			if ((limitcount == limit) && (atom == first_atom)) 
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		Size i;
		for (i = 0; i < atom->countBonds(); ++i)
		{
			if (dfs(atom->getBond(i)->getPartner(*atom), first_atom, limit))
			{
				return true;
			}
		}
		return false;
	}


	bool InRingPredicate::operator () (const Atom& atom) const
	{
		int n = argument_.toInt();
		limitcount = 0;
		if (dfs (&atom,&atom,n))
		{
			return true;
		}
		else
		{
			return false;
		}

	}

	bool doubleBondsPredicate::operator () (const Atom& atom) const
	{
		String s = argument_;
		s.trim();
		int n = ((String) s[1]).toInt();
		int count = 0;
		Size i;
		for (i = 0; i < atom.countBonds(); ++i)
		{
			if ((atom.getBond(i))->getOrder() == Bond::ORDER__DOUBLE)
			{
				count++;
			}
		}

		String op = s[0];
		if (op == "<")
		{
			if (count < n)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			if (op == ">") 
			{
				if (count > n)
				{
					return true;
				}
				else 
				{
					return false;
				}
			}
			else
			{
				if (count == n)
				{
					return true;
				}
				else 
				{
					return false;
				}
			}
		}
		Log.error() << "doubleBond::operator (): Illegal operator " << s[0] << endl;
		return false;
	}
	
	bool tripleBondsPredicate::operator () (const Atom& atom) const
	{
		String s = argument_;
		s.trim();
		int n = ((String) s[1]).toInt();
		int count = 0;
		Size i;
		for (i = 0; i < atom.countBonds(); ++i)
		{
			if ((atom.getBond(i))->getOrder() == Bond::ORDER__TRIPLE)
			{
				count++;
			}
		}
		String op = s[0];
		if (op == "<")
		{
				if (count < n)
				{
					return true;
				}
				else
				{
					return false;
				}
		}
		else
		{
			if (op == ">") 
			{
				if (count > n)
				{
					return true;
				}
				else 
				{
					return false;
				}
			}
			else
			{
				if (count == n)
				{
					return true;
				}
				else 
				{
					return false;
				}
			}
		}
		Log.error() << "tripleBond::operator (): Illegal operator " << s[0] << endl;
		return false;
	}
	
	bool aromaticBondsPredicate::operator () (const Atom& atom) const
	{
		String s = argument_;
		s.trim();
		int n = ((String) s[1]).toInt();
		int count = 0;
		Size i;
		for (i = 0; i < atom.countBonds(); ++i)
		{
			if ((atom.getBond(i))->getOrder() == Bond::ORDER__AROMATIC)
			{
				count++;
			}
		}
		String op = s[0];
		if (op == "<")
		{
			if (count < n)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			if (op == ">") 
			{
				if (count > n)
				{
					return true;
				}
				else 
				{
					return false;
				}
			}
			else
			{
				if (count == n)
				{
					return true;
				}
				else 
				{
					return false;
				}
			}
		}
		Log.error() << "aromaticBond::operator (): Illegal operator " << s[0] << endl;
		return false;
	}
	
	bool numberOfBondsPredicate::operator () (const Atom& atom) const
	{
		String s = argument_;
		s.trim();
		int n = ((String) s[1]).toInt();
		int count = atom.countBonds(); 
		String op = s[0];
		if (op == "<")
		{
			if (count < n)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			if (op == ">") 
			{
				if (count > n)
				{
					return true;
				}
				else 
				{
					return false;
				}
			}
			else
			{
				if (count == n)
				{
					return true;
				}
				else 
				{
					return false;
				}
			}
		}
		Log.error() << "numberOfBonds::operator (): Illegal operator " << s[0] << endl;
		return false;
	}
	
	bool connectedToPredicate::parse(const String& group, list<String>* subs) const
	{
		// BAUSTELLE

		int depth = 0;
		Size position = 0;
		String* tmp;

		for (; position < group.size(); ++position)
		{
			switch (group[position]) 
			{
				case '(' : 
					if (depth == 0)
					{
						tmp = new String;
					}
					if (depth > 1)
					{
						// We're in depth 1, so append this char.
						tmp = tmp + '(';
					}
					depth++;
					break;
				case ')' :
					if (depth > 1)
					{
						// We're in depth 1, so append this char.
						tmp = tmp + ')';
					}
					depth--;
					if (depth == 0) 
					{
						subs->push_back((String) *tmp);
					}
					if (depth < 0)
					{
						Log.error() << "connectedToPredicate::parse(): Got negative bracket count" << endl;
						return false;
					}
					break;
				default :
					// if (group(position).isAlpha())
					// {
						if (depth > 1)
						{
							tmp = tmp + group[position];
						}
					// }
					// else
					// {
					//	Log.error() << "connectedToPredicate::parse(): Found an illegal character: " << group[position] << endl;
					//	return false;
					// }
			}
		}
		return true;
	}

	bool connectedToPredicate::find(const String& group, HashSet<Bond*>* found) const
	{
		// BAUSTELLE

		list<String>* subgroups = new list<String>;
		if (!parse(group,subgroups))
		{
			Log.error() << "connectedToPredicate::find(): couldn't parse()." << endl;
			return false;
		}
		
		list<HashSet <Bond*> > L;
		list<String>::iterator subgroups_it=subgroups->begin();

		// for every subgroup find a definite match
		for (;subgroups_it != subgroups->end(); ++subgroups_it) 
		{
			HashSet<Bond*>* deeper = new HashSet<Bond*>;
			if (find(*subgroups_it, deeper))
			{
				Log.info() << "Subgroup " << *subgroups_it  << " found" << endl;
				L.push_back(*deeper);
			}
			else
			{
				// This subgroup couldn't be found, so the whole subgroup cannot be
				// matched.
				Log.info() << "Subgroup " << *subgroups_it  << " couldn't be found" << endl;
				return false;
			}
		}

		// Now L contains a list of size number of subgroups.
		while (L.size() > 0) 
		{
			list<Bond*> del_list;
			list<Bond*>::iterator del_it;
			list<HashSet <Bond*> >::iterator it = L.begin();
				
			for (; it != L.end(); ++it)
			{
				if (it->size() == 1)
				{
					if (std::find(del_list.begin(), del_list.end(), *(it->begin())) != del_list.end())
					{
						// This match was assumed to be definite, but obviously
						// isn't. So the pattern couldn't be applied.
						// Log.info() << "del_list already has " << /* it->begin() */ << endl;
						Log.info() << "del_list already has this..." << endl;
						return false;
					}
					// this match is now assumed to be definite, so delete it from
					// all other lists...
					del_list.push_back(*(it->begin()));

					// ... and save it for return
					found->insert(*(it->begin()));
					
					// Now clear the hashset.
					it->clear();
				}
			}

			for (it = L.begin(); it != L.end(); ++it) 
			{
				// Delete the contents of del_list in all Hashsets
				for (del_it = del_list.begin(); del_it != del_list.end(); ++del_it)
				{
					if (it->has(*del_it))
					{
						it->erase(*del_it);
					}
					if (it->size() == 0)
					{
						// BAUSTELLE: Das könnte ins Auge gehen...
						L.erase(it);
					}
				}
			}

			// If there are ambiguous results, just grab one and make it definite
			// by deleting a hashset containing it and all occurrences in all
			// other hashsets. 
			// (GREEDY).
	
			if (L.size() > 0)
			{
				Bond* grab = *(L.begin()->begin());
				found->insert(grab);
				L.erase(L.begin());
				for (it = L.begin(); it != L.end(); ++it)
				{
					if (it->has(grab))
					{
						it->erase(grab);
					}
				}
			}
		}
		// L was emptied and no errors occurred, so return true.
		return true;
	}
	
	bool connectedToPredicate::operator () (const Atom& atom) const
	{
		//BAUSTELLE
		return false;

		HashSet<Bond*>* found = new HashSet<Bond*>;
		if (find(argument_, found))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool sp3HybridizedPredicate::operator () (const Atom& atom) const
	{
		Size i;
		for (i = 0; i < atom.countBonds(); ++i)
		{
			if ((atom.getBond(i))->getOrder() != Bond::ORDER__SINGLE)
			{
				return false;
			}
		}
		return true;
	}

	bool sp2HybridizedPredicate::operator () (const Atom& atom) const
	{
		int dcount = 0;
		int acount = 0;
		Size i;
		for (i = 0; i < atom.countBonds(); ++i)
		{
			if ((atom.getBond(i))->getOrder() != Bond::ORDER__DOUBLE)
			{
				dcount++;
			}
			if ((atom.getBond(i))->getOrder() != Bond::ORDER__AROMATIC)
			{
				acount++;
			}
		}
		if ((dcount == 1) || (acount > 1))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool spHybridizedPredicate::operator () (const Atom& atom) const
	{
		int dcount = 0;
		int tcount = 0;
		Size i;
		for (i = 0; i < atom.countBonds(); ++i)
		{
			if ((atom.getBond(i))->getOrder() != Bond::ORDER__DOUBLE)
			{
				dcount++;
			}
			if ((atom.getBond(i))->getOrder() != Bond::ORDER__TRIPLE)
			{
				tcount++;
			}
		}
		if ((dcount == 2) || (tcount == 1))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

} // namespace BALL
