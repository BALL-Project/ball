#include "selectionStrategy.h"
#include "geneticPool.h"
#include "geneticIndividual.h"

using namespace std;

namespace BALL
{
  SelectionStrategy::SelectionStrategy()
  {
  }
  
  SelectionStrategy::~SelectionStrategy()
  {
  }
  
  SelectionStrategy::SelectionStrategy(GeneticPool* gp)
    :gp_(gp)
  {
  }
  
  
  void SelectionStrategy::initialize(GeneticPool* gp)
  {
    gp_ = gp;
  }
}
