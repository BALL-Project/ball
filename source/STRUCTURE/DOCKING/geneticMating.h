#ifndef _GENETIC_MATING_
#define _GENETIC_MATING_


namespace BALL
{
  class SelectionStrategy;
  class GeneticPool;
  class GenericMapping;

  class GeneticMating
    {
    public:

      GeneticMating(GeneticPool* gp, SelectionStrategy* ss, GenericMapping* gm);

      virtual ~GeneticMating();
      
      void initialize(GeneticPool* gp, SelectionStrategy* ss, GenericMapping* gm);
      
      // creats offsprings
      virtual void mate();
      
    private:
      // new individuals are inserted here
      GeneticPool* gp_;
      
      // needed to get individuals
      SelectionStrategy* ss_;
      
      // needed to verify produced individuals
      GenericMapping* gm_;
    };
}


#endif
