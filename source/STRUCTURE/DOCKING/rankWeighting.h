#ifndef _RANK_WEIGHTING_
#define _RANK_WEIGHTING_

#include <vector>

#include "selectionStrategy.h"
#include "geneticPool.h"
#include <BALL/MATHS/randomNumberGenerator.h>


namespace BALL
{
  class GeneticIndividual;  
 
  class RankWeighting:public SelectionStrategy
    {
    public:
      RankWeighting();
      
      RankWeighting(GeneticPool* gp);

      /** select individuals
       */
      virtual void select();
      
      /** clone methode
       */
      virtual RankWeighting* clone();
      
      /** returns next selected indidivudal
       */
      virtual GeneticIndividual* getNextSelected();
      
    private:
      /** calculates rank for a position
       */
      double rank(int&, int&);
      
      /** calculates x + (x-1) + (x-2) + ... + 1
       */
      int sumUp(int&);
      
      /** calculates probabilties for i indidivudals
       */
      void calculateProb(int i);
      
      /** probabilities of each rank to be selected
       */
      std::vector<double> probabilities_;
      
      uint last_;
      
      RandomNumberGenerator rng_;
    };
}

#endif
