
#ifndef BALL_STRUCTURE_HBONDPROCESSOR_H
#define BALL_STRUCTURE_HBONDPROCESSOR_H


#ifndef  BALL_COMMON_H
#include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_PROTEIN_H
#include <BALL/KERNEL/protein.h>
#endif

#ifndef  BALL_KERNEL_CHAIN_H
#include <BALL/KERNEL/chain.h>
#endif

#ifndef  BALL_KERNEL_BOND_H
#include <BALL/KERNEL/bond.h>
#endif

#ifndef BALL_FORMAT_PDBFFILE_H
#include <BALL/FORMAT/PDBFile.h>
#endif

#ifndef BALL_FORMAT_HINFILE_H
#include <BALL/FORMAT/HINFile.h>
#endif

#ifndef MATH_H
#include <math.h>
#endif

#ifndef BALL_STRUCTURE_RESIDUECHECKER_H
#include <BALL/STRUCTURE/residueChecker.h>
#endif

#ifndef BALL_STRUCTURE_FRAGMENTDB_H
#include <BALL/STRUCTURE/fragmentDB.h>
#endif

#ifndef BALL_MATHS_MATRIX44_H
#include <BALL/MATHS/matrix44.h> //4x4 Matrix
#endif

#ifndef BALL_MATHS_VECTOR3_H
#include <BALL/MATHS/vector3.h>  //TVector3
#endif

#ifndef BALL_MATHS_ANGLE_H
#include <BALL/MATHS/angle.h>     //TAngle
#endif

#ifndef VECTOR
#include <vector>
#endif

#ifndef BALL_DATATYPE_HASHGRID_H
#include <BALL/DATATYPE/hashGrid.h>
#endif

#ifndef BALL_MATHS_VECTOR4_H
#include <BALL/MATHS/vector4.h>
#endif

#ifndef  BALL_STRUCTURE_GEOMETRICPROPERTIES_H
#include <BALL/STRUCTURE/geometricProperties.h>
#endif

#ifndef   BALL_KERNEL_SECONDARYSTRUCTURE_H
#include <BALL/KERNEL/secondaryStructure.h>           
#endif

#ifndef  BALL_CONCEPT_PROPERTY_H
#include <BALL/CONCEPT/property.h>
#endif

namespace BALL
{
  

  
  /** This class represents prosessor
   */

  class HBondProcessor :public UnaryProcessor<Composite>
  {
 
  public:

  //constants 
  const float BOND_LENGTH_N_H;
  const float BOND_LENGTH_C_O;
  // The 5.2 Angstroms is the maximum distance between N and O in a hydrogen bond.
  // The 4.2 Angstroms is an upper bound for the distance between N and O in the same
  // residue. Therefore, if two residues are farther apart than MAX_LENGTH, N and O
  // are farther apart than 5.2 Angstroms and there will be no hydrogen bond.
  const float MAX_LENGTH;




  struct POS{
    Vector3 posC;
    Vector3 posN;
    Vector3 posH;
    Vector3 posO;
    Size number;
    Residue *pres; 

		bool is_complete; // sometimes there are defect residues around...
  };
  

     /** Default constructor
     */
     HBondProcessor();
    
    /** Detailed constructor maybe it is helpful to use other criteria for detecting hbonds
     */
    //HBondProcessor();

    /** Destructor
     */
    ~ HBondProcessor();


    /** start
     */

    /** finish computes all hbonds of the composit
     */
    bool finish();

     /** preComputeBonds postions the residues in vec
     */
    void preComputeBonds(ResidueIterator& data);

    /** operator()
     */
    Processor::Result operator() (Composite &composite);

    /**
     */
    const std::vector< std::vector<int> >& getHBondPairs() const;

    /**
     */	
    const std::vector<POS>& getPosVec() const;
    

  protected:  
    Vector3 upper_;
    Vector3 lower_;
    
  private:
    /*    // ??? Why do we need this???
    std::ostream& operator << (std::ostream& s, const POS& p)
      {
	return s << p.posC << " " << p.posN << " " << p.posH << " " << p.posO << " " << p.number << std::endl;
      }  */

    /** vec_ includes the atom positions per residue 
     */
    vector<POS> vec_; //!!!!!!!!!!!!!posvec_ would be more understandable 

    /** HBondPairs_ : vector to save HBondpartners
     */
    vector< vector<int> > HBondPairs_;

  }; //class HBondProcessor

}//namesspace BALL


#endif
