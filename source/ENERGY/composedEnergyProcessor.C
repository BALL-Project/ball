// $Id: composedEnergyProcessor.C,v 1.1 2000/09/05 14:29:16 oliver Exp $

#include <BALL/ENERGY/composedEnergyProcessor.h>

namespace BALL
{

	ComposedEnergyProcessor::ComposedEnergyProcessor()
		:	EnergyProcessor(),
			components_()
	{
	}


	ComposedEnergyProcessor::ComposedEnergyProcessor
		(const ComposedEnergyProcessor& proc)
		:	EnergyProcessor(proc),
			components_(proc.components_)
	{
	}


	ComposedEnergyProcessor::ComposedEnergyProcessor(
			EnergyProcessorList proc_list)
		: components_(proc_list)
	{
	}


	ComposedEnergyProcessor::~ComposedEnergyProcessor()
	{
		destroy();
	}

	
	void ComposedEnergyProcessor::destroy()
	{
		clear();
	}


	void ComposedEnergyProcessor::clear()
	{
		EnergyProcessor::clear();
		components_.clear();
	}


	void ComposedEnergyProcessor::set(const ComposedEnergyProcessor& proc)
	{
		EnergyProcessor::set(proc);
		components_ = proc.components_;
	}


	const ComposedEnergyProcessor& ComposedEnergyProcessor::operator =
		(const ComposedEnergyProcessor& proc)
	{
		set(proc);
		return *this;
	}


	bool ComposedEnergyProcessor::finish()
	{
		EnergyProcessorList::Iterator list_it = components_.begin();
		for (; list_it != components_.end(); ++list_it)
		{
			((AtomContainer*) fragment_)->apply(**list_it);
			energy_ += (*list_it)->getEnergy();
		}
		return 1;
	}

	void ComposedEnergyProcessor::addComponent(EnergyProcessor* proc)
	{
		components_.push_back(proc);
	}

	void ComposedEnergyProcessor::removeComponent(EnergyProcessor* proc)
	{
		components_.remove(proc);
	}

} // namespace BALL
