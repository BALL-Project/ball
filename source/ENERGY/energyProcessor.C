// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: energyProcessor.C,v 1.6 2002/02/27 12:21:14 sturm Exp $

#include <BALL/ENERGY/energyProcessor.h>

namespace BALL
{

	EnergyProcessor::EnergyProcessor() throw()
		: UnaryProcessor<AtomContainer>(),
			fragment_(0),
			energy_(0),
			valid_(true)
	{
	}

	EnergyProcessor::EnergyProcessor(const EnergyProcessor& proc) throw()
		: UnaryProcessor<AtomContainer>(),
			fragment_(proc.fragment_),
			energy_(proc.energy_),
			valid_(proc.valid_)
	{
	}

	EnergyProcessor::~EnergyProcessor() throw()
	{
		clear();
		valid_ = false;
	}

	void EnergyProcessor::clear() throw()
	{
		fragment_ = 0;
		energy_ = 0;
	}

	bool EnergyProcessor::start() throw()
	{
		fragment_ = 0;
		energy_ = 0;
		return true;
	}

	const EnergyProcessor& EnergyProcessor::operator =
		(const EnergyProcessor& proc) throw()
	{
		fragment_ = proc.fragment_;
		energy_ = proc.energy_;

		return *this;
	}

	Processor::Result EnergyProcessor::operator () (AtomContainer& fragment) throw()
	{
		if (fragment_ == 0)
		{
			fragment_ = (const AtomContainer*) &fragment;
		}
		return Processor::BREAK;
	}

	double EnergyProcessor::getEnergy() const throw()
	{
		return energy_;
	}
	
	bool EnergyProcessor::isValid() const throw()
	{
		return valid_;
	}

	bool EnergyProcessor::operator == (const EnergyProcessor& proc) const throw()
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
