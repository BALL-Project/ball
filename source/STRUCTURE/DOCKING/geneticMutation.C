#include "geneticMutation.h"
#include "geneticPool.h"
#include "genericMapping.h"
#include "geneticIndividual.h"
#include <time.h>



using namespace std;


namespace BALL
{
  GeneticMutation::GeneticMutation(GeneticPool* gp, GenericMapping* gm, double& mrate, int& immune)
    :gp_(gp),gm_(gm),mrate_(mrate),immune_(immune)
  {
    unsigned t = (unsigned) time(NULL);

    rng_.setup((t+323)%31329,(t+8382)%30082);
  }    
  
  void GeneticMutation::initialize(GeneticPool* gp, GenericMapping* gm, double& mrate, int& immune)
  {
    gp_ = gp;
    gm_ = gm;
    mrate_ = mrate;
    immune_ = immune;
  }
  
  void GeneticMutation::mutate()
  {
        
    /** mutate individuals
     */
    for ( int x = 0; x < int(mrate_*gp_->size()); x++)
      {
	GeneticIndividual* gi;
	
	/** take random individ, spare best individ
	 */
	do 
	  gi = (*gp_)[rng_.randomInteger(0,gp_->size()-immune_-1) + immune_];
	while (gi->isAltered());
	
	gi->setAltered(true);
	
	gi->getGene(rng_.randomInteger(0,gi->numberOfGenes()-1))->mutate();
      }
  }
}
