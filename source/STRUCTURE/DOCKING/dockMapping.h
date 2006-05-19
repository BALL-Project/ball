
#ifndef _Dock_Mapping_
#define _Dock_Mapping_

#include <vector>
#include <string>

#include <BALL/MATHS/vector3.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/MATHS/vector3.h>

#include "genericMapping.h"
#include "energyGrid.h"
#include "rotatableBonds.h"

#include <BALL/MATHS/matrix44.h>
#include <BALL/MATHS/quaternion.h>

#ifndef BALL_STRUCTURE_DOCKING_CONFORMATIONSET_H
# include <BALL/STRUCTURE/DOCKING/conformationSet.h>
#endif

namespace BALL
{
  class GeneticIndividual;
  class GeneticPool; 

  class DockMapping:public GenericMapping
    {
    public:
      /** constructor
       */
      DockMapping();
      
      DockMapping(System& sys_lig, 
		  const String& file,
		  Vector3 t_box_lower, 
		  Vector3 t_box_higer);

      
      /** copy constructor
       */
      DockMapping(const DockMapping& dm);
      
      /** destructor
       */
      virtual ~DockMapping();
      
      /**assingment operator
       */
      virtual DockMapping& operator=(const DockMapping& dm);
      
      /** initialize mapping class with receptor ligand and name of grid file
       */
      virtual void setup(System& sys_lig, 
			 const String& file,
			 Vector3 t_box_lower, 
			 Vector3 t_box_higer);

      /** clone method
       */ 
      virtual GenericMapping* clone();
      
      /** tests individual without calculating
       */
      virtual bool verify(GeneticIndividual* gi);
      
      /** calculates cost for one individual
       */
      virtual double calculate(GeneticIndividual* gi, uint i);
      
      virtual void initIndivid(GeneticIndividual* gi);

      ConformationSet getConformationSet(Index total_number);

      void setGeneticPool(GeneticPool* pool);
      
    protected:
 
      Molecule* ligand_;
      
      System system_backup_a_;
      System system_backup_b_;

      /** System to calculate energies
       */
      System sys_;
      
      /** variables to store position and extentions of ligand and receptor
       */
      Vector3 t_origin_,t_extension_;
      
      /**  calculate conformation energy
       */
      AmberFF* amber_;
      
      /** Vector to restore the position values of the ligand
       */
      std::vector<Vector3 > ligand_positions_;
      
      /** grid for energy evaluation 
       */
      EnergyGrid* eg_;
      
      /** rotate bonds according to given vector
       */
      RotateBonds* rb_;
      
      /** compares 2 molcecule positions and conformations
       *  returns true if considered different enough
       */
      bool verify(GeneticIndividual* gi1, GeneticIndividual* gi2);
      
      /** move molecules acccording to state_
       */
      void move(GeneticIndividual* gi);
      
      /** set all rotable molecules to original positions
       */
      void restore();

      Options opt_;
    };
}
  
#endif
