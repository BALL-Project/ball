// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: HBondProcessor.h,v 1.4 2005/02/08 17:32:37 oliver Exp $
//

#ifndef BALL_STRUCTURE_HBONDPROCESSOR_H
#define BALL_STRUCTURE_HBONDPROCESSOR_H

#ifndef BALL_CONCEPT_PROCESSOR_H
#include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_KERNEL_RESIDUE_H
#include <BALL/KERNEL/residue.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#include <BALL/MATHS/vector3.h>  //TVector3
#endif

#include <vector>

namespace BALL
{

  /** Computes hydrogen bonds
   */
  class HBondProcessor 
		:	public UnaryProcessor<Composite>
  {
 
		public:

		// constants 
		// 5.2 Angstrom is the maximum distance between N and O in a hydrogen bond.
		// 4.2 Angstrom is an upper bound for the distance between N and O in the same
		// residue. Therefore, if two residues are farther apart than MAX_LENGTH, N and O
		// are farther apart than 5.2 Angstroms and there will be no hydrogen bond.
		const float MAX_LENGTH;
		const float BOND_LENGTH_N_H;
		const float BOND_LENGTH_C_O;




		struct POS
		{
			Vector3 pos_C;
			Vector3 pos_N;
			Vector3 pos_H;
			Vector3 pos_O;
			Size		number;
			Residue*	res; 
			bool is_complete; // sometimes there are defect residues around...
		};
  
		
		/**	Constructors and Descructor */
		//@{
    ///
    HBondProcessor();
    ///
		virtual ~HBondProcessor();
    ///
		HBondProcessor(const HBondProcessor& hbp);
		//@}

		/**	Processor-related methods */
		//@{
    /** Finish computes all hbonds of the composite.
     */
    virtual bool finish();
    ///
    virtual Processor::Result operator() (Composite &composite);
		//@}

		// ????
    const std::vector< std::vector<int> >& getHBondPairs() const;
		// ????
    const std::vector<POS>& getPosVec() const;

    

		protected:  
    void preComputeBonds_(ResidueIterator& data);

    Vector3 upper_;
    Vector3 lower_;
    
    // vec_ includes the atom positions per residue 
    vector<POS> vec_; // ???? !!!!!!!!!!!!!posvec_ would be more understandable -OK
											// the name vec_ is an extremely ppor choice! // -OK

    // HBondPairs_ : vector to store HBondpartners. Why ints? If anything then Position or Index ????
    std::vector<std::vector<int> > h_bond_pairs_;

  }; //class HBondProcessor

} //namesspace BALL


#endif // BALL_STRUCTURE_HBONDPROCESSOR_H
