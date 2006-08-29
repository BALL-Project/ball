// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: EFShiftProcessor.C,v 1.14.20.3 2006/08/29 09:11:52 anne Exp $

#include<BALL/NMR/EFShiftProcessor.h>
#include <BALL/COMMON/limits.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/FORMAT/parameterSection.h>

using namespace std;

namespace BALL 
{
	
	const char* EFShiftProcessor::PROPERTY__EF_SHIFT = "ElectricFieldShift";

	EFShiftProcessor::EFShiftProcessor()
		
		:	ShiftModule()
	{	
	}

	EFShiftProcessor::EFShiftProcessor(const EFShiftProcessor& processor)
		
		:	ShiftModule(processor),
			bond_list_(processor.bond_list_),
			effector_list_(processor.effector_list_),
			first_atom_expressions_(processor.first_atom_expressions_),
			second_atom_expressions_(processor.second_atom_expressions_),
			epsilon1_(processor.epsilon1_),
			epsilon2_(processor.epsilon2_),
			charge_map_(processor.charge_map_),
			exclude_residue_field_(processor.exclude_residue_field_),
			exclude_adjacent_residue_field_(processor.	exclude_adjacent_residue_field_),
			carbonyl_influences_amide_field_(processor.carbonyl_influences_amide_field_),
			exclude_solvent_field_(processor.exclude_solvent_field_),
			cut_off2_(processor.cut_off2_),
			charge_factor_(processor.charge_factor_)
	{
	}
	
	EFShiftProcessor::~EFShiftProcessor()
		
	{
	}

	void EFShiftProcessor::init()
		
	{
std::cout << "******************* EF-Shift ******************* " << std::endl;
		// by default, we assume the worst...
		valid_ = false;

		// if no parameters are assigned, abort immediately
		if (parameters_ == 0)
		{
			return;
		}

		// check that the parameter file contains the correct section...
		ParameterSection parameter_section;
		parameter_section.extractSection(*parameters_, "ElectricFieldEffect");

		// ..and that this section contains the correct column names
		if ( !parameter_section.hasVariable("first_atom") || !parameter_section.hasVariable("second_atom")
				|| !parameter_section.hasVariable("epsilon1") || !parameter_section.hasVariable("epsilon2"))
		{
			return;
		}

		// check for the option "exclude_residue_field"
		exclude_residue_field_ = false;
		if (parameter_section.options.has("exclude_residue_field"))
		{
			exclude_residue_field_ = parameter_section.options.getBool("exclude_residue_field");
		}
		
		// check for the option "exclude_adjacent_field"
		exclude_adjacent_residue_field_ = false;
		if (parameter_section.options.has("exclude_adjacent_residue_field"))
		{
			exclude_adjacent_residue_field_ = parameter_section.options.getBool("exclude_adjacent_residue_field");
		}
		
		// check for the option "carbonyl_influences_amide_field"
		carbonyl_influences_amide_field_ = false;
		if (parameter_section.options.has("carbonyl_influences_amide_field"))
		{
			carbonyl_influences_amide_field_ = parameter_section.options.getBool("carbonyl_influences_amide_field");
		}
		
		// check for the option "exclude_solvent_field"
		exclude_solvent_field_ = false;
		if (parameter_section.options.has("exclude_solvent_field"))
		{
			exclude_solvent_field_ = parameter_section.options.getBool("exclude_solvent_field");
		}

		// clear the arrays containing the expressions, the parameters, and the charge map
		first_atom_expressions_.clear();
		second_atom_expressions_.clear();
		epsilon1_.clear();
		epsilon2_.clear();
		charge_map_.clear();

		// extract the atom expressions and the corresponding polarizabilities
		Position first_atom_column = parameter_section.getColumnIndex("first_atom");
		Position second_atom_column = parameter_section.getColumnIndex("second_atom");
		Position epsilon1_column = parameter_section.getColumnIndex("epsilon1");
		Position epsilon2_column = parameter_section.getColumnIndex("epsilon2");

		for (Position counter = 0; counter < parameter_section.getNumberOfKeys(); counter++)
		{
			first_atom_expressions_.push_back(Expression(parameter_section.getValue(counter, first_atom_column)));
			second_atom_expressions_.push_back(Expression(parameter_section.getValue(counter, second_atom_column)));
			epsilon1_.push_back(parameter_section.getValue(counter, epsilon1_column).toFloat());
			epsilon2_.push_back(parameter_section.getValue(counter, epsilon2_column).toFloat());
		}

		// extract the charge assignment map
		bool result = parameter_section.extractSection(*parameters_, "Charges");

		// check for the cut off
		cut_off2_ = Limits<float>::max();
		if (parameter_section.options.has("cut_off"))
		{
			// store the squared value of the cut off in the member cut_off2_
			cut_off2_ = parameter_section.options.getReal("cut_off");
			cut_off2_ *= cut_off2_;
		}

		// default factor is 1.0 - default unit are elementary charges (e0)
		charge_factor_ = 1.0;
		if (parameter_section.options.has("unit"))
		{
			String unit = parameter_section.options["unit"];
			if (unit == "e0")
			{
				charge_factor_ = 1.0;
			}
			else if (unit == "ESU")
			{
				charge_factor_ = 1.0 / 4.8;
			}
			else
			{
				Log.warn() << "EFShiftProcessor::init: unknown unit for charges in file " 
									 << parameters_->getFilename() << ", section [Charges]: " 
									 << unit << " - using default unit elemtary charges (e0)." << endl;
			}
		}

		// built the hash map
		if ((result == true) && parameter_section.hasVariable("charge"))
		{
			Position charge_column = parameter_section.getColumnIndex("charge");
			for (Position i = 0; i < parameter_section.getNumberOfKeys(); i++)
			{
				charge_map_[parameter_section.getKey(i)] = charge_factor_ * parameter_section.getValue(i, charge_column).toFloat();
			}
		}

		printParameters_();

		// mark the module as initialized
		valid_ = true;
	}
		
	bool EFShiftProcessor::start()
		
	{
		// if the module is invalid, abort
		if (!isValid())
		{
			return false;
		}

		// clear the bond list and the effector list
		bond_list_.clear();
		effector_list_.clear();

		return true;
	}

	bool EFShiftProcessor::finish()
		
	{
		// if the module is in an invalid state, abort
		if (!isValid())
		{
			return false;
		}

		// if there were no effectors or no bonds, return immediately
		if ((bond_list_.size() == 0) || (effector_list_.size() == 0))
		{
			return true;
		}

		// if the solvent atoms should not act as sources 
		if (exclude_solvent_field_)
		{
			// we build a new effector list
			list<Atom*>				tmp_effector_list;
			
			list<Atom*>::const_iterator effector_it = effector_list_.begin();
			for (; effector_it != effector_list_.end(); ++effector_it)
			{			
				if ((*effector_it)->getResidue()->getName() != "HOH")
				{
					tmp_effector_list.push_back(*effector_it);
				}
			}
			effector_list_  = 	tmp_effector_list;
		}

		printEffectors_();	
		printTargets_();

		// iterate over all bonds
		std::vector<std::pair<Atom*, Atom*> >::iterator bond_it = bond_list_.begin();
		Index current_bond=0;
		for (; bond_it != bond_list_.end(); ++bond_it)
		{
			Atom* first_atom = bond_it->first;
			Atom* second_atom = bond_it->second;
			//std::cout <<  first_atom->getFullName() << "  " << second_atom->getFullName()<< std::endl;

			// We found parameters for a bond -- 
			// calculate the electric field and the induced secondary shift.

			Vector3 first_atom_pos = first_atom->getPosition();
			Vector3 second_atom_pos = second_atom->getPosition();
			Vector3 bond_vector(first_atom_pos - second_atom_pos);

			// the electric field 
			Vector3 E(0.0);

			bool same_residue;
			bool adjacent_residues;

			list<Atom*>::const_iterator effector_it = effector_list_.begin();
			list<Atom*>::const_iterator next_effector, last_effector;
			for (; effector_it != effector_list_.end(); ++effector_it)
			{

				// Exclude this effector--target combination from consideration if
				// effector is a cabonyl oxygen (O) and the target is a amid hydrogen (HN)
				// and carbonyl_influences_amide_field is set (read from options in init()).
				if (	 !carbonyl_influences_amide_field_ && ((*effector_it)->getName() == "O") 
						&& ( (first_atom->getName() == "H") || second_atom->getName() == "H" ) 
					 )
				{
					//std::cout << first_atom->getName()<< "-->" << second_atom->getName() << "by" << (*effector_it)->getName() << std::endl; 
					continue;
				}


				//  Exclude effectors from adjacent residue (fragment) if 
				//  exclude_adjacent_residue_field is set (read from options in init()). 
				//  and 
				// 	Exclude effectors from the same residue (fragment) if 
				//  exclude_residue_field is set (read from options in init()).

				//  first test whether we have atoms from same residue 
				same_residue = ((*effector_it)->getFragment() == first_atom->getFragment());

				//  then test whether we have atoms in adjacent residues 
				adjacent_residues = false;


				adjacent_residues = (   (*effector_it)->getFragment()->isNextSiblingOf(*(first_atom->getFragment()))
															||(*effector_it)->getFragment()->isPreviousSiblingOf(*(first_atom->getFragment()))
															||(abs((*effector_it)->getResidue()->getID().toInt() - first_atom->getResidue()->getID().toInt()) <= 1));
				// Exclude effectors if flags are set exclude criterion holds   
				if (    (!exclude_residue_field_					 ||  !same_residue) 
						&& (!exclude_adjacent_residue_field_  ||  !adjacent_residues) )
				{
					//std::cout << (*effector_it)->getName()<< "***>" <<first_atom->getName()<< std::endl;

					Vector3 distance(first_atom_pos - (*effector_it)->getPosition());
					float square_distance = distance.getSquareLength();
					if (square_distance <= cut_off2_)
					{
//std::cout << first_atom->getFullName() << " " << second_atom->getFullName() << " " << (*effector_it)->getFullName() << " ";
//std::cout << square_distance << std::endl;
						// translate the charge to ESU (from elementary charges)
						float charge = (*effector_it)->getCharge() * 1./charge_factor_; // 4.8;

						// add to the field
						E += distance * charge / (square_distance * distance.getLength());
std::cout << first_atom->getResidue()->getID().toInt() - 4 << " " <<first_atom->getName() << " " << second_atom->getName() << "  " << (*effector_it)->getResidue()->getID().toInt() - 4 << "  " << (*effector_it)->getName() << "  " << charge<< std::endl;
std::cout << first_atom->getPosition()<< " " << second_atom->getPosition() << " " << (*effector_it)->getPosition()  << std::endl;
						
					}
				}
			}

			// Calculate the field component E_z along the bond axis
			float Ez = (bond_vector * E) / bond_vector.getLength();

			// calculate the secondary shift induced by this field
			float delta_EF = 		epsilon1_[expression_number_[current_bond]] * Ez 
												+ epsilon2_[expression_number_[current_bond]] * E.getSquareLength();

			// store the shift in the corresponding properties
			float shift = first_atom->getProperty(ShiftModule::PROPERTY__SHIFT).getFloat();
			shift += delta_EF;
			first_atom->setProperty(ShiftModule::PROPERTY__SHIFT, shift);

			first_atom->setProperty(PROPERTY__EF_SHIFT, delta_EF);
			current_bond++;
		}

		return true;
	}
		
	Processor::Result EFShiftProcessor::operator () (Composite& object)
		
	{
		// Here, we collect all bonds
		// and all charged atoms (as effectors of the electric field)
		if (RTTI::isKindOf<Atom>(object))
		{
			Atom* atom_ptr = RTTI::castTo<Atom>(object);
			
	
			// Assign the charge (if it is defined for this atom).
			String full_name = atom_ptr->getFullName();
			full_name.substitute(":", " ");
			if (charge_map_.has(full_name))
			{
//std::cout << full_name << "  " <<  charge_map_[full_name] / charge_factor_ << std::endl;
				atom_ptr->setCharge(charge_map_[full_name]);
			}
			else
			{
				// Try wildcard match for the residue name.
				full_name = "* " + atom_ptr->getName();
				if (charge_map_.has(full_name))
				{
//std::cout << full_name << "  " <<  charge_map_[full_name] / charge_factor_ << std::endl;
					atom_ptr->setCharge(charge_map_[full_name]);
				}
			}

			// Store all charged atoms in the effector list.
			if (atom_ptr->getCharge() != 0.0)
			{
				effector_list_.push_back(atom_ptr);
			}


			std::cout << "Hallo ich bins..." << std::endl;
			Atom::BondIterator bond_it = atom_ptr->beginBond();

			for (; +bond_it; ++bond_it)
			{
				Atom*	first_atom = 0;
				Atom* second_atom = 0;

				bool match_found = false;
				Index j = -1;

				// Iterate over all expressions and try to match them
				// with the bond's atoms.
				for (Position i = 0; i < first_atom_expressions_.size(); ++i)
				{
					// First, try to match first/fist and second/second.
					if (  (first_atom_expressions_[i](*(bond_it->getFirstAtom())))
							&&(second_atom_expressions_[i](*(bond_it->getSecondAtom())))
						 )

					{
						// remember the atoms and the bond type (for the parameters)
						first_atom = const_cast<Atom*>(bond_it->getFirstAtom());
						second_atom = const_cast<Atom*>(bond_it->getSecondAtom());
						match_found=true;
						j = i;
						break;
					}
					// Otherwise: try first/second and second/first
					else if (first_atom_expressions_[i](*(bond_it->getSecondAtom()))
							&& second_atom_expressions_[i](*(bond_it->getFirstAtom())))
					{
						// remember the atoms and the bond type (for the parameters)
						first_atom  = const_cast<Atom*>(bond_it->getSecondAtom());
						second_atom = const_cast<Atom*>(bond_it->getFirstAtom());
						match_found=true;
						j = i;
						break;
					}
				}

				if (match_found)
				{
					// only include each bond once
					if (find(bond_list_.begin(), bond_list_.end(), pair<Atom*, Atom*>(first_atom, second_atom))==bond_list_.end())
					{
						bond_list_.push_back(std::pair<Atom*, Atom*>(first_atom, second_atom));
						expression_number_.push_back(j);
					}
				}
			}	
		}
		
		return Processor::CONTINUE;
	}



	void EFShiftProcessor::printTargets_()
		throw()
	{
		std::cout << "********* \n EF:Liste der Target Bonds" << std::endl;
		std::vector<std::pair<Atom*, Atom*> >::iterator tbond_it = bond_list_.begin();
		for (; tbond_it != bond_list_.end(); ++tbond_it)
		{			
			std::cout << tbond_it->first->getFullName() << "  " << tbond_it->second->getFullName() << std::endl; 
		}
		std::cout << "  " << bond_list_.size() << "\n-------------\n";
	}

	void EFShiftProcessor::printEffectors_()
		throw()
	{
		std::cout << "********* \n EF:Liste der Effektoren" << std::endl;
		list<Atom*>::const_iterator effector_it = effector_list_.begin();
		for (; effector_it != effector_list_.end(); ++effector_it)
		{			
			std::cout << (*effector_it)->getFullName() <<"  " << (*effector_it)->getName()  << "  "  << std::endl; 
							//	<< (*effector_it)->getType() << "  " << std::endl; 
								//(*effector_it)->getTypeName()<< std::endl; 
		}
		std::cout << "  " << effector_list_.size() << "\n-------------\n";

	}

	void	EFShiftProcessor::printParameters_()
		throw()
	{
		std::cout << "********* \n EF:Liste der Parameter" << std::endl;
		std::cout << "exclude_residue_field  " <<  exclude_residue_field_ << std::endl;
		std::cout << "exclude_adjacent_residue_field  "	<< exclude_adjacent_residue_field_ << std::endl;
		std::cout << "carbonyl_influences_amide_field  "	<< carbonyl_influences_amide_field_ << std::endl;
		std::cout << "exclude_solvent_field  " << exclude_solvent_field_ << std::endl;
	  std::cout << 	"cut_off" << cut_off2_ << std::endl;
		std::cout << 	"unit" << (charge_factor_ > 0.9 ? "e0" :"ESU")<< std::endl;
	}


} // namespace BALL
