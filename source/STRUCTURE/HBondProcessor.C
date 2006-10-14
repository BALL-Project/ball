// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/STRUCTURE/HBondProcessor.h>
#include <BALL/DATATYPE/hashGrid.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/system.h>
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/KERNEL/PTE.h>
#include <map>

namespace BALL
{
	const String HBondProcessor::Option::PREDICTION_METHOD = "prediction_method";
	const String HBondProcessor::PredictionMethod::KABSCH_SANDER = "Kabsch_Sander";
	const String HBondProcessor::PredictionMethod::WISHART_ET_AL = "Wishart_et_al";
	const String HBondProcessor::Default::PREDICTION_METHOD = HBondProcessor::PredictionMethod::KABSCH_SANDER;
	
	HBondProcessor::HBondProcessor()
		:	MAX_LENGTH(5.2 + 4.2),
			BOND_LENGTH_N_H(1.020),
			BOND_LENGTH_C_O(1.240),
			AMIDE_PROTON_OXYGEN_SEPARATION_DISTANCE(3.5),
			ALPHA_PROTON_OXYGEN_SEPARATION_DISTANCE(2.77208),
			residue_data_(),
			h_bond_pairs_()
	{
		setDefaultOptions();
	}

	HBondProcessor::HBondProcessor(Options& new_options)
		:	MAX_LENGTH(5.2 + 4.2),
			BOND_LENGTH_N_H(1.020),
			BOND_LENGTH_C_O(1.240),	
			AMIDE_PROTON_OXYGEN_SEPARATION_DISTANCE(3.5),
			ALPHA_PROTON_OXYGEN_SEPARATION_DISTANCE(2.77208),
			options(new_options),
			residue_data_(),
			h_bond_pairs_()
	{
		// make sure to add all defaults that were missing in the options we got
		setDefaultOptions();
	}
		

	HBondProcessor::~HBondProcessor()
	{
	}
		
	void 	HBondProcessor::init()
    throw()
  {
	}
	
	bool 	HBondProcessor::start()
    throw()
  {
    // clear the donor list and the acceptor list
    donors_.clear();
    acceptors_.clear();
		h_bond_pairs_.clear();
    residue_data_.clear();

		residue_id_to_position_.clear();
		
  	return true;
	}
	
	
	Processor::Result HBondProcessor::operator() (Composite &composite)
	{
		// create a bounding box which should include the composite
		BoundingBoxProcessor bp;

		// What happens to ligands, water ...
		ResidueIterator ri;

		// do we have a system?
		if (RTTI::isKindOf<System>(composite))
		{
			System *s = RTTI::castTo<System>(composite);
			s->apply(bp);
      ri = s->beginResidue();
		}
		else if (RTTI::isKindOf<Protein>(composite))
		{
			Protein *s = RTTI::castTo<Protein>(composite);
			s->apply(bp);
      ri = s->beginResidue();
		}
		else if (RTTI::isKindOf<Chain>(composite))
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

		
		if (options[HBondProcessor::Option::PREDICTION_METHOD] == PredictionMethod::KABSCH_SANDER)
		{
			
			upper_ = bp.getUpper();
			lower_ = bp.getLower();

			// compute the H-Bonds
			preComputeBonds_(ri);

			//NOTE: This has to be a __BREAK__, since we want to allow
			//to apply the HBondProcessor at system/protein and chain level! 
			//Processors "selfcall" hierarchically, so in case of a system 
			//the composite system is "subcalled" first and all residues 
			//of the system were taken into account and than we have to break.
			//Otherwise next selfcall would be the protein/chain...
			return Processor::BREAK;

		}
		else if (options[HBondProcessor::Option::PREDICTION_METHOD] == PredictionMethod::WISHART_ET_AL)
		{
			// we collect the possible acceptors and donors
			// Wishart et al allow hydrogen bonds between all kinds of oxigens and H/HA 
			// restricted by certain rules

			// we have to enumerate the residues new, to make sure that they get an ascending number
			Position j = 0;  

			for (; +ri ; ++ri)
			{	
				residue_id_to_position_[ri->getID()] = j;
				j++;
				for (AtomIterator ai = ri->beginAtom(); +ai; ++ai)
				{
					Atom* atom = RTTI::castTo<Atom>((*ai));

					// we store all oxigens as hydrogen bond acceptors
					if (atom->getElement() == PTE[Element::O])
					{			
						acceptors_.push_back(atom);
					}			
					// and the hydrogen bond donors
					if (   (atom->getName().hasSubstring( "HA")) 
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
		for (; +resit ; ++resit)
		{	
// ??????????????? had to remove these lines to prevent segfaults (A.Moll)
//   			if (!resit->isAminoAcid())
//   			{
//   				continue;
//   			}

			bool haveO = false;
			bool haveN = false;
			bool haveC = false;

			ResidueData pos;
	
			Size found = 0;
			for (AtomIterator ai = resit->beginAtom(); +ai; ++ai)
			{
				if (ai->getName() ==  "C")
				{
					pos.pos_C = ai->getPosition();
					haveC = true;
				}
				else if (ai->getName() ==  "O")
				{
					pos.pos_O = ai->getPosition();
					haveO = true;
				}
				else if (ai->getName() ==  "N")
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
	 * and stores them in h_bond_pairs_.
   ***************************************************/
	bool HBondProcessor::finishKabschSander_()
	{
		if (residue_data_.size() == 0) return true;

		// matrix to save the existence of a HBond
		h_bond_pairs_.resize(residue_data_.size()); 

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

		const Index size_x = atom_grid.getSizeX();
		const Index size_y = atom_grid.getSizeY();
		const Index size_z = atom_grid.getSizeZ();

 		// now compute the energies and see whether we have a hydrogen bond
		for (Size i = 0; i < residue_data_.size(); i++)
		{
			const ResidueData& current_res = residue_data_[i];
			if (!current_res.is_complete) continue;

			HashGridBox3<ResidueData*>* const box = atom_grid.getBox(current_res.pos_N);

			// We iterate over the grid ourselves, since the beginBox()... didn't work... :-(
			Position x, y, z;                            // indices of the actual box
			atom_grid.getIndices(*box, x, y, z);         // compute the indices of the actual box

			// Iterate over all the neighbouring boxes
			// for (box_it = box->beginBox(); +box_it; ++box_it) //as mentioned beginBox doesn't work
			for (int nx = x-1; (nx < size_x) && (nx < (int)x+2); nx++)
			{
				if (nx < 0) continue;

				for (int ny = y-1; (ny < size_y) && (ny < (int)y+2); ny++)
				{
					if (ny < 0) continue;

					for (int nz = z-1; (nz < size_z) && (nz < (int)z+2); nz++)
					{
						if (nz < 0) continue;

						// compute the neighbour box
						HashGridBox3<ResidueData*>* const nb = atom_grid.getBox(nx, ny, nz); 

						//iterate over all residues of the neighbouring box
						HashGridBox3<ResidueData*>::DataIterator data_it;
						for (data_it = nb->beginData(); +data_it; ++data_it)
						{
							// TODO: We don't want H-bonds between neighboring residues!
							//       Does this criterion always work? We should check for
							//       an existing bond between data_it and residue_data_[i] instead!

							// data from neighbouring residue
							const ResidueData& ndata = **data_it;

							if (abs((Index)ndata.number - (Index)(current_res.number)) <= 1 || 
									ndata.number == 0)
							{
								continue;
							}

							// compute the distances between the relevant atoms
							const float dist_ON = (current_res.pos_O - ndata.pos_N).getLength();
							const float dist_CH = (current_res.pos_C - ndata.pos_H).getLength();
							const float dist_OH = (current_res.pos_O - ndata.pos_H).getLength();
							const float dist_CN = (current_res.pos_C - ndata.pos_N).getLength();

							// compute the electrostatic energy of the bond-building groups
							float energy  = 0.42 * 0.20 * 332.;
							energy *=  (1./dist_ON + 1./dist_CH - 1./dist_OH - 1./dist_CN);

							if (energy >= -0.5) continue;

							Atom *acceptor = 0;
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

							h_bond_pairs_[current_res.number].push_back(ndata.number);

							Bond* bond = donor->createBond(*acceptor);
							bond->setType(Bond::TYPE__HYDROGEN);
							bond->setOrder(1);
							bond->setName("calculated H-Bond");

							donor->setProperty("HBOND_DONOR", *acceptor);
						}
					}
				}
			}
		}

		return true;
	}

	
	bool HBondProcessor::finishWishartEtAl_()
	{	
		h_bond_pairs_.resize(residue_id_to_position_.size());
		/* distance, donor, acceptor for the ShiftXwise hydrogenbond determination*/
		std::multimap<float, std::pair<Atom*, Atom*> >  hbonds;
		std::map<Atom*, bool> donor_occupied;
		std::map<Atom*, bool> acceptor_occupied;

		// if there were no donors or acceptors, return immediately
		if ((donors_.size() == 0) || acceptors_.size() == 0)
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
		for(Position d = 0; d < donors_.size(); ++d)
		{
			for(Position a = 0; a < acceptors_.size(); ++a)
			{	
				// does the bond fullfill all SHiftX criteria?

				// exclude self interaction
				if (donors_[d]->getResidue() == acceptors_[a]->getResidue())
				{
					continue;	
				}

				// HA does not form hydrogen bonds with its _neighbours_
				if (donors_[d]->getName().hasSubstring("HA"))
				{
					bool adjacent_residues = 		 donors_[d]->getResidue()->isNextSiblingOf(*(acceptors_[a]->getResidue()))
						|| donors_[d]->getResidue()->isPreviousSiblingOf(*(acceptors_[a]->getResidue()))
						||(abs(  donors_[d]->getResidue()->getID().toInt() 
									- acceptors_[a]->getResidue()->getID().toInt()) <= 1);

					if (adjacent_residues)
						continue;
				}

				//  oxygen--hydrogen separation  
				float distance =  (donors_[d]->getPosition() - acceptors_[a]->getPosition()).getLength();
				if (   (  donors_[d]->getName().hasSubstring("HA") && (distance > ALPHA_PROTON_OXYGEN_SEPARATION_DISTANCE ))
						|| ( (donors_[d]->getName() == "H")            && (distance > AMIDE_PROTON_OXYGEN_SEPARATION_DISTANCE )))
					continue;

				// TODO: acceptor is a solvent oxygen, the donor must not be a HA

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
					if ( 		(bond_angle >= (Constants::PI/2.)  // TODO: ist das nicht falsch rum!!!!!! 
								||	(distance >= 2.5 + cos(bond_angle))))
						continue;

					//hydrogen-oxygen distance < 3.5 A and hydrogen-oxygen distance < nitrogen - oxygen distance		
					if ((distance > 3.5) || (distance >  (N->getPosition()- acceptors_[a]->getPosition()).getLength()))
						continue;
				}
				// put the HBond into the hydrogen bond distance list
				std::pair<Atom*, Atom*> bond(donors_[d], acceptors_[a]);
				hbonds.insert(std::pair<float, std::pair<Atom*, Atom*> >(distance, bond));
			}
		}
		
		
		// now  compute all hydrogen bond effect shifts 
		// to ensure that we assign only one (the smalles) bond for each donor and acceptor atom,
		// we iterate over the bonds sorted by their distance. if a bond is assigned, we mark this
		// in the occupied data structures
		std::multimap<float, std::pair<Atom*, Atom*> >::iterator it_b = hbonds.begin();
	
		for (; it_b != hbonds.end(); it_b++)
		{
			//double distance = it_b->first;
			Atom* donor    = it_b->second.first;
			Atom* acceptor = it_b->second.second;

			// is this bond still allowed? i.e. are acceptor and donor still unoccupied?
			if (   (donor_occupied.find(donor) != donor_occupied.end())
					|| (acceptor_occupied.find(acceptor) != acceptor_occupied.end()))
				continue;

			//create a hydrogen bond between acceptor:O and donor:H 
			Bond* bond = donor->createBond(*acceptor);
			bond->setType(Bond::TYPE__HYDROGEN);
			bond->setOrder(1);
			bond->setName("calculated H-Bond");
			
			donor->setProperty("HBOND_DONOR", *acceptor);

			//store the HBond for the Secondary Structure Processor
			h_bond_pairs_[residue_id_to_position_[acceptor->getResidue()->getID()]].push_back(residue_id_to_position_[donor->getResidue()->getID()]);
			
			donor_occupied[donor] = true;
			acceptor_occupied[acceptor] = true;
		}
		return true;
	}

	
		
	std::ostream& operator << (std::ostream& s, const BALL::HBondProcessor::ResidueData& p)
	{
		return s << p.pos_C << " " << p.pos_N << " " << p.pos_H << " " << p.pos_O << " " << p.number << std::endl;
	}

	const std::vector< std::vector<Position> >& HBondProcessor::getHBondPairs() const
	{
		return h_bond_pairs_;
	}

	const std::vector<HBondProcessor::ResidueData>& HBondProcessor::getResidueData() const
	{
		return residue_data_;
	}

	void HBondProcessor::setDefaultOptions()
	{
		options.setDefault(HBondProcessor::Option::PREDICTION_METHOD,
											 HBondProcessor::Default::PREDICTION_METHOD);
	}

} //Namespace BALL

