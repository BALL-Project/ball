// $Id: standardPredicates.C,v 1.23 2001/05/24 14:47:13 anker Exp $

#include <BALL/KERNEL/standardPredicates.h>

#include <BALL/CONCEPT/selectable.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/chain.h>
#include <BALL/KERNEL/secondaryStructure.h>
#include <BALL/KERNEL/nucleotide.h>
#include <BALL/KERNEL/bond.h>

#include <algorithm>

using namespace std;

namespace BALL 
{
	// True predicate

	bool TruePredicate::operator () (const Atom& /* atom */) const
		throw()
	{
    return true;
 	}

	// selected predicate

	bool SelectedPredicate::operator () (const Atom& atom) const
		throw()
	{
    return atom.isSelected();
 	}

	// Atom name predicate

	bool AtomNamePredicate::operator () (const Atom& atom) const
		throw()
	{
    return (atom.getName() == argument_);
 	}

	// Atom type predicate

	bool AtomTypePredicate::operator () (const Atom& atom) const
		throw()
	{
		return (atom.getTypeName() == argument_);
 	}


	// element predicate

	bool ElementPredicate::operator () (const Atom& atom) const
		throw()
	{
		return (atom.getElement().getSymbol() == argument_);
	}

	// residue predicate

	bool ResiduePredicate::operator () (const Atom& atom) const
		throw()
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
		throw()
	{
    const Residue*	res = atom.getAncestor(RTTI::getDefault<Residue>());
		if (res != 0)
		{
			return (res->getID() == argument_);
		}
		
		return false;
	}
	
	// protein predicate

	bool ProteinPredicate::operator () (const Atom& atom) const
		throw()
	{
		const Protein* protein = atom.getAncestor(RTTI::getDefault<Protein>());
		if (protein != 0)
		{
			return (protein->getName() == argument_);
		}

		return false;
	}
	
	// chain predicate

	bool ChainPredicate::operator () (const Atom& atom) const
		throw()
	{
 		const Chain* chain = atom.getAncestor(RTTI::getDefault<Chain>());
		if (chain != 0)
		{
			return (chain->getName() == argument_);
		}

		return false;
	}
	
	// secondary structure predicate

	bool SecondaryStructurePredicate::operator () (const Atom& atom) const
		throw()
	{
 		const SecondaryStructure* sec_struct = atom.getAncestor(RTTI::getDefault<SecondaryStructure>());
		return ((sec_struct != 0) && (sec_struct->getName() == argument_));
	}
	
	// solvent predicate

	bool SolventPredicate::operator () (const Atom& atom) const
		throw()
	{
		const Molecule* molecule = atom.getMolecule();
		return ((molecule != 0) && (molecule->hasProperty(Molecule::IS_SOLVENT)));
	}
	
	// molecule predicate

	bool MoleculePredicate::operator () (const Atom& atom) const
		throw()
	{
		const Molecule* molecule = atom.getMolecule();
		return ((molecule != 0) && (molecule->getName() == argument_));
	}

	// backbone predicate

	bool BackBonePredicate::operator () (const Atom& atom) const
		throw()
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
	
	// nucleic acid predicate

	bool NucleicAcidPredicate::operator () (const Atom& atom) const
		throw()
	{
		// BAUSTELLE
		// return RTTI::isKindOf<NucleicAcid>(atom);
	}

	// nucleotide predicate

	bool NucleotidePredicate::operator () (const Atom& atom) const
		throw()
	{
		return RTTI::isKindOf<Nucleotide>(atom);
	}

	// in ring predicate

	bool InRingPredicate::dfs_(const Atom& atom, const Atom& first_atom,
			const Size limit, HashSet<const Bond*>& visited) const
		throw()
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
				if (dfs_(*descend, first_atom, limit-1, my_visited))
				{
					return true;
				}
			}
		}
		// No partner matched
		return false;
	}


	bool InRingPredicate::operator () (const Atom& atom) const
		throw()
	{
		int n;
		if (argument_.size() == 1)
		{
			if (argument_.isDigit())
			{
				n = argument_.toInt();
			}
			else
			{
				Log.error() << "InRingPredicate::operator () (): "
					<< "Expected a number < 9: " << argument_ << endl;
				return false;
			}
		}
		else
		{
			Log.error() << "InRingPredicate::operator () (): "
				<< "Expected a number < 9: " << argument_ << endl;
			return false;
		}

		// There are no rings with less than 3 atoms
		if (n < 3) 
		{
			return false;
		}

		HashSet<const Bond*> visited;
		if (dfs_(atom,atom,n,visited))
		{
			return true;
		}
		else
		{
			return false;
		}

	}


	bool DoubleBondsPredicate::operator () (const Atom& atom) const
		throw()
	{
		return testPredicate_(atom, Bond::ORDER__DOUBLE);
	}


	bool DoubleBondsPredicate::testPredicate_(const Atom& atom, 
			Bond::Order order) const
		throw()
	{
		String s = argument_;
		s.trim();

		if (s.size() > 2)
		{
			// There can only be an operator followed by a number < 9
			Log.error() << "DoubleBondsPredicate::operator () (): "
				<< "argument_ too long " << endl;
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
		throw()
	{
		return testPredicate_(atom, Bond::ORDER__SINGLE);
	}

	
	bool TripleBondsPredicate::operator () (const Atom& atom) const
		throw()
	{
		return testPredicate_(atom, Bond::ORDER__TRIPLE);
	}
	
	bool AromaticBondsPredicate::operator () (const Atom& atom) const
		throw()
	{
		return testPredicate_(atom, Bond::ORDER__AROMATIC);
	}

	bool NumberOfBondsPredicate::operator () (const Atom& atom) const
		throw()
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


	bool ConnectedToPredicate::parse_(const String& group,
			std::list<std::pair<String,String> >& subs) const
		throw()
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
						this_pair.second = tmp;
						subs.push_back(this_pair);
					}
					if (depth < 0)
					{
						Log.error() << "ConnectedToPredicate::parse_(): Got negative bracket count" << endl;
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
			Log.error() << "ConnectedToPredicate::parse_(): Got positive bracket count" << endl; 
			return false;
		}
		return true;
	}


	bool ConnectedToPredicate::bondOrderMatch_(const String& bond_description,
			const Bond::Order order) const
		throw()
	{
		Bond::Order required_order;
		bool result = false;
		if (bond_description[0] != '.')
		{
			switch (bond_description[0]) 
			{
				case '-' : required_order = Bond::ORDER__SINGLE; break;
				case '=' : required_order = Bond::ORDER__DOUBLE; break;
				case '#' : required_order = Bond::ORDER__TRIPLE; break;
				case '~' : required_order = Bond::ORDER__AROMATIC; break;
			}
			result = (required_order == order);
		}
		else
		{
			result = true;
		}

		return result;
	}


	bool ConnectedToPredicate::findAndTest_(const String& group,
			const Atom& atom, const Bond* source) const
		throw()
	{
		if ((group[0] == atom.getElement().getSymbol()) || (group[0] == '*'))
		{
			return find_(group, atom, source);
		}
		else
		{
			return false;
		}
	}

	bool ConnectedToPredicate::find_(const String& group, const Atom& atom,
			const Bond* source) const
		throw()
	{
		// BAUSTELLE

		// ANNAHME: Nur Elemente mit EINEM Buchstaben. Der Rest muss nuch
		// irgendwie gefummelt werden. Wahrscheinlich über Symbolfolgen, die
		// aus parse_() rausfallen.

		// Now we have to find pattern matches...

		// subgroups is a pair consisting of a String describing the bond
		// that is binding the subgroup described by another String.

		std::list< std::pair<String, String> > subgroups;

		if (!parse_(group, subgroups))
		{
			Log.error() << "ConnectedToPredicate::find_(): couldn't parse_()." 
				<< endl;
			return false;
		}

		// If the bond description matches a bond of the atom, descend the
		// "tree" and search for subgroup matches

		std::list< HashSet<const Bond*> > L;
		std::list< std::pair<String, String> >::iterator subgroups_it;

		// go through all bonds of this atom and through all substrings
		// with matching bond.

		const Bond* bond;
		for (subgroups_it = subgroups.begin(); subgroups_it !=
				 subgroups.end(); ++subgroups_it) 
		{
			if (subgroups.size() >= atom.countBonds())
			{
				// Log.info() << "Too few bonds (" << atom.countBonds() 
				//					 << " instead of " << subgroups.size() << ")" << endl;
				// return false;
			}

			HashSet<const Bond*> deeper; 

			// Iteriere über alle Bindungen von atom.
			for (Size i = 0; i < atom.countBonds(); ++i)
			{
				bond = atom.getBond(i);
				// Log.info() << "Bond: " << atom.getFullName() << " - " <<
				//	bond->getPartner(atom)->getFullName();

				// Follow this bond only if its type matches and it isn't the bond
				// we came from. This implies special treatment of cycles.
				if (bond != source)
				{
					// Log.info() << ": not source";
					if (bondOrderMatch_(subgroups_it->first, bond->getOrder()))
					{
						// Log.info() << ", order match";
						if (subgroups_it->second.size() < 1) 
						{
							Log.error() << "ConnectedToPredicate::find_(): "
								<< "subgroup too short: " 
								<< subgroups_it->second.size() << " " 
								<< subgroups_it->second << endl;
							return false;
						}

						if (subgroups_it->second.size() == 1)
						{
							if ((bond->getPartner(atom)->getElement().getSymbol()
										== subgroups_it->second) || (subgroups_it->second == '*'))
							{
								// Log.info() << ", base match.";
								deeper.insert(bond);
							}
						}
						else
						{
							if (findAndTest_(subgroups_it->second, 
										*(atom.getBond(i)->getPartner(atom)), bond))
							{
								// Log.info() << ", recursion.";
								deeper.insert(bond);
							}
						}
					}
				}
				// Log.info() << endl;
			}
			if (!deeper.isEmpty())
			{
				L.push_back(deeper);
			}
		}

		// Log.info() << "L: " << L.size();
		if (L.empty()  || (L.size() != subgroups.size()))
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
			list< list< HashSet<const Bond*> >:: iterator > hash_del_list;

			// Log.info() << "Sizes of hash sets before first block:";
			for (; it != L.end(); ++it)
			{
				//writeit(*it);
				// Log.info() << " " << it->size();
				if (it->size() == 1)
				{
					if (std::find(del_list.begin(), del_list.end(),
								*(it->begin())) != del_list.end())
					{
						// This match was assumed to be definite, but obviously
						// isn't. So the pattern couldn't be applied.
						// Log.info() << "DOUBLE SINGLE." << endl;
						return false;
					}
					// this match is now assumed to be definite, so delete it from
					// all other lists...
					del_list.push_back(*(it->begin()));

					// ... and save it for return
					// deeper.insert(*(it->begin()));

					// Now clear the hashset.
					it->clear();
					hash_del_list.push_back(it);
				}
			}
			// Log.info() << endl;

			// now erase the empty hashsets in L

			list< list< HashSet<const Bond*> >:: iterator >::iterator
				hash_del_it;

			for (hash_del_it = hash_del_list.begin(); hash_del_it !=
				hash_del_list.end(); ++hash_del_it)
			{
				L.erase(*hash_del_it);
			}

			// delete definite items from all other hashsets

			// Log.info() << "Sizes of remaining sets before GREEDY:";
			for (it = L.begin(); it != L.end(); ++it) 
			{
				// Delete the contents of del_list in all Hashsets
				for (del_it = del_list.begin(); del_it != del_list.end(); ++del_it)
				{
					// delete definite items from those hash sets
					if (it->has(*del_it))
					{
						it->erase(*del_it);
					}


					// if there is an empty hashset, something went wrong
					if (it->size() == 0)
					{
						return false;
					}
				}
				//writeit(*it);
				// Log.info() << " " << it->size();
			}
			// Log.info() << endl;

			// SECOND BLOCK: If there are ambiguous results, just grab one
			// and make it definite by deleting a hashset containing it and
			// all occurrences in all other hashsets. (GREEDY).

			// Log.info() << "Sizes of remaining sets after GREEDY:";

			if (L.size() > 0)
			{
				const Bond* grab = *(L.begin()->begin());
				// deeper.insert(grab);
				L.erase(L.begin());
				for (it = L.begin(); it != L.end(); ++it)
				{
					if (it->has(grab))
					{
						it->erase(grab);
					}

					//writeit(*it);
					// Log.info() << " " << it->size();

					if (it->size() == 0)
					{
						return false;
					}
				}
			}
			// Log.info() << endl;
		} while (L.size() > 0);

		// L was emptied and no errors occurred, so return true.
		return true;
	} 


	bool ConnectedToPredicate::operator () (const Atom& atom) const
		throw()
	{
		// BAUSTELLE
		if (find_(argument_, atom, 0))
		{
			return true;
		}
		else
		{
			return false;
		}
	}


	bool SpHybridizedPredicate::operator () (const Atom& atom) const
		throw()
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
		throw()
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
		throw()
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
