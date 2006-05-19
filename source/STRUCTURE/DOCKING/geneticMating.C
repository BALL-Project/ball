#include "geneticMating.h"
#include "geneticIndividual.h" 
#include "selectionStrategy.h"
#include "geneticPool.h"
#include "genericMapping.h"



using namespace std;

namespace BALL
{
  GeneticMating::GeneticMating(GeneticPool* gp, SelectionStrategy* ss, GenericMapping* gm)
    :gp_(gp), ss_(ss), gm_(gm)
  {
  }
  
  GeneticMating::~GeneticMating()
  {
  }
  
  
  void GeneticMating::mate()
  {
    for (int x = 0; x < gp_->needed(); x++)
      {
	uint counter = 0;
	
	GeneticIndividual* parent1, * parent2, * offspring = 0;
	
	parent1 = ss_->getNextSelected();
	parent2 = ss_->getNextSelected();
      
	do
	  {
	    delete offspring;
	    offspring = new GeneticIndividual;
	    
	    for (uint y = 0; y < parent1->numberOfGenes(); y++)
	      offspring->insert(parent1->getGene(y)->mate(parent2->getGene(y)));
	    
	    if (counter > 3)
	      {
		parent1 = ss_->getNextSelected();
		parent2 = ss_->getNextSelected();
	      }
	    
	    if (counter > 6)
	      break;
	    
	    counter++;
	  }
	while(!gm_->verify(offspring));
	
	gp_->insert(offspring);	     
      }
  }
}
