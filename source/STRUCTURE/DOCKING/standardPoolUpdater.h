#ifndef _STANDARD_POOL_UPDATER_
#define _STANDARD_POOL_UPDATER_

#include "genericPoolUpdater.h"

namespace BALL
{
  class StandardPoolUpdater: public GenericPoolUpdater
    {
    public:
      
      StandardPoolUpdater();

      StandardPoolUpdater(GenericMapping* gm, GeneticPool* gp);
      
      virtual GenericPoolUpdater* clone();
      
      virtual void update(int i);
    };
}

#endif

