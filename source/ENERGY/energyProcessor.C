// $Id: energyProcessor.C,v 1.2 2000/10/05 17:20:28 anker Exp $

#include <BALL/ENERGY/energyProcessor.h>

namespace BALL
{

	EnergyProcessor::EnergyProcessor() throw()
		: UnaryProcessor<AtomContainer>(),
			fragment_(0),
			energy_(0),
			valid_(false)
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
		// BAUSTELLE
		//UnaryProcessor<AtomContainer>::clear();
		fragment_ = 0;
		energy_ = 0;
	}

	bool EnergyProcessor::start() throw()
	{
		fragment_ = 0;
		energy_ = 0;
		return 1;
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
		return ((fragment_ == proc.fragment_) && (energy_ == proc.energy_)
			&& (valid_ == proc.valid_));
	}


} // namespace BALL
