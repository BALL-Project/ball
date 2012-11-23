// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Jan Fuhrmann, Marcel Schumann $
// ----------------------------------------------------

#include <iostream>
#include <map>

#include <BALL/DOCKING/GENETICDOCK/geneticAlgorithm.h>
#include <BALL/DOCKING/GENETICDOCK/dockProblem.h>
#include <BALL/DOCKING/GENETICDOCK/genes.h>
#include <time.h>
#include <BALL/DOCKING/GENETICDOCK/geneticIndividual.h>
#include <BALL/DOCKING/GENETICDOCK/parameter.h>
#include <BALL/DOCKING/COMMON/dockingAlgorithm.h>

using namespace std;

namespace BALL
{
  GeneticAlgorithm::GeneticAlgorithm()
    :  gp_(0),
       max_iterations_(-1),
       finished_(false)
  {
  }

  GeneticAlgorithm::GeneticAlgorithm(const GeneticAlgorithm& ga)
	:  gp_(ga.gp_),
	   max_iterations_(ga.max_iterations_),
	   docking_algorithm_(ga.docking_algorithm_)
  {
  }

  GeneticAlgorithm::GeneticAlgorithm(DockingAlgorithm* docker, DockProblem* gm, int pop_number,
	int iter, int init, int pop, int surv, double mrate, int save,
	int citer, double cvalue, int cstart )
  {
    docking_algorithm_ = docker;
    setup(gm, pop_number, iter, init, pop, surv, mrate, save, citer, cvalue, cstart);
  }

  GeneticAlgorithm::~GeneticAlgorithm()
  {

  }



  GeneticAlgorithm& GeneticAlgorithm::operator = (const GeneticAlgorithm& ga)
  {
    if (this == &ga)
      return *this;

  //  this->Optimizer::operator = (ga);

    gp_ = ga.gp_;

    max_iterations_ = ga.max_iterations_;

    return *this;
  }



  void GeneticAlgorithm::start()
  {
    /** start at iteration 0
     */
    iteration_ = 0;

    /** optimize pool
     */
    while (!docking_algorithm_->wasAborted())
	{
	cout << "\r" << iteration_<<flush;

	/** calulate fitness score for each individual
	 */
	updatePool();

	/** discard individuals
	 */
	cleanUp();

	/** check if convergence has reached
	 */
	if (!multiDemeCheck())
	  {

	    double best = pools_[0][0].getFitnessValue();

	    //Size best_pool = 0;

	    for (Size x = 1; x < pools_.size(); x++)
	      if (pools_[x][0].getFitnessValue() < best)
		{
		  best = pools_[x][0].getFitnessValue();

		  //best_pool = x;
		}

	    finished_ = true;

	    break;
	  }

	/** fill up pool
	 */
	mate();

	/** mutate individuals
	 */
	mutate();
	++iteration_;

	// if started from BALLView, show best pose found so far
	if (docking_algorithm_->getDisplayMode() == DockingAlgorithm::BEST_INTERMEDIATE_POSES)
	{
		double score = calculate(&pools_[0][0]); // assign best conformation-vector to DockProblem
		gp_->move(); // reassign coordinates to molecule (because DockProblem::calculate() restores original atom positions after calculating a score)

		docking_algorithm_->setVisualizationPose(score);
		cout<<"  score = "<<score;
	}
      }
  }


//   GeneticAlgorithm* GeneticAlgorithm::clone()
//   {
//     return new GeneticAlgorithm(*this);
//   }



  void GeneticAlgorithm::setup(DockProblem* gp, int pop_number, int iter,
	int init, int pop, int surv, double mrate, int save,
	int, double, int cstart)
  {
    GenericGene::initializeRNG();

    gp_ = gp;

    /** connect problem class to genetic algorithm
     */
    gp_->connectTo(this);

    unsigned t = (unsigned) time(NULL);

    rng_.setup((t+116)%21349, (t+4382)%31582);

    max_iterations_ = iter;
    conv_start_ = cstart;
    survivors_ = surv;
    mutation_rate_ = mrate;
    immune_ = save;
    population_size_ = pop;

    vector<GeneticIndividual> tmp(init, template_individual_);

    /** initialize populations
     */
    pools_ = vector<vector<GeneticIndividual> >(pop_number, tmp);

    for (Size x = 0; x < pools_.size(); ++x)
      for (Size y = 0; y < pools_[x].size(); ++y)
	pools_[x][y].randomize();
  }



  void GeneticAlgorithm::select(Size x)
  {
    vector<GeneticIndividual>& gp = pools_[x];

    /** number of required offsprings
     */
    int needed = population_size_ - gp.size();

    /** number of available parents
     */
    int parents = gp.size();

    if (needed < 0)
      return;

    selected_.clear();

    selected_.reserve(needed);

    /** if number of parents has changed, recalculate probabilities
     */
    if (Size(parents) != probabilities_.size())
      {
	probabilities_.clear();

	for (int x = 1; x < ( parents + 1) ; x++)
	  {
	    int div = 0;

	    for (int y = 1; y < (parents + 1) ; y++)
	      div += y;

	    probabilities_.push_back(((double(parents) - double(x) + 1)/double(div) ));
	  }

	/** sum up probabilities
	 */
	for (Size x = 1; x < probabilities_.size(); x++)
	  probabilities_[x] += probabilities_[x-1];
      }

    for (Size x = 0; x < Size(needed); )
      {
	pair<GeneticIndividual*, GeneticIndividual*> gi_pair;

	double prob = rng_.randomDouble(0.0, 1.0);

	for (Size y = 0; y < probabilities_.size(); y++)
	  /** look for rank with rank < x < rank + 1
	   */
	  if ( prob < probabilities_[y])
	    {
	      gi_pair.first = &gp[y];
	      break;
	    }

	prob = rng_.randomDouble(0.0, 1.0);

	for (Size y = 0; y < probabilities_.size(); y++)
	  /** look for rank with rank < x < rank + 1
	   */
	  if ( prob < probabilities_[y])
	    {
	      gi_pair.second = &gp[y];
	      break;
	    }

	/** parents must not be the same individual
	 */
	if (gi_pair.first == gi_pair.second)
	  continue;

// 	static int scounter = 0;

	/** parents must be dissimilar
	 */
// 	if (mapping_->areSimilar(gi_pair.first,gi_pair.second))
// 	  {
// 	    if (scounter < 5)
// 	      {
// 		scounter++;
// 		continue;
// 	      }

// 	    if (gi_pair.first->getFitnessValue() < gi_pair.second->getFitnessValue())
// 	      gi_pair.second->randomize();
// 	    else
// 	      gi_pair.first->randomize();
// 	  }

// 	scounter = 0;

	selected_.push_back(gi_pair);
	++x;
      }
  }

  void GeneticAlgorithm::mate()
  {
    for (Size x = 0; x < pools_.size(); ++x)
      {
	vector<GeneticIndividual>& gp = pools_[x];

	select(x);

	/** produce offsprings
	 */
	for (Size y = 0; y < selected_.size(); ++y)
	{
           GeneticIndividual offspring;
	   gp.push_back(offspring);
	   selected_[y].first->mate(*(selected_[y].second), gp[gp.size()-1]);
	}
      }
  }

  void GeneticAlgorithm::mutate()
  {
    for (Size x = 0; x < pools_.size(); ++x)
      {
	vector<GeneticIndividual>& gp = pools_[x];

	for (int y = 0; y < int(mutation_rate_*pools_[y].size()); ++y)
	  {
	    Size index = 0;

	    /** choose random individual
	     */
	    int i = 0;
	    do
	    {
		index = rng_.randomInteger(0, gp.size()-immune_-1) + immune_;
		i++;
	    }
	    while (gp[index].isMutated() && i < 100);

	    if (!gp[index].isMutated())
	    {
		gp[index].setAltered(true);
		gp[index].setMutated(true);

		/** mutate random gene
		*/
		gp[index].getGene(rng_.randomInteger(0, gp[index].numberOfGenes()-1))->mutate();
	    }
	  }
      }
  }

  void GeneticAlgorithm::cleanUp()
  {
    /** delete individuals
     */
    for (Size x = 0; x < pools_.size(); ++x)
      {

	 cleanPool(pools_[x]);

	//vector<GeneticIndividual>& gp = pools_[x];
	//std::sort(gp.begin(), gp.end());
	//gp.erase(gp.begin() + survivors_, gp.end());
      }
  }


  void GeneticAlgorithm::cleanPool(vector<GeneticIndividual>& pool)
  {
	multimap<double, Size> slist;

	for (Size i = 0; i < pool.size(); i++)
	{
		slist.insert(make_pair(pool[i].getFitnessValue(), i));
	}

	vector<GeneticIndividual> new_pool;
	new_pool.reserve(survivors_);
	Size i = 0;
	for (multimap < double, Size > ::iterator it = slist.begin();
		i<survivors_ && it!=slist.end(); it++)
	{
		new_pool.push_back(pool[it->second]);
	}

	pool.clear(); // remove old genes (else memory leak!)
	pool = new_pool;
  }


  bool GeneticAlgorithm::multiDemeCheck()
  {

    /** stop if max number of iterations is reached
     */
    if (iteration_ == max_iterations_)
      return false;

    /** check for migration every 4th iteration starting at iteration 20
     */
    if (pools_.size() == 1 || iteration_ < 20 || iteration_ % 4 != 0)
      return true;

    /** look for best and worst pool
     */
    int best_pool = -1;
    int worst_pool = -1;
    double best_value = pools_[0][0].getFitnessValue();
    double worst_value = best_value;

    for (Size x = 0; x < pools_.size(); ++x)
      {

	double v = pools_[x][0].getFitnessValue();

	if (v < best_value)
	  {
	    best_value = v;
	    best_pool = x;
	  }

	if (v > worst_value)
	  {
	    worst_value = v;
	    worst_pool = x;
	  }
      }

    /** convergence reached if best and worst pool differ only by 0.1 or less
     */
    if (worst_value - best_value < 0.1)
      return false;

    if (best_pool == -1)
      best_pool = 0;
    if (worst_pool == -1)
      worst_pool = 0;

    /** insert individual into worst pool if pool has reach a certain level of convergence
     */
    if (fabs(pools_[worst_pool][1].getFitnessValue() - pools_[worst_pool][0].getFitnessValue()) < 1.)
      {
	/** move worst pools best individual to second position and insert best pools best individual
	 *  to first position
	 */
	pools_[worst_pool][1] = pools_[worst_pool][0];
	pools_[worst_pool][0] = pools_[best_pool][0];

	/** randomize second individual if it is judged to be to similiar to the best
	 */
// 	if (mapping_->areSimilar(&(pools_[worst_pool][0]),&(pools_[worst_pool][1])))
// 	  pools_[worst_pool][1].randomize();

	/** leave only best old and new individual
	 */
	pools_[worst_pool].erase(pools_[worst_pool].begin() + 2, pools_[worst_pool].end());
      }

    return true;
  }

  void GeneticAlgorithm::updatePool()
  {

    for (Size x = 0; x < pools_.size(); x++)
      {
	vector<GeneticIndividual>& gp = pools_[x];

	/** assign fitness value to all altered or new individuals
	 */
	for (Size y = 0; y < gp.size(); y++)
	  if (gp[y].isAltered())
	    {
	      gp[y].setFitnessValue(calculate(&gp[y]));
	      gp[y].setAltered(false);
	    }
      }
  }

  double GeneticAlgorithm::calculate(GeneticIndividual* gi)
  {
    for (Size x = 0; x < parameters_.size(); ++x)
      {
	String name = parameters_[x]->getName();

	if (name == "DoubleParameter")
	  {
	    DoubleParameter* dp = dynamic_cast<DoubleParameter*>(parameters_[x]);

	    dp->values = dynamic_cast<DoubleGene*>(gi->getGene(x))->getValues();
	  }
	else if (name == "QuaternionGene")
	  {
	    QuaternionParameter* qp = dynamic_cast<QuaternionParameter*>(parameters_[x]);

	    qp->quat = dynamic_cast<QuaternionGene*>(gi->getGene(x))->getValue();
	  }
      }

    return gp_->calculate();
  }

  GeneticIndividual* GeneticAlgorithm::getIndividual(Index)
  {
    /** does not work correctly, returns only best individual
     */

    GeneticIndividual* best = 0;

    double best_score = 99999.;

    for (Size x = 0; x < pools_.size(); ++x)
      {
	double fv = pools_[x][0].getFitnessValue();

	if (fv < best_score)
	  {
	    best_score = fv;
	    best = &(pools_[x][0]);
	  }
      }

    return best;
  }

  bool GeneticAlgorithm::registerParameter(GenericParameter* gp)
  {
    String name = gp->getName();

    parameters_.push_back(gp);

    if (name == "DoubleParameter")
      {
	DoubleParameter* dp = dynamic_cast<DoubleParameter*>(gp);

	template_individual_.insert(new DoubleGene(dp->values.size()));

	return true;
      }
    else if (name == "QuaternionParameter")
      {
	template_individual_.insert(new QuaternionGene);

	return true;
      }
    else
      {
	cerr << "GeneticAlgorithm: unkown Parameter" << endl;
	return false;
      }
  }

}
