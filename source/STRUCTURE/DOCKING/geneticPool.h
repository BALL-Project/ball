// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: geneticPool.h,v 1.1.2.1 2006/05/19 14:18:29 leonhardt Exp $
// class implements generic pool


#ifndef _GENETIC_POOL_
#define _GENETIC_POOL_

#include <vector>


namespace BALL
{
	class GeneticIndividual;
	class GenericMapping;

  class GeneticPool
    {
    public:
      //CREATE(GeneticPool)
      
      /** default constructor
       */
      GeneticPool();
      
      /** copy constructor
       */
      GeneticPool(const GeneticPool& gp);
      
			GeneticPool(int init, int pop, int surv,GenericMapping* gm, int conv_iter, double conv_value);

			/** assignment operator
       */
      GeneticPool& operator=(const GeneticPool& gp);
      
      /** destructor
       */
      virtual ~GeneticPool();
      
      /** initializes pool with new values
       */
      void initialize(int init, int pop, int surv,GenericMapping* gm, int conv_iter, double conv_value);
      
      /** number of individuals that survive
       */
      int good();
      
      /** how many pairs are needed to fill up the pool
       */
      int needed();
      
      /** insert indidivual
       */
      void insert(GeneticIndividual* gi);
      
      /** test for convergence
       */
      bool testConvergence();
      
      
      void setConvergence();
      
      /** clone methode
       */
      virtual GeneticPool* clone();
      
      /** operator to access pool
       */
      GeneticIndividual* operator[](int index);
      
      /** number of individuals
       */
      int size();
      
      /** delete no longer needed individuals
       */
      void dismiss();
      
    private:
      /** vector for the individuals
       */
      std::vector<GeneticIndividual* >  pool_;
      
      GenericMapping* gm_;
      
      /** # of individs at the beginning
       */
      int initial_population_;
      
      /** # of surviving individs
       */
      int good_;
      
      /** # of good + bad
       */
      int population_;
      
      /** counts iterations
       */
      int round_;
      
      /** stores best mean value
       */
      double best_;
      
      /** needed for convergence
       */
      int counter_;
      
      /** number of iterations needed to declare convergence
       */
      int conv_iter_;
      
      /** value the pool should improve in each iteration
       */
      double conv_value_;
      
      bool convergence;
	};
}  


#endif
