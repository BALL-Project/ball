// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: composedEnergyProcessor.C,v 1.6 2002/02/27 12:21:13 sturm Exp $

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
		: EnergyProcessor(),
			components_(proc_list)
	{
		checkValidity();
	}


	ComposedEnergyProcessor::~ComposedEnergyProcessor()
	{
		clear();

		valid_ = false;
	}


	void ComposedEnergyProcessor::clear()
	{
		EnergyProcessor::clear();
		components_.clear();
	}


	const ComposedEnergyProcessor& ComposedEnergyProcessor::operator =
		(const ComposedEnergyProcessor& proc)
	{
		EnergyProcessor::operator = (proc);
		components_ = proc.components_;
		valid_ = proc.valid_;

		return *this;
	}


	bool ComposedEnergyProcessor::finish()
	{
		EnergyProcessorList::iterator list_it = components_.begin();
		for (; list_it != components_.end(); ++list_it)
		{
			((AtomContainer*) fragment_)->apply(**list_it);
			energy_ += (*list_it)->getEnergy();
		}
		return true;
	}


	void ComposedEnergyProcessor::addComponent(EnergyProcessor* proc)
	{
		// if there was no processor in the list before, assume that the
		// instance will be valid after insertion.

		if (components_.empty())
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


	void ComposedEnergyProcessor::removeComponent(EnergyProcessor* proc)
	{
		components_.remove(proc);
		checkValidity();
	}

	Size ComposedEnergyProcessor::getNumberOfEnergyProcessors() const
	{
		return (Size)components_.size();
	}
	
	void ComposedEnergyProcessor::checkValidity()
	{
		valid_ = true;
		EnergyProcessorList::iterator it = components_.begin();
		for (; it != components_.end(); ++it)
		{
			if (!(*it)->isValid())
			{
				valid_ =false;
			}
		}
	}

} // namespace BALL
