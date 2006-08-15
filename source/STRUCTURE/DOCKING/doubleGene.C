#include "doubleGene.h"

#include <BALL/MATHS/randomNumberGenerator.h>

#include <BALL/common.h>

namespace BALL
{
  DoubleGene::DoubleGene()
  {
  }
  
  DoubleGene::~DoubleGene()
  {
  }
  
  DoubleGene::DoubleGene(uint i)
  {
    for (uint x = 0; x < i; x++)
      values_.push_back(rng_.randomDouble(0.0,1.0));
  }
  
  
  GenericGene* DoubleGene::mate(GenericGene* gg)
  {
    DoubleGene* partner = dynamic_cast<DoubleGene*>(gg);
    
    if (partner == 0)
      {
				Log.error() << "gene mismatch while mating" << __FILE__ << " " << __LINE__<< std::endl;
				exit(1);
      }
    
    if (values_.size() != partner->values_.size())
      {
				Log.error() << "vector mismatch while mating double gene" << __FILE__ << " " << __LINE__<< std::endl;
				exit(1);
      }
    
    DoubleGene* offspring = new DoubleGene;
    
    offspring->values_.resize(values_.size());
    
    for ( uint x = 0; x < values_.size(); x++) 
    {
			double v1 = values_[x];
			double v2 = partner->values_[x];
	
			if ( v1 > v2)
	  	{
	    	double c = v1;
	    	v1 = v2;
	    	v2 = c;
	  	}
	
			if (v2 - v1 > 0.5)
	  	{
	    	v1 += 1;
	    	double c = v1;
	    	v1 = v2;
	    	v2 = c;
	  	}
	
			double d = v2 - v1;
	
			/** blend two values with respect to a random number
	 			*/
			offspring->values_[x] = v1 - 0.5*d + 2*d*rng_.randomDouble(0.0,1.0);
	
			if (offspring->values_[x] < 0)
	  		offspring->values_[x] += 1;
	
			if (offspring->values_[x] > 1)
	  		offspring->values_[x] -= 1;
    }      
    
    return offspring;
  }
  
  
  void DoubleGene::mutate()
  {
    values_[rng_.randomInteger(0,values_.size()-1)] = rng_.randomDouble(0.0,1.0);
  }
  
  
	std::vector<double> DoubleGene::getValues()
  {
    return values_;
  }
  
  
  void DoubleGene::setValue(std::vector<double> v)
  {
    values_ = v;
  }
  
  GenericGene* DoubleGene::clone()
  {
    return new DoubleGene(*this);
  }
}
