// $Id: composedEnergyProcessor.C,v 1.3 2001/07/15 22:57:39 amoll Exp $

#include <BALL/ENERGY/composedEnergyProcessor.h>

namespace BALL
{

	ComposedEnergyProcessor::ComposedEnergyProcessor() throw()
		:	EnergyProcessor(),
			components_()
	{
	}


	ComposedEnergyProcessor::ComposedEnergyProcessor
		(const ComposedEnergyProcessor& proc) throw()
		:	EnergyProcessor(proc),
			components_(proc.components_)
	{
	}


	ComposedEnergyProcessor::ComposedEnergyProcessor(
			EnergyProcessorList proc_list) throw()
		: EnergyProcessor(),
			components_(proc_list)
	{
		checkValidity();
	}


	ComposedEnergyProcessor::~ComposedEnergyProcessor() throw()
	{
		clear();

		valid_ = false;
	}


	void ComposedEnergyProcessor::clear() throw()
	{
		EnergyProcessor::clear();
		components_.clear();
	}


	const ComposedEnergyProcessor& ComposedEnergyProcessor::operator =
		(const ComposedEnergyProcessor& proc) throw()
	{
		EnergyProcessor::operator = (proc);
		components_ = proc.components_;
		valid_ = proc.valid_;

		return *this;
	}


	bool ComposedEnergyProcessor::finish() throw()
	{
		EnergyProcessorList::Iterator list_it = components_.begin();
		for (; list_it != components_.end(); ++list_it)
		{
			((AtomContainer*) fragment_)->apply(**list_it);
			energy_ += (*list_it)->getEnergy();
		}
		return true;
	}


	void ComposedEnergyProcessor::addComponent(EnergyProcessor* proc) throw()
	{
		// if there was no processor in the list before, assume that the
		// instance will be valid after insertion.

		if (components_.size() == 0)
		{
			valid_ = true;
		}

		components_.push_back(proc);

		// if the processor is invalid, invalidate the whole instance
		if (!proc->isValid())
		{
			valid_ = false;
		}
	}


	void ComposedEnergyProcessor::removeComponent(EnergyProcessor* proc) throw()
	{
		components_.remove(proc);
		checkValidity();
	}

	void ComposedEnergyProcessor::checkValidity() throw()
	{
		valid_ = true;
		EnergyProcessorList::Iterator it = components_.begin();
		for (; it != components_.end(); ++it)
		{
			if (!(*it)->isValid())
			{
				valid_ =false;
			}
		}
	}

} // namespace BALL
