#ifndef _GENETIC_INDIVIDUAL_
#define _GENETIC_INDIVIDUAL_

#include <vector>

#include "genericGene.h"

namespace BALL
{
  class GeneticIndividual
    {
    public:
      
      GeneticIndividual();
      
      GeneticIndividual(const GeneticIndividual& gi);
      
      GeneticIndividual& operator=(const GeneticIndividual& gi);
      
      ~GeneticIndividual();
      
      bool operator<(const GeneticIndividual& gi);
      
      void setFitnessValue(const double& d);
      
      double getFitnessValue();
      
      void setAltered(bool b);
      
      bool isAltered();
      
      uint numberOfGenes();
      
      GenericGene* getGene(const uint& i);

      void insert(GenericGene* gg);
      
    private:
      
      double fitness_value_;
      
      bool altered_;
      
      std::vector<GenericGene*> genes_;
      
    };
}

#endif
