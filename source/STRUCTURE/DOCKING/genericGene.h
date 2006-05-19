
#ifndef _GENERIC_GENE_
#define _GENERIC_GENE_

namespace BALL
{
  class RandomNumberGenerator;

  class GenericGene
    {
    public:
      
      virtual ~GenericGene(){};

      virtual GenericGene* mate(GenericGene* gg) = 0;

      virtual void mutate() = 0;
 
      virtual GenericGene* clone() = 0;

      static void initializeRNG();

    protected:
      
      static RandomNumberGenerator rng_;
   
    };
}

#endif
