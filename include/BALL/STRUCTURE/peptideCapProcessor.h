// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_STRUCTURE_PEPTIDECAPPROCESSOR_H
#define BALL_STRUCTURE_PEPTIDECAPPROCESSOR_H

#ifndef  BALL_COMMON_H
# include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_COMPOSITE_H
# include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
# include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_KERNEL_RESIDUE_H
# include <BALL/KERNEL/residue.h>
#endif

#ifndef BALL_KERNEL_CHAIN_H
# include <BALL/KERNEL/chain.h>
#endif

#ifndef BALL_STRUCTURE_ATOMBIJECTION_H
# include <BALL/STRUCTURE/atomBijection.h>
#endif

namespace BALL
{

  /** Processor for adding caps to proteins
		\ingroup StructureMiscellaneous
   */

  /** \brief This processor adds ACE-N and NME-C caps to proteins.
  */

	class BALL_EXPORT PeptideCapProcessor
		: public UnaryProcessor<Chain>
	{
		public:

		/**	@name	Constructors */
		//@{
				/// Default constructor
				PeptideCapProcessor();
		//@}

		/**	@name Processor-related methods */
		//@{
				/// ()-operator
				virtual Processor::Result operator() (Chain& chain);
		//@}

		protected:

			//function to compute the distance of the cap atoms and the last residue
			float computeDistance(std::vector<Atom*>& a, std::vector<Atom*>& b);

			//function to optimize cap position by rotation
			void optimizeCapPosition(Chain& chain, bool start);
	};

} //namespace BALL

#endif // BALL_STRUCTURE_PEPTIDECAPPROCESSOR_H
