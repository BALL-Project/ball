// $Id: EFShiftProcessor.C,v 1.8 2000/09/21 22:46:24 amoll Exp $

#include<BALL/NMR/EFShiftProcessor.h>
#include <BALL/COMMON/limits.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/FORMAT/parameterSection.h>

namespace BALL 
{
	
	const char* EFShiftProcessor::PROPERTY__EF_SHIFT = "ElectricFieldShift";

	EFShiftProcessor::EFShiftProcessor()
		throw()
		:	ShiftModule()
	{	
	}

	EFShiftProcessor::EFShiftProcessor(const EFShiftProcessor& processor)
		throw()
		:	ShiftModule(processor),
			bond_list_(processor.bond_list_),
			effector_list_(processor.effector_list_),
			first_atom_expressions_(processor.first_atom_expressions_),
			second_atom_expressions_(processor.second_atom_expressions_),
			epsilon1_(processor.epsilon1_),
			epsilon2_(processor.epsilon2_),
			charge_map_(processor.charge_map_),
			exclude_residue_field_(processor.exclude_residue_field_),
			cut_off2_(processor.cut_off2_)
	{
	}
	
	EFShiftProcessor::~EFShiftProcessor()
		throw()
	{
	}

	void EFShiftProcessor::init()
		throw()
	{
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
		float charge_factor = 1.0;
		if (parameter_section.options.has("unit"))
		{
			String unit = parameter_section.options["unit"];
			if (unit == "e0")
			{
				charge_factor = 1.0;
			}
			else if (unit == "ESU")
			{
				charge_factor = 1.0 / 4.8;
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
				charge_map_[parameter_section.getKey(i)] = charge_factor * parameter_section.getValue(i, charge_column).toFloat();
			}
		}

		// mark the module as initialized
		valid_ = true;
	}
		
	bool EFShiftProcessor::start()
		throw()
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
		throw()
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

		// iterate over all bonds
		list<Bond*>::iterator bond_it = bond_list_.begin();
		for (; bond_it != bond_list_.end(); ++bond_it)
		{
			Position bond_type = INVALID_POSITION;
			Atom*	first_atom;
			Atom* second_atom;

			// Iterate over all expressions and try to match them
			// with the bond's atoms.
			for (Position i = 0; i < first_atom_expressions_.size(); ++i)
			{
				// First, try to match first/fist and second/second.
				if (first_atom_expressions_[i](*(*bond_it)->getFirstAtom())
						&& second_atom_expressions_[i](*(*bond_it)->getSecondAtom()))
				{
					// remember the atoms and the bond type (for the parameters)
					first_atom = (*bond_it)->getFirstAtom();
					second_atom = (*bond_it)->getSecondAtom();
					bond_type = i;
					break;
				}
				// Otherwise: try first/second and second/first
				else if (first_atom_expressions_[i](*(*bond_it)->getSecondAtom())
						 && second_atom_expressions_[i](*(*bond_it)->getFirstAtom()))
				{
					// remember the atoms and the bond type (for the parameters)
					first_atom  = (*bond_it)->getSecondAtom();
					second_atom = (*bond_it)->getFirstAtom();
					bond_type = i;
					break;
				}
			}
			
			if (bond_type != INVALID_POSITION)
			{
				// We found parameters for a bond -- 
				// calculate the electric field and the induced secondary shift.
				
				Vector3 first_atom_pos = first_atom->getPosition();
				Vector3 second_atom_pos = second_atom->getPosition();
				Vector3 bond_vector(first_atom_pos - second_atom_pos);
				
				// the electric field 
				Vector3 E(0.0);
				
				list<Atom*>::const_iterator effector_it = effector_list_.begin();
				for (; effector_it != effector_list_.end(); ++effector_it)
				{
					// Exclude effectors from the same residue (fragment) if 
					// exclude_residue_field is set (read from options in init()).
					if (!exclude_residue_field_ || ((*effector_it)->getFragment() != first_atom->getFragment()))
					{
						Vector3 distance(first_atom_pos - (*effector_it)->getPosition());
						float square_distance = distance.getSquareLength();
						if (square_distance <= cut_off2_)
						{
							// translate the charge to ESU (from elementary charges)
							float charge = (*effector_it)->getCharge() * 4.8;
							
							// add to the field
							E += distance * charge / (distance.getSquareLength() * distance.getLength());
						}
					}
				}
					
				// Calculate the field component E_z along the bond axis
				float Ez = (bond_vector * E) / bond_vector.getLength();
				
				// calculate the secondary shift induced by this field
				float delta_EF = epsilon1_[bond_type] * Ez + epsilon2_[bond_type] * E.getSquareLength();

				// store the shift in the corresponding properties
				float shift = first_atom->getProperty(ShiftModule::PROPERTY__SHIFT).getFloat();
				shift += delta_EF;
				first_atom->setProperty(ShiftModule::PROPERTY__SHIFT, shift);

				shift = first_atom->getProperty(PROPERTY__EF_SHIFT).getFloat();
				shift += delta_EF;
				first_atom->setProperty(PROPERTY__EF_SHIFT, shift);
			}
		}

		return true;
	}
		
	Processor::Result EFShiftProcessor::operator () (Composite& object)
		throw()
	{
		// Here, we collect all bonds
		// and all charged atoms (as effectors of the electric field)
		if (RTTI::isKindOf<Atom>(object))
		{
			Atom* atom_ptr = RTTI::castTo<Atom>(object);
			
			// iterate over all bonds of the atom and collect these bonds in 
			// the bond_list_
			Atom::BondIterator bond_it = atom_ptr->beginBond();
			for (; +bond_it; ++bond_it)
			{
				if ((bond_it->getFirstAtom() == atom_ptr) && (bond_it->getSecondAtom() != 0))
				{	
					// store each bond once (isFirstAtom) and take care that
					// it is valid (i.e. it has a second atom defined as well)
					bond_list_.push_back(&*bond_it);
				}
			}
		
			// Store all charged atoms in the effector list.
			if (atom_ptr->getCharge() != 0.0)
			{
				effector_list_.push_back(atom_ptr);
			}

			// Assign the charge (if it is defined for this atom).
			String full_name = atom_ptr->getFullName();
			full_name.substitute(":", " ");
			if (charge_map_.has(full_name))
			{
				atom_ptr->setCharge(charge_map_[full_name]);
			}
			else
			{
				// Try wildcard match for the residue name.
				full_name = "* " + atom_ptr->getName();
				if (charge_map_.has(full_name))
				{
					atom_ptr->setCharge(charge_map_[full_name]);
				}
			}
		}
		
		return Processor::CONTINUE;
	}

} // namespace BALL
