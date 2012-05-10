// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_STRUCTURE_SECONDARYSTRUCTUREPROCESSOR_H
#define BALL_STRUCTURE_SECONDARYSTRUCTUREPROCESSOR_H

#ifndef  BALL_COMMON_H
#include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_COMPOSITE_H
# include <BALL/CONCEPT/composite.h>
#endif

#include <vector>

namespace BALL
{
	class SecondaryStructure;
	  
  /** Secondary Structure Processor.
	  
	    \brief Secondary structure extraction from 3D structure.
			
			This class implements the DSSP algorithm described in
			"Kabsch W & Sander C (1983). 
			Dictionary of protein secondary structure: 
			pattern recognition of hydrogen-bonded and geometrical features. 
			Biopolymers, 22, 2577-2637."

			When applied to a protein, it removes the instances of SecondaryStructure
			from the protein, predicts the secondary structure elements based
			(mostly) on H-bond patterns and backbone torsions and reinserts the
			appropriate secondary structure elements at the predicted positions.
			\ingroup StructureMiscellaneous
   */
  class BALL_EXPORT SecondaryStructureProcessor 
		:	public UnaryProcessor<Composite>
	{
		public:

		/**	@name	Constructors */
		//@{
		///
		SecondaryStructureProcessor();
		//@}

		/**	@name Processor-related methods */
		//@{
		///
		virtual Processor::Result operator() (Composite& composite);
		//@}

		protected:

		/// Compute the secondary structure
		void compute_();

		// helper function for finish()
		char setSecondaryStructureType_(SecondaryStructure* ss, char type);

		bool testString_(const String& s, Size offset, Size offset_2);
		bool testString2_(const String& s, Size offset);
		bool testString3_(const String& s, Size offset, char x);
			
		void insertTurn_(int turn, int position);
    void changeAllXToY_(char X, char Y, String& target);			

		// matrix to save the possible HBondPairs
		std::vector<std::vector<Position> > HBonds_;

		//vector to save the bridges
		std::vector<std::vector<int> > posbridges_; 

		String sheet_;
		String fiveturn_;
		String fourturn_;
		String threeturn_;
		String summary_;
	};

} //namespace BALL

#endif // BALL_STRUCTURE_SECONDARYSTRUCTUREPROCESSOR_H
