// $Id: standardPredicates.C,v 1.8 2000/05/24 15:12:49 anker Exp $

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

	InRingPredicate::InRingPredicate()
	{
	}

	bool InRingPredicate::dfs(const Atom& atom, const Atom& first_atom,
		const Size limit, HashSet<const Bond*>& visited) const
	{
		// the following recursive function performs an ad-hoc dfs and returns
		// true, if a ring was found and false otherwise.

		if (limit == 0) 
		{
			if (atom == first_atom) 
			{
				// Found first atom at limit
				return true;
			}
			else
			{
				// Reached limit without finding the first atom
				return false;
			}
		}
		Size i;
		const Bond* bond;
		Atom* descend;
		HashSet<const Bond*> my_visited(visited);

		// Now iterate over all Bonds an store the visited bonds.

		for (i = 0; i < atom.countBonds(); ++i)
		{
			bond = atom.getBond(i);
			if (!my_visited.has(bond))
			{
				descend = bond->getPartner(atom);
				my_visited.insert(bond);
				if (dfs(*descend, first_atom, limit-1, my_visited))
				{
					return true;
				}
			}
		}
		// No partner matched
		return false;
	}


	bool InRingPredicate::operator () (const Atom& atom) const
	{
		int n = argument_.toInt();
		// There are no rings with less than 3 atoms
		if (n < 3) 
		{
			return false;
		}

		HashSet<const Bond*> visited;
		if (dfs (atom,atom,n,visited))
		{
			return true;
		}
		else
		{
			return false;
		}

	}

	bool DoubleBondsPredicate::operator () (const Atom& atom) const
	{
		return testPredicate_(atom, Bond::ORDER__DOUBLE);
	}

	bool DoubleBondsPredicate::testPredicate_(const Atom& atom, 
			Bond::Order order) const
	{
		String s = argument_;
		s.trim();

		if (s.size() > 2)
		{
			// There can only be an operator followed by a number < 9
			Log.error() << "DoubleBondsPredicate::operator () (): argument_ too long " << endl;
			return false;
		}
		
		Size count = 0;
		Size i = 0;
		for (; i < atom.countBonds(); ++i)
		{
			if ((atom.getBond(i))->getOrder() == order)
			{
				count++;
			}
		}

		Size n;
		if (s.size() == 2)
		{
			n = ((String) s[1]).toInt();
			switch (s[0]) 
			{
				case '<' :
					if (count < n)
					{
						return true;
					}
					else
					{
						return false;
					}

				case '>' :
					if (count > n)
					{
						return true;
					}
					else 
					{
						return false;
					}

				case '=':
					if (count == n)
					{
						return true;
					}
					else 
					{
						return false;
					}

				default:
					Log.error() << "doubleBond::operator (): Illegal operator " 
						<< s[0] << endl;
					return false;
			}
		}
		else 
		{
			n = ((String) s[0]).toInt();
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
	
	bool SingleBondsPredicate::operator () (const Atom& atom) const
	{
		return testPredicate_(atom, Bond::ORDER__SINGLE);
	}
	
	bool TripleBondsPredicate::operator () (const Atom& atom) const
	{
		return testPredicate_(atom, Bond::ORDER__TRIPLE);
	}
	
	bool AromaticBondsPredicate::operator () (const Atom& atom) const
	{
		return testPredicate_(atom, Bond::ORDER__AROMATIC);
	}

	bool NumberOfBondsPredicate::operator () (const Atom& atom) const
	{
		String s = argument_;
		s.trim();

		if (s.size() > 2)
		{
			// There can only be an operator followed by a number < 9
			Log.error() << "DoubleBondsPredicate::operator () (): argument_ too long " << endl;
			return false;
		}
		
		Size count = atom.countBonds();

		Size n;
		if (s.size() == 2)
		{
			n = ((String) s[1]).toInt();
			switch (s[0]) 
			{
				case '<' :
					if (count < n)
					{
						return true;
					}
					else
					{
						return false;
					}

				case '>' :
					if (count > n)
					{
						return true;
					}
					else 
					{
						return false;
					}

				case '=':
					if (count == n)
					{
						return true;
					}
					else 
					{
						return false;
					}

				default:
					Log.error() << "doubleBond::operator (): Illegal operator " 
						<< s[0] << endl;
					return false;
			}
		}
		else 
		{
			n = ((String) s[0]).toInt();
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
	bool ConnectedToPredicate::parse(const String& group, 
		std::list<std::pair<String,String> >& subs) const
	{
		// BAUSTELLE
		// EIGENTLICH müssen hier Symbolfolgen erzeugt werden, keine Strings.

		int depth = 0;
		Size position = 0;
		std::pair<String, String> this_pair;
		String tmp;

		for (; position < group.size(); ++position)
		{
			switch (group[position]) 
			{
				case '(' : 
					if (depth == 0)
					{
						// if there was no bond symbol, we assume any bond.
						if (position < (group.size() - 1))
						{
							switch (group[position+1])
							{
								case '.' :
								case '-' :
								case '=' :
								case '#' :
								case '~' :
									this_pair.first = group[position+1];
									break;
								default:
									this_pair.first = '.';
							}
						} 
						else
						{
							Log.error() << "Invalid String" << endl;
							return false;
						}
						tmp = "";
					}
					if (depth > 0)
					{
						// We're in depth 1, so append this char.
						tmp = tmp + '(';
					}
					depth++;
					break;

				case ')' :
					depth--;
					if (depth > 0)
					{
						// We're in depth 1, so append this char.
						tmp = tmp + ')';
					}
					if (depth == 0) 
					{
						Log.info() << "push_back: " << tmp << endl;
						this_pair.second = tmp;
						subs.push_back(this_pair);
					}
					if (depth < 0)
					{
						Log.error() << "ConnectedToPredicate::parse(): Got negative bracket count" << endl;
						return false;
					}
					break;

				case '.' :
				case '-' :
				case '=' :
				case '#' :
				case '~' :
					if (depth > 1)
					{
						tmp = tmp + group[position];
					}
					break;

					/*
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
					if (depth == 1)
					{
						Log.info() << "about to copy " << group[position-1] << " " << 
							group[position] << " times" << endl;
						if (position == 0)
						{
							Log.error() << "encountered a number as first char." << endl;
							return false;
						}
						if (group[position-1] == ')')
						{
							// BAUSTELLE
						}
						else
						{
							// assumming the string to be correct, i. e.
							// group[position-1] is an element.
							for (int j = 1; j < ((String) group[position]).toInt(); ++j)
							{
								tmp = tmp + group[position-1];
							}
						}
					}
					break;
					*/

				/* 
				case 'H' : 
				case 'C' : 
				case 'O' : 
				case 'N' : 
					if (depth == 0)
					{
						Log.info() << "encountered Element without brackets, applying ()";
						tmp = tmp + '(' + group[position] + ')';
						Log.info() << tmp << endl;
					}
					else
					{
						tmp = tmp + group[position];
					}
					break;
				 */
				
				default :
					if (depth > 0)
					{
						tmp = tmp + group[position];
					}
			}
		}
		if (depth > 0)
		{
			Log.error() << "ConnectedToPredicate::parse(): Got positive bracket count" << endl; 
			return false;
		}
		return true;
	}

	bool ConnectedToPredicate::bondOrderMatch(const String& bond_description,
			const Bond::Order order) const
	{
		switch (bond_description[0]) 
		{
			case '.' :
				return true;

			case '-' :
				if (order == Bond::ORDER__SINGLE)
				{
					return true;
				}
				else
				{
					return false;
				}
				
			case '=' :
				if (order == Bond::ORDER__DOUBLE)
				{
					return true;
				}
				else
				{
					return false;
				}
				
			case '#' :
				if (order == Bond::ORDER__TRIPLE)
				{
					return true;
				}
				else
				{
					return false;
				}
				
			case '~' :
				if (order == Bond::ORDER__AROMATIC)
				{
					return true;
				}
				else
				{
					return false;
				}
		}
		return false;
	}

	bool ConnectedToPredicate::findAndTest(const String& group, const Atom&
			atom, HashSet<const Bond*>& found) const
	{
		if ((group[0] == atom.getElement().getSymbol()) || (group[0] == '*'))
		{
			return find(group, atom, found);
		}
		else
		{
			return false;
		}
	}

	bool ConnectedToPredicate::find
		(const String& group, const Atom& atom,
		 HashSet<const Bond*>& found) const
	{
		// BAUSTELLE

		// ANNAHME: Nur Elemente mit EINEM Buchstaben. Der Rest muss nuch
		// irgendwie gefummelt werden. Wahrscheinlich über Symbolfolgen, die
		// aus parse() rausfallen.

		// Now we have to find pattern matches...

		// subgroups is a pair consisting of a String describing the bond
		// that is binding the subgroup described by another String.

		std::list< std::pair<String, String> > subgroups;

		if (!parse(group, subgroups))
		{
			Log.error() << "ConnectedToPredicate::find(): couldn't parse()." 
				<< endl;
			return false;
		}

		// If the bond description matches a bond of the atom, descend the
		// "tree" and search for subgroup matches

		std::list< HashSet<const Bond*> > L;
		std::list< std::pair<String, String> >::iterator subgroups_it;

		// go through all bonds of this atom and through all substrings
		// with matching bond.

		for (subgroups_it = subgroups.begin(); subgroups_it !=
				subgroups.end(); ++subgroups_it) 
		{
			HashSet<const Bond*> deeper;
			for (Size i = 0; i < atom.countBonds(); ++i)
			{
				if (bondOrderMatch(subgroups_it->first, atom.getBond(i)->getOrder()))
				{
					if (subgroups_it->second.size() < 1) 
					{
						Log.error() << "subgroup too short: " <<
							subgroups_it->second.size() << " " << subgroups_it->second <<
							endl;
						return false;
					}

					if (subgroups_it->second.size() == 1)
					{
						if ((atom.getBond(i)->getPartner(atom)->getElement().getSymbol()
								== subgroups_it->second) || (subgroups_it->second == '*'))
						{
							deeper.insert(atom.getBond(i));
						}
						else 
						{
						}
					}
					else
					{
						if (findAndTest(subgroups_it->second,
									*(atom.getBond(i)->getPartner(atom)), deeper))
						{
						}
						else
						{
						}
					}
				}
			}
			if (!deeper.isEmpty())
			{
				L.push_back(deeper);
			}
		}

		if (L.empty()) 
		{
			return false;
		}

		if (L.size() != subgroups.size())
		{
			return false;
		}

		// Now L contains a list of size number of subgroups.

		while (L.size() > 0) 
		{

			// FIRST BLOCK: delete all definite cases 

			list<const Bond*> del_list;
			list<const Bond*>::iterator del_it;
			list<HashSet <const Bond*> >::iterator it = L.begin();

			for (; it != L.end(); ++it)
			{
				if (it->size() == 1)
				{
					if (std::find(del_list.begin(), del_list.end(),
								*(it->begin())) != del_list.end())
					{
						// This match was assumed to be definite, but obviously
						// isn't. So the pattern couldn't be applied.
						return false;
					}
					// this match is now assumed to be definite, so delete it from
					// all other lists...
					del_list.push_back(*(it->begin()));

					// ... and save it for return
					found.insert(*(it->begin()));

					// Now clear the hashset.
					it->clear();
				}
			}

			list< list< HashSet<const Bond*> >:: iterator > hash_del_list;

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
						// Don't insert iterators twice!

						if (std::find(hash_del_list.begin(), hash_del_list.end(),
									it) == hash_del_list.end()) 
						{
							hash_del_list.push_back(it);
						}
					}
				}
			}

			// now erase the empty hashsets in L

			list< list< HashSet<const Bond*> >:: iterator >::iterator
				hash_del_it;

			for (hash_del_it = hash_del_list.begin(); hash_del_it !=
				hash_del_list.end(); ++hash_del_it)
			{
				L.erase(*hash_del_it);
			}

			// SECOND BLOCK: If there are ambiguous results, just grab one
			// and make it definite by deleting a hashset containing it and
			// all occurrences in all other hashsets. (GREEDY).

			if (L.size() > 0)
			{
				const Bond* grab = *(L.begin()->begin());
				found.insert(grab);
				L.erase(L.begin());
				for (it = L.begin(); it != L.end(); ++it)
				{
					if (it->has(grab))
					{
						it->erase(grab);
					}
				}
			}
		} while (L.size() > 0);

		// L was emptied and no errors occurred, so return true.
		return true;
	} 

	bool ConnectedToPredicate::operator () (const Atom& atom) const
	{
		//BAUSTELLE
		HashSet<const Bond*> found;
		if (find(argument_, atom, found))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool SpHybridizedPredicate::operator () (const Atom& atom) const
	{
		int dcount = 0;
		int tcount = 0;
		Size i;
		for (i = 0; i < atom.countBonds(); ++i)
		{
			if ((atom.getBond(i))->getOrder() == Bond::ORDER__DOUBLE)
			{
				dcount++;
			}
			if ((atom.getBond(i))->getOrder() == Bond::ORDER__TRIPLE)
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

	bool Sp2HybridizedPredicate::operator () (const Atom& atom) const
	{
		int dcount = 0;
		int acount = 0;
		Size i;
		for (i = 0; i < atom.countBonds(); ++i)
		{
			if ((atom.getBond(i))->getOrder() == Bond::ORDER__DOUBLE)
			{
				dcount++;
			}
			if ((atom.getBond(i))->getOrder() == Bond::ORDER__AROMATIC)
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

	bool Sp3HybridizedPredicate::operator () (const Atom& atom) const
	{
		Size i;
		if (atom.countBonds() != 4)
		{
			return false;
		}
		else
		{
			for (i = 0; i < atom.countBonds(); ++i)
			{
				if ((atom.getBond(i))->getOrder() != Bond::ORDER__SINGLE)
				{
					return false;
				}
			}
			return true;
		}
	}


} // namespace BALL
