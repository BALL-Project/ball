#include "geneticIndividual.h"
#include <BALL/common.h>

namespace BALL
{
  GeneticIndividual::GeneticIndividual()
    :fitness_value_(0), altered_(true)
  {
  }
  
  GeneticIndividual::GeneticIndividual(const GeneticIndividual& gi)
    :fitness_value_(gi.fitness_value_),altered_(gi.altered_)
  {
    for (uint x = 0; x < gi.genes_.size(); x++)
      genes_.push_back(gi.genes_[x]->clone());
    
  }      
  
  GeneticIndividual& GeneticIndividual::operator=(const GeneticIndividual& gi)
  {
    if (this == &gi)
      return *this;
    
    fitness_value_ = gi.fitness_value_;
    altered_ = gi.altered_;
    
    genes_.clear();
    
    for (uint x = 0; x < gi.genes_.size(); x++)
      genes_.push_back(gi.genes_[x]->clone());
    
    return *this;
    
  }      
  
  GeneticIndividual::~GeneticIndividual()
  {
    for (uint x = 0; x < genes_.size(); x++)
      delete genes_[x];
  }
  
  
  bool GeneticIndividual::operator<(const GeneticIndividual& gi)
  {
    return fitness_value_ < gi.fitness_value_;
  }
  
  void GeneticIndividual::setFitnessValue(const double& d)
  {
    fitness_value_ = d;
  }     
  
  double GeneticIndividual::getFitnessValue()
  {
    return fitness_value_;
  }      
  
  void GeneticIndividual::setAltered(bool b)
  {
    altered_ = b;
  }      
  
  bool GeneticIndividual::isAltered()
  {
    return altered_;
  }      
  
  uint GeneticIndividual::numberOfGenes()
  {
    return genes_.size();
  }      
  
  GenericGene* GeneticIndividual::getGene(const uint& i)
  {
    
    if (i < genes_.size())
      return genes_[i];
    
    Log.error() << "access out of range" << __FILE__ << " " << __LINE__<< std::endl;
    exit(1);
  }
  
  void GeneticIndividual::insert(GenericGene* gg)
  {
    genes_.push_back(gg);
  }
}
