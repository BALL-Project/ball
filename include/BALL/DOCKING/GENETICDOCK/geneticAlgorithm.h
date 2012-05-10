// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Jan Fuhrmann, Marcel Schumann $
// ----------------------------------------------------

#ifndef BALL_DOCKING_GENETICDOCK_GENETICALGORITHM_H
#define BALL_DOCKING_GENETICDOCK_GENETICALGORITHM_H

#include <vector>
#include <BALL/MATHS/randomNumberGenerator.h>
#include <utility>
#include "geneticIndividual.h"
#include <BALL/DOCKING/GENETICDOCK/parameter.h>

namespace BALL
{

  class DockProblem;
  class DockingAlgorithm;

  class BALL_EXPORT GeneticAlgorithm
    {

      friend class EvolutionaryDocking;

    public:
      //CREATE(GeneticAlgorithm)

      /** default constructor
       */
      GeneticAlgorithm();

      /** copy constructor
       */
      GeneticAlgorithm(const GeneticAlgorithm& ga);

      /** constructor
       */
      GeneticAlgorithm(DockingAlgorithm* docker, DockProblem*,
		       int pop_number,
		       int iter,
		       int init,
		       int pop,
		       int surv,
		       double mrate,
		       int save,
		       int citer,
		       double cvalue,
		       int cstart );

      /** default destructor
       */
      virtual ~GeneticAlgorithm();

      /** assignment operator
       */
      virtual GeneticAlgorithm& operator = (const GeneticAlgorithm& ga);

      /** clone method
       */
    //  virtual GeneticAlgorithm* clone();

      /** setup method
       */
      void setup(DockProblem*,
		 int pop_number,
		 int iter,
		 int init,
		 int pop,
		 int surv,
		 double mrate,
		 int save,
		 int citer,
		 double cvalue,
		 int cstart);

      /** optimize pool
       */
      virtual void start();

      /**
       */
      virtual double getValue() { return 0.0;};

      /** get i-th individual?!?!?
       */
      GeneticIndividual* getIndividual(Index i);

      /** register a parameter for optimization
       */
      bool registerParameter(GenericParameter*);

    protected:

      double calculate(GeneticIndividual*);

      vector<GenericParameter*> parameters_;

      /** select individuals for mating
       */
      void select(Size);

      /** produce offsprings
       */
      void mate();

      /** random alteration
       */
      void mutate();

      /** discard individuals
       */
      void cleanUp();

      /** sorts the given pool according to the fitness values of its members and keeps only the best survivors_>ones (#best ones = survivors_) */
      void cleanPool(vector<GeneticIndividual>& pool);

      /** check for migration between populations
       */
      bool multiDemeCheck();

      /** assign fitness score
       */
      void updatePool();

      /** size of one population
       */
      Size population_size_;

      /** vector of populations
       */
      std::vector<std::vector<GeneticIndividual> > pools_;

      /** vector of selected individuals
       */
      std::vector<std::pair<GeneticIndividual*, GeneticIndividual*> > selected_;

      /** probabilities for rank weighting
       */
      std::vector<double> probabilities_;

      /** assings fitness score to individuals
       */
      DockProblem* gp_;

      /** number of iterations
       */
      Size max_iterations_;

      /** number of individuals immune to mutation
       */
      Size immune_;

      /** fraction of individuals to be mutated
       */
      double mutation_rate_;

      /** count iterations during optimizaton
       */
      Size iteration_;

      /** number of individuals surviving an iteration
       */
      Size survivors_;

      /** iteration for the first convergence test
       */
      int conv_start_;

      /** true if algorithm has stopped
       */
      bool finished_;

      DockingAlgorithm* docking_algorithm_;

      /**
       */
      RandomNumberGenerator rng_;

      /** used to initialize new individuals
       */
      GeneticIndividual template_individual_;

    };
}

#endif /* BALL_DOCKING_GENETICDOCK_GENETICALGORITHM_H */
