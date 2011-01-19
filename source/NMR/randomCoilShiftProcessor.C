// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include<BALL/NMR/randomCoilShiftProcessor.h>
#include<BALL/FORMAT/parameterSection.h>

namespace BALL
{

	const char* RandomCoilShiftProcessor::PROPERTY__RANDOM_COIL_SHIFT = "RandomCoilShift";

	RandomCoilShiftProcessor::RandomCoilShiftProcessor()
	{
		system_ = NULL; 
	}

	RandomCoilShiftProcessor::RandomCoilShiftProcessor(const RandomCoilShiftProcessor& processor)
		:	ShiftModule(processor),
			shift_map_(processor.shift_map_)
	{	
		system_ = NULL;
	}

	RandomCoilShiftProcessor::~RandomCoilShiftProcessor()
	{
	}

	void RandomCoilShiftProcessor::init()
	{
		// We assume the worst case.
		valid_ = false;
		system_ = NULL;
		
		// Make sure we have correct parameters.
		if (parameters_ == 0)
		{
			return;
		}

		// Extract the data from the correct section.
		ParameterSection parameter_section;
		parameter_section.extractSection(*parameters_, "RandomCoilShifts");

		// Make sure the section contains the required columns.
		if (!parameter_section.hasVariable("shift"))
		{
			return;
		}

		// Extract the random coil shifts from the parameter section.
		Position shift_column = parameter_section.getColumnIndex("shift");
		for (Position i = 0; i < parameter_section.getNumberOfKeys(); i++)
		{
			shift_map_.insert(parameter_section.getKey(i), 
												parameter_section.getValue(i, shift_column).toFloat());
		}
				
		// Initialization complete - mark the module as valid.
		valid_ = true;

		return;
	}

	Processor::Result RandomCoilShiftProcessor::operator () (Composite& composite)	
	{
		// We are just interested in atoms.
		Atom* atom_ptr = dynamic_cast<Atom*>(&composite);
		if (atom_ptr == 0)
		{
			return Processor::CONTINUE;
		}

		// Try to get the system.
		if (!system_   && (RTTI::isKindOf<System>(atom_ptr->getRoot())))
		{	
			system_ = dynamic_cast<System*>(&(atom_ptr->getRoot()));
		}

		// Get the atoms full name. 
		String full_name = atom_ptr->getFullName();
		full_name.substitute(":", " ");
		if (!shift_map_.has(full_name))
		{
			full_name = atom_ptr->getFullName(Atom::NO_VARIANT_EXTENSIONS);	
			full_name.substitute(":", " ");
			if (!shift_map_.has(full_name))
			{
				full_name = "* " + atom_ptr->getName();
				if (!shift_map_.has(full_name))
				{
					full_name = "";
				}
			}
		}

		if (full_name != "")
		{
			// Retrieve the random coil shift from the hash map.
			const float& delta_RC = shift_map_[full_name];

			// Add the random coil shift to the total shift value.
			float delta = atom_ptr->getProperty(ShiftModule::PROPERTY__SHIFT).getFloat();
			delta += delta_RC;
			atom_ptr->setProperty(ShiftModule::PROPERTY__SHIFT, delta);
			
			// Store the random coil shift as the random coil shift property.
			atom_ptr->setProperty(RandomCoilShiftProcessor::PROPERTY__RANDOM_COIL_SHIFT, delta_RC);
		}
		
		return Processor::CONTINUE;
	}
	

	void RandomCoilShiftProcessor::postprocessing_()
	{
		if (system_) 
		{
			// Add for all CA 0.2 times the values of HA ramdon coil shift contribution.
			for (BALL::ResidueIterator r_it = system_->beginResidue(); 
					 r_it != system_->endResidue(); 
					 ++r_it)
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
					float ca_shift = CA->getProperty(BALL::RandomCoilShiftProcessor::PROPERTY__RANDOM_COIL_SHIFT).getFloat();
					float ha_shift = HA->getProperty(BALL::RandomCoilShiftProcessor::PROPERTY__RANDOM_COIL_SHIFT).getFloat();
					
					CA->setProperty(BALL::RandomCoilShiftProcessor::PROPERTY__RANDOM_COIL_SHIFT, ca_shift + 0.2*ha_shift);
					CA->setProperty(ShiftModule::PROPERTY__SHIFT, total+ 0.2*ha_shift );
				}
			}
		}
		else
		{
				Log.error() << "Error in RandomCoilShiftProcessor: no system found for postprocessing. (" 
				 					<< __FILE__ << " " << __LINE__ << ")" <<  std::endl;
		}
	}

}//namespace BALL
