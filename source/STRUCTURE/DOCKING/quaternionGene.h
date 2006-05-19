#ifndef _QUATERNION_GENE_
#define _QUATERNION_GENE_

#include "genericGene.h"
#include <BALL/MATHS/quaternion.h>


namespace BALL
{
  class QuaternionGene: public GenericGene
    {
    public:

      QuaternionGene();

      virtual ~QuaternionGene();
      
      virtual GenericGene* mate(GenericGene* gg);
      
      virtual void mutate();

      Quaternion getValue();

      virtual GenericGene* clone();

    private:

      Quaternion quat_;
      
    };
}

#endif
