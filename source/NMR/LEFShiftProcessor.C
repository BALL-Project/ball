// $Id: LEFShiftProcessor.C,v 1.1 2000/09/19 11:38:50 amoll Exp $

#include<BALL/NMR/LEF.h>

#ifndef BALL_COMMON_PATH_H
# include <BALL/SYSTEM/path.h>
#endif

#ifndef BALL_KERNEL_BOND_H
# include <BALL/KERNEL/bond.h>
#endif

using namespace std;

namespace BALL 
{

	LEFShiftProcessor::LEFShiftProcessor()
		throw()
	{	
		Path path;
		ini_filename_ = path.find("NMR/nmr.ini").c_str();
	}
		
	LEFShiftProcessor::~LEFShiftProcessor()
		throw()
	{
	}

	void LEFShiftProcessor::setFilename(const String& filename)
		throw()
	{
		ini_filename_ = filename;
	}
		
	const String& LEFShiftProcessor::getFilename() const
		throw()
	{
		return ini_filename_;
	}
		
	bool LEFShiftProcessor::start()
		throw()
	{
		// building the Expression List for the shiftatoms		
		parameters_.setFilename(ini_filename_);
		parameter_section_.extractSection(parameters_, "LEF-ShiftAtoms");

		// clear the arrays containing the expressions
		first_atom_expressions_.clear();
		second_atom_expressions_.clear();
		
		const Position description_column = parameter_section_.getColumnIndex("description");

		for (Position counter = 0; counter < parameter_section_.getNumberOfKeys(); counter++)
		{
			String description = parameter_section_.getValue(counter, description_column);

			while (description.has('_'))
			{
				description.substitute("_"," "); // warum wird hier nicht durch "" ersetzt ???
			}
			first_atom_expressions_.push_back(Expression(description));
		}	
		
		// build the Expression for the binding-atoms of the shiftAtoms
		const Position second_atom_column = parameter_section_.getColumnIndex("second_atom");

		for (Position counter = 0; counter < parameter_section_.getNumberOfKeys(); counter++)
		{
			String description = parameter_section_.getValue(counter, second_atom_column);

			while (description.has('_'))
			{
				description.substitute("_"," ");
			}
			second_atom_expressions_.push_back(Expression(description));
		}
				
		return true;
	}
		
	bool LEFShiftProcessor::finish()
		throw()
	{
		parameter_section_.extractSection(parameters_, "LEF-ShiftAtoms");

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
					const Vector3 atom_pos = (*effector_iter)->getPosition();
					const Vector3 distance_proton_effector = proton_pos - atom_pos;			

					const float dps_scalar = distance_proton_second.getLength();
					const float dpe_scalar = distance_proton_effector.getLength();

					const float charge = (*effector_iter)->getCharge();
					const float Efact = charge / (dpe_scalar * dpe_scalar);

					const float sc = distance_proton_second * distance_proton_effector;
					const float theta = sc / (dps_scalar * dps_scalar);

					Ez += theta * Efact;
				}
			}
			const float sigmaE = parameter_section_.getValue(key, "sigmaE").toFloat();
			double shift = (*atom_iter)->getProperty("chemical_shift").getFloat();
			shift -= -Ez * sigmaE;

			(*atom_iter)->setProperty("chemical_shift", shift);
			(*atom_iter)->setProperty("LEF", -Ez * sigmaE);
		}

		return true;
	}
		
	Processor::Result LEFShiftProcessor::operator () (Composite& object)
		throw()
	{
		// if object is an H-atom connected to a C-atom it is saved in _proton_list
		// ???
		if (!RTTI::isKindOf<PDBAtom>(object))
		{
			return Processor::CONTINUE;
		}

		Atom* atom_ptr = RTTI::castTo<PDBAtom>(object);
			
		for (Position counter = 0; counter < first_atom_expressions_.size(); counter++)
		{
			if (first_atom_expressions_[counter](*atom_ptr))
			{
				atom_list_.push_back(atom_ptr);
				break;
			}
		}
			
		if (atom_ptr->getCharge() != 0.0)
		{
			effector_list_.push_back(atom_ptr);
		}
		
		return Processor::CONTINUE;
	}

} // namespace BALL
