// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: energyProcessor.C,v 1.6 2002/02/27 12:21:14 sturm Exp $

#include <BALL/ENERGY/energyProcessor.h>

namespace BALL
{

	EnergyProcessor::EnergyProcessor()
		: UnaryProcessor<AtomContainer>(),
			fragment_(0),
			energy_(0),
			valid_(true)
	{
	}

	EnergyProcessor::EnergyProcessor(const EnergyProcessor& proc)
		: UnaryProcessor<AtomContainer>(),
			fragment_(proc.fragment_),
			energy_(proc.energy_),
			valid_(proc.valid_)
	{
	}

	EnergyProcessor::~EnergyProcessor()
	{
		clear();
		valid_ = false;
	}

	void EnergyProcessor::clear()
	{
		fragment_ = 0;
		energy_ = 0;
	}

	bool EnergyProcessor::start()
	{
		fragment_ = 0;
		energy_ = 0;
		return true;
	}

	const EnergyProcessor& EnergyProcessor::operator =
		(const EnergyProcessor& proc)
	{
		fragment_ = proc.fragment_;
		energy_ = proc.energy_;

		return *this;
	}

	Processor::Result EnergyProcessor::operator () (AtomContainer& fragment)
	{
		if (fragment_ == 0)
		{
			fragment_ = (const AtomContainer*) &fragment;
		}
		return Processor::BREAK;
	}

	double EnergyProcessor::getEnergy() const
	{
		return energy_;
	}
	
	bool EnergyProcessor::isValid() const
	{
		return valid_;
	}

	bool EnergyProcessor::operator == (const EnergyProcessor& proc) const
	{
		bool result;
		if ((fragment_ == 0) && (proc.fragment_ == 0))
		{
			result = ((energy_ == proc.energy_) && (valid_ == proc.valid_));
		}
		else
		{
			if ((fragment_ == 0) || (proc.fragment_ == 0))
			{
				result = false;
			}
			else
			{
				result = ((*fragment_ == *proc.fragment_) 
						&& (energy_ 	 == proc.energy_)
						&& (valid_ 	 == proc.valid_));
			}
		}
		return result;
	}


} // namespace BALL
