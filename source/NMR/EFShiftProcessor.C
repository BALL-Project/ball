// $Id: EFShiftProcessor.C,v 1.2 2000/09/19 19:20:28 oliver Exp $

#include<BALL/NMR/EFShiftProcessor.h>

#ifndef BALL_COMMON_PATH_H
# include <BALL/SYSTEM/path.h>
#endif

#ifndef BALL_KERNEL_BOND_H
# include <BALL/KERNEL/bond.h>
#endif

using namespace std;

namespace BALL 
{

	EFShiftProcessor::EFShiftProcessor()
		throw()
		:	ShiftModule()
	{	
	}

	EFShiftProcessor::EFShiftProcessor(const EFShiftProcessor& processor)
		throw()
		:	ShiftModule(processor),
			proton_list_(processor.proton_list_),
			effector_list_(processor.effector_list_),
			atom_list_(processor.atom_list_),
			first_atom_expressions_(processor.first_atom_expressions_),
			second_atom_expressions_(processor.second_atom_expressions_),
			parameter_section_(processor.parameter_section_)
	{
	}
	
	EFShiftProcessor::~EFShiftProcessor()
		throw()
	{
	}

	bool EFShiftProcessor::start()
		throw()
	{
		// if no parameters are assigned, abort immediately
		if (parameters_ == 0)
		{
			return false;
		}

		// building the Expression List for the shiftatoms		
		parameter_section_.extractSection(*parameters_, "EFShift-Bonds");

		// clear the arrays containing the expressions
		first_atom_expressions_.clear();
		second_atom_expressions_.clear();
		
		if (!parameter_section_.hasVariable("first_atom") || !parameter_section_.hasVariable("second_atom"))
		{
			return false;
		}

		Position first_atom_column = parameter_section_.getColumnIndex("first_atom");
		Position second_atom_column = parameter_section_.getColumnIndex("second_atom");
		for (Position counter = 0; counter < parameter_section_.getNumberOfKeys(); counter++)
		{
			first_atom_expressions_.push_back(Expression(parameter_section_.getValue(counter, first_atom_column)));
			second_atom_expressions_.push_back(Expression(parameter_section_.getValue(counter, second_atom_column)));
		}
				
		return true;
	}
		
	bool EFShiftProcessor::finish()
		throw()
	{
		// abort if the parameters_ were not set (using ShiftModule::setParameters)
		if (parameters_ == 0)
		{
			return false;
		}

		parameter_section_.extractSection(*parameters_, "EFShift-Bonds");

		if (atom_list_.size() <= 0 || effector_list_.size() <= 0)
		{
			return true;
		}

		list<Atom*>::iterator atom_iter;
		list<Atom*>::iterator effector_iter;
		atom_iter = atom_list_.begin();
		
		Position key = 0;

		for (; atom_iter != atom_list_.end(); ++atom_iter)
		{
			for (Position counter = 0; counter < first_atom_expressions_.size(); counter++)
			{
				if (first_atom_expressions_[counter](**atom_iter))
				{
					// exit the loop if we found a matching expression
					key = counter;
					break;
				}
			}
			// was passiert wenn keine expression gefunden wurde???

			const Vector3 proton_pos = (*atom_iter)->getPosition();

			Vector3 second_atom_pos;
			for (Position counter = 0; counter < (*atom_iter)->countBonds(); counter++)
			{
				Bond* bond = (*atom_iter)->getBond(counter);
				if (second_atom_expressions_[key](*bond->getBoundAtom(*(*atom_iter)))) 
				{
					second_atom_pos = (*atom_iter)->getBond(counter)->getBoundAtom(*(*atom_iter))->getPosition();
					break;
				}
			}
			// hier kann was nicht stimmen, die letzte zuweisung von second_atom_pos wuerde ueberschrieben
			second_atom_pos = (*atom_iter)->getBond(0)->getBoundAtom(*(*atom_iter))->getPosition();

			const Vector3 distance_proton_second = proton_pos - second_atom_pos;
			float Ez = 0;

			for (effector_iter = effector_list_.begin(); 
					 effector_iter != effector_list_.end();
					 ++effector_iter)
			{
				if ( (*effector_iter)->getFragment() != (*atom_iter)->getFragment() )
				{
					Vector3 atom_pos = (*effector_iter)->getPosition();
					Vector3 distance_proton_effector = proton_pos - atom_pos;			

					float dps_scalar = distance_proton_second.getLength();
					float dpe_scalar = distance_proton_effector.getLength();

					float charge = (*effector_iter)->getCharge();
					float Efact = charge / (dpe_scalar * dpe_scalar);

					float sc = distance_proton_second * distance_proton_effector;
					float theta = sc / (dps_scalar * dps_scalar);

					Ez += theta * Efact;
				}
			}
			float epsilon1 = parameter_section_.getValue(key, "epsilon1").toFloat();
			double shift = (*atom_iter)->getProperty(ShiftModule::PROPERTY__SHIFT).getFloat();
			shift = epsilon1 * Ez;

			(*atom_iter)->setProperty(ShiftModule::PROPERTY__SHIFT, shift);
			(*atom_iter)->setProperty("ElectricFieldShift", -Ez * epsilon1);
		}

		return true;
	}
		
	Processor::Result EFShiftProcessor::operator () (Composite& object)
		throw()
	{
		// Here, we collect all parameterized atoms
		// and all charged atoms (as effectors of the electric field)
		if (RTTI::isKindOf<Atom>(object))
		{
			Atom* atom_ptr = RTTI::castTo<Atom>(object);
		
			// if the atom matches any of our expressions, save it in the
			// atom list (containing those atom whose shift is to be calculated)
			for (Position counter = 0; counter < first_atom_expressions_.size(); counter++)
			{
				if (first_atom_expressions_[counter](*atom_ptr))
				{
					atom_list_.push_back(atom_ptr);
					break;
				}
			}
				
			// store all charged atoms in the effector list
			if (atom_ptr->getCharge() != 0.0)
			{
				effector_list_.push_back(atom_ptr);
			}
		}
		
		return Processor::CONTINUE;
	}

} // namespace BALL
