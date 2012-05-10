// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Jan Fuhrmann, Marcel Schumann $
// ----------------------------------------------------

#ifndef BALL_DOCKING_GENETICDOCK_DOCKPROBLEM_H
#define BALL_DOCKING_GENETICDOCK_DOCKPROBLEM_H

#include <vector>
#include <string>

#include <BALL/MATHS/vector3.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/MATHS/vector3.h>

#include <BALL/DOCKING/GENETICDOCK/rotateBonds.h>
#include <BALL/DOCKING/GENETICDOCK/geneticAlgorithm.h>
#include <BALL/DOCKING/GENETICDOCK/evolutionaryDocking.h>

#include <BALL/DOCKING/GENETICDOCK/parameter.h>
#include <BALL/SCORING/COMMON/scoringFunction.h>

#include <BALL/MATHS/matrix44.h>
#include <BALL/MATHS/quaternion.h>

#ifndef BALL_DOCKING_COMMON_CONFORMATIONSET_H
# include <BALL/DOCKING/COMMON/conformationSet.h>
#endif // BALL_DOCKING_COMMON_CONFORMATIONSET_H

namespace BALL
{

  class BALL_EXPORT DockProblem
    {
    public:
      /** constructor
       */
      DockProblem();

      /** constructor
       */
      DockProblem(AtomContainer& sys_lig, BALL::ScoringFunction* sf,
		  EvolutionaryDocking* docker, bool post_dock_optimization);


      /** destructor
       */
      virtual ~DockProblem();


      virtual void setup(AtomContainer& sys_lig);


      /** calculate fitness score for one individual
       */
      virtual double calculate();

      /** move molecule according to vectors double_parameter_ and quaternion_parameter_, which are set by GeneticAlgorithm::calculate() */
      void move();

      /** true if best solution has changed
       */
      bool redraw();

      AtomContainer* getLigand();


      void connectTo(GeneticAlgorithm* opt);


    protected:

      EvolutionaryDocking* docker_;

      DoubleParameter double_parameter_;

      QuaternionParameter quaternion_parameter_;

      /** pointer to ligand molecule
       */
      AtomContainer* ligand_, *draw_ligand_;

      /** variables to store position and extentions of ligand and receptor
       */
      Vector3 t_origin_, t_extension_;

      Vector3 binding_pocket_center_;

      /**  calculate conformation score
       */
      BALL::ScoringFunction* scoring_function_;

      /** Vector to restore the position values of the ligand
       */
      std::vector<Vector3 > ligand_positions_;

      /** rotate bonds class
       */
      RotateBonds* rb_;



      /** set all rotable molecules to original positions
       */
      void restore();

      /** amber options
       */
      Options opt_;

      bool redraw_;

		bool post_dock_optimization_;

    };
}

#endif /* BALL_DOCKING_GENETICDOCK_DOCKPROBLEM_H */
