//#include <BALL/STRUCTURE/DOCKING/geometricFit.h>
#include "evolutionaryDocking.h"
#include "geneticAlgorithm.h"
#include "dockMapping.h"
#include <iostream>

namespace BALL
{
  const String EvolutionaryDocking::Option::GRID_FILE = "grid_file";
  const String EvolutionaryDocking::Option::TRANSLATION_BOX_BOTTOM_X = "translation_box_bottom_x";
  const String EvolutionaryDocking::Option::TRANSLATION_BOX_BOTTOM_Y = "translation_box_bottom_y";
  const String EvolutionaryDocking::Option::TRANSLATION_BOX_BOTTOM_Z = "translation_box_bottom_z";
  const String EvolutionaryDocking::Option::TRANSLATION_BOX_TOP_X = "translation_box_top_x";
  const String EvolutionaryDocking::Option::TRANSLATION_BOX_TOP_Y = "translation_box_top_y";
  const String EvolutionaryDocking::Option::TRANSLATION_BOX_TOP_Z = "translation_box_top_z";
  const String EvolutionaryDocking::Option::MAX_ITERATIONS = "max_iterations";
  const String EvolutionaryDocking::Option::POPULATION_NUMBER = "population_number";
  const String EvolutionaryDocking::Option::INITIAL_POPULATION = "initial_population";
  const String EvolutionaryDocking::Option::POPULATION = "population";
  const String EvolutionaryDocking::Option::SURVIVORS = "survivors";
  const String EvolutionaryDocking::Option::MUTATION_RATE = "mutation_rate";
  const String EvolutionaryDocking::Option::MUTATION_SAVE = "mutation_save";
  const String EvolutionaryDocking::Option::CONV_ITERATIONS = "conv_iterations";
  const String EvolutionaryDocking::Option::CONV_VALUE = "conv_value";
  const String EvolutionaryDocking::Option::CONV_START = "conv_start";
  const String EvolutionaryDocking::Option::BEST_NUM = "best_num";



  const String EvolutionaryDocking::Default::GRID_FILE = "###DEFAULT###";
    const double EvolutionaryDocking::Default::TRANSLATION_BOX_BOTTOM_X = 0.0;
  const double EvolutionaryDocking::Default::TRANSLATION_BOX_BOTTOM_Y = 0.0;
  const double EvolutionaryDocking::Default::TRANSLATION_BOX_BOTTOM_Z = 0.0;
  const double EvolutionaryDocking::Default::TRANSLATION_BOX_TOP_X = 0.0;
  const double EvolutionaryDocking::Default::TRANSLATION_BOX_TOP_Y = 0.0;
  const double EvolutionaryDocking::Default::TRANSLATION_BOX_TOP_Z = 0.0;
  const int EvolutionaryDocking::Default::MAX_ITERATIONS = 400;
  const int EvolutionaryDocking::Default::POPULATION_NUMBER = 5;
  const int EvolutionaryDocking::Default::INITIAL_POPULATION = 1000;
  const int EvolutionaryDocking::Default::POPULATION = 60;
  const int EvolutionaryDocking::Default::SURVIVORS = 30;
  const double EvolutionaryDocking::Default::MUTATION_RATE = 0.05;
  const int EvolutionaryDocking::Default::MUTATION_SAVE = 1;
  const int EvolutionaryDocking::Default::CONV_ITERATIONS = 5;
  const double EvolutionaryDocking::Default::CONV_VALUE = 0.1;
  const int EvolutionaryDocking::Default::CONV_START = 20;
  const int EvolutionaryDocking::Default::BEST_NUM  = 10;
  

  EvolutionaryDocking::EvolutionaryDocking()
    throw()
    :ga_(0),
     dm_(0)
  {
    options.setDefault(Option::GRID_FILE,Default::GRID_FILE);  
    options.setDefaultReal(Option::TRANSLATION_BOX_BOTTOM_X,Default::TRANSLATION_BOX_BOTTOM_X);  
    options.setDefaultReal(Option::TRANSLATION_BOX_BOTTOM_Y,Default::TRANSLATION_BOX_BOTTOM_Y);  
    options.setDefaultReal(Option::TRANSLATION_BOX_BOTTOM_Z,Default::TRANSLATION_BOX_BOTTOM_Z);  
    options.setDefaultReal(Option::TRANSLATION_BOX_TOP_X,Default::TRANSLATION_BOX_TOP_X);  
    options.setDefaultReal(Option::TRANSLATION_BOX_TOP_Y,Default::TRANSLATION_BOX_TOP_Y);  
    options.setDefaultReal(Option::TRANSLATION_BOX_TOP_Z,Default::TRANSLATION_BOX_TOP_Z);  
    options.setDefaultInteger(Option::MAX_ITERATIONS,Default::MAX_ITERATIONS);  
    options.setDefaultInteger(Option::POPULATION_NUMBER,Default::POPULATION_NUMBER);  
    options.setDefaultInteger(Option::INITIAL_POPULATION,Default::INITIAL_POPULATION);  
    options.setDefaultInteger(Option::POPULATION,Default::POPULATION);  
    options.setDefaultInteger(Option::SURVIVORS,Default::SURVIVORS);  
    options.setDefaultReal(Option::MUTATION_RATE,Default::MUTATION_RATE);  
    options.setDefaultInteger(Option::MUTATION_SAVE,Default::MUTATION_SAVE);  
    options.setDefaultInteger(Option::CONV_ITERATIONS,Default::CONV_ITERATIONS);  
    options.setDefaultReal(Option::CONV_VALUE,Default::CONV_VALUE);  
    options.setDefaultInteger(Option::CONV_START,Default::CONV_START);  
    options.setDefaultInteger(Option::BEST_NUM,Default::BEST_NUM);  
}
  
  EvolutionaryDocking::EvolutionaryDocking(System &system1, System &system2)
    throw()
    :ga_(0),
    dm_(0)
  {
    options.setDefault(Option::GRID_FILE,Default::GRID_FILE);  
    options.setDefaultReal(Option::TRANSLATION_BOX_BOTTOM_X,Default::TRANSLATION_BOX_BOTTOM_X);  
    options.setDefaultReal(Option::TRANSLATION_BOX_BOTTOM_Y,Default::TRANSLATION_BOX_BOTTOM_Y);  
    options.setDefaultReal(Option::TRANSLATION_BOX_BOTTOM_Z,Default::TRANSLATION_BOX_BOTTOM_Z);  
    options.setDefaultReal(Option::TRANSLATION_BOX_TOP_X,Default::TRANSLATION_BOX_TOP_X);  
    options.setDefaultReal(Option::TRANSLATION_BOX_TOP_Y,Default::TRANSLATION_BOX_TOP_Y);  
    options.setDefaultReal(Option::TRANSLATION_BOX_TOP_Z,Default::TRANSLATION_BOX_TOP_Z);  
    options.setDefaultInteger(Option::MAX_ITERATIONS,Default::MAX_ITERATIONS);  
    options.setDefaultInteger(Option::POPULATION_NUMBER,Default::POPULATION_NUMBER);  
    options.setDefaultInteger(Option::INITIAL_POPULATION,Default::INITIAL_POPULATION);  
    options.setDefaultInteger(Option::POPULATION,Default::POPULATION);  
    options.setDefaultInteger(Option::SURVIVORS,Default::SURVIVORS);  
    options.setDefaultReal(Option::MUTATION_RATE,Default::MUTATION_RATE);  
    options.setDefaultInteger(Option::MUTATION_SAVE,Default::MUTATION_SAVE);  
    options.setDefaultInteger(Option::CONV_ITERATIONS,Default::CONV_ITERATIONS);  
    options.setDefaultReal(Option::CONV_VALUE,Default::CONV_VALUE);  
    options.setDefaultInteger(Option::CONV_START,Default::CONV_START);  
    options.setDefaultInteger(Option::BEST_NUM,Default::BEST_NUM);  
  
    setup(system1, system2);
  }

  EvolutionaryDocking::EvolutionaryDocking(Options& new_options)
    throw()
    :ga_(0),
     dm_(0)
  {
    options=new_options;
  }

  EvolutionaryDocking::EvolutionaryDocking(System &system1,System &system2 ,Options& new_options)
    throw()
    :ga_(0),
     dm_(0)
  {
    setup(system1, system2, new_options);
  }

  EvolutionaryDocking::~EvolutionaryDocking()
    throw()
  {
    destroy_();
  }

  void EvolutionaryDocking::setup(System& system1, System& system2, Options& new_options)
    throw()
  {
    options  = new_options;
  
    setup(system1,system2);
  }

  void EvolutionaryDocking::setup(System& system1, System& system2)
    throw()
  {
    if (system1.countAtoms() < system2.countAtoms())
      {
	system1_ = system1;
	system2_ = system2;
      }
    else
      {
	system1_ = system2;
	system2_ = system1;
      }

    delete dm_;

    dm_ = new DockMapping(system2_,
			  options.get(Option::GRID_FILE), 
			  Vector3(options.getReal(Option::TRANSLATION_BOX_BOTTOM_X), 
				  options.getReal(Option::TRANSLATION_BOX_BOTTOM_Y),
				  options.getReal(Option::TRANSLATION_BOX_BOTTOM_Z)),
			  Vector3(options.getReal(Option::TRANSLATION_BOX_TOP_X), 
				  options.getReal(Option::TRANSLATION_BOX_TOP_Y),
				  options.getReal(Option::TRANSLATION_BOX_TOP_Z)));
    
  }
  
  void EvolutionaryDocking::start()
    throw()
  {
    
    DockingAlgorithm::start();
    
    if (dm_ == 0)
      {
	std::cerr << "no mapping class!" << std::endl;
	exit(0);
      }
    
    delete ga_;

    ga_ = new GeneticAlgorithm(dm_,
			       options.getInteger(Option::POPULATION_NUMBER),
			       options.getInteger(Option::MAX_ITERATIONS),
			       options.getInteger(Option::INITIAL_POPULATION),
			       options.getInteger(Option::POPULATION),
			       options.getInteger(Option::SURVIVORS),
			       options.getReal(Option::MUTATION_RATE),
			       options.getInteger(Option::MUTATION_SAVE),
			       options.getInteger(Option::CONV_ITERATIONS),
			       options.getReal(Option::CONV_VALUE),
			       options.getInteger(Option::CONV_START));
     
    ga_->start();
  }

      
  float EvolutionaryDocking::getProgress() const
    throw()
  {
    if (ga_->iteration_ == 0) 
      return 0.0;
    
    return float(ga_->iteration_/ga_->max_iterations_);
  }
  
 
  bool EvolutionaryDocking::hasFinished() const 
    throw()
  {
    return (ga_->finished_);
  }


  ConformationSet EvolutionaryDocking::getConformationSet(Index total_number)
    throw()
  {

    // first see how many conformations we should generate
    if ( (total_number == 0) || (total_number > options.getInteger(Option::BEST_NUM)) )
      total_number = options.getInteger(Option::BEST_NUM);
    
    return dm_->getConformationSet(total_number);
  }
  
  
  void EvolutionaryDocking::destroy_()
    throw()
  {
    delete ga_;
    delete dm_;
  }
  		  
  
}


  
