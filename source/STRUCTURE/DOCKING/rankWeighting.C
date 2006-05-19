// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: rankWeighting.C,v 1.1.2.1 2006/05/19 14:18:30 leonhardt Exp $

#include <iostream>

#include "rankWeighting.h"


using namespace std;


namespace BALL
{
	RankWeighting::RankWeighting()
	{
    unsigned t = (unsigned) time(NULL);
		
    rng_.setup((t+15434)%31329,(t+22213)%30082);

	}
	
	RankWeighting::RankWeighting(GeneticPool* gp):SelectionStrategy(gp)
	{
	}
	
	void RankWeighting::select()
	{
		last_ = 999999999;
		
		/** get number of surviving individuals
		 */
		uint good = gp_->good();
		
		if (probabilities_.size() != good)
			calculateProb(good);
	}
	
	double RankWeighting::rank(int& a, int& b)
	{
		/** calculates lower part of the formula
		 */
		int div = sumUp(b);
		
		/** calculates complete formula
		 */
		double x = (  ( double(b) - double(a) + 1)/double(div) );
		return x;
	}
	
	int RankWeighting::sumUp(int& a)
	{
		int b = 0;
		for (int i = 1; i < (a + 1) ; i++)
			b += i;
		return b;
	}
	
	
	RankWeighting* RankWeighting::clone()
	{
		return new RankWeighting(*this);
	}
	
	
	void RankWeighting::calculateProb(int i)
	{
		probabilities_.clear();
		
		for (int x = 1; x < (i+1) ; x++)
			probabilities_.push_back( rank(x, i));
    
		for (int x = 1; x < i; x++)
			probabilities_[x] += probabilities_[x-1];
	}
  
	GeneticIndividual* RankWeighting::getNextSelected()
	{
		while(true)
			{
				double prob = rng_.randomDouble(0.0,1.0);
				for (uint y = 0; y < probabilities_.size(); y++)
					/** look for rank with rank < x < rank + 1
					 */
					if ( prob < probabilities_[y])
						{
							if ( y == last_) 
								break;
							
							last_ = y;
							
							return (*gp_)[y];
						}
			}
	}
}
