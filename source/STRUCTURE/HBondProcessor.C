// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/STRUCTURE/HBondProcessor.h>
#include <BALL/DATATYPE/hashGrid.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/system.h>
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/KERNEL/PTE.h>
#include <map>
#include <set>

namespace BALL
{
	const String HBondProcessor::Option::PREDICTION_METHOD       = "prediction_method";
	const String HBondProcessor::PredictionMethod::KABSCH_SANDER = "Kabsch_Sander";
	const String HBondProcessor::PredictionMethod::WISHART_ET_AL = "Wishart_et_al";
	const String HBondProcessor::Default::PREDICTION_METHOD      = HBondProcessor::PredictionMethod::KABSCH_SANDER;

	const String HBondProcessor::Option::ADD_HBONDS = "add_hydrogen_bonds";
  const bool HBondProcessor::Default::ADD_HBONDS  = true;

	const String HBondProcessor::Option::KABSCH_SANDER_ENERGY_CUTOFF = "kabsch_sander_energy_cutoff";
  const float HBondProcessor::Default::KABSCH_SANDER_ENERGY_CUTOFF = -0.5;

	float HBondProcessor::MAX_LENGTH = 5.2f + 4.2f;
	float HBondProcessor::BOND_LENGTH_N_H = 1.020f;
	float HBondProcessor::BOND_LENGTH_C_O = 1.240f;

	float HBondProcessor::AMIDE_PROTON_OXYGEN_SEPARATION_DISTANCE = 3.5;
	float HBondProcessor::ALPHA_PROTON_OXYGEN_SEPARATION_DISTANCE = 2.77208;

	HBondProcessor::HBond::HBond()
		: acceptor_(NULL),
			donor_(NULL),
			donor_is_hydrogen_(false)
	{
	}

	HBondProcessor::HBondProcessor()
		: options(),
		  residue_data_(),
			backbone_h_bond_pairs_(),
			donors_(),
			acceptors_(),
			residue_ptr_to_position_(),
			h_bonds_()
	{
		setDefaultOptions();
	}

	HBondProcessor::HBondProcessor(Options& new_options)
		: options(new_options),
			residue_data_(),
			backbone_h_bond_pairs_(),
			donors_(),
			acceptors_(),
			residue_ptr_to_position_(),
			h_bonds_()
	{
		// make sure to add all defaults that were missing in the options we got
		setDefaultOptions();
	}

	HBondProcessor::~HBondProcessor()
	{
	}

	void HBondProcessor::init() //TODO
  {
	}

	bool HBondProcessor::start()
  {
    // clear the donor list and the acceptor list
		backbone_h_bond_pairs_.clear();
    residue_data_.clear();
    donors_.clear();
    acceptors_.clear();
		residue_ptr_to_position_.clear();
		h_bonds_.clear();
		return true;
	}


	Processor::Result HBondProcessor::operator() (Composite &composite)
	{
		// create a bounding box which should include the composite
		BoundingBoxProcessor bp;

		// What happens to ligands, water ...
		ResidueIterator    ri;

		// do we have a system?
        if (RTTI::isKindOf<System>(&composite))
		{
			System *s = RTTI::castTo<System>(composite);
			s->apply(bp);
			ri = s->beginResidue();
		}
        else if (RTTI::isKindOf<Protein>(&composite))
		{
			Protein *s = RTTI::castTo<Protein>(composite);
			s->apply(bp);
			ri = s->beginResidue();
    }
        else if (RTTI::isKindOf<Chain>(&composite))
		{
			Chain *s = RTTI::castTo<Chain>(composite);
			s->apply(bp);
			ri = s->beginResidue();
		}

		// ri doesn't seem to exist
		if (!(+ri))
		{
			return Processor::CONTINUE;
		}

		// delete all previous hydrogen bonds
		std::set< Bond* >  to_delete;
		Atom::BondIterator bi;
		AtomIterator       ai;
		ResidueIterator    ri_del(ri);
		for ( ; +ri_del; ++ri_del)
		{
			BALL_FOREACH_BOND(*ri_del, ai, bi)
			{
				if (bi->getType() == Bond::TYPE__HYDROGEN)
				{
					to_delete.insert(&*bi);
				}
			}
		}
		for (std::set< Bond* >::iterator sit = to_delete.begin();
				 sit != to_delete.end(); 
				 sit++)
		{
			delete *sit;
		}

		// compute the hydrogen bonds
		if (options[HBondProcessor::Option::PREDICTION_METHOD] == PredictionMethod::KABSCH_SANDER)
		{
			upper_ = bp.getUpper();
			lower_ = bp.getLower();

			// compute the H-Bonds
			preComputeBonds_(ri);

			//NOTE: This has to be a __BREAK__, since we want to allow
			//      to apply the HBondProcessor to system, protein, and chain level! 
			// 	    Processors "selfcall" hierarchically, so in case of a system 
			//      the composite system is "subcalled" first and all residues 
			//      of the system were taken into account and than we have to break.
			//      Otherwise next selfcall would be the protein/chain...
			return Processor::BREAK;
		}
		else if (options[HBondProcessor::Option::PREDICTION_METHOD] == PredictionMethod::WISHART_ET_AL)
		{
			// we collect the possible acceptors and donors
			// Wishart et al allow hydrogen bonds between all kinds of oxigens and H/HA 
			// restricted by certain rules

			// we have to enumerate the residues again, to ensure that they have an ascending number
			Position j = 0;

			for (; +ri ; ++ri)
			{
				residue_ptr_to_position_[&*ri] = j;
				j++;
				for (AtomIterator ai = ri->beginAtom(); +ai; ++ai)
				{
					Atom* atom = RTTI::castTo<Atom>((*ai));

					// we store all oxygens as potential hydrogen bond acceptors
					if (atom->getElement() == PTE[Element::O])
					{
						acceptors_.push_back(atom);
					}
					// and the hydrogen as potential hydrogen bond donors
					if (   (atom->getName().hasSubstring("HA"))
							|| (atom->getName() == "H")  )
					{
						donors_.push_back(atom);
					}
				}
			}

			return Processor::BREAK;
		}
		else
		{
			Log.error() << "HBondProcessor::Operator(): Unknown prediction method requested! Aborting!" << std::endl;
			return Processor::ABORT;
		}

		return Processor::CONTINUE;
	}

	/*********************************************************
  *  preComputeBonds identifies characteristic amino acid
	*  atoms (C, N, O), and stores them in residue_data_ 
  *********************************************************/
	void HBondProcessor::preComputeBonds_(ResidueIterator& resit)
	{
		// index to enumerate the residues. Unfortunately, we cannot use
		// the residueID, which is not monotonous
		Position j = 0;

		// iteration over all residues of the protein
		// to find the C,N,O atoms
		//
		// since the N-terminus is special, we treat it differently
		// we also have to take care of residues with missing atoms 
		// we find the first complete residue and use it instead 
		for ( ; +resit ; ++resit)
		{
			if (!resit->isAminoAcid())
			{
				ResidueData pos;
				pos.is_complete = false;

				pos.number = j++;
				pos.res = &(*resit);

				residue_data_.push_back(pos);
				continue;
			}

			bool haveO = false;
			bool haveN = false;
			bool haveC = false;

			ResidueData pos;

			Size found = 0;
			for (AtomIterator ai = resit->beginAtom(); +ai; ++ai)
			{
				if (ai->getName() == "C")
				{
					pos.pos_C = ai->getPosition();
					haveC = true;
				}
				else if (ai->getName() == "O")
				{
					pos.pos_O = ai->getPosition();
					haveO = true;
				}
				else if (ai->getName() == "N")
				{
					pos.pos_N = ai->getPosition();
          haveN = true;
				}
				else
				{
					continue;
				}

				found++;
				if (found == 3) break;
			}

			// we have to overread incomplete residues					
			pos.is_complete = (haveN && haveO && haveC);

			pos.number = j;
			pos.res = &(*resit);

			// the N-terminus is special
			if (pos.is_complete && !(pos.res->isNTerminal()) && (j>0))
			{
				// evaluate the position of H 
				const Vector3 OC(residue_data_[j-1].pos_O - residue_data_[j-1].pos_C);
				const float length = OC.getLength();

				if (!Maths::isZero(length))
				{
					pos.pos_H = pos.pos_N - (OC * BOND_LENGTH_N_H) / length;
				}
				else
				{
					pos.is_complete = false;
				}
			}
			residue_data_.push_back(pos);
			j++;
		}
	}

  bool HBondProcessor::finish()
	{
		bool ret = false;

		if (options[HBondProcessor::Option::PREDICTION_METHOD] == PredictionMethod::KABSCH_SANDER)
		{
			ret = finishKabschSander_();
		}
		else if (options[HBondProcessor::Option::PREDICTION_METHOD] == PredictionMethod::WISHART_ET_AL)
		{
			ret = finishWishartEtAl_();
		}
		else
		{
			Log.error() << "HBondProcessor::finish(): Unknown prediction method requested! Aborting!" << std::endl;
			ret = false;
		}

		return ret;
	}


	/*************************************************** 
	 * Finish computes all hbonds of the composite 
	 * and stores them in backbone_h_bond_pairs_ as position pairs.
   ***************************************************/
	bool HBondProcessor::finishKabschSander_()
	{
		if (residue_data_.empty()) return true;

		// matrix to save the existence of a HBond
		backbone_h_bond_pairs_.resize(residue_data_.size());

		//create a grid inside the bounding box
		HashGrid3<ResidueData*> atom_grid(lower_, upper_ - lower_, MAX_LENGTH);

		// insert all protein-residues at the position of their N-atom
		for (Size i = 0; i < residue_data_.size(); i++)
		{
			if (residue_data_[i].is_complete)
			{
				atom_grid.insert(residue_data_[i].pos_N, &residue_data_[i]);
			}
		}

		float energy_cutoff  = options.getReal(Option::KABSCH_SANDER_ENERGY_CUTOFF);
		bool  add_hbonds     = options.getBool(Option::ADD_HBONDS);

		// now compute the energies and see whether we have a hydrogen bond
		for (Size i=0; i<residue_data_.size(); i++)
		{
			const ResidueData& current_res = residue_data_[i];
			if (!current_res.is_complete) continue;

			HashGridBox3<ResidueData*>* const box = atom_grid.getBox(current_res.pos_N);

			// and iterate over all neighbouring boxes
			for(HashGridBox3<ResidueData*>::BoxIterator bit = box->beginBox(); +bit; ++bit)
			{
				//iterate over all residues of the neighbouring box
				HashGridBox3<ResidueData*>::DataIterator data_it;
				for (data_it = bit->beginData(); +data_it; ++data_it)
				{
					// TODO: We don't want H-bonds between neighboring residues!
					//       Does this criterion always work? We should check for
					//       an existing bond between data_it and residue_data_[i] instead!

					// data from neighbouring residue
					const ResidueData& ndata = **data_it;

					if (   (abs((Index)ndata.number - (Index)(current_res.number)) <= 1)
							|| (ndata.number == 0))
					{
						continue;
					}

					// compute the distances between the relevant atoms
					const float dist_ON = (current_res.pos_O - ndata.pos_N).getLength();
					const float dist_CH = (current_res.pos_C - ndata.pos_H).getLength();
					const float dist_OH = (current_res.pos_O - ndata.pos_H).getLength();
					const float dist_CN = (current_res.pos_C - ndata.pos_N).getLength();

					Vector3 OH = (current_res.pos_O - ndata.pos_H);
					Vector3 NH = (ndata.pos_N - ndata.pos_H);

					float angle = OH.getAngle(NH);

					// compute the electrostatic energy of the bond-building groups
					float energy  = 0.42 * 0.20 * 332.;
					energy *=  (1./dist_ON + 1./dist_CH - 1./dist_OH - 1./dist_CN);

					if (energy >= energy_cutoff) continue;

					Atom* acceptor = 0;
					for (AtomIterator ai = current_res.res->beginAtom(); +ai; ++ai)
					{
						if (ai->getName() == "O")
						{
							acceptor = &(*ai);
							break;
						}
					}

					Atom* donor = 0;
					for (AtomIterator ai = ndata.res->beginAtom(); +ai; ++ai)
					{
						if (ai->getName() == "N")
						{
							donor = &*ai;
							break;
						}
					}

					if (!donor || !acceptor) continue;

					// store the bond:
					//   - add it to the special secondary structure vector 
					backbone_h_bond_pairs_[current_res.number].push_back(ndata.number);

					//   - add the hydrogen bond to the internal data structure
					HBond new_bond(acceptor, donor, false);
					h_bonds_.push_back(new_bond);

					//   - add a real bond	
					if (add_hbonds)
					{
						Bond* bond = donor->createBond(*acceptor);
					  bond->setType(Bond::TYPE__HYDROGEN);
					  bond->setOrder(Bond::ORDER__ANY);
						bond->setName("calculated H-Bond");

						bond->setProperty("HBOND_DONOR",  donor);
						bond->setProperty("HBOND_ACCEPTOR", acceptor);

						bond->setProperty("HBOND_HYDROGEN", (void*)NULL);
						bond->setProperty("HBOND_LENGTH", dist_OH);
						bond->setProperty("HBOND_ANGLE", angle);
					}

				}
			}
		}

		return true;
	}


	bool HBondProcessor::finishWishartEtAl_()
	{
	  // Please note, that in ShiftX notation donor denotes the hydrogen!!

		backbone_h_bond_pairs_.resize(residue_ptr_to_position_.size());

		/* map distance to (donor, acceptor) for the ShiftXwise hydrogen bond determination*/
		std::multimap<float, std::pair<Atom*, Atom*> >  potential_shiftX_hbonds;
		std::map<Atom*, bool> donor_occupied;
		std::map<Atom*, bool> acceptor_occupied;

		// if there were no donors or acceptors, return immediately
		if (donors_.empty() || acceptors_.empty())
		{
			return true;
		}

		/* 	 		The ShiftX definition of hydrogen bonds is: 
		 * 	 				Donors are: H and HA
		 * 	 				Acceptors are: O, OD_n, OE_n, OG_n, OH_n or water in the solvent!
		 * 	 				By now we do not consider water!
		 * 	 				Donors and Acceptors have to be on different residues.
		 * 	 				HA even does not form hydrogen bonds with neighbours.
		 * 	 				If the acceptor is a solvent oxygen, the donor must not be a HA (not yet implemented!)
		 * 	 				The oxygen--hydrogen separation must be less than 3.5 A for H(--N) 
		 * 	 					and 2.77 A for HA.
		 * 	 				For H must hold: 
		 * 	 				  the angle between N_H donor bond vector and the C=O acceptor bond vector must be 
		 * 	 						90 degrees or more, and 
		 * 	 					the distance must be less than 2.5 + cos of the angle, and 
		 * 	 					hydrogen-oxygen distance must be less than the nitrogen - oxygen distance
		 * 			Having applied these rules to each donor--acceptor pair, ShiftX then 
		 * 			sorts the list of possible bonds by the O_H separation distance, shortest to 
		 * 			longest. The list is the processed so that only the single strongest hydrogen 
		 * 			bond is identified for each donor--acceptor pair. Up to that point
		 * 			any bond involving the same donor or acceptor is preculded! 
		 */

		// the potential donors and acceptors are already stored 
		// in donors_ and acceptors_

		// we need a datastructure to collect the hydrogen bonds
		// --> fill potential_shiftX_hbonds
		for (Position d=0; d<donors_.size(); ++d)
		{
			for (Position a=0; a<acceptors_.size(); ++a)
			{
				// does the bond fullfill all ShiftX criteria?
				// exclude self interaction
				if (donors_[d]->getResidue() == acceptors_[a]->getResidue())
				{
					continue;
				}

				// HA does not form hydrogen bonds with its _neighbours_
				if (donors_[d]->getName().hasSubstring("HA"))
				{
					bool adjacent_residues =
						   donors_[d]->getResidue()->isNextSiblingOf(*(acceptors_[a]->getResidue()))
						|| donors_[d]->getResidue()->isPreviousSiblingOf(*(acceptors_[a]->getResidue()))
						|| (   abs(  donors_[d]->getResidue()->getID().toInt()
								 - acceptors_[a]->getResidue()->getID().toInt())  <= 1);

					if (adjacent_residues)
						continue;
				}

				//  oxygen--hydrogen separation  
				float distance = (donors_[d]->getPosition() - acceptors_[a]->getPosition()).getLength();

				if (   (  donors_[d]->getName().hasSubstring("HA") && (distance > ALPHA_PROTON_OXYGEN_SEPARATION_DISTANCE ))
						|| ( (donors_[d]->getName() == "H")            && (distance > AMIDE_PROTON_OXYGEN_SEPARATION_DISTANCE )))
					continue;

				// the angle criterion for H
				if (donors_[d]->getName()== "H")
				{
					Atom* C = NULL;
					Atom* N = NULL;
					// we have to find the C to which the acceptor O is bound and 
					// 								 the N to which the _donor_ H is bound 

					// we can't use countBonds here because of the hydrogen bonds which we want to ignore
					int bond_count_acceptor = 0;
					Atom::BondIterator bi;
					for (bi = acceptors_[a]->beginBond(); +bi; ++bi)
					{
						bond_count_acceptor++;
						if (bi->getPartner(*acceptors_[a])->getName().hasSubstring("C"))
							C = bi->getPartner(*acceptors_[a]);
					}

					int bond_count_donor = 0;
					for (bi = donors_[d]->beginBond(); +bi; ++bi)
					{
						if (bi->getType() != Bond::TYPE__HYDROGEN)
						{
							bond_count_donor++;
							N = bi->getPartner(*donors_[d]);
						}
					}

					// taken from ShitX :-)
					if ( (bond_count_acceptor == 0) || (bond_count_donor != 1) || !C)
					{
						continue;
					}

					// compute the vectors CO and NH
					BALL::Vector3 CO = acceptors_[a]->getPosition() - C->getPosition();
					BALL::Vector3 HN = N->getPosition() - donors_[d]->getPosition();

					float bond_angle = CO.getAngle(HN);
					// NOTE: the following looks different from the SHIFTX paper, but is not :-)
					if (   (bond_angle >= (Constants::PI/2.)
							|| (distance >= 2.5 + cos(bond_angle))))
						continue;

					// hydrogen-oxygen distance < 3.5 A and hydrogen-oxygen distance < nitrogen - oxygen distance		
					if ((distance > 3.5) || (distance >  (N->getPosition()- acceptors_[a]->getPosition()).getLength()))
						continue;
				}

				std::pair<Atom*, Atom*> bond(donors_[d], acceptors_[a]);
				potential_shiftX_hbonds.insert(std::pair<float, std::pair<Atom*, Atom*> >(distance, bond));
			}
		}


		// now filter for all compatible hydrogen bonds.  
		// To ensure that we assign only one (the smallest) bond for each donor and acceptor atom,
		// we iterate over the bonds sorted by their distance. If a bond is assigned, we mark this
		// in the occupied data structures
		std::multimap<float, std::pair<Atom*, Atom*> >::iterator it_b = potential_shiftX_hbonds.begin();

		for ( ; it_b != potential_shiftX_hbonds.end(); ++it_b)
		{
			//double distance = it_b->first;
			Atom* donor    = it_b->second.first;
			Atom* acceptor = it_b->second.second;

			// is this bond still allowed? i.e. are acceptor and donor still unoccupied?
			if (   (donor_occupied.find(donor) != donor_occupied.end())
					|| (acceptor_occupied.find(acceptor)  != acceptor_occupied.end()))
			{
				continue;
			}

			// store the bond:
			//   - add it to the special secondary structure vector 
			//     store the HBond as index pair for the Secondary Structure Processor
			// Note: the index was created by iterating with the ResidueIterator and assigning an ascending number! 
			if (   (acceptor->getName()== "O")
					&& (donor->getName()   == "H"))
			{
				backbone_h_bond_pairs_[residue_ptr_to_position_[acceptor->getResidue()]].push_back(residue_ptr_to_position_[donor->getResidue()]);
			}

			//   - add a real bond	
			bool add_hbonds = options.getBool(Option::ADD_HBONDS);

			//   - add the hydrogen bond to the internal data structure
			HBond new_bond(acceptor, donor, true);
			h_bonds_.push_back(new_bond);

			float bond_length = donor->getPosition().getDistance(acceptor->getPosition());

			Atom const* partner = NULL;
			for (Atom::BondIterator b_it = donor->beginBond(); +b_it; ++b_it)
			{
				if (b_it->getType() != Bond::TYPE__HYDROGEN)
					partner = b_it->getPartner(*donor);
			}

			float bond_angle = Constants::PI;
			if (partner)
				bond_angle = (acceptor->getPosition() - donor->getPosition()).getAngle(partner->getPosition() - donor->getPosition());

			if (add_hbonds && donor)
			{
				Bond* bond = donor->createBond(*acceptor);
			  bond->setType(Bond::TYPE__HYDROGEN);
			  bond->setOrder(Bond::ORDER__ANY);
				bond->setName("calculated H-Bond");

				bond->setProperty("HBOND_DONOR",    donor);
				bond->setProperty("HBOND_ACCEPTOR", acceptor);

				bond->setProperty("HBOND_HYDROGEN", donor);
				bond->setProperty("HBOND_LENGTH", bond_length);
				bond->setProperty("HBOND_ANGLE", bond_angle);
			}

			// finally mark the current participants as occupied
			donor_occupied[donor] = true;
			acceptor_occupied[acceptor] = true;
		}
		return true;
	}



	std::ostream& operator << (std::ostream& s, const BALL::HBondProcessor::ResidueData& p)
	{
		return s << p.pos_C << " " << p.pos_N << " " << p.pos_H << " " << p.pos_O << " " << p.number << std::endl;
	}

	const std::vector< std::vector<Position> >& HBondProcessor::getBackboneHBondPairs() const
	{
		return backbone_h_bond_pairs_;
	}

	const std::vector< std::vector<Position> >& HBondProcessor::getBackboneHBondPattern() const
	{
		return backbone_h_bond_pairs_;
	}

	const std::vector<HBondProcessor::ResidueData>& HBondProcessor::getResidueData() const
	{
		return residue_data_;
	}

	void HBondProcessor::setDefaultOptions()
	{
		options.setDefault(HBondProcessor::Option::PREDICTION_METHOD,
											 HBondProcessor::Default::PREDICTION_METHOD);

		options.setDefaultBool(HBondProcessor::Option::ADD_HBONDS,
											     HBondProcessor::Default::ADD_HBONDS);

		options.setDefaultReal(HBondProcessor::Option::KABSCH_SANDER_ENERGY_CUTOFF,
											     HBondProcessor::Default::KABSCH_SANDER_ENERGY_CUTOFF);
	}

} //Namespace BALL

