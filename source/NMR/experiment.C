// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/NMR/experiment.h>

namespace BALL
{
	SimpleExperiment1D::SimpleExperiment1D()
		:	Experiment1D(),
			expression_("true()")
	{
	}

	SimpleExperiment1D::SimpleExperiment1D(const SimpleExperiment1D& experiment)
		:	Experiment1D(experiment),
			expression_(experiment.expression_)
	{
	}

	SimpleExperiment1D::~SimpleExperiment1D()
	{
	}

	bool SimpleExperiment1D::start()
	{
		// delete all old peaks
		peak_list_.clear();

		// nothing can go wrong here!
		return true;
	}

	Processor::Result SimpleExperiment1D::operator () (Composite& composite)
	{
		Atom* atom_ptr = dynamic_cast<Atom*>(&composite);
		if ((atom_ptr != 0) && expression_(*atom_ptr))
		{
			// create a new peak at the end of the list
			peak_list_.push_back(default_peak_);

			// set the peak's position
			peak_list_.back().setPosition(atom_ptr->getProperty(ShiftModule::PROPERTY__SHIFT).getFloat());

			// copy the atom's properties to the peak
			static_cast<PropertyManager&>(peak_list_.back()).set(*atom_ptr);
		}
			
		return Processor::CONTINUE;
	}
} // namespace BALL
