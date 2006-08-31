#ifndef BALL_STRUCTURE_DOCKING_EVOLUTIONARYDOCKING_H
#define BALL_STRUCTURE_DOCKING_EVOLUTIONARYDOCKING_H

#ifndef BALL_DATATYPE_REGULARDATA3D_H
#include <BALL/DATATYPE/regularData3D.h>
#endif

#ifndef BALL_KERNEL_SYSTEM_H
#include <BALL/KERNEL/system.h>
#endif

#ifndef BALL_STRUCTURE_DOCKING_DOCKINGALGORITHM_H
#include <BALL/STRUCTURE/DOCKING/dockingAlgorithm.h>
#endif

#ifndef BALL_MOLMEC_COMMON_FORCEFIELD_H
# include <BALL/MOLMEC/COMMON/forceField.h>
#endif


namespace BALL
{
  class GeneticAlgorithm;
		
  class DockMapping;


  /**
     class EvolutionaryDocking
     This class is derived from class DockingAlgorithm.
     small ligand docking algorithm via a genetic Algorithm.
  */
  class EvolutionaryDocking : public DockingAlgorithm
    {
    public:
      
      BALL_CREATE(EvolutionaryDocking)

      typedef struct Option
			{
				static const String GRID_FILE;
				static const String TRANSLATION_BOX_BOTTOM_X;
				static const String TRANSLATION_BOX_BOTTOM_Y;
				static const String TRANSLATION_BOX_BOTTOM_Z;
				static const String TRANSLATION_BOX_TOP_X;
				static const String TRANSLATION_BOX_TOP_Y;
				static const String TRANSLATION_BOX_TOP_Z;
				static const String MAX_ITERATIONS;
				static const String POPULATION_NUMBER;
				static const String INITIAL_POPULATION;
				static const String POPULATION;
				static const String SURVIVORS;
				static const String MUTATION_RATE;
				static const String MUTATION_SAVE;
				static const String CONV_ITERATIONS;
				static const String CONV_VALUE;
				static const String CONV_START;
			};

      typedef struct Default
			{
				static const String GRID_FILE;
				static const double TRANSLATION_BOX_BOTTOM_X;
				static const double TRANSLATION_BOX_BOTTOM_Y;
				static const double TRANSLATION_BOX_BOTTOM_Z;
				static const double TRANSLATION_BOX_TOP_X;
				static const double TRANSLATION_BOX_TOP_Y;
				static const double TRANSLATION_BOX_TOP_Z;
				static const int MAX_ITERATIONS;
				static const int POPULATION_NUMBER;
				static const int INITIAL_POPULATION;
				static const int POPULATION;
				static const int SURVIVORS;
				static const double MUTATION_RATE;
				static const int MUTATION_SAVE;
				static const int CONV_ITERATIONS;
				static const double CONV_VALUE;
				static const int CONV_START;
			};

      EvolutionaryDocking()
	throw();

      EvolutionaryDocking(System &system1, System &system2)
	throw();

      EvolutionaryDocking(Options& new_options)
	throw();

      EvolutionaryDocking(System &system1,System &system2 ,Options& new_options, ForceField* ff)
	throw();

      ~EvolutionaryDocking()
	throw();

      /** The setup routines.
       */
      virtual void setup(System& system1, System& system2, Options& new_options, ForceField* ff)
	throw();
      
      virtual void setup(System& system1, System& system2)
				throw();	
     
      /** Start the main loop of the algorithm.
       */
      void start()
				throw();
      
      // return the overall docking progress as a percentage
      float getDockingProgress() const
				throw();
      
      // 
      bool hasFinished() const
				throw();

			bool systemChanged() const
				throw(); 

      /** Return the translation corresponding to conformation con_num.
       */
/*       Vector3 getTranslation(Index con_num) const */
/* 	throw(); */
      
      /** Return the orientation corresponding to conformation con_num.
       */
/*       Vector3 getOrientation(Index con_num) const */
/* 	throw() */
      
      /** Return the ranked conformations.
       */
      ConformationSet getConformationSet(Index total_number = 0)
				throw();
      
      //////////////////////////////////////////
      // the member variables

    protected:
      
      virtual const System& getIntermediateResult_()
	throw();

      // Free all allocated memory and destroys the options and results
      void destroy_()
	throw();

      GeneticAlgorithm* ga_;
		
      DockMapping* dm_;

			/**	The force field */
      ForceField* ff_;

    };
}

#endif
