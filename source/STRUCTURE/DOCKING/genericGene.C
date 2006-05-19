#include "genericGene.h"
#include <time.h>
#include <BALL/MATHS/randomNumberGenerator.h>

namespace BALL
{
  
  RandomNumberGenerator GenericGene::rng_;

  void GenericGene::initializeRNG()
  {
    unsigned t = (unsigned) time(NULL);

    rng_.setup(t%31329,(t+3244)%30082);
  }
}
