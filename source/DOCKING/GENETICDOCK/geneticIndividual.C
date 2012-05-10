// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Jan Fuhrmann, Marcel Schumann $
// ----------------------------------------------------

#include <BALL/DOCKING/GENETICDOCK/geneticIndividual.h>

#include <iostream>

using namespace std;

namespace BALL
{
  GeneticIndividual::GeneticIndividual()
    :fitness_value_(0), altered_(true)
  {
  }

  GeneticIndividual::GeneticIndividual(const GeneticIndividual& gi)
    :fitness_value_(gi.fitness_value_), altered_(gi.altered_)
  {
    for (Size x = 0; x < gi.genes_.size(); x++)
      genes_.push_back(gi.genes_[x]->clone());

  }

  GeneticIndividual& GeneticIndividual::operator = (const GeneticIndividual& gi)
  {
    if (this == &gi)
      return *this;

    fitness_value_ = gi.fitness_value_;
    altered_ = gi.altered_;

    genes_.clear();

    for (Size x = 0; x < gi.genes_.size(); x++)
      genes_.push_back(gi.genes_[x]->clone());

    return *this;

  }

  GeneticIndividual::~GeneticIndividual()
  {
    for (Size x = 0; x < genes_.size(); x++)
      delete genes_[x];
  }


  bool GeneticIndividual::operator<(const GeneticIndividual& gi) const
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

  void GeneticIndividual::setMutated(bool b)
  {
    mutated_ = b;
  }

  bool GeneticIndividual::isMutated()
  {
    return mutated_;
  }

  Size GeneticIndividual::numberOfGenes()
  {
    return genes_.size();
  }

  GenericGene* GeneticIndividual::getGene(const Size& i)
  {

    if (i < genes_.size())
      return genes_[i];

    cerr << "access out of range" << endl;
    exit(1);
  }

  void GeneticIndividual::insert(GenericGene* gg)
  {
    genes_.push_back(gg);
  }

  void GeneticIndividual::mate(GeneticIndividual& parent, GeneticIndividual& offspring)
  {
    /** mate all genes
     */
    for (Size x = 0; x < numberOfGenes(); x++)
      offspring.insert(getGene(x)->mate(parent.getGene(x)));
  }

  void GeneticIndividual::randomize()
  {
    /** randomize all genes
     */
    for (Size x = 0; x < numberOfGenes(); x++)
      getGene(x)->randomize();
  }
}
