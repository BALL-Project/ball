// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: HBondShiftProcessor.C,v 1.6.18.2 2006/09/01 17:19:36 anne Exp $
//

#include <BALL/NMR/HBondShiftProcessor.h>
#include <stdio.h>
#include <math.h>

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

		std::cout << __FILE__ << "hier im HbondProcessor" << __LINE__ << "" << std::endl;
	// by default, we assume the worst...
    valid_ = false;

    // if no parameters are assigned, abort immediately
    if (parameters_ == 0)
    {
      return;
    }

    // check that the parameter file contains the correct section...
    ParameterSection parameter_section;
    parameter_section.extractSection(*parameters_, "HBondEffect");
	
		// ..and that this section contains the correct column names
		if ( 		 !parameter_section.hasVariable("name")) 
		{
			return;
		}

		// check for the options
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


		// read the acceptor types
		acceptor_types_.clear();
		
		// extract the column indices
		Position acceptor_type_column = parameter_section.getColumnIndex("name");
		for (Position counter = 0; counter < parameter_section.getNumberOfKeys(); counter++)
		{
			String acceptor_type = parameter_section.getValue(counter, acceptor_type_column);
			acceptor_types_.push_back(acceptor_type);
		}
    
		// mark the module as initialized
    valid_ = true;
  }

  bool HBondShiftProcessor::start()
  {
    // if the module is invalid, abort
    if (!isValid())
    {
      return false;
    }

    // clear the donor list and the acceptor list
    donors_.clear();
    acceptors_.clear();

    return true;
  }

  bool HBondShiftProcessor::finish()
  {
		printParameters_(); 
  	printEffectors_();
  	printTargets_();
		
    // if the module is invalid, abort
    if (!isValid())
    {
      return false;
    }

    // if there were no donors or acceptors, return immediately
    if ((donors_.size() == 0) || acceptors_.size() == 0)
    {
      return true;
    }

		// should we use  hydrogen bonds computed la ShiftX?
		// elsewise we assume the the hydrogen bonds have been already set 
		
		/* NOTE: This will just compute __temporarily__  hydrogen bonds 
		 * 	 				found with the ShiftX - algorithm, whose definition of a 
		 * 	 				hydrogen bond differ substantially from a BALL definition.
		 * 	 		The ShiftX definition of hydrogen bonds is: 
		 * 	 				Donors are: H and HA
		 * 	 				Acceptors are: O, OD_n, OE_n, OG_n, OH_n or water in the solvent!
		 * 	 				Donors and Acceptors have to be on different residues
		 * 	 				If the acceptor is a solvent oxygen, the donor must not be a HA
		 * 	 				The oxygen--hydrogen separation must be less than 3.5 A for H(N) 
		 * 	 					and 2.77 A for HA.
		 * 	 				the angel between N_H bond vector and the C=O bond vector must be 
		 * 	 					90 degrees or more.
		 * 			Having applied these rules to each donor--acceptor pair, ShiftX then 
		 * 			sorts the list of possible bonds by the O_H separation distance, shortest to 
		 * 			longest. The list is the processed so that only the single strongest hydrogen 
		 * 			bond is identified for each donor--acceptor pair. Up to that point
		 * 			any bond involving the same donor or acceptor is preculded! 
		 */
		if (ShiftXwise_hydrogen_bonds_computation_)
		{
			// the potential donors and acceptors are already stored 
			// in donors_ and acceptors_
			
			// we need a datastructure to collect the hydrogen bonds
			
			for(Position d = 0; d < donors_.size(); ++d)
			{
				for(Position a = 0; a < acceptors_.size(); ++a)
				{	
					// does the bond fullfill all SHiftX criteria?

					// nearest neighbour effect
					if (   exclude_selfinteraction_ 
								&& (donors_[d]->getResidue() == acceptors_[a]->getResidue()))
					{
						continue;	
					}
				
					//  oxygen--hydrogen separation
					float distance =  (donors_[d]->getPosition() - acceptors_[a]->getPosition()).getLength();
					if (   (donors_[d]->getName().hasSubstring("HA") && (distance > alpha_proton_oxygen_hydrogen_separation_distance_ ))
							|| (donors_[d]->getName().hasSubstring("H")  && (distance > amide_proton_oxygen_hydrogen_separation_distance_ )))
						continue;
					
					// TODO: acceptor is a solvent oxygen, the donor must not be a HA
					
					// the angle criterion
					// find the backbone C, O, N, H
					Atom* C = NULL;
					Atom* O = NULL;
					Atom* N = NULL;
					Atom* H = NULL;
					
					for (AtomIterator ai = acceptors_[a]->getResidue()->beginAtom(); +ai; ++ai)
					{
						if (ai->getName() == "C") 
						{
							C = &(*ai);
						}
						else if (ai->getName() == "O") 
						{
							O = &(*ai);
						}
						else if (ai->getName() == "N") 
						{
							N = &(*ai);
						}
						else if (ai->getName() == "H") 
						{
							H = &(*ai);
						}
					}	
				
					if (!C || !O || !N || !H) continue;
					
					// compute the vectors CO and NH
					BALL::Vector3 CO = O->getPosition() - C->getPosition();
					BALL::Vector3 NH = H->getPosition() - H->getPosition();
					
					if (acos((CO*NH)/(CO.getLength() * NH.getLength())) < (Constants::PI/2.) )
						continue;
					
					// put the HBond into the hydrogen bond distance list
					distances_[donors_[d]][distance]= acceptors_[a];  
				}
			}
			
		}
		else  // we assume that the molecule has already got hydrogen bonds 
		{
			// find all hydrogen bonds which include all 
			// donor -- acceptor pairs and store them in distances_

			for(Position d = 0; d < donors_.size(); ++d)
			{
				for(Position a = 0; a < acceptors_.size(); ++a)
				{
//if (donors_[d]->getName().hasSubstring("HA")  && (donors_[d]->getPosition() - acceptors_[a]->getPosition()).getLength() < 10 )
	//std::cout << donors_[d]->getResidue()->getID() << donors_[d]->getFullName() << " -- "<< acceptors_[a]->getResidue()->getID()<< acceptors_[a]->getName()<< " : " << (donors_[d]->getPosition() - acceptors_[a]->getPosition()).getLength() << std::endl;
					
					// NOTE: BALL defines hydrogen bonds in a different way as ShiftX does
					// e.g. in BALL  N is the donor and O the acceptor
					// whereas in ShiftX the H is the donor, O is the acceptor
					Atom* donor = getDonor_(donors_[d]);
					if (donor == NULL)
						continue;

					// do we have to exclude self interactions?
					if (   exclude_selfinteraction_ 
							&& (donor->getResidue() == acceptors_[a]->getResidue()))
					{
						continue;	
					}

					BALL::Atom::BondIterator bi = donor->beginBond();
					for (; +bi; ++bi)
					{

//if (donors_[d]->getName().hasSubstring("HA") )
	//std::cout << donors_[d]->getResidue()->getID() << donors_[d]->getFullName() << " -- "<< acceptors_[a]->getResidue()->getID()<< acceptors_[a]->getName()<< " : " <<  Bond::TYPE__HYDROGEN << "/" << (bi->getType()) << " " << (bi->getFirstAtom() == donor)  << " - " << (bi->getSecondAtom() == acceptors_[a]) << std::endl;

						// compute the distance	
						float distance =  (donors_[d]->getPosition() - acceptors_[a]->getPosition()).getLength();

						if (    (bi->getType() == Bond::TYPE__HYDROGEN) 
								&& (   ( (bi->getFirstAtom() == donor) && (bi->getSecondAtom() == acceptors_[a]) )
									|| ( (bi->getSecondAtom() == donor) && (bi->getFirstAtom() == acceptors_[a])  ) )) 
						{

std::cout 		<< "(" << acceptors_[a]->getResidue()->getID() 	<< " " << acceptors_[a]-> getFullName() 
						<< ") - (" << donor->getResidue()->getID()					<< " " << donors_[d]->getFullName()<< ")"
						<< " : " << (donors_[d]->getPosition() - acceptors_[a]->getPosition()).getLength() << std::endl;
							
							// put the HBond into the hydrogen bond distance list
							distances_[donors_[d]][distance]= acceptors_[a];
						}
					}// end of for all bonds
				}
			}// end of for loop
		}	// end of else (already set hydrogen bonds)


		// now  compute all hydrogen bond effect shifts 
		// by chosing the data structure map <Atom *, map <Atom*, float> >
		// we ensure automatically that 
		// 		each donor/acceptor is just considered once and
		// 		by accessing the map by distances_[i].begin->first / (--(distances_[i].end()))->first 
		// 		we get the shortest/longest hydrogen bond distance for the donor i;
		// 		As an application of the perfect matching problem, 
		// 		this approach is optimal, since it is optimal for the perfect matching problem
		// 		TO THINK: but is it the solution ShiftX got?? 
		std::map<Atom* , std::map <float, Atom*> >::iterator it_d = distances_.begin();
		for (; it_d != distances_.end(); it_d++)
		{
			Atom* donor    = it_d->first;
		 	Atom* acceptor = distances_[donor].begin()->second;
//	Atom* acceptor2 = (--(distances_[donor].end()))->second;
			double distance = distances_[donor].begin()->first;
//float acceptor2 = (--(distances_[donor].end()))->first;
//std::cout << "first: "<< acceptor1 << "   last: " <<acceptor2  << std::endl;			
			double new_hb_shift = 0.;
				
 			// when donor is amide proton, 
			// we compute the shift as done in ShiftX
			// NOTE: the variables' values differ from the values given in the ShiftX paper!
			//
			//					  amide_proton_factor_
			//  shift =  ----------------------  - amide_proton_subtrahend_
			// 								r^3
			// 
			// where r denotes the distance between donor and acceptor of the 
			// hydrogen bond under consideration
			//
	
			if (   amide_protons_are_targets_ && (donor->getName() == "H") 
					&& (distance < amide_proton_oxygen_hydrogen_separation_distance_) ) 
			{
				new_hb_shift = (amide_proton_factor_ /(pow(distance, 3.)) ) - amide_proton_subtrahend_ ;
			}
			// when the donor is an alpha proton
			// we compute the shift as done in the ShiftX
			// NOTE: the formular differs from the formular given in the ShfitX paper!
			//
			//
			//  				0.147521    (1.65458E-05)		 0.000134668				0.0598561			 15.6855
			//	shift = -------  - -------------  - -------------   +  -----------  + --------  - 0.673905
			// 				   	r							r^1.5					  r*r								r^2.5					r^3
			// 				   	
			// 	where r denotes the distance between donor and acceptor of the 
			// hydrogen bond under consideration
			// 
			// 
			else if ((donor->getName().hasSubstring("HA")) && (distance < alpha_proton_oxygen_hydrogen_separation_distance_ ))
			{
				// according to shiftX one has to correct the distance
				if (distance >  2.60756 ) distance = 2.60756;
				if (distance <  2.26808 ) distance = 2.26808;
			
 				new_hb_shift = 0.147521/distance - (1.65458E-05)/pow(distance, 1.5) - 0.000134668/(distance*distance) + 
		 	 0.0598561/pow(distance, 2.5) + 15.6855/(distance*distance*distance) - 0.673905;
			}
		
			// store the shift in the hydrogen if necessary
			if (new_hb_shift !=0)
			{
//std::cout << " hatte doch noch einen Wert " <<  bonds[b].first->getName() << " : " << new_hb_shift <<  std::endl;
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
		// here, we collect all possible acceptors and donors
		if (RTTI::isKindOf<Atom>(object))
		{
			Atom* atom = RTTI::castTo<Atom>(object);

			// we store the hydrogen bond acceptors
			for (Position i = 0; i < 	acceptor_types_.size(); i++)
			{
				if (acceptor_types_[i] == atom->getName())
				{
						acceptors_.push_back(atom);
				}
			}
			
			// and the hydrogen bond donors
			if (    (atom->getName().hasSubstring( "HA")) 
					 || (amide_protons_are_targets_ && (atom->getName() == "H") ) 
				 ) 
			{
				donors_.push_back(atom);
			}
		}

		return Processor::CONTINUE;
	}

	Atom* 		HBondShiftProcessor::getDonor_(Atom* a) 
	{
		Atom * partner = NULL;
		
		BALL::Atom::BondIterator bi = a->beginBond();
		for (; +bi; ++bi)
		{
			Atom* b =  bi->getPartner(*a);
			if (  	(a->getName().hasSubstring("HA") &&  b->getName()=="CA")
			    || (a->getName()=="H" &&  b->getName()=="N"))

			{
				partner = b;
			}
		}
		
		return partner; 
	}


	void 		HBondShiftProcessor::printParameters_() 
	{
		std::cout << "********* \n HB:Liste der Parameter " << std::endl;
		std::cout << "\tacceptor_types" << std::endl;

		vector< BALL::String >::const_iterator a_it = acceptor_types_.begin();
		for(;a_it != acceptor_types_.end(); ++a_it)
		{
			std::cout <<(*a_it) << "  " << std::endl;
		}
		std::cout << "\n\tamide_protons_are_targets_: "<< amide_protons_are_targets_<< " " 
							<< "\n\tamide_proton_factor_: "<< amide_proton_factor_ << " " 
							<< "\n\tamide_proton_subtrahend_: "<< amide_proton_subtrahend_<< " " 
							<< "\n\tHN_oxygen_hydrogen_distance_: "<< amide_proton_oxygen_hydrogen_separation_distance_ << " " 
							<< "\n\tHA_oxygen_hydrogen_distance_: "<< alpha_proton_oxygen_hydrogen_separation_distance_ << " " 
							<< "\n\texclude_selfinteraction_: "<<exclude_selfinteraction_ << " "
//	<< "\n\t: "<< << " "
							<< std::endl;
	}
	
	void  	HBondShiftProcessor::printEffectors_() 
	{
		std::cout << "********* \n HB:Liste der Donoren " << std::endl;
		vector< BALL::Atom*>::const_iterator a_it = donors_.begin();
		for(;a_it != donors_.end(); ++a_it)
		{
			std::cout << (*a_it)->getResidue()->getID() << "  " << (*a_it)->getFullName() << "  " << std::endl;
		}
		std::cout	<< std::endl;
	}
	
	void 		HBondShiftProcessor::printTargets_() 
	{
		std::cout << "********* \n HB:Liste der Akzeptoren " << std::endl;
		
		vector< BALL::Atom*>::const_iterator a_it = 	acceptors_.begin();
		for(;a_it != 	acceptors_.end(); ++a_it)
		{
			std::cout<< (*a_it)->getResidue()->getID() << "  "  << (*a_it)->getFullName() << "  " << std::endl;
		}
		std::cout	<< std::endl;

	}

	
} // namespace BALL
