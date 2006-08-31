// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: HBondShiftProcessor.C,v 1.6.18.1 2006/08/31 17:56:30 anne Exp $
//

#include <BALL/NMR/HBondShiftProcessor.h>
#include <stdio.h>

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
			exclude_selfinteraction_(processor.exclude_selfinteraction_)
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
	
		exclude_selfinteraction_ = false;
		if (parameter_section.options.has("exclude_selfinteraction"))
		{
			exclude_selfinteraction_ = parameter_section.options.getBool("exclude_selfinteraction");
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

		// find all hydrogen bonds which include all donor -- acceptor pairs
		// and store them in 

		vector< std::pair< Atom*, Atom*> > bonds;
		
		for(Position d = 0; d < donors_.size(); ++d)
		{
			for(Position a = 0; a < acceptors_.size(); ++a)
			{
	//			if (donors_[d]->getResidue()->getName() == "ASN"  )
//std::cout << "N "<< donors_[d]->getResidue()->getID() << " -- "<< acceptors_[a]->getResidue()->getID()<< ": " << (donors_[d]->getPosition() - acceptors_[a]->getPosition()).getLength() << std::endl;

				Atom* donor = getDonor_(donors_[d]);
				if (donor == NULL)
					continue;
				
				BALL::Atom::BondIterator bi = donor->beginBond();
				for (; +bi; ++bi)
				{
					if (    (bi->getType() == Bond::TYPE__HYDROGEN) 
							 && (   ( (bi->getFirstAtom() == donor) && (bi->getSecondAtom() == acceptors_[a]) )
						       || ( (bi->getSecondAtom() == donor) && (bi->getFirstAtom() == acceptors_[a])  ) )) 
					{

std::cout << " passiert das überhaupt irgendwann?" << __LINE__ << "" << std::endl;
						// do we have to exclude self interactions?
						if (   exclude_selfinteraction_ 
								&& (donor->getResidue() == acceptors_[a]->getResidue()))
						{
							continue;	
						}
if (donors_[d]->getResidue()->getName() == "ASN" )
std::cout << "N -- "<< acceptors_[a]->getResidue()->getID()<< ": " << (donors_[d]->getPosition() - acceptors_[a]->getPosition()).getLength() << std::endl;
						// TO THINK: How to make sure that we got no bifurcated hydrogen bonds?
						pair <Atom*, Atom*> tuple(donors_[d],acceptors_[a]); 
						bonds.push_back(tuple);
					}
				}
			}
		}	// end of for loop


		std::cout << " anzahl HBONds " << bonds.size() << "" << std::endl;
	
		// compute all hydrogen bond effect shifts 
		// by construction is the first atom in pairs the donor, the second the acceptor! 
		for (Position b = 0; b < bonds.size(); ++b)
		{
			float new_hb_shift = 0.;
			double distance = ((bonds[b].first->getPosition() - bonds[b].second->getPosition()).getLength());
			
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
		
			if (   amide_protons_are_targets_ && (bonds[b].first->getName() == "H") 
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
			else if ((bonds[b].first->getName().hasSubstring("HA")) && (distance < alpha_proton_oxygen_hydrogen_separation_distance_ ))
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
				std::cout << " hatte doch noch einen Wert " <<  bonds[b].first->getName() << " : " << new_hb_shift <<  std::endl;
				float old_shift = bonds[b].first->getProperty(ShiftModule::PROPERTY__SHIFT).getFloat(); 
				float old_hb_shift = bonds[b].first->getProperty(HBondShiftProcessor::PROPERTY__HBOND_SHIFT).getFloat(); 
				
				bonds[b].first->setProperty(ShiftModule::PROPERTY__SHIFT, (old_shift + new_hb_shift));
				bonds[b].first->setProperty(HBondShiftProcessor::PROPERTY__HBOND_SHIFT, (old_hb_shift + new_hb_shift));
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
