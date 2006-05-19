#include "genericPoolUpdater.h"

#include "genericMapping.h"
#include "geneticPool.h"


using namespace std;

namespace BALL
{
  GenericPoolUpdater::GenericPoolUpdater()
  {
  }
  
  GenericPoolUpdater::GenericPoolUpdater(GenericMapping* gm, GeneticPool* gp):gm_(gm),gp_(gp)
  {
  }
  
  GenericPoolUpdater::~GenericPoolUpdater()
  {
  }
  
  
  void GenericPoolUpdater::initialize(GenericMapping* gm, GeneticPool* gp)
  {
    gm_ = gm;
    gp_ = gp;
  }
}
