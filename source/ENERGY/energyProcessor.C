// $Id: energyProcessor.C,v 1.1 2000/09/05 14:29:17 oliver Exp $

#include <BALL/ENERGY/energyProcessor.h>

namespace BALL
{

	EnergyProcessor::EnergyProcessor()
		: UnaryProcessor<AtomContainer>(),
			fragment_(0),
			energy_(0)
	{
	}


	EnergyProcessor::EnergyProcessor(const EnergyProcessor& proc)
		: UnaryProcessor<AtomContainer>(),
			fragment_(proc.fragment_),
			energy_(proc.energy_)
	{
	}


	EnergyProcessor::~EnergyProcessor()
	{
		destroy();
	}


	void EnergyProcessor::destroy()
	{
		clear();
	}


	void EnergyProcessor::clear()
	{
		// BAUSTELLE
		//UnaryProcessor<AtomContainer>::clear();
		fragment_ = 0;
		energy_ = 0;
	}

	bool EnergyProcessor::start()
	{
		fragment_ = 0;
		energy_ = 0;
		return 1;
	}


	void EnergyProcessor::set(const EnergyProcessor& proc)
	{
		fragment_ = proc.fragment_;
		energy_ = proc.energy_;
	}


	const EnergyProcessor& EnergyProcessor::operator =
		(const EnergyProcessor& proc)
	{
		set(proc);
		return *this;
	}


	Processor::Result EnergyProcessor::operator () (AtomContainer& fragment)
	{
		if (fragment_ == 0)
		{
			fragment_ = (const AtomContainer*) &fragment;
		}
		// BAUSTELLE: Sollte hier ein CONTINUE hin?
		return Processor::BREAK;
	}


	double EnergyProcessor::getEnergy() const
	{
		return energy_;
	}

} // namespace BALL
