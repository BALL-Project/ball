#include <BALL/KERNEL/standardPredicates.h>
#include <BALL/KERNEL/atomIterator.h>
#include <BALL/KERNEL/bondIterator.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/system.h>
#include <BALL/DATATYPE/hashSet.h>
#include <BALL/DATATYPE/stringHashMap.h>
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/FORMAT/HINFile.h>

#include <stack>

using namespace std;
using namespace BALL;

// This code comes from the FresnoRotation forcefield component. It should
// go int some decent implementaion in STRUCTURE or another more
// appropriate place.

// This method searches for atoms that are part of cycles (and thus not
// rotatable).

void cycleDFS_(const Atom* atom,
		HashSet<const Atom*>& visited,
		HashSet<const Bond*>& tree,
		stack<const Bond*>& possible_cycle_bonds,
		HashSet<const Bond*>& cycle_bonds,
		int& cycle_count)
	throw()
{
	// mark this atom as visited
	visited.insert(atom);

	// iterate over all bonds of this atom (i. e. this node)
	AtomBondConstIterator it = atom->beginBond();
	for (; +it; ++it)
	{
		const Bond* bond = &*it;
		const Atom* partner = it->getPartner(*atom);

		// if our partner atom wasn't visited yet, start dfs() with it
		if (visited.has(partner) == false)
		{

			// save this bond as a candidate for a cycle bond
			possible_cycle_bonds.push(bond);

			// save this bond in the tree we are building
			tree.insert(bond);

			// apply dfs() to the partner atom
			cycleDFS_(partner, visited, tree,
					possible_cycle_bonds, cycle_bonds, cycle_count);

			// if the current bond wasn't pop()'d by the cycle recognizing
			// heuristic, remove it from the stack. We won't need it anymore
			if (possible_cycle_bonds.top() == bond) 
			{
				possible_cycle_bonds.pop();
			}
		}
		// if we already visited this atom and our bond is not in the tree, we
		// found a cycle. Let's look which bonds actually build the cycle.
		else
		{
			if (!tree.has(bond) && !cycle_bonds.has(bond))
			{
				cycle_count++;
				cycle_bonds.insert(bond);

				const Bond* tmp;
				bool keep_on_searching = true;

				do
				{
					// PARANOIA
					if (possible_cycle_bonds.empty())
					{
						cerr << "Something's wrong here, no possible bonds left" 
							<< endl;
					}
					else
					{
					// /PARANOIA

						tmp = possible_cycle_bonds.top();

						// PARANOIA
						if (tmp == 0)
						{
							cerr << "ERROR: got a NULL ptr." << endl;
							// ?????
							// exception
						}
						// /PARANOIA

						// if we found a bond that itself is part of a cycle, we found
						// two cycles that are sharing a bond. We needn't look further,
						if (cycle_bonds.has(tmp))
						{
							/*
							// PARANOIA
							HashSet<const Bond*>::ConstIterator set_it = cycle_bonds.begin();
							for (; +set_it; ++set_it)
							{
								if ((*set_it)->getFirstAtom() == partner)
								{
									if (((*set_it)->getSecondAtom() == tmp->getFirstAtom())
										|| ((*set_it)->getSecondAtom() == tmp->getSecondAtom()))
									{
										Log.info() << "found missing CYCLE bond: "
											<< (*set_it)->getFirstAtom()->getFullName() << ":" 
											<< (*set_it)->getSecondAtom()->getFullName() << endl;
									}
								}
								if ((*set_it)->getSecondAtom() == partner)
								{
									if (((*set_it)->getFirstAtom() == tmp->getFirstAtom())
										|| ((*set_it)->getFirstAtom() == tmp->getSecondAtom()))
									{
										Log.info() << "found missing CYCLE bond: "
											<< (*set_it)->getFirstAtom()->getFullName() << ":" 
											<< (*set_it)->getSecondAtom()->getFullName() << endl;
									}
								}
							}
							// /PARANOIA
							*/
							keep_on_searching = false;
						}
						else
						{
							possible_cycle_bonds.pop();
							cycle_bonds.insert(tmp);
						}
					}
				}
				while ((tmp->getFirstAtom() != partner) 
						&& (tmp->getSecondAtom() != partner)
						&& (!possible_cycle_bonds.empty())
						&& (keep_on_searching == true));

				possible_cycle_bonds.push(bond);
			}
		}
	}
}


bool find_rotation(const System& system, std::vector<const Bond*>& rotatable_bonds_)
{

	// clear the vector of rotatable bonds
  //std::vector<const Bond*> rotatable_bonds_;
	
	// quadratic run time. not nice.

	Size algorithm_type_ = 1;

	Size verbosity = 10;

	// define the bondlengths for the bond guessing algorithm

	StringHashMap< pair<float, float> > bondlengths;

	const Size ALGORITHM__GUESS = 1;
	const Size ALGORITHM__DATABASE = 2;

	if (algorithm_type_ == ALGORITHM__GUESS)
	{
		// ?????
		// This is not nice and should be done using an INIFile
		pair<float, float> tmp;

		// we need shorter bondlenghts for C-C and C-N
		// tmp = pair<float, float>(1.54, 1.55);
		tmp = pair<float, float>(1.52, 1.55);
		bondlengths["C"] = tmp;
		// tmp = pair<float, float>(1.47, 1.48);
		tmp = pair<float, float>(1.44, 1.48);
		bondlengths["N"] = tmp;
		tmp = pair<float, float>(1.43, 1.47);
		bondlengths["O"] = tmp;
		// S is not taken into account by the original paper.
		// tmp = pair<float, float>(1.80, 1.84);
		// bondlengths["S"] = tmp;
	}

	Size guessed_bonds = 0;

	String sym1, sym2;
	StringHashMap< pair<float, float> >::ConstIterator hash_it;

	// define and initialize all variables we need for extracting the
	// cycles (or rings)
	HashSet<const Atom*> visited;
	HashSet<const Bond*> tree;
	stack<const Bond*> possible_cycle_bonds;
	HashSet<const Bond*> cycle_bonds;
	int cycle_count = 0;

	const Atom* atom1 = &*system.beginAtom();
	const Atom* atom2;

	// initialize the hash map

	cycleDFS_(atom1, visited,
			tree, possible_cycle_bonds, cycle_bonds, cycle_count);

	if (verbosity >= 90) Log.info() << "Tree size: " << tree.size() << endl;

	// initialize the data structures for another dfs and count the heavy
	// atoms in the system 

	Sp2HybridizedPredicate isSp2;
	Sp3HybridizedPredicate isSp3;
	ConnectedToPredicate hasH3Group;
	hasH3Group.setArgument("(H)(H)(H)");
	ConnectedToPredicate hasH2Group;
	hasH2Group.setArgument("(H)(H)");
	ConnectedToPredicate hasF3Group;
	hasF3Group.setArgument("(F)(F)(F)");

	ElementPredicate isCarbon;
	isCarbon.setArgument("C");
	ElementPredicate isNitrogen;
	isNitrogen.setArgument("N");
	ConnectedToPredicate hasAromaticBondedOxygen;
	hasAromaticBondedOxygen.setArgument("(~O)");

	bool A_sp2;
	bool A_sp3;
	bool B_sp2;
	bool B_sp3;
	bool A_CO;
	bool B_CO;

	bool found_rotatable_bond = false;

	// a rotatable bond is *any* bond between sp2-sp3 or sp3-sp3 hybridized
	// atoms that are not in rings and not bonds to terminal NH3 or CH3

	// for every non-cylce bond in the tree find out whether it is
	// rotational and count the number of heavy atoms on either side of the
	// bond
	HashSet<const Bond*>::ConstIterator tree_it = tree.begin();
	for (; +tree_it; ++tree_it)
	{
		atom1 = (*tree_it)->getFirstAtom();
		atom2 = (*tree_it)->getSecondAtom();

		if (!cycle_bonds.has(*tree_it))
		{

			if (verbosity >= 100)
			  {
			    //Log.info() << atom1->getFullName() << "---" << atom2->getFullName()
			  //	<< endl;
				//Log.info() << "hasH3Group(*atom1) " << hasH3Group(*atom1) << endl;
				//Log.info() << "hasH2Group(*atom1) " << hasH2Group(*atom1) << endl;
				//Log.info() << "hasF3Group(*atom1) " << hasF3Group(*atom1) << endl;
				//Log.info() << "isCarbon(*atom1) " << isCarbon(*atom1) << endl;
				//Log.info() << "isNitrogen(*atom1) " << isNitrogen(*atom1) << endl;
				//Log.info() << "hasH3Group(*atom2) " << hasH3Group(*atom2) << endl;
				//Log.info() << "hasH2Group(*atom2) " << hasH2Group(*atom2) << endl;
				//Log.info() << "hasF3Group(*atom2) " << hasF3Group(*atom2) << endl;
				//Log.info() << "isCarbon(*atom2) " << isCarbon(*atom2) << endl;
				//Log.info() << "isNitrogen(*atom2) " << isNitrogen(*atom2) << endl;
			  }

			if (!((hasH3Group(*atom1) & (isCarbon(*atom1) | isNitrogen(*atom1)))
						| (hasH2Group(*atom1) & isNitrogen(*atom1))
						| (hasF3Group(*atom1) & isCarbon(*atom1))
						| (hasH3Group(*atom2) & (isCarbon(*atom2) | isNitrogen(*atom2)))
						| (hasH2Group(*atom2) & isNitrogen(*atom2))
						| (hasF3Group(*atom2) & isCarbon(*atom2))))
			{

				if (algorithm_type_ == ALGORITHM__GUESS)
				{
					float dist = (atom1->getPosition() - atom2->getPosition()).getLength();
					sym1 = atom1->getElement().getSymbol();
					sym2 = atom2->getElement().getSymbol();

					if (sym1 == "C")
					{
						if (bondlengths.has(sym2))
						{
							if (verbosity >= 100)
							{
								if (sym2 == "N")
								{
								  if (verbosity >= 100);
									  //Log.info() << atom1->getFullName() << "---" 
									  //	<< atom2->getFullName() << endl;
									//Log.info() << "length: " << dist << endl;
								}
							}
							float lower = bondlengths[sym2].first;
							float upper = bondlengths[sym2].second;
							if ((dist > (lower * 0.975)) && (dist <= (upper * 1.025)))
							{
								if (verbosity >= 10)
								{
								  //Log.info() << "found rotatable bond:" << endl;
								  //Log.info() << atom1->getFullName() << "---" 
								  //<< atom2->getFullName() << endl;
										//Log.info() << "length: " << dist << endl;
								}
								found_rotatable_bond = true;
								guessed_bonds++;
							}
						}
					}
					else
					{
						if (sym2 == "C")
						{
							if (bondlengths.has(sym1))
							{
								if (verbosity >= 100)
								{
									if (sym2 == "N")
									{
									  //	Log.info() << atom1->getFullName() << "---" 
									  //	<< atom2->getFullName() << endl;
									  //Log.info() << "length: " << dist << endl;
									}
								}
								float lower = bondlengths[sym1].first;
								float upper = bondlengths[sym1].second;
								if ((dist > (lower * 0.975)) && (dist <= (upper * 1.025)))
								{
									if (verbosity >= 10)
									{
									  //Log.info() << "found rotatable bond:" << endl;
									  //Log.info() << atom1->getFullName() << "---" 
									  //<< atom2->getFullName() << endl;
											//Log.info() << "length: " << dist << endl;
									}
									found_rotatable_bond = true;
									guessed_bonds++;
								}
							}
						}
					}
				}
				else
				{
					if (algorithm_type_ == ALGORITHM__DATABASE)
					{
						// ?????
						// make this more efficient
						A_sp2 = isSp2(*atom1);
						A_sp3 = isSp3(*atom1);
						B_sp2 = isSp2(*atom2);
						B_sp3 = isSp3(*atom2);
						A_CO = (isCarbon(*atom1) & hasAromaticBondedOxygen(*atom1));
						B_CO = (isCarbon(*atom2) & hasAromaticBondedOxygen(*atom2));

						if (verbosity >= 100)
						{
/* 							Log.info() << "A SP2: " << A_sp2 << endl; */
/* 							Log.info() << "A SP3: " << A_sp3 << endl; */
/* 							Log.info() << "B SP2: " << B_sp2 << endl; */
/* 							Log.info() << "B SP3: " << B_sp3 << endl; */
/* 							Log.info() << "A CO: " << A_CO << endl; */
/* 							Log.info() << "B CO: " << B_CO << endl; */
						}

						if (((A_sp2 & B_sp3) | (B_sp2 & A_sp3) | (A_sp3 & B_sp3))
								| ((A_CO & B_sp3 & isCarbon(*atom2)) 
									| (A_sp3 & B_CO & isCarbon(*atom1))) == true)
						{
							if (verbosity >= 10)
							{
							/* 	Log.info() << "found possible rotatable bond: "  */
/* 									<< atom1->getFullName() << "---" << atom2->getFullName() */
/* 									<< endl; */
							}
							found_rotatable_bond = true;
						}
					}
					else
					{
					  //	Log.error() << "Unknown algorithm type" << endl;
						return false;
					}
				}
			}
		}

		// Save rotatable bonds
		if (found_rotatable_bond == true)
		{
			found_rotatable_bond = false;
			rotatable_bonds_.push_back(*tree_it);
		}
	}

	if (verbosity >= 9)
	{
	  Log.info() << "Found " << rotatable_bonds_.size() 
			<< " rotatable bonds" << endl << endl;
	}

	return true;
}

