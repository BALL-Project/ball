#ifndef _GENETIC_MUTATION_
#define _GENETIC_MUTATION_

#include <BALL/MATHS/randomNumberGenerator.h>

namespace BALL
{
  class GeneticPool;
  class GenericMapping;
  
  class GeneticMutation
    {
    public:
      
      GeneticMutation(GeneticPool* gp, GenericMapping* gm, double& mrate, int& immune);
    
      void initialize(GeneticPool* gp, GenericMapping* gm, double& mrate, int& immune);

      void mutate();

    private:
      /** pool to be mutated
       */
      GeneticPool* gp_;
      
      GenericMapping* gm_;
      
      /** mutation rate
       */
      double mrate_;
      
      /** number of individuals immune to mutation
       */ 
      int immune_;
      
      RandomNumberGenerator rng_;
      
    };
}

#endif
