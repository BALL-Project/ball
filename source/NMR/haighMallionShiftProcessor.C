// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: haighMallionShiftProcessor.C,v 1.17.18.1 2006/07/02 16:30:10 anne Exp $
//

#include <BALL/NMR/haighMallionShiftProcessor.h>
#include <BALL/KERNEL/atomIterator.h>
#include <BALL/FORMAT/parameterSection.h>
#include <BALL/KERNEL/PTE.h>

using namespace std;

namespace BALL 
{

  const char* HaighMallionShiftProcessor::PROPERTY__RING_CURRENT_SHIFT = "RingCurrentShift[HM]";

	const float HaighMallionShiftProcessor::B_ = 5.455e-6;					//Konstante in iBG(r)

	
	HaighMallionShiftProcessor::HaighMallionShiftProcessor()
		:	ShiftModule()
	{
	}
		
	HaighMallionShiftProcessor::HaighMallionShiftProcessor(const HaighMallionShiftProcessor& processor)
		:	ShiftModule(),	
			H_influenced_by_all_effectors_(processor.H_influenced_by_all_effectors_),
			HA_influenced_by_all_effectors_(processor.HA_influenced_by_all_effectors_),
			use_cut_off_(processor.use_cut_off_),
			cut_off2_(processor.cut_off2_),
			all_hydrogen_are_targets_(processor.all_hydrogen_are_targets_),
			effectors_(processor.effectors_),
			effector_names_(processor.effector_names_),
			intensity_factors_(processor.intensity_factors_),
			ring_atoms_(processor.ring_atoms_),
			target_names_(processor.target_names_),
			targets_(processor.targets_)
	{
	}
		
	HaighMallionShiftProcessor::~HaighMallionShiftProcessor()
	{
	}

	

	void HaighMallionShiftProcessor::init()
	{
std::cout << " ******************* HM-Shift *******************" << std::endl;
		valid_ = false;
		if (parameters_ == 0)
		{
			return;
		}

		// check that the parameter file contains the correct section...
		ParameterSection parameter_section;
		parameter_section.extractSection(*parameters_, "HaighMallionRingCurrent");
		
		// ..and that this section contains the correct column names
		if ( 		 !parameter_section.hasVariable("residue_type") 
					|| !parameter_section.hasVariable("intensity_factor")
					|| !parameter_section.hasVariable("ring_atoms"))
		{
			return;
		}

		// check for the effctor-options
		H_influenced_by_all_effectors_ = false;
		if (parameter_section.options.has("H_influenced_by_all_effectors"))
		{
			H_influenced_by_all_effectors_ = parameter_section.options.getBool("H_influenced_by_all_effectors");
		}

		HA_influenced_by_all_effectors_ = false;
		if (parameter_section.options.has("HA_influenced_by_all_effectors"))
		{
			HA_influenced_by_all_effectors_ = parameter_section.options.getBool("HA_influenced_by_all_effectors");
		}

		// check for the cut off - option
		use_cut_off_ = false;
		if (parameter_section.options.has("use_cut_off"))
		{
			use_cut_off_ = parameter_section.options.getBool("use_cut_off");
		}

		// read the cut off
		cut_off2_ = 0.;
		if (parameter_section.options.has("cut_off"))
		{
			cut_off2_ = parameter_section.options.getReal("cut_off");
			cut_off2_ *= cut_off2_;
		}


		// read the effector and target expressions
		effector_names_.clear();
		intensity_factors_.clear();
		ring_atoms_.clear();
		target_names_.clear();
	
		// extract the column indices
		Position residue_type_column = parameter_section.getColumnIndex("residue_type");
		Position intensity_factor_column = parameter_section.getColumnIndex("intensity_factor");
		Position ring_atoms_column = parameter_section.getColumnIndex("ring_atoms");

		for (Position counter = 0; counter < parameter_section.getNumberOfKeys(); counter++)
		{
			effector_names_.push_back(parameter_section.getValue(counter, residue_type_column));
			intensity_factors_.push_back(parameter_section.getValue(counter, intensity_factor_column).toFloat());
			String current_ring_atoms = parameter_section.getValue(counter, ring_atoms_column);

			// read the ring atoms and split the string containing them
			vector<String> split_vector;
			current_ring_atoms.split(split_vector);
			ring_atoms_.push_back(split_vector);
		}
	
		
		// check that the parameter file contains the correct section...
		parameter_section.extractSection(*parameters_, "RingCurrentTargets");
		
		// ..and that this section contains the correct column names
		if ( 		 !parameter_section.hasVariable("name") )
		{
			return;
		}

		// check for the all-hydrogens are targets - option
		all_hydrogen_are_targets_ = false;
		if (parameter_section.options.has("all_hydrogens_are_targets"))
		{
			all_hydrogen_are_targets_ = parameter_section.options.getBool("all_hydrogens_are_targets");
		}		
		
		// extract the column indices
		Position target_names_column = parameter_section.getColumnIndex("name");

		// read the ring current targets
		for (Position counter = 0; counter < parameter_section.getNumberOfKeys(); counter++)
		{
			target_names_.push_back(parameter_section.getValue(counter,  target_names_column));
		}

		// mark the module as initialized
		valid_ = true;
		
// print the parameter set
std::cout << "\teffector_names" << std::endl;
vector< BALL::String >::const_iterator effector_names_it = effector_names_.begin();
for(;effector_names_it != effector_names_.end(); ++effector_names_it)
{
	std::cout <<(*effector_names_it) << "  " << std::endl;
}
std::cout << "\tintensity_factors" << std::endl;
vector< float>::const_iterator int_it = intensity_factors_.begin();
for(;int_it != intensity_factors_.end(); ++int_it)
{
	std::cout <<(*int_it) << "  " << std::endl;
}

std::cout << "\tring_atoms: " << std::endl;
vector< vector<BALL::String> >::const_iterator ring_it = ring_atoms_.begin();
for(;ring_it != ring_atoms_.end(); ++ring_it)
{
		vector<BALL::String>::const_iterator atom_it = ring_it->begin();
		for(;atom_it != ring_it->end(); ++atom_it )
		{
			std::cout << (*atom_it)<< "  ";
		}
		std::cout << " " << std::endl;
}

std::cout << "\ttarget names: " << std::endl;
vector< BALL::String >::const_iterator targets_it = target_names_.begin();
for(;targets_it != target_names_.end(); ++targets_it)
{
	std::cout <<(*targets_it) << "  " << std::endl;
}



std::cout << "\tH_influenced_by_all_effectors_= " << H_influenced_by_all_effectors_ << std::endl;
std::cout << "\tHA_influenced_by_all_effectors_ = " <<	HA_influenced_by_all_effectors_ << std::endl;
std::cout << "\tuse_cut_off_= " << use_cut_off_<< std::endl;
std::cout << "\tcut_off2_ = " << cut_off2_ << std::endl;
std::cout << "\tall_hydrogen_are_targets_ = " << all_hydrogen_are_targets_ << std::endl;

	}

	
	bool HaighMallionShiftProcessor::start ()
	{
std::cout << "-------------- HM-start() --------------- "<< std::endl;	
		// if the module is invalid, abort
		if (!isValid())
		{
			return false;
		}

		// clear the target and effector lists 
		effectors_.clear();
		targets_.clear();
		
		return true;
	}

	bool HaighMallionShiftProcessor::finish()
		
	{
std::cout << "-------------- HM-finish() --------------- "<< std::endl;	
// ------------------------print the parameter set
std::cout << "\teffectors: " << std::endl;
vector< vector<Atom*> >::const_iterator effector_it = effectors_.begin();
for (;effector_it != effectors_.end(); ++effector_it)
{
		for(Position i = 0;i < (*effector_it).size(); i++ )
		{
			std::cout << (*effector_it)[i]->getName() << "  ";
		}
		std::cout << " " << std::endl;
}
std::cout << "\n\ttargets: " << std::endl;
for (Position i = 0; i<targets_.size(); i++)
{
	std::cout << targets_[i]->getName() << std::endl;
}

//----------------------------end print

		if (!isValid())
		{
			return false;
		}
		
		// Berechnung des shifts fuer jedes Proton der liste proton_list
				
		// hshift bezeichnet den chemicalshift der durch den gerade abgearbeiteten Ring verursacht wird.
			
		// iteriere ueber alle Protonen
		// fuer jedes Proton iteriere ueber alle Ringe und berechne chemical_shift
		
		Vector3 ring_positions[6];

		Size counter = 0;
		Size number_of_rings = 0;
		
		// if there were no effectors or no bonds, return immediately
		if ((targets_.size() == 0) || (effectors_.size() == 0))
		{
			return true;
		}

		// over all targets
		for (Position t = 0; t < targets_.size(); t++ )
		{
			float shift = 0;
			// over all effectors
			for (Position e = 0; e < effectors_.size(); e++ )
			{
				// can H and HA influenced by all effectors or just by effectors of the same residue? 
				if (    (targets_[t].getName() == "H") 
						 && (!H_influenced_by_all_effectors_)
						 && (targets_[t]->getResidue() == effectors_[e][0]->getResidue()))
				{
					continue;
				}
	
				if (    (targets_[t].getName() == "HA") 
						 && (!HA_influenced_by_all_effectors_)
						 && (targets_[t]->getResidue() == effectors_[e][0]->getResidue()))
				{
					continue;
				}
	
				
			}
		}
		
		/*
		for (list<Atom*>::iterator proton_iter = proton_list_.begin();
				 proton_iter != proton_list_.end(); ++proton_iter)
		{
			float shift = 0;
			// for all effectors
			for (list<Residue*>::iterator arom_iter = aromat_list_.begin();
					 arom_iter != aromat_list_.end(); ++arom_iter)
			{
				// consider the HN and Halpha protons in the same residue only	i
				//@H_influenced_by_all_effectors=true
				//@HA_influenced_by_all_effectors=true

				const Residue& residue = *(*arom_iter);
				if (((*proton_iter)->getResidue() == &residue)
						&& ((*proton_iter)->getName() != "HA") 
						&& ((*proton_iter)->getName() != "H"))
				{
					continue;
				}

				// the intensity factor i in  \delta = i B G(r)
				// (different for each aromatic residue and relative to benzene = 1.0)
				float intensity_factor = 1.0;  // effectotr liefert I :-)) 

				if (residue.getName()=="TRP")
				{
					counter = 0;
					number_of_rings = 2;
					intensity_factor = 1.05;
				}
				if (residue.getName()=="PHE")
				{
					counter = 1;
					number_of_rings = 1;
					intensity_factor = 1.05;
				}
				if (residue.getName()=="TYR")
				{
					counter = 2; 
					number_of_rings = 1; 
					intensity_factor = 0.92;
				}
				if (residue.getName()=="HIS")
				{
					counter = 3; 
					number_of_rings = 1; 
					intensity_factor = 0.43;
				}

				Position number_of_ring_atoms = 0;
				Position hilf = 0;
				while (number_of_rings) // for all rings in the amino acid
				{
					if ((number_of_rings == 1) && (residue.getName() == "TRP"))	
					{
						intensity_factor = 1.04;
					}

					//Aufbau von vector_feld
					for (Position pos = hilf; pos < hilf + 6; pos++ )
					{
						if (asrings_[counter][1 + pos] == "NULL")
						{
							break;
						}

						for	(AtomConstIterator atom_iter = residue.beginAtom();
								+atom_iter; ++atom_iter)
						{
							if (asrings_[counter][1 + pos] == atom_iter->getName())
							{
								ring_positions[number_of_ring_atoms] = atom_iter->getPosition();
								number_of_ring_atoms++ ;
								break;
							}	
						}
					}
				
					// ?????: check for missing ring atoms!
					if (number_of_ring_atoms < 5)
					{
						Log.error() << "HMSP:finish: could not identify all ring atoms for " 
								<< residue.getName() << residue.getID() << ": found " << number_of_ring_atoms << endl;
					}
					else	
					{
						// determine the ring center				
						Vector3 center;
						for (Position pos = 0; pos < number_of_ring_atoms; pos++)
						{
							center += ring_positions[pos];
						}
						center /= (double)number_of_ring_atoms;
					
						// if the center of the ring is within the cut off,
						// perform the shift calculation
						Vector3 nucleus_pos = (*proton_iter)->getPosition();					
						if (nucleus_pos.getSquareDistance(center) <= cut_off2)
						{
							// determine the normal vector of the ring plane
							Vector3 normal;
							for (Position pos = 0; pos < number_of_ring_atoms; pos++)
							{
								const Vector3& left  = ring_positions[(pos + 0) % (number_of_ring_atoms)];
								const Vector3& middle  = ring_positions[(pos + 1) % (number_of_ring_atoms)];
								const Vector3& right = ring_positions[(pos + 2) % (number_of_ring_atoms)];
								normal += (middle - right) % (middle - left);  
							}
							// normalize the normal vector to unit length
							normal.normalize(); 

							// determine the sign of the normal
							float normal_sign = 1.0;
							
							Vector3 d1 = ring_positions[0] - center;
							Vector3 d2 = ring_positions[1] - center;
							Vector3 vp = d1 % d2;
							if ((vp * normal) > 0.0)
							{
								normal_sign = -1.0;
							}
							
							// loop over all ring bonds
							float sum = 0;
							for (Position pos = 0; pos < number_of_ring_atoms; pos++ )
							{
								// determine the contributions of one ring bond
								Vector3 r_1 = ring_positions[(pos + 0) % (number_of_ring_atoms)] - nucleus_pos;
								Vector3 r_2 = ring_positions[(pos + 1) % (number_of_ring_atoms)] - nucleus_pos;

								//
								//          	<r_1, (r_2 x n)>   /    1              1    \
								//   value =  ---------------- * |---------   +  ---------|
								//   	               2           \ |r_1|^3        |r_2|^3 /
								//
								float value  = r_1 * (r_2 % normal) * 0.5 * (1.0 / (r_1.getSquareLength() * r_1.getLength()) +  1.0 / (r_2.getSquareLength() * r_2.getLength()));
								
								// add the contributions of this ring
								sum += value;
							}

							// add up all contributions
							shift +=  intensity_factor * B_ * normal_sign * sum;
						}
					}

					number_of_ring_atoms = 0;
					number_of_rings--;
					hilf = 6; // fuer den naechsten schleifendurchlauf
				}
	
			}
			
			// Setze Property chemicalshift des gerade bearbeiteten Protons auf den entsprechenden Wert.
			float hshift = shift * 1e6;
			shift = (*proton_iter)->getProperty(ShiftModule::PROPERTY__SHIFT).getFloat();
			shift += hshift;
			(*proton_iter)->setProperty(ShiftModule::PROPERTY__SHIFT, shift);
			(*proton_iter)->setProperty(PROPERTY__RING_CURRENT_SHIFT, hshift);			
		}
		*/				
		return true;
	}
		
	Processor::Result HaighMallionShiftProcessor::operator () (Composite& composite)
		
	{
		// Here, we collect all effectors (usually the aromatic residues)
		// and all atoms whose shift is to be calculated
		
		// First, let's see if this is a residue...
		Residue* residue = dynamic_cast<Residue*>(&composite);
		if (residue != 0)
		{	
//std::cout << " **** " << residue->getName() << std::endl;

			// It is! Check whether its name matches any of our list
			// of aromatic residue names.
			for (Position i = 0; i < effector_names_.size(); i++)
			{
				if (effector_names_[i] == residue->getName())
				{
					//insert the entire residue into the effector-vector
					//To Think: perhaps it makes sense to store just the backbone_ring atoms (like in ring_atoms) 
					vector <BALL::Atom*> atoms;
					AtomIterator ai = residue->beginAtom();
					for (; +ai; ++ai)
					{
						atoms.push_back(&*ai);
					}
					
					effectors_.push_back(atoms);
				}
			}	
			
			return Processor::CONTINUE;
		} 

		// second we store all targets whose shift is to be calculated
		Atom* atom = dynamic_cast<Atom*>(&composite);
		if (atom != 0)
		{
			if (all_hydrogen_are_targets_  && (atom->getName() == "H"))
			{
					targets_.push_back(atom);
			}	
			
			for (Position i = 0; i < target_names_.size(); i++)
			{
				if (target_names_[i] == atom->getName())
				{
							targets_.push_back(atom);
				}
			}
		}
		return Processor::CONTINUE;
	
	}

} // namespace BALL
