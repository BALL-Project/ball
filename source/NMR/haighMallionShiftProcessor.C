// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
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
			negative_ringcurrent_factor_(processor.negative_ringcurrent_factor_),
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
		valid_ = false;
		if (parameters_ == 0)
		{
			return;
		}

		// Check that the parameter file contains the correct section...
		ParameterSection parameter_section;
		parameter_section.extractSection(*parameters_, "HaighMallionRingCurrent");
		
		// ..and that this section contains the correct column names.
		if ( 		 !parameter_section.hasVariable("residue_type") 
					|| !parameter_section.hasVariable("intensity_factor")
					|| !parameter_section.hasVariable("ring_atoms"))
		{
			return;
		}

		// Check for the effctor-options.
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

		// Check for the cut off - option.
		use_cut_off_ = false;
		if (parameter_section.options.has("use_cut_off"))
		{
			use_cut_off_ = parameter_section.options.getBool("use_cut_off");
		}

		// Read the cut off.
		cut_off2_ = 0.;
		if (parameter_section.options.has("cut_off"))
		{
			cut_off2_ = parameter_section.options.getReal("cut_off");
			cut_off2_ *= cut_off2_;
		}

		// Read the project_target_to_ring_plane - option.
		project_target_to_ring_plane_ = false;
		if (parameter_section.options.has("project_target_to_ring_plane"))
		{
			project_target_to_ring_plane_ = parameter_section.options.getBool("project_target_to_ring_plane");
		}

		// Read the effector and target expressions.
		effector_names_.clear();
		intensity_factors_.clear();
		ring_atoms_.clear();
		target_names_.clear();
	
		// Extract the column indices.
		Position residue_type_column = parameter_section.getColumnIndex("residue_type");
		Position intensity_factor_column = parameter_section.getColumnIndex("intensity_factor");
		Position ring_atoms_column = parameter_section.getColumnIndex("ring_atoms");

		for (Position counter = 0; counter < parameter_section.getNumberOfKeys(); counter++)
		{
			String effector_name = parameter_section.getValue(counter, residue_type_column);

			effector_names_.push_back(effector_name);
			intensity_factors_[effector_name] = (parameter_section.getValue(counter, intensity_factor_column).toFloat());
			String current_ring_atoms = parameter_section.getValue(counter, ring_atoms_column);

			// Read the ring atoms and split the string containing them.
			vector<String> split_vector;
			current_ring_atoms.split(split_vector);
			ring_atoms_.push_back(split_vector);
		}
	
		
		// Check that the parameter file contains the correct section...
		parameter_section.extractSection(*parameters_, "RingCurrentTargets");
		
		// ..and that this section contains the correct column names.
		if ( 		 !parameter_section.hasVariable("name") )
		{
			return;
		}

		// Check for the all-hydrogens are targets - option.
		all_hydrogen_are_targets_ = false;
		if (parameter_section.options.has("all_hydrogens_are_targets"))
		{
			all_hydrogen_are_targets_ = parameter_section.options.getBool("all_hydrogens_are_targets");
		}		

		// Check whether CA shifts should be corrected by their HA shifts
		correct_CA_shifts_ = false;
		if (parameter_section.options.has("correct_CA_shifts"))
		{
			correct_CA_shifts_ = parameter_section.options.getBool("correct_CA_shifts");
		}		
		
		// Read the default hydrogen target nucleus.
		default_hydrogen_target_nucleus_factor_ = 5.13;
		if (parameter_section.options.has("default_hydrogen_target_nucleus_factor"))
		{
			default_hydrogen_target_nucleus_factor_ = parameter_section.options.getReal("default_hydrogen_target_nucleus_factor");
		}

		// Read the negative_ringcurrent_factor.
		negative_ringcurrent_factor_ = 1.1;
		if (parameter_section.options.has("negative_ringcurrent_factor"))
		{
			negative_ringcurrent_factor_ = parameter_section.options.getReal("negative_ringcurrent_factor");
		}
	
		// Extract the column indices.
		Position target_names_column = parameter_section.getColumnIndex("name");
		Position target_nucleus_factor_column = parameter_section.getColumnIndex("target_nucleus_factor");

		// Read the ring current targets and target nucleus factors.
		for (Position counter = 0; counter < parameter_section.getNumberOfKeys(); counter++)
		{
			String target_name = parameter_section.getValue(counter,  target_names_column);
			target_names_.push_back(target_name);
			target_nucleus_factors_[target_name] = parameter_section.getValue(counter, target_nucleus_factor_column).toFloat();
		}

		// Mark the module as initialized.
		valid_ = true;
		
		// print the parameter set
		// printParameters_();
	}

	
	bool HaighMallionShiftProcessor::start ()
	{
		// If the module is invalid, abort.
		if (!isValid())
		{
			return false;
		}

		// Clear the target and effector lists.
		effectors_.clear();
		targets_.clear();
		
		return true;
	}

	bool HaighMallionShiftProcessor::finish()
		
	{
		//printEffectors_();
		//printTargets_();

		if (!isValid())
		{
			return false;
		}
		
		// If there were no effectors or no bonds, return immediately.
		if (targets_.empty() || effectors_.empty())
		{
			return true;
		}

		// For all effectors (rings)...
		for (Position e = 0; e < effectors_.size(); e++ )
		{
			// ...compute the ring normal.
			Size ring_size = effectors_[e].size();
			if (ring_size < 3)
			{
				// With less than three atoms we cannot compute a ring normal here! 
				// Continue to the next combination!
				continue;
			}	
				
			Vector3 ring_normal(0.);
			Vector3 current_normal;
			// Starting from each ring position we compute a ring normal and
			// normalize over all normals.
			for (Position pos = 0; pos < ring_size; pos++)
			{
				const Vector3& left  	 = effectors_[e][(pos + 0) % (ring_size)]->getPosition();
				const Vector3& middle  = effectors_[e][(pos + 1) % (ring_size)]->getPosition();
				const Vector3& right   = effectors_[e][(pos + 2) % (ring_size)]->getPosition();
				
				current_normal = (middle - right) % (middle - left);   

				if (current_normal.getSquareLength() != 0)
				{
					current_normal.normalize();
					// Do they point into the same direction? 
					if((ring_normal.getLength() == 0. )|| (current_normal*ring_normal > 0))
						ring_normal += current_normal;
					else
						Log.error() << "HaighMallionShiftProcessor: ring normals point into different directions (" 
												<< __FILE__ << " " << __LINE__ << ")" <<  std::endl;
				}
			}

			// This should not happen, but who knows... 
			// maybe the ring atoms all lie on a straight line?
			if (ring_normal.getSquareLength() == 0.)
				continue;

			// Normalize the normal vector to unit length.
			ring_normal.normalize(); 

			// For all targets...
			for (Position t = 0; t < targets_.size(); t++ )
			{
				// ...check if H and HA are influenced by all effectors or just by effectors of the same residue? 
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

				// Project the target atom position onto the ring plane, if the corresponding flag is set.
				Vector3 atom_projection = 
						(project_target_to_ring_plane_) ? 	 targets_[t]->getPosition() 
																							+ ((targets_[t]->getPosition() - 
																										effectors_[e][0]->getPosition())*ring_normal)*ring_normal
																						:    targets_[t]->getPosition();

				if (use_cut_off_)
				{
					// Compute the ring center for the cut_off computation.
					Vector3 center;
					for (Position pos = 0; pos < ring_size; pos++)
					{
						center += effectors_[e][pos]->getPosition();
					}
					center /= (double)ring_size;

					if (atom_projection.getSquareDistance(center) > cut_off2_)
						continue;
				}

				// Now we can compute the geometrical factor using the areas of the triangles 
				// of adjacent ring atoms with the point atom_projection.
				float geometrical_factor = 0;

				// We build each triangle.
				for (Position pos = 0; pos < ring_size; pos++ )
				{
					// Build r_i and r_j=r_{i+1}.
					Vector3 r_i = effectors_[e][(pos + 0) % (ring_size)]->getPosition() - atom_projection;
					Vector3 r_j = effectors_[e][(pos + 1) % (ring_size)]->getPosition() - atom_projection;

					// Compute the orientation with respect to the normal => _algebraic_ areas.
					float orientation = 1.0;

					if (((r_i % r_j) * ring_normal) > 0.) 
						orientation = -1.0;
						
					float area = 0.;

					// If desired, project the target atom onto the ring plane.
					if (project_target_to_ring_plane_)
					{
						Vector3 u = r_j - r_i;
						float b = u.getLength();
						Vector3 v = r_i*(-1.);
						Vector3 w = (u*ring_normal)*ring_normal - u%ring_normal;
						w.normalize();
						area = fabs(b * (v*w) / 2.);
					}
					else
					{
						area = r_i * (r_j % ring_normal) / 2.;
					}

					area *= pow((r_i + atom_projection - targets_[t]->getPosition()).getLength(),-3) 
						   +  pow((r_j + atom_projection - targets_[t]->getPosition()).getLength(), -3);
							
					geometrical_factor += area * orientation;	
				}
				

				//
				// Add up the target shift contributions of all effector rings.
				//
				
				float target_nucleus_factor = 0.;

				// Find the right target factor.
				if (target_nucleus_factors_.has(targets_[t]->getName()))
						target_nucleus_factor = target_nucleus_factors_[targets_[t]->getName()];
				else if (targets_[t]->getElement().getSymbol() == "H")
					target_nucleus_factor = default_hydrogen_target_nucleus_factor_;

				float new_rc_shift  = intensity_factors_[effector_types_[e]] * target_nucleus_factor * geometrical_factor;
				
				// Negative ring current correction a la ShitX.
				if (new_rc_shift < 0.)
					new_rc_shift *= negative_ringcurrent_factor_;

				float old_rc_shift = 0.;
				if (targets_[t]->hasProperty(PROPERTY__RING_CURRENT_SHIFT))
					old_rc_shift = targets_[t]->getProperty(PROPERTY__RING_CURRENT_SHIFT).getFloat();

				/** TODO: In case of GLY, which has 1HA and 2HA, build the average of both.
				*/
				
				float shift = targets_[t]->getProperty(ShiftModule::PROPERTY__SHIFT).getFloat();

				targets_[t]->setProperty(ShiftModule::PROPERTY__SHIFT, shift + new_rc_shift);
			  targets_[t]->setProperty(PROPERTY__RING_CURRENT_SHIFT, new_rc_shift + old_rc_shift );			
			}
		}
	
		// We have to do some ShiftX-y postprocessing: 
		// add for all CA-atoms 0.2 times the HaighMallionShift-value of HA-atoms.
		postprocessing_();
		return true;
	}
		
	Processor::Result HaighMallionShiftProcessor::operator () (Composite& composite)
		
	{
		// Here, we collect all effectors (usually the aromatic residues)
		// and all atoms whose shift is to be calculated.
		
		// First, check if this is a residue...
		Residue* residue = dynamic_cast<Residue*>(&composite);
		if (residue != 0)
		{	
			// Yes -> check whether its name matches any of our list
			// of aromatic residue names.
			// To Think: perhaps it makes sense to use a string hash map for fast lookup.
			for (Position i = 0; i < effector_names_.size(); i++)
			{
				// Note:  We want to allow names like TRP1 and TRP2 to denote the 2 rings
				// of tryptophane, so we can't test for equality of the name of the effector and the residue!
				if (effector_names_[i].hasPrefix(residue->getName()))
				{
					effector_types_.push_back(effector_names_[i]);

					vector <BALL::Atom*> atoms;
					// To ensure that the atoms are inserted in the correct order (to have adjacent ring
					// atoms at adjacent positions in the vector), we iterate over all atoms that _should_
					// be in the ring in the correct order and test whether they _are_ indeed there.
					for (Position j = 0; j < ring_atoms_[i].size(); j++)
					{
						// Is the current ring atom contained in the residue?
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

		// Store all targets whose shift is to be calculated.
		Atom* atom = dynamic_cast<Atom*>(&composite);
		if (atom != 0)
		{
			atom->clearProperty(PROPERTY__RING_CURRENT_SHIFT);
			
			if (all_hydrogen_are_targets_ && atom->getElement().getSymbol() == "H")
			{
				targets_.push_back(atom);
			}
			else
			{
				for (Position i = 0; i < target_names_.size(); i++)
				{
					if (target_names_[i] == atom->getName())
					{
							targets_.push_back(atom);
					}
				}
			}
		}
		return Processor::CONTINUE;
	
	}

	void  HaighMallionShiftProcessor::printTargets_()
	{
		Log.info() << "********* \n HM: list of targets" << std::endl;
		for (Position i = 0; i<targets_.size(); i++)
		{
				Log.info() << targets_[i]->getName() << std::endl;
		}
		Log.info() << "------------------------------\n" << std::endl;

	}

	void  HaighMallionShiftProcessor::printEffectors_()
	{
		Log.info() << "********* \n HM: list of effectors" << std::endl;

		vector< vector<Atom*> >::const_iterator effector_it = effectors_.begin();
		for (;effector_it != effectors_.end(); ++effector_it)
		{
				for(Position i = 0;i < (*effector_it).size(); i++ )
				{
					Log.info()  << (*effector_it)[i]->getName() << "  ";
				}
				Log.info() <<  " " << std::endl;
		}
		Log.info() << "------------------------------\n" << std::endl;
	}

	void  HaighMallionShiftProcessor::printParameters_()
	{
		Log.info() << "********* \n HM: parameters" << std::endl;

		Log.info() << "\n\teffector names" << std::endl;
		vector< BALL::String >::const_iterator effector_names_it = effector_names_.begin();
		for(;effector_names_it != effector_names_.end(); ++effector_names_it)
		{
			Log.info() <<(*effector_names_it) << "  ";
		}
		Log.info() << std::endl;

		Log.info() << "\n\tintensity_factors" << std::endl;
		StringHashMap<float>::ConstIterator int_it = intensity_factors_.begin();
		for(;int_it != intensity_factors_.end(); ++int_it)
		{
			Log.info() <<int_it->first << "  " << int_it->second << std::endl;
		}
		Log.info() << std::endl;

		Log.info() << "\n\tring_atoms: " << std::endl;
		vector< vector<BALL::String> >::const_iterator ring_it = ring_atoms_.begin();
		for(;ring_it != ring_atoms_.end(); ++ring_it)
		{
			vector<BALL::String>::const_iterator atom_it = ring_it->begin();
			for(;atom_it != ring_it->end(); ++atom_it )
			{
				Log.info() << (*atom_it)<< "  ";
			}
			Log.info() << " " << std::endl;
		}
		Log.info() << std::endl;

		Log.info() << "\n\ttarget names: " << std::endl;
		vector< BALL::String >::const_iterator targets_it = target_names_.begin();
		for(;targets_it != target_names_.end(); ++targets_it)
		{
			Log.info() << (*targets_it) << "  ";
		}
		Log.info() << " " << std::endl;
	
		Log.info() << "\n\tparameters: " << std::endl;
		Log.info()  << "\tH_influenced_by_all_effectors_= " << H_influenced_by_all_effectors_ << std::endl;
		Log.info()  << "\tHA_influenced_by_all_effectors_ = " <<	HA_influenced_by_all_effectors_ << std::endl;
		Log.info()  << "\tuse_cut_off_= " << use_cut_off_<< std::endl;
		Log.info()  << "\tcut_off2_ = " << cut_off2_ << std::endl;
		Log.info()  << "\tall_hydrogen_are_targets_ = " << all_hydrogen_are_targets_ << std::endl;
		Log.info()  << "\tproject_target_to_ring_plane = " << project_target_to_ring_plane_ << std::endl;
		Log.info()  << "\tdefault_hydrogen_target_nucleus_factor = " << default_hydrogen_target_nucleus_factor_ << std::endl;

		Log.info() << "\n\ttarget nucleus factors: " << std::endl;
		StringHashMap<float>::ConstIterator t_it = target_nucleus_factors_.begin();
		for(;t_it!=target_nucleus_factors_.end(); ++t_it)
		{
			Log.info() << t_it->first << "  " << t_it->second << std::endl;
		}
		
		Log.info() << "------------------------------\n" << std::endl;
	}

	void  HaighMallionShiftProcessor::postprocessing_()
	{
		if (!correct_CA_shifts_)
			return;

		// Try to get the system.
		System* system = NULL;
		
		for (Position i = 0; !system && i<targets_.size(); i++)
		{
            if  (RTTI::isKindOf<System>(&targets_[i]->getRoot()))
			{	
				system = dynamic_cast<System*>(&(targets_[i]->getRoot()));
			}
		}

		if (system) 
		{
			// Add for all CA-atoms 0.2 times the HM shift-values of the bound HA-atom.
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
					float ca_shift = CA->getProperty(BALL::HaighMallionShiftProcessor::PROPERTY__RING_CURRENT_SHIFT).getFloat();
					float ha_shift	 = HA->getProperty(BALL::HaighMallionShiftProcessor::PROPERTY__RING_CURRENT_SHIFT).getFloat();

					CA->setProperty(BALL::HaighMallionShiftProcessor::PROPERTY__RING_CURRENT_SHIFT, ca_shift + 0.2*ha_shift);
					CA->setProperty(ShiftModule::PROPERTY__SHIFT, total+ 0.2*ha_shift );
				}
			}
		}
		else
		{	
			Log.error() << "Error in HaighMallionShiftProcessor: no system found for postprocessing. (" 
				 					<< __FILE__ << " " << __LINE__ << ")" <<  std::endl;
		}
	}
} // namespace BALL
