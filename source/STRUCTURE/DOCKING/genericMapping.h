#ifndef _GENERIC_MAPPING_
#define _GENERIC_MAPPING_

#include <vector>

namespace BALL
{
  class GeneticPool;
  class GeneticIndividual;

  class GenericMapping
    {
    public:
      //CREATE(GenericMapping)
      
      /** constructor
       */
      GenericMapping();
      
      /** copy constructor
       */
      GenericMapping(const GenericMapping& gm);
      
      /**assignment operator
       */
      virtual GenericMapping& operator=(const GenericMapping& gm);
      
      /** default destructor
       */
      virtual ~GenericMapping();
      
      /** calculates the cost for a genotype 
       */
      virtual double calculate(GeneticIndividual* gi, uint i) = 0;

      virtual void update() = 0;
      
      /** pool instance needed
       */
      void setPool(GeneticPool* gp);
      
      /** checks individual
       */
      virtual bool verify(GeneticIndividual* gi) = 0;
      
      /** clone methode
       */
      virtual GenericMapping* clone() = 0;
      
      virtual void initIndivid(GeneticIndividual* gi) = 0;	
      
    protected:
      /** pointer to generic pool
       */
      GeneticPool* gp_;
      
    };
}
#endif
