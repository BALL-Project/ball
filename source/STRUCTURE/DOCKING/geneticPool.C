// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: geneticPool.C,v 1.1.2.3 2006/08/15 14:11:46 leonhardt Exp $

#include <algorithm>

#include "geneticPool.h"
#include "genericMapping.h"
#include "geneticIndividual.h"
#include <BALL/common.h>

namespace BALL
{
	class MyCompare 
	{
	public:
		bool operator()(GeneticIndividual *obj1, GeneticIndividual *obj2) 
		{
			return *obj1 < *obj2;
		}
	};
	
	GeneticPool::GeneticPool():initial_population_(-1), good_(-1), 
														 population_(-1),round_(0),best_(9.99E200),
														 counter_(0),conv_iter_(0),conv_value_(0),convergence(false)
	{
	}
	
	
	GeneticPool::GeneticPool(int init, int pop, int surv,GenericMapping* gm, int conv_iter, double conv_value)
		:gm_(gm),initial_population_(init),good_(surv),population_(pop),
		 conv_iter_(conv_iter),conv_value_(conv_value)
	{
		for (int x = 0; x < initial_population_; x++)
			{
				/** creat new individual
				 */
				GeneticIndividual* individ = new GeneticIndividual;
				
				gm_->initIndivid(individ);
				
				/** insert individ
				 */
				pool_.push_back(individ);
			}
	}
	
	GeneticPool::~GeneticPool()
	{
		for (uint x = 0; x < pool_.size(); x++)
			delete pool_[x];
		
	}
	
	int GeneticPool::good()
	{
		return good_;
	}
	
	int GeneticPool::needed()
	{
		return population_ - good_;
	}
	
	void GeneticPool::initialize(int init, int pop, int surv,GenericMapping* gm,int conv_iter, double conv_value)
	{
		gm_ = gm;
		
		initial_population_= init;
		good_ = surv;
		population_ = pop; 
		
		conv_iter_ = conv_iter;
		conv_value_ = conv_value;
		
		pool_.clear();
		
		for (int x = 0; x < initial_population_; x++)
			{
				/** creat new individual
				 */
				GeneticIndividual* individ = new GeneticIndividual;
				
				gm_->initIndivid(individ);
				
				/** insert individ
				 */
				pool_.push_back(individ);
			}
	}
	
	
	GeneticPool::GeneticPool(const GeneticPool& gp):pool_(gp.pool_),initial_population_(gp.initial_population_), 
																									good_(gp.good_), population_(gp.population_),round_(gp.round_),
																									best_(gp.best_),counter_(gp.counter_),conv_iter_(gp.conv_iter_),
																									conv_value_(gp.conv_value_)
	{
	}
	
	
	GeneticPool& GeneticPool::operator=(const GeneticPool& gp)
	{
		if (this == &gp)
			return *this;
		
		pool_ = gp.pool_;
		
		best_ = gp.best_;
		counter_ = gp.counter_;
		
		initial_population_ = gp.initial_population_;
		
		good_ = gp.good_;
		
		population_ = gp.population_;
		
		conv_iter_ = gp.conv_iter_;
		conv_value_ = gp.conv_value_;
		
		return *this;
	}
	
	
	bool GeneticPool::testConvergence()
	{
		
		if (convergence)
			return true;
		
		/** calculate mean cost
		 */
		round_++;
		double med_cost = 0;
		
		for (uint x = 0; x < pool_.size(); x++)
			med_cost += pool_[x]->getFitnessValue();
		
		med_cost = med_cost / pool_.size();
		
		/** test if mean value has improved enough to satisfy covergence value
		 */
		if (med_cost < best_)
			{
				if ( med_cost < (best_ - conv_value_))
					counter_ = 0;
				else 
					counter_++;
				
				best_ = med_cost;
			}
		else 
			counter_++;
		
		/** if mean value has failed in conv_iter_ consecutiv iterations declare convergence
		 */
		if (counter_ < conv_iter_) 
			return false;
		return true;
	}
	
	
	GeneticPool* GeneticPool::clone()
	{
		return new GeneticPool(*this);
	}
	
	
	GeneticIndividual* GeneticPool::operator[](int index)
	{
		if (index < 0 || index >= size())
			{
				Log.error() << "error while trying to access pool: ";
				Log.error() << index << " as argument, size of pool is " << size() << __FILE__ << " " << __LINE__<< std::endl;
			}
		
		return pool_[index];
	}
	
	
	int GeneticPool::size()
	{
		return pool_.size();
	}
	
	
	void GeneticPool::insert(GeneticIndividual* gi)
	{
		pool_.push_back(gi);
	}
	
	
	void GeneticPool::dismiss()
	{
		std::sort(pool_.begin(),pool_.end(), MyCompare());
		
		for (uint x = good_; x < pool_.size(); x++)
			delete pool_[x];
		
		pool_.erase(pool_.begin() + good_,pool_.end());
	}
	
	
	
	void GeneticPool::setConvergence()
	{
		convergence = true;
	}
}
