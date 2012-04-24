// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/NMR/HBondShiftProcessor.h>
#include <cstdio>
#include <cmath>

using namespace std;

namespace BALL
{
  const char* HBondShiftProcessor::PROPERTY__HBOND_SHIFT = "HBondShift";

  HBondShiftProcessor::HBondShiftProcessor()
    : ShiftModule()
	{
  }

  HBondShiftProcessor::HBondShiftProcessor(const HBondShiftProcessor& processor)
    : ShiftModule(processor),
			acceptor_types_(processor.acceptor_types_),
			donors_(processor.donors_),
			acceptors_(processor.acceptors_),
			amide_protons_are_targets_(processor.amide_protons_are_targets_),
			amide_proton_factor_(processor.amide_proton_factor_),
			amide_proton_subtrahend_(processor.amide_proton_subtrahend_),
			amide_proton_oxygen_hydrogen_separation_distance_(processor.amide_proton_oxygen_hydrogen_separation_distance_),
			alpha_proton_oxygen_hydrogen_separation_distance_(processor.alpha_proton_oxygen_hydrogen_separation_distance_),
			exclude_selfinteraction_(processor.exclude_selfinteraction_),
			ShiftXwise_hydrogen_bonds_computation_(processor.ShiftXwise_hydrogen_bonds_computation_)
  {
  }

  HBondShiftProcessor::~HBondShiftProcessor()
  {
  }

  void HBondShiftProcessor::init()
  {
		// By default, we assume the worst...
    valid_ = false;

    // If no parameters are assigned, abort immediately.
    if (parameters_ == 0)
    {
      return;
    }

    // Check that the parameter file contains the correct section...
    ParameterSection parameter_section;
    parameter_section.extractSection(*parameters_, "HBondEffect");

		// ...and that this section contains the correct column names.
		if (!parameter_section.hasVariable("name"))
		{
			return;
		}

		// Check for the options.
		amide_protons_are_targets_ = false;
		if (parameter_section.options.has("amide_protons_are_targets"))
		{
			amide_protons_are_targets_ = parameter_section.options.getBool("amide_protons_are_targets");
		}

		amide_proton_factor_ = 0.;
		if (parameter_section.options.has("amide_proton_factor"))
		{
			amide_proton_factor_ = parameter_section.options.getReal("amide_proton_factor");
		}

		amide_proton_subtrahend_ = 0.;
		if (parameter_section.options.has("amide_proton_subtrahend"))
		{
			amide_proton_subtrahend_ = parameter_section.options.getReal("amide_proton_subtrahend");
		}

		amide_proton_oxygen_hydrogen_separation_distance_ = 0.;
		if (parameter_section.options.has("amide_proton_oxygen_hydrogen_separation_distance"))
		{
			amide_proton_oxygen_hydrogen_separation_distance_ = parameter_section.options.getReal(
																													"amide_proton_oxygen_hydrogen_separation_distance");
		}

		alpha_proton_oxygen_hydrogen_separation_distance_ = 0.;
		if (parameter_section.options.has("alpha_proton_oxygen_hydrogen_separation_distance"))
		{
			alpha_proton_oxygen_hydrogen_separation_distance_ = parameter_section.options.getReal(
																													"alpha_proton_oxygen_hydrogen_separation_distance");
		}

		exclude_selfinteraction_ = true;
		if (parameter_section.options.has("exclude_selfinteraction"))
		{
			exclude_selfinteraction_ = parameter_section.options.getBool("exclude_selfinteraction");
		}

		ShiftXwise_hydrogen_bonds_computation_ = false;
		if (parameter_section.options.has("ShiftXwise_hydrogen_bonds_computation"))
		{
			ShiftXwise_hydrogen_bonds_computation_ = parameter_section.options.getBool("ShiftXwise_hydrogen_bonds_computation");
		}

		// Read the acceptor types.
		acceptor_types_.clear();

		// Extract the column indices.
		Position acceptor_type_column = parameter_section.getColumnIndex("name");
		for (Position counter = 0; counter < parameter_section.getNumberOfKeys(); counter++)
		{
			String acceptor_type = parameter_section.getValue(counter, acceptor_type_column);
			acceptor_types_.push_back(acceptor_type);
		}

		// Mark the module as initialized.
    valid_ = true;
  }

  bool HBondShiftProcessor::start()
  {
    // If the module is invalid, abort.
    if (!isValid())
    {
      return false;
    }

    // Clear the donor list and the acceptor list.
    donors_.clear();
    acceptors_.clear();

    return true;
  }

  bool HBondShiftProcessor::finish()
  {
    // If the module is invalid, abort.
    if (!isValid())
    {
      return false;
    }

    // If there were no donors or acceptors, return immediately.
    if (donors_.empty() || acceptors_.empty())
    {
      return true;
    }

		/* Should we use hydrogen bonds computed a la ShiftX?
		 * elsewise we assume the the hydrogen bonds have already been set.
		 * 
		 * NOTE: We will just compute __temporarily__  hydrogen bonds 
		 *  found with the ShiftX - algorithm, whose definition of a 
		 *  hydrogen bond differ substantially from a BALL definition.
		 * The ShiftX definition of hydrogen bonds is: 
		 * - Donors are: H and HA
		 * - Acceptors are: O, OD_n, OE_n, OG_n, OH_n or water in the solvent!
		 * 	 By now we do not consider water (as ShiftX does not consider too)!
		 * - Donors and Acceptors have to be on different residues.
		 * - HA even does not form hydrogen bonds with neighbours.
		 * - If the acceptor is a solvent oxygen, the donor must not be a HA (not yet implemented!)
		 * - The oxygen--hydrogen separation must be less than 3.5 A for H(N) 
		 * 	 					and 2.77 A for HA.
		 * - For all hydrogens must hold: 
		 * 	 	 ~the angle between the N_H donor bond vector and the C=O acceptor bond vector must be 
		 * 	 						90 degrees or more, and 
		 * 	 	 ~the hydrogen-oxygen  distance must be less than 2.5 + cos of the angle, and 
		 * 	 	 ~hydrogen-oxygen distance must be less than the nitrogen - oxygen distance
		 * 
		 * Having applied these rules to each donor--acceptor pair, ShiftX then 
		 * sorts the list of possible bonds by the O_H separation distance, shortest to 
		 * longest. The list is then processed so that only the single strongest hydrogen 
		 * bond is identified for each donor--acceptor pair. Up to that point
		 * any bond involving the same donor or acceptor is preculded! 
		 */
		if (ShiftXwise_hydrogen_bonds_computation_)
		{
			// The potential donors and acceptors are already stored 
			// in donors_ and acceptors_.
			// So, we need a datastructure to collect the hydrogen bonds of all donor-acceptor - pairs.
			for(Position d = 0; d < donors_.size(); ++d)
			{
				for(Position a = 0; a < acceptors_.size(); ++a)
				{
					//
					// Does the bond fullfill all SHiftX criteria? 
					//

					// Check the nearest neighbour effect (for HA and H).
					if (    exclude_selfinteraction_
							&& (donors_[d]->getResidue() == acceptors_[a]->getResidue()))
					{
						continue;
					}

					// Does HA  not form hydrogen bonds with its _neighbours_?
					if (donors_[d]->getName().hasSubstring("HA"))
					{
						if (acceptors_[a]->getResidue() && donors_[d]->getResidue())
						{
							bool adjacent_residues =     donors_[d]->getResidue()->isNextSiblingOf(*(acceptors_[a]->getResidue()))
																				|| donors_[d]->getResidue()->isPreviousSiblingOf(*(acceptors_[a]->getResidue()))
																				|| (abs(   donors_[d]->getResidue()->getID().toInt()
																				         - acceptors_[a]->getResidue()->getID().toInt()) <= 1);

							if (adjacent_residues)
								continue;
						}
					}

					//  Check the oxygen--hydrogen separation and do we consider amide protons at all?
					float distance =  (donors_[d]->getPosition() - acceptors_[a]->getPosition()).getLength();
					if (   (donors_[d]->getName().hasSubstring("HA") && (distance > alpha_proton_oxygen_hydrogen_separation_distance_ ))
							|| (    (donors_[d]->getName() == "H")
								   && (distance > amide_proton_oxygen_hydrogen_separation_distance_ ))
							|| (    (donors_[d]->getName() == "H")
									 && !amide_protons_are_targets_))
					{
						continue;
					}

					// Check the angle criterion for HA.
					if (donors_[d]->getName()== "H")
					{
						Atom* C = NULL;
						Atom* N = NULL;

						// We have to find the C to which the acceptor O is bound and 
						// 								 the N to which the _donor_ H is bound. 

						// We can't use countBonds here because of the hydrogen bonds which we want to ignore.
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

						// If we cannot find the bound C or N, continue.
						if ( (bond_count_acceptor == 0) || (bond_count_donor != 1) || !C)
						{
							continue;
						}

						// Otherwise, we compute the vectors CO and NH...
						BALL::Vector3 CO = acceptors_[a]->getPosition() - C->getPosition();
						BALL::Vector3 HN = N->getPosition() - donors_[d]->getPosition();

						// ...compute and check the angle. 
						float bond_angle = CO.getAngle(HN);
						if (   (bond_angle >= (Constants::PI/2.)
							 ||  (distance >= 2.5 + cos(bond_angle))))
							continue;

						// Check the hydrogen-oxygen distance ( < 3.5 A ) 
						// and hydrogen-oxygen distance < nitrogen - oxygen distance.
						if ((distance > 3.5) || (distance >  (N->getPosition()- acceptors_[a]->getPosition()).getLength()))
							continue;
					}
					// Put the HBond into the temporal hydrogen bond distance list.
					std::pair<Atom*, Atom*> bond(donors_[d], acceptors_[a]);
					hbonds_.insert(std::pair<float, std::pair<Atom*, Atom*> >(distance, bond));
				}
			}
		} // end of the SHIFTX-like hydrogenbond computation.
		else  // Otherwise, we assume that the molecule has already got hydrogen bonds.
		{
			// TO THINK: Should we make the same restrictions for a hydrogen bond as ShiftX does? 
			// Note: BALL just detects backbone hydrogen bonds! 

			// We have to find all hydrogen bonds which include all 
			// donor -- acceptor pairs and store them in distances_donors_ / distances_acceptors_.
			for(Position d = 0; d < donors_.size(); ++d)
			{
				for(Position a = 0; a < acceptors_.size(); ++a)
				{
					// NOTE: BALL defines hydrogen bonds in a different way as ShiftX does,
					// e.g. in BALL  N is the donor and O the acceptor
					// whereas in ShiftX the H is the donor, O is the acceptor!
					Atom* donor = getDonor_(donors_[d]);
					if (donor == NULL)
						continue;

					// Do we have to exclude self interactions?
					if (   exclude_selfinteraction_
							&& (donor->getResidue() == acceptors_[a]->getResidue()))
					{
						continue;
					}

					// Check, whether there is a hydrogen bond between.
					BALL::Atom::BondIterator bi = donor->beginBond();
					for (; +bi; ++bi)
					{
						// Compute the distance between donor and acceptor.
						float distance =  (donors_[d]->getPosition() - acceptors_[a]->getPosition()).getLength();

						// If there is a hydrogen bond between donor and acceptor...
						if (    (bi->getType() == Bond::TYPE__HYDROGEN)
								&& (   ( (bi->getFirstAtom() == donor) && (bi->getSecondAtom() == acceptors_[a]) )
									|| ( (bi->getSecondAtom() == donor) && (bi->getFirstAtom() == acceptors_[a])  ) ))
						{
							// ...and the bond fullfills the necessary distance criteria,
							// put the HBond into the hydrogen bond distance list.
							if (    (    amide_protons_are_targets_ && (donor->getName() == "H")
												&& (distance < amide_proton_oxygen_hydrogen_separation_distance_) )
									||  (    donor->getName().hasSubstring("HA")
									      && (distance < alpha_proton_oxygen_hydrogen_separation_distance_ )))
							{
								std::pair<Atom*, Atom*> bond(donors_[d], acceptors_[a]);
								hbonds_.insert(std::pair<float, std::pair<Atom*, Atom*> >(distance, bond));
							}
						}
					} // end of for all bonds of that donor -- acceptor pair
				}
			} // end of for all donors/acceptors
		} // end of else (already set hydrogen bonds)


		// Now  compute all hydrogen bond effect shifts. 
		// To ensure that we assign only one (the smallest) bond for each donor and receptor atom,
		// we iterate over the bonds sorted by their distance. If a bond is assigned, we mark this
		// in the occupied_ data structures and thus exclude them from further hbonds! 
		std::multimap<float, std::pair<Atom*, Atom*> >::iterator it_b = hbonds_.begin();
		for (; it_b != hbonds_.end(); it_b++)
		{
			double distance = it_b->first;
			Atom* donor    = it_b->second.first;
			Atom* acceptor = it_b->second.second;

			// Check, whether this bond is still allowed? 
			// i.e. are acceptor and donor still unoccupied?
			if (   (donor_occupied_.find(donor) != donor_occupied_.end())
					|| (acceptor_occupied_.find(acceptor) != acceptor_occupied_.end()))
				continue;

			donor_occupied_[donor] = true;
			acceptor_occupied_[acceptor] = true;

			// The shift computation.
			// We have to consider two cases: 
			//    donor is an amide proton or an alpha proton. 
			double new_hb_shift = 0.;

			// If the donor is amide proton, 
			// we compute the shift as done in ShiftX.
			// NOTE: the variables' values differ from the values given in the ShiftX paper!
			//
			//					  amide_proton_factor_
			//  shift =  ----------------------  - amide_proton_subtrahend_
			// 								r^3
			// 
			// where r denotes the distance between donor and acceptor of the 
			// hydrogen bond under consideration.
			//
			if (donor->getName() == "H")
			{
				new_hb_shift = (amide_proton_factor_ /(pow(distance, 3.)) ) - amide_proton_subtrahend_;
			}
			// If the donor is an alpha proton,
			// we compute the shift as done in the ShiftX
			// NOTE: the formular differs from the formular given in the ShfitX paper!
			//
			//
			//  				0.147521    (1.65458E-05)		 0.000134668				0.0598561			 15.6855
			//	shift = -------  - -------------  - -------------   +  -----------  + --------  - 0.673905
			// 				   	r							r^1.5					  r*r								r^2.5					r^3
			// 				   	
			// 	where r denotes the distance between donor and acceptor of the 
			// hydrogen bond under consideration.
			// 
			// 
			else if (donor->getName().hasSubstring("HA"))
			{
				// According to ShiftX one has to correct the distances.
				if (distance >  2.60756 ) distance = 2.60756;
				if (distance <  2.26808 ) distance = 2.26808;

				new_hb_shift =   0.147521/distance - (1.65458E-05)/pow(distance, 1.5) - 0.000134668/(distance*distance)
					             + 0.0598561/pow(distance, 2.5) + 15.6855/(distance*distance*distance) - 0.673905;
			}

			// Store the shift in the hydrogen.
			if (new_hb_shift !=0)
			{
				float old_shift = donor->getProperty(ShiftModule::PROPERTY__SHIFT).getFloat();
				float old_hb_shift = donor->getProperty(HBondShiftProcessor::PROPERTY__HBOND_SHIFT).getFloat();

				donor->setProperty(ShiftModule::PROPERTY__SHIFT, (old_shift + new_hb_shift));
				donor->setProperty(HBondShiftProcessor::PROPERTY__HBOND_SHIFT, (old_hb_shift + new_hb_shift));
			}
		}

		return true;
  }

  Processor::Result HBondShiftProcessor::operator () (Composite& object)
	{
		// Here, we collect all possible acceptors and donors.
		if (RTTI::isKindOf<Atom>(object))
		{
			Atom* atom = RTTI::castTo<Atom>(object);

			// Delete the old hydrogen bond shift contribution.
			atom->clearProperty(PROPERTY__HBOND_SHIFT);

			// Test all hydrogen bond acceptor definitions. 
			for (Position i = 0; i < acceptor_types_.size(); i++)
			{
				if (acceptor_types_[i] == atom->getName())
				{
					acceptors_.push_back(atom);
				}
			}

			// As hydrogen bond donors we allow alpha protons or simple hydrogens.
			if (    (atom->getName().hasSubstring( "HA"))
					 || (amide_protons_are_targets_ && (atom->getName() == "H") )
				 )
			{
				donors_.push_back(atom);
			}
		}

		return Processor::CONTINUE;
	}

	Atom* HBondShiftProcessor::getDonor_(Atom* a)
	{
		Atom * partner = NULL;

		BALL::Atom::BondIterator bi = a->beginBond();
		for (; +bi; ++bi)
		{
			Atom* b =  bi->getPartner(*a);
			if (   (a->getName().hasSubstring("HA") &&  b->getName()=="CA")
			    || (a->getName()=="H" &&  b->getName()=="N"))

			{
				partner = b;
			}
		}

		return partner;
	}


	void HBondShiftProcessor::printParameters_()
	{
		Log.info() << "********* \n HB: list of acceptor types" << std::endl;

		vector< BALL::String >::const_iterator a_it = acceptor_types_.begin();
		for(;a_it != acceptor_types_.end(); ++a_it)
		{
			Log.info() <<(*a_it) << "  " ;
		}
		Log.info() << std::endl;
		Log.info() << "------------------------------\n" << std::endl;

		Log.info() << "********* \n HB: list of parmeters" << std::endl;
		Log.info() << "\tamide_protons_are_targets_: "<< amide_protons_are_targets_<< std::endl;
		Log.info() << "\tamide_proton_factor_: "<< amide_proton_factor_ << std::endl;
		Log.info() << "\tamide_proton_subtrahend_: "<< amide_proton_subtrahend_<< std::endl;
		Log.info() << "\tHN_oxygen_hydrogen_distance_: "<< amide_proton_oxygen_hydrogen_separation_distance_ << std::endl;
		Log.info() << "\tHA_oxygen_hydrogen_distance_: "<< alpha_proton_oxygen_hydrogen_separation_distance_ << std::endl;
		Log.info() << "\texclude_selfinteraction_: "<<exclude_selfinteraction_ << std::endl;
		Log.info() << "------------------------------\n" << std::endl;

	}

	void HBondShiftProcessor::printEffectors_()
	{
		Log.info() << "********* \n HB: list of donors" << std::endl;
		vector< BALL::Atom*>::const_iterator a_it = donors_.begin();
		for(;a_it != donors_.end(); ++a_it)
		{
			Log.info() << (*a_it)->getResidue()->getID() << "  " << (*a_it)->getFullName() << "  " << std::endl;
		}
		Log.info() << "------------------------------\n" << std::endl;
	}

	void HBondShiftProcessor::printTargets_()
	{
		Log.info() << "********* \n HB: list of acceptors" << std::endl;

		vector< BALL::Atom*>::const_iterator a_it = acceptors_.begin();
		for( ; a_it!=acceptors_.end(); ++a_it)
		{
			Log.info() << (*a_it)->getResidue()->getID() << "  "  << (*a_it)->getFullName() << "  " << std::endl;
		}
		Log.info() << "------------------------------\n" << std::endl;
	}


	void HBondShiftProcessor::postprocessing_()
	{
		// Try to get the system.
		System* system = NULL;

		for (Position i = 0; !system && i<acceptors_.size(); i++)
		{
			if  (RTTI::isKindOf<System>(acceptors_[i]->getRoot()))
			{
				system = dynamic_cast<System*>(&(acceptors_[i]->getRoot()));
			}
		}

		if (system)
		{
			// Add for all CA-atoms 0.2 times the HB-shift-values of the bound HA-atom.
			for (BALL::ResidueIterator r_it = system->beginResidue(); r_it != system->endResidue(); ++r_it)
			{
				Atom* CA = 0;
				Atom* HA = 0;

				for (BALL::AtomIterator at_it = r_it->beginAtom(); +at_it; ++at_it)
				{
					if (at_it->getName() == "CA")
						CA = &(*at_it);
					if (at_it->getName() == "HA")
						HA = &(*at_it);
				}

				if (CA && HA)
				{
					float total = CA->getProperty(ShiftModule::PROPERTY__SHIFT).getFloat();
					float ca_shift = CA->getProperty(BALL::HBondShiftProcessor::PROPERTY__HBOND_SHIFT).getFloat();
					float ha_shift = HA->getProperty(BALL::HBondShiftProcessor::PROPERTY__HBOND_SHIFT).getFloat();

					CA->setProperty(BALL::HBondShiftProcessor::PROPERTY__HBOND_SHIFT, ca_shift + 0.2*ha_shift);
					CA->setProperty(ShiftModule::PROPERTY__SHIFT, total+ 0.2*ha_shift );
				}
			}
		}
		else
		{
			Log.error() << "Error in HBondShiftProcessor: no system found for postprocessing. ("
									<< __FILE__ << " " << __LINE__ << ")" <<  std::endl;

		}
	}

} // namespace BALL
