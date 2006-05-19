#ifndef _GENERIC_POOL_UPDATER_
#define _GENERIC_POOL_UPDATER_



namespace BALL
{
  class GenericMapping;
  class GeneticPool;

  class GenericPoolUpdater 
    {
    public:
      
      GenericPoolUpdater();
      
      GenericPoolUpdater(GenericMapping* gm, GeneticPool* gp);

      virtual ~GenericPoolUpdater();

      void initialize(GenericMapping* gm, GeneticPool* gp);

      virtual GenericPoolUpdater* clone() = 0;

      virtual void update(int i) = 0;

    protected:

      GenericMapping* gm_;

      GeneticPool* gp_;

    };
}

#endif
