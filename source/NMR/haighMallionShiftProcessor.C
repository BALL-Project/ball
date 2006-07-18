// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: haighMallionShiftProcessor.C,v 1.17.18.3 2006/07/18 18:42:46 anne Exp $
//

#include <BALL/NMR/haighMallionShiftProcessor.h>
#include <BALL/KERNEL/atomIterator.h>
#include <BALL/FORMAT/parameterSection.h>
#include <BALL/KERNEL/PTE.h>

using namespace std;

namespace BALL 
{

  const char* HaighMallionShiftProcessor::PROPERTY__RING_CURRENT_SHIFT = "RingCurrentShift[HM]";
	
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
			project_target_to_ring_plane_(processor.project_target_to_ring_plane_),
			target_nucleus_factors_(processor.target_nucleus_factors_),
			default_hydrogen_target_nucleus_factor_(processor.default_hydrogen_target_nucleus_factor_),
			effectors_(processor.effectors_),
			effector_types_(processor.effector_types_),
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

		//read the project_target_to_ring_plane - option
		project_target_to_ring_plane_ = false;
		if (parameter_section.options.has("project_target_to_ring_plane"))
		{
			project_target_to_ring_plane_ = parameter_section.options.getBool("project_target_to_ring_plane");
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
			String effector_name = parameter_section.getValue(counter, residue_type_column);

			effector_names_.push_back(effector_name);
			intensity_factors_[effector_name] = (parameter_section.getValue(counter, intensity_factor_column).toFloat());
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
		
		//read the default hydrogen target nucleus
		default_hydrogen_target_nucleus_factor_ = 5.13;
		if (parameter_section.options.has("default_hydrogen_target_nucleus_factor"))
		{
			default_hydrogen_target_nucleus_factor_ = parameter_section.options.getReal("default_hydrogen_target_nucleus_factor");
		}

		
		// extract the column indices
		Position target_names_column = parameter_section.getColumnIndex("name");
		Position target_nucleus_factor_column = parameter_section.getColumnIndex("target_nucleus_factor");

		// read the ring current targets and target nucleus factors
		for (Position counter = 0; counter < parameter_section.getNumberOfKeys(); counter++)
		{
			String target_name = parameter_section.getValue(counter,  target_names_column);
			target_names_.push_back(target_name);
			target_nucleus_factors_[target_name] = parameter_section.getValue(counter, target_nucleus_factor_column).toFloat();
		}

		// mark the module as initialized
		valid_ = true;
		
// print the parameter set
		printParameters_();
/*std::cout << "\teffector_names" << std::endl;
vector< BALL::String >::const_iterator effector_names_it = effector_names_.begin();
for(;effector_names_it != effector_names_.end(); ++effector_names_it)
{
	std::cout <<(*effector_names_it) << "  " << std::endl;
}
std::cout << "\tintensity_factors" << std::endl;
StringHashMap<float>::ConstIterator int_it = intensity_factors_.begin();
for(;int_it != intensity_factors_.end(); ++int_it)
{
	std::cout <<int_it->first << "  " << int_it->second << std::endl;
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
std::cout << "\tproject_target_to_ring_plane = " << project_target_to_ring_plane_ << std::endl;
std::cout << "\tdefault_hydrogen_target_nucleus_factor = " << default_hydrogen_target_nucleus_factor_ << std::endl;

std::cout << "\ttarget nucleus factors: " << std::endl;
StringHashMap<float>::ConstIterator t_it = target_nucleus_factors_.begin();
for(;+t_it; ++t_it)
{
	std::cout <<t_it->first << "  " << t_it->second << std::endl;
}
*/
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
		printEffectors_();
		printTargets_();

		if (!isValid())
		{
			return false;
		}
		
		// if there were no effectors or no bonds, return immediately
		if ((targets_.size() == 0) || (effectors_.size() == 0))
		{
			return true;
		}

		// over all effectors
		for (Position e = 0; e < effectors_.size(); e++ )
		{
			// computation of the ring normal
			Size ring_size = effectors_[e].size();
			if (ring_size < 3)
			{
				// we cannot compute a ring normal here! continue to the next combination!
				continue;
			}	
				
			Vector3 ring_normal(0.);
			Vector3 current_normal;
			for (Position pos = 0; pos < ring_size; pos++)
			{
				const Vector3& left  	 = effectors_[e][(pos + 0) % (ring_size)]->getPosition();
				const Vector3& middle  = effectors_[e][(pos + 1) % (ring_size)]->getPosition();
				const Vector3& right   = effectors_[e][(pos + 2) % (ring_size)]->getPosition();
				
				current_normal = (middle - right) % (middle - left);   

				if (current_normal.getSquareLength() != 0)
				{
					current_normal.normalize();
					ring_normal += current_normal;
				}
			}

			// this should not happen, but who knows... maybe the ring atoms all lie on a straight line?
			if (ring_normal.getSquareLength() == 0.)
				continue;

			// normalize the normal vector to unit length
			ring_normal.normalize(); 

			// and print it
			std::cout << ring_normal << std::endl;

			// over all targets
			for (Position t = 0; t < targets_.size(); t++ )
			{
				// can H and HA influenced by all effectors or just by effectors of the same residue? 
				if (    (targets_[t]->getName() == "H") 
						&& (!H_influenced_by_all_effectors_)
						&& (targets_[t]->getResidue() == effectors_[e][0]->getResidue()))
				{
					continue;
				}

				if (    (targets_[t]->getName() == "HA") 
						&& (!HA_influenced_by_all_effectors_)
						&& (targets_[t]->getResidue() == effectors_[e][0]->getResidue()))
				{
					continue;
				}

				// project the target atom position onto the ring plane
				Vector3 atom_projection = 
						(project_target_to_ring_plane_) ? 	 targets_[t]->getPosition() 
																							- (targets_[t]->getPosition()*ring_normal)*ring_normal
																						:    targets_[t]->getPosition();

				if (use_cut_off_)
				{
					// compute the ring center for the cut_off computation
					Vector3 center;
					for (Position pos = 0; pos < ring_size; pos++)
					{
						center += effectors_[e][pos]->getPosition();
					}
					center /= (double)ring_size;

					if (atom_projection.getSquareDistance(center) > cut_off2_)
						continue;
				}

				// now we can compute the areas of the triangles of adjacent ring atoms with the point atom_projection

				// build each triangle
				float geometrical_factor = 0;

				for (Position pos = 0; pos < ring_size; pos++ )
				{
					// build r_i and r_j=r_{i+1}
					Vector3 r_i = effectors_[e][(pos + 0) % (ring_size)]->getPosition() - atom_projection;
					Vector3 r_j = effectors_[e][(pos + 1) % (ring_size)]->getPosition() - atom_projection;

					// orientation with respect to the normal => _algebraic_ areas
					float orientation = 1.0;

					if (((r_i % r_j) * ring_normal) > 0.) 
						orientation = -1.0;
						
					float area=0.;
					if (project_target_to_ring_plane_)
					{
						area = (r_i % r_j).getLength() / 2.;
					}
					else
					{
						area = r_i * (r_j % ring_normal) / 2.;
					}
					
					area *= 1.0 / (r_i.getSquareLength() * r_i.getLength()) +  1.0 / (r_j.getSquareLength() * r_j.getLength());
								
					geometrical_factor += area * orientation;	
				}

				// add up the contributions of all rings

				float target_nucleus_factor=0.;

				if (target_nucleus_factors_.has(targets_[t]->getName()))
						target_nucleus_factor = target_nucleus_factors_[targets_[t]->getName()];
				else if (targets_[t]->getElement() == PTE[Element::H])
						target_nucleus_factor = default_hydrogen_target_nucleus_factor_;

				float new_rc_shift  = intensity_factors_[effector_types_[e]] * target_nucleus_factor * geometrical_factor;
				float old_rc_shift = targets_[t]->getProperty(PROPERTY__RING_CURRENT_SHIFT).getFloat();
				float shift = targets_[t]->getProperty(ShiftModule::PROPERTY__SHIFT).getFloat();

				targets_[t]->setProperty(ShiftModule::PROPERTY__SHIFT, shift+new_rc_shift);
			  targets_[t]->setProperty(PROPERTY__RING_CURRENT_SHIFT, old_rc_shift+new_rc_shift);			
			}
		}

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
			//To Think: perhaps it makes sense to use a string hash map for fast lookup 
			for (Position i = 0; i < effector_names_.size(); i++)
			{
				// Important Note! We want to allow names like TRP1 and TRP2 to denote the 2 rings
				// of tryptophane, so we can't test for equality of the name of the effector and the residue!
				if (effector_names_[i].hasSuffix(residue->getName()))
				{
					effector_types_.push_back(effector_names_[i]);

					vector <BALL::Atom*> atoms;
					// to ensure that the atoms are inserted in the correct order (to have adjacent ring
					// atoms at adjacent positions in the vector), we iterate over all atoms that _should_
					// be in the ring in the correct order and test whether they _are_ indeed there
					for (Position j = 0; j < ring_atoms_[i].size(); j++)
					{
						// is the current ring atom contained in the residue?
						AtomIterator ai = residue->beginAtom();
						for (; +ai; ++ai)
						{
							if (ring_atoms_[i][j] == ai->getName())
							{
								atoms.push_back(&*ai);
							}
						}
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

	void  HaighMallionShiftProcessor::printTargets_()
		throw()
	{
		std::cout << "********* \n HM:Liste der Targets " << std::endl;
		for (Position i = 0; i<targets_.size(); i++)
		{
			std::cout << targets_[i]->getName() << std::endl;
		}


	}

	void  HaighMallionShiftProcessor::printEffectors_()
		throw()
	{
		std::cout << "********* \n HM:Liste der Effectoren " << std::endl;
		vector< vector<Atom*> >::const_iterator effector_it = effectors_.begin();
		for (;effector_it != effectors_.end(); ++effector_it)
		{
				for(Position i = 0;i < (*effector_it).size(); i++ )
				{
					std::cout << (*effector_it)[i]->getName() << "  ";
				}
				std::cout << " " << std::endl;
		}
	}

	void  HaighMallionShiftProcessor::printParameters_()
		throw()
	{
		std::cout << "********* \n HM:Liste der Parameter " << std::endl;
		std::cout << "\teffector_names" << std::endl;
		vector< BALL::String >::const_iterator effector_names_it = effector_names_.begin();
		for(;effector_names_it != effector_names_.end(); ++effector_names_it)
		{
			std::cout <<(*effector_names_it) << "  " << std::endl;
		}
		std::cout << "\tintensity_factors" << std::endl;
		StringHashMap<float>::ConstIterator int_it = intensity_factors_.begin();
		for(;int_it != intensity_factors_.end(); ++int_it)
		{
			std::cout <<int_it->first << "  " << int_it->second << std::endl;
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
		std::cout << "\tproject_target_to_ring_plane = " << project_target_to_ring_plane_ << std::endl;
		std::cout << "\tdefault_hydrogen_target_nucleus_factor = " << default_hydrogen_target_nucleus_factor_ << std::endl;

		std::cout << "\ttarget nucleus factors: " << std::endl;
		StringHashMap<float>::ConstIterator t_it = target_nucleus_factors_.begin();
		for(;t_it!=target_nucleus_factors_.end(); ++t_it)
		{
			std::cout <<t_it->first << "  " << t_it->second << std::endl;
		}
	}


} // namespace BALL
