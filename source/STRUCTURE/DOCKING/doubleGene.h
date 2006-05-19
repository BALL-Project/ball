#ifndef _DOUBLE_GENE_
#define _DOUBLE_GENE_

#include <vector>
#include "genericGene.h"

namespace BALL
{
  class DoubleGene: public GenericGene
    {
    public:

      DoubleGene();

      virtual ~DoubleGene();

      DoubleGene(uint i);

      virtual GenericGene* mate(GenericGene* gg);

      virtual void mutate();

      std::vector<double> getValues();

      void setValue(std::vector<double> v);

      virtual GenericGene* clone();

    private:

      std::vector<double> values_;
    };
}

#endif
