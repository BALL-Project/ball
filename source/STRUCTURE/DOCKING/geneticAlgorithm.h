#ifndef _GENETIC_ALGORITHM_
#define _GENETIC_ALGORITHM_

#include "genericEvolutionaryAlgorithm.h"


namespace BALL
{
  class GeneticPool;
  class GenericMapping;
  class SelectionStrategy;
  class GeneticMutation;
  class GenericPoolUpdater;
  class GeneticMating;
  class GeneticAlgorithm:public GenericEvolutionaryAlgorithm
    {
      friend class EvolutionaryDocking; 
	
    public:
      //CREATE(GeneticAlgorithm)
      
      /** default constructor
       */
      GeneticAlgorithm();
      
      /** copy constructor
       */
      GeneticAlgorithm(const GeneticAlgorithm& ga);
      
   
      GeneticAlgorithm(GenericMapping* gm, int pop_number, int iter, int init, int pop, int surv, double mrate, int save, int citer, double cvalue, int cstart );
      
      /** default destructor
       */
      virtual ~GeneticAlgorithm();
      
      /** assignment operator
       */
      virtual GeneticAlgorithm& operator=(const GeneticAlgorithm& ga);
      
      /** try to optimize generic pool
       */
      virtual void start();
      
      virtual GeneticAlgorithm* clone();
      
      void setup(GenericMapping* gm,int pop_number, int iter, int init, int pop, int surv, double mrate, int save, int citer, double cvalue, int cstart);
      
      
    protected:
      /** stores individuals
       */
      GeneticPool* genetic_pool_;
      
      /** selects individuals
       */
      SelectionStrategy* selection_;
      
      /** produces offsprings
       */
      GeneticMating* mating_;
      
      /** mutats individual
       */
      GeneticMutation* mutation_;
      
      /** assings cost to individuals
       */
      GenericMapping* mapping_;
      
      GenericPoolUpdater* gpu_;
      
      /** number of iterations
       */
      int max_iterations_;
    
      int iteration_;
  
      /** iteration for the first convergence test
       */
      int conv_start_;

      bool finished_;
  
    };
}

#endif
