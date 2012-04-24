// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/STRUCTURE/kekulizer.h>

#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/PTE.h>
//#include <BALL/DATATYPE/hashSet.h>
#include <BALL/STRUCTURE/smartsMatcher.h>

using namespace std;

//  #define DEBUG_KEKULIZER
//  #define BALL_MMFF94_TEST

namespace BALL
{
				
bool Kekuliser::AtomInfo::operator < (const Kekuliser::AtomInfo& info) const
{
	return (*atom) < *info.atom;
}

Kekuliser::AtomInfo& Kekuliser::AtomInfo::operator = (const AtomInfo& ai) 
{
	atom 							= ai.atom;
	double_bond 			= ai.double_bond;
	abonds 						= ai.abonds;
	partner_id 				= ai.partner_id;
	curr_double 			= ai.curr_double;
	min_double 				= ai.min_double;
	max_double 				= ai.max_double;
	uncharged_double 	= ai.uncharged_double;
	return *this;
}


Kekuliser::Kekuliser()
	: use_formal_charges_(true)
{
	clear();
}

bool Kekuliser::setup(Molecule& mol)
{
	molecule_ = &mol;

	// collect aromatic bonds and atoms to speed up SMARTS matching:
	collectAromaticAtoms_();

	SmartsMatcher sm;
	// dont recalculate the smallest set of smallest rings:
	// no ring information needed for smarts matcher:
	vector<vector<Atom*> > rings_vector;
 	sm.setSSSR(rings_vector);

	Position nr_ca = 0;

	vector<set<const Atom*> > result;
	set<const Atom*>::iterator sit;

	//////////////////////////////////////////////////////////////
	// fix carboxlic acid:
	sm.match(result, mol, "[#8;D1]~#6~[#8;D1]", aromatic_atoms_);

	for (Position pos = 0; pos < result.size(); pos++)
	{
		set<const Atom*>& set = result[pos];
		vector<Atom*> oxygen;
		Atom* carbon = 0;

		for (sit = set.begin(); sit != set.end(); ++sit)
		{
			if ((*sit)->getElement().getSymbol() == "C")
			{
				carbon = (Atom*)*sit;
			}
			else
			{
				oxygen.push_back((Atom*)*sit);
			}
		}

		if (!hasAromaticBonds_(*carbon)) continue;

		oxygen[0]->getBond(*carbon)->setOrder(Bond::ORDER__SINGLE);
		oxygen[1]->getBond(*carbon)->setOrder(Bond::ORDER__DOUBLE);
		aromatic_atoms_.erase(oxygen[0]);
		aromatic_atoms_.erase(oxygen[1]);
		aromatic_atoms_.erase(carbon);
		nr_ca++;
	}
	
	//////////////////////////////////////////////////////////////
	// fix amidene and guanidine
	// sm.match(result, mol, "[#7;D1]#6([#7;D1])*");
	Size nr_am_gu = 0;
	result.clear();
	sm.match(result, mol, "[#7;D1]~[#6R0]~[#7;D1]", aromatic_atoms_);
	if (!result.empty())
	{
		for (Position pos = 0; pos < result.size(); pos++)
		{
			set<const Atom*>& set = result[pos];
			vector<Atom*> nitrogen;
			Atom* carbon = 0;

			for (sit = set.begin(); sit != set.end(); ++sit)
			{
				if ((*sit)->getElement().getSymbol() == "C")
				{
					carbon = (Atom*)*sit;
				}
				else
				{
					nitrogen.push_back((Atom*)*sit);
				}
			}

			if (!hasAromaticBonds_(*carbon)) continue;

			nitrogen[0]->getBond(*carbon)->setOrder(Bond::ORDER__SINGLE);
			nitrogen[1]->getBond(*carbon)->setOrder(Bond::ORDER__DOUBLE);
			aromatic_atoms_.erase(carbon);
			aromatic_atoms_.erase(nitrogen[0]);
			aromatic_atoms_.erase(nitrogen[1]);
			nr_am_gu++;
		}
	}
	
	//////////////////////////////////////////////////////////////
	// fix phosphonic acid
	result.clear();
	Size nr_phos = 0;
//   	sm.match(result, mol, "[p]([oD1])([oD1])([oD1])[#6,#8]");
	sm.match(result, mol, "[P]([#8;D1])([#8;D1])([#8;D1])", aromatic_atoms_);

	for (Position pos = 0; pos < result.size(); pos++)
	{
		set<const Atom*>& set = result[pos];
		vector<Atom*> oxygen;
		Atom* phosphor = 0;

		for (sit = set.begin(); sit != set.end(); ++sit)
		{
			if ((*sit)->getElement().getSymbol() == "P")
			{
				phosphor = (Atom*)*sit;
			}
			else
			{
				oxygen.push_back((Atom*)*sit);
			}
		}

		if (!hasAromaticBonds_(*phosphor)) continue;
		
		oxygen[0]->getBond(*phosphor)->setOrder(Bond::ORDER__DOUBLE);
		oxygen[1]->getBond(*phosphor)->setOrder(Bond::ORDER__SINGLE);
		oxygen[2]->getBond(*phosphor)->setOrder(Bond::ORDER__SINGLE);

		aromatic_atoms_.erase(phosphor);
		aromatic_atoms_.erase(oxygen[0]);
		aromatic_atoms_.erase(oxygen[1]);
		aromatic_atoms_.erase(oxygen[2]);
		nr_phos++;
	}
	
	//////////////////////////////////////////////////////////////
	// fix O~N
	Size on = 0;
	sm.match(result, mol, "[#8;D1;R0;$(#8~[#6R0])]", aromatic_atoms_);

	for (Position pos = 0; pos < result.size(); pos++)
	{
		Atom* oxygen = (Atom*)*result[pos].begin();

		if (!hasAromaticBonds_(*oxygen)) continue;

		oxygen->getBond(0)->setOrder(Bond::ORDER__DOUBLE);
		on++;
	}
	

	//////////////////////////////////////////////////////////////
	// fix aromatic rings
	bool ok = fixAromaticRings_();

	// recollect the remaining aromatic bonds:	
	unassigned_bonds_.clear();
	AtomBondIterator bit;
	AtomIterator ait;
	BALL_FOREACH_BOND(mol, ait, bit)
	{
		if (bit->getOrder() == Bond::ORDER__AROMATIC)
		{
			unassigned_bonds_.push_back(&*bit);
		}
	}

	if (ok)
	{
		// set formal charges
		HashMap<Atom*,Index>::Iterator hit = max_valence_.begin();
		for (; hit != max_valence_.end(); ++hit)
		{
			Atom* atom = hit->first;
			Size nr = 0;
			AtomBondIterator bit = atom->beginBond();
			for(;+bit;++bit)
			{
				if (bit->getOrder() >= 1 && bit->getOrder() <= 5)
				{
					nr += (Size) bit->getOrder();
				}
			}

			atom->setFormalCharge(nr - hit->second);
		}
	}

#ifdef BALL_MMFF94_TEST
	Log.error() << "Kekulized bonds: " 
	            << "CA   "  << nr_ca    << " " 
							<< "NH2  "  << nr_am_gu << " " 
							<< "ON   "  << on       << " " 
							<< "PHOS "  << nr_phos  << std::endl;
	Log.error() << "Not kekulized: " << unassigned_bonds_.size() << std::endl;
#endif
	return ok;
}

bool Kekuliser::hasAromaticBonds_(Atom& atom)
{
	AtomBondIterator abit = atom.beginBond();
	for (; +abit; ++abit)
	{
		if (abit->getOrder() == Bond::ORDER__AROMATIC) 
		{
			return true;
		}
	}

	return false;
}


void Kekuliser::dump()
{
	for (Position p = 0; p < atom_infos_.size(); p++)
	{
		AtomInfo& ai = atom_infos_[p];
		Log.error() << ai.atom->getFullName() << " a. Bonds: " << ai.abonds.size()
			 << " Cur " << ai.curr_double 
			 << " min " << ai.min_double
			 << " max " << ai.max_double
			 << " unc " << ai.uncharged_double
		   << " ";
		for (Position b = 0; b < ai.abonds.size(); b++)
		{
			Atom* partner = ai.abonds[b]->getPartner(*ai.atom);
			Log.error() << partner->getName() << " ";
		}
		Log.error() << "  " << current_penalty_;
		if (lowest_penalty_ != std::numeric_limits<int>::max()) Log.error() << " + " << lowest_penalty_;
		Log.error() << std::endl;
	}
}

bool Kekuliser::fixAromaticRings_()
{
	bool ok = true;

	calculateAromaticSystems_();
	if (aromatic_systems_.empty()) return true;

	getMaximumValence_();

	AtomInfo temp_ai;
	temp_ai.uncharged_double = 0;
	temp_ai.curr_double = 0;
	temp_ai.double_bond = 0;

	// iterate over all aromatic systems:
	vector<set<Atom*> >::iterator rit = aromatic_systems_.begin();
	for (; rit != aromatic_systems_.end(); rit++)
	{
		set<Atom*>& atom_set = *rit;

		// abort for strange rings:
		if (atom_set.size() < 3)
		{
#ifdef DEBUG_KEKULIZER
 			Log.error() << "Kekulizer: Could not assign ring with " << (*rit).size()
									<< " atoms. " << std::endl;

			if (atom_set.size())
			{
				Log.error() << (**atom_set.begin()).getFullName() << std::endl;
			}
#endif
			ok = false;
			continue;
		}

		atom_infos_.clear();

		bool abort_this_ring = false;

		// for one aromatic system: collect all needed informations for the individual atoms:
		set<Atom*>::iterator hit = atom_set.begin();
		for (; hit != atom_set.end(); ++hit)
		{
			Atom& atom = *(Atom*)*hit;

			// calculate the current valence:
			Index curr_valence = 0;
			AtomBondIterator bit = atom.beginBond();
			for (; +bit; ++bit)
			{
				if (bit->getType() == Bond::TYPE__HYDROGEN) continue;

				if (bit->getOrder() < 2 ||
						bit->getOrder() > 4)
				{
					curr_valence++;
				}
				else
				{
					curr_valence += (Index) bit->getOrder();
				}
			}

			// calculate the number of needed double bonds:
			Index uncharged_double = max_valence_[&atom] - curr_valence;
			if (uncharged_double < 0)
			{
				Log.error() << "Kekulizer: Could not calculate max number of needed double bonds for " 
										<< atom.getFullName() << std::endl;
				Log.error() << "Max: "  << max_valence_[&atom] << "  Curr:  " << curr_valence << std::endl;
				abort_this_ring = true;
				ok = false;
				break;
			}

			atom_infos_.push_back(temp_ai);
			AtomInfo& info = *atom_infos_.rbegin();
			info.atom = &atom;

			Index max_double = uncharged_double + 1;
			max_double = BALL_MIN(1, max_double);

			Index min_double = uncharged_double - 1;
			min_double = BALL_MAX(0, min_double);

			// allow charged atoms only for selected elements
			Position atomic_number = atom.getElement().getAtomicNumber();
			if (atomic_number != 6 &&
					atomic_number != 7)
			{
				max_double = uncharged_double;
				min_double = uncharged_double;
			}

			info.max_double = max_double;
			info.min_double = min_double;
			info.uncharged_double = uncharged_double;

			// collect aromatic bonds for this atom:
			bit = atom.beginBond();
			for (; +bit; ++bit)
			{
				// add aromatic bonds only once:
				if (bit->getOrder() == Bond::ORDER__AROMATIC &&
						*bit->getPartner(atom) > atom) 
				{
					// set bond order initialy to single
					bit->setOrder(Bond::ORDER__SINGLE);
					info.abonds.push_back(&*bit);
				}
			}

		} // all aromatic atoms of this ring

		if (abort_this_ring) continue;

		std::sort(atom_infos_.begin(), atom_infos_.end());

		// map the AtomInfos to the atoms
		HashMap<Atom*, Position> atom_to_id;
		for (Position p = 0; p < atom_infos_.size(); p++)
		{
			atom_to_id[atom_infos_[p].atom] = p;
		}

		for (Position p = 0; p < atom_infos_.size(); p++)
		{
			AtomInfo& ai = atom_infos_[p];
			Atom* atom = ai.atom;
			for (Position b = 0; b < ai.abonds.size(); b++)
			{
				Atom* partner = ai.abonds[b]->getPartner(*atom);
				Position partnerp = atom_to_id[partner];
				ai.partner_id.push_back(partnerp);
			}
		}

#ifdef DEBUG_KEKULIZER
		Log.error() << "State before Kekulizer:" << std::endl;
   	dump();
#endif

		solutions_.clear();
		lowest_penalty_ = std::numeric_limits<int>::max();
		current_penalty_ = 0;
		fixAromaticSystem_(0);
		// test could be changed to achieve at most a given max value:
		if (lowest_penalty_ < std::numeric_limits<int>::max())
		{
			if (lowest_penalty_ == 0)
			{
				applySolution_(0);
			}
			else
			{
				applySolution_(calculateDistanceScores_());
			}

			continue;
		}
	
		// we were not successfull, so reset the bonds to aromatic:
		ok = false;

	} // all aromatic systems

	return ok;
}

void Kekuliser::fixAromaticSystem_(Position it)
{
#ifdef DEBUG_KEKULIZER
	if (it < atom_infos_.size())
	{
		Log.error() << "fixAromaticSystem_ " << it << " " << atom_infos_[it].atom->getFullName() << std::endl;
		dump();
	}
#endif
	
	if (current_penalty_ > lowest_penalty_) return;

	// no more atoms in this aromatic system?
	if (it >= atom_infos_.size())
	{
		// is this solution maybe better than any we have tested so far?
		//
		// if we had already a solution with a penalty of 0, it can't get any better!
		if (lowest_penalty_ == 0) return;

		if (current_penalty_ <= lowest_penalty_)
		{
			// better than any we have seen so far? 
			// than throw away the old solutions!
			if (current_penalty_ < lowest_penalty_) 
			{
				solutions_.clear();
				lowest_penalty_ = current_penalty_;
			}

			// store the new solution
			solutions_.push_back(atom_infos_);
		}

		return;
	}

	AtomInfo& ai = atom_infos_[it];
	
	// no aromatic bonds left?
	if (ai.abonds.empty())
	{
		// penality for this atom if we have not enough double bonds for it
		// to become uncharged:
		Size tap = 0;
		if (ai.curr_double < ai.uncharged_double)
		{
			tap = getPenalty_(*ai.atom, -1);
		}

		current_penalty_ += tap;
		fixAromaticSystem_(it + 1);
		current_penalty_ -= tap;

		return;
		// no aromatic bonds left to setup, so in case of no success:
		// no need to reset the bonds to single order
	}

	// first try without any double bonds if this atom is than uncharged:
	if (ai.min_double <= ai.curr_double &&
			ai.uncharged_double == ai.curr_double)
	{
		fixAromaticSystem_(it + 1);
	}

	// this atom penalty:
	Size tap = 0;

	// can we try to add a double bond?
	if (ai.curr_double < ai.max_double) 
	{
		if (ai.uncharged_double != 1)
		{
			tap = getPenalty_(*ai.atom, 1);
		}

		for (Position b = 0; b < ai.abonds.size(); b++)
		{
			// get the bond and partner atom:
			Bond* bond = ai.abonds[b];
			Position p = ai.partner_id[b];
			AtomInfo& pi = atom_infos_[p];

			// if partner cant take any more double bonds, take next bond:
			if (pi.curr_double == pi.max_double) 
			{
				continue;
			}

			// partner atom penalty:
			Size pap = 0;
			if (pi.curr_double + 1 > pi.uncharged_double)
			{
				pap += getPenalty_(*pi.atom, 1);
			}
			pap += tap;

			// try an early break
			if (current_penalty_ + pap > lowest_penalty_) 
			{
				continue;
			}

			ai.curr_double++;
			pi.curr_double++;
			ai.double_bond = bond;
			pi.double_bond = bond;
			current_penalty_ += pap;

			// try this solution:
			fixAromaticSystem_(it + 1);

			// remove the bond and reset all values:
			current_penalty_ -= pap;
			ai.double_bond = 0;
			pi.double_bond = 0;
			ai.curr_double--;
			pi.curr_double--;
		}
	}

	// try without any extra double bonds,
	// if this atom will than be charged, 
	// otherwise we have tested it above
	if (ai.uncharged_double != ai.curr_double)
	{
		tap = 0;
		if (ai.curr_double == 0) tap = getPenalty_(*ai.atom, -1);
		
		current_penalty_ += tap;
		fixAromaticSystem_(it + 1);
		current_penalty_ -= tap;
	}
}


void Kekuliser::collectAromaticAtoms_()
{
	aromatic_systems_.clear();
	aromatic_atoms_.clear();

	AtomIterator 		 ait;
	BALL_FOREACH_ATOM(*molecule_, ait)
	{
		AtomBondIterator abit = ait->beginBond();
		for (; +abit; ++abit)
		{
			if (abit->getOrder() == Bond::ORDER__AROMATIC)
			{
				aromatic_atoms_.insert(abit->getFirstAtom());
				aromatic_atoms_.insert(abit->getSecondAtom());
				break;
			}
		}
	}

	all_aromatic_atoms_ = aromatic_atoms_;
}

void Kekuliser::calculateAromaticSystems_()
{
	collectAromaticAtoms_();

	// iterate over all aromatic ring atoms:
	while (!aromatic_atoms_.empty())
	{
		// first ring aromatic atom that is still left:
		Atom* atom = (Atom*)*aromatic_atoms_.begin();

		current_aromatic_system_.clear();

		collectSystems_(*atom);

		aromatic_systems_.push_back(current_aromatic_system_);
	}
}

void Kekuliser::collectSystems_(Atom& atom)
{
	current_aromatic_system_.insert(&atom);
	aromatic_atoms_.erase(&atom);

	// all bonds of this atom:
	AtomBondIterator abit = atom.beginBond();
	for (; +abit; ++abit)
	{
		// if aromatic bond:
		if (abit->getOrder() != Bond::ORDER__AROMATIC) continue;

		Atom* partner = abit->getPartner(atom);
		
		// if not seen partner before:
		if (aromatic_atoms_.find(partner) == aromatic_atoms_.end()) continue;

		collectSystems_(*partner);
	}
}

void Kekuliser::getMaximumValence_()
{
	max_valence_.clear();

	vector<set<Atom*> >::iterator rit = aromatic_systems_.begin();
	for (; rit != aromatic_systems_.end(); rit++)
	{
		set<Atom*>::iterator hit = (*rit).begin();
		for (; hit != rit->end(); ++hit)
		{
			Atom& atom = **hit;
			Index max_valence = 0;
			if (max_valence_.has(&atom)) continue;

			Position atomic_number = atom.getElement().getAtomicNumber();

			switch (atomic_number)
			{
				case 6:
					max_valence = 4;
					break;

				case 8:
				case 16:
				case 34:
				case 52:
					max_valence = 2;
					break;

				 case 7:
				 case 15:
				 case 33:
					 max_valence = 3;
					 break;
			}

			// Nitrogen and sulfur:
			if (atomic_number == 7 || 
					atomic_number == 16)
			{
				AtomBondIterator abit = atom.beginBond();
				for (; +abit; ++abit)
				{
					if (abit->getOrder() != Bond::ORDER__DOUBLE) continue;

					if (abit->getPartner(atom)->getElement().getAtomicNumber() == 8)
					{
						max_valence += 2;
					}
				}
			}

			max_valence_[&atom] = max_valence;
		}
	}
}


void Kekuliser::clear()
{
	aromatic_systems_.clear();
	aromatic_rings_.clear();
	unassigned_bonds_.clear();
	aromatic_atoms_.clear();
	max_valence_.clear();
	current_aromatic_system_.clear();
	atom_infos_.clear();
}

Size Kekuliser::getPenalty_(Atom& atom, Index charge)
{
#define POSITIVE_NITROGEN 10
#define NEGATIVE_NITROGEN 11
#define NEGATIVE_CARBON   25
#define POSITIVE_CARBON   26
// if formal charge information available and we get an other charge:
#define UNEQUAL_CHARGE    100

	if (use_formal_charges_ && atom.getFormalCharge() != 0)
	{
		if (atom.getFormalCharge() != charge) return UNEQUAL_CHARGE;
	
		return 0;
	}

	Position p = atom.getElement().getAtomicNumber();
	if (p == 6)
	{
		if 			(charge == 1)  return POSITIVE_CARBON;
		else if (charge == -1) return NEGATIVE_CARBON;
	}

	if (p == 7)
	{
		if (charge == -1) return NEGATIVE_NITROGEN;
		else if (charge == 1)
		{
			AtomBondIterator abit = atom.beginBond();
			for (; +abit; ++abit)
			{
				if (abit->getPartner(atom)->countBonds() == 1) return POSITIVE_NITROGEN - 1;
			}

			return POSITIVE_NITROGEN;
		}
	}

	return 0;
}

void Kekuliser::applySolution_(Position pos)
{
	vector<AtomInfo> vit = solutions_[pos];
	for (Position i = 0; i < vit.size(); i++)
	{
		AtomInfo& ai = vit[i];
		if (ai.double_bond != 0) ai.double_bond->setOrder(Bond::ORDER__DOUBLE);
	}
}

// calculate a score for the distribution of the different 
// charged atoms in this aromatic system:
// return the best solution
Position Kekuliser::calculateDistanceScores_()
{
	float best_score = std::numeric_limits<int>::max();
	Size best_solution = 0;

	for (Position solp = 0; solp < solutions_.size(); solp++)
	{
		vector<AtomInfo> vit = solutions_[solp];

		vector<Atom*> pos_atoms;
		vector<Atom*> neg_atoms;

		for (Position a = 0; a < vit.size(); a++)
		{
			AtomInfo& ai = vit[a];
			if (ai.curr_double < ai.uncharged_double)
			{
				neg_atoms.push_back(ai.atom);
				continue;
			}

			if (ai.curr_double > ai.uncharged_double)
			{
				pos_atoms.push_back(ai.atom);
			}
		}

		float this_score = 0;

		// atoms with same charge get a high penalty score if they are near each other
		for (Position posp = 0; posp < pos_atoms.size(); posp++)
		{
			for (Position p = posp; p < pos_atoms.size(); p++)
			{
				Atom* a1 = pos_atoms[posp];
				Atom* a2 = pos_atoms[p];

				this_score += 100. / (a1->getPosition().getSquareDistance(a2->getPosition()) + 1.);
			}
		}

		for (Position negp = 0; negp < neg_atoms.size(); negp++)
		{
			for (Position p = negp; p < neg_atoms.size(); p++)
			{
				Atom* a1 = neg_atoms[negp];
				Atom* a2 = neg_atoms[p];

				this_score += 100. / (a1->getPosition().getSquareDistance(a2->getPosition()) + 1.);
			}
		}

		// atoms with different charge get a high penalty score if they are far away
		for (Position p = 0; p < pos_atoms.size(); p++)
		{
			Atom* a1 = pos_atoms[p];

			for (Position n = 0; n < neg_atoms.size(); n++)
			{
				Atom* a2 = neg_atoms[n];
				this_score += (a1->getPosition().getSquareDistance(a2->getPosition())) / 100.;
			}
		}

		if (this_score < best_score)
		{
			best_solution = solp;
			best_score = this_score;
		}
	}

	return best_solution;
}

} // namespace BALL
