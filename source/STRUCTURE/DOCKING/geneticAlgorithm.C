// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: geneticAlgorithm.C,v 1.1.2.2 2006/05/24 17:55:11 leonhardt Exp $

#include <iostream>

#include "geneticAlgorithm.h"
#include "rankWeighting.h"
#include "dockMapping.h"
#include "standardPoolUpdater.h"
#include "geneticPool.h"
#include "geneticMutation.h"
#include "genericPoolUpdater.h"
#include "geneticMating.h"
#include "genericGene.h"

using namespace std;

namespace BALL
{
	GeneticAlgorithm::GeneticAlgorithm()
		:genetic_pool_(0),
		 selection_(0),mating_(0),
		 mutation_(0),mapping_(0),
		 gpu_ (0),
		 max_iterations_(-1),
		 finished_(false)
	{
	}
	
	
	GeneticAlgorithm::GeneticAlgorithm(const GeneticAlgorithm& ga):
		GenericEvolutionaryAlgorithm(ga),
		genetic_pool_(ga.genetic_pool_ == 0 ? 0 : new GeneticPool(*ga.genetic_pool_)),
		selection_(ga.selection_ == 0 ? 0 : ga.selection_->clone()),
		mating_(ga.mating_ == 0 ? 0 : new GeneticMating(*ga.mating_)),
		mutation_(ga.mutation_ == 0 ? 0 : new GeneticMutation(*ga.mutation_)),
		mapping_(ga.mapping_ == 0 ? 0 : ga.mapping_->clone()),
		gpu_(ga.gpu_ == 0 ? 0 : ga.gpu_->clone()),
		max_iterations_(ga.max_iterations_)
	{
	}
	
	GeneticAlgorithm::GeneticAlgorithm(GenericMapping* gm, int pop_number, int iter, int init, int pop, int surv, double mrate, int save, int citer, double cvalue, int cstart )
	{
		setup(gm, pop_number, iter, init, pop, surv, mrate, save, citer, cvalue, cstart);
	}
	
	GeneticAlgorithm::~GeneticAlgorithm()
	{
		
		delete genetic_pool_;
		delete selection_;
		delete mating_;
		delete mutation_;
		delete gpu_;
	}
	
	
  
	GeneticAlgorithm& GeneticAlgorithm::operator=(const GeneticAlgorithm& ga)
	{
		if (this == &ga)
			return *this;
		
		this->GenericEvolutionaryAlgorithm::operator=(ga);
		
		delete genetic_pool_;
		delete selection_;
		delete mating_;
		delete mutation_;
		delete mapping_;
		delete gpu_;
		
		if (ga.genetic_pool_ == 0)
			genetic_pool_ = 0;
		else
			genetic_pool_ = new GeneticPool(*ga.genetic_pool_);
		
		if (ga.selection_ == 0)
			selection_ = 0;
		else
			selection_ = ga.selection_->clone();
		
		if (ga.mating_ == 0)
			mating_ = 0;
		else
			mating_ = new GeneticMating(*ga.mating_);
		
		if (ga.mutation_ == 0)
			mutation_ = 0;
		else 
			mutation_ =  new GeneticMutation(*ga.mutation_);
		
		if (ga.mapping_ == 0)
		mapping_ = 0;
		else 
			mapping_ = ga.mapping_->clone();
		
		if (ga.gpu_ == 0)
			gpu_ = 0;
		else 
			gpu_ = ga.gpu_->clone();
		
		max_iterations_ = ga.max_iterations_;
		
		return *this;
	}
	
	
	void GeneticAlgorithm::start()
	{
		iteration_ = 0;
		
		/** optimize pool
		 */
		while(true)
			{
				/** calculate cost for each indivudal
				 */
				gpu_->update(iteration_);
				
				/** delete no longer needed individuals
				 */
				genetic_pool_->dismiss();

				mapping_->update();
				
				/** test for convergence or max number of iterations have been reached
				 */
				//if ((iteration_ < (conv_start_ - 1) ? false : genetic_pool_->testConvergence()) || iteration_ == max_iterations_)
				if (iteration_ == max_iterations_)
					{
						finished_ = true;
						break;
						
					}
				
				//std::cout << " iteration " << iteration_+1 << " of " << max_iterations_ << std::endl;
				
				
				/** select individul for mating
				 */
				selection_->select();
				
				/** fill up pool
				 */
				mating_->mate();
				
				/** mutate some indivuduals
				 */
				mutation_->mutate();
				
				iteration_ ++;
			}
	}
	
	
	GeneticAlgorithm* GeneticAlgorithm::clone()
	{
		return new GeneticAlgorithm(*this);
	}
	
	
	
	
	void GeneticAlgorithm::setup(GenericMapping* gm, int pop_number, int iter, int init, int pop, int surv, double mrate, int save, 
															 int citer, double cvalue, int cstart)
	{
		GenericGene::initializeRNG();

		mapping_ = gm;
		max_iterations_ = iter;
		conv_start_ = cstart;
		
		/** create and initialize needed classes
		 */
		genetic_pool_ = new GeneticPool(init,pop,surv,mapping_,citer,cvalue);
		selection_ = new RankWeighting(genetic_pool_);
		mating_ = new GeneticMating(genetic_pool_,selection_,mapping_);
		mutation_ = new GeneticMutation(genetic_pool_,mapping_,mrate,save);
		gpu_ = new StandardPoolUpdater(mapping_,genetic_pool_);
		
		mapping_->setPool(genetic_pool_);
	}	
}

 
