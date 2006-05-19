#ifndef _SELECTING_STRATEGY_
#define _SELECTING_STRATEGY_



namespace BALL
{
  class GeneticIndividual;
  class GeneticPool; 
  
  class SelectionStrategy
    {
    public:
      
      SelectionStrategy();
      
      virtual ~SelectionStrategy();
      
      SelectionStrategy(GeneticPool* gp);
      
      /** selects individuals
       */
      virtual void select() = 0;
      
      /** clone methode
       */
      virtual SelectionStrategy* clone() = 0;
      
      /** initialize methode
       */
      virtual void initialize(GeneticPool* gp);
      
      /** get next selected individual for mating
       */
      virtual GeneticIndividual* getNextSelected()=0;
      
    protected:
      
      /** pool containing individuals
       */
      GeneticPool* gp_;
    };
}
#endif
