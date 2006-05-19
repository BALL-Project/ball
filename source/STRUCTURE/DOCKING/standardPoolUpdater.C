#include "standardPoolUpdater.h"

#include "geneticIndividual.h"
#include "geneticPool.h"
#include "genericMapping.h"

using namespace std;

namespace BALL
{
  StandardPoolUpdater::StandardPoolUpdater()
  {
  }
  
  StandardPoolUpdater::StandardPoolUpdater(GenericMapping* gm, GeneticPool* gp)
    :GenericPoolUpdater(gm,gp)
  {
  }
  
  
  GenericPoolUpdater* StandardPoolUpdater::clone()
  {
    return new StandardPoolUpdater(*this);
  }     
  
  void StandardPoolUpdater::update(int i)
  {
    for (int x = 0; x < gp_->size(); x++)
      if ((*gp_)[x]->isAltered())
	{
	  (*gp_)[x]->setFitnessValue(gm_->calculate((*gp_)[x],i));
	  (*gp_)[x]->setAltered(false);			
	}
  }
}
