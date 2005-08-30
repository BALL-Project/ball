// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: secondaryStructureProcessor.h,v 1.8.2.3 2005/08/13 15:58:25 amoll Exp $
//

#ifndef BALL_SECONDARY_STRUCTURE_PROCESSOR_H
#define BALL_SECONDARY_STRUCTURE_PROCESSOR_H

#ifndef  BALL_COMMON_H
#include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_COMPOSITE_H
# include <BALL/CONCEPT/composite.h>
#endif

#include <vector>

namespace BALL
{
  
  /** Secondary structure extraction from 3D structure.
			This class implements the STRIDE algorithm (Argos & Frishman).
			When applied to a protein, it removes the instances of SecondaryStructure
			from the protein, predicts the secondary structure elements based
			(mostly) on H-bond patterns and backbone torsions and reinserts the
			appropriate secondary structure elements at the predicted positions.
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

} //namesspace BALL

#endif // BALL_STRUCTURE_SECONDARYSTRUCTUREPROCESSOR_H
